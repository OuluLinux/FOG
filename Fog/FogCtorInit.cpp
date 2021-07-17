#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogCtorInit, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogCtorInit)
PRIMREFS_IMPL(FogCtorInit)
PRIMMAPOFREFS_IMPL(FogCtorInit)
FOGTOKEN_UNIQUE_IMPL(FogCtorInit)
TMPL_HACK_FIX_DO(FogCtorInit)

FogCtorInit::FogCtorInit() {}

FogCtorInit::FogCtorInit(const FogEntity& anEntity, const FogExpr& anInit)
	:
	_entity(anEntity),
	_id(anEntity.unique_id()),
	_inits(anInit)
{}

FogCtorInit::~FogCtorInit() {}

void FogCtorInit::create_usage(FogUsageContext& usageContext) const {
	for (FogExprConstListOfRefToConstIterator p(_inits); p; ++p)
		p->create_usage(usageContext);
}

bool FogCtorInit::emit(FogEmitContext& emitContext) const {
	emitContext.emit_identifier(_entity->local_signature_id()); //   No scope since inits must be visible.
	emitContext.emit_space_and_text("(");
	FogForUseAsEmitContext changedContext(emitContext, FogEmitContext::EXPRESSION);
	FogSeparatedEmitContext separatedContext(changedContext, ",", 0);
	_inits->emit(separatedContext);
	changedContext.emit_space_and_text(")");
	return true;
}

std::ostream& FogCtorInit::print_this(std::ostream& s) const {
	return s << *_id << '(' << *_inits << ')';
}

std::ostream& FogCtorInit::print_viz(std::ostream& s) const {
	s << "ctor-init: ";
	print_this(s);
	return s << '\"';
}
