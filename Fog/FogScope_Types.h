
#ifndef FOGSCOPE_TYPES_HXX
#define FOGSCOPE_TYPES_HXX





struct FogScope_Type {
	FogEntityRef _definition;                       //   e.g the typedef
	FogEntityRef _resolution;                       //   e.g the target
};

typedef PrimSetElem<FogScope_Type> FogScope_TypeSetOfRefElement;
typedef PrimMapOfRef<FogScope_TypeSetOfRefElement> FogScope_TypeSetOfRef;
typedef PrimConstMapOfRefIterator<FogScope_TypeSetOfRefElement> FogScope_TypeConstSetOfRefIterator;
typedef PrimConstMapOfRefToConstIterator<FogScope_TypeSetOfRefElement> FogScope_TypeConstSetOfRefToConstIterator;
typedef PrimMapOfRefIterator<FogScope_TypeSetOfRefElement> FogScope_TypeSetOfRefIterator;
typedef PrimMapOfRefToConstIterator<FogScope_TypeSetOfRefElement> FogScope_TypeSetOfRefToConstIterator;

class FogScope_Types {
private:                                            //   Defining contents.
	FogTypeNameMapOfRef _other_types;                   //   Map of all unresolved types.
	FogScope_TypeSetOfRef _types;                       //   All local type-names.
	FogScope_TypeSetOfRef _scopes;                      //   All local non-type names.that behave as types (namespaces)
private:
	FogScope_Types& mutate() const { return *(FogScope_Types*)this; }
public:
	void add_scope(FogEntity& typeValue);
	void add_type(const PrimId& typeName, FogEntity& typeValue);
	void add_type(FogEntity& typeName, FogEntity& typeValue);
	void destroy();
	FogEntity* find_local_scope(const PrimId& anId, FogScopeEnums::ResolveAlias resolveAlias);
	FogEntity* find_local_type(const PrimId& anId, FogScopeEnums::ResolveAlias resolveAlias);
	FogTypeName* find_local_other_type(const PrimId& anId) { return _other_types.find(anId); }
	//    bool is_empty() const { return _types.tally() == 0; }
	FogTypeName* make_typename(FogMakerContext& makerContext, bool builtIn = false);
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
};
#endif

