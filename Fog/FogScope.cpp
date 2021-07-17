#include <Fog/FogIncludeAll.h>



TYPEINFO_SINGLE(FogScope, Super)
PRIMREF_NULL_CLAS(FogScope)
    virtual std::ostream& print_viz(std::ostream& s) const;
};
PRIMREF_DERIVED_NULL_IMPL(FogScope)
PRIMREFS_IMPL(FogScope)
PRIMLISTOFREFS_IMPL(FogScope)
PRIMMAPOFREFS_IMPL(FogScope)
TMPL_HACK_FIX_DO(FogScope)

FogScope::FogScope()
:
    _nested(0),
    _braced(false),
    _is_leaf(FogLazyBool::make_true()),
    _is_frozen(true),
    _done_meta_construct(false),
    _inline_usage(0),
    _templates(*this)
{}

//  
//    Make a (templated) primary scope.
//  
FogScope::FogScope(const FogTag& aTag, FogMakerContext& makerContext)
:
    Super(makerContext, FogLazyBool::make_undecided()),
    _nested(makerContext.dynamic_scope().nesting_depth()+1),
    _tag(aTag),
    _braced(false),
    _is_leaf(FogLazyBool::make_undecided()),
    _is_frozen(false),
    _done_meta_construct(false),
    _inline_usage(0),
    _templates(*this, makerContext)
{
    _templates.install_parameters(makerContext);
}

//  
//    Make a (partial) specialisation of primaryScope.
//  
FogScope::FogScope(FogScope& primaryScope, FogMakeTemplateContext& makeTemplateContext)
:
    Super(makeTemplateContext, FogLazyBool::make_undecided()),
    _nested(makeTemplateContext.dynamic_scope().nesting_depth()),
    _tag(primaryScope.tag()),
    _access(makeTemplateContext.dynamic_scope().access()),
    _braced(false),
    _is_leaf(FogLazyBool::make_undecided()),
    _is_frozen(false),
    _done_meta_construct(false),
    _inline_usage(0),
    _templates(makeTemplateContext)
{
    _templates.install_parameters(makeTemplateContext);
//    add_specialisation(*this);
}

//  
//    Make a child scope of parentScope.
//  
FogScope::FogScope(const FogTag& aTag, FogScope& parentScope, const PrimId& anId)
:
    Super(parentScope, anId, anId),
    _nested(&parentScope == this ? 0 : parentScope.nesting_depth()+1),  //   Avoid recursion for global scope.
    _tag(aTag),
    _braced(false),
    _is_leaf(FogLazyBool::make_undecided()),
    _is_frozen(false),
    _done_meta_construct(false),
    _inline_usage(0),
    _templates(*this)
{}

FogScope::~FogScope() {}
const FogAccess& FogScope::access() const { return *_access; }

void FogScope::add_base(FogBaseClass& baseClass)
{
    if (_bases.add_base(*this, baseClass))
    {
        FogScope& baseScope = baseClass.base();
        const PrimId& anId = baseScope.short_id();
        const FogEntity *anEntity = find_local_type(anId);
        if (anEntity)
            WRNMSG("Base class name \"" << anId << "\" is hidden by " << viz(*anEntity));
        else
            add_type(baseScope);
        derive_from(baseClass);
    }
}

void FogScope::add_file_space(FogScope& fileSpace) { global_scope().add_file_space(fileSpace); }

void FogScope::add_function(FogFunction& aFunction, FogPotentialDeclaration *potentialDeclaration)
{
//    _functions.add_function(aFunction, potentialDeclaration);
    _derivations.derive_from(aFunction, potentialDeclaration);
}

void FogScope::add_name(const PrimId& typeId, FogEntity& anEntity)
    { name_scope()._names.add_name(typeId, anEntity); }

FogScope *FogScope::add_scope(FogScope& aScope)
{
    FogScope *theScope = _nested.add_scope(*this, aScope);
    if (theScope)
        name_scope()._types.add_scope(*theScope);
    return theScope;
}

void FogScope::add_type(FogEntity& anEntity) { add_type(anEntity, anEntity); }

void FogScope::add_type(const PrimId& typeName, FogEntity& typeValue)
{
    if (typeValue.is_name_space())
        ERRMSG("Should not install namespace " << viz(typeValue) << " as a type in " << viz(*this));
    name_scope()._types.add_type(typeName, typeValue);
}

void FogScope::add_type(FogEntity& typeName, FogEntity& typeValue)
{
    if (typeValue.is_name_space())
        ERRMSG("Should not install namespace " << viz(typeValue) << " as a type in " << viz(*this));
    name_scope()._types.add_type(typeName, typeValue);
}

void FogScope::add_typedef(FogTypedef& aTypedef, FogPotentialDeclaration *potentialDeclaration)
{
//    _typedefs.add_typedef(aTypedef, potentialDeclaration);
    _derivations.derive_from(aTypedef, potentialDeclaration);
}

void FogScope::add_using(FogUsing& aUsing)
{
    FogEntity& usedEntity = aUsing.entity();
    if (usedEntity.is_type())
        add_type(usedEntity, usedEntity);
    else
        add_name(aUsing.short_id(), usedEntity);
    name_scope()._usings.add_using(aUsing);
}

void FogScope::add_variable(FogVariable& aVariable, FogPotentialDeclaration *potentialDeclaration)
{
//    _variables.add_variable(aVariable, potentialDeclaration);
    _derivations.derive_from(aVariable, potentialDeclaration);
}

void FogScope::changed_base_class(FogBaseClass& baseClass)
{
    _bases.changed_base_class(*this, baseClass);
    _derivations.changed_base_class(*this, baseClass);
}

int FogScope::compare(const FogScopeRefToConst *p1, const FogScopeRefToConst *p2)
    { return Super::compare(**p1, **p2); }

//  
//    Post compilation pass - build the usage graph. Invoked per-scope in construction order.
//  
void FogScope::create_usages()
{
    FogProgressMonitor aMonitor("Establishing usage of ", *this);
	
	// hack fix: FogUsage::add_use_by was called for obviously bad values
	FogUsage::filter_name = &this->name_scope();
	
    Super::create_usages();
    _bases.create_usages(*this);
    _enums.create_usages();
    _friends.create_usages(*this);
    _functions.create_usages();
    _implementation.create_usages(*this);
    _interface.create_usages(*this);
//      _nested.create_usages(*this);             -- in interface_usage_start
    _typedefs.create_usages();
    _usings.create_usages();
    _variables.create_usages(*this);
    FogStaticUsageContext nameByNameContext(name_scope(), *this, FogOfUseBy::of_name_by_name());
    _templates.create_usages(nameByNameContext);
    name_usage();
    if (_braced && !is_auto())
        interface_usage_start();        //   Graph appears by side effect.
    
    FogUsage::filter_name = 0;
}

