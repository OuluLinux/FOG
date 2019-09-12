#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogMakeEntityContext, Super)
TYPEINFO_SINGLE(FogCompileMakeEntityContext, Super)
TYPEINFO_SINGLE(FogDecoratedMakeEntityContext, Super)
TYPEINFO_SINGLE(FogInScopeMakeEntityContext, Super)
TYPEINFO_SINGLE(FogNestedMakeEntityContext, Super)
TYPEINFO_SINGLE(FogReScopedMakeEntityContext, Super)
TYPEINFO_SINGLE(FogRecompileMakeEntityContext, Super)
TYPEINFO_SINGLE(FogResolvableFunctionMakeEntityContext, Super)
TYPEINFO_SINGLE(FogStaticMakeEntityContext, Super)

bool FogMakeEntityContext::find_formal_slots(FogMetaSlotFinder& theFinder) { return false; }
FogMakeEntityContext *FogMakeEntityContext::find_make_entity_context() { return this; }
FogMakeEntityContext::HasContext FogMakeEntityContext::has_context() const { return HAS_CONTEXT; }
FogScopeContext::Resolution FogMakeEntityContext::resolution() const { return RESOLVE_DOLLARS; }
 
FogCompileMakeEntityContext::FogCompileMakeEntityContext(FogMakeSpecifierContext& makeSpecifierContext)
:
    _make_specifier_context(makeSpecifierContext)
{}

const FogDeclSpecifierValue& FogCompileMakeEntityContext::decl_specifiers() const
    { return _make_specifier_context.decl_specifiers(); }
FogToken& FogCompileMakeEntityContext::dynamic_token() { return _make_specifier_context.dynamic_token(); }
const FogToken& FogCompileMakeEntityContext::dynamic_token() const { return _make_specifier_context.dynamic_token(); }
FogScopeContext *FogCompileMakeEntityContext::find_context(size_t dollarCount)
    { return dollarCount > 0 ? _make_specifier_context.find_context(dollarCount) : this; }
bool FogCompileMakeEntityContext::find_slots(FogMetaSlotFinder& theFinder)
    { return _make_specifier_context.find_slots(theFinder); }
const FogTemplateParameterSpecifier *FogCompileMakeEntityContext::find_template_parameter_specifier
    (const PrimId& anId) { return _make_specifier_context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier *FogCompileMakeEntityContext::find_template_parameter_specifier
        (const FogTemplateParameterSpecifierId& anId)
    { return _make_specifier_context.find_template_parameter_specifier(anId); }
FogScopeContext::InScope FogCompileMakeEntityContext::in_scope() const { return _make_specifier_context.in_scope(); }

std::ostream& FogCompileMakeEntityContext::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _make_specifier_context.print_deep(s, aDepth);
    return s;
}

std::ostream& FogCompileMakeEntityContext::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _make_specifier_context.print_on(s, aDepth);
    return s;
}

std::ostream& FogCompileMakeEntityContext::print_this(std::ostream& s) const
{
//    print_resolution(s);
    _make_specifier_context.print_this(s);
    return s;
}

std::ostream& FogCompileMakeEntityContext::print_viz(std::ostream& s) const
{
    _make_specifier_context.print_viz(s);
    return s;
}

FogScopeContext::Resolution FogCompileMakeEntityContext::resolution() const
    { return _make_specifier_context.resolution(); }
FogToken& FogCompileMakeEntityContext::static_token() { return _make_specifier_context.static_token(); }
const FogToken& FogCompileMakeEntityContext::static_token() const { return _make_specifier_context.static_token(); }
FogTargetFile *FogCompileMakeEntityContext::target_file() const { return _make_specifier_context.target_file(); }
const FogTemplateParameterSpecifiers *FogCompileMakeEntityContext::template_parameters() const
    { return _make_specifier_context.template_parameters(); }
const FogUtility& FogCompileMakeEntityContext::utility() const { return _make_specifier_context.utility(); }
 
