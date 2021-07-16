
#ifndef FOGENTITYFINDER_HXX
#define FOGENTITYFINDER_HXX







class FogEntityFindingElement;
typedef PrimMapOfRef<FogEntityFindingElement> FogEntityFindingElementSetOfRef;

class FogEntityFinding : public PrimPrint, public FogScopeEnums
{
    typedef PrimPrint Super;
    typedef FogEntityFinding This;
    TYPEDECL_SINGLE(This, Super)
private:
    enum IsFound {};                        //   Useless type for pointer return.
private:
                                    //   Defining contents.
    const FindStrategy _strategy;           //   Category of entity to be found.
private:
                                    //   Defining contents.
    FogEntity *_entity;                     //   The first found entity.
    const PrimId *_entity_id;               //   The identity of the unambiguous token.
    bool _entity_dominatable;               //   True if _entity could be dominated..
    FogEntityFindingElementSetOfRef _found; //   Map of all found entities (iff more than one).
    bool _dominations_pruned;               //   Set true by invocation of resolve_dominations().
private:
    This& mutate() const { return *(This *)this; }
    void resolve_dominations();
public:
    FogEntityFinding(FindStrategy  aStrategy);
    virtual ~FogEntityFinding();
    operator const IsFound *() const { return (const IsFound *)(_entity != 0 ? this : 0); } 
    bool operator!() const { return _entity == 0; } 
    void add(FogEntity& anEntity, bool isDominatable = false);
//    const FogEntityMapOfRefToConst& findings() const { return _found; }
    FogEntity *found() { return _entity; }
    const FogEntity *found() const { return _entity; }
    FogEntity *get_unambiguous_finding(const FogToken& aToken, const FogScopeContext& inScope);
    FogEntity *get_unambiguous_finding(const PrimId& anId, const FogScope& inScope);
    bool is_ambiguous() const;
    bool is_found() const { return _entity != 0; }
    ResolveAlias is_resolve_alias() const { return _strategy == FIND_SCOPE ? RESOLVE_ALIAS : KEEP_ALIAS; }
    bool is_unambiguous() const { return _entity_id != 0; }
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    FindStrategy strategy() const { return _strategy; }
public:
    static const char *strategy_text(FindStrategy aStrategy);
};

class FogEntityFinder : public PrimPrint, public FogScopeEnums
{
    typedef PrimPrint Super;
    typedef FogEntityFinder This;
    TYPEDECL_SINGLE(This, Super)
private:
    enum IsFound {};                            //   Useless type for pointer return.
private:
    const PrimIdHandle _id;                     //   Untemplated name of declarator.
    const InScope _in_scope;                    //   Scoping with which a find is to be resolved.
    FogEntityFinding& _finding;                 //   What has been found.
    PrimIdMap _visited;                         //   What has been visited.
    bool _is_dominatable;                       //   True if finds are to be marked as dominatable.
public:
    FogEntityFinder(FogEntityFinding& theFinding, const PrimId& anId, InScope inScope);
    FogEntityFinder(FogEntityFinding& theFinding, const FogMakerContext& makerContext);
    FogEntityFinder(FogEntityFinding& theFinding, const FogEntityFinder& aFinder);
    virtual ~FogEntityFinder();
    operator const IsFound *() const { return (const IsFound *)(_finding != 0 ? this : 0); }    
    bool operator!() const { return !_finding; }    
    void add_find(FogEntity& anEntity) { _finding.add(anEntity, _is_dominatable); }
    bool add_visit(FogEntity& anEntity) { return _visited.add_filtered(anEntity.unique_id()); }
//    const FogEntityMapOfRefToConst& findings() const { return _finding.findings(); }
    FogEntity *found() const { return _finding.found(); }
    const PrimId& id() const { return *_id; }
    InScope in_scope() const { return _in_scope; }
    bool is_ambiguous() const { return _finding.is_ambiguous(); }
    bool is_found() const { return _finding.is_found(); }
    ResolveAlias is_resolve_alias() const { return _finding.is_resolve_alias(); }
    bool is_unambiguous() const { return _finding.is_unambiguous(); }
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    bool set_dominatable(bool isDominatable = true)
        { bool wasDominatable = _is_dominatable; _is_dominatable = isDominatable; return wasDominatable; }
    FindStrategy strategy() const { return _finding.strategy(); }
};
#endif
 
