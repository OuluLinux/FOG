#include <Fog/FogIncludeAll.h>









typedef PrimConstMapOfRefIterator<FogEntityFindingElement> FogEntityFindingElementConstSetOfRefIterator;

class FogEntityFindingElement : public PrimPrint, public PrimSetElement<FogEntityRefToConst> {
	typedef PrimPrint Super;
	typedef FogEntityFindingElement This;
	TYPEDECL_SINGLE(This, Super)
	PRIMSHARES_DERIVED_DECLS_INLINE(This)
	PRIMREF_DERIVED_DECLS(This)
private:
	bool _is_dominatable;
private:
	FogEntityFindingElement(const This&);
	This& operator=(const This&);
protected:
	FogEntityFindingElement()
		: PrimSetElement<FogEntityRefToConst>(0, FogEntity::immutable_null()), _is_dominatable(false) {}
	virtual ~FogEntityFindingElement() {}
public:
	FogEntityFindingElement(const FogEntity& anEntity, bool isDominatable)
		: PrimSetElement<FogEntityRefToConst>(anEntity.unique_id(), anEntity), _is_dominatable(isDominatable) {}
	virtual bool check(bool fullCheck = false) const { return Super::check(fullCheck); }
	bool is_dominatable() const { return _is_dominatable; }
	void merge_dominatable(bool isDominatable) { if (!isDominatable) _is_dominatable = false; }
	virtual std::ostream& print_depth(std::ostream& s, int anIndentLevel = 0) const { return Super::print_depth(s); }
	virtual std::ostream& print_instance(std::ostream& s) const { return Super::print_instance(s); }
	virtual std::ostream& print_this(std::ostream& s) const {
		s << (_is_dominatable ? "DOMINATABLE, " : "INDOMINATABLE, ") << *value();
		return s;
	}
};

TYPEINFO_SINGLE(FogEntityFinder, Super)
TYPEINFO_SINGLE(FogEntityFinding, Super)
TYPEINFO_SINGLE(FogEntityFindingElement, Super)

PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogEntityFindingElement)
PRIMMAPOFREFS_IMPL(FogEntityFindingElement)
TMPL_HACK_FIX_DO(FogEntityFindingElement)

FogEntityFinding::FogEntityFinding(FindStrategy  aStrategy)
	:
	_strategy(aStrategy),
	_entity(0),
	_entity_id(0),
	_entity_dominatable(false),
	_found(0),
	_dominations_pruned(false)
{}

FogEntityFinding::~FogEntityFinding() {}

void FogEntityFinding::add(FogEntity& anEntity, bool isDominatable) {
	const PrimId& anId = anEntity.unique_id();
	
	if (!_entity) {                             //   If first addition.
		_entity = &anEntity;
		_entity_id = &anId;
		_entity_dominatable = isDominatable;
		return;
	}
	
	if (_entity_id) {                               //   If second potential addition.
		if (anId == *_entity_id) {
			if (!isDominatable)
				_entity_dominatable = false;
				
			return;
		}
		
		_found.adopt(new FogEntityFindingElement(*_entity, _entity_dominatable));
		_entity_id = 0;
	}
	
	FogEntityFindingElement* anElement = _found.find(anId);
	
	if (anElement)
		anElement->merge_dominatable(isDominatable);
	else {
		_dominations_pruned = false;
		anElement = new FogEntityFindingElement(anEntity, isDominatable);
		
		if (anElement) {
			_found.add_filtered(*anElement);
			anElement->annul();
		}
	}
}

FogEntity* FogEntityFinding::get_unambiguous_finding(const FogToken& aToken, const FogScopeContext& inScope) {
	if (is_ambiguous()) {
		PrimOstrstream s;
		
		for (FogEntityFindingElementConstSetOfRefIterator p(_found); p; ++p) {
			const FogEntityFindingElement& anElement = *p;
			s << "\n\t\t" << viz(*anElement.value());
		}
		
		ERRMSG(strategy_text(_strategy) << " \"" << viz(aToken)
		       << "\" is ambiguous within " << viz(inScope) << " :" << s.str());
	}
	
	return _entity;
}

