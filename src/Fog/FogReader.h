
#ifndef FOGREADER_HXX
#define FOGREADER_HXX






class FogReader : public PrimPrint
{
    typedef PrimPrint Super;
    typedef FogReader This;
    TYPEDECL_SINGLE(This, Super)
private:                                //   Defining context
    FogRoot& _global;                       //   Global namespace.
    FogMacroMapOfRefToConst _macros;        //   Current #define's
private:                                //   Dynamic context
    FogUtilityHandle _utility_mode;         //   Status for new entities.
    const FogSourceFile *_source_file;      //   Current source file.
    FogScope *_scope;                       //   Current scope.
    int _depth;                             //   Diagnostic indentation depth of nested parsers
private:                                //   Accrued context
    PrimIdMap _sources;                     //   Map of all source files.
private:
    FogReader(const This&);
    This& operator=(const This&);
private:
    void set_new_scope(FogScope& aScope);
    void set_new_utility_mode(const FogUtility& utilityMode);
public:
    FogReader(FogRoot& aRoot);
    virtual ~FogReader();
    void add_source(const PrimId& fileIdent) { _sources.add_filtered(fileIdent); }
    void define(const PrimId& macroId, const PrimIdList *formalList, FogTokens& someTokens);
    const FogSourceFile *find_source_file(const PrimId& fileName, const FogSourceFileType& sourceType);
    FogRoot& global() { return _global; }
    const FogMacro *is_defined(const PrimId& anId) const { return _macros.find(anId); }
    const FogSourceFile *make_source_file(const PrimId& fileName);
    bool parse(FogTokens& returnTokens, const char *aString);
    bool parse_tokens(FogTokenRef& returnToken, const FogToken& aToken);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    void print_token(const char *aState, const FogToken *aValue) const;
    FogScope& scope() { return *_scope; }
    const FogScope& scope() const { return *_scope; }
    void set_diagnostic_depth(size_t aDepth) { _depth = aDepth; }
    void set_max_utility_mode(const FogUtility& utilityMode, const FogSourceFile *sourceFile)
        { _source_file = sourceFile; if (utilityMode > utility_mode()) set_new_utility_mode(utilityMode); }
    void set_scope(FogScope& aScope) { if (&aScope != _scope) set_new_scope(aScope); }
    void set_utility_mode(const FogUtility& utilityMode)
        { if (utilityMode != utility_mode()) set_new_utility_mode(utilityMode); }
    void set_utility_mode(const FogUtility& utilityMode, const FogSourceFile *sourceFile)
        { _source_file = sourceFile; if (utilityMode != utility_mode()) set_new_utility_mode(utilityMode); }
    const FogSourceFile *source_file() const { return _source_file; }
    const PrimIdMap& sources() const { return _sources; }
    bool tokenize(FogTokens& returnTokens, const char *aString);
    void undefine(const PrimId& anId);
    const FogUtility& utility_mode() const { return *_utility_mode; }
};
#endif
 
