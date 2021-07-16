
#ifndef FOGSCOPE_ENUMS_HXX
#define FOGSCOPE_ENUMS_HXX




class FogScope_Enums
{
private:          //   Defining contents.
 FogEnumMapOfRef _enum_map;      //   Map of all enumerations.
 FogEnumListOfRef _enum_list;     //   List of all enumerations.
public:
 void create_usages();
 void destroy();
 void do_compile(FogCompileContext& inScope);
 void do_derive(FogDeriveContext& inScope);
 const FogEnumListOfRef& enums() { return _enum_list; }
 FogEnum *find(const PrimId& anId) { return _enum_map.find(anId); }
 FogEnum *make_enum(FogMakerContext& makerContext);
 bool needs_emission() const { return _enum_map.tally() != 0; }
 std::ostream& print_depth(std::ostream& s, int aDepth) const;
 std::ostream& print_members(std::ostream& s, int aDepth) const;
 void validate() const {}
};
#endif
 