FogEntity* FogEntityFinding::get_unambiguous_finding(const PrimId& anId, const FogScope& inScope) {
	if (is_ambiguous()) {
		PrimOstrstream s;
		
		for (FogEntityFindingElementConstSetOfRefIterator p(_found); p; ++p) {
			const FogEntityFindingElement& anElement = *p;
			s << "\n\t\t" << viz(*anElement.value());
		}
		
		ERRMSG(strategy_text(_strategy) << " \"" << anId <<
		       "\" is ambiguous within " << viz(inScope) << " :" << s.str());
	}
	
	return _entity;
}

bool FogEntityFinding::is_ambiguous() const {
	if (!_dominations_pruned)
		mutate().resolve_dominations();
		
	return _found.tally() > 1;
}

std::ostream& FogEntityFinding::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_found.print_deep(s, aDepth);
	return s;
}

std::ostream& FogEntityFinding::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_found.print_on(s, aDepth);
	return s;
}

std::ostream& FogEntityFinding::print_this(std::ostream& s) const {
	if (_entity)
		s << *_entity;
		
	return s;
}

void FogEntityFinding::resolve_dominations() {
	if (_found.tally() > 1) {
		FogEntityFindingElementSetOfRef copiedSet(_found);
		
		for (FogEntityFindingElementConstSetOfRefIterator p(copiedSet); p; ++p) {
			const FogEntityFindingElement& dominatingElement = *p;
			const FogEntity& dominatingEntity = *dominatingElement.value();
			const FogScope& dominatingScope = dominatingEntity.inner_scope();
			const FogScopeListOfRefToConst& dominatedBaseList =  dominatingScope.all_bases();
			
			if (dominatedBaseList.tally()) {
				for (FogEntityFindingElementConstSetOfRefIterator q(copiedSet); q; ++q) {
					const FogEntityFindingElement& dominatedElement = *q;
					
					if ((&dominatedElement != &dominatingElement) && dominatedElement.is_dominatable()) {
						const FogEntity& dominatedEntity = *dominatedElement.value();
						const FogScope& dominatedScope = dominatedEntity.inner_scope();
						
						if (dominatedBaseList.find(dominatedScope))
							_found.remove(dominatedEntity.unique_id());
					}
				}
			}
		}
	}
	
	_dominations_pruned = true;
}

const char* FogEntityFinding::strategy_text(FindStrategy aStrategy) {
	switch (aStrategy) {
	case FIND_ENTITY: return "entity";
	
	case FIND_NAME: return "name";
	
	case FIND_SCOPE: return "scope";
	
	case FIND_SIGNATURE: return "signature";
	
	case FIND_TYPE: return "type";
	};
	
	return "UNKNOWN-FindStrategy";
}

//
//    Normal constructor, supporting find of anId returning results to theFinding.
//
FogEntityFinder::FogEntityFinder(FogEntityFinding& theFinding, const PrimId& anId, InScope inScope)
	:
	_id(anId),
	_in_scope(inScope),
	_finding(theFinding),
	_is_dominatable(false)
{}

FogEntityFinder::FogEntityFinder(FogEntityFinding& theFinding, const FogMakerContext& makerContext)
	:
	_id(makerContext.short_id()),
	_in_scope(makerContext.in_scope()),
	_finding(theFinding),
	_is_dominatable(false)
{}

FogEntityFinder::FogEntityFinder(FogEntityFinding& theFinding, const FogEntityFinder& aFinder)
	:
	_id(aFinder._id),
	_in_scope(aFinder._in_scope),
	_finding(theFinding),
	_is_dominatable(false)
{}

FogEntityFinder::~FogEntityFinder() {}

std::ostream& FogEntityFinder::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_finding.print_deep(s, aDepth);
	return s;
}

std::ostream& FogEntityFinder::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_finding.print_on(s, aDepth);
	return s;
}

std::ostream& FogEntityFinder::print_this(std::ostream& s) const
{ return s << FogEntityFinding::strategy_text(strategy()) << ", " << *_id; }

std::ostream& FogEntityFinder::print_viz(std::ostream& s) const { return s << '\"' << *_id << '\"'; }
