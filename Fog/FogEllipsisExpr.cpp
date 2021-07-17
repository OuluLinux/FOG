#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogEllipsisExpr, Super)
FOGTOKEN_LEAF_IMPL(FogEllipsisExpr)

FogEllipsisExpr::FogEllipsisExpr() {}

FogEllipsisExpr::FogEllipsisExpr(const This& anExpr)
	:
	Super(anExpr)
{}

FogEllipsisExpr::~FogEllipsisExpr() {}

bool FogEllipsisExpr::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text("...");
	return true;
}

char FogEllipsisExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, "...");
	return tailChar;
}

bool FogEllipsisExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.set(FogSemantics::IS_ELLIPSIS);
	return true;
}
