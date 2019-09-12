#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogCallExpr, Super)
FOGTOKEN_LEAF_IMPL(FogCallExpr)

FogCallExpr::FogCallExpr(FogExpr& anExpr, FogParenthesisExpr& theParenthesis)
:
    Super(anExpr),
    _parenthesis(theParenthesis)
{
    if (anExpr.is_null())
        ERRMSG("BUG -- expected non-null call expreession.");
}

FogCallExpr::FogCallExpr(const This& anExpr)
:
    Super(anExpr),
    _parenthesis(anExpr._parenthesis)
{}

FogCallExpr::~FogCallExpr() {}

void FogCallExpr::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _parenthesis->create_usage(aContext);
}

bool FogCallExpr::emit(FogEmitContext& emitContext) const
{
    bool doneSomething = expr().emit(emitContext);
    FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
    if (emitContext.emit_expression())
        doneSomething |= _parenthesis->emit(separatedContext);
    else
    {
        FogForUseAs forUseAs(emitContext.for_use_as() | FogEmitContext::NO_SCOPE);
        if (!forUseAs.emit_parameter_names())
            forUseAs |= FogForUseAs::NO_NAME;
        FogForUseAsEmitContext parameterContext(emitContext, forUseAs);
        doneSomething |= _parenthesis->emit(separatedContext);
    }
    return doneSomething;
}

size_t FogCallExpr::executable_tokens() const
{
    return 1 + expr().executable_tokens() + _parenthesis->executable_tokens();
}

bool FogCallExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    if (inScope.is_unresolvable())
//          return false;
        ERRMSG("INVESTIGATE -- Invoking FogCallExpr::get_object in unexpected phase");
    if (!expr().get_object(tokenValue, inScope))
        return false;
    FogTokenRef tempValue(tokenValue);
    return tempValue->get_dyadic(tokenValue, inScope, FogTokenType::character('('), _parenthesis.to_const()->exprs());
}

void FogCallExpr::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _parenthesis->install(installContext);
}

bool FogCallExpr::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _parenthesis->is_actual(scopeContext);
}

void FogCallExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _parenthesis->make_actual(_parenthesis.to_const(), makeActualContext);
}

//  
//    Create a CtorInit from an expression that should be a call.
//  
bool FogCallExpr::make_ctor_init(FogRawCtorInitRef& ctorInit)
{
    FogName *initName = expr().is_name();
    if (!initName)
    {
        ERRMSG("BUG - name expected for ctor " << viz(*this));
        return false;
    }
    ctorInit.adopt(new FogRawCtorInit(*initName, &_parenthesis->exprs()));
    return true;
}

bool FogCallExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{                                               //   Must distinguish type(name) from type name(args)
    FogName *exprName = expr().is_name();
    if (exprName)                               //   Must be FogName to be is_typed_decl_specifier_seq
    {
//        FogResolutionMakeSpecifierContext resolveSemanticsContext(makeSpecifierContext,
//              FogScopeContext::EXACTLY, FogScopeContext::AT_REQUIRED);
        FogStrongSemanticsContext exprSemantics(makeSpecifierContext);
        FogStrongSemanticsContext parenthesisSemantics(makeSpecifierContext);
        if (!expr().resolve_semantics(exprSemantics)
         || !_parenthesis->resolve_semantics(parenthesisSemantics))
            return false;
        if (exprSemantics.is_typed_decl_specifier_seq()
         && (parenthesisSemantics.is_bit_field_declarator_id_opt()
          || parenthesisSemantics.is_function_declarator_id()
          || parenthesisSemantics.is_object_declarator_id()
          || parenthesisSemantics.is_parameter_declarator_opt()))
        {
            if (_parenthesis->make_specifier(makeSpecifierContext)
             && exprName->make_type_specifier(makeSpecifierContext))
                return true;
        }
    }
    if (!expr().make_specifier(makeSpecifierContext))
        return false;
    FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
    if (objectSpecifier)
        objectSpecifier->adopt(_parenthesis->new_function_modifier(makeSpecifierContext));
    return true;
}

void FogCallExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._parenthesis->merge_into(mergeContext, _parenthesis.to_const());
}

const FogMerge& FogCallExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
    needsMerge |= _parenthesis->needs_merge(mergeContext, *anExpr._parenthesis);
    return needsMerge;
}

std::ostream& FogCallExpr::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _parenthesis->print_deep(s, aDepth);
    return s;
}

std::ostream& FogCallExpr::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _parenthesis->print_on(s, aDepth);
    return s;
}

char FogCallExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = Super::print_named(s, fullId, tailChar);
    tailChar = _parenthesis->print_named(s, 0, tailChar);
    return tailChar;
}

std::ostream& FogCallExpr::print_viz(std::ostream& s) const
{
    s << "\"call-expression: ";
    print_named(s, 0, ' ');
    return s << '\"';
}

