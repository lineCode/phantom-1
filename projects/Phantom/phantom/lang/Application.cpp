// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Application.h"

#include "Module.h"
#include "PackageFolder.h"
#include "Parameter.h"
#include "Source.hxx"

#include <phantom/detail/core.h>
#include <phantom/utils/Path.h>
#ifndef __DOXYGEN__
#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#        include "windows.h"

#        include <TlHelp32.h>
#        include <winternl.h>
#    elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX &&                                   \
    PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
#        include <dlfcn.h>
#    endif
#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_MACOSX
#        include <mach-o/dyld.h>
#        include <mach-o/getsect.h>
#    endif
#endif
#include "ConstVolatileType.h"
#include "CppSymbolParser.h"
#include "LValueReference.h"
#include "Package.h"
#include "Plugin.h"
#include "Pointer.h"
#include "RValueReference.h"
#include "SourceFile.h"
#include "TemplateSpecialization.h"
#include "phantom/detail/ModuleRegistrationInfo.h"
#include "phantom/detail/core_internal.h"
#include "phantom/utils/Placement.h"

#include <fstream>
#include <phantom/utils/SmallSet.h>
#include <phantom/utils/StringUtil.h>

#pragma warning(disable : 4996)
/* *********************************************** */
namespace phantom
{
namespace detail
{
extern RawPlacement<lang::Application> g_pApplication;
}
namespace lang
{
PHANTOM_DEFINE_META_CLASS(Source);

String Application_normalizePathString(StringView a_strPath)
{
    Path p(a_strPath);
    return p.genericString();
}

String Application_makeWindowsPathString(StringView a_strPath)
{
    String result = a_strPath;
    for (size_t i = 0; i < result.size(); ++i)
    {
        if (result[i] == '/')
            result[i] = '\\';
    }
    return result;
}

Application* Application::Get()
{
    return phantom::detail::g_pApplication;
}

Application::Application()
    : Symbol("", PHANTOM_R_NONE, PHANTOM_R_ALWAYS_VALID | PHANTOM_R_FLAG_NATIVE),
      m_pMainModule(nullptr),
      m_OperationCounter(1) /// operation counter initialized to 1 to be able to handle C++ dyanmic
                            /// Initializer and auto loaded dlls before user make manual operations,
                            /// phantoms reset it to 0 on 'main' start.
      ,
      m_pRootPackageFolder(nullptr),
      m_pNullptr(nullptr)
{
}

Application::~Application() {}

void Application::_createNativeModule(ModuleRegistrationInfo* info)
{
    // #endif
    {
        PHANTOM_ASSERT(getModule(info->m_Name) == nullptr, "module '%s' already exists", info->m_Name);
        info->setModule(PHANTOM_DEFERRED_NEW(Module)(info->m_ModuleHandle, info->m_Name, info->m_BinaryFileName,
                                                     info->m_Source, info->m_uiFlags | PHANTOM_R_FLAG_NATIVE));
        PHANTOM_ASSERT(m_OperationCounter,
                       "DLL loader must be responsible for loading phantom modules, don't use "
                       "platform specific function to load them such as LoadLibrary/FreeLibrary, "
                       "use Application::loadLibrary/unloadlibrary");
        _addModule(info->m_pModule);
        info->m_pModule->setOnLoadFunc(info->m_OnLoad);
        info->m_pModule->setOnUnloadFunc(info->m_OnUnload);
        if (!Plugin::HasLoadingInProgress())
        {
            if (!phantom::detail::installed()) // phantom is not installed yet, this module is a
                                               // startup module
            {
                m_StartupModules.push_back(info->m_pModule);
            }
            else
            {
                PHANTOM_LOG(Error,
                            "a module has been loaded manually after startup using "
                            "LoadLibrary/FreeLibrary/dlopen function instead of using the plugin "
                            "system. This can lead to undefined behavior");
            }
        }
    }
}

void Application::_uninstallNativeModule(Module* a_pModule)
{
    PHANTOM_ASSERT(a_pModule->isNative());
    PHANTOM_ASSERT(a_pModule == getMainModule() || !getMainModule() || Plugin::HasUnloadingInProgress());
    // application is still alive, means we are either unloading main, either unloading manually a
    // plugin, either quitting application

    auto startupFound = std::find(m_StartupModules.begin(), m_StartupModules.end(), a_pModule);
    if (startupFound != m_StartupModules.end())
    {
        PHANTOM_ASSERT(!Plugin::HasLoadingInProgress());
        PHANTOM_ASSERT(!Plugin::HasUnloadingInProgress());
        m_StartupModules.erase(startupFound);
    }
    if (!Plugin::HasLoadingInProgress(a_pModule))
        _moduleAboutToBeRemoved(a_pModule);
    if (a_pModule->getOnUnloadFunc())
        a_pModule->getOnUnloadFunc()();
    if (a_pModule->getName() == "Phantom")
    {
        auto* pModuleClass = PHANTOM_CLASSOF(Module);
        pModuleClass->unregisterInstance(pModuleClass);
        phantom::Constructor<Module>::destroy(a_pModule);
        phantom::Allocator<Module>::deallocate(a_pModule);
    }
    else
    {
        PHANTOM_DELETE(Module) a_pModule;
    }
}

void Application::terminate()
{
    PHANTOM_ASSERT(m_OperationCounter == 1);
    setFlag(PHANTOM_R_FLAG_TERMINATED);
    while (m_Modules.size() > 1)
    {
        Module* pMod = m_Modules.back();
        if (pMod->isNative())
            _uninstallNativeModule(pMod);
        else
            PHANTOM_DELETE_DYN pMod;
    }
    PHANTOM_ASSERT(m_Modules.back()->getName() == "Phantom");
    LanguageElements elements = getElements();
    if (elements.size())
    {
        for (size_t i = 0; i < elements.size(); ++i)
        {
            LanguageElement* pElem = elements[i];
            if (pElem != m_Modules.back()) // != "Phantom" module
                PHANTOM_DELETE_DYN pElem;
        }
        PHANTOM_ASSERT(getElements().size() == 1); // "Phantom" module
    }

    _uninstallNativeModule(m_Modules.back());

    PHANTOM_DELETE_DYN Class::MetaClass();

    Symbol::terminate();
}

static Application::CppExpressionParser s_CppExpressionParser;

void Application::setCppExpressionParser(CppExpressionParser a_Parser)
{
    s_CppExpressionParser = a_Parser;
}

Application::CppExpressionParser Application::getCppExpressionParser()
{
    return s_CppExpressionParser;
}

Expression* Application::cppExpression(StringView a_strExp, LanguageElement* a_pScope) const
{
    if (!s_CppExpressionParser.empty())
    {
        return s_CppExpressionParser(a_strExp, a_pScope);
    }
    return nullptr;
}

#if !defined(PHANTOM_MODULE_SUFFIX)

#    if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#        if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X64
#            define compiler_arch "-x64"
#        else
#            define compiler_arch "-x86"
#        endif

#        define plugin_file_suffix                                                                                     \
            "-d-"                                                                                                      \
            "v140" compiler_arch plugin_ext
#    else
#        define plugin_file_suffix plugin_ext
#    endif

#else
#    if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#        define plugin_file_suffix "-d" PHANTOM_PP_QUOTE(PHANTOM_MODULE_SUFFIX) plugin_ext
#    else
#        define plugin_file_suffix plugin_ext
#    endif

#endif

void Application::_prefetchPlugins(StringView a_strPath)
{
    DirectoryEntries entries;
    Path::ListDirectoryEntries(a_strPath, entries);
    for (DirectoryEntry& entry : entries)
    {
        if (entry.isRegularFile())
        {
            if (entry.path().extension() == ".plugin")
            {
                String name = entry.path().stem();
                if (Plugin* pPlugin = getPlugin(name))
                {
                    PHANTOM_LOG(Warning,
                                "plugin with name '%.*s' in '%.*s' was already loaded from another "
                                "path ('%.*s')",
                                PHANTOM_STRING_AS_PRINTF_ARG(name),
                                PHANTOM_STRING_AS_PRINTF_ARG(entry.path().genericString()),
                                PHANTOM_STRING_AS_PRINTF_ARG(pPlugin->getFilePath()));
                }
                else
                {
                    addPlugin(PHANTOM_NEW(Plugin)(entry.path().genericString()));
                }
            }
        }
    }
}
void Application::_prefetchPlugins()
{
    for (auto path : getPluginPaths())
    {
        _prefetchPlugins(path);
    }
}

Symbol* Application::findCppSymbol(StringView a_Text, LanguageElement* a_pScope,
                                   StringBuffer* a_pLastError /*= nullptr*/)
{
    CppSymbolParser parser;
    if (a_pScope == nullptr)
        a_pScope = Namespace::Global();
    return parser.parse(a_Text, a_pScope, a_pLastError);
}

Symbol* Application::findCppSymbol(StringView a_Text, StringBuffer* a_pLastError /*= nullptr*/)
{
    return findCppSymbol(a_Text, Namespace::Global(), a_pLastError);
}

bool Application::findCppSymbols(StringView a_Text, Symbols& a_Symbols, LanguageElement* a_pScope /*= nullptr*/,
                                 StringBuffer* a_pLastError /*= nullptr*/)
{
    CppSymbolParser parser;
    PHANTOM_ASSERT(a_pScope);
    return parser.parse(a_Text, a_Symbols, a_pScope, a_pLastError);
}

bool Application::findCppSymbols(StringView a_Text, Symbols& a_Symbols, StringBuffer* a_pLastError /*= nullptr*/)
{
    return findCppSymbols(a_Text, a_Symbols, Namespace::Global(), a_pLastError);
}

Type* Application::findCppType(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError /*= nullptr*/)
{
    if (Symbol* pSym = findCppSymbol(a_Text, a_pScope, a_pLastError))
        return pSym->toType();
    return nullptr;
}

Type* Application::findCppType(StringView a_Text, StringBuffer* a_pLastError /*= nullptr*/)
{
    return findCppType(a_Text, Namespace::Global(), a_pLastError);
}

Template* Application::findCppTemplate(StringView a_Text, LanguageElement* a_pScope,
                                       StringBuffer* a_pLastError /*= nullptr*/)
{
    if (Symbol* pSym = findCppSymbol(a_Text, a_pScope, a_pLastError))
        return pSym->asTemplate();
    return nullptr;
}

Template* Application::findCppTemplate(StringView a_Text, StringBuffer* a_pLastError /*= nullptr*/)
{
    return findCppTemplate(a_Text, Namespace::Global(), a_pLastError);
}

Class* Application::findCppClass(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError /*= nullptr*/)
{
    if (Type* pType = findCppType(a_Text, a_pScope, a_pLastError))
        return pType->asClass();
    return nullptr;
}

Class* Application::findCppClass(StringView a_Text, StringBuffer* a_pLastError /*= nullptr*/)
{
    return findCppClass(a_Text, Namespace::Global(), a_pLastError);
}

void Application::_loadMain(size_t a_MainHandle, StringView a_strModuleName, StringView a_strFileName,
                            StringView a_strSourceFile, uint a_uiFlags)
{
    // PHANTOM_ASSERT_ON_MAIN_THREAD();
    PHANTOM_ASSERT(m_OperationCounter == 1);
    detail::registerModule(a_MainHandle, a_strModuleName, a_strFileName, a_strSourceFile, a_uiFlags, {}, nullptr,
                           nullptr);
    detail::pushInstallation();
    detail::installModules();
    detail::popInstallation();
    m_pMainModule = getModule(a_strModuleName);
    PHANTOM_ASSERT(m_pMainModule);
    m_OperationCounter--;
}

static int Module_GetNativeRefCount(Module* a_pModule);

void Application::_unloadMain()
{
    // PHANTOM_ASSERT_ON_MAIN_THREAD();

    // release modules content
    while (m_Modules.size())
    {
        Module* pModule = m_Modules.back();
        if (pModule->getPlugin())
        // if module is attached to a plugin, we unload it
        {
            pModule->getPlugin()->unload();
            if (m_Modules.size() && m_Modules.back() == pModule)
            {
                if (Module_GetNativeRefCount(pModule))
                {
                    PHANTOM_LOG(Error, "module '%.*s' dll is still loaded somewhere, probably inside zombie threads",
                                PHANTOM_STRING_AS_PRINTF_ARG(pModule->getName()));
                }
                m_Modules.pop_back();
            }
        }
        else if (pModule == m_pMainModule)
        {
            m_OperationCounter++;
            _uninstallNativeModule(m_pMainModule);
            m_pMainModule = nullptr;
            m_OperationCounter--;
            break;
        }
        else if (NOT(pModule->isNative()))
        // if the module has been manually created, we destroy it to ensure its content is cleared
        {
            PHANTOM_DELETE(Module) pModule;
        }
    }
}

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
String FormatLastError()
{
    DWORD  dw = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

    String clampedMessage = (char*)lpMsgBuf;
    clampedMessage = clampedMessage.substr(0, clampedMessage.find_first_of("\r\n"));
    LocalFree(lpMsgBuf);
    return clampedMessage;
}

#    pragma warning(disable : 4459)

int Module_GetNativeRefCount(Module* a_pModule)
{
    HINSTANCE hinstLib;
    typedef NTSTATUS (*MYPROC)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
    MYPROC NtQueryInformationProcess;

    // Get a handle to the DLL module.
    hinstLib = LoadLibrary(TEXT("ntdll.dll"));

    // If the handle is valid, try to get the function address.
    if (hinstLib == NULL)
        return -1;

    NtQueryInformationProcess = (MYPROC)GetProcAddress(hinstLib, "NtQueryInformationProcess");

    int                       loadCount = 0;
    PROCESS_BASIC_INFORMATION pbi;
    ZeroMemory(&pbi, sizeof(pbi));
    auto hprocess = GetCurrentProcess();
    NtQueryInformationProcess(hprocess, ProcessBasicInformation, &pbi, sizeof(pbi), nullptr);
    size_t dwSize = 0;
    PEB    peb;
    if (FALSE == ReadProcessMemory(GetCurrentProcess(), pbi.PebBaseAddress, &peb, sizeof(peb), &dwSize))
    {
        PHANTOM_LOG(Error, "%.*s", PHANTOM_STRING_AS_PRINTF_ARG(FormatLastError()));
    }
    typedef struct _PEB_LDR_DATA
    {
        ULONG      Length;
        BOOLEAN    Initialized;
        PVOID      SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;

    } PEB_LDR_DATA, *PPEB_LDR_DATA;

    PEB_LDR_DATA peb_ldr_data;
    PHANTOM_VERIFY(ReadProcessMemory(hprocess, peb.Ldr, &peb_ldr_data, sizeof(peb_ldr_data), &dwSize));
    typedef struct _LDR_MODULE
    {
        LIST_ENTRY     InLoadOrderModuleList;
        LIST_ENTRY     InMemoryOrderModuleList;
        LIST_ENTRY     InInitializationOrderModuleList;
        PVOID          BaseAddress;
        PVOID          EntryPoint;
        ULONG          SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG          Flags;
        SHORT          LoadCount;
        SHORT          TlsIndex;
        LIST_ENTRY     HashTableEntry;
        ULONG          TimeDateStamp;

    } LDR_MODULE, *PLDR_MODULE;

    LDR_MODULE peb_ldr_module;
    void*      readAddr = (void*)peb_ldr_data.InLoadOrderModuleList.Flink;

    // Go through each modules one by one in their load order.
    while (ReadProcessMemory(hprocess, readAddr, &peb_ldr_module, sizeof(peb_ldr_module), &dwSize))
    {
        // Get the reference count of the DLL
        loadCount = (signed short)peb_ldr_module.LoadCount;
        if (size_t(peb_ldr_module.BaseAddress) == a_pModule->getHandle())
        {
            return loadCount;
        }

        readAddr = (void*)peb_ldr_module.InLoadOrderModuleList.Flink;
    }
    return -1;
}
#else
int Module_GetNativeRefCount(Module* a_pModule)
{
    return -1;
}
#endif
#pragma warning(default : 4459)

void Application::addModule(Module* a_pModule)
{
    _addModule(a_pModule);
    PHANTOM_EMIT moduleAdded(a_pModule);
}

void Application::_addModule(Module* a_pModule)
{
    PHANTOM_ASSERT(getModule(a_pModule->getName()) == nullptr, "module with same name already loaded");
#if !defined(PHANTOM_STATIC_LIB_HANDLE)
    PHANTOM_ASSERT(m_Modules.size() OR a_pModule->getName() == "Phantom", "phantom must be the first loaded module");
#endif
    m_Modules.push_back(a_pModule);
    if (a_pModule->isNative())
    {
        m_NativeModules.push_back(a_pModule);
    }
    addElement(a_pModule);
}

void Application::removeModule(Module* a_pModule)
{
    _removeModule(a_pModule);
}

void Application::_removeModule(Module* a_pModule)
{
    removeElement(a_pModule);
}

void Application::onElementRemoved(LanguageElement* a_pElement)
{
    if (Module* pModule = a_pElement->asModule())
    {
        PHANTOM_EMIT moduleAboutToBeRemoved(pModule);
        m_Modules.erase(std::find(m_Modules.begin(), m_Modules.end(),
                                  static_cast<Module*>(pModule))); // Remove dependencies reference of this module
    }
    else if (a_pElement->asNamespace())
    {
        PHANTOM_ASSERT(a_pElement->isInvalid() OR Namespace::Global() == a_pElement);
    }
}

void Application::_registerBuiltInTypes()
{
    /// Install pre defined and convenient types
    phantom::lang::BuiltInTypes::Register();
    Module* pPhantomModule = m_Modules.front();
    PHANTOM_ASSERT(pPhantomModule->getName() == "Phantom");
    for (auto pType : m_BuiltInTypes)
        pPhantomModule->addElement(pType);
    Alias* pUnsignedAlias = PHANTOM_DEFERRED_NEW(Alias)(PHANTOM_TYPEOF(unsigned), "unsigned", 0, PHANTOM_R_FLAG_NATIVE);
    pPhantomModule->addSymbol(pUnsignedAlias);
    Namespace::Global()->addReferencedElement(pUnsignedAlias);

    Namespace* pGlobal = Namespace::Global();
    Namespace* pPhantom = pGlobal->findOrCreateNamespace("phantom");

#define _PHNTM_FUND_TD(t) pPhantomModule->addElement(pGlobal->addAlias(PHANTOM_TYPEOF(t), #t, 0, PHANTOM_R_FLAG_NATIVE))

#define _PHNTM_FUND_TD_PHNTM(t)                                                                                        \
    pPhantomModule->addElement(pPhantom->addAlias(PHANTOM_TYPEOF(t), #t, 0, PHANTOM_R_FLAG_NATIVE))

    // #if defined(_M_IA64) || defined(_M_X64) || defined(_M_AMD64)
    //     _PHNTM_FUND_TD(int128);
    // #endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
    _PHNTM_FUND_TD(__int8);
    _PHNTM_FUND_TD(__int16);
    _PHNTM_FUND_TD(__int32);
    _PHNTM_FUND_TD(__int64);
#endif

    _PHNTM_FUND_TD(int8_t);
    _PHNTM_FUND_TD(int16_t);
    _PHNTM_FUND_TD(int32_t);
    _PHNTM_FUND_TD(int64_t);

#if PHANTOM_HAS_BUILT_IN_WCHAR_T == 0
    _PHNTM_FUND_TD(wchar_t);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T == 0
    _PHNTM_FUND_TD(char16_t);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T == 0
    _PHNTM_FUND_TD(char32_t);
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
    /// workaround for clang va_list built-in type
    pPhantomModule->addElement(pGlobal->addAlias(PHANTOM_TYPEOF(char), "__va_list_tag", 0, PHANTOM_R_FLAG_NATIVE));
#endif
    _PHNTM_FUND_TD(size_t);

    _PHNTM_FUND_TD_PHNTM(float32);
    _PHNTM_FUND_TD_PHNTM(float64);
    _PHNTM_FUND_TD_PHNTM(int8);
    _PHNTM_FUND_TD_PHNTM(int16);
    _PHNTM_FUND_TD_PHNTM(int32);
    _PHNTM_FUND_TD_PHNTM(int64);
    _PHNTM_FUND_TD_PHNTM(uint8);
    _PHNTM_FUND_TD_PHNTM(uint16);
    _PHNTM_FUND_TD_PHNTM(uint32);
    _PHNTM_FUND_TD_PHNTM(uint64);
    _PHNTM_FUND_TD_PHNTM(sint8);
    _PHNTM_FUND_TD_PHNTM(sint16);
    _PHNTM_FUND_TD_PHNTM(sint32);
    _PHNTM_FUND_TD_PHNTM(sint64);
    _PHNTM_FUND_TD_PHNTM(longlong);
    _PHNTM_FUND_TD_PHNTM(uchar);
    _PHNTM_FUND_TD_PHNTM(ushort);
    _PHNTM_FUND_TD_PHNTM(uint);
    _PHNTM_FUND_TD_PHNTM(ulong);
    _PHNTM_FUND_TD_PHNTM(ulonglong);
    _PHNTM_FUND_TD_PHNTM(schar);
    _PHNTM_FUND_TD_PHNTM(sshort);
    _PHNTM_FUND_TD_PHNTM(sint);
    _PHNTM_FUND_TD_PHNTM(slong);
    _PHNTM_FUND_TD_PHNTM(slonglong);
    _PHNTM_FUND_TD_PHNTM(wchar);
    _PHNTM_FUND_TD_PHNTM(character);
    _PHNTM_FUND_TD_PHNTM(longdouble);
    _PHNTM_FUND_TD_PHNTM(size_t);
    _PHNTM_FUND_TD_PHNTM(byte);

#undef _PHNTM_FUND_TD
}

Module* Application::getMainModule() const
{
    return m_pMainModule;
}

Modules Application::getNativeModules() const
{
    Modules m;
    for (auto pMod : m_Modules)
        if (pMod->isNative())
            m.push_back(pMod);
    return m;
}

Constant* Application::getNullptr() const
{
    if (m_pNullptr == nullptr)
    {
        m_pNullptr = Constant::Create<std::nullptr_t>(nullptr, "nullptr");
        const_cast<Application*>(this)->addSymbol(m_pNullptr);
    }
    return m_pNullptr;
}

Type* Application::getAuto() const
{
    return getBuiltInType("auto");
}

void Application::_addBuiltInType(Type* a_pType)
{
    PHANTOM_ASSERT(getBuiltInType(a_pType->getDecoratedName()) == nullptr);
    m_BuiltInTypes.push_back(a_pType);
}

void Application::_removeBuiltInType(Type*) {}

Type* Application::getBuiltInType(StringView a_strDecoratedName) const
{
    StringBuffer buf;
    for (auto pType : m_BuiltInTypes)
    {
        pType->getDecoratedName(buf);
        if (buf.size() == a_strDecoratedName.size() && memcmp(buf.data(), a_strDecoratedName.data(), buf.size()) == 0)
            return pType;
        buf.clear();
    }
    return nullptr;
}

#define BREAK_INSTRUCTION_SIZE 1
#define CALL_INSTRUCTION_SIZE 5

Module* Application::getSharedLibraryModule(StringView a_strPath) const
{
    for (auto pModule : m_Modules)
    {
        if (Path::Equivalent(pModule->getLibraryFullName(), a_strPath))
            return pModule;
    }
    return nullptr;
}

Module* Application::nativeModuleFromStackContext(const byte* ebp, uint stackLevel, byte* epc /*= 0*/)
{
    PHANTOM_ASSERT(epc OR stackLevel > 0);
    while (stackLevel--)
    {
        // down the stack
        byte* up_epc = *((byte**)(ebp + sizeof(void*))) - CALL_INSTRUCTION_SIZE;
        byte* up_ebp = *((byte**)ebp);
        epc = up_epc;
        ebp = up_ebp;
    }
    return nativeModuleFromProgramCounter(epc);
}

Module* Application::nativeModuleFromProgramCounter(const byte* epc)
{
    Module*   pBestMatch = nullptr;
    ptrdiff_t distanceFromModuleStart = PTRDIFF_MAX;
    Modules   nativeModules = getNativeModules();
    for (auto pMod : nativeModules)
    {
        byte* pMemoryStart = pMod->getMemoryStart();
        if (pMemoryStart AND(epc > pMemoryStart)) // current instruction pointer is after current
                                                  // module start => current module is candidate
        {
            if ((epc - pMemoryStart) < distanceFromModuleStart)
            {
                // distance from memory start if less than current best module => current module
                // become best candidate
                pBestMatch = pMod;
                distanceFromModuleStart = (epc - pMemoryStart);
            }
        }
    }
    return pBestMatch;
}

LanguageElement* Application::invalid(LanguageElement* a_pElement)
{
    addElement(a_pElement);
    return a_pElement;
}

bool Application::setDefaultPluginPath(StringView a_strPath)
{
    if (a_strPath.empty())
    {
        PHANTOM_LOG(Error, "default module path is mandatory, you cannot set an empty default module path");
        return false;
    }
    if (!hasPluginPath(a_strPath) AND !addPluginPath(a_strPath))
    {
        return false;
    }
    m_DefaultPluginPath = a_strPath;
    return true;
}

bool Application::setDefaultExportPath(StringView a_strPath)
{
    if (a_strPath.empty())
        return false;
    if (!Path::Exists(a_strPath))
    {
        std::error_code ec;
        if (!Path::CreateDirectories(a_strPath, ec) AND ec.value())
        {
            PHANTOM_LOG(Error, "cannot create export path '%s' [system error: %s]",
                        (const char*)a_strPath.nullTerminated(), (const char*)ec.message().c_str());
            return false;
        }
    }
    if (Path::Equivalent(Path::Absolute(m_DefaultExportPath), Path::Absolute(a_strPath)))
        return true;

    /// first module path to search is the export path
    if (m_DefaultExportPath.size())
    {
        PHANTOM_ASSERT(m_PluginPaths.size());
        PHANTOM_ASSERT(Path::Equivalent(Path::Absolute(m_PluginPaths.front()), Path::Absolute(a_strPath)));
        m_PluginPaths[0] = a_strPath;
    }
    else
    {
        m_PluginPaths.insert(m_PluginPaths.begin(), a_strPath);
    }
    m_DefaultExportPath = a_strPath;
    return true;
}

bool Application::addPluginPath(StringView a_strPath)
{
    if (hasPluginPath(a_strPath))
        return true;
    if (!Path::Exists(a_strPath))
    {
        std::error_code ec;
        if (!Path::CreateDirectories(a_strPath, ec) AND ec.value())
        {
            PHANTOM_WARNING(false, "cannot create plugin path '%.*s' [system error: %.*s]",
                            PHANTOM_STRING_AS_PRINTF_ARG(a_strPath), PHANTOM_STRING_AS_PRINTF_ARG(ec.message()));
            return false;
        }
    }
    m_PluginPaths.push_back(a_strPath);
    _prefetchPlugins(a_strPath);
    PHANTOM_EMIT pluginPathAdded(a_strPath);
    return true;
}

bool Application::removePluginPath(StringView a_strPath)
{
    for (auto it = m_PluginPaths.begin(); it != m_PluginPaths.end(); ++it)
    {
        if (Path::Equivalent(Path::Absolute(*it), Path::Absolute(a_strPath)))
        {
            String path = *it;
            m_PluginPaths.erase(it);
            PHANTOM_EMIT pluginPathRemoved(path);
            return true;
        }
    }
    return false;
}

bool Application::addBinaryPath(StringView a_strPath)
{
    if (hasBinaryPath(a_strPath))
        return true;
    if (!Path::Exists(a_strPath))
    {
        std::error_code ec;
        if (!Path::CreateDirectories(a_strPath, ec) AND ec.value())
        {
            PHANTOM_WARNING(false, "cannot create binary path '%.*s' [system error: %.*s]",
                            PHANTOM_STRING_AS_PRINTF_ARG(a_strPath), PHANTOM_STRING_AS_PRINTF_ARG(ec.message()));
            return false;
        }
    }
    if (m_BinaryPaths.empty())
    {
        m_DefaultBinaryPath = a_strPath;
    }
    m_BinaryPaths.push_back(a_strPath);
    PHANTOM_EMIT binaryPathAdded(a_strPath);
    return true;
}

bool Application::removeBinaryPath(StringView a_strPath)
{
    for (auto it = m_BinaryPaths.begin(); it != m_BinaryPaths.end(); ++it)
    {
        if (Path::Equivalent(Path::Absolute(*it), Path::Absolute(a_strPath)))
        {
            String path = *it;
            m_BinaryPaths.erase(it);
            PHANTOM_EMIT binaryPathRemoved(path);
            return true;
        }
    }
    return false;
}

bool Application::hasBinaryPath(StringView a_strPath) const
{
    for (auto it = m_BinaryPaths.begin(); it != m_BinaryPaths.end(); ++it)
    {
        if (Path(a_strPath).absolute() == Path(*it).absolute())
            return true;
    }
    return false;
}

void Application::getSources(Sources& a_Sources) const
{
    for (auto pModule : m_Modules)
    {
        pModule->getSources(a_Sources);
    }
}

StringView Application::getDefaultSourcePath() const
{
    static String null_string;
    return m_SourcePaths.empty() ? null_string : m_SourcePaths.front();
}

bool Application::setDefaultSourcePath(StringView a_strPath)
{
    String path = Path::Absolute(a_strPath).canonical().genericString();
    auto   it = std::find(m_SourcePaths.begin(), m_SourcePaths.end(), path);
    if (it == m_SourcePaths.end())
    {
        if (NOT(addSourcePath(a_strPath)))
            return false;
        String front = m_SourcePaths.front();
        m_SourcePaths.front() = m_SourcePaths.back();
        m_SourcePaths.back() = front;
    }
    else
    {
        String front = m_SourcePaths.front();
        m_SourcePaths.front() = *it;
        *it = front;
    }
    return true;
}

bool Application::addSourcePath(StringView a_strPath)
{
    if (!Path::Exists(a_strPath))
    {
        std::error_code ec;
        if (!Path::CreateDirectories(a_strPath, ec) AND ec.value())
        {
            PHANTOM_WARNING(false,
                            "cannot create source path '%.*s' [system error: %s], keep previously "
                            "defined source repository path",
                            PHANTOM_STRING_AS_PRINTF_ARG(a_strPath), (const char*)ec.message().c_str());
            return false;
        }
    }
    if (!Path::IsDirectory(a_strPath))
    {
        PHANTOM_LOG(Error, "source path '%.*s' is not a directory", PHANTOM_STRING_AS_PRINTF_ARG(a_strPath));
        return false;
    }
    String path = Path::Absolute(a_strPath).canonical().genericString();
    if (std::find(m_SourcePaths.begin(), m_SourcePaths.end(), path) == m_SourcePaths.end())
    {
        m_SourcePaths.push_back(path);
        return true;
    }
    return false;
}

bool Application::removeSourcePath(StringView a_strPath)
{
    String path = Path::Absolute(a_strPath).canonical().genericString();
    auto   found = std::find(m_SourcePaths.begin(), m_SourcePaths.end(), path);
    if (found != m_SourcePaths.end())
    {
        m_SourcePaths.erase(found);
        return true;
    }
    return false;
}

Module* Application::getModule(StringView a_strName) const
{
    for (auto it = m_Modules.begin(); it != m_Modules.end(); ++it)
    {
        if ((*it)->getName() == a_strName)
        {
            return *it;
        }
    }
    return nullptr;
}

Module* Application::getModuleByHandle(size_t a_ModuleHandle) const
{
    for (auto it = m_Modules.begin(); it != m_Modules.end(); ++it)
    {
        if ((*it)->getHandle() == a_ModuleHandle)
        {
            return *it;
        }
    }
    return nullptr;
}

Plugin* Application::getPlugin(StringView a_strName) const
{
    for (auto& pPlugin : m_Plugins)
    {
        if (pPlugin->getName() == a_strName)
            return pPlugin;
    }
    return nullptr;
}

void Application::addPlugin(Plugin* a_pPlugin)
{
    m_Plugins.push_back(a_pPlugin);
    PHANTOM_EMIT pluginAdded(a_pPlugin);
}

void Application::removePlugin(Plugin* a_pPlugin)
{
    PHANTOM_EMIT pluginAboutToBeRemoved(a_pPlugin);
    m_Plugins.erase(std::find(m_Plugins.begin(), m_Plugins.end(), a_pPlugin));
}

void Application::getUniqueName(StringBuffer&) const {}

PackageFolder* Application::rootPackageFolder() const
{
    if (m_pRootPackageFolder == nullptr)
    {
        const_cast<Application*>(this)->m_pRootPackageFolder = PHANTOM_DEFERRED_NEW(PackageFolder);
        const_cast<Application*>(this)->addElement(m_pRootPackageFolder);
    }
    return m_pRootPackageFolder;
}

Source* Application::getSource(StringView a_strUniqueName) const
{
    size_t sourceNamePos = a_strUniqueName.find_last_of(".");
    if (sourceNamePos == String::npos)
    {
        PHANTOM_WARNING(false,
                        "missing source name in '%.*s', only package provided; reminder source "
                        "unique name must be of the form 'package.source'",
                        PHANTOM_STRING_AS_PRINTF_ARG(a_strUniqueName));
        return nullptr;
    }
    StringView     packageName = a_strUniqueName.substr(0, sourceNamePos);
    PackageFolder* pPackageFolder = getPackageFolder(packageName);
    if (pPackageFolder == nullptr)
    {
        return nullptr;
    }
    String sourceName = a_strUniqueName.substr(sourceNamePos + 1);
    return pPackageFolder->getSource(sourceName);
}

bool Application::hasPluginPath(StringView a_strPath) const
{
    for (auto it = m_PluginPaths.begin(); it != m_PluginPaths.end(); ++it)
    {
        if (Path::Equivalent(Path::Absolute(*it), Path::Absolute(a_strPath)))
            return true;
    }
    return false;
}

bool Application::setDefaultBinaryPath(StringView a_strPath)
{
    m_DefaultBinaryPath = a_strPath;
    return true;
}

String Application::findPluginPath(StringView a_strName) const
{
    for (auto it = m_PluginPaths.begin(); it != m_PluginPaths.end(); ++it)
    {
        if (Path::IsRegularFile(*it + "/" + a_strName + ".plugin"))
            return *it;
    }
    return "";
}

void Application::findPlugins(StringView a_strPath, Strings& a_PluginNames) const
{
    DirectoryEntries entries;
    Path::ListDirectoryEntries(a_strPath, entries, false);
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        if (!it->isDirectory())
        {
            if (it->path().extension() == ".plugin")
            {
                a_PluginNames.push_back(it->path().stem());
            }
        }
    }
}

PackageFolder* Application::getPackageFolder(StringView a_strUniqueName) const
{
    return m_pRootPackageFolder->getPackageFolder(a_strUniqueName);
}

PackageFolder* Application::getOrCreatePackageFolder(StringView a_strUniqueName)
{
    return m_pRootPackageFolder->getOrCreatePackageFolder(a_strUniqueName);
}

Type* Application::findType(hash64 a_Hash) const
{
    for (auto pModule : m_Modules)
    {
        if (Type* pType = pModule->findType(a_Hash))
            return pType;
    }
    return nullptr;
}

void Application::_moduleAdded(Module* a_pModule)
{
    PHANTOM_EMIT moduleAdded(a_pModule);
}

void Application::_moduleDiscarded(Module* a_pModule)
{
    PHANTOM_EMIT moduleDiscarded(a_pModule);
}

// TODO : rename "nativeModuleAboutToBeRemoved"

void Application::_moduleAboutToBeRemoved(Module* a_pModule)
{
    PHANTOM_EMIT moduleAboutToBeRemoved(a_pModule);
}

Undefineds* Application::undefineds(StringView a_TypeName)
{
    return m_Undefineds[a_TypeName];
}

Undefineds* Application::getUndefineds(StringView a_TypeName) const
{
    auto found = m_Undefineds.find(a_TypeName);
    return found == m_Undefineds.end() ? nullptr : found->second;
}

bool Application::_findSymbols(StringView a_strUniqueName, Symbols& a_OutSymbols) const
{
    String symbolString;
    int    templateSignature = 0;
    for (size_t i = 0; i < a_strUniqueName.size(); ++i)
    {
        char c = a_strUniqueName[i];
        if (c == '<')
        {
            templateSignature++;
        }
        else if (c == '>')
        {
            templateSignature--;
        }
        else if (c == ',' AND templateSignature == 0)
        {
            Symbol* pSymbol = findSymbol(symbolString);
            if (pSymbol == nullptr)
            {
                Expression* pExp = cppExpression(symbolString, Namespace::Global());
                if (pExp)
                {
                    Symbol* pUS = reinterpret_cast<LanguageElement*>(pExp)->removeExpressionAsSymbol();
                    if (pUS AND pUS->asConstant())
                    {
                        pSymbol = static_cast<Constant*>(pUS)->clone();
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            a_OutSymbols.push_back(pSymbol);
            symbolString.clear();
            continue;
        }
        symbolString.append(c);
    }
    Symbol* pSymbol = findSymbol(symbolString);
    if (pSymbol == nullptr)
    {
        Expression* pExp = cppExpression(symbolString, Namespace::Global());
        if (pExp)
        {
            Symbol* pUS = reinterpret_cast<LanguageElement*>(pExp)->removeExpressionAsSymbol();
            if (pUS AND pUS->asConstant())
            {
                pSymbol = static_cast<Constant*>(pUS)->clone();
            }
            else
                return false;
        }
        else
            return false;
    }
    a_OutSymbols.push_back(pSymbol);
    return true;
}

static void Application_clearClonedConstants(const Symbols& a_Symbols)
{
    for (auto it = a_Symbols.begin(); it != a_Symbols.end(); ++it)
    {
        Symbol* pSym = *it;
        if (pSym AND pSym->asConstant() AND pSym->getOwner() == nullptr)
            PHANTOM_DELETE_DYN pSym;
    }
}

Symbol* Application::_findSymbol(const Strings& words, const Types* a_pFunctionSignature, Modifiers a_RefQualifiers,
                                 LanguageElement* a_pScope /*= nullptr*/) const
{
    if (words.size() == 1 AND words.front().front() == '@')
    {
        for (auto it = m_BuiltInTypes.begin(); it != m_BuiltInTypes.end(); ++it)
        {
            if (words.back() == (*it)->getUniqueName())
                return *it;
        }
        return nullptr;
    }
    String           ext;
    PackageFolder*   pFolder = rootPackageFolder();
    LanguageElement* pScope = a_pScope;
    for (size_t i = 0; i < words.size(); ++i)
    {
        String name = words[i];
        if (name.empty())
            return nullptr;
        if (pScope)
        {
            Symbols templateSignature;
            String  templateSignatureString;
            String  dotTemplatedName;
            size_t  templatePos = name.find_first_of('<');
            if (templatePos != String::npos)
            {
                templateSignatureString = name.substr(templatePos + 1);
                name = name.substr(0, templatePos);
                templatePos = templateSignatureString.find_last_of('>');
                if (templatePos == String::npos)
                    return nullptr;
                ext = templateSignatureString.substr(templatePos + 1);
                templateSignatureString = templateSignatureString.substr(0, templatePos);
                if (templateSignatureString.empty())
                    return nullptr;
                if (!_findSymbols(templateSignatureString, templateSignature))
                    return nullptr;
            }
            if (i == words.size() - 1)
            {
                if (a_pFunctionSignature)
                {
                    if (templatePos != String::npos)
                    {
                        PHANTOM_WARNING(false, "template subroutine not yet supported");
                        return nullptr;
                    }
                    Scope* pAsScope = pScope->asScope();
                    return pAsScope ? pAsScope->getSubroutine(name, *a_pFunctionSignature, a_RefQualifiers) : nullptr;
                }
            }
            if (templatePos != String::npos)
            {
                Scope*                  pAsScope = pScope->asScope();
                TemplateSpecialization* pSpec = pAsScope
                ? pAsScope->getTemplateSpecialization(name, (const LanguageElements&)templateSignature)
                : nullptr;
                if (pSpec)
                {
                    pScope = pSpec->getTemplated();
                    Application_clearClonedConstants(templateSignature);
                }
                // TODO : review template instantiation in Application::findSymbol (see code in
                // history here for old implem)
            }
            else
            {
                Subroutine* pSubroutine;
                if (name.size() && name.front() == '$' AND(pSubroutine = pScope->asSubroutine())) // parameter
                {
                    int i2 = -1;
                    if (sscanf(name.c_str() + 1, "%d", &i2) != 1 OR i2 < 0 OR i2 >= pSubroutine->getParameters().size())
                    {
                        return nullptr;
                    }
                    pScope = pSubroutine->getParameters()[i2];
                }
                else
                {
                    Symbols symbols;
                    pScope->asLanguageElement()->getSymbolsWithName(name, symbols);
                    if (symbols.size() == 0)
                    {
                        if (PackageFolder* pFolder2 = pScope->asPackageFolder())
                        {
                            for (auto pPack : pFolder2->getPackages())
                            {
                                pPack->getSymbolsWithName(name, symbols);
                            }
                        }
                    }
                    if (symbols.size() != 1)
                    {
                        return nullptr;
                    }
                    pScope = symbols.back();
                }
            }
        }
        else
        {
            if ((pScope = pFolder->getSource(name)))
                continue;
            for (auto pFold : pFolder->getPackageFolders())
            {
                if (pFold->getName() == name)
                {
                    pFolder = pFold;
                    break;
                }
            }
        }
    }
    if (pScope)
        return pScope->asSymbol();
    return pFolder;
}

Symbol* Application::findSymbol(StringView a_strUniqueName, LanguageElement* a_pScope /*= nullptr*/) const
{
    Symbol*   pResultSymbol = nullptr;
    String    baseName = a_strUniqueName;
    Types     functionSignature;
    String    functionSignatureString;
    size_t    parenPos = baseName.find_first_of('(');
    Modifiers refqualifiers = 0;
    if (parenPos != String::npos)
    {
        functionSignatureString = baseName.substr(parenPos + 1);
        baseName = baseName.substr(0, parenPos);

        parenPos = functionSignatureString.find_last_of(')');
        if (parenPos == String::npos)
            return nullptr;
        if (parenPos != functionSignatureString.size())
        {
            // search for const & and && ref qualifiers
            String refqual = functionSignatureString.substr(parenPos + 1);
            size_t constPos = refqual.find("@const");
            if (constPos == 0)
            {
                refqualifiers |= PHANTOM_R_CONST;
                refqual = refqual.substr(5);
            }
            if (refqual == "&&")
                refqualifiers |= PHANTOM_R_RVALUEREF;
            else if (refqual == "&")
                refqualifiers |= PHANTOM_R_LVALUEREF;
        }
        functionSignatureString = functionSignatureString.substr(0, parenPos);
        if (functionSignatureString.size())
        {
            Symbols symbols;
            if (_findSymbols(functionSignatureString, symbols))
            {
                for (auto it = symbols.begin(); it != symbols.end(); ++it)
                {
                    Type* pType = (*it)->asType();
                    if (pType == nullptr)
                        return nullptr;
                    functionSignature.push_back(pType);
                }
            }
            else
                return nullptr;
        }
    }

    String  extension;
    Strings words;
    size_t  extensionOffset = 1;
    {
        String wordString;
        int    templateLevel = 0;
        bool   bExtension = false;
        String extensionString;
        for (size_t i = 0; i < baseName.size(); ++i)
        {
            char c = baseName[i];
            switch (c)
            {
            case '<':
            {
                templateLevel++;
                wordString.append(c);
            }
            break;
            case '>':
            {
                templateLevel--;
                wordString.append(c);
                if (templateLevel == 0)
                {
                    extensionOffset = wordString.size();
                }
            }
            break;
            case '.':
                if (templateLevel == 0)
                {
                    extensionOffset = 1;
                    if (extensionString.size())
                        return nullptr;
                    bExtension = false;
                    words.push_back(wordString);
                    wordString.clear();
                    break;
                }
            default:
                wordString.append(c);
            }
        }
        if (wordString.empty())
            return nullptr;
        words.push_back(wordString);
    }

    if (words.empty())
        return nullptr;

    size_t ext_pos = words.back().find_first_of("&[*@", extensionOffset);
    if (ext_pos != String::npos)
    {
        extension = words.back().substr(ext_pos);
        words.back() = words.back().substr(0, ext_pos);
    }

    if (a_pScope == nullptr)
        pResultSymbol =
        _findSymbol(words, parenPos != String::npos ? &functionSignature : nullptr, refqualifiers, nullptr);
    else
        do
        {
            if (Package* pPackage = a_pScope->asPackage())
                a_pScope = pPackage->getPackageFolder();
            if ((pResultSymbol =
                 _findSymbol(words, parenPos != String::npos ? &functionSignature : nullptr, refqualifiers, a_pScope)))
                break;
            a_pScope = a_pScope->getOwner();
        } while (a_pScope);

    if (pResultSymbol && extension.size())
    {
        bool    arrayExtent = false;
        String  internalString;
        Strings templateTypes;
        Type*   pType = pResultSymbol->asType();
        for (size_t i = 0; i < extension.size(); ++i)
        {
            if (pType == nullptr)
                return nullptr;
            char c = extension[i];
            if (arrayExtent)
            {
                if (c == ']')
                {
                    size_t extent = ~size_t(0);
                    if (internalString.empty())
                    {
                        pType = pType->makeArray(0);
                        continue;
                    }
                    if (sscanf(internalString.c_str(), "%zu", &extent) == EOF)
                        return nullptr;
                    if (extent == 0)
                        return nullptr;
                    pType = pType->makeArray(extent);
                    arrayExtent = false;
                    internalString.clear();
                }
                else
                    internalString.append(c);
            }
            else
                switch (c)
                {
                case '&':
                {
                    if (pType->asLValueReference())
                    {
                        pType = pType->removeReference()->makeRValueReference();
                    }
                    else if (pType->asRValueReference())
                        return nullptr;
                    else
                        pType = pType->makeLValueReference();
                }
                break;
                case '*':
                {
                    pType = pType->makePointer();
                }
                break;
                case '[':
                {
                    arrayExtent = true;
                }
                break;
                case '@':
                {
                    size_t offset = i + 1;
                    if (extension.find("const", offset) == offset)
                    {
                        pType = pType->makeConst();
                        i += 5;
                    }
                    else if (extension.find("volatile", offset) == offset)
                    {
                        pType = pType->makeVolatile();
                        i += 8;
                    }
                }
                break;
                }
        }
        if (arrayExtent)
            return nullptr;
        return pType;
    }

    return pResultSymbol;
}

Type* Application::findType(StringView a_strUniqueName, LanguageElement* a_pScope /*= nullptr*/) const
{
    Symbol* pSymbol = findSymbol(a_strUniqueName, a_pScope);
    return pSymbol ? pSymbol->asType() : nullptr;
}

void Application::findClasses(Classes& a_Classes, Class* a_pBaseClass /*= nullptr*/,
                              bool a_bDefaultInstanciable /*= false*/) const
{
    for (auto p : m_Modules)
    {
        p->findClasses(a_Classes, a_pBaseClass, a_bDefaultInstanciable);
    }
}

} // namespace lang
} // namespace phantom
