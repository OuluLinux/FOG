#include <Prim/PrimIncludeAll.h>

#define VERBOSE(x)

#include "config.h"

#include "Fog.h"
#include "FogMerge.h"
#include "FogScopeEnums.h"
#include "FogTokenType.h"
#include "FogToken.h"
#include "FogRaw.h"
#include "FogExpr.h"
#include "FogName.h"
#include "FogEntityName.h"
#include "FogWordName.h"
#include "FogKeyword.h"
#include "FogReservedId.h"
#include "FogEnumHandle.h"
#include "FogDeclSpecifierEnums.h"
#include "FogDeclSpecifierValue.h"
#include "FogDeclSpecifier.h"
#include "FogModifier.h"
#include "FogDecoratedName.h"
#include "FogIdentifier.h"
#include "FogUtility.h"
#include "FogSpecifier.h"
#include "FogDecoratedExpr.h"
#include "FogDyadicExpr.h"
#include "FogMetaToken.h"
#include "FogUnique.h"
#include "FogLazyBool.h"
#include "FogEntity.h"
#include "FogScope_Bases.h"
#include "FogScope_Derivations.h"
#include "FogScope_Enums.h"
#include "FogScope_Friends.h"
#include "FogScope_Functions.h"
#include "FogScope_Implementation.h"
#include "FogScope_Interface.h"
#include "FogScope_Names.h"
#include "FogScope_Nested.h"
#include "FogScope_Typedefs.h"
#include "FogScope_Types.h"
#include "FogScope_Using.h"
#include "FogScope_Variables.h"
#include "FogTag.h"
#include "FogAccess.h"
#include "FogTemplateArg.h"
#include "FogTemplateArgs.h"
#include "FogTemplateManager.h"
#include "FogScope.h"
#include "FogScopeContext.h"
#include "FogScopeSpecifier.h"
#include "FogObjectSpecifier.h"
#include "FogMetaObjectSpecifier.h"
#include "FogVirtual.h"
#include "FogStatement.h"
#include "FogTypeSpecifier.h"
#include "FogPosition.h"
#include "FogSegment.h"
#include "FogFileIds.h"
#include "FogInline.h"
#include "FogTokens.h"



#include "FogMetaSlot.h"
#include "FogMetaSlotFinder.h"
#include "FogMetaParameterSpecifier.h"
#include "Fog0.h"
#include "FogAccessibilitySpecifier.h"
#include "FogActualDeclaration.h"
#include "FogAnonId.h"
#include "FogArgumentId.h"
#include "FogArrayModifier.h"
#include "FogArrayOfExpr.h"
#include "FogAsmSpecifier.h"
#include "FogAssignExpr.h"
#include "FogAuto.h"
#include "FogBaseClass.h"
#include "FogBaseSpecifier.h"
#include "FogBaseSpecifiers.h"
#include "FogBitFieldExpr.h"
#include "FogBrace.h"
#include "FogBreakStatement.h"
#include "FogBuiltInScope.h"
#include "FogBuiltInTypeId.h"
#include "FogCallContext.h"
#include "FogCallExpr.h"
#include "FogCaseStatement.h"
#include "FogCastExpr.h"
#include "FogCatchStatement.h"
#include "FogCharacter.h"
#include "FogClass.h"
#include "FogClassSpecifier.h"
#include "FogCode.h"
#include "FogCodeTokens.h"
#include "FogCondExpr.h"
#include "FogConversionId.h"
#include "FogCtorExpr.h"
#include "FogCtorInit.h"
#include "FogCv.h"
#include "FogDeclSpecifiedName.h"
#include "FogDeclSpecifier.h"
#include "FogDeclSpecifierEnums.h"
#include "FogDeclSpecifierValue.h"
#include "FogDecoratedExpr.h"
#include "FogDecoratedName.h"
#include "FogDeleteExpr.h"
#include "FogDerivable.h"
#include "FogDerivedClause.h"
#include "FogDestructorId.h"
#include "FogDoWhileStatement.h"
#include "FogDyadicExpr.h"
#include "FogElaboratedTypeSpecifier.h"
#include "FogElementType.h"
#include "FogEllipsisExpr.h"
#include "FogEmitContext.h"
#include "FogEntityFinder.h"
#include "FogEntityName.h"
#include "FogEnum.h"
#include "FogEnumHandle.h"
#include "FogEnumSpecifier.h"
#include "FogEnumerator.h"
#include "FogEnumeratorSpecifier.h"
#include "FogEnumeratorSpecifiers.h"
#include "FogExpr.h"
#include "FogExpressionStatement.h"
#include "FogFailure.h"
#include "FogFileComparator.h"
#include "FogFileId.h"
#include "FogFileIdent.h"
#include "FogFileManager.h"
#include "FogFilespace.h"
#include "FogForStatement.h"
#include "FogFunction.h"
#include "FogFunctionExpr.h"
#include "FogFunctionModifier.h"
#include "FogFunctionSpecifier.h"
#include "FogGlobalName.h"
#include "FogGotoStatement.h"
#include "FogIdentifier.h"
#include "FogIfStatement.h"
#include "FogIndexExpr.h"
#include "FogInitExpr.h"
#include "FogIterationDomain.h"
#include "FogIteratorExpr.h"
#include "FogKeyword.h"
#include "FogLabelStatement.h"

