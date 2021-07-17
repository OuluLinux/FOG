#include <Fog/FogIncludeAll.h>




























































































TYPEINFO_SINGLE(FogParserMaker, Super)

#define FOGPARSERMAKER_(Q,N,M) \
	Q *FogParserMaker::N() \
	{ \
		Q *q = M; \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_C(Q,N,M,P) \
	Q *FogParserMaker::N(P *p) \
	{ \
		if (!p) \
			return 0; \
		Q *q = M(*p); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_U(Q,N,M,P) \
	Q *FogParserMaker::N(P p) \
	{ \
		Q *q = M(p); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_V(Q,N,M,V) \
	Q *FogParserMaker::N() \
	{ \
		Q *q = M(V); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CC(Q,N,M,P1,P2) \
	Q *FogParserMaker::N(P1 *p1, P2 *p2) \
	{ \
		if (!p1 || !p2) \
			return 0; \
		Q *q = M(*p1, *p2); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CU(Q,N,M,P1,P2) \
	Q *FogParserMaker::N(P1 *p1, P2 p2) \
	{ \
		if (!p1) \
			return 0; \
		Q *q = M(*p1, p2); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_UC(Q,N,M,P1,P2) \
	Q *FogParserMaker::N(P1 p1, P2 *p2) \
	{ \
		if (!p2) \
			return 0; \
		Q *q = M(p1, *p2); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_UU(Q,N,M,P1,P2) \
	Q *FogParserMaker::N(P1 p1, P2 p2) \
	{ \
		Q *q = M(p1, p2); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CCC(Q,N,M,P1,P2,P3) \
	Q *FogParserMaker::N(P1 *p1, P2 *p2, P3 *p3) \
	{ \
		if (!p1 || !p2 || !p3) \
			return 0; \
		Q *q = M(*p1, *p2, *p3); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CCU(Q,N,M,P1,P2,P3) \
	Q *FogParserMaker::N(P1 *p1, P2 *p2, P3 p3) \
	{ \
		if (!p1 || !p2) \
			return 0; \
		Q *q = M(*p1, *p2, p3); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CUC(Q,N,M,P1,P2,P3) \
	Q *FogParserMaker::N(P1 *p1, P2 p2, P3 *p3) \
	{ \
		if (!p1 || !p3) \
			return 0; \
		Q *q = M(*p1, p2, *p3); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CUU(Q,N,M,P1,P2,P3) \
	Q *FogParserMaker::N(P1 *p1, P2 p2, P3 p3) \
	{ \
		if (!p1) \
			return 0; \
		Q *q = M(*p1, p2, p3); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_UCU(Q,N,M,P1,P2,P3) \
	Q *FogParserMaker::N(P1 p1, P2 *p2, P3 p3) \
	{ \
		if (!p2) \
			return 0; \
		Q *q = M(p1, *p2, p3); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CCCU(Q,N,M,P1,P2,P3,P4) \
	Q *FogParserMaker::N(P1 *p1, P2 *p2, P3 *p3, P4 p4) \
	{ \
		if (!p1 || !p2 || !p3) \
			return 0; \
		Q *q = M(*p1, *p2, *p3, p4); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CUCU(Q,N,M,P1,P2,P3,P4) \
	Q *FogParserMaker::N(P1 *p1, P2 p2, P3 *p3, P4 p4) \
	{ \
		if (!p1 || !p3) \
			return 0; \
		Q *q = M(*p1, p2, *p3, p4); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CUUU(Q,N,M,P1,P2,P3,P4) \
	Q *FogParserMaker::N(P1 *p1, P2 p2, P3 p3, P4 p4) \
	{ \
		if (!p1) \
			return 0; \
		Q *q = M(*p1, p2, p3, p4); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_UUUC(Q,N,M,P1,P2,P3,P4) \
	Q *FogParserMaker::N(P1 p1, P2 p2, P3 p3, P4 *p4) \
	{ \
		if (!p4) \
			return 0; \
		Q *q = M(p1, p2, p3, *p4); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_CCCUC(Q,N,M,P1,P2,P3,P4,P5) \
	Q *FogParserMaker::N(P1 *p1, P2 *p2, P3 *p3, P4 p4, P5 *p5) \
	{ \
		if (!p1 || !p2 || !p3 || !p5) \
			return 0; \
		Q *q = M(*p1, *p2, *p3, p4, *p5); \
		garbage_adopt(q); \
		return q; \
	}

#define FOGPARSERMAKER_APPLY(Q, N, P0, M) \
	Q *FogParserMaker::N(P0 *aTarget) \
	{ \
		if (aTarget) \
			aTarget->M; \
		return aTarget; \
	}

#define FOGPARSERMAKER_APPLY_C(Q, N, P0, M, P1) \
	Q *FogParserMaker::N(P0 *aTarget, P1 *p1) \
	{ \
		if (aTarget && p1) \
			aTarget->M(*p1); \
		return aTarget; \
	}

#define FOGPARSERMAKER_APPLY_U(Q, N, P0, M, P1) \
	Q *FogParserMaker::N(P0 *aTarget, P1 p1) \
	{ \
		if (aTarget) \
			aTarget->M(p1); \
		return aTarget; \
	}

#define FOGPARSERMAKER_ASSIGN(Q,N,E) Q *FogParserMaker::N(E *p) { return p; }
#define FOGPARSERMAKER_VALUE(Q,N,E) Q *FogParserMaker::N() { return E; }
#define FOGPARSERMAKER_VALUE_U(Q,N,E,P0) Q *FogParserMaker::N(P1 p1) { return E(p1); }
#define FOGPARSERMAKER_ERROR(D) D { ERRMSG("BUG -- no implementation of " #D); return 0; }

#define FOGPARSERMAKER_LIST(N,L,E) \
	L *FogParserMaker::N(L *aList, E *anElement) \
	{ \
		if (!anElement) \
			return aList; \
		if (!aList) \
		{ \
			aList = new L; \
			garbage_adopt(aList); \
		} \
		if (aList) \
			aList->add(*anElement); \
		return aList; \
	}

#define FOGPARSERMAKER_OPERATOR(N,T) \
	CxxName *FogParserMaker::N() { static FogName& anId = FogName::make_operator(T); return &anId; }

CxxExpression* FogParserMaker::make_abstract_array_expression(CxxExpression* sizeExpr) {
	if (!sizeExpr)
		return 0;
		
	FogExpr* anExpr = new FogIndexExpr(FogAnonId::abstract(), *sizeExpr);
	garbage_adopt(anExpr);
	return anExpr;
}

FOGPARSERMAKER_ASSIGN(CxxExpression, make_abstract_function_expression, CxxParenthesised)
FOGPARSERMAKER_APPLY_C(CxxBaseSpecifier, make_access_base_specifier, CxxBaseSpecifier, set_access, CxxAccessSpecifier)
FOGPARSERMAKER_ASSIGN(CxxDeclSpecifierId, make_access_specifier_id, CxxAccessSpecifier)
FOGPARSERMAKER_C(CxxDeclaration, make_accessibility_specifier, new FogAccessibilitySpecifier, CxxAccessSpecifier)
FOGPARSERMAKER_CC(CxxExpression, make_add_expression, new FogAddExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_and_expression, new FogAndExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_array_expression, new FogIndexExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_arrow_expression, new FogArrowExpr, CxxExpression, CxxName)
FOGPARSERMAKER_CC(CxxExpression, make_arrow_star_expression, new FogArrowStarExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_C(CxxDeclaration, make_asm_definition, new FogAsmSpecifier, CxxStrings)

CxxExpression* FogParserMaker::make_assignment_expression(CxxExpression* leftExpr, CxxToken* assOp, CxxExpression* rightExpr) {
	if (!leftExpr || !assOp || !rightExpr)
		return 0;
		
	const FogTokenType::TokenType tokenType = assOp->token_type_enum();
	FogExpr* newExpr = 0;
	
	switch (tokenType) {
	case '=': newExpr = new FogAssExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_ADD: newExpr = new FogAssAddExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_AND: newExpr = new FogAssAndExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_DIV: newExpr = new FogAssDivExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_MOD: newExpr = new FogAssModExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_MUL: newExpr = new FogAssMulExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_OR: newExpr = new FogAssOrExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_SHL: newExpr = new FogAssShlExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_SHR: newExpr = new FogAssShrExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_SUB: newExpr = new FogAssSubExpr(*leftExpr, *rightExpr); break;
	
	case FogTokenType::ASS_XOR: newExpr = new FogAssXorExpr(*leftExpr, *rightExpr); break;
	
	default:
		ERRMSG("BUG -- unknown operator " << viz(*assOp) << " for make_assignment_expression.");
	}
	
	garbage_adopt(newExpr);
	return newExpr;
}

FOGPARSERMAKER_C(CxxBaseSpecifier, make_base_specifier, new FogBaseSpecifier, CxxName)
FOGPARSERMAKER_LIST(make_base_specifiers, CxxBaseSpecifiers, CxxBaseSpecifier)

CxxExpression* FogParserMaker::make_bit_field_expression(CxxExpression* anExpression, CxxExpression* fieldWidth) {
	if (fieldWidth) {
		if (!anExpression)
			anExpression = &FogName::make_anon();
			
		anExpression = new FogBitFieldExpr(*anExpression, *fieldWidth);
		garbage_adopt(anExpression);
	}
	
	return anExpression;
}

FOGPARSERMAKER_(CxxStatement, make_break_statement, new FogBreakStatement(FogBreakStatement::BREAK))
FOGPARSERMAKER_ASSIGN(CxxName, make_built_in_id, CxxBuiltInId)

CxxName* FogParserMaker::make_built_in_name(CxxName* aName, CxxBuiltInId* builtInName) {
	if (!aName || !builtInName)
		return 0;
		
	FogNameRef theName;
	aName->make_built_in(theName, *builtInName);
	
	if (theName.pointer() != aName)
		garbage_add(theName.pointer());
		
	return theName.pointer();
	//    FogDeclSpecifiedName *declSpecifiedName = aName->is_decl_specified_name();
	//    if (!declSpecifiedName)
	//    {
	//        declSpecifiedName = new FogDeclSpecifiedName(*aName);
	//        if (!declSpecifiedName)
	//            return 0;
	//        garbage_adopt(declSpecifiedName);
	//    }
	//    declSpecifiedName->make_built_in(*builtInName);
	//    return declSpecifiedName;
}

FOGPARSERMAKER_ASSIGN(CxxName, make_built_in_id_id, CxxBuiltInId)

CxxBuiltInId* FogParserMaker::make_built_in_ids(CxxBuiltInId* aTarget, CxxBuiltInId* p1) {
	if (aTarget && p1)
		aTarget = aTarget->make(*p1);
		
	return aTarget;
}

FOGPARSERMAKER_CC(CxxExpression, make_call_expression, new FogCallExpr, CxxExpression, CxxParenthesised)
FOGPARSERMAKER_CC(CxxStatement, make_case_statement, new FogCaseStatement, CxxExpression, CxxStatement)

//  FOGPARSERMAKER_CC(CxxExpression, make_cast_expression, new FogExplicitCastExpr, CxxExpression, CxxExpression)
//  FOGPARSERMAKER_CC(CxxExpression, make_index_cast_expression, new FogIndexCastExpr, CxxExpression, CxxExpression)
CxxExpression* FogParserMaker::make_cast_expression(CxxExpression* aCast, CxxExpression* anExpr) {
	if (!aCast || !anExpr)
		return 0;
		
	CxxExpression* theExpr = 0;
	FogIndexExpr* indexExpr = aCast->is_index_expr();
	
	if (indexExpr)
		theExpr = indexExpr->new_index_cast_expression(*anExpr);
	else
		theExpr = new FogExplicitCastExpr(*aCast, *anExpr);
		
	garbage_adopt(theExpr);
	return theExpr;
}

FOGPARSERMAKER_ASSIGN(CxxExpression, make_character_literal_expression, CxxCharacterLiteral)
FOGPARSERMAKER_APPLY_U(CxxName, make_class_members, CxxClass, set_declarations, CxxMemberDeclarations*)
FOGPARSERMAKER_CUU(CxxClass, make_class_specifier_id, new FogClassSpecifier, CxxClassKey, CxxName*, CxxBaseSpecifiers*)

CxxSimpleTypeParameter* FogParserMaker::make_class_type_parameter(CxxName* aName) {
	if (!aName)
		return 0;
		
	CxxSimpleTypeParameter* typeParameter = new FogElaboratedTypeSpecifier(FogTag::class_tag(),
	        aName ? *aName : FogName::make_anon());
	garbage_adopt(typeParameter);
	return typeParameter;
}

FOGPARSERMAKER_C(CxxExpression, make_complement_expression, new FogComplExpr, CxxExpression)

CxxStatement* FogParserMaker::make_compound_statement(CxxStatements* someStatements) {
	if (!someStatements) {
		someStatements = new FogRawStatements;
		garbage_adopt(someStatements);
	}
	
	return someStatements;
}

FOGPARSERMAKER_ASSIGN(CxxCondition, make_condition, CxxParameters)
FOGPARSERMAKER_CCC(CxxExpression, make_conditional_expression, new FogCondExpr, CxxExpression, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_const_cast_expression, new FogConstCastExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_(CxxStatement, make_continue_statement, new FogBreakStatement(FogBreakStatement::CONTINUE))
FOGPARSERMAKER_C(CxxName, make_conversion_function_id, new FogConversionId, CxxExpression)

CxxExpression* FogParserMaker::make_ctor_definition(CxxExpressions* anExpr, CxxFunctionBody* functionBody) {
	if (!anExpr || !functionBody)
		return 0;
		
	FogExprConstListOfRefIterator p(*anExpr);
	
	if (!p)
		return 0;
		
	FogRawCtorInitsRef ctorInits(new FogRawCtorInits, FogRawCtorInitsRef::ADOPT);
	FogExpr& bitExpr = *p;
	
	for (++p; p; ++p) {
		FogExpr& ctorExpr = *p;
	}
	
	//  .bugbug rest of conversion to CtorInits needed
	return make_function_definition(anExpr, functionBody);
}

FOGPARSERMAKER_C(CxxExpression, make_ctor_expression, new FogCtorExpr, CxxExpression)

CxxFunctionBody* FogParserMaker::make_ctor_function_block(CxxFunctionBody* functionBody, CxxMemInitializers* ctorList) {
	if (!functionBody) {
		functionBody = new FogCode;
		
		if (!functionBody)
			return 0;
			
		garbage_adopt(functionBody);
	}
	
	if (ctorList)
		functionBody->set_ctor_inits(*ctorList);
		
	return functionBody;
}

FOGPARSERMAKER_ASSIGN(CxxDeclSpecifierId, make_cv_decl_specifier, CxxCvQualifiers)
FOGPARSERMAKER_CC(CxxPointerDeclarator, make_cv_declarator, &FogPointerModifier::new_cv, CxxPointerDeclarator, CxxCvQualifiers)

CxxCvQualifiers* FogParserMaker::make_cv_qualifiers(CxxCvQualifiers* aList, CxxCvQualifiers* anElement) {
	if (!aList)
		return anElement ? anElement : &FogCv::initial();
	else
		return anElement ? &aList->add(*anElement) : aList;
}

CxxDeclaration* FogParserMaker::make_decl_specifier_declaration(CxxDeclaration* aDeclaration, CxxDeclSpecifierId* declSpecifier) {
	if (!aDeclaration || !declSpecifier)
		return aDeclaration;
		
	FogRawRef aRaw;
	aDeclaration->set_decl_specifier_raw(aRaw, *declSpecifier);
	
	if (aRaw.pointer() != aDeclaration)
		garbage_add(aRaw.pointer());
		
	return aRaw.pointer();
}

CxxExpression* FogParserMaker::make_decl_specifier_expression(CxxExpression* anExpr, CxxDeclSpecifierId* declSpecifier) {
	if (!anExpr || !declSpecifier)
		return anExpr;
		
	FogExprRef theExpr;
	anExpr->set_decl_specifier_expression(theExpr, *declSpecifier);
	
	if (theExpr.pointer() != anExpr)
		garbage_add(theExpr.pointer());
		
	return theExpr.pointer();
}

CxxName* FogParserMaker::make_decl_specifier_name(CxxName* aName, CxxDeclSpecifierId* declSpecifier) {
	if (!aName || !declSpecifier)
		return aName;
		
	FogNameRef theName;
	aName->set_decl_specifier_name(theName, *declSpecifier);
	
	if (theName.pointer() != aName)
		garbage_add(theName.pointer());
		
	return theName.pointer();
}

CxxParameter* FogParserMaker::make_decl_specifier_parameter(CxxParameter* anExpr, CxxDeclSpecifierId* declSpecifier) {
	return make_decl_specifier_expression(anExpr, declSpecifier);
}

FOGPARSERMAKER_ERROR(CxxTokens * FogParserMaker::make_decl_specifier_tree_arguments(CxxTokens* treeArguments, CxxDeclSpecifierId* aName))
FOGPARSERMAKER_ERROR(CxxDeclSpecifierId * FogParserMaker::make_decl_specifiers(CxxDeclSpecifierId* aList, CxxDeclSpecifierId* anElement))
FOGPARSERMAKER_LIST(make_declarations, CxxDeclarations, CxxDeclaration)
FOGPARSERMAKER_ASSIGN(CxxStatement, make_declaration_statement, CxxDeclaration)
FOGPARSERMAKER_C(CxxStatement, make_default_statement, new FogCaseStatement, CxxStatement)

CxxDeleteExpression* FogParserMaker::make_delete_expression(CxxExpression* anExpr) {
	if (!anExpr)
		return 0;
		
	FogIndexCastExpr* indexCastExpr = anExpr->is_index_cast_expr();
	
	if (indexCastExpr && !indexCastExpr->index().is_null())
		WRNMSG("Obsolete size of deleted object ignored.");
		
	FogDeleteExpr* deleteExpr = indexCastExpr ? new FogDeleteExpr(indexCastExpr->expr(), ARRAY_DELETE)
	                            : new FogDeleteExpr(*anExpr, SCALAR_DELETE);
	garbage_adopt(deleteExpr);
	return deleteExpr;
}

FOGPARSERMAKER_CU(CxxDeclaration, make_derived_clause, new FogDerivedClause, CxxExpression, CxxDeclaration*)
FOGPARSERMAKER_C(CxxName, make_destructor_id, new FogDestructorId, CxxName)
FOGPARSERMAKER_CC(CxxExpression, make_divide_expression, new FogDivExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxStatement, make_do_while_statement, new FogDoWhileStatement, CxxStatement, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_dot_expression, new FogDotExpr, CxxExpression, CxxName)
FOGPARSERMAKER_CC(CxxExpression, make_dot_star_expression, new FogDotStarExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_dynamic_cast_expression, new FogDynamicCastExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxName, make_elaborated_type_specifier, new FogElaboratedTypeSpecifier, CxxClassKey, CxxName)
FOGPARSERMAKER_(CxxParameter, make_ellipsis_expression, new FogEllipsisExpr)
FOGPARSERMAKER_UU(CxxName, make_enum_specifier_id, new FogEnumSpecifier, CxxName*, CxxEnumerators*)
FOGPARSERMAKER_CU(CxxEnumerator, make_enumerator, new FogEnumeratorSpecifier, CxxName, CxxExpression*)
FOGPARSERMAKER_LIST(make_enumerators, CxxEnumerators, CxxEnumerator)

CxxName* FogParserMaker::make_epsilon() {
	CxxName* q = &FogAnonId::abstract();
	garbage_add(q);
	return q;
}

FOGPARSERMAKER_CC(CxxExpression, make_equal_expression, new FogEqExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_ASSIGN(CxxExceptionDeclaration, make_exception_declaration, CxxParameter)
FOGPARSERMAKER_ASSIGN(CxxExceptionSpecification, make_exception_specification, CxxExpressions)
FOGPARSERMAKER_CC(CxxExpression, make_exclusive_or_expression, new FogXorExpr, CxxExpression, CxxExpression)

CxxDeclaration* FogParserMaker::make_explicit_specialization(CxxDeclaration* aDeclaration) {
	if (aDeclaration) {
		aDeclaration = new FogRawTemplate(FogListOfExpr::mutable_null(), *aDeclaration);
		garbage_adopt(aDeclaration);
	}
	
	return aDeclaration;
}

FOGPARSERMAKER_C(CxxDeclaration, make_export_all_declaration, FogRaw::new_set_all, CxxFileId)
FOGPARSERMAKER_C(CxxDeclaration, make_export_implementation_declaration, FogRaw::new_set_implementation, CxxFileId)
FOGPARSERMAKER_C(CxxDeclaration, make_export_interface_declaration, FogRaw::new_set_interface, CxxFileId)
FOGPARSERMAKER_(CxxDeclaration, make_export_noimplementation_declaration, FogRaw::new_set_no_implementation())
FOGPARSERMAKER_C(CxxDeclaration, make_export_utility_declaration, FogRaw::new_set_utility, CxxUtility)

CxxExpression* FogParserMaker::make_expression(CxxExpressions* aList) {
	if (aList)
		return aList;
	else
		return &FogAnonId::abstract();
}

FOGPARSERMAKER_ASSIGN(CxxParameter, make_expression_parameter, CxxExpression)
FOGPARSERMAKER_LIST(make_expressions, CxxExpressions, CxxExpression)
FOGPARSERMAKER_VALUE(CxxExpression, make_false_expression, &FogNumber::make_false())
FOGPARSERMAKER_ASSIGN(CxxFileId, make_file_id, CxxFileName)
FOGPARSERMAKER_C(CxxFileId, make_file_id_implementation, FogFileId::new_implementation, CxxName)
FOGPARSERMAKER_C(CxxFileId, make_file_id_interface, FogFileId::new_interface, CxxName)
FOGPARSERMAKER_LIST(make_file_ids, CxxFileIds, CxxFileId)

CxxFileName* FogParserMaker::make_file_name(CxxExpression* fileName) {
	if (!fileName)
		return 0;
		
	FogStaticScopeContext scopeContext(scope());
	PrimIdHandle aName;
	fileName->get_string(aName, scopeContext);
	FogFileIdent* fileId = FogFileId::new_id(*aName);
	garbage_adopt(fileId);
	return fileId;
}

CxxFileName* FogParserMaker::make_file_name_guard(CxxFileName* fileName, CxxExpression* aString) {
	if (fileName && aString) {
		FogStaticScopeContext scopeContext(scope());
		PrimIdHandle anId;
		aString->get_identifier(anId, scopeContext);
		fileName->set_guard(*anId);
	}
	
	return fileName;
}

CxxFileName* FogParserMaker::make_file_name_implementation(CxxFileName* fileName)
{ if (fileName) fileName->set_implementation(); return fileName; }

CxxFileName* FogParserMaker::make_file_name_interface(CxxFileName* fileName)
{ if (fileName) fileName->set_interface(); return fileName; }

CxxFileName* FogParserMaker::make_file_name_noguard(CxxFileName* fileName)
{ if (fileName) fileName->set_guard(PrimId::null()); return fileName; }

CxxFileName* FogParserMaker::make_file_name_path(CxxFileName* fileName, CxxExpression* aString) {
	if (fileName && aString) {
		FogStaticScopeContext scopeContext(scope());
		PrimIdHandle anId;
		aString->get_identifier(anId, scopeContext);
		fileName->set_path(*anId);
	}
	
	return fileName;
}

CxxFileName* FogParserMaker::make_file_name_prefix(CxxFileName* fileName, CxxExpression* aString) {
	if (fileName && aString) {
		FogStaticScopeContext scopeContext(scope());
		PrimIdHandle anId;
		aString->get_identifier(anId, scopeContext);
		fileName->set_prefix(*anId);
	}
	
	return fileName;
}

CxxFileName* FogParserMaker::make_file_name_suffix(CxxFileName* fileName, CxxExpression* aString) {
	if (fileName && aString) {
		FogStaticScopeContext scopeContext(scope());
		PrimIdHandle anId;
		aString->get_identifier(anId, scopeContext);
		fileName->set_suffix(*anId);
	}
	
	return fileName;
}

CxxFileName* FogParserMaker::make_file_name_template(CxxFileName* fileName)
{ if (fileName) fileName->set_template(); return fileName; }

CxxFileName* FogParserMaker::make_file_name_utility(CxxFileName* fileName, CxxUtility* aUtility)
{ if (fileName && aUtility) fileName->set_utility(*aUtility); return fileName; }

FOGPARSERMAKER_ASSIGN(CxxDeclaration, make_filespace_declaration, CxxName)

FOGPARSERMAKER_ERROR(CxxName * FogParserMaker::make_filespace_specifier(CxxFileName* aName, CxxDeclarations* aDeclaration))
//  CxxName *FogParserMaker::make_filespace_specifier(CxxFileName *aName, CxxDeclarations *aDeclaration)
//  {
//    return make_class_members(make_class_specifier_id(&FogTag::filespace_tag(), aName, 0), aDeclaration);
//  }

FOGPARSERMAKER_ASSIGN(CxxExpression, make_floating_literal_expression, CxxFloatingLiteral)
FOGPARSERMAKER_UUUC(CxxStatement, make_for_statement, new FogForStatement, CxxExpression*, CxxCondition*, CxxExpression*, CxxStatement)
FOGPARSERMAKER_C(CxxFunctionBody, make_function_block, new FogCode, CxxStatement)
FOGPARSERMAKER_CC(CxxExpression, make_function_definition, new FogFunctionExpr, CxxExpression, CxxFunctionBody)

CxxDeclarator* FogParserMaker::make_global_declarator(CxxIsTemplate isTemplate, CxxDeclarator* aDeclarator) {
	if (isTemplate)
		ERRMSG("Unexpected template keyword ignored preceding ::ptr-operator");
		
	if (!aDeclarator)
		return 0;
		
	CxxDeclarator* globalDeclarator = new FogScopedModifier(*aDeclarator);
	garbage_adopt(globalDeclarator);
	return globalDeclarator;
}

CxxExpression* FogParserMaker::make_global_expression(CxxIsTemplate isTemplate, CxxDeleteExpression* aTarget) {
	if (isTemplate)
		ERRMSG("Unexpected template keyword ignored preceding ::delete");
		
	if (aTarget)
		aTarget->set_global();
		
	return aTarget;
}

CxxExpression* FogParserMaker::make_global_expression(CxxIsTemplate isTemplate, CxxNewExpression* aTarget) {
	if (isTemplate)
		ERRMSG("Unexpected template keyword ignored preceding ::new");
		
	if (aTarget)
		aTarget->set_global();
		
	return aTarget;
}

CxxName* FogParserMaker::make_global_id(CxxIsTemplate isTemplate, CxxName* nestedId) {
	if (!nestedId)
		return 0;
		
	CxxName* globalName = new FogGlobalName(isTemplate, *nestedId);
	garbage_adopt(globalName);
	return globalName;
}

FOGPARSERMAKER_C(CxxStatement, make_goto_statement, new FogGotoStatement, CxxName)
FOGPARSERMAKER_CC(CxxExpression, make_greater_equal_expression, new FogGeExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_greater_than_expression, new FogGtExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxHandler, make_handler, new FogCatchStatement, CxxExceptionDeclaration, CxxStatement)
FOGPARSERMAKER_LIST(make_handlers, CxxHandlers, CxxHandler)
FOGPARSERMAKER_CCU(CxxStatement, make_if_statement, new FogIfStatement, CxxCondition, CxxStatement, CxxStatement*)

CxxDeclaration* FogParserMaker::make_include_declaration(CxxExpression* aString, CxxUtility* aUtility) {
	FogStaticScopeContext scopeContext(scope());
	PrimIdHandle aName;
	aString->get_string(aName, scopeContext);
	include(*aName, aUtility);
	return 0;
}

FOGPARSERMAKER_CC(CxxExpression, make_inclusive_or_expression, new FogOrExpr, CxxExpression, CxxExpression)
//  FOGPARSERMAKER_CC(CxxExpression, make_index_cast_expression, new FogIndexCastExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_C(CxxExpression, make_init_expression, new FogInitExpr, CxxExpression)

CxxSimpleTypeParameter* FogParserMaker::make_init_simple_type_parameter(CxxSimpleTypeParameter* simpleTypeParameter,
        CxxExpression* anExpr) {
	if (simpleTypeParameter) {
		if (anExpr) {
			simpleTypeParameter = new FogAssExpr(*simpleTypeParameter, *anExpr);
			garbage_adopt(simpleTypeParameter);
		}
	}
	
	return simpleTypeParameter;
}

CxxTemplatedTypeParameter* FogParserMaker::make_init_templated_parameter(CxxTemplatedTypeParameter* typeParameter,
        CxxName* aName) {
	if (typeParameter) {
		if (aName) {
			typeParameter = new FogAssExpr(*typeParameter, *aName);
			garbage_adopt(typeParameter);
		}
	}
	
	return typeParameter;
}

CxxInitializerClauses* FogParserMaker::make_initializer_clauses(CxxInitializerClauses* aList,
        CxxInitializerClause* anElement) {
	if (!anElement)
		return aList;
		
	if (!aList) {
		aList = new FogListOfExpr(IS_ENCAPSULATED);;
		garbage_adopt(aList);
	}
	
	if (aList)
		aList->add(*anElement);
		
	return aList;
}

FOGPARSERMAKER_ASSIGN(CxxInitializerClause, make_initializer_expression_clause, CxxExpression)

CxxInitializerClause* FogParserMaker::make_initializer_list_clause(CxxInitializerClauses* aList) {
	const FogListOfExpr* constList = aList;
	FogArrayOfExpr* exprArray = constList ? new FogArrayOfExpr(constList->exprs()) : new FogArrayOfExpr;
	garbage_adopt(exprArray);
	return exprArray;
}

FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_inline_in_implementation, &FogInline::in_implementation())
FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_inline_in_interface, &FogInline::in_interface())
FOGPARSERMAKER_ASSIGN(CxxFileId, make_input_file, CxxFileId)
FOGPARSERMAKER_ASSIGN(CxxExpression, make_integer_literal_expression, CxxIntegerLiteral)
FOGPARSERMAKER_CC(CxxStatement, make_label_statement, new FogLabelStatement, CxxName, CxxStatement)
FOGPARSERMAKER_CC(CxxExpression, make_less_equal_expression, new FogLeExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_less_than_expression, new FogLtExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_VALUE(CxxLine, make_line, &make_current_line())
FOGPARSERMAKER_APPLY_C(FogRaw, make_lined_declaration, FogRaw, set_line, FogLine)
FOGPARSERMAKER_APPLY_C(FogRaw, make_lined_statement, FogRaw, set_line, FogLine)
FOGPARSERMAKER_APPLY_C(FogToken, make_lined_token, FogToken, set_line, FogLine)
FOGPARSERMAKER_ASSIGN(CxxDeclaration, make_linkage_specification, CxxName)

CxxName* FogParserMaker::make_linkage_specifier(CxxStrings* aString, CxxDeclaration* aDeclaration) {
	if (!aString)
		return 0;
		
	FogStaticScopeContext scopeContext(scope());
	PrimIdHandle anId;
	aString->get_identifier(anId, scopeContext);
	CxxName* q = new FogLinkageSpecifier(*anId, aDeclaration);
	garbage_adopt(q);
	return q;
}

FOGPARSERMAKER_CC(CxxExpression, make_logical_and_expression, new FogLogAndExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_logical_or_expression, new FogLogOrExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CU(CxxMemInitializer, make_mem_initializer, new FogRawCtorInit, CxxName, CxxExpression*)
FOGPARSERMAKER_LIST(make_mem_initializers, CxxMemInitializers, CxxMemInitializer)
FOGPARSERMAKER_LIST(make_member_declarations, CxxMemberDeclarations, CxxDeclaration)

CxxExpression* FogParserMaker::make_meta_assignment_expression(CxxExpression* leftExpr, CxxToken* assOp, CxxToken* rightExpr) {
	if (!leftExpr || !assOp || !rightExpr)
		return 0;
		
	FogExpr* rightLiteral = new FogMetaLiteral(*rightExpr);
	garbage_adopt(rightLiteral);
	return make_assignment_expression(leftExpr, assOp, rightLiteral);
}

FOGPARSERMAKER_APPLY(CxxBaseSpecifier, make_meta_base_specifier, CxxBaseSpecifier, set_access(FogAccess::auto_access()))

CxxMetaType* FogParserMaker::make_meta_built_in_type(CxxBuiltInId* aName) {
	if (!aName)
		return 0;
		
	return &aName->number_type();
}

CxxMetaClass* FogParserMaker::make_meta_class(CxxName* aName, CxxBaseSpecifiers* baseSpecifiers, CxxDeclarations* aList) {
	CxxMetaClass* metaClass = new FogClassSpecifier(FogTag::meta_tag(), aName, baseSpecifiers);
	
	if (metaClass)
		metaClass->set_declarations(aList);
		
	garbage_adopt(metaClass);
	return metaClass;
}

CxxDeclaration* FogParserMaker::make_meta_declaration(CxxExpression* anExpr) {
	if (!anExpr)
		return 0;
		
	CxxDeclaration* metaDeclaration = new FogExpressionStatement(*anExpr, true);
	garbage_adopt(metaDeclaration);
	return metaDeclaration;
}

CxxMetaFunction* FogParserMaker::make_meta_function(char isTwiddle, CxxExpression* anExpr, CxxTokenStatements* aBody) {
	if (!anExpr || !aBody)
		return 0;
		
	IsExposed isExposed = IS_ENCAPSULATED;
	FogIndexExpr* indexExpr = anExpr->is_index_expr();
	
	if (indexExpr) {
		anExpr = &indexExpr->expr();
		isExposed = IS_EXPOSED;
	}
	
	FogTreeCallExpr* treeCallExpr = anExpr->is_tree_call_expr();
	
	if (!treeCallExpr) {
		ERRMSG("BUG -- expected tree-call-expression rather than " << viz(*anExpr) << " as meta-function-naming");
		return 0;
	}
	
	anExpr = &treeCallExpr->expr();
	
	if (isTwiddle)
		anExpr = make_complement_expression(anExpr);        //  .bugbug the old way - needlessly inefficient
		
	if (!anExpr)
		return 0;
		
	//  .bugbug pull type apart, handle gratuitous parentheses
	CxxMetaFunction* metaFunction = new FogMetaFunctionExpr(*anExpr, treeCallExpr->args(), isExposed, *aBody);
	garbage_adopt(metaFunction);
	return metaFunction;
}

CxxStatement* FogParserMaker::make_meta_statement(CxxStatement* aStatement) {
	if (!aStatement)
		return 0;
		
	FogStatement* theStatement = aStatement->is_statement();
	
	if (!theStatement)
		ERRMSG("BUG -- FogStatememnt expected rather than " << viz(*aStatement));
	else
		theStatement->set_is_meta();
		
	return aStatement;
}

CxxMetaType* FogParserMaker::make_meta_type(CxxName* aName) {
	if (!aName)
		return 0;
		
	const PrimId* theId = aName->is_resolved();
	
	if (!theId) {
		ERRMSG("Need a resolved meta-type rather than " << viz(*aName));
		return 0;
	}
	
	return &FogMetaType::get(*theId);
}

FOGPARSERMAKER_C(CxxExpression, make_minus_expression, new FogNegExpr, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_modulus_expression, new FogModExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CCC(CxxExpression, make_multiply_expression, new FogMulExpr, CxxExpression, CxxDeclarator, CxxExpression)
FOGPARSERMAKER_ASSIGN(CxxName, make_name, CxxName)
FOGPARSERMAKER_ASSIGN(CxxName, make_name_expression, CxxName)
FOGPARSERMAKER_CC(CxxDeclaration, make_namespace_alias_definition, new FogNamespaceAliasSpecifier, CxxName, CxxName)
FOGPARSERMAKER_ASSIGN(CxxDeclaration, make_namespace_declaration, CxxName)

CxxName* FogParserMaker::make_namespace_definition(CxxName* aName, CxxDeclarations* aDeclaration) {
	return make_class_members(make_class_specifier_id(&FogTag::namespace_tag(), aName, 0), aDeclaration);
}

FOGPARSERMAKER_CC(CxxDeclarator, make_nested_declarator, new FogScopedModifier, CxxName, CxxDeclarator)
FOGPARSERMAKER_CC(CxxName, make_nested_id, new FogNestedName, CxxName, CxxName)
FOGPARSERMAKER_ASSIGN(CxxName, make_nested_scope, CxxName)
FOGPARSERMAKER_CUU(CxxNewExpression, make_new_expression, new FogNewExpr, CxxParameters, CxxParameters*, CxxExpression*)
FOGPARSERMAKER_UCU(CxxNewExpression, make_new_type_id_expression, new FogNewExpr, CxxParameters*, CxxExpression, CxxExpression*)
FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_not_const, &FogCv::not_const())
FOGPARSERMAKER_CC(CxxExpression, make_not_equal_expression, new FogNeExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_C(CxxExpression, make_not_expression, new FogLogNotExpr, CxxExpression)
FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_not_inline, &FogInline::out_of_line())
FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_not_static, &FogStatic::not_static())
FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_not_virtual, &FogVirtual::not_virtual())
FOGPARSERMAKER_APPLY(CxxBaseSpecifier, make_not_virtual_base_specifier, CxxBaseSpecifier, set_virtual(FogVirtual::not_virtual()))
FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_not_volatile, &FogCv::not_volatile())
FOGPARSERMAKER_ASSIGN(CxxExpression, make_number_literal_expression, CxxExpression)

CxxExpression* FogParserMaker::make_object_scope_expression(CxxExpression* anExpression, CxxSubspace* aSubspace) {
	if (anExpression && aSubspace) {
		anExpression = new FogScopedExpr(*anExpression, *aSubspace);
		garbage_adopt(anExpression);
	}
	
	return anExpression;
}

FOGPARSERMAKER_OPERATOR(make_operator_add_id, "operator+")
FOGPARSERMAKER_OPERATOR(make_operator_arrow_id, "operator->")
FOGPARSERMAKER_OPERATOR(make_operator_arrow_star_id, "operator->*")
FOGPARSERMAKER_OPERATOR(make_operator_ass_add_id, "operator+=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_bit_and_id, "operator&=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_bit_or_id, "operator|=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_div_id, "operator/=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_id, "operator=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_mod_id, "operator%=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_mul_id, "operator*=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_shl_id, "operator<<=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_shr_id, "operator>>=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_sub_id, "operator-=")
FOGPARSERMAKER_OPERATOR(make_operator_ass_xor_id, "operator^=")
FOGPARSERMAKER_OPERATOR(make_operator_bit_and_id, "operator&")
FOGPARSERMAKER_OPERATOR(make_operator_bit_not_id, "operator~")
FOGPARSERMAKER_OPERATOR(make_operator_bit_or_id, "operator|")
FOGPARSERMAKER_OPERATOR(make_operator_call_id, "operator()")
FOGPARSERMAKER_OPERATOR(make_operator_comma_id, "operator,")
FOGPARSERMAKER_OPERATOR(make_operator_dec_id, "operator--")
FOGPARSERMAKER_OPERATOR(make_operator_delete_id, "operator delete")
FOGPARSERMAKER_OPERATOR(make_operator_div_id, "operator/")
FOGPARSERMAKER_OPERATOR(make_operator_eq_id, "operator==")
FOGPARSERMAKER_ASSIGN(CxxName, make_operator_function_id, CxxName)
FOGPARSERMAKER_OPERATOR(make_operator_ge_id, "operator>=")
FOGPARSERMAKER_OPERATOR(make_operator_gt_id, "operator>")
FOGPARSERMAKER_OPERATOR(make_operator_inc_id, "operator++")
FOGPARSERMAKER_OPERATOR(make_operator_index_id, "operator[]")
FOGPARSERMAKER_OPERATOR(make_operator_le_id, "operator<=")
FOGPARSERMAKER_OPERATOR(make_operator_log_and_id, "operator&&")
FOGPARSERMAKER_OPERATOR(make_operator_log_not_id, "operator!")
FOGPARSERMAKER_OPERATOR(make_operator_log_or_id, "operator||")
FOGPARSERMAKER_OPERATOR(make_operator_lt_id, "operator<")
FOGPARSERMAKER_OPERATOR(make_operator_mod_id, "operator%")
FOGPARSERMAKER_OPERATOR(make_operator_mul_id, "operator*")
FOGPARSERMAKER_OPERATOR(make_operator_ne_id, "operator!=")
FOGPARSERMAKER_OPERATOR(make_operator_new_id, "operator new")
FOGPARSERMAKER_OPERATOR(make_operator_shl_id, "operator<<")
FOGPARSERMAKER_OPERATOR(make_operator_shr_id, "operator>>")
FOGPARSERMAKER_OPERATOR(make_operator_sub_id, "operator-")
FOGPARSERMAKER_OPERATOR(make_operator_xor_id, "operator^")

CxxParameters* FogParserMaker::make_parameters(CxxParameters* aList, CxxParameter* anElement) {
	if (!aList) {
		aList = new FogListOfExpr;
		garbage_adopt(aList);
	}
	
	if (aList && anElement)
		aList->add(*anElement);
		
	return aList;
}

FOGPARSERMAKER_CCU(CxxParenthesised, make_parenthesised, new FogParenthesisExpr, CxxParameters, CxxCvQualifiers, CxxExceptionSpecification*)
FOGPARSERMAKER_ASSIGN(CxxExpression, make_plus_expression, CxxExpression)
FOGPARSERMAKER_VALUE(CxxPointerDeclarator, make_pointer_declarator, &FogPointerModifier::make())

CxxExpression* FogParserMaker::make_pointer_expression(CxxDeclarator* aDeclarator, CxxExpression* anExpr) {
	if (!aDeclarator || !anExpr)
		return 0;
		
	FogExpr* theExpr = 0;
	
	if (aDeclarator->is_reference())
		theExpr = new FogAddrExpr(*anExpr);
	else
		theExpr = new FogStarExpr(*aDeclarator, *anExpr);
		
	garbage_adopt(theExpr);
	return theExpr;
}

//  FOGPARSERMAKER_ASSIGN(CxxPosition, make_position, CxxPosition)
//  FOGPARSERMAKER_APPLY_C(CxxFunctionBody, make_position_function_block, CxxFunctionBody, set_position, CxxPosition)
FOGPARSERMAKER_C(CxxExpression, make_post_decrement_expression, new FogPostDecExpr, CxxExpression)
FOGPARSERMAKER_C(CxxExpression, make_post_increment_expression, new FogPostIncExpr, CxxExpression)
FOGPARSERMAKER_C(CxxExpression, make_pre_decrement_expression, new FogPreDecExpr, CxxExpression)
FOGPARSERMAKER_C(CxxExpression, make_pre_increment_expression, new FogPreIncExpr, CxxExpression)

CxxName* FogParserMaker::make_pseudo_destructor_id(CxxBuiltInId* aScope, CxxBuiltInId* aName) {
	return make_nested_id(aScope, make_destructor_id(aName));
}

FOGPARSERMAKER_VALUE(CxxDeclSpecifierId, make_pure_virtual, &FogVirtual::pure_virtual())
FOGPARSERMAKER_VALUE(CxxDeclarator, make_reference_declarator, &FogReferenceModifier::make())
FOGPARSERMAKER_CC(CxxExpression, make_reinterpret_cast_expression, new FogReinterpretCastExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_U(CxxStatement, make_return_statement, new FogReturnStatement, CxxExpression*)
FOGPARSERMAKER_ERROR(CxxExpression * FogParserMaker::make_scoped_pointer_expression(CxxExpression* aScope, CxxDeclarator* aDeclarator, CxxExpression* anExpr))
FOGPARSERMAKER_ASSIGN(CxxSegment, make_segment, CxxSegment)
FOGPARSERMAKER_APPLY_C(CxxFunctionBody, make_segment_function_block, CxxFunctionBody, set_segment, CxxSegment)
FOGPARSERMAKER_ERROR(CxxDeclSpecifierId * FogParserMaker::make_set_template_decl_specifier(CxxDeclSpecifierId* aName))
FOGPARSERMAKER_ERROR(CxxDeclaration * FogParserMaker::make_set_template_declaration(CxxDeclaration* aDeclaration))
FOGPARSERMAKER_ERROR(CxxExpression * FogParserMaker::make_set_template_expression(CxxExpression* anExpr))
FOGPARSERMAKER_ERROR(CxxName * FogParserMaker::make_set_template_id(CxxName* aName))
FOGPARSERMAKER_ERROR(CxxName * FogParserMaker::make_set_template_name(CxxName* aName))
FOGPARSERMAKER_CC(CxxExpression, make_shift_left_expression, new FogShlExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_shift_right_expression, new FogShrExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_C(CxxDeclaration, make_simple_declaration, new FogExpressionStatement, CxxExpression)
FOGPARSERMAKER_C(CxxExpression, make_sizeof_expression, new FogSizeofExpr, CxxExpression)
FOGPARSERMAKER_LIST(make_statements, CxxStatements, CxxStatement)
FOGPARSERMAKER_CC(CxxExpression, make_static_cast_expression, new FogStaticCastExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_ASSIGN(CxxExpression, make_string_literal_expression, CxxStrings)
FOGPARSERMAKER_LIST(make_subspace, CxxSubspace, CxxDeclaration)
FOGPARSERMAKER_CC(CxxExpression, make_subtract_expression, new FogSubExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxStatement, make_switch_statement, new FogSwitchStatement, CxxCondition, CxxStatement)
FOGPARSERMAKER_ERROR(CxxMetaFunction * FogParserMaker::make_syntax_macro_definition(CxxMetaType* metaType, CxxIsExposed isExposed, CxxName* aName,
                     CxxSyntaxMacroParameters* aList, CxxToken* aBody))
FOGPARSERMAKER_ERROR(CxxSyntaxMacroParameter * FogParserMaker::make_syntax_macro_parameter(CxxToken* metaType, CxxIsExposed isExposed, CxxName* aName))
FOGPARSERMAKER_LIST(make_syntax_macro_parameters, CxxSyntaxMacroParameters, CxxSyntaxMacroParameter)
FOGPARSERMAKER_ASSIGN(CxxTemplateArgument, make_template_argument, CxxParameter)
FOGPARSERMAKER_LIST(make_template_arguments, CxxTemplateArguments, CxxTemplateArgument)
FOGPARSERMAKER_CC(CxxDeclaration, make_template_declaration, new FogRawTemplate, CxxTemplateParameters, CxxDeclaration)
FOGPARSERMAKER_CC(CxxName, make_template_name, new FogTemplatedName, CxxName, CxxTemplateArguments)

FOGPARSERMAKER_ERROR(CxxTemplatedTypeParameter * FogParserMaker::make_templated_type_parameter(CxxTemplateParameters* templateParameters, CxxName* aName))
#if 0
CxxTemplatedTypeParameter* FogParserMaker::make_templated_type_parameter(CxxTemplateParameters* templateParameters,
        CxxName* aName) {
	if (!templateParameters || !aName)
		return 0;
		
	CxxTemplatedTypeParameter* typeParameter = new FogTemplatedParameterSpecifier(
	        garbage_adopt(typeParameter);
	        return typeParameter;
}
#endif
FOGPARSERMAKER_ASSIGN(CxxTemplateParameter, make_template_parameter, CxxParameter)
FOGPARSERMAKER_LIST(make_template_parameters, CxxTemplateParameters, CxxTemplateParameter)
FOGPARSERMAKER_(CxxExpression, make_this_expression, new FogThisExpr)
FOGPARSERMAKER_U(CxxExpression, make_throw_expression, new FogThrowExpr, CxxExpression*)
//  FOGPARSERMAKER_ERROR(CxxExpression *FogParserMaker::make_tokens_expression(CxxTokens *someTokens))

CxxExpression* FogParserMaker::make_tokens_expression(CxxTokens* someTokens) {
	if (!someTokens)
		return 0;
		
	CxxExpression* anExpr = new FogTokensExpr(*someTokens);
	garbage_adopt(anExpr);
	return anExpr;
}

FOGPARSERMAKER_ASSIGN(CxxToken, make_tree_argument, CxxToken)

CxxTokens* FogParserMaker::make_tree_arguments(CxxTokens* aList, CxxToken* anElement) {
	if (!aList) {
		aList = new FogTokens(IS_EXPOSED);
		garbage_adopt(aList);
	}
	
	if (aList && anElement)
		aList->add(*anElement);
		
	return aList;
}

CxxTokens* FogParserMaker::make_tree_arguments(CxxTokens* aList, CxxTokens* anotherList) {
	if (!aList)
		return anotherList;
		
	if (!anotherList)
		return aList;
		
	for (FogTokenConstListOfRefIterator p(anotherList->tokens()); p; ++p)
		aList->add(*p);
		
	return aList;
}

FOGPARSERMAKER_CC(CxxExpression, make_tree_array_expression, new FogIndexExpr, CxxExpression, CxxExpression)
FOGPARSERMAKER_CC(CxxExpression, make_tree_arrow_expression, new FogArrowExpr, CxxExpression, CxxName)
FOGPARSERMAKER_CC(CxxExpression, make_tree_call_expression, new FogTreeCallExpr, CxxExpression, CxxTokens)
FOGPARSERMAKER_CC(CxxExpression, make_tree_dot_expression, new FogDotExpr, CxxExpression, CxxName)
FOGPARSERMAKER_ASSIGN(CxxExpression, make_tree_expression, CxxName)

CxxName* FogParserMaker::make_tree_id(CxxName* aName) {
	garbage_add(aName);
	return aName;
}

CxxExpression* FogParserMaker::make_tree_pointer_expression(CxxExpression* anExpr) {
	return make_pointer_expression(&FogPointerModifier::make(), anExpr);
}

CxxTokenStatements* FogParserMaker::make_tree_statements(CxxTokens* aList, CxxToken* anElement) {
	CxxTokenStatements* theList = new FogTokenStatements(FogMetaType::token_type(), aList);
	garbage_adopt(theList);
	
	if (theList && anElement) {
		FogTokens* tokenList = anElement->is_tokens();
		
		if (tokenList)                                  //  .bugbug should not have got confused while parsing.
			theList->add(tokenList->tokens());
		else
			theList->add(*anElement);
	}
	
	return theList;
}

FOGPARSERMAKER_VALUE(CxxExpression, make_true_expression, &FogNumber::make_true())
FOGPARSERMAKER_CC(CxxStatement, make_try_block, new FogTryStatement, CxxStatement, CxxHandlers)
FOGPARSERMAKER_APPLY_C(CxxFunctionBody, make_try_function_block, CxxFunctionBody, set_handlers, CxxHandlers)
FOGPARSERMAKER_ERROR(CxxExpression * FogParserMaker::make_type1_expression(CxxExpression* functionName, CxxParenthesised* aParenthesis, CxxType1Parameters* type1Parameters))
FOGPARSERMAKER_ERROR(CxxTemplateParameter * FogParserMaker::make_type_template_parameter(CxxSimpleTypeParameter* typeParameter, CxxExpression* typeId))

CxxExpression* FogParserMaker::make_typed_expression(CxxName* typeName, CxxExpression* theExpr) {
	if (!typeName || !theExpr)
		return 0;
		
	FogExprRef netExpr(*theExpr);
	
	if (!theExpr->make_typed_expression(netExpr, *typeName))
		return 0;
		
	if (netExpr.pointer() != theExpr)
		garbage_add(netExpr.pointer());
		
	return netExpr.pointer();
}

CxxName* FogParserMaker::make_typed_name(CxxName* frontName, CxxName* backName) {
	if (!frontName || !backName)
		return 0;
		
	FogNameRef theName(*frontName);
	FogTypedName::append_name(theName, *backName);
	
	if (theName.pointer() != frontName)
		garbage_add(theName.pointer());
		
	return theName.pointer();
}

FOGPARSERMAKER_C(CxxExpression, make_typeid_expression, new FogTypeidExpr, CxxExpression)

CxxSimpleTypeParameter* FogParserMaker::make_typename_type_parameter(CxxName* aName) {
	CxxSimpleTypeParameter* typeParameter = new FogElaboratedTypeSpecifier(FogTag::typename_tag(),
	        aName ? *aName : FogName::make_anon());
	garbage_adopt(typeParameter);
	return typeParameter;
}

FOGPARSERMAKER_ERROR(CxxType1Parameters * FogParserMaker::make_type1_parameters(CxxType1Parameters* aList, CxxParameters* someParameters))
FOGPARSERMAKER_ERROR(CxxDeclaration * FogParserMaker::make_using_declaration(bool isTypename, CxxName* aName))
FOGPARSERMAKER_C(CxxDeclaration, make_using_directive, new FogUsingSpecifier, CxxName)
FOGPARSERMAKER_APPLY_C(CxxFunctionBody, make_using_function_block, CxxFunctionBody, set_using_files, CxxFileIds)
FOGPARSERMAKER_C(CxxDeclaration, make_using_implementation_declaration, FogRaw::new_use_implementation, CxxFileId)
FOGPARSERMAKER_C(CxxDeclaration, make_using_interface_declaration, FogRaw::new_use_interface, CxxFileId)
FOGPARSERMAKER_ASSIGN(CxxUtility, make_utility, CxxUtility)
FOGPARSERMAKER_VALUE(FogUtility, make_utility_mode, &utility_mode().mutate())
FOGPARSERMAKER_APPLY(CxxBaseSpecifier, make_virtual_base_specifier, CxxBaseSpecifier, set_virtual(FogVirtual::yes_virtual()))
FOGPARSERMAKER_CC(CxxStatement, make_while_statement, new FogWhileStatement, CxxCondition, CxxStatement)
