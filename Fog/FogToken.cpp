#include <Fog/FogIncludeAll.h>



























TYPEINFO_SINGLE(FogToken, Super)
PRIMSHARES_IMPL(FogToken)
PRIMREF_NULL_CLAS(FogToken)
typedef FogToken Super;
FOGTOKEN_SHAREDLEAF_DECLS
virtual FogTokenType::TokenType pp_token_type_enum() const { return FogTokenType::init(); }
virtual FogTokenType::TokenType token_type_enum() const { return FogTokenType::init(); }
};
PRIMREF_NULL_IMPL(FogToken)
PRIMREFS_IMPL(FogToken)
PRIMLISTOFREFS_IMPL(FogToken)
TMPL_HACK_FIX_DO(FogToken)

FOGTOKEN_SHAREDLEAF_IMPL(FogTokenNull)
//TMPL_HACK_FIX_DO(FogTokenNull)

//
//    Add a dependent usage of this entity by usageContext.
//
void FogToken::add_use_by(FogEntity& anEntity, const FogOfUseBy& ofUseBy) {
	if (!is_null())
		ERRMSG("BUG -- should not add_use_by for " << viz(*this));
}

//
//    Add metaEntity to this token, retuyrning it if it has been accepted.
//    Returns 0 if adoption failed, and metaEntity was annulled.
//
FogMetaEntity* FogToken::adopt_meta_entity(FogMetaEntity* metaEntity) {
	//    if (!is_null())
	ERRMSG("BUG -- should not adopt_meta_entity for " << viz(*this));
	
	if (metaEntity)
		metaEntity->annul();
		
	return 0;
}

//
//    Incorporate aSlot into this object. Default error handling implementation diagnoses
//    the anomally, annuls the object and returns it is 0.
//
const FogMetaSlot* FogToken::adopt_slot(const FogMetaSlot* aSlot) {
	ERRMSG("BUG -- should not adopt_slot for " << viz(*this));
	
	if (aSlot)
		aSlot->annul();
		
	return 0;
}

//
//    Create and return a clone of this token.
//
FogToken* FogToken::clone() const { return error_clone(); }

bool FogToken::compile_declaration(FogParseContext& parseContext) const {
	FogTokenRef actualToken;
	make_actual(actualToken.to_const(), parseContext);
	return actualToken->compile_parsed(parseContext);
}

bool FogToken::compile_parsed(FogParseContext& parseContext) {
	if (!is_null())
		ERRMSG("BUG -- should not compile_parsed for " << viz(*this));
		
	return false;
}

bool FogToken::compile_parsed_case(FogParseContext& parseContext, const FogToken& theCase) {
	if (!is_null())
		ERRMSG("BUG -- should not compile_parsed_case for " << viz(*this));
		
	return false;
}

//
//    Eliminate all references so that destruction is complete and static destruction well behaved.
//
void FogToken::destroy() {}

//
//    Print the actual lexical value of this token to abd as required by emitContext.
//    Returns true if anything emitted, enabling the degenerate empty emission to be detected by the caller.
//
bool FogToken::emit(FogEmitContext& emitContext) const {
	if (!is_null())
		ERRMSG("BUG -- should not emit for " << viz(*this));
		
	return false;
}

//
//    Print the actual lexical value of this token starting at theCase in a switch-atateenmt..
//    Returns true if anything emitted, enabling the degenerate empty emission to be detected by the caller.
//
bool FogToken::emit_case(FogEmitContext& emitContext, const FogToken& theCase) const {
	if (!is_null())
		ERRMSG("BUG -- should not emit_case for " << viz(*this));
		
	return false;
}

//
//    Genertae a cloning error message.
//
FogToken* FogToken::error_clone() const {
	ERRMSG("BUG -- should not invoke FogToken::clone() for " << viz(*this));
	return 0;
}

void FogToken::error_merge(FogMergeContext& mergeContext, const FogToken& aToken) const {
	ERRMSG("BUG -- should not invoke FogToken::error_merge() for " << viz(*this));
}

