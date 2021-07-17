
#ifndef FOGMACRO_HXX
#define FOGMACRO_HXX







class FogMacro : public PrimPrint {
	typedef FogMacro This;
	typedef PrimPrint Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMSHARES_DECLS(This)
	PRIMREF_DECLS(This)
	
private:
	//   Defining contents.
	const PrimIdHandle _id;     //   Macro name.
	PrimIdMap _formal_map;     //   Macro arguments.
	PrimIdList _formal_list;    //   Macro arguments in sequential order.
	FogTokensRef _tokens;   //   Token sequence defining the macro body.
	
	bool _function_like : 1;    //   True if a function-like macro
	
	mutable bool _is_bad : 1;    //   Set true once an error generated for bad macro syntax.
	
private:
	FogMacro(const This&);
	FogMacro& operator=(const This&);
	This& mutate() const {
		return *(This*)this;
	}
	
protected:
	FogMacro();
	virtual ~FogMacro();
	
public:
	FogMacro(const PrimId& anId, const PrimIdList* formalList, FogTokens& someTokens);
	const FogTokens& body() const {
		return *_tokens;
	}
	
	int find(const PrimId& anId) const;
	const PrimId& formal(size_t anIndex) const;
	size_t formal_tally() const {
		return _formal_list.tally();
	}
	
	const PrimId& id() const {
		return *_id;
	}
	
	bool is_bad() const {
		return _is_bad;
	}
	
	bool is_compatible_with(const PrimIdList* formalList, const FogTokens& someTokens) const;
	bool is_function_like() const {
		return _function_like;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void set_bad() const {
		mutate()._is_bad = true;
	}
	
	const FogTokenListOfRef& tokens() const {
		return _tokens->tokens();
	}
};

#endif

