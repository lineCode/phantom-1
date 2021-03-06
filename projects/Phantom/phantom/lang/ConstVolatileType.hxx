#pragma once

// haunt {

#include "ConstVolatileType.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ConstVolatileType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ConstVolatileType)
        {
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::QualifiedType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .method<Type*() const>("getConstVolatiledType", &_::getConstVolatiledType)
            .method<Type*() const, virtual_|override_>("addConst", &_::addConst)
            .method<Type*() const, virtual_|override_>("addVolatile", &_::addVolatile)
            .method<Type*() const, virtual_>("addConstVolatile", &_::addConstVolatile)
            .method<Type*() const, virtual_|override_>("removeConst", &_::removeConst)
            .method<Type*() const, virtual_|override_>("removeAllConst", &_::removeAllConst)
            .method<Type*() const, virtual_|override_>("removeVolatile", &_::removeVolatile)
            .method<Type*() const, virtual_|override_>("removeConstVolatile", &_::removeConstVolatile)
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)
            .method<ConstVolatileType*() const, virtual_|override_>("asConstVolatileType", &_::asConstVolatileType)
            .method<ConstVolatileType*() const, virtual_>("asConstVolatileClass", &_::asConstVolatileClass)
            .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .protected_()
            .method<ConstVolatileType*() const, virtual_|override_>("createConstVolatileType", &_::createConstVolatileType)
            .method<ConstType*() const, virtual_|override_>("createConstType", &_::createConstType)
            .method<VolatileType*() const, virtual_|override_>("createVolatileType", &_::createVolatileType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ConstVolatileType")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
