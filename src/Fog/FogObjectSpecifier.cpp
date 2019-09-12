#include <Fog/FogIncludeAll.h>




























TYPEINFO_SINGLE(FogObjectSpecifier, Super)
PRIMREF_NULL_CLAS(FogObjectSpecifier)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogObjectSpecifier)
PRIMREFS_IMPL(FogObjectSpecifier)
TMPL_HACK_FIX_DO(FogObjectSpecifier)

FogObjectSpecifier::FogObjectSpecifier()
:
    _is_nested(false)
{}

FogObjectSpecifier::FogObjectSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(nameName, makeSpecifierContext.utility()),
    _is_nested(makeSpecifierContext.is_nested())
{}

FogObjectSpecifier::FogObjectSpecifier(const This& aSpecifier)
:
    Super(aSpecifier),
    _modifiers(aSpecifier._modifiers),
    _type(aSpecifier._type),
    _subspace(aSpecifier._subspace),
    _is_nested(aSpecifier._is_nested)
{}

FogObjectSpecifier::~FogObjectSpecifier() {}

void FogObjectSpecifier::add_subspace(FogRawSubspace& aSubspace)
{
    if (!_subspace)
        _subspace = aSubspace;
    else
        for (FogRawConstListOfRefIterator p(aSubspace.raws()); p; ++p)
            add_to_subspace(*p);
}

void FogObjectSpecifier::add_to_subspace(FogRaw& aRaw)
{
    if (!_subspace)
        _subspace.adopt(new FogRawSubspace);
    _subspace->add(aRaw);
}

const FogLazyBool& FogObjectSpecifier::can_be_concrete(FogScopeContext& inScope) const
{
    if (!decl_specifiers().is_pure_virtual())
        return FogLazyBool::make_true();
    bool atRoot = inScope.at_root();
    if (atRoot)
        return FogLazyBool::make_false();
    if (!_subspace)
        return FogLazyBool::make_bool(atRoot);
    return _subspace->derive_at(inScope);
}

bool FogObjectSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!Super::compile_specifier(makeSpecifierContext))
        return false;
    const FogDeclSpecifierValue& declSpecifiers = decl_specifiers();
    if (!declSpecifiers.is_using())
    {
        if (!declSpecifiers.is_const())
            set_decl_specifiers(FogDeclSpecifierValue::NOT_CONST_MASK);
        if (!declSpecifiers.is_volatile())
            set_decl_specifiers(FogDeclSpecifierValue::NOT_VOLATILE_MASK);
    }
    for (FogModifierConstListOfRefIterator p(_modifiers); p; ++p)
        if (!p->compile_specifier(makeSpecifierContext))
            return false;
    return true;
}

void FogObjectSpecifier::compile_subspace(FogParseContext& parseContext, FogSpecifier& actualSpecifier) const
{
//    FogToken& inToken = parseContext.dynamic_token();
//    FogPotentialDeclaration *potentialDeclaration = inToken.is_potential_declaration();
//    if (!potentialDeclaration)
//    {
//        ERRMSG("BUG - PotentialDeclaration expected as context of FogObjectSpecifier::compile_subspace");
//        return;
//    }
    for (FogRawListOfRefIterator p(_subspace->raws()); p; ++p)
    {
        FogTokenMonitor aMonitor(*p);                           //  .bugbug why doesnt' DynamicparseContext do this?
        FogDynamicParseContext nestedParseContext(parseContext, *p);
        const FogLazyBool& isDerived = p->derive_at(nestedParseContext);
        if (isDerived.is_undecided())
            ERRMSG("BUG -- Use of undecided derivation for " << viz(*p));
        if (!isDerived.is_false())
        {
            FogStrongSemanticsContext theSemantics(nestedParseContext);
            if (!p->resolve_semantics(theSemantics))
                ;
            else if (theSemantics.is_epsilon())
                ;
            else if (theSemantics.is_declaration()
                  || theSemantics.is_function_body()
                  || (parseContext.at_meta() && theSemantics.is_meta_declaration()))
            {
                static const FogDeclSpecifierValue declSpecifiers;
                FogBaseMakeSpecifierContext makeSpecifierContext(nestedParseContext, theSemantics,
                    declSpecifiers, FogMakeSemantics::SUBSPACE);
                if (!p->make_specifier(makeSpecifierContext))
                {
                    ERRMSG("Failed to make_specifier for " << viz(*p));
                    break;
                }
                makeSpecifierContext.compile_specifier();
                makeSpecifierContext.compile_type_specifier();
            }
            else
            {
                PrimOstrstream s;
                s << "Failed to resolve semantics of " << viz(*p);
                ERRMSGZ(s.str());
                break;
            }
        }
    }
}