const FogAccess& FogScope::default_access() const { return FogAccess::flyweight(FogAccess::PUBLIC); }

const FogExpr *FogScope::default_initialiser(bool isCopy) const
{
    //  .bugbug
    return 0;
}

void FogScope::derive_from(FogBaseClass& baseClass)
{
    _functions.derive_from(*this, baseClass);
    _typedefs.derive_from(*this, baseClass);
    _variables.derive_from(*this, baseClass);
    _bases.derive_from(*this, baseClass);
    _derivations.derive_from(*this, baseClass);
    _templates.derive_from(*this, baseClass);
}

void FogScope::derive_from(FogFunction& baseFunction, FogPotentialDeclaration *potentialDeclaration)
{
    _functions.derive_from(*this, baseFunction, potentialDeclaration);
    _templates.derive_from(*this, baseFunction, potentialDeclaration);
}

void FogScope::derive_from(FogTypedef& baseTypedef, FogPotentialDeclaration *potentialDeclaration)
{
    _typedefs.derive_from(*this, baseTypedef, potentialDeclaration);
    _templates.derive_from(*this, baseTypedef, potentialDeclaration);
}

void FogScope::derive_from(FogVariable& baseVariable, FogPotentialDeclaration *potentialDeclaration)
{
    _variables.derive_from(*this, baseVariable, potentialDeclaration);
    _templates.derive_from(*this, baseVariable, potentialDeclaration);
}

void FogScope::destroy()
{
    Super::destroy();
    _bases.destroy();
    _derivations.destroy();
    _enums.destroy();
    _friends.destroy();
    _functions.destroy();
    _implementation.destroy();
    _interface.destroy();
    _names.destroy();
    _nested.destroy();
    _typedefs.destroy();
    _types.destroy();
    _usings.destroy();
    _variables.destroy();
    _templates.destroy();
}

//  
//    Final compilation pass - put this scope together. Invocated per-scope in construction order.
//  
bool FogScope::do_compile(FogCompileContext& outerScope)
{
    FogProgressMonitor aMonitor("Compiling", *this);
    if (!Super::do_compile(outerScope))
        return false;
    FogNestedCompileContext innerScope(outerScope, *this, IN_ANY_SCOPE);
    meta_execute(innerScope, FogMetaSlot::destructor_id());
    FogScopeListOfRef theBases(all_bases());
    for (size_t i = theBases.tally(); i-- ; )
        theBases[i]->meta_execute(innerScope, FogMetaSlot::destructor_id());
    FogScope& metaBase = global_scope().meta_base();
    if (&metaBase != this)
        metaBase.meta_execute(innerScope, FogMetaSlot::destructor_id());
    _bases.do_compile(outerScope);
    _enums.do_compile(innerScope);
//    _friends.do_compile(innerScope);
    _functions.do_compile(innerScope);
    _typedefs.do_compile(innerScope);
//    _usings.do_compile(innerScope);
    _variables.do_compile(innerScope);
    bool isConcrete = _functions.is_concrete();
    if (isConcrete)
    {
        if (is_pure().is_true())
           ERRMSG("BUG - Inconsistent purity prediction for " << viz(*this));
//          else
//              _is_pure = FogLazyBool::make_false();
    }
    else
    {
        if (is_pure().is_false())
           ERRMSG("BUG - Inconsistent purity prediction for " << viz(*this));
//          else
//              _is_pure = FogLazyBool::make_true();
    }
//    if (!has_distinct_primary() || _braced)
    if (_braced)
    {
        FogTargetFile *implementationFile = implementation_file();  //   Resolve the implementation file
        FogTargetFile *interfaceFile = interface_file_source();         //   Resolve the interface file
        if (implementationFile)
            implementationFile->set_max_utility(*this);
        if (interfaceFile)
            interfaceFile->set_max_utility(*this);
    }
    if (raw_implementation_file())
        _implementation.do_compile(innerScope, *raw_implementation_file());
    if (raw_interface_file())
        _interface.do_compile(innerScope, *raw_interface_file());
    return true;
}

//  
//    Main compilation pass - usings become potentials, then potentials become actuals.
//    Invoked hierarchically.
//  
bool FogScope::do_derive(FogDeriveContext& outerScope)
{
    if (has_distinct_primary() && !primary().done_derive())         //   Occurs if derivong from an instantiation
        primary().do_derive(outerScope);
    FogNestedDeriveContext innerScope(outerScope, *this, IN_ANY_SCOPE);
    if (!_done_meta_construct)
        do_meta_construct(innerScope);
    FogProgressMonitor aMonitor("Resolving derivation rules within", *this);
    if (!Super::do_derive(outerScope))
        return false;
    _bases.do_derive(outerScope);
//    _functions.do_using(*this);
    _usings.do_derive(innerScope);      //   Late so that own names can be used.
    if (!tag() && !is_global_scope())
    {
        WRNMSG("No definition specified the structure tag for " << viz(*this));
        _tag = FogTag::class_tag();
    }
    //  
    //    Determine concrete/pure from the potential derivation of all potential member functions.
    //  
    bool isConcrete = _functions.can_be_concrete(innerScope);
    if (isConcrete)
    {
        set_is_pure(FogLazyBool::make_false());
        //  
        //    Determine the concrete boundary if concrete with no or an abstract base class.
        //  
    //    _is_boundary = _is_concrete && (!bases().tally() || _bases.is_pure());
        set_is_boundary(FogLazyBool::make_bool(!_bases.all_bases().tally() || _bases.is_pure()));
    }
    else
    {
        set_is_pure(FogLazyBool::make_true());
        set_is_boundary(FogLazyBool::make_false());
    }
    if (_is_leaf->is_undecided())
        _is_leaf = FogLazyBool::make_true();
//    _usings.do_derive(innerScope);      // Early to make used names available for really obscure code.
    _enums.do_derive(innerScope);
    _friends.do_derive(innerScope);
    _typedefs.do_derive(innerScope);
    _variables.do_derive(innerScope);
    _functions.do_derive(innerScope);
    _nested.do_derive(innerScope);                      //   Last to since derivations may create more scopes.
    FogRoot& globalScope = global_scope();
    globalScope.add_ordered_scope(*this);               //   Memorise construction order.
    const FogScope& nameScope = parent_scope().name_scope();
    _templates.do_derive(innerScope);
    if (needs_emission())
        set_braced();
    if (!_braced && (&globalScope == &nameScope))       //   Still empty and only ever forward declared globally.
        set_max_utility(FogUtility::utility_utility()); //    means that this is a utility class.
//    if (!needs_emission() && !_braced && (&globalScope == &nameScope))  // Still empty and only ever forward declared globally.
//        set_max_utility(FogUtility::utility_utility()); //  means that this is a utility class.
    _is_frozen = true;
    return true;
}

