#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogThisExpr, Super)
FOGTOKEN_LEAF_IMPL(FogThisExpr)

FogThisExpr::FogThisExpr() {}
 
FogThisExpr::FogThisExpr(const This& anExpr)
:
 Super(anExpr)
{}

FogThisExpr::~FogThisExpr() {}

bool FogThisExpr::emit(FogEmitContext& emitContext) const
{
 emitContext.emit_space_and_text("this");
 return true;
}

bool FogThisExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
 tokenValue.assign(inScope.dynamic_token());
 return true;
}

char FogThisExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = FogStream::space_and_emit(s, tailChar, "this");
 return tailChar;
}

bool FogThisExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 theSemantics.set(FogSemantics::IS_THIS);
 return true;
}