void FogObjectSpecifier::create_usage(FogUsageContext& usageContext) const
{
    bool indirectType = _modifiers.tally() && _modifiers[0]->is_indirect();
    bool ofName = usageContext.of_use_by().is_of_name() || indirectType;
    if (!ofName)                                    //   Create type usage in original context
    {
        FogEntity *theType = _type ? _type->get_type_in(usageContext) : 0;
        if (theType)
            theType->add_use_by(usageContext);
    }
    FogChangedUsageContext ofNameContext(usageContext, usageContext.of_use_by().of_name());
    Super::create_usage(ofNameContext);
//    _subspace->create_usage(usageContext);  -- surely subspace potentials moved to actuals ?
    for (FogModifierConstListOfRefToConstIterator p(_modifiers); p; ++p)
        p->create_usage(ofNameContext);
    if (ofName)                                     //   Create type usage in changed context
    {
        FogEntity *theType = _type ? _type->get_type_in(ofNameContext) : 0;
        if (theType)
            theType->add_use_by(ofNameContext);
    }
}

const FogLazyBool& FogObjectSpecifier::derive_at(FogScopeContext& inScope) const
{
    bool atRoot = inScope.at_root();
    if (!_subspace)
        return FogLazyBool::make_bool(atRoot);
    const FogLazyBool& isActive = _subspace->derive_at(inScope);
    if (!isActive.is_false())
        return isActive;
    if (atRoot && decl_specifiers().is_pure_virtual())
        return FogLazyBool::make_true();
    return FogLazyBool::make_false();
}

bool FogObjectSpecifier::emit(FogEmitContext& emitContext) const
{
    bool doneSomething = false;
    if (!emitContext.emit_declarator_scope())               //   If interface
    {
        FogNamespaceEmitContext nestedEmitContext(emitContext, emitContext.dynamic_scope());
        FogForUseAsEmitContext changedContext(nestedEmitContext,
            emitContext.emit_full_types() ? FogEmitContext::TYPE_SIGNATURE : FogEmitContext::TYPE_INTERFACE);
        doneSomething |= changedContext.emit_cv(decl_specifiers());
        doneSomething |= _type->emit(changedContext);
    }
    else                                                    //   If implementation
    {
        FogNamespaceEmitContext nestedEmitContext(emitContext, emitContext.dynamic_scope().name_space());
        FogForUseAsEmitContext changedContext(nestedEmitContext, FogEmitContext::TYPE_INTERFACE);
        doneSomething |= changedContext.emit_cv(decl_specifiers());
        doneSomething |= _type->emit(changedContext);
    }
    for (size_t i = _modifiers.tally(); i--; )
    {
        const FogModifier& iDecl = *_modifiers[i];
        doneSomething |= iDecl.emit_prefix(emitContext, i > 0 ? _modifiers[i-1]->is_indirect() : false);
    }
    if (emitContext.emit_declarator())
        doneSomething |= Super::emit(emitContext);
    for (size_t j = 0; j < _modifiers.tally(); j++)
    {
        const FogModifier& jDecl = *_modifiers[j];
        doneSomething |= jDecl.emit_suffix(emitContext, j > 0 ? _modifiers[j-1]->is_indirect() : false);
    }
    return doneSomething;
}

//  --- only used by FogScope_Typedefs::make_typedef
const FogName *FogObjectSpecifier::find_scalar_name() const
{
    if (!name().is_null() && !_modifiers.tally())
        return &name();
    return 0;
}

