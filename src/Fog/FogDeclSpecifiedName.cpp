#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogDeclSpecifiedName, Super)
FOGTOKEN_LEAF_IMPL(FogDeclSpecifiedName)

FogDeclSpecifiedName::FogDeclSpecifiedName(FogName& aName)
:
    Super(aName)
{}

FogDeclSpecifiedName::FogDeclSpecifiedName(FogName& aName, const FogDeclSpecifierValue& declSpecifiers)
:
    Super(aName),
    _decl_specifiers(declSpecifiers)
{}

FogDeclSpecifiedName::~FogDeclSpecifiedName() {}
const FogDeclSpecifierValue& FogDeclSpecifiedName::decl_specifiers() const { return _decl_specifiers; }

bool FogDeclSpecifiedName::emit(FogEmitContext& emitContext) const
{
    bool doneSomething = false;
    if (emitContext.emit_expression())
        doneSomething |= _decl_specifiers.emit(emitContext);
    else
        doneSomething |= emitContext.emit_cv(_decl_specifiers); //   caller knows better for rest (e.g. static sometimes only in interface).
    doneSomething |= name().emit(emitContext);
    return doneSomething;
}

bool FogDeclSpecifiedName::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!name().make_specifier(makeSpecifierContext))       //   Occurs for virtual ~T();
        return false;
    makeSpecifierContext.specifier().set_decl_specifiers(decl_specifiers());
    return true;
}

bool FogDeclSpecifiedName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!name().make_type_specifier(makeSpecifierContext))
        return false;
    makeSpecifierContext.specifier().set_decl_specifiers(decl_specifiers());
    return true;
}

//  .bugbug merge

char FogDeclSpecifiedName::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = _decl_specifiers.print_named(s, tailChar);
    return Super::print_named(s, fullId, tailChar);
}

std::ostream& FogDeclSpecifiedName::print_viz(std::ostream& s) const
{
    s << "\"decl-specified-name: ";
    print_this(s);
    return s << '\"';
}

