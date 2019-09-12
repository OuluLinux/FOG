#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogSemantics, Super)

void FogSemantics::assign_identifier(IsPotential isPotential)
{
    set(FogSemantics::IS_IDENTIFIER);
    set(FogSemantics::IS_META_NAME);
    if (isPotential)
    {
        set(FogSemantics::IS_ENUM_NAME);
        set(FogSemantics::IS_NAMESPACE_NAME);
        set(FogSemantics::IS_TEMPLATE_TYPE_NAME);
        set(FogSemantics::IS_TEMPLATE_VALUE_NAME);
        set(FogSemantics::IS_TYPEDEF_NAME);
        set(FogSemantics::IS_CLASS_NAME);
    }
}

std::ostream& FogSemantics::print_this(std::ostream& s) const
{
    const char *prefixCharacter = "";
    prefixCharacter = SuperName::print(s, prefixCharacter);
    prefixCharacter = SuperMeta::print(s, prefixCharacter);
    prefixCharacter = SuperAuto::print(s, prefixCharacter);
    prefixCharacter = SuperExpr::print(s, prefixCharacter);
    prefixCharacter = SuperDeclarator::print(s, prefixCharacter);
    prefixCharacter = SuperDeclaration::print(s, prefixCharacter);
    prefixCharacter = SuperEntity::print(s, prefixCharacter);
    prefixCharacter = SuperSpecifier::print(s, prefixCharacter);
    return s;
}

#define FOGSEMANTICS_NAME_BIT(N) { IS_##N, #N }
#define FOGSEMANTICS_PRINT_METHOD(TYPE, MACRO, ELEMENT) \
const char *name2(FogSemantics_,TYPE)::print(std::ostream& s, const char *prefixCharacter) const \
{ \
    struct ListElement { TYPE _bit; const char *_text; }; \
    static const ListElement aTable[] = \
    { \
        MACRO(FOGSEMANTICS_NAME_BIT), \
        { TYPE(0), 0 } \
    }; \
    for (const ListElement *p = aTable; p->_text; ++p) \
        if (ELEMENT & p->_bit) \
        { \
            s << prefixCharacter << p->_text; \
            prefixCharacter = "+"; \
        } \
    return prefixCharacter; \
}
FOGSEMANTICS_PRINT_METHOD(Declaration, FOG_SEMANTICS_DECLARATIONS, _declaration)
FOGSEMANTICS_PRINT_METHOD(Declarator, FOG_SEMANTICS_DECLARATORS, _declarator)
FOGSEMANTICS_PRINT_METHOD(Entity, FOG_SEMANTICS_ENTITIES, _entity)
FOGSEMANTICS_PRINT_METHOD(Expression, FOG_SEMANTICS_EXPRESSIONS, _expression)
FOGSEMANTICS_PRINT_METHOD(Meta, FOG_SEMANTICS_METAS, _meta)
FOGSEMANTICS_PRINT_METHOD(Auto, FOG_SEMANTICS_AUTOS, _auto)
FOGSEMANTICS_PRINT_METHOD(Name, FOG_SEMANTICS_NAMES, _name)
FOGSEMANTICS_PRINT_METHOD(Specifier, FOG_SEMANTICS_SPECIFIERS, _specifier)
#undef FOGSEMANTICS_PRINT_METHOD
#undef FOGSEMANTICS_NAME_BIT

#define FOG_SEMANTICS_DECLARATION_VALUE(Name, Semantic) \
const FogSemantics& FogSemantics_Declaration::Name() \
    { static const FogSemantics theSemantics(Semantic); return theSemantics; }
FOG_SEMANTICS_DECLARATION_VALUE(file_dependency_specifier_value, IS_FILE_DEPENDENCY_SPECIFIER)
FOG_SEMANTICS_DECLARATION_VALUE(file_placement_specifier_value, IS_FILE_PLACEMENT_SPECIFIER)
FOG_SEMANTICS_DECLARATION_VALUE(function_specifier_value, IS_FUNCTION_DECLARATIVE)
FOG_SEMANTICS_DECLARATION_VALUE(variable_specifier_value, IS_OBJECT_DECLARATION)
#undef FOG_SEMANTICS_DECLARATION_VALUE

#define FOG_SEMANTICS_DECLARATOR_VALUE(Name, Semantic) \
const FogSemantics& FogSemantics_Declarator::Name() \
    { static const FogSemantics theSemantics(Semantic); return theSemantics; }
FOG_SEMANTICS_DECLARATOR_VALUE(character_value, IS_CHARACTER)
FOG_SEMANTICS_DECLARATOR_VALUE(identifier_value, IS_IDENTIFIER)
FOG_SEMANTICS_DECLARATOR_VALUE(number_value, IS_NUMBER)
FOG_SEMANTICS_DECLARATOR_VALUE(string_value, IS_STRING)
#undef FOG_SEMANTICS_DECLARATOR_VALUE

