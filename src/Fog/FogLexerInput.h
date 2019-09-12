
#ifndef FOGLEXERINPUT_HXX
#define FOGLEXERINPUT_HXX






class FogLexerInput_Mark;

class FogLexerInput : public PrimMonitor
{
 typedef PrimMonitor Super;
 typedef FogLexerInput This;
 TYPEDECL_SINGLE(This, Super)
private:
 const FogToken& _token;       //   Invoking context.
 FogTokenListOfRefToConst _garbage;    //   Tokens to garbage collect
 PrimAdopted<FogLexerInput_Mark> _mark;   //   Stack of marked contexts. Current context always at top.
private:
 FogLexerInput(const This&);
 This& operator=(const This&);
 void garbage_collect(size_t garbageThreshold);
 This& mutate() const { return *(This *)this; }
protected:
 FogLexerInput(const FogToken& aToken);
 virtual ~FogLexerInput();
public:
 void add_garbage(const FogToken& aToken) { _garbage.add(aToken); }
 void adopt_garbage(const FogToken& aToken) { _garbage.adopt(aToken); }
 const FogMetaSlot *find(const PrimId& anId) const;
 const char *get(FogTokenContext& tokenContext);
 void mark();
 const char *peek(FogTokenRef& tokenValue, bool skipWhite);
 virtual bool post_error(const PrimNotice_Error& aNotice) const;
 virtual bool post_warning(const PrimNotice_Warning& aNotice) const;
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual void print_token(const char *aState, const FogToken *aValue) const = 0;
 void push_include(const PrimId& fileStem, const FogUtility& aUtility);
 void push_stream(FogReader& aReader, std::istream& inputStream, const FogSourceFile& sourceFile);
 void push_token(const FogToken& inToken, FogToken& theToken);
 void push_tokens(const FogToken& aToken, const FogTokenListOfRef& tokenValues);
 void remark();
 void unget(const FogTokenContext& tokenContext, bool wsNeeded);
 void unmark();
};
#endif
 