const FogMerge& FogToken::error_needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const {
	if (this == &aToken)
		return FogMerge::both_valid();
		
	//    ERRMSG("BUG -- should not invoke FogToken::error_needs_merge() for " << viz(*this));
	if (aToken.is_null())
		return is_null() ? FogMerge::both_valid() : FogMerge::right_invalid();
	else
		return is_null() ? FogMerge::left_invalid() : impossible_merge();
}

//
//    Return the number of executable tokens contributed by this token value. This gives a crude estimate
//    of the code complexity to support automatic inlining decisions.
//
size_t FogToken::executable_tokens() const { return 1; }

//
//    Accumulate all entities satisfying FogEntityFinder within the scope of this token.
//
void FogToken::find_entities(FogEntityFinder& theFinder) {
	ERRMSG("BUG -- should not find_entities for " << viz(theFinder) << " in " << viz(*this));
}

//
//    Accumulate all entities named by this token in FogEntityFinding within the scope of inScope.
//
void FogToken::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const
{ ERRMSG("BUG - Should not invoke FogToken::find_entities_in for " << viz(*this)); }

//
//    Return the aStrategy determined name or type described by this token within scopeContext,
//        without generating an error message if not found.
//
FogEntity* FogToken::find_entity_in(FogScopeContext& scopeContext, FindStrategy aStrategy) const {
	FogEntityFinding entityFinding(aStrategy);
	find_entities_in(scopeContext, entityFinding);
	return entityFinding.get_unambiguous_finding(*this, scopeContext);
}

//
//    Locate an slot matching anId.
//
const FogMetaSlot* FogToken::find_local_slot(const PrimId& anId) const {
	return meta_type().find_meta_slot(*this, anId);
}

//
//    Locate the met-entity named anId visible within this scope.
//
FogMetaEntity* FogToken::find_meta_entity(const PrimId& anId) {
	return 0;
}

//
//    Add all slots satisfying FogMetaSlotFinding.
//
bool FogToken::find_slots(FogMetaSlotFinder& theFinding) {
	const FogMetaSlot* aSlot = find_local_slot(theFinding.id());
	
	if (!aSlot)
		return false;
		
	theFinding.add(*this, *aSlot);
	return true;
}

//
//    Return the value of this tree as a (mult-byte)character.
//
//    Leaf implementations return their character value.
//    The default implementation return the result of applying get_character to the get_object return.
//    Optimised derived implementations must satisfy the identity that get_character = get_object then get_character.
//
bool FogToken::get_character(PrimIdHandle& returnId, FogScopeContext& inScope) {
	FogTokenRef tokenValue;
	
	if (get_object(tokenValue, inScope) && (tokenValue.pointer() != this))
		return tokenValue->get_character(returnId, inScope);
		
	ERRMSG("Failed to get_character of " << viz(*this));
	returnId = PrimId::null();
	return false;
}

//  ********Derived classes should note that their existence may be transient and be terminated by the
//    assignment to returnValue, which should therefore be delayed until the object is done.
//
bool FogToken::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                          FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const {
	FogTokenRef tempValue;
	
	if (get_object(tempValue, inScope) && (tempValue.pointer() != this))
		return tempValue->get_dyadic(returnValue, inScope, dyadicOp, rightValue);
		
	switch (dyadicOp) {
	case '=':
		returnValue = (FogToken&)rightValue;        //  .bugbug cast ?
		return true;
		
	default:
		ERRMSG("Unsupported dyadic operation " << viz(FogTokenType::get(dyadicOp))
		       << " of " << viz(rightValue) << " applied to " << viz(*this));
		returnValue = FogFailure::make();
		return false;
	}
}

//
//    Return the aStrategy determined name or type described by this token within scopeContext,
//        generating an error message if not found.
//
FogEntity* FogToken::get_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const {
	FogEntity* anEntity = find_entity_in(inScope, aStrategy);
	
	if (!anEntity)
		ERRMSG("Failed to locate " << FogEntityFinding::strategy_text(aStrategy)
		       << " " << viz(*this) << " in " << viz(inScope));
		       
	return anEntity;
}