//  
//    First user compilation pass. Invocated hierarchically.
//  
bool FogScope::do_meta_construct(FogScopeContext& outerScope)
{
    FogProgressMonitor aMonitor("Meta-constructing", *this);
    if (_done_meta_construct)
    {
        ERRMSG("BUG - should not re-do_meta_construct " << viz(*this));
        return false;
    }
    _done_meta_construct = true;
    _bases.do_meta_construct(outerScope);
    FogNestedDeriveContext innerScope(outerScope, *this, IN_ANY_SCOPE);
    PrimIdMap visitMap(64);
    visitMap.add(unique_id());
    FogScopeListOfRef nonVirtualBases;  
    FogScopeListOfRef virtualBases; 
    const FogScope *multipleBase = find_bases(visitMap, nonVirtualBases, virtualBases);
    FogScope& metaBase = global_scope().meta_base();
    if (&metaBase != this)
        metaBase.meta_execute(innerScope, FogMetaSlot::constructor_id());
    for (FogScopeConstListOfRefIterator p1(virtualBases); p1; ++p1)
        p1->meta_execute(innerScope, FogMetaSlot::constructor_id());
    for (FogScopeConstListOfRefIterator p2(nonVirtualBases); p2; ++p2)
        p2->meta_execute(innerScope, FogMetaSlot::constructor_id());
    meta_execute(innerScope, FogMetaSlot::constructor_id());
    _nested.do_meta_construct(outerScope);
//    _templates.do_meta_construct(outerScope);
    return true;
}

void FogScope::emit_base_interface(FogEmitContext& emitContext) const
{
    FogProgressMonitor aMonitor("Emitting base interface for", *this);
    _bases.emit_base_interface(emitContext);
}

void FogScope::emit_forward_reference(FogStream& s)
{
    if (is_instantiation())                                     //   Instantiations are just
        return;                                                 //    dummy nodes in the usage graph.
    FogProgressMonitor aMonitor("Emitting forward reference for", *this);
    FogScope *outerScope = outer_scope();
    if (!outerScope)                                        //   Never happens
    {
        /*if (tag().is_namespace()) {
	        FogStreamEntityInterface nestedStream(s, *this);
	        FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
	        emit_template_prefix(emitContext);
	        emitContext.emit_space_and_text(tag().str());
	        non_virtual_scope().emit_scope(emitContext, emitContext.dynamic_scope());
	        emitContext.emit_space_and_text(local_id().str());  //   Not local_signature_id since < T > suffix not wanted.
	        emitContext.emit_space_and_text(" {}\n");
        }
        else*/ {
            FogStreamEntityInterface nestedStream(s, *this);
	        FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
	        emit_template_prefix(emitContext);
	        emitContext.emit_space_and_text(tag().str());
	        non_virtual_scope().emit_scope(emitContext, emitContext.dynamic_scope());
	        emitContext.emit_space_and_text(local_id().str());  //   Not local_signature_id since < T > suffix not wanted.
	        emitContext.emit_space_and_text(";\n");
        }
    }
    else if (outerScope->is_name_space())                   //   Global forward reference.
    {
        FogStreamEntityInterface nestedStream(s, *outerScope);
        FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
        if (1) {
            emit_template_prefix(emitContext);
			emitContext.emit_space_and_text(tag().str());
			non_virtual_scope().emit_scope(emitContext, emitContext.dynamic_scope());
			emitContext.emit_space_and_text(local_id().str());  //   Not local_signature_id since < T > suffix not wanted.
			emitContext.emit_space_and_text(";\n");
        }
        // Fix exported code compilation problems
        else {
            emitContext.emit_space_and_text("namespace");
            
			non_virtual_scope().emit_scope_namespace(emitContext, emitContext.dynamic_scope());
			emitContext.emit_space_and_text(" {");
			emit_template_prefix(emitContext);
			emitContext.emit_space_and_text(tag().str());
			emitContext.emit_space_and_text(local_id().str());  //   Not local_signature_id since < T > suffix not wanted.
			emitContext.emit_space_and_text(";\n}\n");
        }
    }
    else
    {
        /*if (tag().is_namespace()) {
            FogStreamEntityInterface nestedStream(s, *this);
	        FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
	        emit_template_prefix(emitContext);
	        emitContext.emit_space_and_text(tag().str());
	//        non_virtual_scope().emit_scope(emitContext, emitContext.dynamic_scope());
	        emitContext.emit_space_and_text(local_id().str());  //   Not local_signature_id since < T > suffix not wanted.
	        emitContext.emit_space_and_text(" {}\n");
        }
        else*/ {
	        FogStreamEntityInterface nestedStream(s, *this);
	        FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
	        emit_template_prefix(emitContext);
	        emitContext.emit_space_and_text(tag().str());
	//        non_virtual_scope().emit_scope(emitContext, emitContext.dynamic_scope());
	        emitContext.emit_space_and_text(local_id().str());  //   Not local_signature_id since < T > suffix not wanted.
	        emitContext.emit_space_and_text(";\n");
        }
    }
}