#include "FogTokenMonitor.h"
#include "FogTokenContext.h"
#include "FogParserValue.h"
#include "FogString.h"
#include "FogLexerUnget.h"
#include "FogLexerContext.h"
#include "FogLexer.h"

#undef INITIAL

#include "FogLexerMacro.h"
#include "FogParserBase.h"
#include "FogParserMaker.h"
#include "FogParser.h"
#include "FogHashParser.h"
#include "FogSubstituteParser.h"
#include "FogReplaceParser.h"

#include "FogLexerHash.h"
#include "FogLexerCpp.h"
#include "FogLexerInput.h"
#include "FogLexerState.h"
#include "FogLexerTower.h"
#include "FogLine.h"
#include "FogLinkage.h"
#include "FogLinkageSpecifier.h"
#include "FogListOfExpr.h"
#include "FogMacro.h"
#include "FogMacroId.h"
#include "FogMakeEntityContext.h"
#include "FogMakeSemantics.h"
#include "FogMakeSpecifierContext.h"
#include "FogMakeTemplateContext.h"
#include "FogMakerContext.h"
#include "FogMemberExpr.h"
#include "FogMerge.h"
#include "FogMetaVariableSlot.h"
#include "FogMetaArgument.h"
#include "FogMetaBuiltInFunction.h"
#include "FogMetaEntity.h"
#include "FogMetaFunctionSlot.h"
#include "FogMetaFunction.h"
#include "FogMetaFunctionEntity.h"
#include "FogMetaFunctionExpr.h"
#include "FogMetaFunctionSpecifier.h"
#include "FogMetaIterator.h"
#include "FogMetaIteratorEntity.h"
#include "FogMetaLiteral.h"
#include "FogWordName.h"
#include "Fog.h"
#include "FogMetaType.h"
#include "FogMetaType_Methods.h"
#include "FogMetaVariable.h"
#include "FogMetaVariableEntity.h"
#include "FogMetaVariableSpecifier.h"
#include "FogModifier.h"
#include "FogMonadicExpr.h"
#include "FogMultipleId.h"
#include "FogName.h"
#include "FogNamespaceAliasSpecifier.h"
#include "FogNestedName.h"
#include "FogNewExpr.h"
#include "FogNonReservedId.h"
#include "FogNullMonitor.h"
#include "FogNumber.h"
#include "FogOfUseBy.h"
#include "FogOperatorId.h"
#include "FogParameterSpecifier.h"
#include "FogParenthesisExpr.h"
#include "FogParseContext.h"
#include "FogParserMacros.h"
#include "FogLocateParser.h"
#include "FogPointerModifier.h"
#include "FogPotentialDeclaration.h"
#include "FogPotentialDeclarationMonitor.h"
#include "FogProgressMonitor.h"
#include "FogPunctuation.h"
#include "FogRaw.h"
#include "FogRawCtorInit.h"
#include "FogRawCtorInits.h"
#include "FogRaws.h"
#include "FogRawDeclarations.h"
#include "FogRawStatements.h"
#include "FogRawSubspace.h"
#include "FogRawTemplate.h"
#include "FogReader.h"
#include "FogReferenceModifier.h"
#include "FogReservedId.h"
#include "FogReturnStatement.h"
#include "FogUsageEnums.h"
#include "FogUsageManager.h"
#include "FogRoot.h"
#include "FogScope.h"
#include "FogScopeEnums.h"
#include "FogScopedExpr.h"
#include "FogScopedModifier.h"
#include "FogSemantics.h"
#include "FogSemanticsContext.h"
#include "FogSetSpecifier.h"
#include "FogSourceFile.h"
#include "FogSourceFileType.h"
#include "FogSpacing.h"
#include "FogSpecifier.h"
#include "FogStatic.h"
#include "FogStd.h"
#include "FogStream.h"
#include "FogSwitchStatement.h"
#include "FogTargetFile.h"
#include "FogTemplateParameterSpecifier.h"
#include "FogTemplateParameterSpecifierId.h"
#include "FogTemplateParameterSpecifiers.h"
#include "FogTemplatedName.h"
#include "FogThisExpr.h"
#include "FogThrowExpr.h"
#include "FogToken.h"
#include "FogTokenStatements.h"
#include "FogTokenType.h"
#include "FogTokensExpr.h"
#include "FogTreeCallExpr.h"
#include "FogTreeLiteral.h"
#include "FogTryStatement.h"
#include "FogTypeName.h"
#include "FogTypedName.h"
#include "FogTypedef.h"
#include "FogTypedefSpecifier.h"
#include "FogUsage.h"
#include "FogUsageContext.h"
#include "FogUsing.h"
#include "FogUsingSpecifier.h"
#include "FogUsingStyle.h"
#include "FogUtility.h"
#include "FogVariable.h"
#include "FogVariableSpecifier.h"
#include "FogWhileStatement.h"

