#include <Fog/FogIncludeAll.h>




typedef PrimSetElement<FogExprRef> FogExprSetOfRefElement;
typedef PrimMapOfRef<FogExprSetOfRefElement> FogExprSetOfRef;
typedef PrimConstMapOfRefToConstIterator<FogExprSetOfRefElement> FogExprConstSetOfRefToConstIterator;

TYPEINFO_SINGLE(FogFunction, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogFunction)
PRIMREFS_IMPL(FogFunction)
PRIMLISTOFREFS_IMPL(FogFunction)
PRIMMAPOFREFS_IMPL(FogFunction)
TMPL_HACK_FIX_DO(FogFunction)

TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogExprSetOfRefElement)
PRIMMAPOFREFS_IMPL(FogExprSetOfRefElement)
PRIMSETELEMENT_IMPL(FogExprSetOfRefElement)
TMPL_HACK_FIX_DO_TEMPLATE(FogExprSetOfRefElement)

FogFunction::FogFunction()
:
    _templates(*this),
    _in_line(FogInline::invalid_inline()),
    _virtual(FogVirtual::invalid_virtual()),
    _sort_class(0),
    _is_not_implemented(false),
    _has_explicit_implementation_file(false),
    _explicit_no_implementation(false)
{}

//  
//    Construct a primary function.
//  
FogFunction::FogFunction(FogMakerContext& makerContext)
:
    Super(makerContext, FogLazyBool::make_undecided()),
    _templates(*this, makerContext),
    _in_line(FogInline::invalid_inline()),
    _virtual(FogVirtual::invalid_virtual()),
    _sort_class(0),
    _is_not_implemented(false),
    _has_explicit_implementation_file(false),
    _explicit_no_implementation(false)
{
    _templates.install_parameters(makerContext);
//    const FogFunctionSpecifier *functionSpecifier = makerContext.specifier().get_function_specifier(*this);
//    if (functionSpecifier && functionSpecifier->is_constructor(makerContext))
//        *(int *)&_sort_class = FogRawEnums::CONSTRUCTOR;                    //.bugbug cast
    if (scope().is_instantiation())
    {
        FogScope& primaryScope = scope().primary_instantiation();
        FogFunction *primaryFunction = primaryScope.find_local_function(*this);
        if (primaryFunction)
            set_primary_instantiation(*primaryFunction);
        else
            ERRMSG("Failed to find primary function for specialisation by " << viz(makerContext));
    }
}

//  
//    Construct a specialisation of primaryFunction.
//  
FogFunction::FogFunction(This& primaryFunction, FogMakeTemplateContext& makeTemplateContext)
:
    Super(makeTemplateContext, FogLazyBool::make_undecided()),
    _templates(makeTemplateContext),
    _in_line(FogInline::invalid_inline()),
    _virtual(FogVirtual::invalid_virtual()),
    _sort_class(primaryFunction._sort_class),
    _is_not_implemented(false),
    _has_explicit_implementation_file(false),
    _explicit_no_implementation(false)
{
    _templates.install_parameters(makeTemplateContext);
    set_primary_specialisation(primaryFunction);
    scope().add_name(short_id(), *this);
}

FogFunction::~FogFunction() {}

void FogFunction::adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration)
{
    Super::adopt(inScope, potentialDeclaration);
    inner_scope().add_function(*this, &potentialDeclaration);
}

//  
//    Return true if the derived contribution of this function to inScope will be concrete if inScope is,
//        false if pure-virtual, undecided otherwise.
//  
const FogLazyBool& FogFunction::can_be_concrete(FogScopeContext& inScope)
{
    for (FogPotentialDeclarationConstListOfRefIterator p(potentials()); p; ++p)
    {
        const FogLazyBool& isConcrete = p->can_be_concrete(inScope);
        if (!isConcrete.is_undecided())
            return isConcrete;
    }
    return FogLazyBool::make_undecided();
}

int FogFunction::compare(const FogFunctionRefToConst *p1, const FogFunctionRefToConst *p2)
{
    int aDelta = (*p1)->global_signature_id().compare((*p2)->global_signature_id());
    return aDelta;
}

void FogFunction::compile_code(FogCodeTokensRef localCodes[], FogCodeTokensRef derivedCodes[])
{
    FogCodeTokensRef codeTokens;
    FogCodeTokensRef *pDerived = derivedCodes;
    FogCodeTokensRef *pLocal = localCodes;
    for (FogSegmentIterator i; i; ++i, ++pDerived, ++pLocal)
    {
        FogCodeTokens& derivedTokens = **pDerived;
        FogCodeTokens& localTokens = **pLocal;
        if (!localTokens.tally() && !derivedTokens.tally())
            continue;
        if (!codeTokens)
        {
            codeTokens.adopt(new FogCodeTokens);
            if (!codeTokens)
                break;
        }
        const FogSegment& aSegment = *i;
        const bool isCtorOrder = aSegment.ctor_order();
        if (isCtorOrder && derivedTokens.tally())
            codeTokens->add(derivedTokens);
        if (localTokens.tally())
            codeTokens->add(localTokens);
        if (!isCtorOrder && derivedTokens.tally())
            for (int j = derivedTokens.tally(); j-- > 0; )
                codeTokens->add(*derivedTokens.tokens()[j]);
    }
    if (codeTokens)
    {
        if (_is_not_implemented)
            ERRMSG("Implementation provided for !implemented " << viz(*this));
//        _code = codeTokens;
        FogStaticScopeContext scopeContext(scope());
        codeTokens->make_actual(_code, scopeContext);
    }
}

