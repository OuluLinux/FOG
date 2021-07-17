
#ifndef FOGEMITCONTEXT_HXX
#define FOGEMITCONTEXT_HXX


class FogEmitContext;
class FogStaticEmitContext;

class FogForUseAsEnums {
	friend class FogForUseAs;                   //   To access ForUseAsBits
private:
	enum ForUseAsBits {                         //   Enumerated format options for decl
		EMIT_NO_INIT = 1,                       //   Suppress default values for function/template
		EMIT_NO_SCOPE = 2,                      //   Suppress scope on declarator names (no effect on types)
		EMIT_EXCEPTIONS = 8,                    //   Include exception specificaton in name
		EMIT_NO_NAME = 16,                      //   Suppress declarator names (emitting type)
		EMIT_NO_PARAMETER_NAMES = 32,           //   Suppress parameter names
		EMIT_EXPRESSION = 64,                   //   Simple token emission in progress
		EMIT_RAW = 128,                         //   Raw token emission in progress (no #line's in diagnostics)
		EMIT_FULL_TYPES = 256                   //   Reference each type name with respect to ::
	};
public:
	enum ForUseAs {                             //   Enumerated format options for decl
		GLOBAL_IMPLEMENTATION = EMIT_NO_INIT,
		GLOBAL_INTERFACE = 0,
		LOCAL_INTERFACE = EMIT_NO_SCOPE,
		CONVERSION_INTERFACE = EMIT_NO_SCOPE | EMIT_NO_NAME | EMIT_NO_PARAMETER_NAMES,
		TYPE_INTERFACE = EMIT_NO_NAME | EMIT_NO_PARAMETER_NAMES,
		ABSTRACT_SIGNATURE = EMIT_NO_NAME | EMIT_NO_SCOPE | EMIT_NO_INIT,
		GLOBAL_SIGNATURE = EMIT_NO_INIT | EMIT_NO_PARAMETER_NAMES | EMIT_FULL_TYPES,
		LOCAL_SIGNATURE = EMIT_NO_SCOPE | GLOBAL_SIGNATURE,
		TYPE_SIGNATURE = EMIT_NO_SCOPE | EMIT_NO_NAME | EMIT_NO_PARAMETER_NAMES | EMIT_FULL_TYPES,
		EXPRESSION = EMIT_EXPRESSION | EMIT_NO_SCOPE,
		RAW = EMIT_RAW | EXPRESSION,
		CONVERSION_TYPE_ID = GLOBAL_INTERFACE,
		DESTRUCTOR_ID = LOCAL_INTERFACE,
		NO_INIT = EMIT_NO_INIT,
		NO_SCOPE = EMIT_NO_SCOPE,
		NO_NAME = EMIT_NO_NAME
	};
};

class FogForUseAs : public FogForUseAsEnums {
private:
	ForUseAs _for_use_as;                       //   Formatting constraints.
protected:
	FogForUseAs& operator=(const FogForUseAs& forUseAs) { _for_use_as = forUseAs._for_use_as; return *this; }
public:
	FogForUseAs(ForUseAs forUseAs) : _for_use_as(forUseAs) {}
	void operator|=(ForUseAs b) { _for_use_as = ForUseAs(_for_use_as | b); }
	void operator|=(const FogForUseAs& b) { _for_use_as = ForUseAs(_for_use_as | b._for_use_as); }
	bool emit_declarator() const { return (_for_use_as & EMIT_NO_NAME) == 0; }
	bool emit_declarator_scope() const { return (_for_use_as & EMIT_NO_SCOPE) == 0; }
	bool emit_default_initialisers() const { return (_for_use_as & (EMIT_NO_INIT)) == 0; }
	bool emit_exceptions() const { return (_for_use_as & EMIT_EXCEPTIONS) != 0; }
	bool emit_expression() const { return (_for_use_as & EMIT_EXPRESSION) != 0; }
	bool emit_full_types() const { return (_for_use_as & EMIT_FULL_TYPES) != 0; }
	bool emit_parameter_names() const { return (_for_use_as & EMIT_NO_PARAMETER_NAMES) == 0; }
	bool emit_raw() const { return (_for_use_as & EMIT_RAW) != 0; }
	bool emit_type() const { return (_for_use_as & EMIT_NO_NAME) != 0; }
	const FogForUseAs& for_use_as() const { return *this; }
	friend inline FogForUseAs operator|(const FogForUseAs& a, const FogForUseAs& b) {
		return ForUseAs(a._for_use_as | b._for_use_as);
	}
};

