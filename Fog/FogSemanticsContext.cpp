#include <Fog/FogIncludeAll.h>




TYPEINFO_SINGLE(FogSemanticsContext, Super)
TYPEINFO_SINGLE(FogChildSemanticsContext, Super)
TYPEINFO_SINGLE(FogMetaSemanticsContext, Super)
TYPEINFO_SINGLE(FogNestedSemanticsContext, Super)
TYPEINFO_SINGLE(FogStrongSemanticsContext, Super)
TYPEINFO_SINGLE(FogWeakSemanticsContext, Super)

std::ostream& FogSemanticsContext::print_this(std::ostream& s) const
{
    Super::print_this(s);
    s << ", ";
    return FogSemantics::print_this(s);
}

void FogSemanticsContext::set_diagnostic(DiagnosticBits aDiaganotic)
{
    ERRMSG("BUG -- should not invoke FogSemanticsContext::set_diagnostic for " << viz(*this));
}

//  ---------------------------------------------------------------------------------------------------------------------

FogChildSemanticsContext::FogChildSemanticsContext(FogSemanticsContext& scopeContext)
:
    Super(scopeContext)
{}

void FogChildSemanticsContext::set_diagnostic(DiagnosticBits aDiaganotic)
    { semantics_context().set_diagnostic(aDiaganotic); }

//  ---------------------------------------------------------------------------------------------------------------------

FogMetaSemanticsContext::FogMetaSemanticsContext(FogSemanticsContext& scopeContext)
:
    Super(scopeContext)
{}

const FogScopeContext *FogMetaSemanticsContext::meta_context() const { return this; }
void FogMetaSemanticsContext::set_diagnostic(DiagnosticBits aDiaganotic)
    { semantics_context().set_diagnostic(aDiaganotic); }
 
FogNestedSemanticsContext::FogNestedSemanticsContext(FogSemanticsContext& scopeContext, FogToken& inToken)
:
    Super(scopeContext),
    _dynamic_token(inToken),
    _in_scope(scopeContext.in_scope() == IN_THIS_SCOPE ? IN_THIS_SCOPE : IN_BASE_SCOPE)
{}

FogToken& FogNestedSemanticsContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogNestedSemanticsContext::dynamic_token() const { return _dynamic_token; }

bool FogNestedSemanticsContext::find_slots(FogMetaSlotFinder& theFinder)
{
    ERRMSG("INVESTIGATE -- use of FogNestedSemanticsContext::find_slots");
    FogMakeEntityContext *makeEntityContext = find_make_entity_context();
    if (makeEntityContext && makeEntityContext->find_formal_slots(theFinder))
        return true;
    if (_dynamic_token.find_slots(theFinder))
        return true;
    return Super::find_slots(theFinder);
}

FogScopeContext::InScope FogNestedSemanticsContext::in_scope() const { return _in_scope; }
void FogNestedSemanticsContext::set_diagnostic(DiagnosticBits aDiaganotic)
    { semantics_context().set_diagnostic(aDiaganotic); }

//  ---------------------------------------------------------------------------------------------------------------------

FogStrongSemanticsContext::FogStrongSemanticsContext(FogScopeContext& scopeContext)
:
    Super(scopeContext)
{}

FogScopeContext::Resolution FogStrongSemanticsContext::resolution() const { return RESOLVE_TYPE_STRONGLY; }

//  ---------------------------------------------------------------------------------------------------------------------

FogWeakSemanticsContext::FogWeakSemanticsContext(FogScopeContext& scopeContext)
:
    Super(scopeContext),
    _diagnostics(NO_DIAGNOSTIC)
{}

const char *FogWeakSemanticsContext::reason() const
{
    if (_diagnostics & BAD_IDENTIFIER)
        return "an identifier did not contain a valid character sequence within the";
    else if (_diagnostics & BAD_TEMPLATE)
        return "a template has been used prior to declaration in the";
    else
        return "a type has been used prior to declaration in the";
}

FogScopeContext::Resolution FogWeakSemanticsContext::resolution() const { return RESOLVE_TYPE_WEAKLY; }

void FogWeakSemanticsContext::set_diagnostic(DiagnosticBits aDiaganotic)
{
    _diagnostics = DiagnosticBits(_diagnostics | aDiaganotic);
}

