
#ifndef FOGTOKEN_HXX
#define FOGTOKEN_HXX








class FogToken : public PrimPrint, public FogScopeEnums {
	typedef PrimPrint Super;
	typedef FogToken This;
	TYPEDECL_SINGLE(This, Super)
	PRIMSHARES_DECLS(This)
	PRIMREF_DECLS(This)
private:
	This& operator=(const This& aToken);
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogToken() {}
	FogToken(const This& aToken) {}
	virtual ~FogToken() {}
	virtual FogToken* clone() const = 0;
	virtual bool compile_parsed(FogParseContext& parseContext);
	FogToken* error_clone() const;
	void error_merge(FogMergeContext& mergeContext, const FogToken& aToken) const;
	const FogMerge& error_needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const;
	virtual void make_actual_from(FogMakeActualContext& makeActualContext) {}
	virtual void make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const;
	virtual void merge(FogMergeContext& mergeContext, const FogToken& aToken) {}
	void merge_from(FogMergeContext& mergeContext, const This& aToken) {}
	bool merge_bool(const char* someText, bool thisBool, bool thatBool) const;
	const PrimId& merge_id(const char* someText, const PrimId& thisId, const PrimId& thatId) const;
	int merge_int(const char* someText, int thisInt, int thatInt) const;
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const
	{ return FogMerge::both_valid(); }
public:
	virtual void add_use_by(FogEntity& anEntity, const FogOfUseBy& ofUseBy);
	virtual FogMetaEntity* adopt_meta_entity(FogMetaEntity* metaEntity);
	virtual const FogMetaSlot* adopt_slot(const FogMetaSlot* aSlot);
	virtual bool compile_declaration(FogParseContext& parseContext) const;
	virtual bool compile_parsed_case(FogParseContext& parseContext, const FogToken& theCase);
	virtual void create_usage(FogUsageContext& usageContext) const {}
	virtual void create_usages() {}
	virtual void destroy();
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool emit_case(FogEmitContext& emitContext, const FogToken& theCase) const;
	virtual size_t executable_tokens() const;
	virtual void find_entities(FogEntityFinder& theFinder);
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual FogEntity* find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
	FogEntity* find_entity_in(FogScopeContext& inScope) const { return find_entity_in(inScope, FIND_ENTITY); }
	virtual const FogMetaSlot* find_local_slot(const PrimId& anId) const;
	virtual FogMetaEntity* find_meta_entity(const PrimId& anId);
	FogEntity* find_name_in(FogScopeContext& inScope) const { return find_entity_in(inScope, FIND_NAME); }
	FogEntity* find_signature_in(FogScopeContext& inScope) const { return find_entity_in(inScope, FIND_SIGNATURE); }
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	bool find_slots(FogMetaSlotFinder& theFinder) const { return mutate().find_slots(theFinder); }  //  .bugbug cast
	FogEntity* find_type_in(FogScopeContext& inScope) const { return find_entity_in(inScope, FIND_TYPE); }
	virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
	virtual FogEntity* get_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
	FogEntity* get_entity_in(FogScopeContext& inScope) const { return get_entity_in(inScope, FIND_ENTITY); }
	virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                         FogTokenType::TokenType monadicOp) const;
	FogEntity* get_name_in(FogScopeContext& inScope) const { return get_entity_in(inScope, FIND_NAME); }
	virtual const PrimNumber& get_number(FogScopeContext& inScope) const;
	virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	FogEntity* get_signature_in(FogScopeContext& inScope) const { return get_entity_in(inScope, FIND_SIGNATURE); }
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual FogName* get_type();
	const FogName* get_type() const { return mutate().get_type(); }
	FogEntity* get_type_in(FogScopeContext& inScope) const { return get_entity_in(inScope, FIND_TYPE); }
	virtual FogRoot& global_scope();
	const FogRoot& global_scope() const { return mutate().global_scope(); }
	virtual const FogMerge& impossible_merge() const;
	virtual FogScope& inner_scope();
	const FogScope& inner_scope() const { return mutate().inner_scope(); }
	virtual void install(FogInstallContext& installContext) const {}
	virtual bool is_actual(const FogScopeContext& scopeContext) const { return true; }
	virtual const FogArrayModifier* is_array_modifier() const;
	virtual FogBaseClass* is_base_class();
	virtual const FogSpacing* is_brace() const;
	virtual FogRaw* is_case(FogScopeContext& scopeContext, const FogToken& theCase);
	const FogRaw* is_case(FogScopeContext& scopeContext, const FogToken& theCase) const
	{ return mutate().is_case(scopeContext, theCase); }
	virtual const FogCharacter* is_character() const;
	virtual FogClass* is_class();
	FogClass* is_class() const { return mutate().is_class(); }
	virtual bool is_compound() const;
	virtual FogDerivable* is_derivable();
	const FogDerivable* is_derivable() const { return mutate().is_derivable(); }
	virtual FogEntity* is_entity();
	const FogEntity* is_entity() const { return mutate().is_entity(); }
	virtual FogEnum* is_enum();
	const FogEnum* is_enum() const { return mutate().is_enum(); }
	virtual const FogEnumSpecifier* is_enum_specifier() const;
	virtual FogEnumerator* is_enumerator();
	virtual const FogEnumeratorSpecifier* is_enumerator_specifier() const;
	virtual IsExposed is_exposed() const;
	virtual FogExpr* is_expr();
	const FogExpr* is_expr() const { return mutate().is_expr(); }
	virtual FogFailure* is_failure();
	const FogFailure* is_failure() const { return mutate().is_failure(); }
	virtual FogFunction* is_function();
	const FogFunction* is_function() const { return mutate().is_function(); }
	virtual const FogFunctionModifier* is_function_modifier() const;
	virtual FogIterationDomain* is_iteration_domain();
	const FogIterationDomain* is_iteration_domain() const { return mutate().is_iteration_domain(); }
	virtual FogLine* is_line() const;
	virtual IsList is_list() const;
	virtual FogListOfExpr* is_list_of_expr();
	const FogListOfExpr* is_list_of_expr() const { return mutate().is_list_of_expr(); }
	virtual FogMetaEntity* is_meta_entity();
	const FogMetaEntity* is_meta_entity() const { return mutate().is_meta_entity(); }
	virtual FogMetaFunctionEntity* is_meta_function_entity();
	virtual const FogMetaFunctionSlot* is_meta_function_slot() const;
	virtual FogMetaIteratorEntity* is_meta_iterator_entity();
	const FogMetaIteratorEntity* is_meta_iterator_entity() const { return mutate().is_meta_iterator_entity(); }
	virtual const FogMetaSlot* is_meta_slot() const;
	virtual FogMetaVariableEntity* is_meta_variable_entity();
	virtual const FogMetaVariableSlot* is_meta_variable_slot() const;
	virtual FogMetaType* is_meta_type();
	const FogMetaType* is_meta_type() const { return mutate().is_meta_type(); }
	virtual FogModifier* is_modifier();
	const FogModifier* is_modifier() const { return mutate().is_modifier(); }
	virtual FogName* is_name();
	const FogName* is_name() const { return mutate().is_name(); }
	virtual FogClass* is_namespace();
	const FogClass* is_namespace() const { return mutate().is_namespace(); }
	virtual FogNumber* is_number();
	const FogNumber* is_number() const { return mutate().is_number(); }
	virtual FogPotentialDeclaration* is_potential_declaration();
	const FogPotentialDeclaration* is_potential_declaration() const { return mutate().is_potential_declaration(); }
	virtual FogRaw* is_raw();
	const FogRaw* is_raw() const { return mutate().is_raw(); }
	virtual FogRaws* is_raw_list();
	const FogRaws* is_raw_list() const { return mutate().is_raw_list(); }
	virtual FogScope* is_scope();
	const FogScope* is_scope() const { return mutate().is_scope(); }
	bool is_skip(bool skipWhite) const { return skipWhite && (is_white() != 0); }
	virtual const FogSpacing* is_spacing() const;
	virtual FogSpecifier* is_specifier();
	const FogSpecifier* is_specifier() const { return mutate().is_specifier(); }
	virtual const FogString* is_string() const;
	virtual FogTemplateParameterSpecifier* is_template_parameter_specifier();
	const FogTemplateParameterSpecifier* is_template_parameter_specifier() const
	{ return mutate().is_template_parameter_specifier(); }
	FogToken* is_token() { return is_null() ? 0 : this; }
	const FogToken* is_token() const { return mutate().is_token(); }
	virtual FogTokens* is_tokens();
	const FogTokens* is_tokens() const { return mutate().is_tokens(); }
	virtual FogTypedef* is_typedef();
	const FogTypedef* is_typedef() const { return mutate().is_typedef(); }
	virtual FogVariable* is_variable();
	const FogVariable* is_variable() const { return mutate().is_variable(); }
	virtual const FogToken* is_white() const;
	virtual bool is_wide() const;
	virtual const PrimId* is_word() const;
	virtual FogLine& line() const;
	void make_actual(PrimRef<This>& actualToken, FogMakeActualContext& makeActualContext) const
	{ make_actual((PrimRefToConst<This>&)actualToken, makeActualContext); } /*   .bugbug cast */
	void make_actual(PrimRefToConst<This>& actualToken, FogMakeActualContext& makeActualContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	void merge_into(FogMergeContext& mergeContext, PrimRefToConst<This>& aToken) const;
	virtual const FogMetaType& meta_type() const;
	virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	                      FogScopeContext& inScope) const;
	virtual FogScope& name_scope();
	const FogScope& name_scope() const { return mutate().name_scope(); }
	virtual FogScope& name_space();
	const FogScope& name_space() const { return mutate().name_space(); }
	virtual const FogMerge& needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const;
	virtual FogNumber& number();
	const FogNumber& number() const { return mutate().number(); }
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_stringized(std::ostream& s) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_tokens(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_exposed();
	virtual void set_line(FogLine& aLine);
	virtual size_t source_columns() const;
	virtual size_t source_lines() const;
	virtual const PrimId& short_id() const;
	virtual FogTokenType::TokenType token_type_enum() const;
	virtual FogTokenListOfRef& tokens();
	virtual const FogTokenListOfRefToConst& tokens() const;
	virtual const FogUtility& utility() const;
public:
	static void merge_short_into(FogMergeContext& mergeContext,
	                             PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList);
	static void merge_long_into(FogMergeContext& mergeContext,
	                            PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList);
	static const FogMerge& needs_merge_exact(FogMergeContext& mergeContext,
	        const PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList);
};

