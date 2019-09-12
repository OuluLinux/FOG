
#ifndef FOGSCOPE_USING_HXX
#define FOGSCOPE_USING_HXX





class FogScope_Using {

private:
            //   Defining contents.
	FogSpecifierListOfRefToConst _using_specifiers;   //   Parsed using declarations.
	FogUsingSpecifierListOfRefToConst _potential_usings; //   Parsed using name declarations.
	FogUsingMapOfRef _actual_usings;      //   Resolved using name declarations.
	
private:
           //   Derived contents.
//  	FogUniqueMapOfRef _using;								// Map of used name spaces.

private:
	FogScope_Using(const FogScope_Using&);
	
public:
	FogScope_Using() {}
	
	void add_using(FogUsing& aUsing);
//  	void add_using_directive(FogScopeImpl& scopeImpl, const FogUsingDeclaration& usingDirective);
//  	void adopt_using_name(const FogUsingSpecifier *rawUsing) { _potential_usings.adopt(rawUsing); }
	void create_usages();
	void destroy();
	void do_derive(FogDeriveContext& inScope);
//  	FogScope *find(const PrimId& anId) { return _directives.find(anId); }
//  	const FogScope *find_local_class(const PrimId& anId) const { return _directives.find(anId); }
	FogUsing *make_using(FogMakerContext& makerContext);
	bool needs_emission() const {
		return _actual_usings.tally() != 0;
	}
	
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
//  	const FogUsingDirectiveMapOfRefToConst& scopes() const { return _directives; }
};

#endif

