#include <Fog/FogIncludeAll.h>





TYPEINFO_SINGLE(FogMetaSlotFinder, Super)
TYPEINFO_SINGLE(FogMetaSlotFinding, Super)

FogMetaSlotFinding::FogMetaSlotFinding()
	:
	_slot(0),
	_id(0),
	_found(0)
{}

FogMetaSlotFinding::~FogMetaSlotFinding() {}

void FogMetaSlotFinding::add(const FogMetaSlot& metaSlot) {
	const PrimId& anId = metaSlot.id();
	
	if (!_slot) {      //   If first addition.
		_slot = &metaSlot;
		_id = &anId;
		return;
	}
	
	if (_id) {       //   If second potential addition.
		if (anId == *_id)
			return;
			
		_found.add(*_id);
		_id = 0;
	}
	
	_found.add_filtered(anId);
}

const FogMetaSlot* FogMetaSlotFinding::get_unambiguous_finding(const FogToken& aToken,
        const FogScopeContext& inScope) const {
	if (is_ambiguous()) {
		PrimOstrstream s;
		
		for (PrimIdConstMapIterator p(_found); p; ++p)
			s << "\n\t\t" << *p;
			
		ERRMSG("Slot \"" << viz(aToken) << "\" is ambiguous within " << viz(inScope) << " :" << s.str());
	}
	
	return _slot;
}

//  const FogMetaSlot *FogMetaSlotFinding::get_unambiguous_finding(const PrimId& anId, const FogScope& inScope) const
//  {
//  	if (is_ambiguous())
//  	{
//  		PrimOstrstream s;
//  		for (PrimIdConstMapIterator p(_found); p; ++p)
//  			s << "\n\t\t" << *p;
//  		ERRMSG("Slot \"" << anId << "\" is ambiguous within " << viz(inScope) << " :" << s.str());
//  	}
//  	return _slot;
//  }

FogMetaSlotFinder::FogMetaSlotFinder(FogMetaSlotFinding& theFinding, const PrimId& anId, InScope inScope)
	:
	_id(anId),
	_in_scope(inScope),
	_finding(theFinding),
	_in_token(0)
{}

FogMetaSlotFinder::~FogMetaSlotFinder() {}

void FogMetaSlotFinder::add(FogToken& inToken, const FogMetaSlot& metaSlot) {
	_finding.add(metaSlot);
	_in_token = &inToken;
}

std::ostream& FogMetaSlotFinder::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	//  	_finding.print_deep(s, aDepth);
	return s;
}

std::ostream& FogMetaSlotFinder::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	//  	_finding.print_on(s, aDepth);
	return s;
}

std::ostream& FogMetaSlotFinder::print_this(std::ostream& s) const { return s << *_id; }
std::ostream& FogMetaSlotFinder::print_viz(std::ostream& s) const { return s << '\"' << *_id << '\"'; }

void FogMetaSlotFinder::set_id(const PrimId& anId, InScope inScope) {
	_id = anId;
	_in_scope = inScope;
}