//
//    Return the value of this tree as an identifier to returnId, and a success indication by value.
//
//    Leaf implementations return their identifier value.
//    The default implementation return the result of applying get_identifier to the get_object return.
//    Optimised derived implementations must satisfy the identity that get_identifier = get_object then get_identifier.
//
//    Derived implementation may return false for a premature attempt to resolve a $expression, provided that there
//    is some chance that a later evaluation may return true.
//
bool FogToken::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	FogTokenRef tokenValue;
	
	if (get_object(tokenValue, inScope) && (tokenValue.pointer() != this))
		return tokenValue->get_identifier(returnId, inScope);
		
	ERRMSG("Failed to get_identifier of " << viz(*this));
	returnId = PrimId::null();
	return false;
}

//  ********Derived classes should note that their existence may be transient and be terminated by the
//    assignment to returnValue, which should therefore be delayed until the object is done.
//
bool FogToken::get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                           FogTokenType::TokenType monadicOp) const {
	FogTokenRef tempValue;
	
	if (get_object(tempValue, inScope) && (tempValue.pointer() != this))
		return tempValue->get_monadic(returnValue, inScope, monadicOp);
		
	switch (monadicOp) {
	case '=':                       //   Monadic = is used to access a MetaVariable (a counterpart to dyadic f(args))
		returnValue = mutate();
		return true;
		
	default:
		ERRMSG("Unsupported monadic operation " << viz(FogTokenType::get(monadicOp)) << " applied to " << viz(*this));
		returnValue = FogFailure::make();
		return false;
	}
}

//
//    Return the numeric value of this token evaluated inScope.
//    Generates an error message and returns null on error.
//    Derived classes may provide a faster implementation than that obtained by the
//    default approach that invokes get_number_token.
//
const PrimNumber& FogToken::get_number(FogScopeContext& inScope) const {
	FogTokenRef tokenValue;
	
	if (get_number_token(tokenValue, inScope))
		return tokenValue->number().number_value();
		
	ERRMSG("Failed to get_number of " << viz(*this));
	return PrimNumber::null();
}

//
//    Return the FogNumber numeric value of this token evaluated inScope to returnValue.
//    Generates an error message and returns false on error.
//    Derived classes may provide a faster implementation than that obtained by the
//    default approach that invokes get_object to find the value token and then
//    get_number_value to evaluate at the value token. Derived value tokens must
//    reimplement.
//
bool FogToken::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	FogTokenRef tempValue;
	
	if (get_object(tempValue, inScope) && (tempValue.pointer() != this)
	        && tempValue->get_number_token(returnValue, inScope))
		return true;
		
	if (inScope.requires_resolution())
		ERRMSG("Failed to get_number_token of " << viz(*this));
		
	returnValue = FogFailure::make();
	return false;
}

//
//    Convert an expression into the same expression, but traverse any proxies, decorators. This avoids
//    derived classes having to reimplement get_character etc which can safely be resolved by the default
//    get_object then get_character. get_object also evaluates any explicit tree-literals associated with
//    an explicit inScope.
//
//  ********Derived classes should note that their existence may be transient and be termimnated by the
//    assignment to returnValue, which should therefore be delayed until the object is done.
//
bool FogToken::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	returnValue = mutate();             //  .bugbug cast ?
	return true;
}

//
//    Return the value of this tree as a string.
//
//    Leaf implementations return their string value.
//    The default implementation return the result of applying get_string to the get_object return.
//    Optimised derived implementations must satisfy the identity that get_string = get_object then get_string.
//
bool FogToken::get_string(PrimIdHandle& returnId, FogScopeContext& inScope) {
	FogTokenRef tokenValue;
	
	if (get_object(tokenValue, inScope) && (tokenValue.pointer() != this))
		return tokenValue->get_string(returnId, inScope);
		
	ERRMSG("Failed to get_string of " << viz(*this));
	returnId = PrimId::null();
	return false;
}

