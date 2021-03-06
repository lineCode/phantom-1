// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Signature.h"

#include "Application.h"
#include "Parameter.h"
#include "Source.h"
#include "Subroutine.h"

#include <phantom/detail/new.h>
#include <phantom/utils/StringUtil.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Signature* Signature::Create(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "filter PHANTOM_R_METHOD_QUAL_MASK in SignatureH, not here")
#endif
    return PHANTOM_DEFERRED_NEW_EX(Signature)(a_Modifiers & (PHANTOM_R_METHOD_QUAL_MASK), a_uiFlags);
}

Signature* Signature::Create(Type* a_pReturnType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    return PHANTOM_DEFERRED_NEW_EX(Signature)(a_pReturnType, a_Modifiers & (PHANTOM_R_METHOD_QUAL_MASK), a_uiFlags);
}

Signature* Signature::Create(Type* a_pReturnType, Type* a_pSingleParameterType, Modifiers a_Modifiers /*= 0*/,
                             uint a_uiFlags /*= 0*/)
{
    return PHANTOM_DEFERRED_NEW_EX(Signature)(a_pReturnType, a_pSingleParameterType,
                                              a_Modifiers & (PHANTOM_R_METHOD_QUAL_MASK), a_uiFlags);
}

Signature* Signature::Create(StringView a_strCode, LanguageElement* a_pContextScope /*= nullptr*/,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    return PHANTOM_DEFERRED_NEW_EX(Signature)(a_strCode, a_pContextScope, a_Modifiers & (PHANTOM_R_METHOD_QUAL_MASK),
                                              a_uiFlags);
}

Signature* Signature::Create(Type* a_pReturnType, Type* a_pFirstParameterType, Type* a_pSecondParameterType,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Signature* pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(a_pReturnType, a_Modifiers, a_uiFlags);
    pSignature->addParameter(a_pFirstParameterType);
    pSignature->addParameter(a_pSecondParameterType);
    return pSignature;
}

Signature* Signature::Create(Type* a_pReturnType, TypesView a_ParameterTypes, Modifiers a_Modifiers /*= 0*/,
                             uint a_uiFlags /*= 0*/)
{
    Signature* pSignature = PHANTOM_DEFERRED_NEW_EX(Signature)(a_pReturnType, a_Modifiers, a_uiFlags);
    for (auto pParamT : a_ParameterTypes)
        pSignature->addParameter(pParamT);
    return pSignature;
}

Signature* Signature::Create(Type* a_pReturnType, uint a_uiFlags)
{
    return Create(a_pReturnType, Modifier::None, a_uiFlags);
}

Signature::Signature(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pReturnType(nullptr),
      m_pReturnTypeName(nullptr)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    setReturnType(PHANTOM_TYPEOF(void));
}

Signature::Signature(Type* a_pReturnType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pReturnType(nullptr),
      m_pReturnTypeName(nullptr)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    setReturnType(a_pReturnType);
}