bool FogDeclSpecifiedName::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext typeSemantics(theSemantics);
    FogChildSemanticsContext nameSemantics(theSemantics);
    if (!_decl_specifiers.resolve_semantics(typeSemantics) || !name().resolve_semantics(nameSemantics))
        return false;
    if (nameSemantics.is_scoped_identifier() && !nameSemantics.is_scoped_type_name() && !nameSemantics.is_scoped_meta_name())
    {                                           //  .bugbug a bit wider than just ::id
        ERRMSG("Type name expected in " << viz(*this) << ".");
//        return false;
    }
    theSemantics.reset();
    if (typeSemantics.is_untyped_decl_specifier_seq())
    {
        if (nameSemantics.is_bit_field_declaration())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATION);
        if (nameSemantics.is_constructor_declarator())
            theSemantics.set(FogSemantics::IS_CONSTRUCTOR_DECLARATOR);
        if (nameSemantics.is_destructor_declarator())
            theSemantics.set(FogSemantics::IS_DESTRUCTOR_DECLARATOR);
        if (nameSemantics.is_function_declarative())
            theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATIVE);
        if (nameSemantics.is_init_declarator_list())
            theSemantics.set(FogSemantics::IS_INIT_DECLARATION_LIST);
        if (nameSemantics.is_name_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_NAME_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_object_declaration())
        {
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
            if (decl_specifiers().is_typedef())
                theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
        }
        if (nameSemantics.is_parameter_declaration())
            theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATION);
        if (nameSemantics.is_untyped_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_UNTYPED_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_using_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_USING_DECL_SPECIFIER_SEQ);
    }
    if (typeSemantics.is_typed_decl_specifier_seq())
    {
        if (nameSemantics.is_bit_field_declarator_id_opt())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
        if (nameSemantics.is_function_declarator())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (nameSemantics.is_init_declarator_list())
            theSemantics.set(FogSemantics::IS_INIT_DECLARATION_LIST);
        if (nameSemantics.is_object_declarator_id())
        {
            theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
            if (decl_specifiers().is_typedef())
                theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
        }
        if (nameSemantics.is_parameter_declarator_opt())
            theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
    }
    if (typeSemantics.is_using_decl_specifier_seq())
    {
        if (nameSemantics.is_bit_field_declarator_id_opt())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
        if (nameSemantics.is_constructor_declarator())
            theSemantics.set(FogSemantics::IS_CONSTRUCTOR_DECLARATOR);
        if (nameSemantics.is_function_declarator_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (nameSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
        if (nameSemantics.is_constructor_name() || nameSemantics.is_function_name())
            theSemantics.set(FogSemantics::IS_USING_DECLARATIVE);
        if (nameSemantics.is_name_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_USING_NAME_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_untyped_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_USING_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_typed_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_USING_NAME_DECL_SPECIFIER_SEQ);
    }
    if (typeSemantics.is_using_name_decl_specifier_seq())
    {
        if (nameSemantics.is_bit_field_declarator_id_opt())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
        if (nameSemantics.is_function_declarator_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (nameSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
        if (nameSemantics.is_function_name())
            theSemantics.set(FogSemantics::IS_USING_DECLARATIVE);
        if (nameSemantics.is_untyped_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_USING_NAME_DECL_SPECIFIER_SEQ);
    }
    if (typeSemantics.is_decl_specifier_seq())
    {
        if (nameSemantics.is_function_declarator_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATION_ID);
        if (nameSemantics.is_typed_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_TYPED_DECL_SPECIFIER_SEQ);
    }
    if (typeSemantics.is_type_specifier_seq())
    {
        if (nameSemantics.is_conversion_declarator_opt())
            theSemantics.set(FogSemantics::IS_CONVERSION_TYPE_ID);
        if (nameSemantics.is_type_id_declarator_opt())
            theSemantics.set(FogSemantics::IS_TYPE_ID);
        if (nameSemantics.is_type_specifier_seq())
            theSemantics.set(FogSemantics::IS_TYPE_SPECIFIER_SEQ);
    }
    if (typeSemantics.is_unmeta_decl_specifier_seq())
    {
        if (nameSemantics.is_unmeta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_UNMETA_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_meta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_auto_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_AUTO_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_auto_meta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
    }
    if (typeSemantics.is_meta_decl_specifier_seq())
    {
        if (nameSemantics.is_unmeta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_auto_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_meta_variable_declarator_elem())
            theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION_ELEM);
        if (nameSemantics.is_meta_variable_declarator_elem())
            theSemantics.set(FogSemantics::IS_META_VARIABLE_TYPE_ID);
        if (nameSemantics.is_meta_variable_declarator())
            theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION);
        if (nameSemantics.is_auto_variable_declaration_elem())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
        if (nameSemantics.is_auto_variable_declarator())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
        if (nameSemantics.is_meta_function_declarator_elem())
            theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION_ELEM);
        if (nameSemantics.is_meta_function_declarator())
            theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION);
        if (nameSemantics.is_meta_function_definitor())
            theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITION);
        if (nameSemantics.is_auto_function_declarator())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
        if (nameSemantics.is_auto_function_definitor())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
    }
    if (typeSemantics.is_auto_decl_specifier_seq())
    {
        if (nameSemantics.is_unmeta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_AUTO_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_meta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_meta_ctor_name())
            theSemantics.set(FogSemantics::IS_AUTO_CTOR_NAME);
        if (nameSemantics.is_meta_ctor_declaration())
            theSemantics.set(FogSemantics::IS_AUTO_CTOR_DECLARATION);
        if (nameSemantics.is_meta_ctor_definition())
            theSemantics.set(FogSemantics::IS_AUTO_CTOR_DEFINITION);
        if (nameSemantics.is_meta_dtor_name())
            theSemantics.set(FogSemantics::IS_AUTO_DTOR_NAME);
        if (nameSemantics.is_meta_dtor_declaration())
            theSemantics.set(FogSemantics::IS_AUTO_DTOR_DECLARATION);
        if (nameSemantics.is_meta_dtor_definition())
            theSemantics.set(FogSemantics::IS_AUTO_DTOR_DEFINITION);
        if (nameSemantics.is_meta_function_declaration_elem())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION_ELEM);
        if (nameSemantics.is_meta_function_declarator())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATOR);
        if (nameSemantics.is_meta_function_declaration())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
        if (nameSemantics.is_meta_function_definitor())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITOR);
        if (nameSemantics.is_meta_function_definition())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
        if (nameSemantics.is_meta_variable_declarator())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATOR);
        if (nameSemantics.is_meta_variable_declarator_elem())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION_ELEM);
        if (nameSemantics.is_meta_variable_type_id())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
        if (nameSemantics.is_meta_variable_declaration())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
    }
    if (typeSemantics.is_auto_meta_decl_specifier_seq())
    {
        if (nameSemantics.is_unmeta_decl_specifier_seq())
            theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
        if (nameSemantics.is_meta_function_declarator_elem())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION_ELEM);
        if (nameSemantics.is_meta_function_declarator())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
        if (nameSemantics.is_meta_function_definitor())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
        if (nameSemantics.is_meta_variable_declarator_elem())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
        if (nameSemantics.is_meta_variable_declarator())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
    }
    if (nameSemantics.is_epsilon())
        theSemantics.assign(typeSemantics);
//    if (_decl_specifiers.is_auto() && nameSemantics.is_scoped_meta_name())
//        theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
    if (typeSemantics.is_cv_qualifier_seq())
    {
        if (nameSemantics.is_scope_specifier())
            theSemantics.set(FogSemantics::IS_SCOPE_SPECIFIER);
        if (nameSemantics.is_type_specifier_seq())
            theSemantics.set(FogSemantics::IS_TYPE_SPECIFIER_SEQ);
    }
    if (_decl_specifiers.is_just_template())
    {
        if (nameSemantics.is_unqualified_id())
            theSemantics.set(FogSemantics::IS_TEMPLATE_UNQUALIFIED_ID);
        if (nameSemantics.is_id_expression())
            theSemantics.set(FogSemantics::IS_MEMBER_ID_EXPRESSION);
        if (nameSemantics.is_global_id_expression())
            theSemantics.set(FogSemantics::IS_MEMBER_ID_EXPRESSION);
    }
//    if (_decl_specifiers.is_just_auto())
//    {
//        if (nameSemantics.is_meta_ctor_id())
//            theSemantics.set(FogSemantics::IS_META_CTOR_NAME);
//        if (nameSemantics.is_meta_dtor_id())
//            theSemantics.set(FogSemantics::IS_META_DTOR_NAME);
//    }
    return true;
}

void FogDeclSpecifiedName::set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers)
{
    _decl_specifiers |= declSpecifiers.decl_specifiers();
    aName = this;
}

void FogDeclSpecifiedName::set_destructor_name(FogNameRef& aName)
{
    set_decorated_destructor_name(aName);
}