//
//    Return the name of the type of this tree.
//
FogName* FogToken::get_type() {
	ERRMSG("Should not get_type for " << viz(*this));
	return 0;
}

//
//    Return the global scope, which is unknwon for a token.
//
FogRoot& FogToken::global_scope() {
	ERRMSG("BUG - should not global_scope() for " << viz(*this));
	return FogRoot::mutable_null();
}

//
//    Return the kind of merge failure to be reported by needs_merge.
//
const FogMerge& FogToken::impossible_merge() const {
	return FogMerge::set_incompatible(FogMerge::incompatible_declarator());
}

//
//    Return the scope of this entity, which is this for a scope, or outer_scope for a non-scope.
//
FogScope& FogToken::inner_scope() {
	//      ERRMSG("BUG - should not inner_scope() for " << viz(*this));
	return FogRoot::mutable_null();
}

//
//    install() forms the second compilation pass, and is invoked from the overall
//    compilation pass to install definitions for all type references.
//
//  void FogToken::install(FogInstallContext& installContext) const {}

//
//    Return true if this token satisfies the requirements of an actual declaration.
//    That is, it is free of string and identifier expressions.
//
//  bool FogToken::is_actual(const FogScopeContext& scopeContext) const { return true; }

const FogArrayModifier* FogToken::is_array_modifier() const { return 0; }
FogBaseClass* FogToken::is_base_class() { return 0; }
const FogSpacing* FogToken::is_brace() const { return 0; }

//
//    Return the non-0 case argument if the is a case statement matching theCase, null for the default case.
//
FogRaw* FogToken::is_case(FogScopeContext& scopeContext, const FogToken& theCase) { return 0; }

const FogCharacter* FogToken::is_character() const { return 0; }
FogClass* FogToken::is_class() { return 0; }

//
//    Return true if this will be surrounded by {} when emitted.
//
bool FogToken::is_compound() const { return false; }

FogDerivable* FogToken::is_derivable() { return 0; }
FogEnum* FogToken::is_enum() { return 0; }
FogEntity* FogToken::is_entity() { return 0; }
const FogEnumSpecifier* FogToken::is_enum_specifier() const { return 0; }
FogEnumerator* FogToken::is_enumerator() { return 0; }
const FogEnumeratorSpecifier* FogToken::is_enumerator_specifier() const { return 0; }
FogToken::IsExposed FogToken::is_exposed() const { return IS_ENCAPSULATED; }
FogExpr* FogToken::is_expr() { return 0; }
FogFailure* FogToken::is_failure() { return 0; }
FogFunction* FogToken::is_function() { return 0; }
const FogFunctionModifier* FogToken::is_function_modifier() const { return 0; }
FogIterationDomain* FogToken::is_iteration_domain() { return 0; }
FogLine* FogToken::is_line() const { return 0; }
FogToken::IsList FogToken::is_list() const { return IS_NOT_LIST; }
FogListOfExpr* FogToken::is_list_of_expr() { return 0; }
FogMetaEntity* FogToken::is_meta_entity() { return 0; }
FogMetaFunctionEntity* FogToken::is_meta_function_entity() { return 0; }
const FogMetaFunctionSlot* FogToken::is_meta_function_slot() const { return 0; }
FogMetaIteratorEntity* FogToken::is_meta_iterator_entity() { return 0; }
const FogMetaSlot* FogToken::is_meta_slot() const { return 0; }
FogMetaType* FogToken::is_meta_type() { return 0; }
FogMetaVariableEntity* FogToken::is_meta_variable_entity() { return 0; }
const FogMetaVariableSlot* FogToken::is_meta_variable_slot() const { return 0; }
FogModifier* FogToken::is_modifier() { return 0; }
FogName* FogToken::is_name() { return 0; }
FogClass* FogToken::is_namespace() { return 0; }
FogNumber* FogToken::is_number() { return 0; }
FogPotentialDeclaration* FogToken::is_potential_declaration() { return 0; }
FogRaw* FogToken::is_raw() { return 0; }
FogRaws* FogToken::is_raw_list() { return 0; }
FogScope* FogToken::is_scope() { return 0; }
const FogSpacing* FogToken::is_spacing() const { return 0; }
FogSpecifier* FogToken::is_specifier() { return 0; }
const FogString* FogToken::is_string() const { return 0; }
FogTemplateParameterSpecifier* FogToken::is_template_parameter_specifier() { return 0; }
FogTokens* FogToken::is_tokens() { return 0; }
FogTypedef* FogToken::is_typedef() { return 0; }
FogVariable* FogToken::is_variable() { return 0; }
const FogToken* FogToken::is_white() const { return 0; }
bool FogToken::is_wide() const { return false; }
const PrimId* FogToken::is_word() const { return 0; }