FogName *FogObjectSpecifier::get_type() { return _type.pointer(); }

//  --- only used by FogMetaType_Function_Return::resolve_function
const FogName *FogObjectSpecifier::get_type_name() const
{
    const FogName *typeName = _type ? _type.pointer() : 0;
    if (!typeName)
        ERRMSG("Failed to find a type as part of " << viz(*this));
    return typeName;
}

const FogFunctionModifier *FogObjectSpecifier::has_function_signature() const
{
    for (size_t j = 0; j < _modifiers.tally(); j++)
    {
        const FogModifier& jDecl = *_modifiers[j];
        const FogFunctionModifier *functionModifier = jDecl.is_function_modifier();
        if (functionModifier)
            return functionModifier;
    }
    return 0;
}

void FogObjectSpecifier::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    for (FogModifierConstListOfRefToConstIterator p(_modifiers); p; ++p)
        p->install(installContext);
    _type->install(installContext);
}

void FogObjectSpecifier::instantiate()
{
    _subspace.reset();
}

bool FogObjectSpecifier::is_actual(const FogScopeContext& scopeContext) const
{
    if (!Super::is_actual(scopeContext))
        return false;
    for (FogModifierConstListOfRefToConstIterator p(_modifiers); p; ++p)
        if (!p->is_actual(scopeContext))
            return false;
    if (!_subspace->is_actual(scopeContext))
        return false;
    if (!_type->is_actual(scopeContext))
        return false;
    return true;
}

FogObjectSpecifier *FogObjectSpecifier::is_object_specifier() { return this; }

void FogObjectSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    for (FogModifierListOfRefToConstIterator p(_modifiers); p; ++p)
        p->make_actual(p.ref(), makeActualContext);
    _type->make_actual(_type, makeActualContext);
    if (_subspace && !_subspace->derive_at(makeActualContext).is_false())
        _subspace->make_actual(_subspace, makeActualContext);
}

FogEntity *FogObjectSpecifier::make_entity(FogMakerContext& makerContext) const
{
    return name().make_entity(makerContext);
}

FogEntity *FogObjectSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
    return name().make_name_entity(makeEntityContext);
}

bool FogObjectSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!makeSpecifierContext.make_specifier(name()))
        return false;
    FogToken& inToken = makeSpecifierContext.dynamic_token();
    FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(inToken);
    if (!objectSpecifier)
        return false;
    objectSpecifier->set_specifier(*this);
    return true;
}

FogEntity *FogObjectSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const
{
    if (makeEntityContext.decl_specifiers().is_using())
        return 0;
    else if (!_type)
        return 0;
    else
        return _type->make_type_entity(makeEntityContext);
}

void FogObjectSpecifier::merge_from(FogMergeContext& mergeContext, const This& aSpecifier)
{
    Super::merge_from(mergeContext, aSpecifier);
    FogModifier::merge_short_into(mergeContext, _modifiers, aSpecifier._modifiers); //  .bugbug short/long/ ?
    aSpecifier._type->merge_into(mergeContext, _type.to_const());
    if (!_subspace)
        _subspace = aSpecifier._subspace;
    else if (aSpecifier._subspace)
    {
        for (FogRawConstListOfRefIterator p(aSpecifier._subspace->raws()); p; ++p)
            add_to_subspace(*p);
    }
}

const FogMetaType& FogObjectSpecifier::meta_type() const { return FogMetaType::object_specifier_type(); }

const FogMerge& FogObjectSpecifier::needs_header_merge_from(FogMergeContext& mergeContext, const This& aSpecifier) const
{
//  .note INCOMPATIBLE is a reportable error and so relatively rare. There is therefore no point
//    optimising to achieve premature returns.
//  
    FogMergeHandle needsMerge;
    needsMerge |= Super::needs_merge_from(mergeContext, aSpecifier);
    FogModifierConstListOfRefToConstIterator p1(_modifiers);
    FogModifierConstListOfRefToConstIterator p2(aSpecifier._modifiers);
    if (p1.to_go() != p2.to_go())
        return FogMerge::set_incompatible(FogMerge::incompatible_signature());
    for ( ; p1; ++p1, ++p2)
        needsMerge |= p1->needs_merge(mergeContext, *p2);       //  .bugbug inexact merge
    needsMerge |= _type->needs_merge(mergeContext, *aSpecifier._type);
    return needsMerge;
}

