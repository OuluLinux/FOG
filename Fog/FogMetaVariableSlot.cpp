#include <Fog/FogIncludeAll.h>

#include <map>













TYPEINFO_SINGLE(FogMetaVariableSlot, Super)

FogMetaVariableSlot::FogMetaVariableSlot() {}

//  
//    Constructor for built-in meta-variables.
//  
FogMetaVariableSlot::FogMetaVariableSlot(FogToken& staticMetaScope,
    const FogMetaType& metaType, IsExposed isExposed, const char *anId, const FogToken *aToken)
:
    Super(staticMetaScope, IS_STATIC, metaType, isExposed, anId),
    _value(aToken ? *aToken : FogToken::immutable_null())
{
//    FogNestedMakeEntityContext nestedMakeEntityContext(makerContext, *this);
//    _value->make_actual(_value, makerContext);          -- not necessary since built-ins are actual
}

//  
//    Constructor for meta-arguments.
//  
FogMetaVariableSlot::FogMetaVariableSlot(FogToken& staticMetaScope,
    const FogMetaType& metaType, IsExposed isExposed, const PrimId& anId, const FogToken *aToken)
:
    Super(staticMetaScope, IS_STATIC, metaType, isExposed, anId),
    _value(aToken ? *aToken : FogToken::immutable_null())
{
//    FogNestedMakeEntityContext nestedMakeEntityContext(makerContext, *this);
//    _value->make_actual(_value, makerContext);          -- .bugbug resolve defaults when and in which context ?
}

//  
//    Constructor for user meta-variables.
//  
FogMetaVariableSlot::FogMetaVariableSlot(FogMakerContext& makerContext,
    const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed, const FogToken& aToken)
:
    Super(makerContext, aStatic, metaType, isExposed),
    _value(aToken)
{
    FogResolutionScopeContext weakContext(makerContext, FogResolutionScopeContext::RESOLVE_DOLLARS);
    _value->make_actual(_value, weakContext);
}

FogMetaVariableSlot::~FogMetaVariableSlot() {}

bool FogMetaVariableSlot::emit(FogEmitContext& emitContext) const
{
    FogTokenRef returnValue;
    if (resolve_object(returnValue, emitContext))
        return returnValue->emit(emitContext);
    return false;
}

//std::map<const FogMetaVariableSlot*, FogMetaEntity*> found_meta_entities;

FogMetaEntity *FogMetaVariableSlot::get_meta_entity(FogScopeContext& inScope) const
{
    FogToken *inToken = NULL;
    
    VERBOSE(printf("DEBUG FogMetaVariableSlot::get_meta_entity %x inScope %x\n", this, &inScope);)
    
    FogMetaEntity *metaEntity = NULL;//found_meta_entities[this];
    
    if (!metaEntity) {
	inToken = get_scope_in(inScope);
		
	VERBOSE(printf("DEBUG   inToken %x\n", &inToken);)
	    
	if (!inToken)
	    return 0;
	    
	VERBOSE(printf("DEBUG   find entity %s\n", id().str());)
		
	metaEntity = inToken->find_meta_entity(id());
		
	VERBOSE(if (metaEntity) {std::strstream str;metaEntity->print_viz(str);printf("DEBUG   found %x %s\n", metaEntity, str.str());})
    }
    else {
	VERBOSE(printf("DEBUG   old entity %x\n", metaEntity);)
    }
    
    
    if (!metaEntity)
    {
        metaEntity = inToken->adopt_meta_entity(new_meta_entity(*inToken));
        if (!metaEntity)
            ERRMSG("Failed to get_meta_entity from " << viz(*this));
        else {
            metaEntity->set_value(token(), inScope);            //   Generates error internally
        }
    }
    
    //found_meta_entities[this] = metaEntity;
    return metaEntity;
}

bool FogMetaVariableSlot::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    /*FogMetaEntity *metaEntity = get_meta_entity(inScope);
	if (!metaEntity)
	    return false;
    tokenValue.assign(*metaEntity);
    return true;*/

    return resolve_object(tokenValue, inScope);
}

const FogMetaVariableSlot *FogMetaVariableSlot::is_meta_variable_slot() const { return this; }

//  
//    Create the storage for this meta object. The default implementation returns 0 to suit
//    FogMetaObjectNull and FogMetaArgument that have no associated storage.
//  
FogMetaEntity *FogMetaVariableSlot::new_meta_entity(FogToken& inToken) const { return 0; }

std::ostream& FogMetaVariableSlot::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_value)
        s << indent(aDepth) << *_value << '\n';
    return s;
}

std::ostream& FogMetaVariableSlot::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_value)
        s << indent(aDepth) << *_value << '\n';
    return s;
}

std::ostream& FogMetaVariableSlot::print_viz(std::ostream& s) const
{
    s << "\"meta-variable: ";
    print_long_id(s);
    return s << '\"';
}

bool FogMetaVariableSlot::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const
{
    FogMetaEntity *metaEntity = get_meta_entity(getScope);
    return metaEntity ? metaEntity->set_value(tokenValue, getScope) : false;
}

//  
//    The default implementatiojn here is appropriate for member-meta-variables such as MetaIterator and MetaVariable.
//    Built-in meta-variables should re-implement.
//  
bool FogMetaVariableSlot::resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    FogMetaEntity *metaEntity = get_meta_entity(inScope);
    if (!metaEntity)
    {
        returnValue = FogFailure::make();
        return false;
    }
    else
    {
        returnValue.assign(*metaEntity);
        return true;
    }
}
