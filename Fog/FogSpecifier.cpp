#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogSpecifier, Super)
PRIMREF_NULL_CLAS(FogSpecifier)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogSpecifier)
PRIMREFS_IMPL(FogSpecifier)
TMPL_HACK_FIX_DO(FogSpecifier)

FogSpecifier::FogSpecifier() {}

FogSpecifier::FogSpecifier(FogName& aName)
:
    Super(aName),
    _utility(FogUtility::emit_utility())
{}

FogSpecifier::FogSpecifier(FogName& aName, const FogUtility& aUtility)
:
    Super(aName),
    _utility(FogUtility::emit_utility())
{
    if (aUtility > *_utility) set_new_utility(aUtility);    //   set_max_utility() without premature virtual call
}

FogSpecifier::FogSpecifier(const This &thatSpecifier)
:
    Super(thatSpecifier),
    _utility(thatSpecifier._utility),
//      _line(thatSpecifier._line),
    _decl_specifiers(thatSpecifier._decl_specifiers)
{}

FogSpecifier::~FogSpecifier() {}

const FogAccess& FogSpecifier::access() const { return FogAccess::flyweight(_decl_specifiers); }

FogMetaEntity *FogSpecifier::adopt_meta_entity(FogMetaEntity *metaEntity)
{
    _meta_entities.adopt(metaEntity);
    return metaEntity;
}

//  
//    Return true if this declaration will be concrete if its scope is. (This inquiry is made during
//    compilation of the scope, so of course the scope has not yet determined whether it is concrete).
//  
const FogLazyBool& FogSpecifier::can_be_concrete(FogScopeContext& inScope) const
{
    return FogLazyBool::make_true();
}

//  
//    Resolve internal consistency. A false return may indicate a failure, after generation of an error message
//    or just a detection of redundancy for a typedef naming a type.
//  
bool FogSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
//    _derivation.compile();
    if (!_decl_specifiers.is_friend())
    {
//          if (!makeSpecifierContext.dynamic_scope().is_global_scope()
//           && (makeSpecifierContext.utility() > utility()))
//              ERRMSG("INVESTIGATE - changing " << viz(*this) << " from " << utility().str()
//                  << " to " << makeSpecifierContext.utility().str());
        set_max_utility(makeSpecifierContext.utility());
    }
    set_line(makeSpecifierContext.line());  //   Could move this to construction time
//    if (!_decl_specifiers.is_using())
//    {
//        if (!_decl_specifiers.is_const())
//            _decl_specifiers |= FogDeclSpecifierValue::NOT_CONST_MASK;
//        if (!_decl_specifiers.is_volatile())
//            _decl_specifiers |= FogDeclSpecifierValue::NOT_VOLATILE_MASK;
//    }
    return true;
}

void FogSpecifier::compile_subspace(FogParseContext& parseContext, FogSpecifier& actualSpecifier) const
{
    ERRMSG("BUG -- should not invoke FogSpecifier::compile_subspace.");
}

const FogDeclSpecifierValue& FogSpecifier::decl_specifiers() const { return _decl_specifiers; }

#define FOGSPECIFIER_GET_SPECIFIER(T,N) \
T *FogSpecifier::name3(get_,N,_specifier)(const FogToken& inToken) \
{ \
    T *theSpecifier = name3(is_,N,_specifier)(); \
    if (!theSpecifier) \
        ERRMSG("BUG -- expected " #N "-specifier for " << viz(inToken)); \
    return theSpecifier; \
}

const FogLazyBool& FogSpecifier::derive_at(FogScopeContext& inScope) const
{
    return FogLazyBool::make_bool(inScope.at_root() && !is_null());
}

//  
//    Eliminate all references so that destruction is complete and static destruction well behaved.
//  
void FogSpecifier::destroy()
{
    Super::destroy();
    _meta_entities.vacate();
}

bool FogSpecifier::emit(FogEmitContext& emitContext) const { return name().emit(emitContext); }
FogEntity *FogSpecifier::find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const
    { return name().find_entity_in(inScope, aStrategy); }

FogMetaEntity *FogSpecifier::find_meta_entity(const PrimId& anId)
{
    FogMetaEntity *metaEntity = _meta_entities.find(anId);
    return metaEntity ? metaEntity : Super::find_meta_entity(anId);
}

FOGSPECIFIER_GET_SPECIFIER(FogFunctionSpecifier, function)
FOGSPECIFIER_GET_SPECIFIER(FogMetaFunctionSpecifier, meta_function)
FOGSPECIFIER_GET_SPECIFIER(FogMetaParameterSpecifier, meta_parameter)
FOGSPECIFIER_GET_SPECIFIER(FogObjectSpecifier, object)
FOGSPECIFIER_GET_SPECIFIER(FogParameterSpecifier, parameter)
FOGSPECIFIER_GET_SPECIFIER(FogTemplateParameterSpecifier, template_parameter)
FOGSPECIFIER_GET_SPECIFIER(FogTypedefSpecifier, typedef)
FOGSPECIFIER_GET_SPECIFIER(FogVariableSpecifier, variable)

#undef FOGSPECIFIER_GET_SPECIFIER

bool FogSpecifier::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const
{
    switch (dyadicOp)
    {
        case '.':
        {
            const FogName *rightName = rightValue.is_name();
            if (!rightName)
            {
                ERRMSG("Expected name as member name for " << viz(*this) << " in " << viz(inScope));
                returnValue = FogFailure::make();
                return false;
            }
            FogNestedScopeContext scopeContext(inScope, mutate());                      //  .bugbug cast
            return rightName->get_meta_entity_in(returnValue, scopeContext);
        }
        default:
            return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);
    }
}

