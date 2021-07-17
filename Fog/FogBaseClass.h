
#ifndef FOGBASECLASS_HXX
#define FOGBASECLASS_HXX







class FogBaseClass : public FogMetaToken {
	typedef FogMetaToken Super;
	typedef FogBaseClass This;
	TYPEDECL_SINGLE(FogBaseClass, Super)
	PRIMREF_DERIVED_DECLS(FogBaseClass)
	FOGTOKEN_LEAF_DECLS
	
private:
	//   Defining contents.
	FogScope& _base;      //   Base class.
	FogAccessHandle _access;
	FogVirtualHandle _virtual;
	
private:
	//   Derived contents.
	PrimIdHandle _base_id;
	bool _compiled;
	
private:
	const FogVirtual& get_virtual() const {
		return *_virtual;
	}
	
	void compile(const FogScope& inScope);
	This& mutate() const {
		return *(This*)this;
	}
	
	void set_access(const FogAccess& anAccess);
	void set_virtual(const FogVirtual& aVirtual);
	
protected:
	FogBaseClass();
	virtual ~FogBaseClass();
	void merge_from(FogMergeContext& mergeContext, const FogBaseClass& baseClass);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& anId) const;
	
public:
	FogBaseClass(FogScope& baseClass, const FogAccess& anAccess, const FogVirtual& aVirtual);
	const FogAccess& access() const {
		return *_access;
	}
	
	FogScope& base() {
		return _base;
	}
	
	const FogScope& base() const {
		return _base;
	}
	
	void do_compile(FogCompileContext& inScope);
	void emit_interface(FogEmitContext& emitContext) const;
	const FogScope* find_bases(const FogScope& inScope, PrimIdMap& visitMap,
	                           FogScopeListOfRef& nonVirtualBases, FogScopeListOfRef& virtualBases) const;
	const PrimId& id() const;
	bool is_auto() const {
		return _access->is_auto();
	}
	
	virtual FogBaseClass* is_base_class();
	bool is_private() const {
		return _access->is_private();
	}
	
	bool is_protected() const {
		return _access->is_protected();
	}
	
	bool is_public() const {
		return _access->is_public();
	}
	
	bool is_virtual() const {
		return _virtual->is_virtual();
	}
	
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	//  	virtual FogTokenType::TokenType token_type_enum() const;
};

#endif

