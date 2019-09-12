
#ifndef FOGCODETOKENS_HXX
#define FOGCODETOKENS_HXX




class FogCodeTokens : public FogToken
{
    typedef FogToken Super;
    typedef FogCodeTokens This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_DERIVED_DECLS
    FOGTOKEN_READ_ONLY_DECLS
private:
    FogTokenListOfRef _tokens;
    size_t _source_lines;                           //   Number of FogTokenType::Line tokens in _tokens.
    size_t _executable_tokens;                      //   Number of non-whitespace tokens in _tokens 
private:
    FogCodeTokens(const This&);
    FogCodeTokens& operator=(const This&);
protected:
    virtual ~FogCodeTokens();
public:
    FogCodeTokens();
    void add(FogToken& aToken);
//    void add(const FogTokenListOfRef& someConstTokens);
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual size_t executable_tokens() const;
    virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
//    virtual FogCodeTokens *is_const_tokens();
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual size_t source_columns() const;
    virtual size_t source_lines() const;
    size_t tally() const { return _tokens.tally(); }
    virtual FogTokenListOfRef& tokens() { return Super::tokens(); }
    virtual const FogTokenListOfRefToConst& tokens() const;
    friend void operator==(const FogCodeTokens& firstConstTokens, const FogCodeTokens& secondConstTokens);
//        { return &firstLine == &secondLine; }
};
#endif
 