void FogScope::emit_friend_interface(FogStream& s)
{
    FogProgressMonitor aMonitor("Emitting friend interface for", *this);
//    FogStreamEntityInterface nestedStream(s, *this);        -- redundant since within a scope anyway.
    FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
    emit_template_prefix(emitContext);
    emitContext.emit_space_and_text("friend");
    emitContext.emit_space_and_text(tag().id().str());
    non_virtual_scope().emit_scope(emitContext, emitContext.dynamic_scope());
    emitContext.emit_identifier(local_signature_id());
    emitContext.emit_space_and_text(";\n");
}

void FogScope::emit_implementation(FogStream& s)
{
	if (_implementation.is_no_implementation()) {
    	FogProgressMonitor aMonitor("Skipping the emitting of implementation for", *this);
	}
	else {
    	FogProgressMonitor aMonitor("Emitting implementation for", *this);
    	FogStaticEmitContext emitContext(s, s.inner_scope(), FogEmitContext::LOCAL_INTERFACE);
    	_implementation.emit_implementation(emitContext);
	}
}

void FogScope::emit_interface(FogStream& s)
{
    FogProgressMonitor aMonitor("Emitting interface for", *this);
    FogStaticEmitContext emitContext(s, s.inner_scope(), FogEmitContext::LOCAL_INTERFACE);
    _interface.emit_interface(emitContext);
}

//  
//    Emit the necessary part of the scope string to describe this scope within inScope.
//  
//    The emitted text is either blank or :: terminated.
//  
void FogScope::emit_scope(FogEmitContext& emitContext, const FogScope& inScope) const
{
    for (const FogScope *aScope = &inScope; //   Search for thisScope as a parent of inScope.
//            !aScope->is_null() && (nesting_depth() <= aScope->nesting_depth());
//                aScope = &aScope->scope())
            aScope && (nesting_depth() <= aScope->nesting_depth());
                aScope = aScope->outer_scope())
        if (aScope == this)                 //   If this scope visible as a parent of inScope.
//        if ((aScope == this) && (inScope.find_type(id()) == this))  // Is this necessary to defeat occlusion ?
            return;
    const FogScope *outerScope = outer_scope();
    if (outerScope)                             //   Else need to retry with parent and emit this scope as suffix
    {
        outerScope->emit_scope(emitContext, inScope);
        if (!is_global_scope())                         //   Note that root may have ids if given a file name.
            emitContext.emit_identifier(local_signature_id());
    }
    emitContext.emit_space_and_text("::");
}

// like emit_scope but without "::"
void FogScope::emit_scope_namespace(FogEmitContext& emitContext, const FogScope& inScope) const
{
    for (const FogScope *aScope = &inScope; //   Search for thisScope as a parent of inScope.
//            !aScope->is_null() && (nesting_depth() <= aScope->nesting_depth());
//                aScope = &aScope->scope())
            aScope && (nesting_depth() <= aScope->nesting_depth());
                aScope = aScope->outer_scope())
        if (aScope == this)                 //   If this scope visible as a parent of inScope.
//        if ((aScope == this) && (inScope.find_type(id()) == this))  // Is this necessary to defeat occlusion ?
            return;
    const FogScope *outerScope = outer_scope();
    if (outerScope)                             //   Else need to retry with parent and emit this scope as suffix
    {
        outerScope->emit_scope(emitContext, inScope);
        if (!is_global_scope())                         //   Note that root may have ids if given a file name.
            emitContext.emit_identifier(local_signature_id());
    }
}

//  
//    Emit an introducer to define this name scope. Derived name space and extern classes reimplement.
//  
void FogScope::emit_scope_head(FogStream& s) const {}

//  
//    Emit a trailer to complete this name scope. Derived name space and extern classes reimplement.
//  
void FogScope::emit_scope_tail(FogStream& s) const {}

void FogScope::emit_template_prefix(FogEmitContext& emitContext) const
{
    const FogScope *streamScope = emitContext.stream_scope();
    if (streamScope != this)
    {
        Super::emit_template_prefix(emitContext);
        _templates.emit_prefix(emitContext);
    }
}

//  
//    Locate the names or types satisfying and updating theFinder.
//  
void FogScope::find_entities(FogEntityFinder& theFinder)
{
    switch (theFinder.in_scope())
    {
        case IN_THIS_SCOPE:
            find_local_entity(theFinder);
            break;
        case IN_BASE_SCOPE:
            find_visible_entity(theFinder);
            break;
        case IN_BASE_NOT_THIS_SCOPE:
            find_scope_entity(theFinder);
            break;
        case IN_ANY_SCOPE:
        {
            for (FogScope *aScope = this; aScope; aScope = aScope->outer_scope())
                if (aScope->is_name_scope() && aScope->find_visible_entity(theFinder))
                    break;
            break;
        }
        case IN_ANY_NOT_THIS_SCOPE:
        {
            if (find_scope_entity(theFinder))
                break;
            for (FogScope *aScope = outer_scope(); aScope; aScope = aScope->outer_scope())
                if (aScope->is_name_scope() && aScope->find_visible_entity(theFinder))
                    break;
            break;
        }
    }
}

void FogScope::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const
{
    const FogScope *outerScope = outer_scope();
    if (!outerScope || outerScope->is_global_scope())
        Super::find_entities_in(inScope, theFinding);
    else
    {
        FogEntity *anEntity = outerScope->get_entity_in(inScope);
        if (anEntity)
        {
            FogNestedScopeContext nestedContext(inScope, *anEntity);
            Super::find_entities_in(nestedContext, theFinding);
        }
    }
}

//  
//    Locate the name or type named anId in this scope, or its base classes, or its enclosing scopes.
//    Generates an error message and returns the first option if ambiguous.
//  
//    inScope determines whether the search is restricted to this scope, its bases or all visible scopes.
//  
FogEntity *FogScope::find_entity(const PrimId& anId, InScope inScope, FindStrategy aStrategy)
{
    FogEntityFinding theFinding(aStrategy);
    FogEntityFinder theFinder(theFinding, anId, inScope);
    find_entities(theFinder);
    return theFinding.get_unambiguous_finding(anId, *this);
}

