// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "InitializerListType.h"

namespace phantom
{
namespace lang
{
InitializerListType::InitializerListType(TypesView a_Types) : Type(TypeKind::Unknown, "", 0, 0, 0, 0), m_Types(a_Types)
{
    for (auto pType : m_Types)
        addReferencedElement(pType);
}

bool InitializerListType::matches(TypesView a_Types) const
{
    if (a_Types.size() != m_Types.size())
        return false;
    for (size_t i = 0; i < m_Types.size(); ++i)
    {
        if (NOT(m_Types[i]->isSame(a_Types[i])))
            return false;
    }
    return true;
}

void InitializerListType::getUniqueName(StringBuffer& a_Buf) const
{
    a_Buf += '{';
    size_t c = 0;
    for (Type* pType : m_Types)
    {
        if (c++)
            a_Buf += ',';
        pType->getUniqueName(a_Buf);
    }
    a_Buf += '}';
}

void InitializerListType::getQualifiedName(StringBuffer& a_Buf) const
{
    a_Buf += '{';
    size_t c = 0;
    for (Type* pType : m_Types)
    {
        if (c++)
            a_Buf += ',';
        pType->getQualifiedName(a_Buf);
    }
    a_Buf += '}';
}

void InitializerListType::getDecoratedName(StringBuffer& a_Buf) const
{
    a_Buf += '{';
    size_t c = 0;
    for (Type* pType : m_Types)
    {
        if (c++)
            a_Buf += ',';
        pType->getDecoratedName(a_Buf);
    }
    a_Buf += '}';
}

void InitializerListType::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    a_Buf += '{';
    size_t c = 0;
    for (Type* pType : m_Types)
    {
        if (c++)
            a_Buf += ',';
        pType->getQualifiedDecoratedName(a_Buf);
    }
    a_Buf += '}';
}

void InitializerListType::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    auto found = std::find(m_Types.begin(), m_Types.end(), a_pElement);
    if (found != m_Types.end())
    {
        m_Types.erase(found);
    }
}

} // namespace lang
} // namespace phantom
