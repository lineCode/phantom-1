#pragma once

// haunt {

#include "RValueReference.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("RValueReference")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(RValueReference)
        {
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Reference>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(Type*)>()
        
        .public_()
            .method<RValueReference*() const, virtual_|override_>("asRValueReference", &_::asRValueReference)
            .method<RValueReference*() const, virtual_|override_>("asClassRValueReference", &_::asClassRValueReference)
            .method<RValueReference*() const, virtual_|override_>("asConstClassRValueReference", &_::asConstClassRValueReference)
            .method<Type*() const, virtual_|override_>("addLValueReference", &_::addLValueReference)
            .method<Type*() const, virtual_|override_>("addRValueReference", &_::addRValueReference)
            .method<Type*() const, virtual_|override_>("addPointer", &_::addPointer)
            .method<Type*() const, virtual_|override_>("removeRValueReference", &_::removeRValueReference)
            .method<Type*() const, virtual_|override_>("removeAllConst", &_::removeAllConst)
            .method<Type*() const, virtual_|override_>("removeAllQualifiers", &_::removeAllQualifiers)
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("RValueReference")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
