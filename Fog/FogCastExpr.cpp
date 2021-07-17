#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogCastExpr, Super)

FogCastExpr::FogCastExpr(FogExpr& typeId, FogExpr& anExpr)
	:
	Super(anExpr),
	_cast(typeId)
{}

FogCastExpr::FogCastExpr(const This& anExpr)
	:
	Super(anExpr),
	_cast(anExpr._cast)
{}

FogCastExpr::~FogCastExpr() {}

void FogCastExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_cast->create_usage(aContext);
}

bool FogCastExpr::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text(prefix());
	_cast->emit(emitContext);
	emitContext.emit_space_and_text(midfix());
	expr().emit(emitContext);
	emitContext.emit_space_and_text(suffix());
	return true;
}

size_t FogCastExpr::executable_tokens() const {
	return 1 + expr().executable_tokens(); //   ignore _cast->executable_tokens()
}

void FogCastExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_cast->install(installContext);
}

bool FogCastExpr::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _cast->is_actual(scopeContext);
}

void FogCastExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_cast->make_actual(_cast.to_const(), makeActualContext);
}

void FogCastExpr::merge_from(FogMergeContext& mergeContext, const This& anExpr) {
	Super::merge_from(mergeContext, anExpr);
	anExpr._cast->merge_into(mergeContext, _cast.to_const());
}

const char* FogCastExpr::midfix() const { return ""; }

const FogMerge& FogCastExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	needsMerge |= _cast->needs_merge(mergeContext, *anExpr._cast);
	return needsMerge;
}

const char* FogCastExpr::prefix() const { return ""; }

std::ostream& FogCastExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_cast->print_deep(s, aDepth + 1);
	return s;
}

std::ostream& FogCastExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_cast->print_on(s, aDepth + 1);
	return s;
}

char FogCastExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, prefix());
	tailChar = _cast->print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, midfix());
	tailChar = Super::print_named(s, fullId, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, suffix());
	return tailChar;
}

bool FogCastExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext exprSemantics(theSemantics);
	FogChildSemanticsContext castSemantics(theSemantics);
	
	if (!expr().resolve_semantics(exprSemantics)
	        || !_cast->resolve_semantics(castSemantics))
		return false;
		
	theSemantics.reset();
	
	if (exprSemantics.is_basic_expression() && castSemantics.is_type_id())
		theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
		
	return true;
}

const char* FogCastExpr::suffix() const { return ""; }

#define FOGCASTEXPR_IMPLEMENTATION(T, P, M, S) \
	TYPEINFO_SINGLE(T, Super) \
	FOGTOKEN_LEAF_IMPL(T) \
	const char *T::midfix() const { return M; } \
	const char *T::prefix() const { return P; } \
	const char *T::suffix() const { return S; }

FOGCASTEXPR_IMPLEMENTATION(FogConstCastExpr, "const_cast<", ">(", ")")
FOGCASTEXPR_IMPLEMENTATION(FogDynamicCastExpr, "dynamic_cast<", ">(", ")")
//  FOGCASTEXPR_IMPLEMENTATION(FogExplicitCastExpr, "(", ")", "")
FOGCASTEXPR_IMPLEMENTATION(FogExplicitCastExpr, "", "", "")             //   Body is always a ParenthesisExpr.
FOGCASTEXPR_IMPLEMENTATION(FogIndexCastExpr, "[", "]", "")
FOGCASTEXPR_IMPLEMENTATION(FogReinterpretCastExpr, "reinterpret_cast<", ">(", ")")
FOGCASTEXPR_IMPLEMENTATION(FogStaticCastExpr, "static_cast<", ">(", ")")

void FogExplicitCastExpr::make_actual_token(FogTokenRefToConst& actualToken,
        FogMakeActualContext& makeActualContext) const {
	FogExpr& castExpr = mutate().cast();                                    //  .bugbug cast - ok make_actual copies
	FogStrongSemanticsContext castSemantics(makeActualContext);
	bool isCast = castExpr.resolve_semantics(castSemantics) && castSemantics.is_type_cast();
	FogParenthesisExpr* theArgs = mutate().expr().is_parenthesis_expr();    //  .bugbug cast - ok make_actual copies
	
	if (!isCast && theArgs) {
		actualToken.adopt(new FogCallExpr(castExpr, *theArgs));
		actualToken->make_actual(actualToken, makeActualContext);
	}
	
	Super::make_actual_token(actualToken, makeActualContext);
}

bool FogExplicitCastExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	if (!cast().make_specifier(makeSpecifierContext))
		return false;
		
	FogObjectSpecifier* objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
	
	if (objectSpecifier) {
		FogParenthesisExpr* parenthesisExpr = expr().is_parenthesis_expr();
		
		if (!parenthesisExpr)
			ERRMSG("BUG -- expected parenthesised expression for " << viz(expr()));
		else
			objectSpecifier->adopt(parenthesisExpr->new_function_modifier(makeSpecifierContext));
	}
	
	return true;
}

bool FogExplicitCastExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext exprSemantics(theSemantics);
	FogChildSemanticsContext castSemantics(theSemantics);
	
	if (!cast().resolve_semantics(castSemantics)
	        || !expr().resolve_semantics(exprSemantics))
		return false;
		
	theSemantics.reset();
	
	if (castSemantics.is_type_cast() && exprSemantics.is_basic_expression())
		theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
		
	if (exprSemantics.is_parameter_declaration_clause()) {
		if (castSemantics.is_function_declarator())
			theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR);
			
		if (castSemantics.is_function_declarator_id())
			theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR_ID);
			
		if (castSemantics.is_object_declarator_id())
			theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR_ID);
			
		if (castSemantics.is_parameter_declarator_opt())
			theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATOR_OPT);
			
		if (castSemantics.is_type_id_declarator_opt())
			theSemantics.set(FogSemantics::IS_TYPE_ID_DECLARATOR_OPT);
	}
	
	return true;
}

FogIndexCastExpr* FogIndexCastExpr::is_index_cast_expr() { return this; }

bool FogIndexCastExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext exprSemantics(theSemantics);
	FogChildSemanticsContext castSemantics(theSemantics);
	
	if (!expr().resolve_semantics(exprSemantics)
	        || !cast().resolve_semantics(castSemantics))
		return false;
		
	theSemantics.reset();
	
	if (castSemantics.is_constant_expression() && exprSemantics.is_assignment_expression())
		theSemantics.set(FogSemantics::IS_INITIALIZER_CLAUSE);
		
	return true;
}