//  
//    Return the unique type or entity corresponding to anId, 0 if nothing.
//    Other ambiguities accumulate in foundMap.
//  
bool FogScope::find_local_entity(FogEntityFinder& theFinder)
{
    if (&name_scope() != this)
        ERRMSG("BUG - should not find_local_entity for non-name-scope " << viz(*this));
    switch (theFinder.strategy())
    {
        case FIND_ENTITY:
        {
            bool gotName = find_local_name(theFinder);
            bool gotType = find_local_type(theFinder);
            return gotType || gotName;
        }
        case FIND_NAME:
            return find_local_name(theFinder);
        case FIND_SIGNATURE:
        {
            FogFunction *aFunction = _functions.find_local_function(theFinder.id());
            if (aFunction)
                theFinder.add_find(*aFunction);
            return aFunction != 0;
        }
        case FIND_SCOPE:
        case FIND_TYPE:
            return find_local_type(theFinder);
    }
    ERRMSG("BUG - should not FogScope::find_local_entity for unknown find strategy.");
    return true;        //   True suppresses a multiplicity of messages.
}

//  
//    Accumulate the names that satisfy theFinder.
//  
bool FogScope::find_local_name(FogEntityFinder& theFinder)
{
    if (&name_scope() != this)
        ERRMSG("BUG - should not find_local_name for non-name-scope " << viz(*this));
    if (_names.find_local_name(theFinder))
        return true;
    if (!is_instantiation())
        return false;
    FogEntityFinding theFinding(FogEntityFinder::FIND_NAME);
    FogEntityFinder primaryFinder(theFinding, theFinder.id(), IN_THIS_SCOPE);
    if (primary_instantiation()._names.find_local_name(primaryFinder))
    {                                           //  .bugbug loop over all overloads
        FogEntity *primaryEntity = primaryFinder.found();
        if (primaryEntity)
        {
            FogStaticScopeContext inScope(*this);
            primaryEntity->instantiate(inScope, FogEntity::FORCED);
        }
    }
    return _names.find_local_name(theFinder) != 0;
}

const FogMetaSlot *FogScope::find_local_slot(const PrimId& anId) const { return Super::find_local_slot(anId); }

//  
//    Accumulate the slot that satisfy theFinder.
//  
bool FogScope::find_local_slot(FogMetaSlotFinder& theFinder)
{
    const FogMetaSlot *metaSlot = find_local_slot(theFinder.id());
    if (!metaSlot)
        return false;
    theFinder.add(*this, *metaSlot);
    return true;
}

//  
//    Locate a directly visible primary type from this scope, but not its enclosing scopes.
//  
bool FogScope::find_local_type(FogEntityFinder& theFinder)
{
    if (&name_scope() != this)
        ERRMSG("BUG - should not find_local_type for non-name-scope " << viz(*this));
    if (has_distinct_primary() && !is_specialisation())
        return primary().find_local_type(theFinder);
    const PrimId& anId = theFinder.id();
    FogEntity *primaryType = (theFinder.strategy() == FIND_TYPE)
        ? _types.find_local_type(anId, theFinder.is_resolve_alias())
        : _types.find_local_scope(anId, theFinder.is_resolve_alias());
    if (!primaryType)
        primaryType = _types.find_local_other_type(anId);
    if (!primaryType)
        return false;
//    const PrimId& templateArguments = theFinder.template_arguments().id();
//    if (!primaryType->is_templated())
//    {
//        if (templateArguments)
//            ERRMSG("Template arguments " << templateArguments << " ignored for non-template "
//                    << viz(*primaryType));
        theFinder.add_find(*primaryType);
        return true;
//    }
#if 0
    const FogTemplateParameterSpecifiers *templateParameters = theFinder.template_parameters();
    if (!templateArguments && !templateParameters)
    {
        theFinder.add_find(*primaryType);
        return true;
    }
    if (theFinder.is_declaration() && !templateParameters && !Fog::no_specialisation_prefix())
    {
        WRNMSG("Missing \"template <>\" for \"" << templateArguments
            << "\" specialisation of " << viz(*primaryType));
//        theFinder.add_template_parameter(FogTemplateParameterSpecifiers::empty());
    }
    FogEntity *templatedType = primaryType->find_template(theFinder);
    if (!templatedType)
        return false;
    theFinder.add_find(*templatedType);
    return true;
#endif
}

//  
//    Locate the name or type satisfying theFinder in this scope and its base classes, but not its enclosing scopes.
//  
bool FogScope::find_scope_entity(FogEntityFinder& theFinder)
{
    if (!theFinder.add_visit(*this))
        return false;
    return _bases.find_entity(theFinder);
}

//  
//    Locate the slot satisfying theFinder in this scope and its base classes, but not its enclosing scopes.
//  
bool FogScope::find_scope_slot(FogMetaSlotFinder& theFinder)
{
    if (!theFinder.add_visit(*this))
        return false;
    return _bases.find_slot(*this, theFinder);
}

//  
//    Locate the slots satisfying and updating theFinder.
//  
bool FogScope::find_slots(FogMetaSlotFinder& theFinder)
{
    switch (theFinder.in_scope())
    {
        case IN_THIS_SCOPE:
            return find_local_slot(theFinder);
        case IN_BASE_SCOPE:
            return find_visible_slot(theFinder);
        case IN_BASE_NOT_THIS_SCOPE:
            return find_scope_slot(theFinder);
        case IN_ANY_SCOPE:
        {
            for (FogScope *aScope = this; aScope; aScope = aScope->outer_scope())
                if (aScope->is_name_scope() && aScope->find_visible_slot(theFinder))
                    return true;
            break;
        }
        case IN_ANY_NOT_THIS_SCOPE:
        {
            if (find_scope_slot(theFinder))
                return true;
            for (FogScope *aScope = outer_scope(); aScope; aScope = aScope->outer_scope())
                if (aScope->is_name_scope() && aScope->find_visible_slot(theFinder))
                    return true;
            break;
        }
    }
    return false;
}

//  
//    Return the <templateArguments> specialisation of this templated scope.
//  
FogEntity *FogScope::find_template(FogMakeTemplateContext& makeTemplateContext)
{
    if (has_distinct_primary())
        return primary().find_template(makeTemplateContext);
//    if (&makeContext.scope() != outer_scope())
//        ERRMSG("BUG - bad scope for FogScope::find_template for " << viz(makeContext));
//    if (!makerContext.template_arguments())         //.bugbug validate that new parameters match old spelling
//        return this;                    //  resolved forward declaration of primary.
    FogEntity *foundEntity = _templates.find_template(makeTemplateContext);
    if (foundEntity)
    {
        FogScope *foundScope = foundEntity->is_scope();
        if (foundScope)
            foundScope->_templates.check_classification(makeTemplateContext);
        return foundEntity;
    }
    foundEntity = _templates.make_template(makeTemplateContext);
    if (!foundEntity)
        return 0;
    FogScope *foundScope = foundEntity->is_scope();
    if (!foundScope)
        return 0;
    if (done_meta_construct() && !foundScope->done_meta_construct())            //   Tacky
         foundScope->do_meta_construct(makeTemplateContext);
    return foundEntity;
}

