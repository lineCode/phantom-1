// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/UserData.h>
#include <phantom/lang/LanguageElement.h>
#include <phantom/utils/SmallSet.h>
#include <phantom/utils/StringHash.h>
#include <phantom/utils/StringWithHash.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
typedef SmallMap<StringWithHash, Variant> MetaDatas;
typedef SmallSet<String, 1>               Annotations;

class PHANTOM_EXPORT_PHANTOM SymbolExtension : public LanguageElement
{
    friend class Symbol;

protected:
    SymbolExtension() = default;
    SymbolExtension(uint a_uiFlags) : LanguageElement(a_uiFlags)
    {
    }

public:
    Symbol* getSymbol() const;
};

typedef SmallVector<SymbolExtension*, 1, 1> SymbolExtensions;

/// \brief  Represents a named element (symbol).
class PHANTOM_EXPORT_PHANTOM Symbol : public LanguageElement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Symbol);

    friend class phantom::lang::LanguageElement;
    friend class phantom::lang::ClassType;
    friend class phantom::lang::Class;
    friend class phantom::lang::Scope;
    friend class phantom::lang::TemplateSpecialization;
    friend class phantom::lang::Module;
    friend class phantom::lang::Application;
    friend class phantom::lang::Source;
    friend class phantom::lang::Namespace;

public:
    static bool IsCppIdentifier(StringView a_Name);

