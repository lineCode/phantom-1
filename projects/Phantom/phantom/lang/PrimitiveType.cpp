// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "PrimitiveType.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Constant* PrimitiveType::createConstant(void*, StringView /*= ""*/, PrimitiveType* /*= nullptr*/) const
{
    return nullptr;
}

PrimitiveType::PrimitiveType(TypeKind a_eTypeKind) : Type(a_eTypeKind, "", 0, 0, PHANTOM_R_NONE, PHANTOM_R_NONE)
{
}

PrimitiveType::PrimitiveType(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