//  
//    Return true if this scope supports the use of access specifiers.
//  
bool FogScope::has_access() const { return false; }

FogTargetFile *FogScope::implementation_file()
{
    if (is_auto())
        ;
    else if (raw_implementation_file())
        ;
    else if (utility().is_not_emitted() && _braced && !is_name_space()) //   Avoid impls of utilities unless top-level
        ;                                                   //  .bugbug need to resolve extern
    else if (has_distinct_primary())
        set_implementation_file(primary().implementation_file());
    else if (outer_scope() && !outer_scope()->is_global_scope())
        set_implementation_file(outer_scope()->implementation_file());
    else
    {
        FogFileIdent *fileIdent = new FogFileIdent(file_id().str(), _templates.template_arguments().id());
        if (fileIdent)
        {
            fileIdent->set_implementation();
            if (is_primary())
                fileIdent->set_template();
            set_implementation_file(fileIdent->make_target_file(global_scope().file_manager()));
            fileIdent->annul();
        }
    }
    FogTargetFile *aFile = raw_implementation_file();
    return aFile && !aFile->is_null() ? aFile : 0;
}

FogUsage& FogScope::inline_usage()
{
    if (!_inline_usage)
    {
        _inline_usage = usage_manager().make_inline_usage(*this);
        if (!_inline_usage)
            _inline_usage = &FogUsage::mutable_null();
    }
    return *_inline_usage;
}

FogScope& FogScope::inner_scope() { return *this; }

//  
//    Install special types.
//  
void FogScope::install_types()
{
    name_scope()._types.add_scope(*this);
    _templates.install(*this);
}

void FogScope::instantiate_into(FogEntity& instantiatingEntity)
{
    FogScope *instantiatingScope = instantiatingEntity.is_scope();
    const FogTemplateParameterSpecifiers *templateParameters =
        instantiatingScope ? &instantiatingScope->template_parameters() : 0;
    if (templateParameters && templateParameters->is_using_template())
    {
        _functions.instantiate_into(*this, *instantiatingScope);
    }
}

FogTargetFile *FogScope::interface_file_sink()
{
    return is_instantiation() ? 0 : interface_file_source();
}

FogTargetFile *FogScope::interface_file_source()
{
    if (is_auto())
        ;
    else if (raw_interface_file())
        ;
    else if (_interface.no_interface())
        ;
    else if (has_distinct_primary())
        set_interface_file(primary().interface_file_source());
    else if (outer_scope() && !outer_scope()->is_global_scope())
        set_interface_file(outer_scope()->interface_file_source());
    else
    {
        FogFileIdent *fileIdent = new FogFileIdent(file_id().str(), _templates.template_arguments().id());
        if (fileIdent)
        {
            fileIdent->set_interface();
            if (is_primary())
                fileIdent->set_template();
            set_interface_file(fileIdent->make_target_file(global_scope().file_manager()));
            fileIdent->annul();
        }
    }
    FogTargetFile *aFile = raw_interface_file();
    return aFile && !aFile->is_null() ? aFile : 0;
}

FogUsage& FogScope::interface_usage_finish()
{
    FogUsage *existingInterfaceUsageFinish = raw_interface_usage_finish();
    if (existingInterfaceUsageFinish)
        return *existingInterfaceUsageFinish;
    FogUsage& interfaceFinish = Super::interface_usage_finish();
    if (_braced)
    {
        interfaceFinish.add_use_by(inline_usage());
    }
    return interfaceFinish;
}

FogUsage& FogScope::interface_usage_start()
{
    FogUsage *existingInterfaceUsageStart = raw_interface_usage_start();
    if (existingInterfaceUsageStart)
        return *existingInterfaceUsageStart;
    if (is_auto())
        ERRMSG("INVESTIGATE -- creating usage node for " << viz(*this));
    FogUsage& interfaceStart = Super::interface_usage_start();
    if (_braced)
    {
        FogUsage& nameUsage = name_usage();
        FogUsage& interfaceFinish = interface_usage_finish();
        if (is_specialisation())
        {
            primary().interface_usage_finish().add_use_by(interfaceStart);
        }
        if (is_instantiation())
        {
        }
        nameUsage.add_use_by(interfaceStart);
        interfaceStart.add_use_by(interfaceFinish);
        interfaceFinish.add_use_by(inline_usage());
        if (Fog::unnest())
            non_virtual_scope().interface_usage_finish().add_use_by(interfaceStart);
        else
        {
            non_virtual_scope().interface_usage_start().add_use_by(interfaceStart);
            interfaceFinish.add_use_by(non_virtual_scope().interface_usage_finish());
        }
    }
    return interfaceStart;
}

bool FogScope::is_active() const { return true; }
bool FogScope::is_auto() const { return tag().is_meta_tag(); }
bool FogScope::is_instantiation() const { return _templates.is_instantiation(); }
FogScope *FogScope::is_scope() { return this; }
bool FogScope::is_templated() const { return _templates.is_templated(); }
FogEntity *FogScope::make_enum(FogMakerContext& makerContext) { return _enums.make_enum(makerContext); }
FogEntity *FogScope::make_function_entity(FogMakerContext& makerContext)
    { return _functions.make_function(makerContext); }
FogUsage *FogScope::make_implementation_usage() { return usage_manager().make_implementation_usage(*this); }

FogScope *FogScope::make_linkage(const FogLinkageSpecifier& rawLinkage)
{
    ERRMSG("Should not attempt to make_linkage in " << viz(*this));
    return 0;
}

FogEntity *FogScope::make_linkage_entity(FogMakerContext& makerContext)
{
    const FogLinkageSpecifier *linkageSpecifier = makerContext.specifier().is_linkage_specifier();
    if (linkageSpecifier)
        return make_linkage(*linkageSpecifier);
    ERRMSG("BUG - should not make_linkage_entity from " << viz(makerContext));
    return 0;
}

