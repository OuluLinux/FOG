#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogIndexExpr, Super)
FOGTOKEN_LEAF_IMPL(FogIndexExpr)

FogTokenType::TokenType FogIndexExpr::dyadic_token() const { return FogTokenType::character('['); }

bool FogIndexExpr::emit(FogEmitContext& emitContext) const
{
 expr().emit(emitContext);
 emitContext.emit_space_and_text("[");
 expr1().emit(emitContext);
 emitContext.emit_space_and_text("]");
 return true;
}

FogIndexExpr *FogIndexExpr::is_index_expr() { return this; }

bool FogIndexExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 if (!expr().make_specifier(makeSpecifierContext))
  return false;
 FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
 if (objectSpecifier)
  objectSpecifier->adopt(new FogArrayModifier(expr1().is_null() ? 0 : &expr1()));
 return true;
}

FogExpr *FogIndexExpr::new_index_cast_expression(FogExpr& anExpr)
{
    if (!expr().is_anon())
    if (!expr().is_anon())
        ERRMSG("Expected abstract index expression for index cast");
    return  new FogIndexCastExpr(expr1(), anExpr);
}

char FogIndexExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = expr().print_named(s, 0, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, "[");
 tailChar = expr1().print_named(s, 0, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, "]");
 return tailChar;
}

bool FogIndexExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext leftSemantics(theSemantics);
 FogChildSemanticsContext rightSemantics(theSemantics);
 if (!expr().resolve_semantics(leftSemantics) || !expr1().resolve_semantics(rightSemantics))
  return false;
 theSemantics.reset();
 if (leftSemantics.is_basic_expression() && rightSemantics.is_expression())
  theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
 if (rightSemantics.is_constant_expression_opt())
 {
  if (leftSemantics.is_function_declarator_id())
   theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR_ID);
  if (leftSemantics.is_function_declarator())
   theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR);
  if (leftSemantics.is_function_type_id())
   theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
  if (leftSemantics.is_meta_object_name())
   theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATOR_ELEM);
  if (leftSemantics.is_meta_variable_declaration_elem())
   theSemantics.set(FogSemantics::IS_META_VARIABLE_TYPE_ID);
  if (leftSemantics.is_auto_variable_declaration_elem())
   theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
  if (leftSemantics.is_meta_function_declarator_elem())
   theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATOR);
  if (leftSemantics.is_meta_function_declaration_elem())
   theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION);
  if (leftSemantics.is_auto_function_declarator_elem())
   theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATOR);
  if (leftSemantics.is_auto_function_declaration_elem())
   theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
  if (leftSemantics.is_meta_parameter_name())
   theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATOR_ELEM);
  if (leftSemantics.is_meta_parameter_declaration_elem())
   theSemantics.set(FogSemantics::IS_META_PARAMETER_TYPE_ID);
  if (leftSemantics.is_object_declarator_id())
   theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR_ID);
  if (leftSemantics.is_object_type_id())
   theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
  if (leftSemantics.is_parameter_declarator_opt())
   theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATOR_OPT);
  if (leftSemantics.is_parameter_type_id())
   theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
  if (leftSemantics.is_typedef_declaration())
   theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
  if (leftSemantics.is_type_id_declarator_opt())
   theSemantics.set(FogSemantics::IS_TYPE_ID_DECLARATOR_OPT);
  if (leftSemantics.is_type_id())
   theSemantics.set(FogSemantics::IS_TYPE_ID);
 }
 return true;
}

void FogIndexExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
 set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
