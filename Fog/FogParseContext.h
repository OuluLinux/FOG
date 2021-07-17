
#ifndef FOGPARSECONTEXT_HXX
#define FOGPARSECONTEXT_HXX






//
//    The abstract FogParseContext defines the protocol for the context used during processing of parsed declarations
//    and for subsequent make of a specifier.
//
class FogParseContext : public FogScopeContext {
	typedef FogScopeContext Super;
	typedef FogParseContext This;
	TYPEDECL_SINGLE(This, Super)
protected:
	FogParseContext() {}
	FogParseContext(const FogScopeContext& ctx) : Super(ctx, 123) {}
public:
	virtual const FogAccess& access() const = 0;
	virtual const FogRaw& declaration() const = 0;
	virtual const FogTemplateParameterSpecifiers& explicit_template_parameters() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual Resolution resolution() const;
	virtual void set_access(const FogAccess& anAccess) = 0;         //   used by AccessibilityDeclaration
	virtual void set_do_break();
	virtual void set_do_continue();
	virtual void set_utility(const FogUtility& aUtility) = 0;       //   used by SetUtilityDeclaration
	virtual FogTargetFile* target_file() const = 0;
	virtual const FogUtility& utility() const = 0;
};

//
//    A FogScopeParseContext refines the parse abstraction for new contexts
//
class FogDeclarationParseContext : public FogParseContext {
	typedef FogParseContext Super;
	typedef FogDeclarationParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _static_token;                                        //   Current token.
	//    FogTargetFile *const _target_file;                              // Pre-determined destination of declaration.
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogDeclarationParseContext(FogToken& aToken);
	FogDeclarationParseContext(FogToken& aToken, FogScopeContext& _scope_context);
public:
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual FogTargetFile* target_file() const;
};

//
//    A FogSubspaceParseContext refines the context to make a subspace. (Invoked from FogDerivable::actualize).
//
class FogSubspaceParseContext : public FogDeclarationParseContext {
	typedef FogDeclarationParseContext Super;
	typedef FogSubspaceParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogScopeContext& _scope_context;
	FogPotentialDeclaration& _potential_declaration;
public:
	FogSubspaceParseContext(FogScopeContext& scopeContext, FogPotentialDeclaration& potentialDeclaration);
	virtual ~FogSubspaceParseContext();
	virtual const FogAccess& access() const;
	virtual bool at_root() const;
	virtual const FogRaw& declaration() const;
	virtual void set_access(const FogAccess& anAccess);
	virtual void set_utility(const FogUtility& aUtility);
	virtual const FogUtility& utility() const;
};

//
//    A FogScopeParseContext refines the parse abstraction for new scoped contexts
//
class FogScopeParseContext : public FogDeclarationParseContext {
	typedef FogDeclarationParseContext Super;
	typedef FogScopeParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogTokenMonitor _monitor;                                       //   Error diagnosis linkage
	const FogRawRefToConst _declaration;                            //   The source declaration
	FogAccessHandle _access;                                        //   Accessibility of declaration.
	FogUtilityHandle _utility;                                      //   Status of declaration.
protected:
	FogScopeParseContext(FogToken& aToken, const FogRaw& aDeclaration, const FogAccess& anAccess,
	                     const FogUtility& aUtility);
	virtual ~FogScopeParseContext();
public:
	virtual const FogAccess& access() const;
	virtual const FogRaw& declaration() const;
	using Super::print_depth;
	using Super::print_members;
	virtual void set_access(const FogAccess& anAccess);
	virtual void set_utility(const FogUtility& aUtility);
	virtual const FogUtility& utility() const;
};

//
//    A FogEmitParseContext is the starting point for the parse of meta-code during emisdsion.
//    (Invoked from Fog*Statement::emit.)
//
class FogEmitParseContext : public FogScopeParseContext {
	typedef FogScopeParseContext Super;
	typedef FogEmitParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	//      FogTargetFile *const _target_file;                              // Pre-determined destination of declaration.
public:
	FogEmitParseContext(FogEmitContext& emitContext, const FogRaw& aDeclaration);
	//      virtual FogScopeContext *find_context(size_t dollarCount);
	//      virtual FogTargetFile *target_file() const;
	virtual const FogScopeContext* meta_context() const;
};

//
//    A FogMembersParseContext refines the context to make a lexically nested context.
//    (Invoked from FogTypeSpecifier::make_body(FogMakeContext& makeContext, FogScope& aScope)).
//
class FogMembersParseContext : public FogScopeParseContext {
	typedef FogScopeParseContext Super;
	typedef FogMembersParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMakeEntityContext& _make_entity_context;
public:
	FogMembersParseContext(FogMakeEntityContext& makeEntityContext, const FogRaw& aDeclaration,
	                       FogScope& aScope, const FogAccess& anAccess);
	virtual ~FogMembersParseContext();
	virtual FogScopeContext* find_context(size_t dollarCount);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
};

//
//    A FogStaticParseContext is the starting point for the parse of a source text declaration.
//    (Invoked from FogDerivable::make_used, FogParserBase::compile_declaration, FogScope::meta_construct.)
//
class FogStaticParseContext : public FogScopeParseContext {
	typedef FogScopeParseContext Super;
	typedef FogStaticParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogTargetFile* const _target_file;                              //   Pre-determined destination of declaration.
public:
	FogStaticParseContext(FogToken& aToken, const FogRaw& aDeclaration,
	                      const FogAccess& anAccess, const FogUtility& aUtility);
	virtual FogScopeContext* find_context(size_t dollarCount);
	virtual FogTargetFile* target_file() const;
};