FogLine& FogToken::line() const {
	FogLine* aLine = is_line();
	
	if (aLine)
		return *aLine;
		
	ERRMSG("BUG - should not invoke line() for " << *this);
	return FogLine::mutable_null();
}

//
//    Participate in the make_actual activity by converting the children of this clone to be actuals.
//
//  void FogToken::make_actual_from(FogMakeActualContext& makeActualContext) {}
void FogToken::make_actual(PrimRefToConst<This>& actualToken, FogMakeActualContext& makeActualContext) const {
	if (is_actual(makeActualContext))
		actualToken = *this;        //   Usually redundant, but necessary if a MetaSlot traversed.
	else
		make_actual_token(actualToken, makeActualContext);
}

//
//    Assign a version of this token to actualToken that satisfies the requirements of
//    an actual declaration by resolving any identifier expressions within makeContext. If at all possible
//    part or all of this token is returned directly.
//
//    Derived classes such as FogEntityName that may need to morph to a different class reimplement.
//    Derived classes that merely need to customise members such reimplement make_actual_from.
//
void FogToken::make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const {
	FogToken* clonedToken = clone();
	
	if (!clonedToken)
		ERRMSG("BUG - failed to make_actual from non-actual " << viz(*this));
	else {
		actualToken.adopt(*clonedToken);
		clonedToken->make_actual_from(makeActualContext);
	}
}

//
//    Create and assign the specifier for a potential meta-object to and in accordance with makeSpecifierContext.
//
bool FogToken::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	ERRMSG("BUG -- should not make_specifier for " << viz(*this) << " in " << viz(makeSpecifierContext));
	return false;
}

//
//    Update this token by merging from aToken. This virtual method should be implemented by each leaf-most
//    derived class with mergeable datums to invoke a local type-specific merge_from to ascend the
//    inheritance hoerarchy to perform the complete merge.
//
//  void FogToken::merge(FogMergeContext& mergeContext, const FogToken& aToken) { merge_from(aToken); }

//
//    Perform a consistency check for a merged boolean.
//
bool FogToken::merge_bool(const char* someText, bool thisBool, bool thatBool) const {
	if (thisBool != thatBool)
		ERRMSG("BUG - should not merge conflicting \"" << someText << "\" for " << viz(*this));
		
	return thisBool;
}

//
//    Perform a consistency check for a merged id.
//
const PrimId& FogToken::merge_id(const char* someText, const PrimId& thisId, const PrimId& thatId) const {
	if (thisId != thatId)
		ERRMSG("BUG - should not merge conflicting \"" << someText << "\" for " << viz(*this));
		
	return thisId;
}

//
//    Perform a consistency check for a merged int.
//
int FogToken::merge_int(const char* someText, int thisInt, int thatInt) const {
	if (thisInt != thatInt)
		ERRMSG("BUG - should not merge conflicting \"" << someText << "\" for " << viz(*this));
		
	return thisInt;
}