#define FOGTOKEN_DERIVED_DECLS \
	public: \
	void make_actual(PrimRef<This>& actualToken, FogMakeActualContext& makeActualContext) const \
	{ make_actual((PrimRefToConst<This>&)actualToken, makeActualContext); } /*   .bugbug cast */ \
	void make_actual(PrimRefToConst<This>& actualToken, FogMakeActualContext& makeActualContext) const \
	{ Super::make_actual((PrimRefToConst<Super>&)actualToken, makeActualContext); } \
	void merge_into(FogMergeContext& mergeContext, PrimRefToConst<This>& aToken) const \
	{ Super::merge_into(mergeContext, (PrimRefToConst<Super>&)aToken); } \
	static void merge_short_into(FogMergeContext& mergeContext, \
	                             PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList) \
	{ Super::merge_short_into(mergeContext, (PrimListOfRefToConst<Super>&)intoList, \
		                      (PrimListOfRefToConst<Super>&)fromList); } \
	static void merge_long_into(FogMergeContext& mergeContext, \
	                            PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList) \
	{ Super::merge_long_into(mergeContext, (PrimListOfRefToConst<Super>&)intoList, \
		                     (PrimListOfRefToConst<Super>&)fromList); } \
	static const FogMerge& needs_merge_exact(FogMergeContext& mergeContext, \
	        const PrimListOfRefToConst<This>& intoList, const PrimListOfRefToConst<This>& fromList) \
	{ return Super::needs_merge_exact(mergeContext, (PrimListOfRefToConst<Super>&)intoList, \
		                              (PrimListOfRefToConst<Super>&)fromList); } \
	private:

#define FOGTOKEN_MEMBER_DECLS_LESS_EMIT \
	protected: \
	virtual void make_actual_from(FogMakeActualContext& makeActualContext); \
	void merge_from(FogMergeContext& mergeContext, const This& aToken); \
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const; \
	public: \
	virtual void create_usage(FogUsageContext& aContext) const; \
	virtual void install(FogInstallContext& installContext) const; \
	virtual bool is_actual(const FogScopeContext& scopeContext) const; \
	virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const; \
	private:

#define FOGTOKEN_MEMBER_DECLS FOGTOKEN_MEMBER_DECLS_LESS_EMIT \
	public: \
	virtual bool emit(FogEmitContext& emitContext) const; \
	private:

#define FOGTOKEN_LEAF_DECLS \
	protected: \
	virtual FogToken *clone() const; \
	virtual void merge(FogMergeContext& mergeContext, const FogToken& aToken); \
	public: \
	virtual const FogMerge& needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const; \
	private:

#define FOGTOKEN_READ_ONLY_DECLS \
	protected: \
	virtual FogToken *clone() const; \
	virtual void make_actual_from(FogMakeActualContext& makeActualContext); \
	public: \
	virtual bool is_actual(const FogScopeContext& scopeContext) const; \
	private:

#define FOGTOKEN_ACTUAL_DECLS \
	protected: \
	virtual FogToken *clone() const; \
	public: \
	virtual bool is_actual(const FogScopeContext& scopeContext) const; \
	private:
#define FOGTOKEN_UNIQUE_DECLS FOGTOKEN_ACTUAL_DECLS

#define FOGTOKEN_SHAREDLEAF_DECLS FOGTOKEN_LEAF_DECLS

#define FOGTOKEN_LEAF_IMPL(CLASS) \
	FogToken *CLASS::clone() const { return new This(*this); } \
	void CLASS::merge(FogMergeContext& mergeContext, const FogToken& aToken) \
	{ \
		bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info(); \
		if (sameClass) merge_from(mergeContext, (const This&)aToken); \
		else Super::merge(mergeContext, aToken); \
	} \
	const FogMerge& CLASS::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const \
	{ \
		bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info(); \
		return sameClass ? needs_merge_from(mergeContext, (const This&)aToken) \
		       : Super::needs_merge(mergeContext, aToken); \
	}

#define FOGTOKEN_SHAREDLEAF_IMPL(CLASS) \
	FogToken *CLASS::clone() const { share(); return (FogToken *)this; } \
	void CLASS::merge(FogMergeContext& mergeContext, const FogToken& aToken) \
	{ \
		bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info(); \
		if (!sameClass) Super::merge(mergeContext, aToken); \
	} \
	const FogMerge& CLASS::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const \
	{ \
		return Super::needs_merge(mergeContext, aToken); \
	}

#define FOGTOKEN_READ_ONLY_IMPL(CLASS) \
	FogToken *CLASS::clone() const { return new This(*this); }

#define FOGTOKEN_ACTUAL_IMPL(CLASS) \
	FogToken *CLASS::clone() const { return FogToken::error_clone(); } \
	bool CLASS::is_actual(const FogScopeContext& scopeContext) const { return true; }
#define FOGTOKEN_UNIQUE_IMPL(CLASS) FOGTOKEN_ACTUAL_IMPL(CLASS)

#define FOGTOKEN_NULL_LEAF \
	protected: \
	virtual FogToken *clone() const { share(); return (FogToken *)this; } \
	virtual void merge(FogMergeContext& mergeContext, const FogToken& aToken) { error_merge(mergeContext, aToken); } \
	public: \
	virtual const FogMerge& needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const \
	{ return error_needs_merge(mergeContext, aToken); } \
	private:

#endif