const FogMerge& FogObjectSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& aSpecifier) const
{
//  .note INCOMPATIBLE is a reportable error and so relatively rare. There is therefore no point
//    optimising to achieve premature returns.
//  
    FogMergeHandle needsMerge;
    needsMerge |= Super::needs_merge_from(mergeContext, aSpecifier);
    FogModifierConstListOfRefToConstIterator p1(_modifiers);
    FogModifierConstListOfRefToConstIterator p2(aSpecifier._modifiers);
    if (p1.to_go() != p2.to_go())
        return FogMerge::set_incompatible(FogMerge::incompatible_signature());
    for ( ; p1; ++p1, ++p2)
        needsMerge |= p1->needs_merge(mergeContext, *p2);       //  .bugbug inexact merge
    needsMerge |= _type->needs_merge(mergeContext, *aSpecifier._type);
    if (!_subspace)
    {
        if (aSpecifier._subspace)
            needsMerge |= FogMerge::left_invalid();
    }
    else
    {
        if (!aSpecifier._subspace)
            needsMerge |= FogMerge::right_invalid();
        else
            needsMerge |= FogMerge::both_invalid();     //   One day could try to match up internals
//            needsMerge |= _subspace->needs_merge(mergeContext, *aSpecifier._subspace);
    }
    return needsMerge;
}

std::ostream& FogObjectSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_modifiers.tally())
    {
        s << indent(aDepth) << "modifiers\n";
        _modifiers.print_depth(s, aDepth);
    }
    if (_type)
    {
        s << indent(aDepth) << "type\n";
        _type->print_deep(s, aDepth+1);
    }
    if (_subspace)
        _subspace->print_deep(s, aDepth);
    return s;
}

std::ostream& FogObjectSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_modifiers.tally())
    {
        s << indent(aDepth) << "modifiers\n";
        _modifiers.print_members(s, aDepth);
    }
    if (_type)
    {
        s << indent(aDepth) << "type\n";
        _type->print_on(s, aDepth+1);
    }
    if (_subspace)
        _subspace->print_on(s, aDepth);
    return s;
}

char FogObjectSpecifier::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
    tailChar = decl_specifiers().print_named(s, tailChar);
    if (_type)
        tailChar = _type->print_named(s, 0, tailChar);
//    else
//        tailChar = FogStream::space_and_emit(s, tailChar, "--type--");      
    for (size_t i = _modifiers.tally(); i--; )
    {
        const FogModifier& iDecl = *_modifiers[i];
        tailChar = iDecl.print_prefix(s, tailChar, i > 0 ? _modifiers[i-1]->is_indirect() : false);
    }
    if (!name().is_null())
        tailChar = Super::print_named(s, scopeId, tailChar);
    else if (scopeId)
    {
        ERRMSG("INVESTIGATE - did not expect a scope for a null name in FogObjectSpecifier::print_named.");
        tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
    }
    for (size_t j = 0; j < _modifiers.tally(); j++)
    {
        const FogModifier& jDecl = *_modifiers[j];
        tailChar = jDecl.print_suffix(s, tailChar, j > 0 ? _modifiers[j-1]->is_indirect() : false);
    }
    return tailChar;
}

//  bool FogObjectSpecifier::resolve_semantics(FogSemantics& theSemantics, FogScopeContext& inScope) const
//  {
//    for (size_t j = 0; j < _modifiers.tally(); j++)
//    {
//        const FogModifier& jDecl = *_modifiers[j];
//        if (jDecl.is_function_modifier())
//        {
//            theSemantics.assign(FogSemantics::IS_FUNCTION_DEFINITION);
//            return true;
//        }
//    }
//    theSemantics.assign(FogSemantics::IS_OBJECT_DECLARATION);
//    return true;
//  }

