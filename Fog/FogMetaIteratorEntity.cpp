#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogMetaIteratorEntity, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogMetaIteratorEntity)
PRIMREFS_IMPL(FogMetaIteratorEntity)
TMPL_HACK_FIX_DO(FogMetaIteratorEntity)

FogMetaIteratorEntity::FogMetaIteratorEntity()
	:
	_index(0)
{}

FogMetaIteratorEntity::FogMetaIteratorEntity(FogToken& inToken, const FogMetaIterator& metaIterator)
	:
	Super(inToken, metaIterator),
	_index(0)
{}

FogMetaIteratorEntity::FogMetaIteratorEntity(const FogMetaIteratorEntity& anIterator, int anIndex)
	:
	_domain(anIterator._domain),
	_index(anIterator._index) {
	if (anIndex > 0) {
		size_t anIncrement = anIndex;
		size_t toGo = _domain->tally() - _index;
		
		if (anIncrement < toGo)
			_index += anIncrement;
		else {
			if (anIncrement > toGo)
				ERRMSG("Should not increment past end of iterator domain.");
				
			_index = _domain->tally() + 1;
		}
	}
	else if (anIndex < 0) {
		size_t aDecrement = -anIndex;
		
		if (_index < aDecrement) {
			if (aDecrement > _index + 1)
				ERRMSG("Should not decrement past end of iterator domain.");
				
			_index = _domain->tally() + 1;
		}
		else
			_index -= aDecrement;
	}
	
	update();
}

FogMetaIteratorEntity::~FogMetaIteratorEntity() {}

void FogMetaIteratorEntity::destroy() {
	_domain.reset();
	_value.reset();
	Super::destroy();
}

bool FogMetaIteratorEntity::emit(FogEmitContext& emitContext) const {       //   Only occurs on duff code constructs
	emitContext.emit_number(FogNumber::make_logical(is_ok()));
	return true;
}

bool FogMetaIteratorEntity::get_character(PrimIdHandle& returnId, FogScopeContext& inScope)
{ return get_identifier(returnId, inScope); }

bool FogMetaIteratorEntity::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                                       FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const {
	switch (dyadicOp) {
	case '(': {
		ERRMSG("Meta-function rather than meta-iterator expected for " << viz(*this));
		returnValue = FogFailure::make();
		return false;
	}
	
	case FogTokenType::ARROW: {
		if (!is_ok()) {
			ERRMSG("Out of domain access suppressed for " << viz(*this));
			returnValue = FogFailure::make();
			return false;
		}
		
		FogToken& inToken = mutate().token();           //  .bugbug cast
		{
			FogNestedScopeContext nestedContext(inScope, inToken);
			
			if (rightValue.get_object(returnValue, nestedContext))
				return true;
		}
		returnValue = FogFailure::make();
		return false;
	}
	
	default:
		return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);
	}
}

bool FogMetaIteratorEntity::get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                                        FogTokenType::TokenType monadicOp) const {
	switch (monadicOp) {
	case '*':
		if (!is_ok()) {
			ERRMSG("Out of domain access to " << viz(*this));
			returnValue = FogFailure::make();
			return false;
		}
		
		returnValue = mutate().token();         //  .bugbug cast
		return true;
		
	case '=':                       //   Monadic = is used to access a MetaVariable (a counterpart to dyadic f(args))
		returnValue = mutate();         //  .bugbug cast
		return true;
		
	case FogTokenType::DEC: {
		FogMetaIteratorEntityRef newIterator(new FogMetaIteratorEntity(*this, -1), FogMetaIteratorEntityRef::ADOPT);
		returnValue.assign(*newIterator);
		return true;
	}
	
	case FogTokenType::INC: {
		FogMetaIteratorEntityRef newIterator(new FogMetaIteratorEntity(*this, +1), FogMetaIteratorEntityRef::ADOPT);
		returnValue.assign(*newIterator);
		return true;
	}
	
	default:
		return Super::get_monadic(returnValue, inScope, monadicOp);
	}
}

bool FogMetaIteratorEntity::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	static PrimIdHandle falseId("0");;
	static PrimIdHandle trueId("1");;
	returnId = is_ok() ? trueId : falseId;
	return true;
}

const PrimNumber& FogMetaIteratorEntity::get_number(FogScopeContext& inScope) const {
	return PrimNumber::make_bool(is_ok());
}

bool FogMetaIteratorEntity::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	returnValue.assign(FogNumber::make_logical(is_ok()));
	return true;
}

//  bool FogMetaIteratorEntity::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
//  {
//    returnValue.assign(*this);
//    return true;
//  }

bool FogMetaIteratorEntity::get_string(PrimIdHandle& returnId, FogScopeContext& inScope)
{ return get_identifier(returnId, inScope); }

FogMetaIteratorEntity* FogMetaIteratorEntity::is_meta_iterator_entity() { return this; }
bool FogMetaIteratorEntity::is_ok() const { return _domain->tally() && (_index <= _domain->tally()); }
const FogMetaType& FogMetaIteratorEntity::meta_type() const { return FogMetaType::iterator_type(); }

std::ostream& FogMetaIteratorEntity::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_domain->print_depth(s, aDepth);
	return s;
}

std::ostream& FogMetaIteratorEntity::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_domain->print_members(s, aDepth);
	return s;
}

std::ostream& FogMetaIteratorEntity::print_this(std::ostream& s) const {
	Super::print_this(s);
	return s << ", " << _index;
}

std::ostream& FogMetaIteratorEntity::print_viz(std::ostream& s) const {
	s << "\"meta-iterator: ";
	Super::print_this(s);
	return s << '\"';
}

bool FogMetaIteratorEntity::set_value(const FogToken& tokenValue, FogScopeContext& inScope) {
	FogMetaScopeContext unMetaScope(inScope, false);
	//      FogTokenRef gotValue;
	FogTokenRef domainValue;
	
	if (!tokenValue.get_object(domainValue, unMetaScope))
		return false;
		
	//      else if (!gotValue->morph_to(domainValue, FogMetaType::list_type(), unMetaScope))
	//          return false;
	const FogMetaIteratorEntity* thatIterator = tokenValue.is_meta_iterator_entity();
	const FogIterationDomain* thatTokenList = domainValue->is_iteration_domain();
	
	if (thatIterator) {
		_domain = thatIterator->_domain;
		_index = thatIterator->_index;
	}
	else {
		FogTokenRef actualTokenValue(domainValue);
		domainValue->make_actual(actualTokenValue, inScope);
		
		if (thatTokenList) {
			const FogToken* aToken = actualTokenValue.pointer();
			_domain = aToken ? aToken->is_iteration_domain() : 0;
			_index = _domain && _domain->tally() ? 0 : 1;                       //   Out of domain if empty.
		}
		else {
			FogIterationDomainRef newDomain(new FogIterationDomain(FogMetaType::token_type()), FogIterationDomainRef::ADOPT);
			newDomain->add(*actualTokenValue);
			_domain = newDomain;
			_index = 0;
		}
	}
	
	update();
	return true;
}

FogToken& FogMetaIteratorEntity::token() {
	return (FogToken&) * _domain->tokens()[_index];         //  .bugbug cast
}
//  const FogToken& FogMetaIteratorEntity::token() const { return *_domain->tokens()[_index]; }
//  FogTokenType::TokenType FogMetaIteratorEntity::token_type_enum() const { return FogTokenType::Iterator; }

void FogMetaIteratorEntity::update() {
	if (_index < _domain->tally())
		_value = _domain->tokens()[_index];
	else
		_value.reset();
}
