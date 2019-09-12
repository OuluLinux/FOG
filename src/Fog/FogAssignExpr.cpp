#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogAssignExpr, Super)

bool FogAssignExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{                           //   NB the put is here. Invoked +='s must therefore do +.
    FogTokenRef tempValue;
    bool returnStatus = Super::get_object(tempValue, inScope) && expr().put_value(inScope, *tempValue, inScope);
    tokenValue.reset();
    return returnStatus;
}

#define FOGASSIGNEXPR_IMPLEMENTATION(T, N, DT) \
TYPEINFO_SINGLE(T, Super) \
FOGTOKEN_LEAF_IMPL(T) \
FogTokenType::TokenType T::dyadic_token() const { return FogTokenType::TokenType(DT); } \
const char *T::separator() const { return N; }

FOGASSIGNEXPR_IMPLEMENTATION(FogAssExpr, " = ", '=')
FOGASSIGNEXPR_IMPLEMENTATION(FogAssAddExpr, " += ", FogTokenType::ASS_ADD)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssAndExpr, " &= ", FogTokenType::ASS_AND)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssDivExpr, " /= ", FogTokenType::ASS_DIV)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssModExpr, " %= ", FogTokenType::ASS_MOD)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssMulExpr, " *= ", FogTokenType::ASS_MUL)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssOrExpr, " |= ", FogTokenType::ASS_OR)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssShlExpr, " <<= ", FogTokenType::ASS_SHL)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssShrExpr, " >>= ", FogTokenType::ASS_SHR)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssSubExpr, " -= ", FogTokenType::ASS_SUB)
FOGASSIGNEXPR_IMPLEMENTATION(FogAssXorExpr, " ^= ", FogTokenType::ASS_XOR)

bool FogAssExpr::emit(FogEmitContext& emitContext) const
{
    expr().emit(emitContext);
    emitContext.emit_space_and_text(" = ");
    FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
    expr1().emit(separatedContext);
    return true;
}

bool FogAssExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    FogTokenRef rightValue;
    bool returnStatus = false;
    
    VERBOSE(std::strstream str2; expr1().print_viz(str2); printf("DEBUG get_object expr1 %s\n", str2.str());)
    
    returnStatus = expr1().get_object(rightValue, inScope);
    
    VERBOSE(std::strstream str0; rightValue->print_viz(str0); printf("DEBUG get_object right_value %s\n", str0.str());)
    
    if (returnStatus) {
        VERBOSE(std::strstream str1; expr().print_viz(str1); printf("DEBUG get_object expr %s\n", str1.str());)
        
		returnStatus = expr().put_value(inScope, *rightValue, inScope);
    }
        
    tokenValue.reset();
    return returnStatus;
}

bool FogAssExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!expr().make_specifier(makeSpecifierContext))
        return false;
    FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
    if (objectSpecifier)
    {
//          FogResolutionScopeContext weakContext(makeSpecifierContext, FogResolutionScopeContext::OPTIONAL);
//          FogResolutionScopeContext weakContext(makeSpecifierContext, FogResolutionScopeContext::DOLLAR_REQUIRED);
        FogMakeSpecifierContext& weakContext = makeSpecifierContext;
        FogExprRef initExpr;
        expr1().make_actual(initExpr.to_const(), weakContext);
        objectSpecifier->set_initializer(*initExpr);
    }
    return true;
}

bool FogAssExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext leftSemantics(theSemantics);
    if (!expr().resolve_semantics(leftSemantics))
        return false;
    FogChildSemanticsContext rightSemantics(theSemantics);
    if (!expr1().resolve_semantics(rightSemantics))
        return false;
    theSemantics.reset();
    if (rightSemantics.is_assignment_expression())
    {
        if (leftSemantics.is_basic_expression())
            theSemantics.set(FogSemantics::IS_ASSIGNMENT_EXPRESSION);
        if (leftSemantics.is_parameter_type_id())
            theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATION);
    }
    if (rightSemantics.is_constant_expression())
    {
        if (leftSemantics.is_bit_field_declarator_elem())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATOR);
        if (leftSemantics.is_bit_field_declaration_elem())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATION);
    }
    if (rightSemantics.is_initializer_clause())
    {
        if (leftSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR);
        if (leftSemantics.is_object_type_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
    }
    if (rightSemantics.is_zero())
    {
        if (leftSemantics.is_function_type_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATIVE);
        if (leftSemantics.is_destructor_type_id())
            theSemantics.set(FogSemantics::IS_DESTRUCTOR_DECLARATIVE);
    }
    if (leftSemantics.is_auto_variable_type_id())
        theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
    if (leftSemantics.is_meta_variable_declarator_elem())
        theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATOR);
    if (leftSemantics.is_meta_variable_type_id())
        theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION);
    if (leftSemantics.is_meta_parameter_declarator_elem())
        theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATOR);
    if (leftSemantics.is_meta_parameter_type_id())
        theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATION);
    if (leftSemantics.is_simple_type_parameter() && rightSemantics.is_type_id())
        theSemantics.set(FogSemantics::IS_TYPE_PARAMETER);
    if (leftSemantics.is_templated_parameter() && rightSemantics.is_template_name())
        theSemantics.set(FogSemantics::IS_TYPE_PARAMETER);
    return true;
}

void FogAssExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