//
//    Update intoDecl to point at an instance comprising the merge of the *aDecl and *this trees.
//    If possible one of other of *aDecl or *this is returned to avoid the expense of creating a new object.
//    Error messages are likely to arise and a null object may be returned if a merge is not possible.
//    needs_merge() should be invoked as a predicate to determine whether errors could arise.
//
void FogToken::merge_into(FogMergeContext& mergeContext, FogTokenRefToConst& aToken) const {
	if (aToken.pointer() == this)
		return;
		
	const FogMerge& needsMerge = aToken->needs_merge(mergeContext, *this);
	
	if (needsMerge.left_is_valid())
		;
	else if (needsMerge.right_is_valid())
		aToken = *this;
	else if (needsMerge.is_impossible()) {
		ERRMSG("BUG - cannot merge incompatible " << viz(*aToken) << " and " << viz(*this));
		
		if (Fog::debug_repeat())
			aToken->needs_merge(mergeContext, *this);
			
		aToken.reset();
	}
	else {
		if (needsMerge.is_incompatible()) {
			ERRMSG("Should not merge incompatible " << viz(*aToken) << " and " << viz(*this));
			
			if (Fog::debug_repeat())
				aToken->needs_merge(mergeContext, *this);
		}
		
		FogTokenRef clonedToken(aToken->clone(), FogTokenRef::ADOPT);
		
		if (clonedToken)
			clonedToken->merge(mergeContext, *this);
			
		aToken = clonedToken;
	}
}

//
//    Update those elements within the larger tally of intoList and fromList to be the element-wise
//    merge of the two lists. Under-tally elements from intoList are copied fromList. Under-tally
//    elements fromList do not affect the corresponding intoList elements.
//
void FogToken::merge_long_into(FogMergeContext& mergeContext,
                               PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList) {
	intoList.set_tally(std::max(intoList.tally(), fromList.tally()));
	PrimConstListOfRefToConstIterator<This> p(fromList);
	PrimListOfRefToConstIterator<This> q(intoList);
	
	for ( ; p && q; ++p, ++q)
		if (p->is_null())
			;
		else if (q->is_null())
			q.ref() = *p;
		else
			p->merge_into(mergeContext, q.ref());
			
	for ( ; p; ++p, ++q)
		q.ref() = *p;
}

//
//    Update those elements within the common tally of intoList and fromList to be the element-wise
//    merge of the two lists. Over-tally elements from either list are ignored in fromList and
//    unaffected in toList.
//
void FogToken::merge_short_into(FogMergeContext& mergeContext,
                                PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList) {
	PrimListOfRefToConstIterator<This> q(intoList);
	PrimConstListOfRefToConstIterator<This> p(fromList);
	
	for ( ; p && q; ++p, ++q)
		p->merge_into(mergeContext, q.ref());
}

//
//    Return the meta-type defining the characteristics of this token.
//
const FogMetaType& FogToken::meta_type() const { return FogMetaType::token_type(); }

