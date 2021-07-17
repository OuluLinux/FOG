//
// 	  Title:			Standard include file for Flexible Object Generator
//
// 	  File Name:		Fog.h
//
// 	  Author:			E.D.Willink
// END
//
#ifndef FOG_H
#define FOG_H

#ifdef _MSC_VER
	#pragma warning(disable:4355)   //  this in base list
#endif

class Fog;
PRIM_WITH_REF(FogAccess);
class FogActualDeclaration;
class FogActualScopeContext;
class FogAnonId;
class FogArrayModifier;
class FogArrayOfExpr;
PRIM_WITH_LIST_MAP(FogBaseClass)
PRIM_WITH_LIST_MAP(FogBaseFunction)
class FogBaseMakeSpecifierContext;
PRIM_WITH_LIST(FogBaseSpecifier)
PRIM_WITH_REF(FogBaseSpecifiers)
class FogBoolExpr;
PRIM_WITH_LIST(FogBrace)
PRIM_WITH_REF(FogBuiltInScope)
PRIM_WITH_LIST_MAP(FogBuiltInTypeId)
PRIM_WITH_REF(FogBuiltInTypeIds)
class FogCallContext;
class FogCallExpr;
PRIM_WITH_LIST(FogCatchStatement)
PRIM_WITH_MAP(FogCharacter)
class FogCharacterExpr;
PRIM_WITH_LIST_MAP(FogClass)
class FogClassSpecifier;
PRIM_WITH_LIST(FogCode)
class FogCodeContext;
PRIM_WITH_REF(FogCodeTokens);
PRIM_WITH_LIST_MAP(FogCtorInit)
PRIM_WITH_REF(FogCv);
PRIM_WITH_REF(FogDeclSpecifiedName);
PRIM_WITH_REF(FogDeclSpecifier);
class FogDeclSpecifierValue;
PRIM_WITH_REF(FogDecoratedName);
PRIM_WITH_LIST_MAP(FogDependency)
class FogDeleteExpr;
class FogDerivable;
class FogDerivedClause;
class FogDestructorId;
class FogElaboratedTypeSpecifier;
class FogElementType;
class FogEmitContext;
PRIM_WITH_LIST_MAP(FogEntity)
class FogEntityFinder;
class FogEntityFinding;
PRIM_WITH_REF(FogEntityName);
PRIM_WITH_LIST_MAP(FogEnum)
class FogEnumSpecifier;
PRIM_WITH_LIST_MAP(FogEnumerator)
PRIM_WITH_LIST(FogEnumeratorSpecifier)
PRIM_WITH_REF(FogEnumeratorSpecifiers);
PRIM_WITH_LIST_MAP(FogException)
PRIM_WITH_LIST(FogExpr)
PRIM_WITH_REF(FogExprs)
class FogFailure;
PRIM_WITH_LIST(FogFileId)
PRIM_WITH_REF(FogFileIds);
class FogFileIdent;
class FogFileManager;
PRIM_WITH_LIST_MAP(FogFilespace)
PRIM_WITH_LIST_MAP(FogFunction)
class FogFunctionModifier;
class FogFunctionSpecifier;
class FogIdentifier;
class FogIndexCastExpr;
class FogIndexExpr;
PRIM_WITH_REF(FogInline)
PRIM_WITH_REF(FogIterationDomain);
PRIM_WITH_LIST_MAP(FogKeyword)
class FogLazyBool;
class FogLexerContext;
class FogLexerHash;
class FogLexerInput;
struct FogLexerState;
class FogLexerTower;
PRIM_WITH_REF(FogLine)
class FogLinkage;
class FogLinkageSpecifier;
PRIM_WITH_REF(FogListOfExpr)
PRIM_WITH_LIST_MAP(FogMacro)
class FogMakeEntityContext;
class FogMakeSpecifierContext;
class FogMakeTemplateContext;
class FogMakerContext;
class FogMerge;
class FogMergeContext;
PRIM_WITH_LIST_MAP(FogMetaArgument)
class FogMetaBuiltInFunction;
PRIM_WITH_MAP(FogMetaEntity)
class FogMetaFunction;
class FogMetaFunctionEntity;
class FogMetaFunctionExpr;
class FogMetaFunctionSlot;
class FogMetaFunctionSpecifier;
class FogMetaFunctor;
class FogMetaIterator;
PRIM_WITH_REF(FogMetaIteratorEntity)
PRIM_WITH_REF(FogMetaMethod)
class FogMetaObjectSpecifier;
PRIM_WITH_LIST(FogMetaParameterSpecifier);
class FogMetaScopeContext;
PRIM_WITH_LIST_MAP(FogMetaSlot)
class FogMetaSlotFinder;
class FogMetaSlotFinding;
PRIM_WITH_LIST_MAP(FogMetaType);
class FogMetaVariable;
class FogMetaVariableEntity;
class FogMetaVariableSlot;
class FogMetaVariableSpecifier;
PRIM_WITH_LIST(FogModifier)
PRIM_WITH_REF(FogMultipleId)
PRIM_WITH_REF(FogName)
PRIM_WITH_LIST_MAP(FogNamespace)
PRIM_WITH_LIST_MAP(FogNamespaceAlias)
class FogNestedMakeEntityContext;
class FogNestedName;
class FogNestedScopeContext;
class FogNewExpr;
PRIM_WITH_REF(FogNumber)
PRIM_WITH_LIST(FogObjectSpecifier);
class FogOfUseBy;
PRIM_WITH_LIST(FogParameterSpecifier);
PRIM_WITH_REF(FogParenthesisExpr);
class FogParserBase;
class FogParseContext;
class FogParser;
union FogParserValue;
PRIM_WITH_REF(FogPointerModifier)
PRIM_WITH_REF(FogPosition);
PRIM_WITH_LIST(FogPotentialDeclaration)
PRIM_WITH_LIST(FogRaw)
PRIM_WITH_LIST(FogRawCtorInit)
PRIM_WITH_REF(FogRawCtorInits)
class FogRawDeclarations;
class FogRawStatements;
PRIM_WITH_REF(FogRawSubspace);
PRIM_WITH_REF(FogRawTemplate);
class FogRaws;
class FogReader;
class FogReferenceModifier;
PRIM_WITH_REF(FogRoot);
PRIM_WITH_LIST_MAP(FogScope)
class FogScopeContext;
PRIM_WITH_REF(FogScopeSpecifier);
PRIM_WITH_REF(FogSegment);
class FogSemantics;
class FogSemanticsContext;
class FogSingleId;
PRIM_WITH_LIST_MAP(FogSourceFile)
class FogSourceFileType;
PRIM_WITH_LIST(FogSpacing)
PRIM_WITH_LIST(FogSpecifier);
class FogStatement;
PRIM_WITH_REF(FogStatic);
class FogStaticScopeContext;
class FogStream;
PRIM_WITH_MAP(FogString)
PRIM_WITH_REF(FogTag);
PRIM_WITH_LIST_MAP(FogTargetFile)
PRIM_WITH_LIST(FogTemplateArg)
PRIM_WITH_REF(FogTemplateArgs)
class FogTemplateManager;
PRIM_WITH_LIST(FogTemplateParameter)
PRIM_WITH_LIST_MAP(FogTemplateParameterSpecifier)
PRIM_WITH_MAP(FogTemplateParameterSpecifierId)
PRIM_WITH_LIST(FogTemplateParameterSpecifiers)
class FogTemplatedName;
class FogTemplatedParameterSpecifier;
PRIM_WITH_LIST(FogToken)
class FogTokenContext;
PRIM_WITH_REF(FogTokenStatements);
class FogTokenType;
class FogTokenTypeEnum;
PRIM_WITH_REF(FogTokens);
class FogTreeCallExpr;
PRIM_WITH_MAP(FogTypeName)
class FogTypeParameterSpecifier;
class FogTypeSpecifier;
class FogTypedName;
PRIM_WITH_LIST_MAP(FogTypedef)
class FogTypedefSpecifier;
PRIM_WITH_LIST_MAP(FogUnique)
PRIM_WITH_LIST_MAP(FogUsage)
class FogUsageContext;
PRIM_WITH_LIST(FogUsageManager)
PRIM_WITH_MAP(FogUsing)
PRIM_WITH_LIST(FogUsingSpecifier)
class FogUsingStyle;
PRIM_WITH_REF(FogUtility);
class FogValueParameterSpecifier;
PRIM_WITH_LIST_MAP(FogVariable)
class FogVariableId;
class FogVariableSpecifier;
PRIM_WITH_REF(FogVirtual)
PRIM_WITH_REF(FogWordName);

