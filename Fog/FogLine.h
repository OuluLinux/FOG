
#ifndef FOGLINE_HXX
#define FOGLINE_HXX




class FogLine : public FogToken {
	typedef FogToken Super;
	typedef FogLine This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	const FogSourceFile& _file;
	const size_t _number;
	const PrimStringHandle _text;
private:
	FogLine(const This&);
	This& operator=(const This&);
private:
	FogLine(const FogSourceFile& sourceFile, size_t lineNumber, const PrimString& sourceText);
	This& mutate() const { return *(This*)this; }
protected:
	FogLine();
	virtual ~FogLine() {}
public:
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual size_t executable_tokens() const;
	const FogSourceFile& file() const { return _file; }
	virtual FogLine* is_line() const;
	virtual const FogToken* is_white() const;
	virtual FogLine& line() const;
	size_t line_number() const { return _number; }
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual size_t source_lines() const;
	const PrimString& text() const { return *_text; }
	virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
	virtual size_t source_columns() const;
	virtual FogTokenType::TokenType token_type_enum() const;
public:
	static FogLine* new_line(const FogSourceFile* sourceFile, size_t lineNumber, const PrimString& sourceText);
public:
	friend bool operator==(const FogLine& firstLine, const FogLine& secondLine)
	{ return &firstLine == &secondLine; }
	friend bool operator!=(const FogLine& firstLine, const FogLine& secondLine)
	{ return &firstLine != &secondLine; }
};
#endif

