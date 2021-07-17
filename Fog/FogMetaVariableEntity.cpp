#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogMetaVariableEntity, Super)

FogMetaVariableEntity::FogMetaVariableEntity(FogToken& inToken, const FogMetaVariableSlot& metaObject)
	:
	Super(inToken, metaObject)
{}

FogMetaVariableEntity::~FogMetaVariableEntity() {}

void FogMetaVariableEntity::destroy() {
	_value.reset();
	Super::destroy();
}

bool FogMetaVariableEntity::emit(FogEmitContext& emitContext) const {
	return !_value->is_null() ? _value->emit(emitContext) : false;
}

bool FogMetaVariableEntity::get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                                        FogTokenType::TokenType monadicOp) const {
	FogTokenRef tempValue;
	
	if (get_object(tempValue, inScope) && (tempValue.pointer() != (FogToken*)this))
		return tempValue->get_monadic(returnValue, inScope, monadicOp);
		
	switch (monadicOp) {
	case '=':                       //   Monadic = is used to access a MetaVariable (a counterpart to dyadic f(args))
		returnValue = mutate();
		return true;
		
	default:
		return Super::get_monadic(returnValue, inScope, monadicOp);
	}
}

bool FogMetaVariableEntity::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	FogNestedScopeContext nestedScope(inScope, mutate().token());
	FogResolutionScopeContext resolutionScope(nestedScope, FogScopeContext::RESOLVE_ATS);
	FogTokenRef tempValue;
	_value->make_actual(tempValue, resolutionScope);
	return tempValue->morph_to(returnValue, type(), is_exposed(), inScope);
}

FogMetaVariableEntity* FogMetaVariableEntity::is_meta_variable_entity() { return this; }
//  const FogMetaType& FogMetaVariableEntity::meta_type() const { return FogMetaType::meta_variable_type(); }
const FogMetaType& FogMetaVariableEntity::meta_type() const { return FogMetaType::token_type(); }

std::ostream& FogMetaVariableEntity::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_value->print_deep(s, aDepth);
	return s;
}

std::ostream& FogMetaVariableEntity::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_value->print_on(s, aDepth);
	return s;
}

std::ostream& FogMetaVariableEntity::print_viz(std::ostream& s) const {
	s << "\"meta-variable: ";
	print_this(s);
	return s << '\"';
}

bool FogMetaVariableEntity::set_value(const FogToken& tokenValue, FogScopeContext& inScope) {
	VERBOSE(std::strstream str1; _value->print_viz(str1); printf("DEBUG FogMetaVariableEntity %x set_value pre value %s\n", this, str1.str());)
	bool returnStatus = tokenValue.morph_to(_value, type(), is_exposed(), inScope);
	VERBOSE(std::strstream str2; _value->print_viz(str2); printf("DEBUG FogMetaVariableEntity %x set_value post value %s\n", this, str2.str());)
	
	if (is_exposed() && !_value->is_exposed()) {
		FogTokenStatements* tokenList = new FogTokenStatements(type());
		
		if (tokenList) {
			tokenList->set_exposed();
			tokenList->add(*_value);
			_value.adopt(*tokenList);
			tokenList->set_line(tokenValue.line());
		}
	}
	
	return returnStatus;
}
