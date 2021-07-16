
#ifndef FOGSCOPE_BASES_HXX
#define FOGSCOPE_BASES_HXX





class FogScope_Bases
{
private:          //   Defining contents.
 FogBaseClassListOfRef _direct_bases;   //   List of declared direct base classes.
 FogBaseClassMapOfRef _base_map;     //   Map of declared direct base classes.
private:          //   Defined by build_base_classes.
 FogScopeListOfRef _ctor_bases;     //   Direct bases in construction order (virtuals first)
 FogScopeListOfRef _all_bases;     //   All bases in construction order (virtuals first)
 size_t _virtual_bases;       //   Number of virtual bases at head of _ctor_bases, _all_bases.
private:
 const FogScope *build_base_classes(FogScope& thisScope);
public:
 FogScope_Bases();
 bool add_base(FogScope& thisScope, FogBaseClass& baseClass);
 FogScopeListOfRef& all_bases() { return _all_bases; }
 const FogScopeListOfRefToConst& all_bases() const { return _all_bases; }
 void changed_base_class(FogScope& thisScope, FogBaseClass& baseClass);
 void create_usages(FogScope& inScope);
 FogScopeListOfRef& ctor_bases() { return _ctor_bases; }
 const FogScopeListOfRefToConst& ctor_bases() const { return _ctor_bases; }
 void derive_from(FogScope& derivedScope, FogBaseClass& baseClass);
 void destroy();
 FogBaseClassListOfRef& direct_bases() { return _direct_bases; }
 const FogBaseClassListOfRefToConst& direct_bases() const { return _direct_bases; }
 void do_compile(FogCompileContext& inScope);
 void do_derive(FogDeriveContext& inScope);
 void do_meta_construct(FogScopeContext& inScope);
 void emit_base_interface(FogEmitContext& emitContext) const;
 const FogScope *find_bases(const FogScope& inScope, PrimIdMap& visitMap,
  FogScopeListOfRef& nonVirtualBases, FogScopeListOfRef& virtualBases) const;
 bool find_entity(FogEntityFinder& theFinder);
 bool find_slot(FogScope& inScope, FogMetaSlotFinder& theFinder);
 bool is_pure() const;
 bool needs_emission() const;
 std::ostream& print_depth(std::ostream& s, int aDepth) const;
 std::ostream& print_members(std::ostream& s, int aDepth) const;
 size_t virtual_bases() const { return _virtual_bases; }
};
#endif
 