#define FOG_SEMANTICS_EXPRESSION_VALUE(Name, Semantic) \
const FogSemantics& FogSemantics_Expression::Name() \
    { static const FogSemantics theSemantics(Semantic); return theSemantics; }
FOG_SEMANTICS_EXPRESSION_VALUE(function_body_value, IS_FUNCTION_BODY)
#undef FOG_SEMANTICS_EXPRESSION_VALUE

#define FOG_SEMANTICS_NAME_VALUE(Name, Semantic) \
const FogSemantics& FogSemantics_Name::Name() \
    { static const FogSemantics theSemantics(Semantic); return theSemantics; }
#undef FOG_SEMANTICS_NAME_VALUE

#define FOG_SEMANTICS_ENTITY_VALUE(Name, Semantic) \
const FogSemantics& FogSemantics_Entity::Name() \
    { static const FogSemantics theSemantics(Semantic); return theSemantics; }
FOG_SEMANTICS_ENTITY_VALUE(base_value, IS_BASE)
FOG_SEMANTICS_ENTITY_VALUE(built_in_value, IS_BUILT_IN)
FOG_SEMANTICS_ENTITY_VALUE(class_value, IS_CLASS)
FOG_SEMANTICS_ENTITY_VALUE(entity_value, IS_ENTITY)
FOG_SEMANTICS_ENTITY_VALUE(enum_value, IS_ENUM)
FOG_SEMANTICS_ENTITY_VALUE(exception_value, IS_EXCEPTION)
FOG_SEMANTICS_ENTITY_VALUE(filespace_value, IS_FILESPACE)
FOG_SEMANTICS_ENTITY_VALUE(function_value, IS_FUNCTION)
FOG_SEMANTICS_ENTITY_VALUE(linkage_value, IS_LINKAGE)
FOG_SEMANTICS_ENTITY_VALUE(meta_class_value, IS_META_CLASS)
FOG_SEMANTICS_ENTITY_VALUE(meta_function_value, IS_META_FUNCTION)
FOG_SEMANTICS_ENTITY_VALUE(meta_parameter_value, IS_META_PARAMETER)
FOG_SEMANTICS_ENTITY_VALUE(meta_variable_value, IS_META_VARIABLE)
FOG_SEMANTICS_ENTITY_VALUE(namespace_value, IS_NAMESPACE)
FOG_SEMANTICS_ENTITY_VALUE(namespace_alias_value, IS_NAMESPACE_ALIAS)
FOG_SEMANTICS_ENTITY_VALUE(object_value, IS_OBJECT)
FOG_SEMANTICS_ENTITY_VALUE(parameter_value, IS_PARAMETER)
FOG_SEMANTICS_ENTITY_VALUE(scope_value, IS_SCOPE)
FOG_SEMANTICS_ENTITY_VALUE(struct_value, IS_STRUCT)
FOG_SEMANTICS_ENTITY_VALUE(type_value, IS_TYPE)
FOG_SEMANTICS_ENTITY_VALUE(typedef_value, IS_TYPEDEF)
FOG_SEMANTICS_ENTITY_VALUE(typename_value, IS_TYPENAME)
FOG_SEMANTICS_ENTITY_VALUE(union_value, IS_UNION)
FOG_SEMANTICS_ENTITY_VALUE(using_value, IS_USING)
FOG_SEMANTICS_ENTITY_VALUE(variable_value, IS_VARIABLE)
#undef FOG_SEMANTICS_ENTITY_VALUE

#define FOG_SEMANTICS_SPECIFIER_VALUE(Name, Semantic) \
const FogSemantics& FogSemantics_Specifier::Name() \
    { static const FogSemantics theSemantics(Semantic); return theSemantics; }
FOG_SEMANTICS_SPECIFIER_VALUE(asm_definition_value, IS_ASM_DEFINITION)
FOG_SEMANTICS_SPECIFIER_VALUE(namespace_alias_definition_value, IS_NAMESPACE_ALIAS_DEFINITION)
FOG_SEMANTICS_SPECIFIER_VALUE(using_directive_value, IS_USING_DIRECTIVE)
#undef FOG_SEMANTICS_SPECIFIER_VALUE

bool FogSemantics_Missing::missing(const char *metaName) const
{
    ERRMSG("BUG - unimplemented semantics predicate \"" << metaName << "\" test invoked.");
    return false;
}

const FogSemantics& FogSemantics_Missing::missing_value(const char *metaName)
{
    ERRMSG("BUG - unimplemented semantics predicate value \"" << metaName << "\" used.");
    static FogSemantics nullSemantics;
    nullSemantics.assign_identifier(FogSemantics::POTENTIAL);
    return nullSemantics;
}
