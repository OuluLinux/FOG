#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogTypedefSpecifier, Super)
FOGTOKEN_LEAF_IMPL(FogTypedefSpecifier)

FogTypedefSpecifier::FogTypedefSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(makeSpecifierContext, nameName)
{}

FogTypedefSpecifier::~FogTypedefSpecifier() {}

bool FogTypedefSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!decl_specifiers().is_explicit_access())
        set_decl_specifier(makeSpecifierContext.access());
    FogAnonId *anonId = type().is_anon();
    if (anonId && !anonId->is_named())
    {
        PrimIdHandle theId;
        resolve_id(theId, makeSpecifierContext);
        anonId->set_id(*theId);
        return false;
    }
    //   In template <class Element> class X { typedef Element Element; }, the typedef gets normalised to
    //   typedef _1 _1; which must now be partially unnormalised back to typedef _1 Element;
    const FogTemplateParameterSpecifierId *parameterId = name().is_template_parameter_specifier_id();
    if (parameterId)
    {
        const FogTemplateParameterSpecifier *aSpecifier =
            makeSpecifierContext.find_template_parameter_specifier(*parameterId);
        if (aSpecifier)
        {
            FogNameRef aName;
            aSpecifier->unnormalised_name().make_actual(aName, makeSpecifierContext);
            set_name(*aName);
        }
    }
    return Super::compile_specifier(makeSpecifierContext);
}

bool FogTypedefSpecifier::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_hash_line(line());
    emitContext.emit_space_and_text("typedef");
    Super::emit(emitContext);
    emitContext.emit_space_and_text(";\n");
    return true;
}

FogTypedefSpecifier *FogTypedefSpecifier::is_typedef_specifier() { return this; }

FogEntity *FogTypedefSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
    if (makeEntityContext.decl_specifiers().is_using())
    {
        FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_using_entity);
        return make_entity(makerContext);
    }
    else
    {
        FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_typedef_entity);
        FogEntity *anEntity = make_entity(makerContext);
        FogTypedef *aTypedef = anEntity ? anEntity->is_typedef() : 0;
        if (aTypedef && makerContext.has_context())
            aTypedef->add(makerContext);
        return anEntity;
    }
}   

const FogMetaType& FogTypedefSpecifier::meta_type() const { return FogMetaType::typedef_specifier_type(); }

std::ostream& FogTypedefSpecifier::print_viz(std::ostream& s) const
{
    s << "\"typedef-specifier: ";
    print_this(s);
    return s << '\"';
}
