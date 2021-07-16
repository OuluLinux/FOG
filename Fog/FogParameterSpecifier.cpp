#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogParameterSpecifier, Super)
PRIMREF_NULL_CLAS(FogParameterSpecifier)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogParameterSpecifier)
PRIMREFS_IMPL(FogParameterSpecifier)
FOGTOKEN_LEAF_IMPL(FogParameterSpecifier)
TMPL_HACK_FIX_DO(FogParameterSpecifier)

FogParameterSpecifier::FogParameterSpecifier() {}

FogParameterSpecifier::FogParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(makeSpecifierContext, nameName)
{}

FogParameterSpecifier::~FogParameterSpecifier() {}

bool FogParameterSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (type().is_null())
    {
        set_type(name());
        set_name(FogAnonId::make());
    }
    return Super::compile_specifier(makeSpecifierContext);
}

void FogParameterSpecifier::create_usage(FogUsageContext& usageContext) const
{
    Super::create_usage(usageContext);
    _initializer->create_usage(usageContext);
}

//  
//    Deduce the specialised value of a template parameter used as a type actualParameterSpecifier with respect to this
//    formal paramter specifier. Returns 0 if nothing resolved, or a pointer to the resolved template paramter,
//    with the equivalence tree assigned to aValue. 
//  
const FogTemplateParameterSpecifierId *FogParameterSpecifier::deduce_template_parameter
        (FogParameterSpecifier& actualParameterSpecifier, FogExprRef& deducedValue) const
{
    FogNameRef deducedType;
    const FogTemplateParameterSpecifierId *templateId =
        type().deduce_template_parameter_type(actualParameterSpecifier.type(), deducedType);
    if (!templateId)
        return 0;
    FogModifierConstListOfRefToConstIterator formalModifiers(modifiers());
    FogModifierConstListOfRefIterator actualModifiers(actualParameterSpecifier.modifiers());
    if (actualModifiers.to_go() < formalModifiers.to_go())
        return 0;                                   //   Actual cannot be less modified.
    for ( ; formalModifiers && actualModifiers; ++formalModifiers, ++actualModifiers)
        if (!formalModifiers->is_formal_template_parameter_for(*actualModifiers))
            return 0;                               //   Actual cannot have incompatible modifier prefix.
    if (!decl_specifiers().deduce_template_parameter_type(actualParameterSpecifier.decl_specifiers(), deducedType))
        return 0;                                   //   Actual cannot be less cv-qualified.
    deducedValue = *deducedType;
    if (actualModifiers)
    {
        FogExprRef deducedExpr(FogName::make_anon());
        for ( ; actualModifiers; ++actualModifiers)
            if (actualModifiers.to_go() <= 1)
                actualModifiers->deduce_template_parameter(*deducedType, deducedExpr);
            else
                actualModifiers->deduce_template_parameter(deducedExpr);
        deducedValue = *deducedExpr;
    }
    return templateId;
}

bool FogParameterSpecifier::emit(FogEmitContext& emitContext) const
{
    const FogDeclSpecifierValue& declSpecifiers = decl_specifiers();
    if (emitContext.emit_parameter_names())
    {
        if (declSpecifiers.is_auto())
            emitContext.emit_space_and_text("auto");
        if (declSpecifiers.is_register())
            emitContext.emit_space_and_text("register");
    }
    Super::emit(emitContext);
    if (emitContext.emit_default_initialisers() && _initializer)
    {
        emitContext.emit_space_and_text("=");
        _initializer->emit(emitContext);
    }
    return true;
}

void FogParameterSpecifier::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _initializer->install(installContext);
}

bool FogParameterSpecifier::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _initializer->is_actual(scopeContext);
}

FogParameterSpecifier *FogParameterSpecifier::is_parameter_specifier() { return this; }

void FogParameterSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _initializer->make_actual(_initializer.to_const(), makeActualContext);
}

void FogParameterSpecifier::merge_from(FogMergeContext& mergeContext, const This& aSpecifier)
{
    Super::merge_from(mergeContext, aSpecifier);
    if (aSpecifier._initializer)
        aSpecifier._initializer->merge_into(mergeContext, _initializer.to_const());
}

const FogMetaType& FogParameterSpecifier::meta_type() const { return FogMetaType::parameter_specifier_type(); }

const FogMerge& FogParameterSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatSpecifier) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatSpecifier));
    if (!_initializer)
    {
        if (thatSpecifier._initializer)
            needsMerge |= FogMerge::left_invalid();
    }
    else
    {
        if (!thatSpecifier._initializer)
            needsMerge |= FogMerge::right_invalid();
        else
            needsMerge |= _initializer->needs_merge(mergeContext, *thatSpecifier._initializer);
    }
    return needsMerge;
}

std::ostream& FogParameterSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_initializer)
        _initializer->print_deep(s, aDepth+1);
    return s;
}

std::ostream& FogParameterSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_initializer)
        _initializer->print_on(s, aDepth+1);
    return s;
}

char FogParameterSpecifier::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = Super::print_named(s, fullId, tailChar);
    if (_initializer)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, " = ");
        tailChar = _initializer->print_named(s, 0, tailChar);
    }
    return tailChar;
}

std::ostream& FogParameterSpecifier::print_viz(std::ostream& s) const
{
    s << "\"parameter-specifier: ";
    print_this(s);
    return s << '\"';
}

void FogParameterSpecifier::set_initializer(const FogExpr& anExpr) { _initializer = anExpr; }