//  
//    The ordered list of ctor-inits comprises (in order)
//        miscellaneous identifiers (such as Super(*this))
//        immediate non-virtual base classes (in declaration order)
//        local member variables (in declaration order)
//        virtual base classes (in ?declaration order)
//  
void FogFunction::compile_ctors(FogCtorInitMapOfRefToConst& ctorInits, bool isCopy)
{
    PrimIdMap ctorInitMap;                                  //   ctorInits put into ctorInitList.
    FogCtorInitListOfRefToConst ctorInitList;               //   Ordered ctor-inits excluding misc's
    for (FogBaseClassConstListOfRefToConstIterator p2(scope().direct_bases()); p2; ++p2)
//    for (FogScopeConstListOfRefToConstIterator p2(scope().ctor_bases()); p2; ++p2)
    {                                                       //   Install base-class ctor-inits
        const FogBaseClass& baseClass = *p2;
        if (baseClass.is_virtual())
            continue;
        const FogScope& baseScope = baseClass.base();
        const PrimId& baseClassName = baseScope.unique_id();
        const FogCtorInit *ctorInit = ctorInits.find(baseClassName);
        if (!ctorInit && !isCopy)
        {
            const FogExpr *defaultInit = baseScope.default_initialiser(isCopy);
            if (defaultInit)
                ctorInit = new FogCtorInit(baseScope, *defaultInit);
            if (ctorInit)
                ctorInits.adopt(*ctorInit);
        }
        if (ctorInit)
        {
            ctorInitList.add(*ctorInit);
            ctorInitMap.add(ctorInit->id());
        }
    }

    for (FogVariableConstListOfRefToConstIterator p4(scope().distinct_variable_list()); p4; ++p4)
    {                                                       //   Install member-variable ctor-inits
        const FogVariable& aVariable = *p4;
        if (!aVariable.is_active())
            continue;
        if (!aVariable.decl_specifiers().is_static())
        {
            const PrimId& variableName = aVariable.unique_id();
            const FogCtorInit *ctorInit = ctorInits.find(variableName);
            if (!ctorInit)
            {
                const FogExpr *defaultInit = 0;
                if (isCopy)
                {
                    //  .bugbug Build member access - DotExpr(variableName), array variable copy synthesis
                }
                else
                    defaultInit = aVariable.default_initialiser(isCopy);
                if (defaultInit)
                    ctorInit = new FogCtorInit(aVariable, *defaultInit);
                if (ctorInit)
                    ctorInits.adopt(*ctorInit);
            }
            if (ctorInit)
            {
                ctorInitList.add(*ctorInit);
                ctorInitMap.add(ctorInit->id());
            }
        }
    }

    size_t virtualBases = scope().virtual_bases();
    const FogScope& constScope = scope();
    for (FogScopeConstListOfRefToConstIterator p5(constScope.all_bases()); p5; ++p5)
    {                                                       //   Install virtual base-class ctor-inits
        if (!virtualBases--)
            break;
        const FogScope& baseScope = *p5;
        const PrimId& baseClassName = baseScope.unique_id();
        const FogCtorInit *ctorInit = ctorInits.find(baseClassName);
        if (!ctorInit)
        {
            const FogExpr *defaultInit = baseScope.default_initialiser(isCopy);
            if (defaultInit)
                ctorInit = new FogCtorInit(baseScope, *defaultInit);
            if (ctorInit)
                ctorInits.adopt(*ctorInit);
        }
        if (ctorInit)
        {
            ctorInitList.add(*ctorInit);
            ctorInitMap.add(ctorInit->id());
        }
    }
    for (FogCtorInitConstMapOfRefToConstIterator p6(ctorInits); p6; ++p6)       //   Copy bonus inits to output
        if (!ctorInitMap.find(p6->id()))
        {
//              WRNMSG("BUG -- unexpected " << viz(*p6) << " copied.");   -- INVESTIGATE occurrence for 13_5.fog
            _ctors.add(*p6);
        }
    for (FogCtorInitConstListOfRefToConstIterator p7(ctorInitList); p7; ++p7)   //   Copy real inits to output
        _ctors.add(*p7);
}

void FogFunction::compile_friends()
{
//    const FogRawFunction *theDeclaration = specifier().is_raw_function();
//    if (theDeclaration)
//        for (FogScopeConstMapOfRefIterator p(theDeclaration->reverse_friends()); p; ++p)
//            p->add_friend(*this);
}