Signature::Signature(Type* a_pReturnType, Type* a_pSingleParameterType, Modifiers a_Modifiers /*= 0*/,
                     uint a_uiFlags /*= 0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pReturnType(nullptr),
      m_pReturnTypeName(nullptr)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    setReturnType(a_pReturnType);
    addParameter(
    PHANTOM_DEFERRED_NEW_EX(Parameter)(a_pSingleParameterType, Modifier::None, a_uiFlags & PHANTOM_R_FLAG_NATIVE));
}

Signature::Signature(Type* a_pType, const Parameters& a_Parameters, Modifiers a_Modifiers /*= 0 */,
                     uint a_uiFlags /*= 0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pReturnType(nullptr),
      m_pReturnTypeName(nullptr)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    setReturnType(a_pType);
    for (auto it = a_Parameters.begin(); it != a_Parameters.end(); ++it)
    {
        addParameter(*it);
    }
}

Signature::Signature(Type* a_pType, TypesView a_Types, Modifiers a_Modifiers /*= 0 */, uint a_uiFlags /*=0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pReturnType(nullptr),
      m_pReturnTypeName(nullptr)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    setReturnType(a_pType);
    for (auto pType : a_Types)
    {
        addParameter(PHANTOM_DEFERRED_NEW_EX(Parameter)(pType, Modifier::None, a_uiFlags & PHANTOM_R_FLAG_NATIVE));
    }
}

Signature::Signature(StringView a_strCode, LanguageElement* a_pContextScope, Modifiers a_Modifiers /*= 0 */,
                     uint a_uiFlags /*=0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pReturnType(nullptr),
      m_pReturnTypeName(nullptr)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    a_pContextScope->addScopedElement(this);
    parse(a_strCode, this);
    a_pContextScope->removeScopedElement(this);
}

void Signature::parse(StringView a_strSignature, LanguageElement* a_pContextScope)
{
    size_t i = 0;
    size_t length = a_strSignature.size();
    String returnType;
    char   prevChar = '0';
    for (; i < length; ++i)
    {
        char c = a_strSignature[i];
        if (c == '(')
        {
            PHANTOM_ASSERT(NOT(returnType.empty()), "no return type specified in the signature String");
            Type* pReturnType = m_pReturnType;
            if (pReturnType == nullptr)
            {
                pReturnType = Application::Get()->findCppType(returnType, a_pContextScope);
            }
            PHANTOM_ASSERT(pReturnType, "invalid return type %s", returnType.c_str());
            setReturnType(pReturnType);
            PHANTOM_VERIFY(parseParameterTypeList(a_strSignature.substr(i), a_pContextScope),
                           "signature parsing failed '%.*s'",
                           PHANTOM_STRING_AS_PRINTF_ARG(StringView(a_strSignature).substr(i)));
            return;
        }
        else
        {
            if (prevChar == '>' AND c == '>') /// avoid shift right / end of template ambiguity
                returnType += ' ';
            returnType += c;
        }
        prevChar = c;
    }
    PHANTOM_ASSERT(false, "invalid signature formating, missing '(types...)'");
}

Signature::~Signature()
{
}

Parameter* Signature::addParameter(Type* a_pType, StringView a_strName)
{
    Parameter* p =
    PHANTOM_DEFERRED_NEW_EX(Parameter)(a_pType, a_strName, Modifier::None, getFlags() & PHANTOM_R_FLAG_NATIVE);
    addParameter(p);
    return p;
}

void Signature::addParameter(Parameter* a_pParameter)
{
    if (m_Parameters.size())
    {
        if (m_Parameters.back()->hasDefaultArgument())
        {
            PHANTOM_ASSERT(a_pParameter->hasDefaultArgument(),
                           "parameter must have a default argument because the previous parameter had one");
        }
    }
    PHANTOM_ASSERT(NOT(isVariadic()), "cannot add parameters after a variadic parameter pack");
    PHANTOM_ASSERT(a_pParameter);
    addElement(a_pParameter);
    m_Parameters.push_back(a_pParameter);
}

void Signature::setReturnType(Type* a_pType)
{
    PHANTOM_ASSERT(
    a_pType AND(a_pType->isTemplateDependant() OR(a_pType == PHANTOM_TYPEOF(void)) OR a_pType->isMoveConstructible()));
    m_pReturnType = a_pType;
    PHANTOM_ASSERT(m_pReturnType);
    addReferencedElement(a_pType);
}

void Signature::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pReturnType == a_pElement)
    {
        m_pReturnType = nullptr;
    }
    LanguageElement::onReferencedElementRemoved(a_pElement);
}

void Signature::onElementRemoved(LanguageElement* a_pElement)
{
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        if ((*it) == a_pElement)
        {
            m_Parameters.erase(std::find(m_Parameters.begin(), m_Parameters.end(), a_pElement));
            break;
        }
    }
    LanguageElement::onElementRemoved(a_pElement);
}

size_t Signature::getParameterCount() const
{
    return m_Parameters.size();
}

Type* Signature::getParameterType(size_t a_uiParamIndex) const
{
    PHANTOM_ASSERT(a_uiParamIndex < getParameterCount(), "Index too big");
    return m_Parameters[a_uiParamIndex]->getValueType();
}

Type* Signature::getReturnType() const
{
    return m_pReturnType;
}

bool Signature::separateParameters(StringView a_strText, Strings& a_OutParameters)
{
    String parameter;
    size_t length = a_strText.size();
    size_t i = 0;
    size_t template_level = 0;
    for (; i < length; ++i)
    {
        char c = a_strText[i];
        if (c == ',' AND template_level == 0)
        {
            a_OutParameters.push_back(parameter);
            parameter.clear();
        }
        else
        {
            if (c == '<')
            {
                ++template_level;
            }
            else if (c == '>')
            {
                if (template_level != 0)
                    --template_level;
            }
            parameter += c;
        }
    }
    if (template_level == 0 AND NOT(parameter.empty()))
    {
        a_OutParameters.push_back(parameter);
        return true;
    }
    a_OutParameters.clear();
    return false;
}

bool Signature::parseParameterTypeList(StringView a_strText, LanguageElement* a_pContextScope)
{
    if (a_strText.empty())
        return false;
    Strings words;
    StringUtil::Split(words, a_strText, "()");
    words.erase(std::remove_if(words.begin(), words.end(), std::bind(&String::empty, std::placeholders::_1)),
                words.end());
    if (words.empty())
        return true;
    if (a_strText[0] != '(')
    {
        words.erase(words.begin());
    }
    if (words.size() != 1)
        return false;

    Strings parameters;
    if (NOT(separateParameters(words.front(), parameters)))
        return false;
    for (auto it = parameters.begin(); it != parameters.end(); ++it)
    {
        Type*  pType = nullptr;
        String name;
        StringUtil::ReplaceAll(*it, ">", "> ");
        Symbol* pParamElement = Application::Get()->findCppSymbol(*it, a_pContextScope);
        if (pParamElement == nullptr)
        {
            PHANTOM_ASSERT(!isNative());
            return false;
        }
        else
        {
            if ((pType = pParamElement->toType())) // if alias, toType will unalias
            {
                addParameter(PHANTOM_DEFERRED_NEW_EX(Parameter)(pType, name));
            }
            else if (pParamElement->asParameter())
            {
                addParameter(static_cast<Parameter*>(pParamElement));
            }
            else
            {
                PHANTOM_ASSERT(false, "symbol is not a type or parameter");
            }
        }
    }
    return true;
}

bool Signature::isSame(Signature* a_pOther) const
{
    if (m_Modifiers != a_pOther->m_Modifiers)
        return false;
    Types types;
    for (size_t i = 0; i < m_Parameters.size(); ++i)
    {
        if (NOT(m_Parameters[i]->getValueType()->isSame(a_pOther->m_Parameters[i]->getValueType())))
            return false;
    }
    return m_pReturnType->isSame(a_pOther->m_pReturnType);
}

void Signature::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getQualifiedDecoratedName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += " const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += " volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';
        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

void Signature::getUniqueName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getUniqueName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += "@const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += "@volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';
        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

void Signature::getQualifiedName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getQualifiedName(a_Buf);
    }
    a_Buf += ')';
}

void Signature::getDecoratedName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getDecoratedName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += " const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += " volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';
        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

Signature* Signature::clone() const
{
    Signature* pSignature = PHANTOM_DEFERRED_NEW_EX(Signature);
    for (Parameter* pParameter : m_Parameters)
    {
        pSignature->addParameter(static_cast<Parameter*>(pParameter->clone()));
    }
    pSignature->setReturnType(m_pReturnType);
    return pSignature;
}

bool Signature::isSame(Symbol* a_pOther) const
{
    if (NOT(a_pOther->asSignature()))
        return false;
    return isSame(static_cast<Signature*>(a_pOther));
}

StringView Signature::getParameterName(size_t a_uiParamIndex) const
{
    return m_Parameters[a_uiParamIndex]->getName();
}

Expression* Signature::getParameterDefaultValueExpression(size_t a_uiParamIndex) const
{
    return m_Parameters[a_uiParamIndex]->getDefaultArgumentExpression();
}

void Signature::setParameterName(size_t i, StringView a_strName)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(m_Parameters[i]->m_strName == a_strName || m_Parameters[i]->m_strName.empty(),
                   "attempt to change a non-empty parameter name");
    m_Parameters[i]->m_strName = a_strName;
}

Types Signature::getParameterTypes() const
{
    Types types;
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        types.push_back((*it) ? (*it)->getValueType() : nullptr);
    }
    return types;
}

size_t Signature::getRequiredArgumentCount() const
{
    size_t defaultParameterCount = 0;
    size_t i = m_Parameters.size();
    while (i--)
    {
        if (m_Parameters[i]->hasDefaultArgument())
            defaultParameterCount++;
        else
            break;
    }
    return m_Parameters.size() - defaultParameterCount;
}

bool Signature::matches(TypesView a_ParameterTypes, Modifiers a_Modifiers) const
{
    if (a_ParameterTypes.size() != getParameterCount() OR(getModifiers() != a_Modifiers))
    {
        return false;
    }
    for (size_t i = 0; i < m_Parameters.size(); ++i)
    {
        if (a_ParameterTypes[i] != nullptr AND NOT(m_Parameters[i]->getValueType()->isSame(a_ParameterTypes[i])))
            return false;
    }
    return true;
}

void Signature::getUniqueParameterTypeNameList(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        if (it != m_Parameters.begin())
            a_Buf += ',';
        (*it)->getValueType()->getUniqueName(a_Buf);
    }
    a_Buf += ')';
    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += "@const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += "@volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';

        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

bool Signature::isRVOCandidate() const
{
    return m_pReturnType->removeConst()->asClassType() != nullptr;
}

Subroutine* Signature::getOwnerSubroutine() const
{
    return static_cast<Subroutine*>(getOwner());
}

Parameter* Signature::getParameter(StringView a_strName) const
{
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        if ((*it)->getName() == a_strName)
            return *it;
    }
    return nullptr;
}

void Signature::setNativeDefaultArgumentStrings(ArrayView<StringView> a_DefaultArgs)
{
    if (a_DefaultArgs.empty())
        return;
    size_t paramCount = m_Parameters.size();
    PHANTOM_ASSERT(paramCount, "empty signature cannot have default arguments");
    PHANTOM_ASSERT(a_DefaultArgs.size() <= paramCount, "too many default arguments");
    for (size_t i = 0; i < a_DefaultArgs.size(); ++i)
    {
        auto       arg = a_DefaultArgs[i];
        size_t     paramIndex = paramCount - a_DefaultArgs.size() + i;
        Parameter* pParameter = getParameter((size_t)paramIndex);
        pParameter->m_NativeDefaultArgumentStr = arg;
        paramIndex--;
    }
}

StringViews Signature::getNativeDefaultArgumentStrings() const
{
    StringViews args;
    for (auto pParam : m_Parameters)
    {
        StringView view = pParam->getNativeDefaultArgumentString();
        if (view.size())
            args.push_back(view);
    }
    return args;
}

FunctionType* Signature::getFunctionType() const
{
    PHANTOM_ASSERT(getSource(), "signature must be attached to a source via its subroutine");
    return getSource()->functionType(m_pReturnType, getParameterTypes(), m_Modifiers);
}

} // namespace lang
} // namespace phantom
