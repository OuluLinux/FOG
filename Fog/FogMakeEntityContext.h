
#ifndef FOGMAKEENTITYCONTEXT_HXX
#define FOGMAKEENTITYCONTEXT_HXX



class FogMakeEntityContext : public FogScopeContext {
	typedef FogScopeContext Super;
	typedef FogMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMakeEntityContext(const This& aContext);
	This& operator=(const This& aContext);
	This& mutate() const { return *(This*)this; }
protected:
	FogMakeEntityContext() {}
public:
	virtual const FogDeclSpecifierValue& decl_specifiers() const = 0;
	virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
	virtual FogMakeEntityContext* find_make_entity_context();
	virtual HasContext has_context() const;
	virtual Resolution resolution() const;
	virtual FogTargetFile* target_file() const = 0;
	virtual const FogUtility& utility() const = 0;
};

//
//    Normal make_entity support invoked from FogExpr/FogListOfExpr::compile_parsed
//
class FogCompileMakeEntityContext : public FogMakeEntityContext {
	typedef FogMakeEntityContext Super;
	typedef FogCompileMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMakeSpecifierContext& _make_specifier_context;
private:
	FogCompileMakeEntityContext(const This& aContext);
	This& operator=(const This& aContext);
public:
	FogCompileMakeEntityContext(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual FogScopeContext* find_context(size_t dollarCount);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual InScope in_scope() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual Resolution resolution() const;
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual FogTargetFile* target_file() const;
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
	virtual const FogUtility& utility() const;
};

//
//    Repeated make_entity support invoked from FogDerivable::make_derived/make_used and derived instantiate()
//
class FogRecompileMakeEntityContext : public FogMakeEntityContext {
	typedef FogMakeEntityContext Super;
	typedef FogRecompileMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _dynamic_token;                           //   Current scope.
	FogPotentialDeclaration& _potential_declaration;
	const HasContext _has_context;
private:
	FogRecompileMakeEntityContext(const This& aContext);
	This& operator=(const This& aContext);
public:
	FogRecompileMakeEntityContext(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration,
	                              HasContext hasContext);
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual HasContext has_context() const;
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual FogTargetFile* target_file() const;
	virtual const FogUtility& utility() const;
};

//
//    Undisciplined make_entity support invoked (to be phased out)
//        FogClassSpecifier::make_type_entity for base clauses
//        FogCode::compile_parsed for ctor-inits
//        FogName::get_type_in to create an error suppressing type
//        FogScope_Using::do_derive to recreate earlier context
//
class FogStaticMakeEntityContext : public FogMakeEntityContext {
	typedef FogMakeEntityContext Super;
	typedef FogStaticMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _dynamic_token;                           //   Current scope.
private:
	FogStaticMakeEntityContext(const This& aContext);
	This& operator=(const This& aContext);
public:
	FogStaticMakeEntityContext(FogScopeContext& inScope);
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual FogTargetFile* target_file() const;
	virtual const FogUtility& utility() const;
};

//
//    FogResolvableFunctionMakeEntityContext defines the nested context to resolve the formal meta-functions parameters
//    upon invocation from FogMetaFunction::FogMetaFunction.
//
class FogDecoratedMakeEntityContext : public FogMakeEntityContext {
	typedef FogMakeEntityContext Super;
	typedef FogDecoratedMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMakeEntityContext& _make_entity_context;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogDecoratedMakeEntityContext(FogMakeEntityContext& makeEntityContext) : _make_entity_context(makeEntityContext) {}
	FogDecoratedMakeEntityContext(This& makeEntityContext) : _make_entity_context(makeEntityContext) {}
	FogMakeEntityContext& make_entity_context() { return _make_entity_context; }
	const FogMakeEntityContext& make_entity_context() const { return _make_entity_context; }
public:
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual FogScopeContext* find_context(size_t dollarCount);
	virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual HasContext has_context() const;
	virtual InScope in_scope() const;
	virtual const FogScopeContext* meta_context() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual Resolution resolution() const;
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual FogTargetFile* target_file() const;
	//      virtual const FogExpr *template_arguments() const;
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
	virtual const FogUtility& utility() const;
};

//
//    A FogInScopeMakeEntityContext decorates a FogMakeEntityContext to change the name resolution strategy.
//
class FogInScopeMakeEntityContext : public FogDecoratedMakeEntityContext {
	typedef FogDecoratedMakeEntityContext Super;
	typedef FogInScopeMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const InScope _in_scope;
private:
	FogInScopeMakeEntityContext(const This& makeEntityContext);
	This& operator=(const This& makeEntityContext);
public:
	FogInScopeMakeEntityContext(FogMakeEntityContext& makeEntityContext, InScope inScope);
	virtual InScope in_scope() const;
};

class FogReScopedMakeEntityContext : public FogDecoratedMakeEntityContext {
	typedef FogDecoratedMakeEntityContext Super;
	typedef FogReScopedMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _dynamic_token;                           //   Current token.
private:
	FogReScopedMakeEntityContext(const This& makeEntityContext);
	This& operator=(const This& makeEntityContext);
protected:
	FogReScopedMakeEntityContext(FogMakeEntityContext& makeEntityContext, FogToken& inToken);
public:
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
};

//
//    Decorate the context with a scope nesting on behalf of
//        FogNestedName::make_name_entity(FogMakeEntityContext& makeEntityContext)
//        FogElaboratedTypeSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext)
//
class FogNestedMakeEntityContext : public FogReScopedMakeEntityContext {
	typedef FogReScopedMakeEntityContext Super;
	typedef FogNestedMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogNestedMakeEntityContext(const This& makeEntityContext);
	This& operator=(const This& makeEntityContext);
	This& mutate() const { return *(This*)this; }
public:
	FogNestedMakeEntityContext(FogMakeEntityContext& makeEntityContext, FogToken& inToken);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	//      virtual const FogExpr *template_arguments() const;
};

//
//    FogResolvableFunctionMakeEntityContext defines the nested context to resolve the formal meta-functions parameters
//    upon invocation from FogMetaFunction::FogMetaFunction.
//
class FogResolvableFunctionMakeEntityContext : public FogReScopedMakeEntityContext {
	typedef FogReScopedMakeEntityContext Super;
	typedef FogResolvableFunctionMakeEntityContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogMetaFunction& _function;
public:
	FogResolvableFunctionMakeEntityContext(FogMakeEntityContext& makeEntityContext, FogMetaFunction& metaFunction);
	virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual Resolution resolution() const;
};
#endif

