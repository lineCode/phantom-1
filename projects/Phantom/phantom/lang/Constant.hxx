#pragma once

// haunt {

#include "Constant.h"

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
#include <phantom/field>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Constant")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Constant)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .public_()
            .constructor<void(Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, StringView, Modifiers, uint)>()["0"]["0"]
            .method<Type*() const, virtual_>("getValueType", &_::getValueType)
            .method<void(void*) const, pure_virtual>("getValue", &_::getValue)
            .method<bool(const void*) const, pure_virtual>("hasValue", &_::hasValue)
            .method<Constant*() const, virtual_|override_>("asConstant", &_::asConstant)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<Constant*() const>("clone", &_::clone)
            .method<Constant*() const, pure_virtual>("cloneImpl", &_::cloneImpl)
            .method<bool() const, virtual_>("isIntegral", &_::isIntegral)
            .method<bool() const, pure_virtual>("isZero", &_::isZero)
            .method<bool() const>("isIntegralZero", &_::isIntegralZero)
            .method<void(StringBuffer&) const, pure_virtual>("toString", &_::toString)
            .method<void(StringBuffer&) const, pure_virtual>("toLiteral", &_::toLiteral)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Constant")
PHANTOM_END("phantom.lang")
}
}
namespace phantom {
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
