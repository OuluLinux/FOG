
#ifndef FOGRAWS_HXX
#define FOGRAWS_HXX




class FogRaws : public FogRaw
{
    typedef FogRaws This;
    typedef FogRaw Super;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_DERIVED_DECLS
    FOGTOKEN_MEMBER_DECLS
private:
    FogRawListOfRef _raws;
protected:
    FogRaws();
    virtual ~FogRaws();
public:
    void add(FogRaw& aRaw) { _raws.add(aRaw); }
    virtual bool compile_declaration(FogParseContext& parseContext) const;
    virtual bool compile_parsed_case(FogParseContext& parseContext, const FogToken& theCase);
    virtual bool emit_case(FogEmitContext& emitContext, const FogToken& theCase) const;
    virtual size_t executable_tokens() const;
 virtual bool is_compound() const;
    virtual IsExposed is_exposed() const;
    virtual FogRaws *is_raw_list();
    virtual FogLine& line() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    FogRawListOfRef& raws() { return _raws; }
    const FogRawListOfRefToConst& raws() const { return _raws; }
    virtual void set_explicit_instantiation();
    virtual size_t source_columns() const;
    virtual size_t source_lines() const;
};
#endif
 
