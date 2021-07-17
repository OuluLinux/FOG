
#ifndef FOGLEXERCONTEXT_HXX
#define FOGLEXERCONTEXT_HXX





struct FogLexerContext_Element;

class FogLexerContext : public PrimPrint {
	typedef PrimPrint Super;
	typedef FogLexerContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	PrimAdopted<FogLexerContext_Element> _context;
private:
	FogLexerContext(const This&);
	This& operator=(const This&);
private:
	virtual void popped_file(const FogUtility& utilityMode, const FogSourceFile* sourceFile) = 0;
	virtual const FogUtility& pushed_file(const FogUtility& utilityMode, const FogSourceFile& sourceFile) = 0;
protected:
	FogLexerContext(const FogSourceFile& sourceFile);
	virtual ~FogLexerContext();
	void advance_line();
	void advance_token(size_t extraTabCols);
	FogLine& current_line() const;
	size_t detabbed_col() const;
	int get_line(std::istream& yyin, char* buffer, int max_size);
	struct yy_buffer_state* pop_buffer();
	std::istream* push_buffer(const PrimId& sourceId, PrimAdopted<std::istream>& aStream,
	                          struct yy_buffer_state* currentBuffer, const FogUtility& utilityMode, const FogSourceFile& sourceFile);
	void set_line(const FogSourceFile* sourceFile, size_t lineNumber);
public:
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
};
#endif

