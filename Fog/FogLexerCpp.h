
#ifndef FOGLEXERCPP_HXX
#define FOGLEXERCPP_HXX




class FogLexerCpp : private FogLexerHash
{
 typedef FogLexerCpp This;
 typedef FogLexerHash Super;
 TYPEDECL_SINGLE(This, Super)
private:
 FogReader& _reader;       //   Reading context.
 FogLexerUnget _unget;      //   Tokens backed up in support of peek.
private:
 FogLexerCpp(const This&);
 FogLexerCpp& operator=(const This&);
private:
 virtual void hash_include(const PrimId& fileStem);
 void include_file(const FogSourceFile& sourceFile, const FogUtility& aUtility);
 virtual const FogSourceFile *make_source_file(const PrimId& fileName);
 virtual void popped_file(const FogUtility& utilityMode, const FogSourceFile *sourceFile);
 virtual const FogUtility& pushed_file(const FogUtility& utilityMode, const FogSourceFile& sourceFile);
protected:
 virtual void define(const PrimId& macroId, const PrimIdList *formalList, FogTokens& someTokens);
 virtual void undefine(const PrimId& anId);private:
public:
 FogLexerCpp(FogReader& aReader, std::istream& inputStream, const FogSourceFile& sourceFile);
 virtual ~FogLexerCpp();
 const char *get(FogTokenContext& tokenContext);
 void include(const PrimId& fileStem, const FogUtility& aUtility);
 virtual const FogMacro *is_defined(const PrimId& anId) const;
 const char *peek(FogTokenRef& tokenValue, bool skipWhite);
 using Super::print_message_line;
 using Super::print_this;
};
#endif
 