public:
    Symbol(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

protected:
    Symbol(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    ~Symbol() override;

    void terminate();

    int destructionPriority() const override;

public:
    Symbol* asSymbol() const override
    {
        return (Symbol*)this;
    }

    Type* toType() const override
    {
        return nullptr;
    }

    virtual Namespace* toNamespace() const
    {
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the access of this symbol inside its scope.
    ///
    /// \param  a_eAccess The access (Access::Undefined, Access::Public, Access::Protected,
    /// Access::Private).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setAccess(Access a_eAccess);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the access of this symbol inside its scope.
    ///
    /// \return  The access (Access::Undefined, Access::Public, Access::Protected, Access::Private).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Access getAccess() const
    {
        return m_eAccess;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the modifiers of this symbol.
    ///
    /// \param  a_Modifiers The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setModifiers(Modifiers a_Modifiers);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add modifiers to this symbol.
    ///
    /// \param  a_Modifiers The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addModifiers(Modifiers a_Modifiers);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Remove modifiers from this symbol.
    ///
    /// \param  a_Modifiers The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeModifiers(Modifiers a_Modifiers);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the modifiers of this symbol.
    ///
    /// \return The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE Modifiers getModifiers() const
    {
        return m_Modifiers;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if given modifiers match this symbol's modifiers.
    ///
    /// \param  a_Modifiers   The modifiers.
    ///
    /// \return true if the test passes, false if the test fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE bool testModifiers(Modifiers a_Modifiers) const
    {
        return ((m_Modifiers & a_Modifiers) == a_Modifiers);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the name of this symbol.
    ///
    /// \return The name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE StringView getName() const
    {
        return m_strName;
    }

    void setName(StringView a_strName);

    hash64 getHash() const;

    bool hasElementWithName(StringView a_strName) const;

    virtual bool isPOD() const
    {
        return false;
    }

    PHANTOM_FORCEINLINE bool isStatic() const
    {
        return ((m_Modifiers & PHANTOM_R_STATIC) == PHANTOM_R_STATIC);
    }
    PHANTOM_FORCEINLINE bool isProtected() const
    {
        return m_eAccess == Access::Protected;
    }
    PHANTOM_FORCEINLINE bool isPrivate() const
    {
        return m_eAccess == Access::Private;
    }
    PHANTOM_FORCEINLINE bool isPublic() const
    {
        return m_eAccess == Access::Public;
    }
    PHANTOM_FORCEINLINE bool isAbstract() const
    {
        return ((m_Modifiers & PHANTOM_R_ABSTRACT) == PHANTOM_R_ABSTRACT);
    }
    PHANTOM_FORCEINLINE bool isFinal() const
    {
        return ((m_Modifiers & PHANTOM_R_FINAL) == PHANTOM_R_FINAL);
    }
    PHANTOM_FORCEINLINE bool isPureVirtual() const
    {
        return ((m_Modifiers & PHANTOM_R_PURE_VIRTUAL) == PHANTOM_R_PURE_VIRTUAL);
    }
    PHANTOM_FORCEINLINE bool isVirtual() const
    {
        return ((m_Modifiers & PHANTOM_R_VIRTUAL) == PHANTOM_R_VIRTUAL);
    }
    PHANTOM_FORCEINLINE bool isConst() const
    {
        return ((m_Modifiers & PHANTOM_R_CONST) == PHANTOM_R_CONST);
    }
    PHANTOM_FORCEINLINE bool isMutable() const
    {
        return ((m_Modifiers & PHANTOM_R_MUTABLE) == PHANTOM_R_MUTABLE);
    }
    PHANTOM_FORCEINLINE bool isImportable() const
    {
        return ((m_Modifiers & PHANTOM_R_FLAG_IMPORTABLE) == PHANTOM_R_FLAG_IMPORTABLE);
    }
    PHANTOM_FORCEINLINE bool isTransient() const
    {
        return ((m_Modifiers & PHANTOM_R_TRANSIENT) == PHANTOM_R_TRANSIENT);
    }

    virtual bool hasPublicMember(Symbol* a_pSymbol) const;
    virtual bool hasProtectedMember(Symbol* a_pSymbol) const;
    virtual bool hasPrivateMember(Symbol* a_pSymbol) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Replace whole meta data with given list.
    ///
    /// \param  a_MetaDatas     The meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setMetaDatas(const MetaDatas& a_MetaDatas);
    void setMetaDatas(MetaDatas&& a_MetaDatas);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds new meta data from the given list.
    ///
    /// \param  a_MetaDatas     The meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addMetaDatas(const MetaDatas& a_MetaDatas);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add / Set a meta data with a given name and value.
    ///
    /// \param  a_strName   The meta data name (can match 'meta' or 'child0.child1...childN.meta').
    /// \param  a_Variant   The meta data value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setMetaData(StringView a_Name, const Variant& a_Value);
    void setMetaData(StringView a_Name, Variant&& a_Value);

    void setMetaData(StringWithHash a_Hash, const Variant& a_Value);
    void setMetaData(StringWithHash a_Hash, Variant&& a_Value);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes the meta data matching the given name.
    ///
    /// \param  a_strName   The meta data name (can match 'meta' or 'child0.child1...childN.meta').
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeMetaData(StringView a_Name);
    void removeMetaData(StringWithHash a_NameHash);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a meta data from its name.
    ///
    /// \param  a_strName   The meta data name (can match 'meta' or 'child0.child1...childN.meta').
    ///
    /// \return The meta data value as String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Variant& getMetaData(StringView a_Name) const;
    const Variant& getMetaData(StringWithHash a_Name) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this symbol has meta data with given name.
    ///
    /// \param  a_strName   The meta data name (can match 'meta' or 'child0.child1...childN.meta').
    ///
    /// \return True is a meta data has been found, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasMetaData(StringView a_strName) const;
    bool hasMetaData(StringWithHash a_Hash) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of meta data.
    ///
    /// \param  a_MetaDatas [in,out] The meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const MetaDatas& getMetaDatas() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this symbol has the given annotation.
    ///
    /// \param  a_strName   The annotation string
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasAnnotation(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an annotation to this symbol.
    ///
    /// \param  a_strName   The annotation string
    /// \return  true if annotation did not exist before
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addAnnotation(StringView a_strName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes an annotation from this symbol.
    ///
    /// \param  a_strName   The annotation string
    /// \return  true if annotation has been removed (existed)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool removeAnnotation(StringView a_strName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of meta data.
    ///
    /// \param  a_MetaDatas [in,out] The meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Annotations& getAnnotations() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of extensions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SymbolExtension* getExtension(Class* a_pClass, size_t a_Num = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of extensions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class SymbolExtensionClass>
    SymbolExtensionClass* getExtension(size_t a_Num = 0) const
    {
        return static_cast<SymbolExtensionClass*>(getExtension((Class*)PHANTOM_TYPEOF(SymbolExtensionClass), a_Num));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of extensions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const SymbolExtensions& getExtensions() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add extensions.
    ///
    /// \param  a_Extensions The extensions to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addExtensions(const SymbolExtensions& a_Extensions);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add an extension.
    ///
    /// \param  a_pExtension The extension to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addExtension(SymbolExtension* a_pExtension);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Remove an extension.
    ///
    /// \param  a_pExtension The extension to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeExtension(SymbolExtension* a_pExtension);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the doubles of this named elements (i.e. equivalent element registered in other
    /// sources from multiple template instance registration in different modules).
    ///
    /// \return the meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void         getDoubles(Symbols& out) const;
    bool         isSame(LanguageElement* a_pOther) const override;
    virtual bool isSame(Symbol* a_pOther) const;
    Scope*       getNamingScope() const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the namespace enclosing this symbol.
    ///
    /// \return null if it fails, else the namespace.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getNamespace() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template decoration constituted from unique names (see getUniqueName()).
    ///
    /// \return The unique template decoration.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getUniqueTemplateDecoration(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template decoration constituted from decorated names (see
    /// getDecoratedName()).
    ///
    /// \return The template decoration.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getTemplateDecoration(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template decoration constituted from qualified decorated names (see
    /// getQualifiedDecoratedName()).
    ///
    /// \return The template decoration.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getTemplateQualifiedDecoration(StringBuffer& a_Buf) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template specialization associated with this symbol, if exists.
    ///
    /// \return null if no template specialization, else the template specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getTemplateSpecialization() const
    {
        return m_pOwner ? m_pOwner->asTemplateSpecialization() : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Associates a user data to this class (for quicker custom data access than symbol
    /// extensions or meta datas).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setUserData(UserData&& a_UserData);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the user data back reinterpret casted in the given type
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T* getUserDataAs()
    {
        return reinterpret_cast<T*>(m_UserData.getData());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the user data back reinterpret casted in the given type
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T const* getUserDataAs() const
    {
        return reinterpret_cast<T const*>(m_UserData.getData());
    }

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getUniqueName;

    void getName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

protected:
    PHANTOM_FORCEINLINE void setImportable(bool a_bValue)
    {
        if (a_bValue)
            m_uiFlags |= PHANTOM_R_FLAG_IMPORTABLE;
        else
            m_uiFlags &= ~PHANTOM_R_FLAG_IMPORTABLE;
    }
    virtual hash64 computeHash() const;
    static hash64  ComputeHash(const char* a_Str, size_t a_Len);
    void           onElementRemoved(LanguageElement* a_pElement) override;
    virtual void   formatAnonymousName(StringBuffer& a_Buf) const;
    void           onAncestorChanged(LanguageElement* a_pAncestor) override;

private:
    hash64 _computeHash() const;

protected:
    String     m_strName;
    Namespace* m_pNamespace = nullptr; /// Namespace represents the abstract, qualifying container of the element, for
                                       /// example a global function is scoped inside the global namespace
    MetaDatas*        m_pMetaDatas = nullptr;
    Annotations*      m_pAnnotations = nullptr;
    SymbolExtensions* m_pExtensions = nullptr;
    Modifiers         m_Modifiers = PHANTOM_R_NONE;
    mutable hash64    m_Hash = 0;
    Access            m_eAccess = Access::Undefined;
    UserData          m_UserData;
};

} // namespace lang
} // namespace phantom

//#include "Symbol.hxx"
