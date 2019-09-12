
#ifndef FOGLEXERMACRO_HXX
#define FOGLEXERMACRO_HXX






struct FogLexerMacro_Element;

class FogLexerMacro : private FogLexer {

	friend class FogHashParser;      //   To raw_unget/raw_get
	
	friend class FogLocateParser;     //   To raw_unget/raw_get
	typedef FogLexerMacro This;
	typedef FogLexer Super;
	TYPEDECL_SINGLE(This, Super)
	
protected:
	FogLexerUnget _unget;       //   Backed up token - NewLine at end of #directive or
	//    arbitrary token after bad function-like macro locate
	PrimAdopted<FogLexerMacro_Element> _stack;  //   Stack of macro invocations. Necessary to support rescan of
	//    macro substitutions at source level where part of text
	//     comes from multiple first substitutions.
	
private:
	FogLexerMacro(const This&);
	FogLexerMacro& operator=(const This&);
	
protected:
	FogLexerMacro(std::istream& inputStream, const FogSourceFile& sourceFile, std::ostream *outputStream);
	virtual ~FogLexerMacro();
	const char *get(FogTokenContext& tokenContext);
	bool hash_push(const PrimId& anId);
	using Super::include_file;
	void raw_get(FogTokenContext& tokenContext);
	void raw_unget(const FogTokenContext& tokenContext);
	using Super::set_line;
	using Super::set_truth;
	
public:
	void invoke_function(FogTokenListOfRef& replacedTokens, const FogMacro& aMacro,
			const FogMacroListOfRefToConst& locatedArguments);
	void invoke_object(FogTokenListOfRef& replacedTokens, const FogMacro& aMacro);
	virtual const FogMacro *is_defined(const PrimId& anId) const = 0;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	using Super::print_message_line;
	using Super::print_this;
};

#endif

