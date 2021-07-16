#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogMetaFunctionSlot, Super)

//  
//    Constructor for user meta-functions
//  
FogMetaFunctionSlot::FogMetaFunctionSlot(FogMakerContext& makerContext,
    const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed)
:
    Super(makerContext, aStatic, metaType, isExposed)
{
    add_built_ins();
}

FogMetaFunctionSlot::FogMetaFunctionSlot(FogToken& staticMetaScope,
    IsStatic isStatic, const FogMetaType& metaType, IsExposed isExposed, const char *anId)
:
    Super(staticMetaScope, isStatic, metaType, isExposed, anId)
{
    add_built_ins();
}

FogMetaFunctionSlot::~FogMetaFunctionSlot() {}

void FogMetaFunctionSlot::add(const FogMetaArgument& metaArgument)
{
    _formal_map.add(metaArgument);
    _formal_list.add(metaArgument);
}

void FogMetaFunctionSlot::add(const FogMetaType& metaType, IsExposed isExposed, const PrimId& aFormal,
    const FogToken *aToken)
{
    bool alreadyExists = _formal_map.find(aFormal) != 0;
    if (alreadyExists)
        ERRMSG("Duplicate parameter \"" << aFormal << "\" ignored in " << viz(*this));
    else
    {
        const FogMetaArgument *metaArgument = new FogMetaArgument(
            _formal_list.tally(), metaType, isExposed, aFormal, aToken);
        if (metaArgument)
        {
            _formal_map.adopt(*metaArgument);
            _formal_list.add(*metaArgument);
        }
    }
}

void FogMetaFunctionSlot::add_built_ins()
{
    static const PrimIdHandle dynamicId("Dynamic");
    static const PrimIdHandle staticId("Static");
    _formal_map.adopt(new FogMetaArgument(
        FogMetaArgument::STATIC, FogMetaType::meta_function_type(), IS_ENCAPSULATED, *staticId, &static_meta_scope()));
    _formal_map.adopt(new FogMetaArgument(
        FogMetaArgument::DYNAMIC, FogMetaType::meta_function_type(), IS_ENCAPSULATED, *dynamicId, 0));
}

void FogMetaFunctionSlot::adopt(const FogMetaArgument& metaArgument)
{
    _formal_map.adopt(metaArgument);
    _formal_list.add(metaArgument);
}

const FogMetaArgument& FogMetaFunctionSlot::formal(size_t anIndex) const
{
    if (anIndex < _formal_list.tally())
        return *_formal_list[anIndex];
    ERRMSG("There is no formal parameter number " << (anIndex+1) << " for " << viz(*this));
    return FogMetaArgument::immutable_null();
}

bool FogMetaFunctionSlot::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    tokenValue.assign(inScope.dynamic_token());
    return true;
}

const FogMetaFunctionSlot *FogMetaFunctionSlot::is_meta_function_slot() const { return this; }
const FogMetaType& FogMetaFunctionSlot::meta_type() const { return FogMetaType::meta_function_type(); }

std::ostream& FogMetaFunctionSlot::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_formal_list.tally())
    {
        s << indent(aDepth) << "formals\n";
        _formal_list.print_depth(s, aDepth+1);
    }
    return s;
}

std::ostream& FogMetaFunctionSlot::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_formal_list.tally())
    {
        s << indent(aDepth) << "formals\n";
        _formal_list.print_members(s, aDepth+1);
    }
    return s;
}

std::ostream& FogMetaFunctionSlot::print_viz(std::ostream& s) const
{
    s << "\"meta-function: ";
    print_long_id(s);
    return s << '\"';
}

bool FogMetaFunctionSlot::resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    FogToken *inToken = get_scope_in(inScope);
    if (inToken)
    {
        FogMetaEntity *metaEntity = inToken->find_meta_entity(id());
        if (!metaEntity)
        {
            metaEntity = inToken->adopt_meta_entity(new FogMetaFunctionEntity(*inToken, *this));
            if (!metaEntity)
                ERRMSG("Failed to find meta_entity for " << viz(*this));
        }
        if (metaEntity)
        {
            returnValue.assign(*metaEntity);
            return true;
        }
    }
    returnValue = FogFailure::make();
    return false;
}
