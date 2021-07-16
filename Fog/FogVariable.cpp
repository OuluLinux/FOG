#include <Fog/FogIncludeAll.h>


















TYPEINFO_SINGLE(FogVariable, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogVariable)
PRIMREFS_IMPL(FogVariable)
PRIMLISTOFREFS_IMPL(FogVariable)
PRIMMAPOFREFS_IMPL(FogVariable)
TMPL_HACK_FIX_DO(FogVariable)

FogVariable::FogVariable() {}

FogVariable::FogVariable(FogMakerContext& makerContext)
:
    Super(makerContext)
{
    if (scope().is_instantiation())
    {
        FogScope& primaryScope = scope().primary_instantiation();
        FogVariable *primaryVariable = primaryScope.find_local_variable(*this);
        if (primaryVariable)
            set_primary_instantiation(*primaryVariable);
        else
            ERRMSG("Failed to find primary variable for specialisation by " << viz(makerContext));
    }
}

FogVariable::~FogVariable() {}

void FogVariable::adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration)
{
    Super::adopt(inScope, potentialDeclaration);
    inner_scope().add_variable(*this, &potentialDeclaration);
}

int FogVariable::compare(const FogVariableRefToConst *p1, const FogVariableRefToConst *p2)
{
    return Super::compare(**p1, **p2);
}

void FogVariable::create_usages()
{
    if (specifier().is_null())
        return;
    const FogVariableSpecifier *aSpecifier = get_specifier();
    if (!aSpecifier)
        return;
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier->decl_specifiers();
    if (!declSpecifiers.is_static() && !name_scope().is_name_space())
    {
        FogStaticUsageContext tailUsage(scope(), *this, FogOfUseBy::of_tail_by_head());
        aSpecifier->create_usage(tailUsage);                //  .bugbug statics could be templated ?
    }
    else
    {
        //   Following test is a pragmatic fudge to keep 13.5 test ok, by forcing excessive template
        //   include file resolution to ensure that partial specialisations are in place for forward ref.
        if (!declSpecifiers.is_static())
        {
            FogStaticUsageContext tailUsage(scope(), *this, FogOfUseBy::of_tail_by_head());
            aSpecifier->create_usage(tailUsage);                //  .bugbug statics could be templated ?
        }
        FogStaticUsageContext nameUsage(scope(), *this, FogOfUseBy::of_name_by_head());
        aSpecifier->create_usage(nameUsage);
        if (aSpecifier->initializer())
        {
            FogStaticUsageContext tailUsage(scope(), *this, FogOfUseBy::of_tail_by_implementation());
            aSpecifier->create_usage(tailUsage);
            scope().interface_usage_finish().add_use_by(implementation_usage());
        }
    }
    if (!scope().is_instantiation())                                    //   Instantiations have no interface.
    {
        scope().interface_usage_start().add_use_by(interface_usage_start());
        interface_usage_finish().add_use_by(scope().interface_usage_finish());
    }
}

const FogExpr *FogVariable::default_initialiser(bool isCopy) const
{
    const FogVariableSpecifier *aSpecifier = get_specifier();
    return aSpecifier ? aSpecifier->initializer() : 0;
}

bool FogVariable::do_compile(FogCompileContext& inScope)
{
    FogProgressMonitor aMonitor("Compiling", *this);
    if (!Super::do_compile(inScope))
        return false;
    if (!specifier().is_null())                                 //   If this is an actual entity
        return true;                                            //    great, it stays
    FogInScopeContext inContext(inScope, IN_BASE_NOT_THIS_SCOPE);   //   If not we have to find out what
    set_referenced_entity(find_name_in(inContext));             //    actual entity gets is referenced
    return false;
}

bool FogVariable::do_derive(FogDeriveContext& inScope)
{
    FogProgressMonitor aMonitor("Deriving into", *this);
    return Super::do_derive(inScope);
}

void FogVariable::emit_implementation(FogStream& s)
{
    FogProgressMonitor aMonitor("Emitting implementation for", *this);
    if (!compiled())
        ERRMSG("BUG -- should not invoke emit_implementation before compilation of " << viz(*this));
    const FogVariableSpecifier *aSpecifier = get_specifier();
    if (!aSpecifier)
        return;
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier->decl_specifiers();
    if (!declSpecifiers.is_static() && !name_scope().is_name_space())
        return;
    FogStreamEntityImplementation nestedStream(s, *this);
    FogStaticEmitContext emitContext(s, scope(), FogEmitContext::GLOBAL_IMPLEMENTATION);
    emitContext.emit_hash_line(aSpecifier->line());
    emit_template_prefix(emitContext);
    aSpecifier->emit_implementation(emitContext);
}

