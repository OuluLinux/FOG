
#ifndef FOGDECORATEDNAME_HXX
#define FOGDECORATEDNAME_HXX




class FogDecoratedName : public FogName {
	typedef FogName Super;
	typedef FogDecoratedName This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS_LESS_EMIT
	
private:
	FogNameRef _name;        //   The less decorated name
	
private:
	This& mutate() const {
		return *(This*)this;
	}
	
protected:
	FogDecoratedName() {}
	
	FogDecoratedName(FogName& anId) : _name(anId) {}
	
	FogDecoratedName(FogName* anId) : _name(anId) {}
	
	FogDecoratedName(const This& proxyId) : Super(proxyId), _name(proxyId._name) {}
	
	virtual ~FogDecoratedName();
	FogName& name() {
		return *_name;
	}
	
	FogNameRef& name_ref() {
		return _name;
	}
	
	void set_decorated_destructor_name(FogNameRef& aName);
	void set_name(FogName& aName) {
		_name = aName;
	}
	
public:
	virtual size_t executable_tokens() const;
	const FogName& name() const {
		return *_name;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual void set_line(FogLine& aLine);
};

#endif

