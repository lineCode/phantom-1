#pragma once

// haunt {

#include "Module.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/signal>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Module")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Module)
        {
            using Classes = typedef_< phantom::lang::Classes>;
            using Dependencies = typedef_<_::Dependencies>;
            using FuncT = typedef_<_::FuncT>;
            using Functions = typedef_< phantom::lang::Functions>;
            using Modules = typedef_< phantom::lang::Modules>;
            using Packages = typedef_< phantom::lang::Packages>;
            using Sources = typedef_< phantom::lang::Sources>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::lang::Types>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<Dependencies>("Dependencies")
        
        .public_()
            .constructor<void(StringView, uint)>()["0"]
            .method<Module*() const, virtual_|override_>("asModule", &_::asModule)
            .method<bool() const>("isPlugin", &_::isPlugin)
            .method<Plugin*() const>("getPlugin", &_::getPlugin)
            .method<StringView() const>("getLibraryFullName", &_::getLibraryFullName)
            .method<StringView() const>("getDeclarationCppFullName", &_::getDeclarationCppFullName)
            .method<byte*() const>("getMemoryStart", &_::getMemoryStart)
            .method<size_t() const>("getHandle", &_::getHandle)
            .method<Packages const&() const>("getPackages", &_::getPackages)
            .method<bool() const, virtual_|override_>("canBeUnloaded", &_::canBeUnloaded)
            .method<Package*() const>("getDefaultPackage", &_::getDefaultPackage)
            .method<Package*(StringView) const>("getPackage", &_::getPackage)
            .method<void(Package*)>("addPackage", &_::addPackage)
            .method<void(Package*)>("removePackage", &_::removePackage)
            .method<Modules const&() const>("getDependencies", &_::getDependencies)
            .method<void(Module*)>("addDependency", &_::addDependency)
            .method<void(Module*)>("removeDependency", &_::removeDependency)
            .method<bool(Module*) const>("hasDependency", &_::hasDependency)
            .method<bool(Module*) const>("hasDependencyCascade", &_::hasDependencyCascade)
            .method<void(Sources&) const>("getSources", &_::getSources)
            .method<void(Modules&) const>("fetchDependencies", &_::fetchDependencies)
            .method<void(Classes&, ::phantom::lang::Class *, bool) const>("findClasses", &_::findClasses)["nullptr"]["false"]
            .method<void(Functions&, StringView, const Types*, Type*) const>("findFunctions", &_::findFunctions)["\"\""]["nullptr"]["nullptr"]
            .method<Source*() const>("getAnonymousSource", &_::getAnonymousSource)
            /// missing symbol(s) reflection (phantom::MemoryContext) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<MemoryContext&()>("getMemoryContext", &_::getMemoryContext)
            .method<Type*(hash64) const>("findType", &_::findType)
            .method<Type*(hash64) const>("findUsableType", &_::findUsableType)
            .typedef_<FuncT>("FuncT")
            .method<void(FuncT)>("setOnLoadFunc", &_::setOnLoadFunc)
            .method<void(FuncT)>("setOnUnloadFunc", &_::setOnUnloadFunc)
            .method<FuncT() const>("getOnLoadFunc", &_::getOnLoadFunc)
            .method<FuncT() const>("getOnUnloadFunc", &_::getOnUnloadFunc)
            .method<bool() const>("isMarkedOutdated", &_::isMarkedOutdated)
            .method<void()>("markOutdated", &_::markOutdated)
            .method<void()>("markUpToDate", &_::markUpToDate)
        
        .public_()
            .signal("packageAdded", &_::packageAdded)
            .signal("packageAboutToBeRemoved", &_::packageAboutToBeRemoved)
            .signal("changed", &_::changed)
        
        .protected_()
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
            ;
        }
        PHANTOM_STRUCT(ModuleReverseDependencySorter)
        {
            this_()
            .method<bool(Module*, Module*) const>("operator()", &_::operator())
            ;
        }
        PHANTOM_STRUCT(ModuleDependencySorter)
        {
            this_()
            .method<bool(Module*, Module*) const>("operator()", &_::operator())
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Module")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