//  
//    Strip components that do not belong to an instantiation.
//  
void FogSpecifier::instantiate()
{
    ERRMSG("BUG -- Should not FogSpecifier::instantiate() " << viz(*this));
}

FogAnonId *FogSpecifier::is_anon() { return name().is_anon(); }
bool FogSpecifier::is_explicit_instantiation() const { return decl_specifiers().is_template(); }
FogFunctionSpecifier *FogSpecifier::is_function_specifier() { return 0; }
FogLinkageSpecifier *FogSpecifier::is_linkage_specifier() { return 0; }
FogMetaFunctionSpecifier *FogSpecifier::is_meta_function_specifier() { return 0; }
FogMetaObjectSpecifier *FogSpecifier::is_meta_object_specifier() { return 0; }
FogMetaParameterSpecifier *FogSpecifier::is_meta_parameter_specifier() { return 0; }
FogMetaVariableSpecifier *FogSpecifier::is_meta_variable_specifier() { return 0; }
FogObjectSpecifier *FogSpecifier::is_object_specifier() { return 0; }
FogParameterSpecifier *FogSpecifier::is_parameter_specifier() { return 0; }
FogSpecifier *FogSpecifier::is_specifier() { return this; }
FogScopeSpecifier *FogSpecifier::is_scope_specifier() { return 0; }
FogTemplateParameterSpecifier *FogSpecifier::is_template_parameter_specifier() { return 0; }
FogTypedefSpecifier *FogSpecifier::is_typedef_specifier() { return 0; }
FogObjectSpecifier *FogSpecifier::is_using_specifier() { return 0; }
FogVariableSpecifier *FogSpecifier::is_variable_specifier() { return 0; }
//  FogLine& FogSpecifier::line() const { return *_line; }

FogEntity *FogSpecifier::make_entity(FogMakerContext& makerContext) const
{
    return name().make_entity(makerContext);
}

bool FogSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    return makeSpecifierContext.make_specifier(*this) != 0;
}

void FogSpecifier::merge_from(FogMergeContext& mergeContext, const This& aSpecifier)
{
    Super::merge_from(mergeContext, aSpecifier);
    aSpecifier._decl_specifiers.merge_into(mergeContext, _decl_specifiers);
}

const FogMetaType& FogSpecifier::meta_type() const { return FogMetaType::specifier_type(); }

const FogMerge& FogSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& aSpecifier) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, aSpecifier));
    needsMerge |= _decl_specifiers.needs_merge(mergeContext, aSpecifier._decl_specifiers);
    return needsMerge;
}

std::ostream& FogSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    s << indent(aDepth) << "line = " << line() << '\n';
    if (_meta_entities.tally())
    {
        s << indent(aDepth) << "meta_entities\n";
        _meta_entities.print_depth(s, aDepth+1);
    }
    return s;
}

std::ostream& FogSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    s << indent(aDepth) << "line = " << line() << '\n';
    if (_meta_entities.tally())
    {
        s << indent(aDepth) << "meta_entities\n";
        _meta_entities.print_members(s, aDepth+1);
    }
    return s;
}

bool FogSpecifier::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
    { return name().resolve_id(returnId, inScope); }

bool FogSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    theSemantics.assign(meta_type().is_semantics());
    return true;
}

void FogSpecifier::set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers)
{
    if (!is_null())
        _decl_specifiers |= declSpecifiers.decl_specifiers();
    aName = this;
}

void FogSpecifier::set_decl_specifier(const FogDeclSpecifier& declSpecifier)
    { if (!is_null()) _decl_specifiers |= declSpecifier.decl_specifier_value(); }
void FogSpecifier::set_decl_specifiers(const FogDeclSpecifierValue& declSpecifiers)
    { if (!is_null()) _decl_specifiers |= declSpecifiers; }

//  void FogSpecifier::set_line(FogLine& aLine)
//  {
//      if (!is_null())
//      {
//          _line = aLine;
//          Super::set_line(aLine);         //.bugbug lose Super::_line
//      }
//  }

void FogSpecifier::set_new_utility(const FogUtility& aUtility)
{
    if (is_null())
        ERRMSG("BUG - should not set_new_utility for " << viz(*this));
    else
    {
        if (Fog::debug_utility())
            DIAMSG("Utility of " << viz(*this) << " changing from " << utility().str() << " to " << aUtility.str());
        _utility = aUtility;
    }
}

const FogUtility& FogSpecifier::utility() const { return *_utility; }
