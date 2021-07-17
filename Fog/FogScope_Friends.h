
#ifndef FOGSCOPE_FRIENDS_HXX
#define FOGSCOPE_FRIENDS_HXX




class FogScope_Friends {
private:          //   Defining contents.
	FogUniqueMapOfRef _friends;
public:
	void add(const FogScope& aScope, FogEntity& anEntity);
	void create_usages(FogScope& inScope);
	void destroy();
	void do_derive(FogDeriveContext& inScope);
	bool needs_emission() const { return _friends.tally() != 0; }
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
};
#endif

