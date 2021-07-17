#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogCtorExpr, Super)
FOGTOKEN_LEAF_IMPL(FogCtorExpr)

bool FogCtorExpr::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text(" : ");
	expr().emit(emitContext);
	return true;
}

size_t FogCtorExpr::executable_tokens() const { return 1 + expr().executable_tokens(); }

bool FogCtorExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogToken& inToken = makeSpecifierContext.dynamic_token();
	FogPotentialDeclaration* potentialDeclaration = inToken.is_potential_declaration();
	FogDerivable* aDerivable = potentialDeclaration ? &potentialDeclaration->entity() : 0;
	FogActualDeclaration* actualDeclaration = aDerivable ? &aDerivable->actual_declaration() : 0;
	FogSpecifier* inSpecifier = actualDeclaration ? &actualDeclaration->mutable_specifier() : 0;
	FogObjectSpecifier* objectSpecifier = inSpecifier ? inSpecifier->is_object_specifier() : 0;
	
	if (objectSpecifier) {
		//          FogResolutionScopeContext weakContext(makeSpecifierContext, FogResolutionScopeContext::LOCATE_META_FUNCTION_FORMALS);
		//          FogExprRef initExpr;
		//          expr().make_actual(initExpr.to_const(), weakContext);
		//          objectSpecifier->set_colon_value(*initExpr);
		objectSpecifier->set_colon_value(expr());
	}
	else
		ERRMSG("Expected object-specifier as context for " << viz(*this));
		
	return true;
}

char FogCtorExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, " : ");
	tailChar = expr().print_named(s, fullId, tailChar);
	return tailChar;
}

bool FogCtorExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext exprSemantics(theSemantics);
	
	if (!expr().resolve_semantics(exprSemantics))
		return false;
		
	theSemantics.reset();
	
	if (exprSemantics.is_basic_expression())
		theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);  //  .bugbug pragmatic way of triggering make_specifoer
		
	return true;
}