void FogFunction::compile_inline()
{
    bool hasImplementation = has_implementation();
    if (utility().is_emit() && scope().is_global_scope())
    {
        FogTargetFile *implFile = implementation_file();
    }
    const FogSpecifier& aSpecifier = specifier();
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier.decl_specifiers();
    _in_line = FogInline::flyweight(declSpecifiers);
    if (get_inline().is_if_short() && get_inline().is_redundant())
    {
        const FogFunctionSpecifier *functionSpecifier = get_specifier();
        if (_has_explicit_implementation_file)
            _in_line = FogInline::OUT_OF_LINE;
        else if (get_virtual().is_virtual())
            _in_line = FogInline::OUT_OF_LINE;
        else if (!hasImplementation)
            _in_line = FogInline::OUT_OF_LINE;
        else if (_is_not_implemented)
            _in_line = FogInline::OUT_OF_LINE;
        else if (functionSpecifier && functionSpecifier->reverse_friends().tally())
            _in_line = FogInline::OUT_OF_LINE;
        else if (executable_tokens() >= Fog::inline_threshold())
            _in_line = FogInline::OUT_OF_LINE;
        else if (scope().is_specialisation()
              && (scope().interface_file_source() != scope().primary().interface_file_source()))
            _in_line = FogInline::OUT_OF_LINE;
        else if (declSpecifiers.is_static() && name_scope().is_name_space())    //   exclude local statics from interface
            _in_line = FogInline::IN_IMPLEMENTATION;
        else
            _in_line = FogInline::IN_INTERFACE;
    }
}

void FogFunction::compile_missing()
{
    bool hasImplementation = has_implementation();
    bool explicit_no_implementation = is_explicit_no_implementation();
    const FogSpecifier& aSpecifier = specifier();
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier.decl_specifiers();
    if (utility().is_hash())
        ;
    else if (declSpecifiers.is_friend())
        ;
    else if (!interface_file_source())
        global_scope().add_missing_interface_file(unique_id());
    else if (get_virtual().is_pure())
        ;
    else if (declSpecifiers.is_extern())
        ;
    else if (!hasImplementation)
    {
        if (!_is_not_implemented && !explicit_no_implementation)
        {
            const FogTargetFile *aFile = interface_file_source();
            if (!aFile || (!aFile->is_input() && !aFile->is_frozen_input()))
                global_scope().add_missing_implementation(unique_id());
        }
    }
    else if (utility().is_emit() && !implementation_file())
        global_scope().add_missing_implementation_file(unique_id());
}

void FogFunction::compile_sort_class(FogScopeContext& scopeContext)
{
    const FogFunctionSpecifier *functionSpecifier = get_specifier();
    if (!functionSpecifier)
        ;
    else if (functionSpecifier && functionSpecifier->is_constructor(scopeContext))
        _sort_class = CONSTRUCTOR_NAME;
    else
        _sort_class = functionSpecifier->name().sort_class();
}

//  
//    A function is concrete.
//  
//    If explicitly declared pure, it is pure - so not concrete.
//    Else if no immediate base function is pure, function is not pure - so concrete. 
//    Else if can_be_concrete() and scope is not pure - function is not pure - so concrete.
//    Else (inherited pure, without any local implementation) - function is still pure - so not concrete.
//  
void FogFunction::compile_virtual(FogCompileContext& inScope)
{
    const FogSpecifier& aSpecifier = specifier();
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier.decl_specifiers();
    if (!declSpecifiers.is_virtual())
    {
        if (!_virtual->is_virtual())
        {
            _virtual = FogVirtual::NOT_VIRTUAL;
            set_is_pure(FogLazyBool::make_false());
        }
        return;
    }
//    if (scope().is_pure() || !can_be_concrete(scope()))
    const FogLazyBool& isPure = scope().is_pure();
    if (isPure.is_undecided())
     ERRMSG("BUG -- use of undecided is_pure for " << viz(*this));
    const FogLazyBool& isConcrete = can_be_concrete(inScope);
//      if (isConcrete.is_undecided())
//  	    ERRMSG("BUG -- use of undecided can_be_concrete for " << viz(*this));
    if (!isPure.is_false() || isConcrete.is_false())            //   Tacky form of declSpecifiers.is_pure_virtual() ??
    {
        for (FogPotentialDeclarationConstListOfRefIterator p(potentials()); p; ++p)
        {
            FogPotentialDeclaration& potentialDeclaration = *p;
            const FogSpecifier& aSpecifier = p->specifier();
            if (potentialDeclaration.is_active(inScope))
            {
                if (aSpecifier.decl_specifiers().is_pure_virtual())
                {
                    _virtual = FogVirtual::PURE_VIRTUAL;
                    set_is_pure(FogLazyBool::make_true());
                    return;
                }
            }
        }
    }
    _virtual = FogVirtual::VIRTUAL;
    set_is_pure(FogLazyBool::make_false());
}

