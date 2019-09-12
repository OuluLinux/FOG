#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogBitFieldExpr, Super)
FOGTOKEN_LEAF_IMPL(FogBitFieldExpr)

FogTokenType::TokenType FogBitFieldExpr::dyadic_token() const {
	return FogTokenType::character(':');
}

bool FogBitFieldExpr::emit(FogEmitContext& emitContext) const {
	expr().emit(emitContext);
	emitContext.emit_space_and_text(" : ");
	expr1().emit(emitContext);
	return true;
}

bool FogBitFieldExpr::make_ctor_init(FogRawCtorInitRef& ctorInit) {
	return expr1().make_ctor_init(ctorInit);
}

bool FogBitFieldExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	if (!expr().make_specifier(makeSpecifierContext))
		return false;
		
	FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
	
	if (objectSpecifier)
		objectSpecifier->set_colon_value(expr1());
		
	return true;
}

char FogBitFieldExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	tailChar = expr().print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, " : ");
	tailChar = expr1().print_named(s, 0, tailChar);
	return tailChar;
}

bool FogBitFieldExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext leftSemantics(theSemantics);
	
	if (!expr().resolve_semantics(leftSemantics))
		return false;
		
	FogChildSemanticsContext rightSemantics(theSemantics);
	
	if (!expr1().resolve_semantics(rightSemantics))
		return false;
		
	theSemantics.reset();
	
	if (rightSemantics.is_constant_expression()) {
		if (leftSemantics.is_bit_field_declarator_id_opt())
			theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATOR_ELEM);
			
		if (leftSemantics.is_bit_field_type_id() || leftSemantics.is_typed_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATION_ELEM);
	}
	
	if (rightSemantics.is_mem_initializer()) {
		if (leftSemantics.is_constructor_type_id())
			theSemantics.set(FogSemantics::IS_CONSTRUCTOR_DECLARATIVE);
	}
	
	return true;
}

void FogBitFieldExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {
	set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
