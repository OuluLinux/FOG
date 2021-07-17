
#ifndef FOGCODE_HXX
#define FOGCODE_HXX








class FogCode : public FogRaw {
	typedef FogRaw Super;
	typedef FogCode This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
	
private:
	//   Defining contents.
	FogPositionHandle _position;    //   Required positioning of this contribution.
	FogSegmentHandle _segment;     //   Required segment for this contribution.
	FogFileIdsRefToConst _used_files;   //   Files used by the implementation.
	
	bool _is_exclusive : 1;      //   True if code is not to be augmented.
	bool _is_not_implemented : 1;    //   True if code is not to be implemented.
	bool _is_redundant : 1;      //   True if code required only to complement non-redundant code.
	bool _is_replacement : 1;     //   True if code is to replace existing definitions.
	FogRawCtorInitsRefToConst _ctor_inits;
	FogRawRef _body;
	
private:
	FogCode(const FogCode&);
	FogCode& operator=(const FogCode&);
	
private:
	FogCode& mutate() const {
		return *(FogCode*)this;
	}
	
	void set_code(const PrimString& aString);
	
protected:
	virtual ~FogCode();
	
public:
	FogCode();
	//  	FogCode(const FogConstTokens& someTokens);
	FogCode(FogRaw& codeBody);
	//  	void add(const FogFileId& fileId) { _used_files.add(fileId); }
	void compile();
	virtual bool compile_parsed(FogParseContext& parseContext);
	//  	void create_usages(const FogCompileContext& compileContext, FogTargetFile& byFile) const;
	size_t executable_tokens() const;
	bool is_exclusive() const {
		return _is_exclusive;
	}
	
	bool is_redundant() const {
		return _is_redundant;
	}
	
	bool is_replacement() const {
		return _is_replacement;
	}
	
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogMetaType& meta_type() const;
	bool not_implemented() const {
		return _is_not_implemented;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	//  	virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	const FogSegment& segment() const {
		return *_segment;
	}
	
	void set_ctor_inits(const FogRawCtorInits& ctorInits) {
		_ctor_inits = ctorInits;
	}
	
	void set_handlers(const FogHandlers& exceptionHandlers) {}
	
	void set_not_implemented() {
		_is_not_implemented = true;
	}
	
	void set_position(const FogPosition& aPosition) {
		_position = aPosition;
	}
	
	void set_segment(const FogSegment& aSegment) {
		_segment = aSegment;
	}
	
	void set_using_files(const FogFileIds& usedFiles) {
		_used_files = usedFiles;
	}
};

#endif