void FogFunction::create_usages()
{
    if (specifier().is_null())
        return;
    FogStaticUsageContext nameByInterface(scope(), *this, FogOfUseBy::of_name_by_head());
    specifier().create_usage(nameByInterface);
 if (!scope().is_instantiation())         //   Instantiations have no interface.
    {
        scope().interface_usage_start().add_use_by(interface_usage_start());
        interface_usage_finish().add_use_by(scope().interface_usage_finish());
    }
    if (is_specialisation())
        primary_specialisation().interface_usage_finish().add_use_by(interface_usage_start());
    if (has_implementation())
    {
        if (!scope().is_name_space())
            scope().inline_usage().add_use_by(implementation_usage());
        else
            scope().interface_usage_finish().add_use_by(implementation_usage());
    }
    FogStaticUsageContext interfaceByImplementation(scope(), *this, FogOfUseBy::of_tail_by_implementation());
    for (FogCtorInitConstListOfRefToConstIterator p(_ctors); p; ++p)
        p->create_usage(interfaceByImplementation);
    _code->create_usage(interfaceByImplementation);
    _templates.create_usages(nameByInterface);
}

//  
//    Return the specialisation of this primary function, deducing any missing template arguments
//    from the function signature. A new specialisation may be created if required. 
//  
FogFunction *FogFunction::deduce_specialisation(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!is_templated())
    {
        ERRMSG("BUG -- should not deduce_specialisation of non-template " << viz(*this));
        return this;
    }
    if (has_distinct_primary())
        return primary_specialisation().deduce_specialisation(makeSpecifierContext);
    FogTemplateArgsRefToConst templateArgs;
    bool gotIt = makeSpecifierContext.resolve_template(*this, templateArgs);
    if (!gotIt)             //  .bugbug must handle partial case with short argument too.
    {
        const FogSpecifier& aSpecifier = makeSpecifierContext.specifier();
        const FogFunctionSpecifier *functionSpecifier = aSpecifier.is_function_specifier();
        const FogFunctionModifier *functionModifier = functionSpecifier ? functionSpecifier->has_function_signature() : 0;
        const FogParameterSpecifierListOfRef *functionParameters = functionModifier ? &functionModifier->parameters() : 0;
        const FogObjectSpecifier& primarySpecifier = specifier(makeSpecifierContext);
        const FogFunctionSpecifier *primaryFunctionSpecifier = primarySpecifier.is_function_specifier();
        const FogFunctionModifier *primaryFunctionModifier = primaryFunctionSpecifier ? primaryFunctionSpecifier->has_function_signature() : 0;
        const FogParameterSpecifierListOfRef *primaryFunctionParameters = primaryFunctionModifier ? &primaryFunctionModifier->parameters() : 0;
        if (primaryFunctionParameters && functionParameters)
        {
            const FogTemplateParameterSpecifiers& templateParameters = template_parameters();
            const size_t firstNumber =templateParameters.first_template_parameter_number();
            const size_t lastNumber = templateParameters.last_template_parameter_number();
            size_t failedDeductions = 0;
            //  
            //    Initialise map of parameters to be deduced.
            //  
            FogExprSetOfRef deducedValues;
            for (int i1 = firstNumber+1; i1 <= lastNumber; i1++)
            {
                const FogTemplateParameterSpecifier& tP = templateParameters.template_parameter_number(i1);
                const FogName& tPN = tP.name();
                const FogTemplateParameterSpecifierId *tPSId = tPN.is_template_parameter_specifier_id();
                if (!tPSId)
                    ERRMSG("BUG -- Expected FogTemplateParameterSpecifierId for " << viz(tP));
                else
                    deducedValues.adopt(new FogExprSetOfRefElement(tPSId->id(), FogExpr::mutable_null()));
            }
            //  
            //    Do a deduction on each function parameter.
            //  
         FogParameterSpecifierConstListOfRefIterator p2(*functionParameters);
         for (FogParameterSpecifierConstListOfRefIterator p1(*primaryFunctionParameters); p1 && p2; ++p1, ++p2)
            {
                FogExprRef deducedValue;
                const FogTemplateParameterSpecifierId *templateId = p1->deduce_template_parameter(*p2, deducedValue);
                if (templateId)
                {
                 const PrimId& anId = templateId->id();
                 FogExprSetOfRefElement *theExpr = deducedValues.find(anId);
                    if (!theExpr)
                        ERRMSG("BUG -- Did not expect to deduce value of " << viz(*templateId));
                    else if (theExpr->value()->is_null())
                        theExpr->set_value(*deducedValue);
                    else
                    {
                        const FogExpr& oldValue = *theExpr->value();
                        FogMergeContext mergeContext(makeSpecifierContext);
                        const FogMerge& theMerge = oldValue.needs_merge(mergeContext, *deducedValue);
                        if (!theMerge.both_are_valid())
                        {
                            ERRMSG("Inconsistent deductions for " << viz(*templateId) << " of " << viz(*this)
                                    << "\n\t" << viz(oldValue) << " and " << viz(*deducedValue));
                            failedDeductions++;
                        }
                    }
                }
            }
            //  
            //    Check all parameters got deduced.
            //  
            for (FogExprConstSetOfRefToConstIterator p(deducedValues); p; p++)
            {
             const FogExprSetOfRefElement& anElement = *p;
                if (anElement.value()->is_null())
                {
                    ERRMSG("Failed to deduce value for " << anElement.id() << " of " << viz(*this));
                    failedDeductions++;
                }
            }
            if (failedDeductions)
                return 0;
            FogListOfExprRef exprList(new FogListOfExpr, FogListOfExprRef::ADOPT);
            for (int i2 = firstNumber+1; i2 <= lastNumber; i2++)
            {
                const FogTemplateParameterSpecifier& tP = templateParameters.template_parameter_number(i2);
                const FogName& tPN = tP.name();
                const FogTemplateParameterSpecifierId *tPSId = tPN.is_template_parameter_specifier_id();
                if (!tPSId)
                    ERRMSG("BUG -- Expected FogTemplateParameterSpecifierId for " << viz(tP));
                else
                    exprList->add(*deducedValues.find(tPSId->id())->value());
            }
            FogName& untemplatedName = makeSpecifierContext.specifier().name();
            const FogTemplatedName *templatedName = new FogTemplatedName(untemplatedName, *exprList);
            FogNameRefToConst garbageCollectedName(templatedName, FogNameRefToConst::ADOPT);
            FogTemplatedMakeSpecifierContext templatedMakeSpecifierContext(makeSpecifierContext, *templatedName);
            return deduce_specialisation(templatedMakeSpecifierContext);
        }
    }
    FogMakeTemplateContext makeTemplateContext(makeSpecifierContext, *this, *templateArgs, IS_DEFINITION);
    FogEntity *foundEntity = _templates.find_template(makeTemplateContext);
    if (foundEntity)
        return foundEntity->is_function();
    foundEntity = _templates.make_template(makeTemplateContext);
    if (foundEntity)
        return foundEntity->is_function();
    ERRMSG("Failed to find " << viz(*templateArgs) << " specialisation of " << viz(*this));
    return 0;
}

