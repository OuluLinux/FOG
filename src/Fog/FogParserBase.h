
#ifndef FOGPARSERBASE_HXX
#define FOGPARSERBASE_HXX





class FogParserBase : public PrimPrint {
	typedef PrimPrint Super;
	typedef FogParserBase This;
	TYPEDECL_SINGLE(This, Super)
	
private: 
       //   Level 5 to 6 context
	const char *_title;       //   Name of parsing target.
	FogLexerTower& _lexer;      //   Tower of lexing stages.
	FogScope& _saved_scope;      //   Cached _lexer.scope().
	const FogUtility& _saved_utility_mode;  //   Cached _lexer.utility_mode().
	FogTokenContext _primed[2];     //   Primed next tokens (for almost instant consumption)
	const int _depth;       //   Depth of nested parsers
	
private:
        //   Level 6 to 7 context
	bool _ws_prefix;       //   WhiteSpace or Line precedes _lookahead
	FogTokenContext _lookahead;     //   Current token and origin - albeit a lookahead for parser.
	FogLineRef _line;       //   Most recent non-null line context.
	FogAccessHandle _access;     //   Default access for nested declarations.
	size_t _nest;        //   Declaration nesting depth
	FogToken *_result;       //   Scratch location to pass result out of parser.
	FogTargetFile *_target_file;    //   Cached target file for _lookahead.
	
private: 
       //   Level 5 to 6 interface
	void get_nonstring_concatenation(FogTokenContext& tokenContext);
	void get_string_concatenation(FogTokenContext& tokenContext);
	const char *peek(FogTokenRef& peekedValue, bool skipWhite = false);
	bool peek_for_nonstring_concatenation();
	bool peek_for_string_concatenation();
	virtual int yy_debug() const = 0;
	const char *get(FogTokenContext& tokenContext);
	
protected:
        //   Level 5 to 7 interface
	FogParserBase(FogLexerTower& aLexer, FogToken& firstToken, const char *aTitle, int aDepth);
	virtual ~FogParserBase();
	void check_lookahead(int yyChar);
	void garbage_add(const FogToken *aToken);
	void garbage_adopt(const FogToken *aToken);
	FogScope& global_scope();
	void include(const PrimId& fileStem, const FogUtility *aUtility);
	void queue(FogToken& firstToken);
	FogScope& scope();
	void unget(bool hasLookAhead);
	
protected:
        //   Level 6 to 7 interface
public:
	FogRaw *compile_declaration(FogUtility *utilityMode, FogRaw *aDeclaration);
protected:
	FogRaw *compile_statement(FogRaw *aDeclaration);
	void diagnostic(const char *msg);
	void error(char *msg);
	FogLine& make_current_line();
	void *mark(bool hasLookAhead);
	size_t nest() {
		return _nest++;
	}
	
	void raw_error(char *msg);
	void remark(void *);
	void set_result(FogToken *aResult, bool hasLookAhead);
	void unmark(const FogToken *aToken, bool hasLookAhead);
	void unnest(size_t aNest) {
		_nest = aNest;
	}
	
	void warning(const char *msg);
	FogToken *yylex();
	
public:
	virtual std::ostream& print_this(std::ostream& s) const;
	FogToken *result() const {
		return _result;
	}
	
	void set_nested_access(const FogAccess *anAccess);
	void set_utility_mode(const FogUtility& utilityMode);
	const char *title() const {
		return _title;
	}
	
	const FogUtility& utility_mode() const;
};

#endif