const FogDeclSpecifierValue& FogDecoratedMakeEntityContext::decl_specifiers() const
    { return _make_entity_context.decl_specifiers(); }
FogToken& FogDecoratedMakeEntityContext::dynamic_token() { return _make_entity_context.dynamic_token(); }
const FogToken& FogDecoratedMakeEntityContext::dynamic_token() const { return _make_entity_context.dynamic_token(); }
FogScopeContext *FogDecoratedMakeEntityContext::find_context(size_t dollarCount)
    { return dollarCount > 0 ? _make_entity_context.find_context(dollarCount) : this; }
bool FogDecoratedMakeEntityContext::find_formal_slots(FogMetaSlotFinder& theFinder)
    { return _make_entity_context.find_formal_slots(theFinder); }
bool FogDecoratedMakeEntityContext::find_slots(FogMetaSlotFinder& theFinder)
    { return _make_entity_context.find_slots(theFinder); }
const FogTemplateParameterSpecifier *FogDecoratedMakeEntityContext::find_template_parameter_specifier
    (const PrimId& anId) { return _make_entity_context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier *FogDecoratedMakeEntityContext::find_template_parameter_specifier
        (const FogTemplateParameterSpecifierId& anId)
    { return _make_entity_context.find_template_parameter_specifier(anId); }
FogMakeEntityContext::HasContext FogDecoratedMakeEntityContext::has_context() const
    { return _make_entity_context.has_context(); }
FogScopeContext::InScope FogDecoratedMakeEntityContext::in_scope() const { return _make_entity_context.in_scope(); }
const FogScopeContext *FogDecoratedMakeEntityContext::meta_context() const
    { return _make_entity_context.meta_context(); }

std::ostream& FogDecoratedMakeEntityContext::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _make_entity_context.print_deep(s, aDepth);
    return s;
}

std::ostream& FogDecoratedMakeEntityContext::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _make_entity_context.print_on(s, aDepth);
    return s;
}

FogScopeContext::Resolution FogDecoratedMakeEntityContext::resolution() const
    { return _make_entity_context.resolution(); }
FogToken& FogDecoratedMakeEntityContext::static_token() { return _make_entity_context.static_token(); }
const FogToken& FogDecoratedMakeEntityContext::static_token() const { return _make_entity_context.static_token(); }
FogTargetFile *FogDecoratedMakeEntityContext::target_file() const { return _make_entity_context.target_file(); }
//  const FogExpr *FogDecoratedMakeEntityContext::template_arguments() const
//      { return _make_entity_context.template_arguments(); }
const FogTemplateParameterSpecifiers *FogDecoratedMakeEntityContext::template_parameters() const
    { return _make_entity_context.template_parameters(); }
const FogUtility& FogDecoratedMakeEntityContext::utility() const { return _make_entity_context.utility(); }
 
FogInScopeMakeEntityContext::FogInScopeMakeEntityContext(FogMakeEntityContext& makeEntityContext, InScope inScope)
:
    Super(makeEntityContext),
    _in_scope(inScope)
{}

FogScopeContext::InScope FogInScopeMakeEntityContext::in_scope() const { return _in_scope; }

//  ---------------------------------------------------------------------------------------------------------------------

FogNestedMakeEntityContext::FogNestedMakeEntityContext(FogMakeEntityContext& makeEntityContext, FogToken& inToken)
:
    Super(makeEntityContext, inToken)
{}

bool FogNestedMakeEntityContext::find_slots(FogMetaSlotFinder& theFinder)
    { return dynamic_token().find_slots(theFinder); }
//  const FogExpr *FogNestedMakeEntityContext::template_arguments() const { return 0; }

//  ---------------------------------------------------------------------------------------------------------------------

FogReScopedMakeEntityContext::FogReScopedMakeEntityContext(FogMakeEntityContext& makeEntityContext, FogToken& inToken)
:
    Super(makeEntityContext),
    _dynamic_token(inToken)
{}

FogToken& FogReScopedMakeEntityContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogReScopedMakeEntityContext::dynamic_token() const { return _dynamic_token; }
 
FogRecompileMakeEntityContext::FogRecompileMakeEntityContext(FogScopeContext& inScope,
    FogPotentialDeclaration& potentialDeclaration, HasContext hasContext)
:
    _dynamic_token(inScope.dynamic_token()),
    _potential_declaration(potentialDeclaration),
    _has_context(hasContext)
{}

const FogDeclSpecifierValue& FogRecompileMakeEntityContext::decl_specifiers() const
{
    static const FogDeclSpecifierValue nullValue;
    return nullValue;
}

FogToken& FogRecompileMakeEntityContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogRecompileMakeEntityContext::dynamic_token() const { return _dynamic_token; }
bool FogRecompileMakeEntityContext::find_slots(FogMetaSlotFinder& theFinder)
    { return _dynamic_token.find_slots(theFinder); }
FogMakeEntityContext::HasContext FogRecompileMakeEntityContext::has_context() const { return _has_context; }
FogToken& FogRecompileMakeEntityContext::static_token() { return _dynamic_token; }
const FogToken& FogRecompileMakeEntityContext::static_token() const { return _dynamic_token; }

FogTargetFile *FogRecompileMakeEntityContext::target_file() const
{
    return _potential_declaration.target_file();
//    FogScope *aScope = _dynamic_token.is_scope();
//    return aScope ? aScope->interface_file_source() : 0;
}

const FogUtility& FogRecompileMakeEntityContext::utility() const { return _dynamic_token.utility(); }
 
FogResolvableFunctionMakeEntityContext::FogResolvableFunctionMakeEntityContext(FogMakeEntityContext& makeEntityContext,
    FogMetaFunction& metaFunction) 
:
    Super(makeEntityContext, metaFunction),
    _function(metaFunction)
{}

bool FogResolvableFunctionMakeEntityContext::find_formal_slots(FogMetaSlotFinder& theFinder)
{
    const FogMetaArgument *metaArgument = _function.find(theFinder.id());
    if (!metaArgument)
        return false;
    theFinder.add(dynamic_token(), *metaArgument);  //   Successful parameter find, not in nested context, so resolve it.
    return true;
}

bool FogResolvableFunctionMakeEntityContext::find_slots(FogMetaSlotFinder& theFinder)
{
    return find_formal_slots(theFinder);
}

std::ostream& FogResolvableFunctionMakeEntityContext::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _function.print_deep(s, aDepth);
    return s;
}

std::ostream& FogResolvableFunctionMakeEntityContext::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
//    _function.print_on(s, aDepth);
    return s;
}

FogScopeContext::Resolution FogResolvableFunctionMakeEntityContext::resolution() const
    { return LOCATE_META_FUNCTION_FORMALS; }
 
FogStaticMakeEntityContext::FogStaticMakeEntityContext(FogScopeContext& inScope)
:
    _dynamic_token(inScope.dynamic_token())
{}

const FogDeclSpecifierValue& FogStaticMakeEntityContext::decl_specifiers() const
{
    static const FogDeclSpecifierValue nullValue;
    return nullValue;
}

FogToken& FogStaticMakeEntityContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogStaticMakeEntityContext::dynamic_token() const { return _dynamic_token; }
bool FogStaticMakeEntityContext::find_slots(FogMetaSlotFinder& theFinder)
    { return _dynamic_token.find_slots(theFinder); }
FogToken& FogStaticMakeEntityContext::static_token() { return _dynamic_token; }
const FogToken& FogStaticMakeEntityContext::static_token() const { return _dynamic_token; }

FogTargetFile *FogStaticMakeEntityContext::target_file() const
{
    FogScope *aScope = _dynamic_token.is_scope();
    return aScope ? aScope->interface_file_source() : 0;
}

const FogUtility& FogStaticMakeEntityContext::utility() const { return _dynamic_token.utility(); }