bool FogFunction::do_compile(FogCompileContext& inScope)
{
    FogProgressMonitor aMonitor("Compiling", *this);
    if (!Super::do_compile(inScope))
        return false;
    _virtual = FogVirtual::initial();                       //   Need _virtual to support leap-frogged pure-virtuals
    for (FogBaseClassConstListOfRefIterator p(scope().direct_bases()); p; ++p)
//    for (FogScopeConstListOfRefIterator p(scope().ctor_bases()); p; ++p)
    {
        FogScope& baseClass = p->base();
        const FogFunction *baseFunction = baseClass.find_local_function(*this);
        if (baseFunction)
            _virtual = _virtual->merge_virtual(baseFunction->get_virtual());
    }
    if (specifier().is_null())                                  //   If this is not an actual entity
    {
        FogInScopeContext inContext(inScope, IN_BASE_NOT_THIS_SCOPE);   //   We have to find out what
        set_referenced_entity(find_signature_in(inContext));            //    actual entity gets is referenced
        return false;
    }
    for (FogPotentialDeclarationConstListOfRefIterator p7(potentials()); p7; ++p7)
    {
        FogPotentialDeclaration& potentialDeclaration = *p7;
        const FogSpecifier& aSpecifier = p7->specifier();
        if (!potentialDeclaration.is_active(inScope))
            ;
        else if (potentialDeclaration.is_root())
            /*  aSpecifier.compile_code(*this, localCodes, ctorInits)*/;
        else
            /*  aSpecifier.compile_code(*this, derivedCodes, ctorInits)*/; //  .bugbug is order defined ?
    }
    bool isImplemented = !_is_not_implemented || _raw_ctor_inits.tally();
    if (!isImplemented)
    {
        const FogCodeTokensRef *pDerived = _raw_derived_codes;
        const FogCodeTokensRef *pLocal = _raw_local_codes;
        for (FogSegmentIterator i; i; ++i, ++pDerived, ++pLocal)
        {
            const FogCodeTokens& derivedTokens = **pDerived;
            const FogCodeTokens& localTokens = **pLocal;
            if (!localTokens.tally() && !derivedTokens.tally())
                continue;
            isImplemented = true;
            break;
        }
    }   
    if (isImplemented)
    {
        const FogFunctionSpecifier *functionSpecifier = get_specifier();
        if (functionSpecifier && functionSpecifier->is_constructor(inScope))
        {
            bool isCopy = functionSpecifier->is_copy(short_id());
            compile_ctors(_raw_ctor_inits, isCopy);
        }
        compile_code(_raw_local_codes, _raw_derived_codes);
    }
    compile_friends();
    compile_virtual(inScope);
    compile_inline();                               //   After compile_friends(), compile_virtual().
    compile_missing();
    compile_sort_class(inScope);
    _templates.do_compile(inScope);
    return true;
}

bool FogFunction::do_derive(FogDeriveContext& inScope)
{
	FogFunction* prev = this;
	std::swap(prev, inScope.function);
    
    FogProgressMonitor aMonitor("Deriving into", *this);
    if (!Super::do_derive(inScope)) {
        std::swap(prev, inScope.function);
        return false;
    }
    _templates.do_derive(inScope);
    
    std::swap(prev, inScope.function);
    return true;
}

void FogFunction::emit_friend_interface(FogStream& s)
{
    FogProgressMonitor aMonitor("Emitting friend interface for", *this);
    FogStaticEmitContext emitContext(s, scope(), FogEmitContext::LOCAL_INTERFACE);
    emit_template_prefix(emitContext);
    emitContext.emit_space_and_text("friend");
    emit_partial_interface(emitContext);
    emitContext.emit_space_and_text(";\n");
}

