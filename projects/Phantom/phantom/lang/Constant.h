// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/MetaNewDelete.h>
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Constant : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Constant);

public:
    template<typename t_Ty>
    static inline Constant* Create(t_Ty a_Constant, StringView name = "", Type* a_pType = nullptr);

public:
    Constant(Type* a_pValueType, Modifiers modifiers = 0, uint a_uiFlags = 0);
    Constant(Type* a_pValueType, StringView a_strName, Modifiers modifiers = 0, uint a_uiFlags = 0);

    virtual Type* getValueType() const
    {
        return m_pValueType;
    }
    virtual void getValue(void* dest) const = 0;
    virtual bool hasValue(const void* a_pSrc) const = 0;
    Constant*    asConstant() const override
    {
        return const_cast<Constant*>(this);
    }

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

    using LanguageElement::getQualifiedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getUniqueName;

    Constant*         clone() const;
    virtual Constant* cloneImpl() const = 0;

    virtual bool isIntegral() const
    {
        return getValueType()->asIntegralType() != nullptr;
    }
    virtual bool isZero() const = 0;

    inline bool isIntegralZero() const
    {
        return isIntegral() AND isZero();
    }

    virtual void toString(StringBuffer& a_Buf) const = 0;

    virtual void toLiteral(StringBuffer& a_Buf) const = 0;

protected:
    Type* m_pValueType;
};

} // namespace lang
} // namespace phantom

#include <phantom/lang/ConstantT.h>

namespace phantom
{
namespace lang
{
template<typename t_Ty>
inline Constant* Constant::Create(t_Ty a_Constant, StringView name, Type* a_pType)
{
    if (a_pType)
        return PHANTOM_META_NEW(ConstantT<t_Ty>)(a_pType, name, a_Constant);
    return PHANTOM_META_NEW(ConstantT<t_Ty>)(name, a_Constant);
}

} // namespace lang
} // namespace phantom