FogEntity *FogScope::make_meta_slot(FogMakerContext& makerContext)
{
    const FogMetaObjectSpecifier *theSpecifier = makerContext.specifier().is_meta_object_specifier();
    if (theSpecifier)
    {
        theSpecifier->make_slot(makerContext);
        return &FogEntity::mutable_null();                  //   Ugh! satisfy caller's null/not-null test.
    }
    else
    {
//        ERRMSG("BUG - should not make_meta_slot from " << viz(makerContext.declaration()));
        ERRMSG("BUG - should not make_meta_slot from " << viz(makerContext));
        return 0;
    }
}

//  FogEntity *FogScope::make_null(FogMakeContext& makeContext)
//  {
//    ERRMSG("BUG - should not make_null from " << viz(makeContext));
//    return 0;
//  }

//  
//    makeContext.declaration() is a FogRawDerivable for a forward declaration.
//    makeContext.declaration() is a FogRawScope for a full declaration.
//  
FogEntity *FogScope::make_scope(FogMakerContext& makerContext)
{
    const FogName *typeId = makerContext.specifier().get_type();
    if (!typeId)
        return 0;
    const FogTag& aTag = typeId->tag();
//      FogEntity *aType = makerContext.find_type();
    FogEntity *aType = makerContext.find_scope();
    if (aType)
    {
        FogScope *aScope = aType->is_scope();
        const void *const okTag = aTag;                     //   DevStudio V5 workaround.
        if (aScope && okTag)
        {
            if (aScope->tag().needs_merge_tag(aTag).is_incompatible())
                ERRMSG("Inconsistent tag from " << viz(makerContext)
                        << " ignored for " << viz(*aScope));
            else
                aScope->set_tag(aTag);
        }
        if (makerContext.utility() < aType->utility())
            ERRMSG("Should not provide " << makerContext.utility().str() << " extension to "
                << aType->utility().str() << " " << viz(*aType));
        else
        {
            aType->set_max_utility(makerContext.utility());
//              if (aType->utility().is_hash())
//                  aType->set_interface_file(makerContext.target_file());
        }
    }
    else
    {
        const PrimId& anId = makerContext.short_id();
    //    const FogTemplateArgs& templateArguments = makerContext.template_arguments();
    //    if (templateArguments)
    //    {
    //        ERRMSG("Template specialisation of " << viz(*this) << " :: " << anId << " used before declaration.");
    //        return 0;
    //    }
        switch (aTag.value())
        {
            case FogTag::ENUM:
                ERRMSG("BUG - should not FogScope::make_scope for " << viz(makerContext));
    //            return impl().make_enum(anId);
    //        case FogTag::FILESPACE:
    //            aScope = new FogFilespace(*this, anId);
    //            break;
    //        case FogTag::CLASS:
    //        case FogTag::META:
    //        case FogTag::NAMESPACE:
    //        case FogTag::STRUCT:
    //        case FogTag::UNION:
            default:
                FogClass *aClass = new FogClass(aTag, makerContext, typeId->is_anon());
                if (aClass)
                {
                    if (add_scope(*aClass))
                    {
                        aClass->annul();
                        return aClass;
                    }
                    aClass->annul();
                }
                break;
        }
        if (!aType)
        {
            switch (makerContext.in_scope())
            {
                case IN_THIS_SCOPE:
                    ERRMSG("No definition of type \"" << anId << "\" in " << viz(*this));
                    return make_typename(makerContext);
                case IN_ANY_SCOPE:
                case IN_ANY_NOT_THIS_SCOPE:
                case IN_BASE_SCOPE:
                case IN_BASE_NOT_THIS_SCOPE:
                    ERRMSG("No definition of type \"" << anId << "\" visible in " << viz(*this));
                    return name_space().make_typename(makerContext);
            }
        }
    }
//    if (templateArguments)
//        return aType->find_template(makerContext);
//    else
        return aType;
};

//  FogEntity *FogScope::make_template_parameter_entity(FogMakerContext& makerContext)
//  {
//      FogTemplateParameter *templateParameter = new FogTemplateParameter(makerContext);
//      return makerContext.dynamic_token().adopt_template_parameter(templateParameter);
//  }

FogEntity *FogScope::make_type(FogMakerContext& makerContext) { return makerContext.find_type(); }
FogEntity *FogScope::make_typedef_entity(FogMakerContext& makerContext) { return make_typedef(makerContext); }
FogEntity *FogScope::make_typename_entity(FogMakerContext& makerContext) { return make_typename(makerContext); }
FogEntity *FogScope::make_using_entity(FogMakerContext& makerContext) { return _usings.make_using(makerContext); }
FogEntity *FogScope::make_variable_entity(FogMakerContext& makerContext) { return make_variable(makerContext); }

void FogScope::meta_execute(FogScopeContext& inScope, const PrimId& anId)
{
    const FogMetaSlot *metaSlot = find_local_slot(anId);
    if (!metaSlot)
        return;
    const FogMetaFunctionSlot *metaFunction = metaSlot->is_meta_function_slot();
    if (!metaFunction)
        return;
    FogTokenRef metaEntity(new FogMetaFunctionEntity(inScope.dynamic_scope(), *metaFunction), FogTokenRef::ADOPT);
//      FogTokenRef metaEntity;
//      if (!metaFunction->resolve_object(metaEntity, inScope))
//          return;
    FogTokenRef metaValue;
    FogStaticParseContext parseContext(inScope.dynamic_token(), FogRaw::immutable_null(),
        FogAccess::invalid_access(), FogUtility::emit_utility());
//    if (metaFunction && metaFunction->derivation().derive_at(*this, &inScope != this))
    static const FogToken& nullArgs = FogTokens::immutable_null();
    if (metaEntity->get_dyadic(metaValue, parseContext, FogTokenType::character('('), nullArgs))
        metaValue->compile_declaration(parseContext);
}

const FogMetaType& FogScope::meta_type() const { return tag().entity_meta_type(); }
FogTargetFile *FogScope::name_file() { return is_auto() ? 0 : Super::name_file(); }