/*
#include "Fog.h"
#include "Fog0.h"

#include "FogMerge.h"
#include "FogScopeEnums.h"
#include "FogTokenType.h"
#include "FogToken.h"
#include "FogRaw.h"
#include "FogExpr.h"
#include "FogName.h"
#include "FogEntityName.h"
#include "FogWordName.h"
#include "FogKeyword.h"
#include "FogReservedId.h"
#include "FogEnumHandle.h"
#include "FogDeclSpecifierEnums.h"
#include "FogDeclSpecifierValue.h"
#include "FogDeclSpecifier.h"
#include "FogModifier.h"
#include "FogDecoratedName.h"
#include "FogIdentifier.h"
#include "FogUtility.h"
#include "FogSpecifier.h"
#include "FogDecoratedExpr.h"
#include "FogDyadicExpr.h"
#include "FogScope.h"

#include "FogAccess.h"
#include "FogAccessibilitySpecifier.h"
#include "FogActualDeclaration.h"
#include "FogAnonId.h"
#include "FogArgumentId.h"
#include "FogArrayModifier.h"
#include "FogArrayOfExpr.h"
#include "FogAsmSpecifier.h"
#include "FogAssignExpr.h"
#include "FogAuto.h"
#include "FogBaseClass.h"
#include "FogBaseSpecifier.h"
#include "FogBaseSpecifiers.h"
#include "FogBitFieldExpr.h"
#include "FogBrace.h"
#include "FogBreakStatement.h"
#include "FogBuiltInScope.h"
#include "FogBuiltInTypeId.h"
#include "FogCallContext.h"
#include "FogCallExpr.h"
#include "FogCaseStatement.h"
#include "FogCastExpr.h"
#include "FogCatchStatement.h"
#include "FogCharacter.h"
#include "FogClass.h"
#include "FogClassSpecifier.h"
#include "FogCode.h"
#include "FogCodeTokens.h"
#include "FogCondExpr.h"
#include "FogConversionId.h"
#include "FogCtorExpr.h"
#include "FogCtorInit.h"
#include "FogCv.h"
#include "FogDeclSpecifiedName.h"
#include "FogDeleteExpr.h"
#include "FogDerivable.h"
#include "FogDerivedClause.h"
#include "FogDestructorId.h"
#include "FogDoWhileStatement.h"
#include "FogElaboratedTypeSpecifier.h"
#include "FogElementType.h"
#include "FogEllipsisExpr.h"
#include "FogEmitContext.h"
#include "FogEntity.h"
#include "FogEntityFinder.h"
#include "FogEnum.h"
#include "FogEnumSpecifier.h"
#include "FogEnumerator.h"
#include "FogEnumeratorSpecifier.h"
#include "FogEnumeratorSpecifiers.h"
#include "FogExpressionStatement.h"
#include "FogFailure.h"
#include "FogFileComparator.h"
#include "FogFileId.h"
#include "FogFileIdent.h"
#include "FogFileIds.h"
#include "FogFileManager.h"
#include "FogFilespace.h"
#include "FogForStatement.h"
#include "FogFunction.h"
#include "FogFunctionExpr.h"
#include "FogFunctionModifier.h"
#include "FogFunctionSpecifier.h"
#include "FogGlobalName.h"
#include "FogGotoStatement.h"
#include "FogIfStatement.h"
#include "FogIndexExpr.h"
#include "FogInitExpr.h"
#include "FogInline.h"
#include "FogIterationDomain.h"
#include "FogIteratorExpr.h"
#include "FogLabelStatement.h"
#include "FogLazyBool.h"
#include "FogLexerContext.h"
#include "FogLexerCpp.h"
#include "FogLexerHash.h"
#include "FogLexerInput.h"
#include "FogLexerMacro.h"
#include "FogLexerState.h"
#include "FogLexerTower.h"
#include "FogLexerUnget.h"
#include "FogLine.h"
#include "FogLinkage.h"
#include "FogLinkageSpecifier.h"
#include "FogListOfExpr.h"
#include "FogMacro.h"
#include "FogMacroId.h"
#include "FogMakeEntityContext.h"
#include "FogMakeSemantics.h"
#include "FogMakeSpecifierContext.h"
#include "FogMakeTemplateContext.h"
#include "FogMakerContext.h"
#include "FogMemberExpr.h"
#include "FogMetaArgument.h"
#include "FogMetaBuiltInFunction.h"
#include "FogMetaEntity.h"
#include "FogMetaFunction.h"
#include "FogMetaFunctionEntity.h"
#include "FogMetaFunctionExpr.h"
#include "FogMetaFunctionSlot.h"
#include "FogMetaFunctionSpecifier.h"
#include "FogMetaIterator.h"
#include "FogMetaIteratorEntity.h"
#include "FogMetaLiteral.h"
#include "FogMetaObjectSpecifier.h"
#include "FogMetaParameterSpecifier.h"
#include "FogMetaSlot.h"
#include "FogMetaSlotFinder.h"
#include "FogMetaToken.h"
#include "FogMetaType.h"
#include "FogMetaType_Methods.h"
#include "FogMetaVariable.h"
#include "FogMetaVariableEntity.h"
#include "FogMetaVariableSlot.h"
#include "FogMetaVariableSpecifier.h"
#include "FogMonadicExpr.h"
#include "FogMultipleId.h"
#include "FogNamespaceAliasSpecifier.h"
#include "FogNestedName.h"
#include "FogNewExpr.h"
#include "FogNonReservedId.h"
#include "FogNullMonitor.h"
#include "FogNumber.h"
#include "FogObjectSpecifier.h"
#include "FogOfUseBy.h"
#include "FogOperatorId.h"
#include "FogParameterSpecifier.h"
#include "FogParenthesisExpr.h"
#include "FogParseContext.h"
#include "FogParserBase.h"
#include "FogParserMacros.h"
#include "FogParserMaker.h"
#include "FogParserValue.h"
#include "FogPointerModifier.h"
#include "FogPosition.h"
#include "FogPotentialDeclaration.h"
#include "FogPotentialDeclarationMonitor.h"
#include "FogProgressMonitor.h"
#include "FogPunctuation.h"
#include "FogRawCtorInit.h"
#include "FogRawCtorInits.h"
#include "FogRawDeclarations.h"
#include "FogRawStatements.h"
#include "FogRawSubspace.h"
#include "FogRawTemplate.h"
#include "FogRaws.h"
#include "FogReader.h"
#include "FogReferenceModifier.h"
#include "FogReturnStatement.h"
#include "FogRoot.h"
#include "FogScopeContext.h"
#include "FogScopeSpecifier.h"
#include "FogScope_Bases.h"
#include "FogScope_Derivations.h"
#include "FogScope_Enums.h"
#include "FogScope_Friends.h"
#include "FogScope_Functions.h"
#include "FogScope_Implementation.h"
#include "FogScope_Interface.h"
#include "FogScope_Names.h"
#include "FogScope_Nested.h"
#include "FogScope_Typedefs.h"
#include "FogScope_Types.h"
#include "FogScope_Using.h"
#include "FogScope_Variables.h"
#include "FogScopedExpr.h"
#include "FogScopedModifier.h"
#include "FogSegment.h"
#include "FogSemantics.h"
#include "FogSemanticsContext.h"
#include "FogSetSpecifier.h"
#include "FogSourceFile.h"
#include "FogSourceFileType.h"
#include "FogSpacing.h"
#include "FogStatement.h"
#include "FogStatic.h"
#include "FogStd.h"
#include "FogStream.h"
#include "FogString.h"
#include "FogSwitchStatement.h"
#include "FogTag.h"
#include "FogTargetFile.h"
#include "FogTemplateArg.h"
#include "FogTemplateArgs.h"
#include "FogTemplateManager.h"
#include "FogTemplateParameterSpecifier.h"
#include "FogTemplateParameterSpecifierId.h"
#include "FogTemplateParameterSpecifiers.h"
#include "FogTemplatedName.h"
#include "FogThisExpr.h"
#include "FogThrowExpr.h"
#include "FogTokenContext.h"
#include "FogTokenMonitor.h"
#include "FogTokenStatements.h"
#include "FogTokens.h"
#include "FogTokensExpr.h"
#include "FogTreeCallExpr.h"
#include "FogTreeLiteral.h"
#include "FogTryStatement.h"
#include "FogTypeName.h"
#include "FogTypeSpecifier.h"
#include "FogTypedName.h"
#include "FogTypedef.h"
#include "FogTypedefSpecifier.h"
#include "FogUnique.h"
#include "FogUsage.h"
#include "FogUsageContext.h"
#include "FogUsageEnums.h"
#include "FogUsageManager.h"
#include "FogUsing.h"
#include "FogUsingSpecifier.h"
#include "FogUsingStyle.h"
#include "FogVariable.h"
#include "FogVariableSpecifier.h"
#include "FogVirtual.h"
#include "FogWhileStatement.h"*/