void FogFunction::emit_implementation(FogStream& s)
{
    if (!compiled())
        ERRMSG("BUG -- should not invoke emit_implementation before compilation of " << viz(*this));
    if (!has_implementation())
        return;
    FogProgressMonitor aMonitor("Emitting implementation for", *this);
    const FogSpecifier& aSpecifier = specifier();
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier.decl_specifiers();
//    if (get_virtual().is_pure_virtual())
//        return;
    next(s);
    FogStreamEntityImplementation nestedStream(s, *this);
    FogStaticEmitContext emitContext(s, scope(), FogEmitContext::GLOBAL_IMPLEMENTATION);
    emitContext.emit_hash_line(aSpecifier.line());
    emit_template_prefix(emitContext);
    if (declSpecifiers.is_extern())
        emitContext.emit_space_and_text("extern");
    if (!get_inline().is_redundant())
        emitContext.emit_space_and_text("inline ");
//        emitContext.emit_space_and_text(get_inline().str());
    if (scope().is_name_space() && declSpecifiers.is_static())
        emitContext.emit_space_and_text("static");
//    if (scope().is_name_space() && !get_static().is_redundant())
//        emitContext.emit_space_and_text(get_static().str());
    aSpecifier.emit(emitContext);
    if (_ctors.tally())
    {
        emitContext.emit_space_and_text("\n:");
        FogIndentEmitContext indentedContext(emitContext, 1, FogEmitContext::INDENTS);
        const char *prefix = "\n";
        for (FogCtorInitConstListOfRefToConstIterator p(_ctors); p; ++p)
        {
            indentedContext.emit_space_and_text(prefix);
            p->emit(indentedContext);
            prefix = ",\n";
        }
    }
    if (_code->executable_tokens())
    {
        emitContext.emit_space_and_text("\n{");
        {
            FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
            FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
            FogForUseAsEmitContext changedContext(separatedContext, FogEmitContext::EXPRESSION);
            _code->emit(changedContext);
        }
        emitContext.emit_space_and_text("\n};\n\n");
    }
    else if (_ctors.tally())
        emitContext.emit_space_and_text("\n{};\n\n");
    else
        emitContext.emit_space_and_text(" {};\n\n");
}

void FogFunction::emit_interface(FogStream& s)
{
    const FogTargetFile *targetFile = s.file();
    if (targetFile != interface_file_source())
    {
        if (targetFile)
            ERRMSG("BUG should not emit interface of " << viz(*this) << " to " << viz(*targetFile));
        else
            ERRMSG("BUG should not emit interface of " << viz(*this) << " to null file.");
        return;
    }
    if (is_instantiation())
        ERRMSG("BUG -- should not invoke emit_interface for instantiation " << viz(*this));
    FogProgressMonitor aMonitor("Emitting interface for", *this);
    FogStreamEntityInterface nestedStream(s, *this);
    FogStaticEmitContext emitContext(s, scope(), FogEmitContext::LOCAL_INTERFACE);
    emitContext.emit_hash_line(specifier().line());
    emit_template_prefix(emitContext);
    emit_partial_interface(emitContext);
    if (get_virtual().is_pure_virtual())
        emitContext.emit_space_and_text(" = 0");
    emitContext.emit_space_and_text(";\n");
}

void FogFunction::emit_partial_interface(FogEmitContext& emitContext) const
{
    if (!compiled())
        ERRMSG("BUG -- should not invoke emit_partial_interface before compilation of " << viz(*this));
    const FogSpecifier& aSpecifier = specifier();
    const FogDeclSpecifierValue& declSpecifiers = aSpecifier.decl_specifiers();
    if (declSpecifiers.is_extern())
        emitContext.emit_space_and_text("extern ");
    if (declSpecifiers.is_explicit())
        emitContext.emit_space_and_text("explicit ");
    if (!get_inline().is_redundant())
        emitContext.emit_space_and_text("inline ");
    if (declSpecifiers.is_static())
        emitContext.emit_space_and_text("static");
    if (!get_virtual().is_redundant())
        emitContext.emit_space_and_text(get_virtual().str());
    aSpecifier.emit(emitContext);
}

void FogFunction::emit_template_prefix(FogEmitContext& emitContext) const
{
    Super::emit_template_prefix(emitContext);
    _templates.emit_prefix(emitContext);
    emitContext.start();
}

//  
//    Estimate the complexity of the function/constructor implementation as the sum of the
//    non-trivial tokens in the body plus 5 tokens per constuctor initialiser.
//  
size_t FogFunction::executable_tokens() const
{
    size_t codeTokens = _code->executable_tokens();
    size_t ctorLines = _ctors.tally();
    return 5 * ctorLines + codeTokens;
}