typedef FogScopeContext FogCompileContext;
typedef FogMetaScopeContext FogMetaCompileContext;
typedef FogNestedScopeContext FogNestedCompileContext;
typedef FogStaticScopeContext FogStaticCompileContext;
typedef FogScopeContext FogDeriveContext;
typedef FogMetaScopeContext FogMetaDeriveContext;
typedef FogNestedScopeContext FogNestedDeriveContext;
typedef FogStaticScopeContext FogStaticDeriveContext;
typedef FogScopeContext FogMakeActualContext;  //  Place holder till code converted.
typedef FogScopeContext FogInstallContext;   //  Place holder till code converted.

typedef FogToken FogHandler;
typedef FogTokens FogHandlers;
typedef FogCode FogTryBlock;

typedef PrimSetElement<FogEntityRef> FogEntitySetOfRefElement;
typedef PrimMapOfRef<FogEntitySetOfRefElement> FogEntitySetOfRef;
typedef PrimConstMapOfRefIterator<FogEntitySetOfRefElement> FogEntityConstSetOfRefIterator;
typedef PrimConstMapOfRefToConstIterator<FogEntitySetOfRefElement> FogEntityConstSetOfRefToConstIterator;
typedef PrimMapOfRefIterator<FogEntitySetOfRefElement> FogEntitySetOfRefIterator;
typedef PrimMapOfRefToConstIterator<FogEntitySetOfRefElement> FogEntitySetOfRefToConstIterator;

typedef PrimSetElem<FogEntityListOfRef> FogEntitySetOfListOfRefElement;
typedef PrimMapOfRef<FogEntitySetOfListOfRefElement> FogEntitySetOfListOfRef;
typedef PrimConstMapOfRefToConstIterator<FogEntitySetOfListOfRefElement> FogEntityConstSetOfListOfRefToConstIterator;

#endif
