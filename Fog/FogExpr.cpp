#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogExpr, Super)
PRIMREF_NULL_CLAS(FogExpr)
FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogExpr)
PRIMREFS_IMPL(FogExpr)
TMPL_HACK_FIX_DO(FogExpr)

bool FogExpr::compile_parsed(FogParseContext& parseContext) {
	FogStrongSemanticsContext theSemantics(parseContext);
	
	if (!resolve_semantics(theSemantics))
		;
	else if (theSemantics.is_epsilon())
		return true;
	else if (theSemantics.is_declaration() || (parseContext.at_meta() && theSemantics.is_meta_declaration())) {
		FogBaseMakeSpecifierContext makeSpecifierContext(parseContext, theSemantics, decl_specifiers(),
		        parseContext.at_meta() ? FogMakeSemantics::META_DECLARATION : FogMakeSemantics::DECLARATION);
		        
		if (!make_specifier(makeSpecifierContext)) {
			ERRMSG("Failed to make_specifier for " << viz(*this));
			return false;
		}
		
		makeSpecifierContext.compile_specifier();
		makeSpecifierContext.compile_type_specifier();
		FogCompileMakeEntityContext compileContext(makeSpecifierContext);
		FogEntity* typeEntity = makeSpecifierContext.specifier().make_type_entity(compileContext);
		FogEntity* anEntity = makeSpecifierContext.specifier().make_name_entity(compileContext);
		VERBOSE(std::strstream str1; anEntity->print_viz(str1); printf("DEBUG compile_parsed Entity %x %s\n", anEntity, str1.str());)
		
		if (!anEntity)
			ERRMSG("Failed to make_entity for " << viz(*this));
			
		return anEntity != 0;
	}
	else if (theSemantics.is_expression()) {
		FogTokenRef tokenValue;
		FogMetaScopeContext metaScopeContext(parseContext);
		bool ret = get_object(tokenValue, metaScopeContext);
		VERBOSE(std::strstream str; tokenValue->print_viz(str); printf("DEBUG compile_parsed %s\n", str.str());)
		return ret;
	}
	else if (theSemantics.is_using_directive()) {
		WRNMSG("using directive semantics are not implemented");
		return true;
	}
	
	FogWeakSemanticsContext weakSemantics(FogScopeContext::null());
	
	if (!resolve_semantics(weakSemantics) || !weakSemantics)
		ERRMSG("Failed to resolve semantics of " << viz(*this));
	else {
		const char* weakReason = weakSemantics.reason();
		FogMakeSemantics makeSemantics(weakSemantics, decl_specifiers(), FogMakeSemantics::DECLARATION);
		PrimOstrstream s;
		s << "Failed to resolve semantics of " << viz(*this) << "\n\t\tLooks like " << weakReason << " ";
		makeSemantics.print_this(s);
		ERRMSGZ(s.str());
	}
	
	return false;
}

const FogDeclSpecifierValue& FogExpr::decl_specifiers() const {
	ERRMSG("BUG -- should not decl_specifiers for " << viz(*this));
	static const FogDeclSpecifierValue defaultDeclSpecifiers;
	return defaultDeclSpecifiers;
}

//
//    Find the formal argument matching this name inScope.
//    Returns 0 since arbitrary expressions are not names and so cannot be formals.
//
const FogMetaArgument* FogExpr::find_formal_in(FogScopeContext& scopeContext) const { return 0; }

FogAnonId* FogExpr::is_anon() { return 0; }
FogExpr* FogExpr::is_expr() { return this; }
FogIndexCastExpr* FogExpr::is_index_cast_expr() { return 0; }
FogIndexExpr* FogExpr::is_index_expr() { return 0; }
FogParenthesisExpr* FogExpr::is_parenthesis_expr() { return 0; }
FogTreeCallExpr* FogExpr::is_tree_call_expr() { return 0; }

//
//    Create a CtorInit from an expression that should be a call.
//
bool FogExpr::make_ctor_init(FogRawCtorInitRef& ctorInit) {
	ERRMSG("BUG -- should not make_ctor_init for " << viz(*this));
	return false;
}

//
//    Insert theType as a FogTypedName for the name of this exporession tree. Returns true if successfull.
//
bool FogExpr::make_typed_expression(FogExprRef& theExpr, FogName& theType) {
	ERRMSG("BUG -- should not make_typed_expression for " << viz(*this));
	return false;
}

const FogMetaType& FogExpr::meta_type() const { return FogMetaType::expression_type(); }

bool FogExpr::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                       FogScopeContext& inScope) const {
	const FogMetaType& myMetaType = meta_type();
	
	if (metaType.is(FogMetaType::statement_type()) && myMetaType.is(FogMetaType::expression_type())) {
		FogExpressionStatement* aStatement = new FogExpressionStatement(mutate());
		
		if (aStatement) {
			aStatement->set_line(line());
			returnValue.adopt(*aStatement);
		}
	}
	else
		return Super::morph_to(returnValue, metaType, isExposed, inScope);
		
	return true;
}

std::ostream& FogExpr::print_this(std::ostream& s) const {
	if (!is_null())
		print_named(s, 0, 0);
		
	return s;
}

std::ostream& FogExpr::print_viz(std::ostream& s) const {
	s << "\"expression: ";
	print_this(s);
	return s << '\"';
}

//
//    Transfer the value of objectValue within scopeContext to returnValue given that objectValue
//    takes the form of this expression.
//
//    This method is only invoked for FogTreeLiteral, and so derived
//        name-like terms (FogName) apply get_monadic('=') to the object slot
//        call-like terms (FogTreeCallExpr) apply get_dyadic('(') to the function slot
//
bool FogExpr::resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext, const FogToken& objectValue) const {
	ERRMSG("Should not resolve_slot for " << viz(*this));
	return false;
}

void FogExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {
	ERRMSG("Should not set_decl_specifier_expression for " << viz(*this));
}

void FogExpr::set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers) {
	FogExprRef anExpr;
	set_decl_specifier_expression(anExpr, declSpecifiers);
	aRaw = *anExpr;
}

void FogExpr::set_destructor_expression(FogExprRef& anExpr) {
	ERRMSG("Should not set_destructor_expression for " << viz(*this));
}