//  
//    Return the specialisation of this primary function, deducing any missing template arguments
//    from the function signature. A new specialisation may be created if required. 
//  
FogFunction *FogFunction::find_specialisation(FogMakerContext& makerContext)
{
    if (!is_templated())
    {
        ERRMSG("BUG -- should not find_specialisation of non-template " << viz(*this));
        return this;
    }
    if (has_distinct_primary())
        return primary_specialisation().find_specialisation(makerContext);
    FogTemplateArgsRefToConst templateArgs;
    bool gotIt = makerContext.resolve_template(*this, templateArgs);
    if (!gotIt)
    {
        ERRMSG("Failed to resolve template arguments for " << viz(*this));
        return 0;
    }
    FogMakeTemplateContext makeTemplateContext(makerContext, *this, *templateArgs, IS_DEFINITION);
    FogEntity *foundEntity = _templates.find_template(makeTemplateContext);
    if (foundEntity)
        return foundEntity->is_function();
    foundEntity = _templates.make_template(makeTemplateContext);
    if (foundEntity)
        return foundEntity->is_function();
    ERRMSG("Failed to find " << viz(*templateArgs) << " specialisation of " << viz(*this));
    return 0;
}

//  
//    Return the <templateArguments> specialisation of this templated function.
//  
FogEntity *FogFunction::find_template(FogMakeTemplateContext& makeTemplateContext)
{
    if (has_distinct_primary())
        return primary_specialisation().find_template(makeTemplateContext);
    FogEntity *foundFunction = _templates.find_template(makeTemplateContext);
    if (foundFunction)
        return foundFunction;
    foundFunction = _templates.make_template(makeTemplateContext);
    if (!foundFunction)
        return 0;
    FogFunction *theFunction = foundFunction->is_function();
    if (theFunction)
        theFunction->_templates.check_classification(makeTemplateContext);
    return foundFunction;
}

const FogInline& FogFunction::get_inline() const
{
    if (actual_specifier().is_null())
        ERRMSG("Should not get_inline for " << viz(*this) << " without an actual declaration.");
    return *_in_line;
}

const FogFunctionSpecifier *FogFunction::get_specifier() const
{
    const FogFunctionSpecifier *functionSpecifier = specifier().is_function_specifier();
    if (!functionSpecifier)
        ERRMSG("BUG - expected function-specifier for " << viz(*this));
    return functionSpecifier;
}

const FogVirtual& FogFunction::get_virtual() const
{
    if (!done_compile())
        ERRMSG("BUG - should not access get_virtual() before compiling " << viz(*this));
    return *_virtual;
}

//  
//    Return true if there is some contribution to define a function implementation.
//  
bool FogFunction::has_implementation() const
{
    if (_ctors.tally())
        return true;
    if (_code->source_lines())
        return true;
    if (_code->tokens().tally())            //   Catch the case of {}
        return true;
    return false;
}

FogTargetFile *FogFunction::implementation_file()
{
    if (!has_implementation())
        return 0;
    else if (get_inline().is_in_interface())
        return interface_file_sink();
    else
        return Super::implementation_file();
}

void FogFunction::install_names()
{
    scope().add_name(short_id(), *this);
}

FogEntity *FogFunction::instantiate(FogScopeContext& inScope, InstantiateOption ifRequired)
{
    if (is_instantiation())
        ERRMSG("BUG -- should not invoke instantiate for instantiation " << viz(*this));
    FogFunction *aFunction = 0;
    for (FogPotentialDeclarationConstListOfRefIterator p(potentials()); p; ++p)
    {
        FogPotentialDeclaration& potentialDeclaration = *p;
        const FogSpecifier& aSpecifier = p->specifier();
//        if (!aFunction)
//        {
//            aFunction = scopeContext.find_local_function(*this);
//            FogMakeContext makeContext(scopeContext, short_id(), FogTemplateArgs::immutable_null());
//            PrimIdMap foundMap;
//            FogEntity *anEntity = intoScope.find_local_name(makeContext, foundMap);
//            aFunction = anEntity ? anEntity->is_function() : 0;
//        }
        if (aFunction)
        {
//            clonedDeclaration->compile(scopeContext);
            FogRecompileMakeEntityContext makeEntityContext(inScope, potentialDeclaration, NO_CONTEXT);
            FogEntityMakerContext makerContext(makeEntityContext, aSpecifier, &FogScope::make_function_entity);
            aFunction->add(makerContext);
        }
        else if (ifRequired != FORCED)
            break;
        else if (potentialDeclaration.is_active(inScope))
        {
            FogRecompileMakeEntityContext explicitContext(inScope, potentialDeclaration, NO_CONTEXT);
            FogEntity *anEntity = aSpecifier.make_name_entity(explicitContext);
            aFunction = anEntity ? anEntity->is_function() : 0;
            if (aFunction)
                aFunction->set_utility(min(utility(), inScope.dynamic_token().utility()));
        }
    }
//    if (aFunction)
//        aFunction->set_primary(*this);
    return aFunction;
}

FogUsage& FogFunction::interface_usage_finish()
{
    if (decl_specifiers().is_static() && name_scope().is_name_space())
        return FogUsage::mutable_null();                        //   No interface for global statics
    else
        return Super::interface_usage_finish();
}

FogUsage& FogFunction::interface_usage_start() { return interface_usage_finish(); }
FogFunction *FogFunction::is_function() { return this; }
bool FogFunction::is_instantiation() const { return _templates.is_instantiation(); }
bool FogFunction::is_templated() const { return _templates.is_templated(); }
FogUsage *FogFunction::make_implementation_usage() { return usage_manager().make_implementation_usage(*this); }

