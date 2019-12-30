#pragma once

// haunt {

#include "Application.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/signal>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Application")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Application)
        {
            using Classes = typedef_< phantom::reflection::Classes>;
            using LoadedLibraries = typedef_<_::LoadedLibraries>;
            using LoadedLibraryModules = typedef_<_::LoadedLibraryModules>;
            using Modules = typedef_< phantom::reflection::Modules>;
            using Plugins = typedef_< phantom::reflection::Plugins>;
            using Sources = typedef_< phantom::reflection::Sources>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Strings = typedef_< phantom::Strings>;
            using Symbols = typedef_< phantom::reflection::Symbols>;
            using Types = typedef_< phantom::reflection::Types>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<LoadedLibraryModules>("LoadedLibraryModules")
            .typedef_<LoadedLibraries>("LoadedLibraries")
            /// missing symbol(s) reflection (phantom::reflection::Expression, phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .typedef_<CppExpressionParser>("CppExpressionParser")
        
        .public_()
            .staticMethod<Application*()>("Get", &_::Get)
        
        .public_()
            .constructor<void()>()
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(CppExpressionParser)>("setCppExpressionParser", &_::setCppExpressionParser)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<CppExpressionParser()>("getCppExpressionParser", &_::getCppExpressionParser)
            /// missing symbol(s) reflection (phantom::reflection::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(StringView, LanguageElement*) const>("cppExpression", &_::cppExpression)
            .method<Symbol*(StringView, LanguageElement*, StringBuffer*)>("findCppSymbol", &_::findCppSymbol)["nullptr"]
            .method<bool(StringView, Symbols&, LanguageElement*, StringBuffer*)>("findCppSymbols", &_::findCppSymbols)["nullptr"]
            .method<Type*(StringView, LanguageElement*, StringBuffer*)>("findCppType", &_::findCppType)["nullptr"]
            .method<Template*(StringView, LanguageElement*, StringBuffer*)>("findCppTemplate", &_::findCppTemplate)["nullptr"]
            .method<Template*(StringView, StringBuffer*)>("findCppTemplate", &_::findCppTemplate)["nullptr"]
            .method<Class*(StringView, LanguageElement*, StringBuffer*)>("findCppClass", &_::findCppClass)["nullptr"]
            .method<Symbol*(StringView, StringBuffer*)>("findCppSymbol", &_::findCppSymbol)["nullptr"]
            .method<bool(StringView, Symbols&, StringBuffer*)>("findCppSymbols", &_::findCppSymbols)["nullptr"]
            .method<Type*(StringView, StringBuffer*)>("findCppType", &_::findCppType)["nullptr"]
            .method<Class*(StringView, StringBuffer*)>("findCppClass", &_::findCppClass)["nullptr"]
            .method<Module*() const>("getMainModule", &_::getMainModule)
            .method<Modules() const>("getNativeModules", &_::getNativeModules)
            .method<Module*(StringView) const>("getSharedLibraryModule", &_::getSharedLibraryModule)
            .method<Module*(const byte*, uint, byte*)>("nativeModuleFromStackContext", &_::nativeModuleFromStackContext)["1"]["nullptr"]
            .method<Module*(const byte*)>("nativeModuleFromProgramCounter", &_::nativeModuleFromProgramCounter)
            .method<bool(StringView)>("setDefaultExportPath", &_::setDefaultExportPath)
            .method<StringView() const>("getDefaultExportPath", &_::getDefaultExportPath)
            .method<bool(StringView)>("setDefaultPluginPath", &_::setDefaultPluginPath)
            .method<StringView() const>("getDefaultPluginPath", &_::getDefaultPluginPath)
            .method<bool(StringView)>("addPluginPath", &_::addPluginPath)
            .method<bool(StringView)>("removePluginPath", &_::removePluginPath)
            .method<bool(StringView) const>("hasPluginPath", &_::hasPluginPath)
            .method<bool(StringView)>("setDefaultBinaryPath", &_::setDefaultBinaryPath)
            .method<StringView() const>("getDefaultBinaryPath", &_::getDefaultBinaryPath)
            .method<bool(StringView)>("addBinaryPath", &_::addBinaryPath)
            .method<bool(StringView)>("removeBinaryPath", &_::removeBinaryPath)
            .method<bool(StringView) const>("hasBinaryPath", &_::hasBinaryPath)
            .method<Type*(StringView) const>("getBuiltInType", &_::getBuiltInType)
            .method<Types const&() const>("getBuiltInTypes", &_::getBuiltInTypes)
            .method<LanguageElement*(LanguageElement*)>("invalid", &_::invalid)
            .method<Module*(StringView) const>("getModule", &_::getModule)
            .method<Modules const&() const>("getModules", &_::getModules)
            .method<Modules const&() const>("getStartupModules", &_::getStartupModules)
            .method<Module*(size_t) const>("getModuleByHandle", &_::getModuleByHandle)
            .method<Plugin*(StringView) const>("getPlugin", &_::getPlugin)
            .method<Plugins const&() const>("getPlugins", &_::getPlugins)
            .method<void(Plugin*)>("addPlugin", &_::addPlugin)
            .method<void(Plugin*)>("removePlugin", &_::removePlugin)
            .method<void(Module*)>("addModule", &_::addModule)
            .method<void(Module*)>("removeModule", &_::removeModule)
            .method<void(Sources&) const>("getSources", &_::getSources)
            .method<Operator(StringView) const>("getBuiltInBinaryOperatorId", &_::getBuiltInBinaryOperatorId)
            .method<Operator(StringView) const>("getBuiltInPreUnaryOperatorId", &_::getBuiltInPreUnaryOperatorId)
            .method<Operator(StringView) const>("getBuiltInPostUnaryOperatorId", &_::getBuiltInPostUnaryOperatorId)
            .method<const Strings&() const>("getSourcePaths", &_::getSourcePaths)
            .method<StringView() const>("getDefaultSourcePath", &_::getDefaultSourcePath)
            .method<bool(StringView)>("setDefaultSourcePath", &_::setDefaultSourcePath)
            .method<bool(StringView)>("addSourcePath", &_::addSourcePath)
            .method<bool(StringView)>("removeSourcePath", &_::removeSourcePath)
            .method<PackageFolder*() const>("rootPackageFolder", &_::rootPackageFolder)
            .method<Source*(StringView) const>("getSource", &_::getSource)
            .method<PackageFolder*(StringView) const>("getPackageFolder", &_::getPackageFolder)
            .method<PackageFolder*(StringView)>("getOrCreatePackageFolder", &_::getOrCreatePackageFolder)
            .method<String(StringView) const>("findPluginPath", &_::findPluginPath)
            .method<void(StringView, Strings&) const>("findPlugins", &_::findPlugins)
            .method<Strings const&() const>("getPluginPaths", &_::getPluginPaths)
            .method<Type*(hash64) const>("findType", &_::findType)
            .method<Symbol*(StringView, LanguageElement*) const>("findSymbol", &_::findSymbol)["nullptr"]
            .method<Type*(StringView, LanguageElement*) const>("findType", &_::findType)["nullptr"]
            .method<void(Classes&, ::phantom::reflection::Class *, bool) const>("findClasses", &_::findClasses)["nullptr"]["false"]
            .method<Undefineds*(StringView) const>("getUndefineds", &_::getUndefineds)
            .method<Undefineds*(StringView)>("undefineds", &_::undefineds)
            .method<Constant*() const>("getNullptr", &_::getNullptr)
            .method<Type*() const>("getAuto", &_::getAuto)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            /// missing symbol(s) reflection (phantom::MemoryContext) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<MemoryContext&()>("getMemoryContext", &_::getMemoryContext)
        
        .public_()
            .signal("pluginPathAdded", &_::pluginPathAdded)
            .signal("pluginPathRemoved", &_::pluginPathRemoved)
            .signal("binaryPathAdded", &_::binaryPathAdded)
            .signal("binaryPathRemoved", &_::binaryPathRemoved)
            .signal("moduleAdded", &_::moduleAdded)
            .signal("moduleDiscarded", &_::moduleDiscarded)
            .signal("moduleAboutToBeRemoved", &_::moduleAboutToBeRemoved)
            .signal("moduleUnloaded", &_::moduleUnloaded)
            .signal("pluginAdded", &_::pluginAdded)
            .signal("pluginAboutToBeRemoved", &_::pluginAboutToBeRemoved)
            .signal("pluginLoaded", &_::pluginLoaded)
            .signal("pluginLoadingFailed", &_::pluginLoadingFailed)
            .signal("pluginAboutToBeUnloaded", &_::pluginAboutToBeUnloaded)
            .signal("sourceAdded", &_::sourceAdded)
            .signal("sourceAboutToBeRemoved", &_::sourceAboutToBeRemoved)
            .signal("packageAdded", &_::packageAdded)
            .signal("packageAboutToBeRemoved", &_::packageAboutToBeRemoved)
            .signal("sourceDeleted", &_::sourceDeleted)
        
        .protected_()
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Application")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