void FogObjectSpecifier::set_colon_value(FogExpr& anExpr)
{
    ERRMSG("BUG - should not set_colon_value " << viz(anExpr) << " to " << viz(*this));
}

void FogObjectSpecifier::set_initializer(const FogExpr& anExpr)
{
    ERRMSG("BUG - should not set_initializer " << viz(anExpr) << " to " << viz(*this));
}

//  
//    Copy the fields of objectSpecifier as a result of a make_specifier making this from objectSpecifier.
//  
void FogObjectSpecifier::set_specifier(FogObjectSpecifier& objectSpecifier)
{
    _modifiers = objectSpecifier.modifiers();
    set_decl_specifiers(objectSpecifier.decl_specifiers());
    set_type(objectSpecifier.type());
}

void FogObjectSpecifier::set_type(FogName& typeName)
{
    if (_type)
        ERRMSG("More than one type-specifier for " << viz(*this));
    _type = typeName;
}

//  
//    Convert this specifier to be using the type characteristics of usedSpecifier.
//  
void FogObjectSpecifier::set_used(const FogObjectSpecifier& usedSpecifier)
{
    if (decl_specifiers().is_using())
        ERRMSG("BUG -- already using when applying FogObjectSpecifier::set_used to " << viz(*this));
    set_decl_specifiers(FogDeclSpecifierValue::USING_MASK);
    if ((decl_specifiers() & FogDeclSpecifierValue::ANY_CV_MASK) != FogDeclSpecifierValue())
        ERRMSG("BUG -- already cv when applying FogObjectSpecifier::set_used to " << viz(*this));
    set_decl_specifiers(usedSpecifier.decl_specifiers() & FogDeclSpecifierValue::ANY_CV_MASK);
    if ((decl_specifiers() & FogDeclSpecifierValue::ANY_STATIC_MASK) != FogDeclSpecifierValue())
        ERRMSG("BUG -- already static when applying FogObjectSpecifier::set_used to " << viz(*this));
    set_decl_specifiers(usedSpecifier.decl_specifiers() & FogDeclSpecifierValue::ANY_STATIC_MASK);
    if (_type)
        ERRMSG("BUG -- type when applying FogObjectSpecifier::set_used to " << viz(*this));
    _type = usedSpecifier._type;
    if (_modifiers.tally())
        ERRMSG("BUG -- modifiers when applying FogObjectSpecifier::set_used to " << viz(*this));
    _modifiers = usedSpecifier._modifiers;
    if (_subspace)
        ERRMSG("BUG -- subspace when applying FogObjectSpecifier::set_used to " << viz(*this));
}

//  
//    Convert this specifier to be the using successor of usingSpecifier.
//  
void FogObjectSpecifier::set_using(const FogObjectSpecifier& usingSpecifier)
{
    if (usingSpecifier._modifiers.tally())
        ERRMSG("type modifiers of " << viz(usingSpecifier) << " ignored for " << viz(*this));
    if ((usingSpecifier.decl_specifiers() & FogDeclSpecifierValue::ANY_CV_MASK) != FogDeclSpecifierValue())
        ERRMSG("cv-qualifiers of " << viz(usingSpecifier) << " ignored for " << viz(*this));
    if ((usingSpecifier.decl_specifiers() & FogDeclSpecifierValue::ANY_STATIC_MASK) != FogDeclSpecifierValue())
        ERRMSG("static-qualifiers of " << viz(usingSpecifier) << " ignored for " << viz(*this));
    set_decl_specifiers(usingSpecifier.decl_specifiers() & FogDeclSpecifierValue::ACCESS_MASK);
    set_decl_specifiers(usingSpecifier.decl_specifiers() & FogDeclSpecifierValue::INLINE_MASK);
    set_decl_specifiers(usingSpecifier.decl_specifiers() & FogDeclSpecifierValue::ANY_VIRTUAL_MASK);
    if (_subspace)
        ERRMSG("BUG -- subspace when applying FogObjectSpecifier::set_using to " << viz(*this));
    _subspace = usingSpecifier._subspace;
    _is_nested |= usingSpecifier._is_nested;
}
