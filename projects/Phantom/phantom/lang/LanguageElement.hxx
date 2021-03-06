#pragma once

// haunt {

#include "LanguageElement.h"

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
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallSet.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("LanguageElement")

        PHANTOM_CLASS_T((class), (t_Scope), Members)
        {
            this_()
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Ellipsis)
        {
            this_()
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<PlaceholderMap>("PlaceholderMap"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ModuleSet>("ModuleSet"); }
        PHANTOM_CLASS(LanguageElement)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()
            .inherits<::phantom::Object>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, VisitorData), virtual_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .typedef_<SymbolFilter>("SymbolFilter")
            .staticMethod<Symbol*(Symbol*, bool)>("NoFilter", &_::NoFilter)
            .staticMethod<Symbol*(Symbol*, bool)>("PublicFilter", &_::PublicFilter)
            .staticMethod<Symbol*(Symbol*, bool)>("PublicIfUnamedSubSymbolFilter", &_::PublicIfUnamedSubSymbolFilter)
        
        .public_()
            .method<LanguageElements const&() const>("getElements", &_::getElements)
            .method<LanguageElements const&() const>("getReferencedElements", &_::getReferencedElements)
            .method<LanguageElements const&() const>("getReferencingElements", &_::getReferencingElements)
            .method<bool() const>("isTemplateElement", &_::isTemplateElement)
            .method<bool(LanguageElement*) const>("hasReferencedElement", &_::hasReferencedElement)
            .method<bool(LanguageElement*) const>("hasReferencingElement", &_::hasReferencingElement)
            .method<Type*() const, virtual_>("asAddressType", &_::asAddressType)
            .method<Alias*() const, virtual_>("asAlias", &_::asAlias)
            .method<AnonymousSection*() const, virtual_>("asAnonymousSection", &_::asAnonymousSection)
            .method<AnonymousStruct*() const, virtual_>("asAnonymousStruct", &_::asAnonymousStruct)
            .method<AnonymousUnion*() const, virtual_>("asAnonymousUnion", &_::asAnonymousUnion)
            .method<PrimitiveType*() const, virtual_>("asArithmeticType", &_::asArithmeticType)
            .method<Array*() const, virtual_>("asArray", &_::asArray)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Block*() const, virtual_>("asBlock", &_::asBlock)
            .method<Class*() const, virtual_>("asClass", &_::asClass)
            .method<Pointer*() const, virtual_>("asClassPointer", &_::asClassPointer)
            .method<LValueReference*() const, virtual_>("asClassLValueReference", &_::asClassLValueReference)
            .method<RValueReference*() const, virtual_>("asClassRValueReference", &_::asClassRValueReference)
            .method<Type*() const, virtual_>("asClassAddressType", &_::asClassAddressType)
            .method<ClassType*() const, virtual_>("asClassType", &_::asClassType)
            .method<Constant*() const, virtual_>("asConstant", &_::asConstant)
            .method<ConstType*() const, virtual_>("asConstClass", &_::asConstClass)
            .method<Pointer*() const, virtual_>("asConstClassPointer", &_::asConstClassPointer)
            .method<LValueReference*() const, virtual_>("asConstClassLValueReference", &_::asConstClassLValueReference)
            .method<RValueReference*() const, virtual_>("asConstClassRValueReference", &_::asConstClassRValueReference)
            .method<Type*() const, virtual_>("asConstClassAddressType", &_::asConstClassAddressType)
            .method<Constructor*() const, virtual_>("asConstructor", &_::asConstructor)
            .method<ConstType*() const, virtual_>("asConstType", &_::asConstType)
            .method<Reference*() const>("asConstLValueReference", &_::asConstLValueReference)
            .method<ConstVolatileType*() const, virtual_>("asConstVolatileType", &_::asConstVolatileType)
            .method<ContainerClass*() const, virtual_>("asContainerClass", &_::asContainerClass)
            .method<DataElement*() const, virtual_>("asDataElement", &_::asDataElement)
            .method<FieldPointer*() const, virtual_>("asFieldPointer", &_::asFieldPointer)
            .method<Pointer*() const, virtual_>("asPointer", &_::asPointer)
            .method<Destructor*() const, virtual_>("asDestructor", &_::asDestructor)
            .method<Ellipsis*() const, virtual_>("asEllipsis", &_::asEllipsis)
            .method<Enum*() const, virtual_>("asEnum", &_::asEnum)
            /// missing symbol(s) reflection (phantom::lang::Evaluable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Evaluable*() const, virtual_>("asEvaluable", &_::asEvaluable)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*() const, virtual_>("asExpression", &_::asExpression)
            .method<Field*() const, virtual_>("asField", &_::asField)
            .method<PrimitiveType*() const, virtual_>("asFloatingPointType", &_::asFloatingPointType)
            .method<Function*() const, virtual_>("asFunction", &_::asFunction)
            .method<FunctionPointer*() const, virtual_>("asFunctionPointer", &_::asFunctionPointer)
            .method<FunctionType*() const, virtual_>("asFunctionType", &_::asFunctionType)
            .method<PrimitiveType*() const, virtual_>("asFundamentalType", &_::asFundamentalType)
            .method<InitializerListType*() const, virtual_>("asInitializerListType", &_::asInitializerListType)
            .method<PrimitiveType*() const, virtual_>("asIntegralType", &_::asIntegralType)
            .method<LanguageElement*() const>("asLanguageElement", &_::asLanguageElement)
            .method<LocalVariable*() const, virtual_>("asLocalVariable", &_::asLocalVariable)
            .method<LValueReference*() const, virtual_>("asLValueReference", &_::asLValueReference)
            .method<MapClass*() const, virtual_>("asMapContainerClass", &_::asMapContainerClass)
            .method<MemberAnonymousSection*() const, virtual_>("asMemberAnonymousSection", &_::asMemberAnonymousSection)
            .method<MemberAnonymousStruct*() const, virtual_>("asMemberAnonymousStruct", &_::asMemberAnonymousStruct)
            .method<MemberAnonymousUnion*() const, virtual_>("asMemberAnonymousUnion", &_::asMemberAnonymousUnion)
            .method<MethodPointer*() const, virtual_>("asMethodPointer", &_::asMethodPointer)
            .method<MemberPointer*() const, virtual_>("asMemberPointer", &_::asMemberPointer)
            .method<Method*() const, virtual_>("asMethod", &_::asMethod)
            .method<Module*() const, virtual_>("asModule", &_::asModule)
            .method<Symbol*() const, virtual_>("asSymbol", &_::asSymbol)
            .method<Namespace*() const, virtual_>("asNamespace", &_::asNamespace)
            .method<Type*() const, virtual_>("asNullptrType", &_::asNullptrType)
            .method<Package*() const, virtual_>("asPackage", &_::asPackage)
            .method<PackageFolder*() const, virtual_>("asPackageFolder", &_::asPackageFolder)
            .method<Parameter*() const, virtual_>("asParameter", &_::asParameter)
            .method<Placeholder*() const, virtual_>("asPlaceholder", &_::asPlaceholder)
            .method<Type*() const, virtual_>("asPOD", &_::asPOD)
            .method<PointerType*() const, virtual_>("asPointerType", &_::asPointerType)
            .method<ExtendedType*() const, virtual_>("asExtendedType", &_::asExtendedType)
            .method<PrimitiveType*() const, virtual_>("asPrimitiveType", &_::asPrimitiveType)
            .method<Property*() const, virtual_>("asProperty", &_::asProperty)
            .method<Reference*() const, virtual_>("asReference", &_::asReference)
            .method<RValueReference*() const, virtual_>("asRValueReference", &_::asRValueReference)
            .method<Scope*() const, virtual_>("asScope", &_::asScope)
            .method<SequentialContainerClass*() const, virtual_>("asSequentialContainerClass", &_::asSequentialContainerClass)
            .method<SetClass*() const, virtual_>("asSetContainerClass", &_::asSetContainerClass)
            .method<Signal*() const, virtual_>("asSignal", &_::asSignal)
            .method<Signature*() const, virtual_>("asSignature", &_::asSignature)
            .method<Method*() const, virtual_>("asSlot", &_::asSlot)
            .method<Source*() const, virtual_>("asSource", &_::asSource)
            /// missing symbol(s) reflection (phantom::lang::Statement) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Statement*() const, virtual_>("asStatement", &_::asStatement)
            .method<StaticField*() const, virtual_>("asStaticField", &_::asStaticField)
            .method<StaticMethod*() const, virtual_>("asStaticMethod", &_::asStaticMethod)
            .method<Structure*() const, virtual_>("asStructure", &_::asStructure)
            .method<Subroutine*() const, virtual_>("asSubroutine", &_::asSubroutine)
            .method<Template*() const, virtual_>("asTemplate", &_::asTemplate)
            .method<TemplateParameter*() const, virtual_>("asTemplateParameter", &_::asTemplateParameter)
            .method<TemplateSignature*() const, virtual_>("asTemplateSignature", &_::asTemplateSignature)
            .method<TemplateSpecialization*() const, virtual_>("asTemplateSpecialization", &_::asTemplateSpecialization)
            .method<Type*() const, virtual_>("asType", &_::asType)
            .method<Union*() const, virtual_>("asUnion", &_::asUnion)
            .method<ValueMember*() const, virtual_>("asValueMember", &_::asValueMember)
            .method<Variable*() const, virtual_>("asVariable", &_::asVariable)
            .method<VirtualMethodTable*() const, virtual_>("asVirtualMethodTable", &_::asVirtualMethodTable)
            .method<VolatileType*() const, virtual_>("asVolatileType", &_::asVolatileType)
            .method<Type*() const, virtual_>("toType", &_::toType)
            .method<bool(Symbol*) const, virtual_>("hasFriend", &_::hasFriend)
            .method<bool(LanguageElement*) const>("hasElement", &_::hasElement)
            .method<bool(Symbol*) const>("hasFriendCascade", &_::hasFriendCascade)
            .method<bool(LanguageElement*) const>("hasOwnerCascade", &_::hasOwnerCascade)
            .method<bool(LanguageElement*) const>("hasElementCascade", &_::hasElementCascade)
            .method<bool() const, virtual_>("isCompileTime", &_::isCompileTime)
            .method<LanguageElement*() const>("getOwner", &_::getOwner)
            .method<LanguageElement*() const>("getRootElement", &_::getRootElement)
            .method<void(LanguageElements&, Class*) const>("fetchElementsDeep", &_::fetchElementsDeep)["nullptr"]
            .method<void(LanguageElements&, Class*) const>("fetchElements", &_::fetchElements)["nullptr"]
            .method<void(StringView, Symbols&) const>("getSymbolsWithName", &_::getSymbolsWithName)
            .method<void(Symbols&) const>("getSymbols", &_::getSymbols)
            .method<bool(StringView) const>("hasSymbol", &_::hasSymbol)
            .method<Module*() const>("getModule", &_::getModule)
            .method<Package*() const>("getPackage", &_::getPackage)
            .method<Source*() const>("getSource", &_::getSource)
            .method<bool() const, virtual_>("canBeDestroyed", &_::canBeDestroyed)
            .method<bool() const, virtual_>("canBeUnloaded", &_::canBeUnloaded)
            .method<void() const, virtual_>("checkCompleteness", &_::checkCompleteness)
            .method<bool(LanguageElement*) const, virtual_>("isSame", &_::isSame)
            .method<LanguageElement*(), virtual_>("hatchExpression", &_::hatchExpression)
            .method<LanguageElement*() const, virtual_>("removeExpression", &_::removeExpression)
            .method<Symbol*() const>("removeExpressionAsSymbol", &_::removeExpressionAsSymbol)
            .method<ClassType*() const>("getEnclosingClassType", &_::getEnclosingClassType)
            /// missing symbol(s) reflection (phantom::lang::Statement) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Statement*() const>("getEnclosingStatement", &_::getEnclosingStatement)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Block*() const>("getEnclosingBlock", &_::getEnclosingBlock)
            .method<Scope*() const>("getEnclosingScope", &_::getEnclosingScope)
            .method<Namespace*() const>("getEnclosingNamespace", &_::getEnclosingNamespace)
            .method<Subroutine*() const>("getEnclosingSubroutine", &_::getEnclosingSubroutine)
            .method<TemplateSpecialization*() const>("getEnclosingTemplateSpecialization", &_::getEnclosingTemplateSpecialization)
            .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_>("partialAccepts", &_::partialAccepts)
            .method<void(LanguageElement*)>("addReferencedElement", &_::addReferencedElement)
            .method<void(LanguageElement*)>("addUniquelyReferencedElement", &_::addUniquelyReferencedElement)
            .method<void(LanguageElement*)>("removeReferencedElement", &_::removeReferencedElement)
            .method<void(LanguageElement*)>("addScopedElement", &_::addScopedElement)
            .method<void(LanguageElement*)>("removeScopedElement", &_::removeScopedElement)
            .method<void(ModuleSet&) const>("fetchReferencedModules", &_::fetchReferencedModules)
            .method<void(ModuleSet&) const>("fetchReferencingModules", &_::fetchReferencingModules)
            .method<void(ModuleSet&) const>("fetchReferencedModulesDeep", &_::fetchReferencedModulesDeep)
            .method<void(ModuleSet&) const>("fetchReferencingModulesDeep", &_::fetchReferencingModulesDeep)
            .method<void(LanguageElements&) const>("getElements", &_::getElements)
            .method<void(Symbols&) const>("fetchSymbols", &_::fetchSymbols)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Symbols&, SymbolFilter, bool, bool) const>("fetchSymbols", &_::fetchSymbols)["true"]["false"]
            .method<void(LanguageElements&) const>("getElementsDeep", &_::getElementsDeep)
            .method<void(LanguageElement*)>("addElement", &_::addElement)
            .method<void(Symbol*)>("addSymbol", &_::addSymbol)
            .method<void(LanguageElement*)>("removeElement", &_::removeElement)
            .method<Symbol*(StringView, Modifiers, uint) const>("getUniqueElement", &_::getUniqueElement)["0"]["0"]
            .method<size_t(LanguageElement*) const>("getElementIndex", &_::getElementIndex)
            /// missing symbol(s) reflection (std::basic_ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::basic_ostream<char> &) const>("dumpElementList", &_::dumpElementList)
            /// missing symbol(s) reflection (std::basic_ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::basic_ostream<char> &) const>("dumpElementListCascade", &_::dumpElementListCascade)
            .method<String() const, virtual_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<String() const, virtual_>("getQualifiedName", &_::getQualifiedName)
            .method<String() const, virtual_>("getDecoratedName", &_::getDecoratedName)
            .method<String() const>("getName", &_::getName)
            .method<String() const>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_>("getName", &_::getName)
            .method<void(StringBuffer&) const, virtual_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_>("getUniqueName", &_::getUniqueName)
            .method<Scope*() const, virtual_>("getNamingScope", &_::getNamingScope)
            .method<bool(Scope*) const>("hasNamingScopeCascade", &_::hasNamingScopeCascade)
            .method<void()>("clear", &_::clear)
            .method<void(LanguageElement*)>("steal", &_::steal)
            .method<void()>("detach", &_::detach)
            .method<bool(Symbol*) const, virtual_>("isSymbolHidden", &_::isSymbolHidden)
            .method<uint() const>("getFlags", &_::getFlags)
            .method<void(uint)>("setFlag", &_::setFlag)
            .method<void(uint)>("addFlags", &_::addFlags)
            .method<void(uint)>("removeFlags", &_::removeFlags)
            .method<void(uint)>("setFlags", &_::setFlags)
            .method<bool(uint) const>("testFlags", &_::testFlags)
            .method<bool() const>("isNative", &_::isNative)
            .method<bool() const>("isInvalid", &_::isInvalid)
            .method<bool() const>("isTemplateDependant", &_::isTemplateDependant)
            .method<void()>("setTemplateDependant", &_::setTemplateDependant)
            .method<void()>("setInvalid", &_::setInvalid)
            .method<bool() const>("isIncomplete", &_::isIncomplete)
            .method<void()>("setIncomplete", &_::setIncomplete)
            .method<bool() const>("isAlwaysValid", &_::isAlwaysValid)
            .method<bool() const, virtual_>("isTemplateInstance", &_::isTemplateInstance)
            .method<void()>("setShared", &_::setShared)
            .method<bool() const>("isShared", &_::isShared)
            .method<void(const CodePosition&)>("setCodePosition", &_::setCodePosition)
            .method<const CodeRange&() const>("getCodeRange", &_::getCodeRange)
            .method<void(const CodeRange&)>("setCodeRange", &_::setCodeRange)
            .method<const CodePosition&() const>("getCodePosition", &_::getCodePosition)
            .method<CodeLocation() const>("getCodeLocation", &_::getCodeLocation)
            .method<CodeRangeLocation() const>("getCodeRangeLocation", &_::getCodeRangeLocation)
            .method<Source*() const, virtual_>("getCodeLocationSource", &_::getCodeLocationSource)
            .method<LanguageElement*(const CodePosition&) const>("getElementAtCodePosition", &_::getElementAtCodePosition)
            .method<LanguageElement*(uint16) const>("getElementAtLine", &_::getElementAtLine)
            .method<int() const, virtual_>("destructionPriority", &_::destructionPriority)
        
        .protected_()
            .constructor<void(uint)>()["0"]
        
        .protected_()
            .method<void(LanguageElement*), virtual_>("onAncestorChanged", &_::onAncestorChanged)
            .method<void(LanguageElement*), virtual_>("onAncestorAboutToBeChanged", &_::onAncestorAboutToBeChanged)
            .method<void(LanguageElement*), virtual_>("onElementAdded", &_::onElementAdded)
            .method<void(LanguageElement*), virtual_>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*), virtual_>("onReferencedElementAdded", &_::onReferencedElementAdded)
            .method<void(LanguageElement*), virtual_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(), virtual_>("onInvalidated", &_::onInvalidated)
            .method<void(), virtual_>("onElementsAccess", &_::onElementsAccess)
        
        .protected_()
            .method<void()>("_onElementsAccess", &_::_onElementsAccess)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElement")
PHANTOM_END("phantom.lang")
}
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("LanguageElement")

        PHANTOM_STRUCT_T((class, bool), (T, Is), ConstructorIfLanguageElement)
        {
            this_()
            .PHANTOM_T inherits</* ::phantom::Constructor<T, ConstructorOverloadTag::Enum(ConstructorOverloadTag::MetaElement - 1)> */>()
            .PHANTOM_T staticMethod<void(T*)>("destroy", &_::destroy)
            ;
        }
        PHANTOM_STRUCT_TS((class), (T), (T, false), ConstructorIfLanguageElement)
        {
            this_()
            .PHANTOM_T inherits</* ::phantom::Constructor<T, ConstructorOverloadTag::Enum(ConstructorOverloadTag::MetaElement - 1)> */>()
            ;
        }
        /// missing symbol(s) reflection (phantom::Constructor) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class), (T), (T, ConstructorOverloadTag::MetaElement), Constructor)
        {
            this_()
            // .PHANTOM_T inherits<::phantom::ConstructorIfLanguageElement<T, std::is_base_of<lang::LanguageElement, T>::value>>()
            ;
        } */
    PHANTOM_END("LanguageElement")
PHANTOM_END("phantom.lang")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
