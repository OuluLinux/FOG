#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogElaboratedTypeSpecifier, Super)
FOGTOKEN_READ_ONLY_IMPL(FogElaboratedTypeSpecifier)

FogElaboratedTypeSpecifier::FogElaboratedTypeSpecifier(const FogTag& aTag, FogName& aName)
:
    Super(aTag, aName)
{}

FogElaboratedTypeSpecifier::FogElaboratedTypeSpecifier(const This &thatSpecifier)
:
    Super(thatSpecifier)
{}

FogElaboratedTypeSpecifier::~FogElaboratedTypeSpecifier() {}

FogEntity *FogElaboratedTypeSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const
{
    FogEntity *theEntity = 0;
    if (makeEntityContext.decl_specifiers().is_friend())
    {
        FogScope& targetScope = makeEntityContext.dynamic_scope();
        FogInScopeMakeEntityContext inScopeContext(makeEntityContext, FogInScopeMakeEntityContext::IN_THIS_SCOPE);
        FogNestedMakeEntityContext nestedContext(inScopeContext, makeEntityContext.dynamic_scope().name_space());
        theEntity = name().find_type_in(nestedContext);
        if (!theEntity)
        {
            FogEntityMakerContext makerContext(nestedContext, *this, &FogScope::make_scope);
            theEntity = name().make_entity(makerContext);
            if (theEntity)
                targetScope.add_friend(*theEntity);
        }
    }
    else
    {
        theEntity = name().find_type_in(makeEntityContext);
        if (!theEntity)
        {
            FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_scope);
            theEntity = name().make_entity(makerContext);
            if (theEntity)
                theEntity->set_access(access());
        }
    }
    return theEntity;
}

const FogMetaType& FogElaboratedTypeSpecifier::meta_type() const { return FogMetaType::elaborated_type_specifier_type(); }

std::ostream& FogElaboratedTypeSpecifier::print_viz(std::ostream& s) const
{
    s << "\"elaborated-type-specifier: " << tag().str() << ' ';
    name().print_this(s);
    return s << '\"';
}

bool FogElaboratedTypeSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext nameSemantics(theSemantics);
    if (!name().resolve_semantics(nameSemantics))
        return false;
    theSemantics.reset();
    if (nameSemantics.is_epsilon())
        theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
    if (nameSemantics.is_scoped_identifier() || nameSemantics.is_scoped_class_name())
        theSemantics.set(FogSemantics::IS_ELABORATED_TYPE_SPECIFIER);
    if (tag().is_typename_tag() && nameSemantics.is_identifier_opt())
            theSemantics.set(FogSemantics::IS_TYPENAME_TYPE_PARAMETER);
    if (tag().is_class_tag() && nameSemantics.is_identifier_opt())
        theSemantics.set(FogSemantics::IS_CLASS_TYPE_PARAMETER);
    if (tag().meta_type_name() && nameSemantics.is_meta_parameter_name())
        theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATION_ELEM);
    return true;
}

void FogElaboratedTypeSpecifier::set_new_utility(const FogUtility& aUtility) {}