FogUsage *FogFunction::make_interface_usage_finish()
{
    return usage_manager().make_function_usage(*this, decl_specifiers());
}

const FogMetaType& FogFunction::meta_type() const { return FogMetaType::function_type(); }

FogEntity *FogFunction::new_template(FogMakeTemplateContext& makeTemplateContext)
{
    return new FogFunction(*this, makeTemplateContext);
}

const FogExprListOfRef& FogFunction::parameters()
{
    const FogCallExpr *callExpr = 0; //  specifier().find_call_expr();
    ERRMSG("ROUNDTUIT -- FogFunction::parameters()");
//    if (callExpr)
//        return callExpr->parameters();
    static const FogExprListOfRef nullList;
    return nullList;
}

const FogExprListOfRefToConst& FogFunction::parameters() const
{
    const FogCallExpr *callExpr = 0; //  specifier().find_call_expr();
    ERRMSG("ROUNDTUIT -- FogFunction::parameters()");
//    if (callExpr)
//        return callExpr->parameters();
    static const FogExprListOfRefToConst nullList;
    return nullList;
}

std::ostream& FogFunction::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _templates.print_deep(s, aDepth);
    return s;
}

std::ostream& FogFunction::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _templates.print_on(s, aDepth);
    return s;
}

std::ostream& FogFunction::print_this(std::ostream& s) const
{
    char tailChar = 0;
    const FogTemplateParameterSpecifiers& templateParameters = template_parameters();
    if (!templateParameters.is_null())
    {
        tailChar = templateParameters.print_named(s, 0, tailChar);
        s << ' ';
    }
    Super::print_this(s);
    if (_virtual->is_pure_virtual())
        s << " = 0";
    return s;
}

std::ostream& FogFunction::print_viz(std::ostream& s) const
{
    s << "\"function: ";
    print_this(s);
    return s << '\"';
}

bool FogFunction::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    theSemantics.assign(FogSemantics::IS_FUNCTION_DEFINITION);
    return true;
}

void FogFunction::set_implementation_file(FogTargetFile *aFile)
{
    Super::set_implementation_file(aFile);
    if (aFile)
        _has_explicit_implementation_file = true;
}
   
void FogFunction::set_is_pure(const FogLazyBool& isPure)
{
    Super::set_is_pure(isPure);
    if (isPure.is_true())
        scope().set_is_pure(isPure);
}

//  const FogModifier& FogFunction::signature() const
//  {
//    return declaration().declarator().signature();
//  }

int FogFunction::sort_class() const { return _sort_class; }
FogTemplateManager& FogFunction::template_manager() { return _templates; }
const FogTemplateParameterSpecifiers& FogFunction::template_parameters() const
    { return _templates.parameter_specifiers(); }

bool FogFunction::validate() const
{
    if (!Super::validate())
        return false;
    const FogSpecifier& d1 = specifier();
    if (d1.is_null())                                       //   Happens for a propagating pure virtual.
        return true;
    if (utility().is_hash())                                //   Avoid complaining about naff #include's
        return true;
    const FogDeclSpecifierValue& ds1 = d1.decl_specifiers();
    const FogAccess& access1 = FogAccess::flyweight(ds1);
    const FogStatic& static1 = FogStatic::flyweight(ds1);
    const FogVirtual& virtual1 = get_virtual();
    if (scope().is_name_space())
    {
        if (get_virtual().is_virtual())
            WRNMSG("Should not use virtual for global " << viz(*this) << ".");
    }
    if (get_virtual().is_pure_nonvirtual())
        WRNMSG("\"=0\" suffix ignored in the absence of \"virtual\" for " << viz(*this) << ".");
    for (FogScopeConstListOfRefToConstIterator p(scope().all_bases()); p; ++p)
    {
        const FogScope& baseScope = *p;
//        FogScopeConstContext baseScopeContext(baseScope);
        const FogFunction *baseFunction = baseScope.find_local_function(*this);
        if (baseFunction)
        {
            const FogSpecifier& d2 = baseFunction->specifier();
            if (d2.is_null())
                continue;
            const FogDeclSpecifierValue& ds2 = d2.decl_specifiers();
            const FogAccess& access2 = FogAccess::flyweight(ds2);
            const FogStatic& static2 = FogStatic::flyweight(ds2);
            const FogVirtual& virtual2 = baseFunction->get_virtual();
            if (access2.is_valid() && (access1.value() != access2.value()))
                WRNMSG("Conflicting access between\n\t\t" << viz(*this) << " and\n\t\t" << viz(*baseFunction));
            if (static2.is_valid() && (static1.value() != static2.value()))
                WRNMSG("Conflicting \"static\" between\n\t\t" << viz(*this)
                        << " and\n\t\t" << viz(*baseFunction));
            if (virtual2 && (virtual1.is_virtual() != virtual2.is_virtual()))
                WRNMSG("Conflicting \"virtual\" between\n\t\t" << viz(*this)
                        << " and\n\t\t" << viz(*baseFunction));
        }
    }
    return true;
}