bool FogCallExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    FogChildSemanticsContext parenthesisSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics)
     || !_parenthesis->resolve_semantics(parenthesisSemantics))
        return false;
    theSemantics.reset();
    if (parenthesisSemantics.is_expression_list_opt())
    {
        if ((exprSemantics.is_basic_expression() || exprSemantics.is_simple_type_specifier()))
            theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
        if (exprSemantics.is_mem_initializer_id())
            theSemantics.set(FogSemantics::IS_MEM_INITIALIZER);
    }
    if (parenthesisSemantics.is_expression_list())
    {
        if (exprSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR);
        if (exprSemantics.is_object_type_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
    }
    if (parenthesisSemantics.is_parameter_declaration_clause())
    {
        if (parenthesisSemantics.is_epsilon())
        {
            if (exprSemantics.is_class_name())
                theSemantics.set(FogSemantics::IS_DESTRUCTOR_DECLARATOR);
            if (exprSemantics.is_pseudo_destructor_name())
                theSemantics.set(FogSemantics::IS_DESTRUCTOR_TYPE_ID);
        }
        if (exprSemantics.is_constructor_declarator())
            theSemantics.set(FogSemantics::IS_CONSTRUCTOR_TYPE_ID);
        if (exprSemantics.is_function_declarator_id() || exprSemantics.is_function_declarator())
            theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR);
        if (exprSemantics.is_function_declaration_id() || exprSemantics.is_function_type_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (exprSemantics.is_parameter_declarator_opt())
            theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATOR_OPT);
        if (exprSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR_ID);
        if (exprSemantics.is_object_type_id())
            theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
        if (exprSemantics.is_parameter_type_id())
            theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
        if (exprSemantics.is_type_id_declarator_opt())
            theSemantics.set(FogSemantics::IS_TYPE_ID_DECLARATOR_OPT);
        if (exprSemantics.is_typedef_declaration())
            theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
        if (exprSemantics.is_type_id())
            theSemantics.set(FogSemantics::IS_TYPE_ID);
    }
    if (parenthesisSemantics.is_meta_parameter_declaration_clause())
    {
        if (parenthesisSemantics.is_epsilon())
        {
            if (exprSemantics.is_meta_ctor_name())
                theSemantics.set(FogSemantics::IS_META_CTOR_DECLARATION);
            if (exprSemantics.is_meta_dtor_name())
                theSemantics.set(FogSemantics::IS_META_DTOR_DECLARATION);
            if (exprSemantics.is_auto_ctor_name())
                theSemantics.set(FogSemantics::IS_AUTO_CTOR_DECLARATION);
            if (exprSemantics.is_auto_dtor_name())
                theSemantics.set(FogSemantics::IS_AUTO_DTOR_DECLARATION);
        }
        if (exprSemantics.is_scoped_meta_name())
            theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATOR_ELEM);
        if (exprSemantics.is_meta_variable_declaration_elem())
            theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION_ELEM);
        if (exprSemantics.is_auto_variable_declaration_elem())
            theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION_ELEM);
    }
    if (exprSemantics.is_typed_decl_specifier_seq())
    {
        if (parenthesisSemantics.is_bit_field_declarator_id_opt())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
        if (parenthesisSemantics.is_function_declarator_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (parenthesisSemantics.is_object_declarator_id())
        {
            theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
            if (expr().decl_specifiers().is_typedef())
                theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
        }
        if (parenthesisSemantics.is_parameter_declarator_opt())
            theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
        if (parenthesisSemantics.is_typedef_declaration())
            theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
    }
    if (exprSemantics.is_using_decl_specifier_seq())
    {
        if (parenthesisSemantics.is_bit_field_declarator_id_opt())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
        if (parenthesisSemantics.is_function_declarator_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (parenthesisSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
    }
    if (exprSemantics.is_using_name_decl_specifier_seq())
    {
        if (parenthesisSemantics.is_bit_field_declarator_id_opt())
            theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
        if (parenthesisSemantics.is_function_declarator_id())
            theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
        if (parenthesisSemantics.is_object_declarator_id())
            theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
    }
    if (exprSemantics.is_type_specifier_seq())
    {
        if (parenthesisSemantics.is_type_id_declarator_opt())
            theSemantics.set(FogSemantics::IS_TYPE_ID);
    }
    if (exprSemantics.is_meta_decl_specifier_seq())
    {
        if (parenthesisSemantics.is_meta_variable_declarator_elem())
            theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION_ELEM);
    }
    if (exprSemantics.is_scoped_meta_type_name())
    {
        if (parenthesisSemantics.is_meta_parameter_name())
            theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATION_ELEM);
    }
    if (exprSemantics.is_auto_decl_specifier_seq())
    {
        if (parenthesisSemantics.is_meta_variable_declaration_elem())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION_ELEM);
    }
    if (exprSemantics.is_auto_meta_decl_specifier_seq())
    {
        if (parenthesisSemantics.is_meta_object_name())
            theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION_ELEM);
    }
    return true;
}

void FogCallExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}

void FogCallExpr::set_destructor_expression(FogExprRef& anExpr)
{
    Super::set_destructor_expression();
    anExpr = *this;
}
