#include <Fog/FogIncludeAll.h>




TYPEINFO_SINGLE(FogMetaToken, Super)

FogMetaToken::FogMetaToken() {}
FogMetaToken::~FogMetaToken() {}

FogMetaEntity* FogMetaToken::adopt_meta_entity(FogMetaEntity* metaEntity) {
	_meta_entities.adopt(metaEntity);
	return metaEntity;
}

void FogMetaToken::destroy() {
	Super::destroy();
	_meta_entities.vacate();
}

FogMetaEntity* FogMetaToken::find_meta_entity(const PrimId& anId) {
	FogMetaEntity* metaEntity = _meta_entities.find(anId);
	return metaEntity ? metaEntity : Super::find_meta_entity(anId);
}

std::ostream& FogMetaToken::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_meta_entities.tally()) {
		s << indent(aDepth) << "meta_entities\n";
		_meta_entities.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogMetaToken::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_meta_entities.tally()) {
		s << indent(aDepth) << "meta_entities\n";
		_meta_entities.print_members(s, aDepth + 1);
	}
	
	return s;
}
