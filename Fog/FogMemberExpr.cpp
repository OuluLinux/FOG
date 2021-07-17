#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogMemberExpr, Super)

FogMemberExpr::FogMemberExpr(FogExpr& anExpr, FogName& aMember)
	:
	Super(anExpr),
	_member(aMember)
{}

FogMemberExpr::FogMemberExpr(const This& anExpr)
	:
	Super(anExpr),
	_member(anExpr._member)
{}

FogMemberExpr::~FogMemberExpr() {}

void FogMemberExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_member->create_usage(aContext);
}

bool FogMemberExpr::emit(FogEmitContext& emitContext) const {
	return expr().emit(emitContext);
}

size_t FogMemberExpr::executable_tokens() const {
	return 1 + expr().executable_tokens() + _member->executable_tokens();
}

void FogMemberExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_member->install(installContext);
}

bool FogMemberExpr::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _member->is_actual(scopeContext);
}

void FogMemberExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_member->make_actual(_member, makeActualContext);
}

void FogMemberExpr::merge_from(FogMergeContext& mergeContext, const This& anExpr) {
	Super::merge_from(mergeContext, anExpr);
	anExpr._member->merge_into(mergeContext, _member);
}

const FogMerge& FogMemberExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	needsMerge |= _member->needs_merge(mergeContext, *anExpr._member);
	return needsMerge;
}

std::ostream& FogMemberExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_member->print_deep(s, aDepth + 1);
	return s;
}

std::ostream& FogMemberExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_member->print_on(s, aDepth + 1);
	return s;
}

char FogMemberExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	return Super::print_named(s, 0, tailChar);
}

bool FogMemberExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext exprSemantics(theSemantics);
	FogChildSemanticsContext memberSemantics(theSemantics);
	
	if (!expr().resolve_semantics(exprSemantics) || !_member->resolve_semantics(memberSemantics))
		return false;
		
	theSemantics.reset();
	
	if (exprSemantics.is_basic_expression() && memberSemantics.is_member_id_expression())
		theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
		
	return true;
}

#define FOGMEMBEREXPR_IMPLEMENTATION(T) \
	TYPEINFO_SINGLE(T, Super) \
	FOGTOKEN_LEAF_IMPL(T)

FOGMEMBEREXPR_IMPLEMENTATION(FogArrowExpr)
FOGMEMBEREXPR_IMPLEMENTATION(FogDotExpr)

bool FogArrowExpr::emit(FogEmitContext& emitContext) const {
	Super::emit(emitContext);
	emitContext.emit_space_and_text("->");
	member().emit(emitContext);
	return true;
}

bool FogArrowExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const {
	FogTokenRef tempValue;
	
	if (!expr().get_object(tempValue, inScope)) {
		tempValue = FogFailure::make();
		return false;
	}
	
	FogNestedScopeContext nestedContext(inScope, *tempValue);
	return tempValue->get_dyadic(tokenValue, nestedContext, FogTokenType::ARROW, member());
}

char FogArrowExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = Super::print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, "->");
	tailChar = member().print_named(s, 0, tailChar);
	return tailChar;
}

bool FogDotExpr::emit(FogEmitContext& emitContext) const {
	Super::emit(emitContext);
	emitContext.emit_space_and_text(".");
	member().emit(emitContext);
	return true;
}

bool FogDotExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const {
	FogTokenRef tempValue;
	
	if (!expr().get_object(tempValue, inScope)) {
		tempValue = FogFailure::make();
		return false;
	}
	
	FogNestedScopeContext nestedContext(inScope, *tempValue);
	return tempValue->get_dyadic(tokenValue, nestedContext, FogTokenType::character('.'), member());
}

char FogDotExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = Super::print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, ".");
	tailChar = member().print_named(s, 0, tailChar);
	return tailChar;
}
