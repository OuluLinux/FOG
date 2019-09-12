#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogThrowExpr, Super)
FOGTOKEN_LEAF_IMPL(FogThrowExpr)

bool FogThrowExpr::emit(FogEmitContext& emitContext) const
{
 emitContext.emit_space_and_text("throw");
 expr().emit(emitContext);
 return true;
}

size_t FogThrowExpr::executable_tokens() const { return 1 + expr().executable_tokens(); }

bool FogThrowExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
 ERRMSG("There is no support for compile-time exception for " << viz(*this));
 tokenValue = FogFailure::make(); 
 return false;
}

char FogThrowExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = FogStream::space_and_emit(s, tailChar, "throw");
 if (!expr().is_null())
    {
//      	tailChar = FogStream::space_and_emit(s, tailChar, " ");
        tailChar = expr().print_named(s, fullId, tailChar);
    }
 return tailChar;
}

bool FogThrowExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext exprSemantics(theSemantics);
 if (!expr().resolve_semantics(exprSemantics))          //  .bugbug null expression
  return false;
 theSemantics.reset();
 if (exprSemantics.is_basic_expression())
  theSemantics.set(FogSemantics::IS_ASSIGNMENT_EXPRESSION);
 return true;
}