void FogVariable::emit_interface(FogStream& s)
{
    FogProgressMonitor aMonitor("Emitting interface for", *this);
    if (!compiled())
        ERRMSG("BUG -- should not invoke emit_interface before compilation of " << viz(*this));
    if (scope().is_instantiation())
        ERRMSG("BUG -- should not invoke emit_interface for instantiation " << viz(*this));
    const FogVariableSpecifier *aSpecifier = get_specifier();
    if (!aSpecifier)
        return;
    FogStreamEntityInterface nestedStream(s, *this);
    FogStaticEmitContext emitContext(s, scope(), FogEmitContext::LOCAL_INTERFACE);
    aSpecifier->emit_interface(emitContext);
}

const FogVariableSpecifier *FogVariable::get_specifier() const
{
    const FogVariableSpecifier *aSpecifier = specifier().is_variable_specifier();
    if (!aSpecifier)
        ERRMSG("BUG -- expected variable-specifier for " << viz(*this));
    return aSpecifier;
}

void FogVariable::install_names()
{
    scope().add_name(*this);
    //  .bugbug if instance of anon-union add all members as names.
}

FogEntity *FogVariable::instantiate(FogScopeContext& inScope, InstantiateOption ifRequired)
{
    FogStaticScopeContext thisScope(scope());
    if (!decl_specifiers(thisScope).is_static())
        return 0;
    if (scope().is_instantiation())
        ERRMSG("BUG -- should not invoke instantiate for instantiation " << viz(*this));
    else if (scope().is_specialisation())
        ERRMSG("BUG -- should not invoke instantiate for specialisation " << viz(*this));
    FogVariable *aVariable = 0;
    for (FogPotentialDeclarationConstListOfRefIterator p(potentials()); p; ++p)
    {
        FogPotentialDeclaration& potentialDeclaration = *p;
        const FogSpecifier& aSpecifier = p->specifier();
        const FogVariableSpecifier *variableSpecifier = aSpecifier.get_variable_specifier(*this);           
        if (!aVariable)
            aVariable = inScope.dynamic_scope().find_local_variable(*this);
        if (aVariable)
        {
//            FogRecompileMakeEntityContext makeEntityContext(inScope, potentialDeclaration);
//            FogEntityMakerContext makerContext(makeEntityContext, aSpecifier, &FogScope::make_variable_entity);
//            aVariable->add(makerContext);
        }
        else if (potentialDeclaration.is_active(thisScope))
        {
            FogRecompileMakeEntityContext explicitContext(inScope, potentialDeclaration, HAS_CONTEXT);
            FogEntity *anEntity = aSpecifier.make_name_entity(explicitContext);
            aVariable = anEntity ? anEntity->is_variable() : 0;
            if (aVariable)
                aVariable->set_utility(min(utility(), inScope.dynamic_token().utility()));
        }
    }
//    if (aVariable)
//        aVariable->set_primary(*this);
    return aVariable;
}

FogUsage& FogVariable::interface_usage_finish()
{
    if (decl_specifiers().is_static() && name_scope().is_name_space())
        return FogUsage::mutable_null();                        //   No interface for global statics
    else
        return Super::interface_usage_finish();
}

FogUsage& FogVariable::interface_usage_start() { return interface_usage_finish(); }
FogVariable *FogVariable::is_variable() { return this; }
FogUsage *FogVariable::make_implementation_usage() { return usage_manager().make_implementation_usage(*this); }

FogUsage *FogVariable::make_interface_usage_finish()
{
    return usage_manager().make_variable_usage(*this, decl_specifiers());
}

const FogMetaType& FogVariable::meta_type() const { return FogMetaType::variable_type(); }

#if 0
const PrimNumber *FogVariable::number_value() const
{
    //  .bugbug - should evaluate constants. string/id too.
    //  .bugbug - should check derive_at ?.
    //  .bugbug - should resolve conflicting values.
    const PrimNumber *theNumber = 0;
    bool isConst = false;
    FogScopeConstContext scopeContext(scope());
    for (FogPotentialDeclarationConstListOfRefToConstIterator p(potentials()); p; ++p)
    {
        const FogPotentialDeclaration& potentialDeclaration = *p;
        const FogRawDerivable& aDeclaration = potentialDeclaration.declaration();
        const FogModifier& aDecl = aDeclaration.decl();
        if (aDecl.is_const())
            isConst = true;
        const FogExpr *initExpr = aDeclaration.init_expr();
        if (initExpr)
        {
            const PrimNumber& aNumber = initExpr->get_number(scopeContext());
            if (!aNumber.is_null() && !theNumber)
                theNumber = &aNumber;
        }
    }
    if (isConst && theNumber)
        return theNumber;
    return Super::number_value();
}
#endif

std::ostream& FogVariable::print_viz(std::ostream& s) const
{
    s << "\"variable: ";
    print_this(s);
    return s << '\"';
}

bool FogVariable::validate() const { return Super::validate(); }
