
#ifndef FOGMETASLOTFINDER_HXX
#define FOGMETASLOTFINDER_HXX










class FogMetaSlotFinding : private PrimPrint {
	typedef PrimPrint Super;
	typedef FogMetaSlotFinding This;
	TYPEDECL_SINGLE(This, Super)
private:
	enum IsFound {};      //   Useless type for pointer return.
private:         //   Defining contents.
	const FogMetaSlot* _slot;    //   The first found token.
	const PrimId* _id;      //   The identity of the unambiguous token.
	PrimIdMap _found;      //   Map of all found entities (iff more than one).
public:
	FogMetaSlotFinding();
	virtual ~FogMetaSlotFinding();
	operator const IsFound* () const { return (const IsFound*)(_slot != 0 ? this : 0); }
	bool operator!() const { return _slot == 0; }
	void add(const FogMetaSlot& metaSlot);
	const PrimIdMap& findings() const { return _found; }
	const FogMetaSlot* found() const { return _slot; }
	const FogMetaSlot* get_unambiguous_finding(const FogToken& aToken, const FogScopeContext& inScope) const;
	//  	const FogMetaSlot *get_unambiguous_finding(const PrimId& anId, const FogScope& inScope) const;
	bool is_ambiguous() const { return _found.tally() != 0; }
	bool is_found() const { return _slot != 0; }
	bool is_unambiguous() const { return _id != 0; }
};

class FogMetaSlotFinder : public PrimPrint, public FogScopeEnums {
	typedef PrimPrint Super;
	typedef FogMetaSlotFinder This;
	TYPEDECL_SINGLE(This, Super)
private:
	enum IsFound {};       //   Useless type for pointer return.
private:
	PrimIdHandle _id;       //   Untemplated name of declarator.
	InScope _in_scope;       //   Scoping with which a find is to be resolved.
	FogMetaSlotFinding& _finding;    //   What has been found.
	PrimIdMap _visited;       //   What has been visited.
	FogToken* _in_token;      //   Non-0 if _finding.found() non-0
public:
	FogMetaSlotFinder(FogMetaSlotFinding& theFinding, const PrimId& anId, InScope inScope);
	virtual ~FogMetaSlotFinder();
	operator const IsFound* () const { return (const IsFound*)(_finding != 0 ? this : 0); }
	bool operator!() const { return !_finding; }
	void add(FogToken& inToken, const FogMetaSlot& metaSlot);
	bool add_visit(FogEntity& anEntity) { return _visited.add_filtered(anEntity.unique_id()); }
	const PrimIdMap& findings() const { return _finding.findings(); }
	const FogMetaSlot* found_slot() const { return _finding.found(); }
	FogToken* found_token() { return _in_token; }
	const PrimId& id() const { return *_id; }
	InScope in_scope() const { return _in_scope; }
	bool is_ambiguous() const { return _finding.is_ambiguous(); }
	bool is_found() const { return _finding.is_found(); }
	bool is_unambiguous() const { return _finding.is_unambiguous(); }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void set_id(const PrimId& anId, InScope inScope);
};
#endif

