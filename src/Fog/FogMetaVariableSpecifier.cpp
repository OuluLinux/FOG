#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogMetaVariableSpecifier, Super)
FOGTOKEN_UNIQUE_IMPL(FogMetaVariableSpecifier)

FogMetaVariableSpecifier::FogMetaVariableSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(makeSpecifierContext, nameName)
{}

FogMetaVariableSpecifier::~FogMetaVariableSpecifier() {}
FogMetaVariableSpecifier *FogMetaVariableSpecifier::is_meta_variable_specifier() { return this; }

bool FogMetaVariableSpecifier::make_slot(FogMakerContext& makerContext) const
{
    const PrimId& anId = makerContext.short_id();
    if (!_initializer)
    {
        ERRMSG("Initial value required for meta-variable " << c_string(anId.str()) << '.');
        return 0;
    }
    FogToken& inToken = makerContext.dynamic_token();
    FogTokenRef metaEntityValue;
    bool foundIt = name().find_meta_entity_in(metaEntityValue, makerContext);
    const FogMetaType& metaType = resolve_type_meta_type(makerContext);
    FogMetaEntity *metaEntity = metaEntityValue->is_meta_entity();
    if (!metaEntity)
        ;
    else if (!metaEntity->is_meta_iterator_entity() && !metaEntity->is_meta_variable_entity())
        ERRMSG("Cannot create meta-variable when " << viz(*metaEntity) << " already defined.");
//      else if (&type() != &metaEntity->type())
    else if (&metaType != &metaEntity->type())
        ERRMSG("Cannot change type of " << viz(*metaEntity) << " to " << type());
    else
    {
        foundIt = false;                    //   Force recreation.
        FogMetaEntity *metaEntity = inToken.find_meta_entity(anId);
        if (metaEntity)
            metaEntity->set_value(*_initializer, makerContext);
//        else
//            ERRMSG("BUG -- expected meta-entity for " << viz(*returnValue) << " in FogMetaVariableSpecifier::make_slot");
    }
    if (foundIt)
        return true;
    const FogMetaVariableSlot *metaObject = 0;
    if (&type() == &FogMetaType::iterator_type())
        metaObject = new FogMetaIterator(makerContext, get_static(), metaType, is_exposed(), *_initializer);
    else
        metaObject = new FogMetaVariable(makerContext, get_static(), metaType, is_exposed(), *_initializer);
    return inToken.adopt_slot(metaObject) != 0;
}

const FogMetaType& FogMetaVariableSpecifier::meta_type() const { return FogMetaType::meta_variable_specifier_type(); }

std::ostream& FogMetaVariableSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _initializer->print_deep(s, aDepth);
    return s;
}

std::ostream& FogMetaVariableSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _initializer->print_on(s, aDepth);
    return s;
}

std::ostream& FogMetaVariableSpecifier::print_viz(std::ostream& s) const
{
    s << "\"meta-variable-specifier: ";
    print_this(s);
    return s << '\"';
}

void FogMetaVariableSpecifier::set_initializer(const FogExpr& anExpr) { _initializer = anExpr; }