bool FogToken::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                        FogScopeContext& inScope) const {
	//  .bugbug list exposure
	//    if (!is_actual(inScope))
	//    {
	//        FogTokenRefToConst actualToken;
	//        make_actual(actualToken, inScope);
	//        if (!actualToken->is_actual(inScope))   -- may still not be actual if a meta-argument
	//        {
	//            ERRMSG("Cannot actualize prior to morph of " << viz(*this) << " to " << viz(metaType));
	//            returnValue = FogFailure::make();
	//            return false;
	//        }
	//        return actualToken->morph_to(returnValue, metaType, inScope);
	//    }
	const FogMetaType& myMetaType = meta_type();
	
	if (myMetaType.is(metaType))
		returnValue = mutate();
	else if (metaType.is(FogMetaType::number_type())) {
		get_number_token(returnValue, inScope);
		returnValue = returnValue->number().make(metaType, inScope);
	}
	else if (metaType.is(FogMetaType::character_type())) {
		PrimIdHandle anId;
		
		if (mutate().get_character(anId, inScope))              //  .bugbug cast
			returnValue = FogCharacter::make(*anId, is_wide() ? FogCharacter::WIDE : FogCharacter::NARROW);
		else {
			ERRMSG("Cannot morph " << viz(*this) << " to " << viz(metaType));
			returnValue = FogFailure::make();
			return false;
		}
	}
	else if (metaType.is(FogMetaType::identifier_type())) {
		PrimIdHandle anId;
		
		if (mutate().get_identifier(anId, inScope))             //  .bugbug cast
			returnValue = FogIdentifier::make(*anId);
		else {
			ERRMSG("Cannot morph " << viz(*this) << " to " << viz(metaType));
			returnValue = FogFailure::make();
			return false;
		}
	}
	else if (metaType.is(FogMetaType::string_type())) {
		PrimIdHandle anId;
		
		if (mutate().get_string(anId, inScope))             //  .bugbug cast
			returnValue = FogString::make(*anId, is_wide() ? FogString::WIDE : FogString::NARROW);
		else {
			ERRMSG("Cannot morph " << viz(*this) << " to " << viz(metaType));
			returnValue = FogFailure::make();
			return false;
		}
	}
	else if (metaType.is(FogMetaType::entity_type())) {     //  .bugbug type_type first ?
		FogEntity* anEntity = find_entity_in(inScope);
		
		if (anEntity)
			returnValue = *anEntity;
		else {
			returnValue = FogFailure::make();
			return false;
		}
	}
	else if (metaType.is(FogMetaType::void_type())) {
		if (!is_null())
			//          {
			ERRMSG("Cannot morph " << viz(*this) << " to " << viz(metaType));
			
		returnValue = FogFailure::make();
		return false;
		//          }
		//          returnValue = mutate();
	}
	else if (metaType.satisfied_by(*this, inScope)) {
		returnValue = mutate();
		VERBOSE(std::strstream str1; returnValue->print_viz(str1); printf("DEBUG morph_to mutate %s\n", str1.str());)
	}
	else if (!inScope.requires_resolution()) {
		return false;
	}
	else {
		ERRMSG("Cannot morph " << viz(*this) << " to " << viz(metaType));
		returnValue = FogFailure::make();
		return false;
	}
	
	return true;
}


//
//    Return the scope in which the name of a child entity should be defined.
//
//    (That is a class/struct/union or namespace, but not extern "C", or filespace).
//
FogScope& FogToken::name_scope() {
	ERRMSG("BUG - should not name_scope() for " << viz(*this));
	return FogScope::mutable_null();
}

//
//    Return the name-space in which the name of a child entity should be defined.
//
//    (That is a namespace, but not class/struct/union, extern "C", or filespace).
//
FogScope& FogToken::name_space() {
	ERRMSG("BUG - should not name_space() for " << viz(*this));
	return FogScope::mutable_null();
}

//
//    Return an indication of the amount of merging possible and necessary to produce a composite token
//    tree from this token and aToken. No error messages arise,
//
const FogMerge& FogToken::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const {
	if (this == &aToken)
		return FogMerge::both_valid();
		
	//    else if (aToken.is_null())
	//        return is_null() ? FogMerge::both_valid() : FogMerge::right_invalid();
	//    else
	//        return is_null() ? FogMerge::left_invalid() : impossible_merge();
	if (!is_null() && aToken.is_null())
		ERRMSG("INVESTIGATE mismatched null for FogToken::needs_merge for " << viz(*this));
		
	if (is_null() && !aToken.is_null())
		ERRMSG("INVESTIGATE mismatched null for FogToken::needs_merge for " << viz(aToken));
		
	return impossible_merge();
}

const FogMerge& FogToken::needs_merge_exact(FogMergeContext& mergeContext,
        const PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList) {
	PrimConstListOfRefToConstIterator<This> p1(intoList);
	PrimConstListOfRefToConstIterator<This> p2(fromList);
	
	if (p1.to_go() != p2.to_go())
		return FogMerge::set_incompatible(FogMerge::incompatible_signature());
		
	FogMergeHandle needsMerge;
	
	for ( ; p1; ++p1, ++p2)
		needsMerge |= p1->needs_merge(mergeContext, *p2);
		
	return needsMerge;
}