FogUsage& FogScope::name_usage()
{
    FogUsage *existingNameUsage = raw_name_usage();
    if (existingNameUsage)
        return *existingNameUsage;
    FogUsage& nameUsage = Super::name_usage();
    if (_braced)
    {
        if (!is_primary() && !Fog::unnest())
            nameUsage.set_weak();               //   Name can be omitted if definition follows immediately
        if (is_specialisation())
            primary().name_usage().add_use_by(nameUsage);
        if (is_instantiation())
            primary().name_usage().add_use_by(nameUsage);
    }
    else
    {
        non_virtual_scope().interface_usage_start().add_use_by(nameUsage);
        nameUsage.add_use_by(non_virtual_scope().interface_usage_finish());
    }
    return nameUsage;
}

bool FogScope::needs_emission() const
{
    return _bases.needs_emission()
        || _enums.needs_emission()
        || _friends.needs_emission()
        || _functions.needs_emission()
        || _nested.needs_emission()
//        || !_templates.is_empty()       -- no, instantiations do not make the original not empty.
//                                        .bugbug but specialisations do.
        || _typedefs.needs_emission()
        || _usings.needs_emission()
        || _variables.needs_emission();
}

//  
//    Contribute to a search for the parent scope, by returning the scope that is a parent
//    of a child of this scope. Returns this, except for a filespace which returns its parent's parentscope.
//  
FogScope& FogScope::parent_scope() { return *this; }

std::ostream& FogScope::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _bases.print_members(s, aDepth);
    _names.print_depth(s, aDepth);
    _types.print_depth(s, aDepth);
    s << indent(aDepth) << "access = " << access() << '\n';
    _templates.print_depth(s, aDepth);
    _friends.print_members(s, aDepth);
    _usings.print_members(s, aDepth);
    _typedefs.print_depth(s, aDepth);
    _enums.print_depth(s, aDepth);
    _nested.print_depth(s, aDepth);
    _functions.print_depth(s, aDepth);
    _variables.print_depth(s, aDepth);
 if (raw_interface_file())
  s << indent(aDepth) << "interface file = " << *raw_interface_file() << "\n";
    _interface.print_depth(s, aDepth);
 if (raw_implementation_file())
  s << indent(aDepth) << "implementation file = " << *raw_implementation_file() << "\n";
    _implementation.print_depth(s, aDepth);
    _derivations.print_depth(s, aDepth);
    return s;
}

std::ostream& FogScope::print_long_id(std::ostream& s) const
{
    return s << global_signature_id();
}

std::ostream& FogScope::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _bases.print_members(s, aDepth);
    _names.print_members(s, aDepth);
    _types.print_members(s, aDepth);
    s << indent(aDepth) << "access = " << access()<< '\n';
    _templates.print_members(s, aDepth);
    _friends.print_members(s, aDepth);
    _usings.print_members(s, aDepth);
    _typedefs.print_members(s, aDepth);
    _enums.print_members(s, aDepth);
    _nested.print_members(s, aDepth);
    _functions.print_members(s, aDepth);
    _variables.print_members(s, aDepth);
 if (raw_interface_file())
  s << indent(aDepth) << "interface file = " << *raw_interface_file() << "\n";
    _interface.print_members(s, aDepth);
 if (raw_implementation_file())
  s << indent(aDepth) << "implementation file = " << *raw_implementation_file() << "\n";
    _implementation.print_members(s, aDepth);
    _derivations.print_members(s, aDepth);
    return s;
}

std::ostream& FogScope::print_this(std::ostream& s) const
{
    const FogTemplateParameterSpecifiers& templateParameters = template_parameters();
    if (!templateParameters.is_null())
    {
        templateParameters.print_named(s, 0, 0);
        s << ' ';
    }
    return s << tag().str() << ' ' << unique_id();
}

std::ostream& FogScope::print_viz(std::ostream& s) const
{
    return s << '\"' << tag().str() << ": " << unique_id() << '\"';
}

void FogScope::set_access(const FogAccess& anAccess) { _access = access().merge_access(anAccess); }

void FogScope::set_braced() {_braced = true;}

void FogScope::set_braced(FogMakeEntityContext& makeEntityContext)
{
    _braced = true;
     if (utility().is_hash())
        set_interface_file(makeEntityContext.target_file());
}

void FogScope::set_default_access(const FogAccess& anAccess) { if (!access()) _access = anAccess; }

void FogScope::set_implementation_file(FogTargetFile *aFile)
{
    if (raw_implementation_file() == aFile)
        return;
    if (raw_implementation_file())
        ERRMSG("Should not change implementation file for " << viz(*this)
            << " from " << viz(*raw_implementation_file()));
    Super::set_implementation_file(aFile);
}

void FogScope::set_interface_file(FogTargetFile *aFile)
{
    if (raw_interface_file() == aFile)
        return;
    if (raw_interface_file())
        ERRMSG("Should not change interface file for " << viz(*this) << " from " << viz(*raw_interface_file()));
    Super::set_interface_file(aFile);
}

void FogScope::set_not_leaf() { _is_leaf = FogLazyBool::make_false(); }
FogTemplateManager& FogScope::template_manager() { return _templates; }
const FogTemplateParameterSpecifiers& FogScope::template_parameters() const
    { return _templates.parameter_specifiers(); }
FogTokenType::TokenType FogScope::token_type_enum() const { return tag().entity_meta_type().is_token(); }

bool FogScope::validate() const
{
    FogProgressMonitor aMonitor("Validating", *this);
    if (!Super::validate())
        return false;
    _enums.validate();
    _functions.validate();
    _nested.validate();
    _templates.validate();
    _typedefs.validate();
    _variables.validate();
    return true;
}

//  
//    Validate the consistency of the is_utility attribute between this scope and its member anEntity.
//  
bool FogScope::validate_utility(const FogEntity& anEntity) const
{
    if (is_global_scope() || (anEntity.utility().is_utility() == utility().is_utility()))   //  .bugbug pool ?
        return true;
    if (utility().is_utility())
        ERRMSG("Utility " << viz(*this) << " extended by non-utility " << viz(anEntity));
    else
        ERRMSG("Non-utility " << viz(*this) << " contains utility " << viz(anEntity));
    return false;
}


void FogScope::set_no_implementation() {
	_implementation.set_no_implementation();
	for(FogFunctionListOfRefIterator p(_functions.distinct_function_list()); p; ++p) {
		p->set_explicit_no_implementation();
	}
}

std::ostream& FogScopeNull::print_viz(std::ostream& s) const
{
    return s << "\"null-scope\"";
}