//
//    A FogDecoratedParseContext provides the redirection to decorate a parse context..
//
class FogDecoratedParseContext : public FogParseContext {
	typedef FogParseContext Super;
	typedef FogDecoratedParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogParseContext& _parse_context;
protected:
	FogDecoratedParseContext(FogParseContext& parseContext) : FogParseContext(parseContext), _parse_context(parseContext) {}
	FogParseContext& parse_context() { return _parse_context; }
public:
	virtual const FogAccess& access() const;
	virtual const FogRaw& declaration() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual const FogTemplateParameterSpecifiers& explicit_template_parameters() const;
	virtual FogScopeContext* find_context(size_t dollarCount);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual InScope in_scope() const;
	virtual const FogScopeContext* meta_context() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual Resolution resolution() const;
	virtual void set_access(const FogAccess& anAccess);
	virtual void set_do_break();
	virtual void set_do_continue();
	virtual void set_utility(const FogUtility& aUtility);
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual FogTargetFile* target_file() const;
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
	virtual const FogUtility& utility() const;
};

//
//    A FogBreakableParseContext is the starting point for the parse of a meta-statement.
//    (Invoked from FogDoWhileStatement/FogForStatement/FogSwitchStatement/FogWhileStatement::compile_parsed)
//
class FogBreakableParseContext : public FogDecoratedParseContext, private FogTokenMonitor {
	typedef FogDecoratedParseContext Super;
	typedef FogBreakableParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogRawRefToConst _declaration;                            //   The source declaration
	bool _do_break;
	bool _do_continue;
private:
	This& mutate() const { return *(This*)this; }
	virtual std::ostream& print_message(std::ostream& s) const;
public:
	FogBreakableParseContext(FogParseContext& parseContext, const FogRaw& aDeclaration);
	virtual ~FogBreakableParseContext();
	virtual const FogRaw& declaration() const;
	bool do_break() const { return _do_break; }
	bool do_continue() const { return _do_continue; }
	virtual void set_do_break();
	virtual void set_do_continue();
};

//
//    A FogDynamicParseContext is the starting point for the parse of a nested declaration.
//    (Invoked from FogRaws::compile_parsed, FogTokens::compile_parsed, FogObjectSpoecifier::compile_subspace)
//
class FogDynamicParseContext : public FogDecoratedParseContext, private FogTokenMonitor {
	typedef FogDecoratedParseContext Super;
	typedef FogDynamicParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogRawRefToConst _declaration;                            //   The source declaration
	mutable FogTemplateParameterSpecifiersRefToConst _template_parameters;
private:
	This& mutate() const { return *(This*)this; }
public:
	FogDynamicParseContext(FogParseContext& parseContext, const FogRaw& aDeclaration);
	virtual ~FogDynamicParseContext();
	virtual const FogRaw& declaration() const;
	virtual const FogTemplateParameterSpecifiers& explicit_template_parameters() const;
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
};

//
//    A FogMetaParseContext decorates a FogParseContext to raise name resolution to the meta-level.
//    From FogDerivedClause::compile_parsed, FogDoWhileStatement::compile_parsed, etc.
//
class FogMetaParseContext : public FogDecoratedParseContext {
	typedef FogDecoratedParseContext Super;
	typedef FogMetaParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMetaParseContext(const This& parseContext);
	This& operator=(const This& parseContext);
public:
	FogMetaParseContext(FogParseContext& parseContext);
	virtual const FogScopeContext* meta_context() const;
};

//
//    A FogTemplateParameterParseContext refines the context in response to a template <...> declaration prefix.
//    Constructed by FogRawTemplate::compile_parsed.
//
class FogTemplateParameterParseContext : public FogDecoratedParseContext, private FogTokenMonitor {
	typedef FogDecoratedParseContext Super;
	typedef FogTemplateParameterParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogRawRefToConst _declaration;                        //   The source declaration
	const FogTemplateParameterSpecifiersRefToConst _template_parameters;
public:
	FogTemplateParameterParseContext(FogParseContext& parseContext,
	                                 const FogTemplateParameterSpecifiers& templateParameters, const FogRaw& aDeclaration);
	virtual ~FogTemplateParameterParseContext();
	virtual const FogRaw& declaration() const;
	virtual const FogTemplateParameterSpecifiers& explicit_template_parameters() const;
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
};

//
//    The nested ParseContext - constructed by FogToken::compile_declaration.
//
class FogTokenParseContext : public FogDecoratedParseContext, private FogTokenMonitor {
	typedef FogDecoratedParseContext Super;
	typedef FogTokenParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogTokenParseContext(const This& scopeContext);
	This& operator=(const This& scopeContext);
public:
	FogTokenParseContext(FogParseContext& parseContext, const FogToken& aToken);
};

//
//    A FogUnbreakableParseContext is the starting point for the parse of a meta-condition statement.
//    (Invoked from FogIfStatement::compile_parsed)
//
class FogUnbreakableParseContext : public FogDecoratedParseContext, private FogTokenMonitor {
	typedef FogDecoratedParseContext Super;
	typedef FogUnbreakableParseContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogRawRefToConst _declaration;                            //   The source declaration
private:
	This& mutate() const { return *(This*)this; }
	virtual std::ostream& print_message(std::ostream& s) const;
public:
	FogUnbreakableParseContext(FogParseContext& parseContext, const FogRaw& aDeclaration);
	virtual ~FogUnbreakableParseContext();
	virtual const FogRaw& declaration() const;
};
#endif