FogNumber& FogToken::number() {
	FogNumber* aNumber = is_number();
	
	if (aNumber)
		return *aNumber;
		
	ERRMSG("BUG - should not invoke number() for " << *this);
	return FogNumber::null();
}

//
//    Return the token type for use at the pre-processor level.
//
FogTokenType::TokenType FogToken::pp_token_type_enum() const { return token_type_enum(); }

//
//    Contribute this token to a diagnostic print-out indented by aDepth.
//
std::ostream& FogToken::print_diagnostic(std::ostream& s, int aDepth) const {
	s << meta_type().id() << ' ';
	return print_viz(s);
}

std::ostream& FogToken::print_long_id(std::ostream& s) const {
	if (is_null())
		s << "/*null*/";
	else
		ERRMSG("BUG - should not print_long_id() for a " << class_name());
		
	return s;
}

char FogToken::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	ERRMSG("BUG - should not print_named() for a " << class_name());
	return tailChar;
}

//
//    Contribute this token to a source reconstruction indented by aDepth.
//
std::ostream& FogToken::print_source(std::ostream& s, int aDepth) const {
	return print_this(s);
}

//
//    Contribute this token to a stringized stream.
//
std::ostream& FogToken::print_stringized(std::ostream& s) const {
	return print_this(s);
}

std::ostream& FogToken::print_this(std::ostream& s) const {
	if (!is_null())
		print_named(s, 0, 0);
	else
		s << "/*null*/";
		
	return s;
}
//  {
//    if (is_null())
//        s << "/*null*/";
//    return Super::print_this(s);
//  }

//
//    Contribute this token to a diagnostic one token per line print-out.
//
std::ostream& FogToken::print_tokens(std::ostream& s, int aDepth) const {
	s << indent(aDepth);
	s << meta_type().id() << ' ';
	print_viz(s);
	return s << '\n';
}

std::ostream& FogToken::print_viz(std::ostream& s) const {
	return s << (is_null() ? "\"null-token\"" : "\"token: \"");
}

bool FogToken::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const {
	ERRMSG("BUG -- should not put_value to " << viz(*this) << " in " << viz(putScope));
	return false;
}

//
//    Assign the bit-mask of semantic hypotheses satisfied by this syntax tree when evaluated inScope, which may
//    be null to request potential rather than actual semantics. Returns false without necessarily assigning on failure.
//
bool FogToken::resolve_semantics(FogSemanticsContext& theSemantics) const {
	ERRMSG("BUG -- should not resolve_semantics from " << viz(*this) << " for " << viz(theSemantics));
	return false;
}

//
//    Eliminate any surrounding {}.
//
void FogToken::set_exposed() {}

//
//    Associate a line context with this syntax tree, but only if this tree is able to accommodate the information.
//
void FogToken::set_line(FogLine& aLine) {
	ERRMSG("Should not set_line for " << viz(*this));
}

const PrimId& FogToken::short_id() const {
	ERRMSG("BUG - should not short_id() from " << viz(*this));
	return PrimId::null();
}

//
//    Return the number of columns contributed by this token value.
//
size_t FogToken::source_columns() const {
	ERRMSG("BUG - should not source_columns() for " << viz(*this));
	return 0;
}

//
//    Return the number of lines contributed by this token value.
//
size_t FogToken::source_lines() const { return 0; }

//
//    Return the token type for use at the C++ level.
//
FogTokenType::TokenType FogToken::token_type_enum() const { return FogTokenType::ERROR; }

FogTokenListOfRef& FogToken::tokens() {
	ERRMSG("BUG - should not tokens() for " << viz(*this));
	static FogTokenListOfRef aList;
	return aList;
}

const FogTokenListOfRefToConst& FogToken::tokens() const {
	ERRMSG("BUG - should not tokens() for " << viz(*this));
	static const FogTokenListOfRefToConst aList;
	return aList;
}

const FogUtility& FogToken::utility() const {
	ERRMSG("BUG - should not utility() from " << viz(*this));
	return FogUtility::invalid_utility();
}