//
//    A FogEmitContext defines the abstract interface for the context in which to perform code emission.
//
class FogEmitContext : public FogScopeContext, public FogForUseAsEnums {
	typedef FogScopeContext Super;
	typedef FogEmitContext This;
	TYPEDECL_SINGLE(This, Super)
	friend class FogDecoratedEmitContext;       //   To access set_depth().
public:
	enum Columns { COLUMNS };
	enum Indents { INDENTS };
private:
	FogEmitContext(const This& aContext);
	This& operator=(const This& aContext);
protected:
	FogEmitContext() {}
	virtual ~FogEmitContext() {}
	virtual size_t set_depth(size_t numColumns, bool isRelative) = 0;
	virtual const char* set_separator(const char* elementSeparator) = 0;
	virtual const char* set_terminator(const char* elementTerminator) = 0;
public:
	const This& as_const() const { return *this; }
	virtual bool do_jump() const;
	//      virtual void emit_bool(bool aNumber) = 0;
	virtual void emit_character(const PrimId& anId, bool isWide) = 0;
	virtual FogStaticEmitContext& emit_context() = 0;
	bool emit_cv(const FogDeclSpecifierValue& declSpecifiers);
	bool emit_declarator() const { return for_use_as().emit_declarator(); }
	bool emit_declarator_scope() const { return for_use_as().emit_declarator_scope(); }
	bool emit_default_initialisers() const { return for_use_as().emit_default_initialisers(); }
	bool emit_exceptions() const { return for_use_as().emit_exceptions(); }
	bool emit_expression() const { return for_use_as().emit_expression(); }
	bool emit_full_types() const { return for_use_as().emit_full_types(); }
	virtual bool emit_hash_line(const FogLine& aLine) = 0;
	bool emit_id(const PrimId& anId, const FogToken& aToken);
	virtual bool emit_identifier(const PrimId& anId) = 0;
	virtual void emit_number(const FogNumber& aNumber) = 0;
	bool emit_parameter_names() const { return for_use_as().emit_parameter_names(); }
	bool emit_raw() const { return for_use_as().emit_raw(); }
	void emit_scope();
	void emit_scope(const FogScope& theScope);
	virtual void emit_spaces(size_t numSpaces) = 0;
	virtual bool emit_space_and_text(const char* aString) = 0;
	virtual void emit_string(const PrimId& anId, bool isWide) = 0;
	virtual void emit_text(const char* aString) = 0;
	bool emit_type() const { return for_use_as().emit_type(); }
	//    const FogTargetFile *file();
	virtual const FogForUseAs& for_use_as() const = 0;
	virtual FogEmitContext* is_emit_context();
	virtual const FogScope& name_space() const;
	//    FogStream& s();
	virtual void set_do_break();
	virtual void set_do_continue();
	virtual bool separate() = 0;
	virtual const char* separator() const = 0;
	virtual void start() const = 0;
	virtual const FogScope* stream_scope() const = 0;       //   Only called from FogScope::emit_template_prefix
	virtual bool terminate() = 0;
	virtual const char* terminator() const = 0;
};

//
//    A FogStaticEmitContext implements the context for code emission providing a stream for the output.
//
class FogStaticEmitContext : public FogEmitContext {
	typedef FogEmitContext Super;
	typedef FogStaticEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _static_token;                    //   Current token.
	const InScope _in_scope;                    //   Scoping with which a find is to be resolved.
	FogStream& _s;                              //   Output stream.
	const FogForUseAs _for_use_as;              //   Formatting constraints.
	const char* _separator;                     //   Inter element separation used by separate().
	const char* _terminator;                    //   Post element termination used by separate() and terminate().
private:
	FogStaticEmitContext(const This& aContext);
	This& operator=(const This& aContext);
protected:
	virtual size_t set_depth(size_t numColumns, bool isRelative);
	virtual const char* set_separator(const char* elementSeparator);
	virtual const char* set_terminator(const char* elementTerminator);
public:
	FogStaticEmitContext(FogStream& s, FogToken& aToken, ForUseAs forUseAs);
	virtual ~FogStaticEmitContext();
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	//      virtual void emit_bool(bool aNumber);
	virtual void emit_character(const PrimId& anId, bool isWide);
	virtual FogStaticEmitContext& emit_context();
	virtual bool emit_hash_line(const FogLine& aLine);
	virtual bool emit_identifier(const PrimId& anId);
	virtual void emit_number(const FogNumber& aNumber);
	virtual void emit_spaces(size_t numSpaces);
	virtual bool emit_space_and_text(const char* aString);
	virtual void emit_string(const PrimId& anId, bool isWide);
	virtual void emit_text(const char* aString);
	//    const FogTargetFile *file() { return _s.file(); }
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogForUseAs& for_use_as() const;
	virtual InScope in_scope() const;
	//    FogStream& s() { return _s; }
	virtual bool separate();
	virtual const char* separator() const;
	virtual void start() const;
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual const FogScope* stream_scope() const;
	virtual bool terminate();
	virtual const char* terminator() const;
};

//  inline const FogTargetFile *FogEmitContext::file() { return emit_context().file(); }
//  inline FogStream& FogEmitContext::s() { return emit_context().s(); }

//
//    A FogDecoratedEmitContext provides the generic Decorator behaviour for derived classes that adjust
//    the emission context temporaily.
//
class FogDecoratedEmitContext : public FogEmitContext {
	typedef FogEmitContext Super;
	typedef FogDecoratedEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogEmitContext& _context;               //   Context that this decorates.
private:
	FogDecoratedEmitContext(const This& emitContext);
	This& operator=(const This& emitContext);
protected:
	FogDecoratedEmitContext(FogEmitContext& emitContext) : _context(emitContext) {}
	virtual size_t set_depth(size_t numColumns, bool isRelative);
	virtual const char* set_separator(const char* elementSeparator);
	virtual const char* set_terminator(const char* elementTerminator);
public:
	FogEmitContext& get_context() {return _context;}
	
	virtual bool do_jump() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	//      virtual void emit_bool(bool aNumber);
	virtual void emit_character(const PrimId& anId, bool isWide);
	virtual FogStaticEmitContext& emit_context();
	virtual bool emit_hash_line(const FogLine& aLine);
	virtual bool emit_identifier(const PrimId& anId);
	virtual void emit_number(const FogNumber& aNumber);
	virtual void emit_spaces(size_t numSpaces);
	virtual bool emit_space_and_text(const char* aString);
	virtual void emit_string(const PrimId& anId, bool isWide);
	virtual void emit_text(const char* aString);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual const FogForUseAs& for_use_as() const;
	virtual InScope in_scope() const;
	virtual const FogScopeContext* meta_context() const;
	virtual const FogScope& name_space() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual Resolution resolution() const;
	virtual bool separate();
	virtual const char* separator() const;
	virtual void set_do_break();
	virtual void set_do_continue();
	virtual void start() const;
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	virtual const FogScope* stream_scope() const;
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
	virtual bool terminate();
	virtual const char* terminator() const;
};

//
//    A FogBreakableEmitContext is the starting point for the parse of a meta-iteration/switch statement.
//    (Invoked from FogDoWhileStatement/FogForStatement/FogSwitchStatement/FogWhileStatement::emit)
//
class FogBreakableEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogBreakableEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	bool _do_break;
	bool _do_continue;
public:
	FogBreakableEmitContext(FogEmitContext& emitContext, const FogRaw& aDeclaration);
	virtual ~FogBreakableEmitContext();
	bool do_break() const { return _do_break; }
	bool do_continue() const { return _do_continue; }
	virtual bool do_jump() const;
	virtual void set_do_break();
	virtual void set_do_continue();
};

//
//    A FogForUseAsEmitContext decorates a FogEmitContext to change the emission strategy.
//
class FogForUseAsEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogForUseAsEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogForUseAs _for_use_as;              //   Formatting constraints.
private:
	FogForUseAsEmitContext(const This&);
	This& operator=(const This&);
public:
	FogForUseAsEmitContext(FogEmitContext& emitContext, const FogForUseAs& forUseAs);
	virtual const FogForUseAs& for_use_as() const;
};

//
//    A FogIndentEmitContext decorates a FogEmitContext to add indentation.
//
class FogIndentEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogIndentEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	size_t _saved_depth;
private:
	FogIndentEmitContext(const This&);
	This& operator=(const This&);
public:
	FogIndentEmitContext(FogEmitContext& emitContext, int extraIndents, FogEmitContext::Indents);
	FogIndentEmitContext(FogEmitContext& emitContext, int extraColumns, FogEmitContext::Columns);
	virtual ~FogIndentEmitContext();
};

//
//    A FogNamespaceEmitContext decorates a FogEmitContext to add a deviant emission namespace, as
//    required for the return type scope of a function declared inline within a class..
//
class FogNamespaceEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogNamespaceEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogScope& _name_space;
private:
	FogNamespaceEmitContext(const This&);
	This& operator=(const This&);
public:
	FogNamespaceEmitContext(FogEmitContext& emitContext, const FogScope& nameSpace);
	virtual ~FogNamespaceEmitContext();
	virtual const FogScope& name_space() const;
};

//
//    A FogNestedEmitContext decorates a FogEmitContext to emission within a nested scope.
//
class FogNestedEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogNestedEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _dynamic_token;                           //   Current token.
private:
	FogNestedEmitContext(const This& emitContext);
	This& operator=(const This& emitContext);
public:
	FogNestedEmitContext(FogEmitContext& emitContext, FogToken& inToken);
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual InScope in_scope() const;
	virtual const FogScope& name_space() const;
};

//
//    A FogSeparatedEmitContext decorates a FogEmitContext to define an inter element separator..
//
class FogSeparatedEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogSeparatedEmitContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const char* _saved_separator;
	const char* _saved_terminator;
private:
	FogSeparatedEmitContext(const This&);
	This& operator=(const This&);
public:
	FogSeparatedEmitContext(FogEmitContext& emitContext, const char* elementSeparator, const char* elementTerminator);
	virtual ~FogSeparatedEmitContext();
};

//
//    A FogUnbreakableEmitContext is the starting point for the parse of a meta-condition statement.
//    (Invoked from FogIfStatement::emit)
//
class FogUnbreakableEmitContext : public FogDecoratedEmitContext {
	typedef FogDecoratedEmitContext Super;
	typedef FogUnbreakableEmitContext This;
	TYPEDECL_SINGLE(This, Super)
public:
	FogUnbreakableEmitContext(FogEmitContext& emitContext, const FogRaw& aDeclaration);
	virtual ~FogUnbreakableEmitContext();
};
#endif

