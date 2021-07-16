
#ifndef FOGENUM_HXX
#define FOGENUM_HXX






class FogEnum : public FogEntity
{
 typedef FogEntity Super;
 TYPEDECL_SINGLE(FogEnum, Super)
 PRIMREF_DERIVED_DECLS(FogEnum)
private:        //   Defining contents.
 bool _is_named_anon_type : 1;   //   True if an anonymous enum that got named by a typedef.
 bool _is_unnamed_anon_type : 1;   //   True if an anonymous enum that stayed anonymous.
 FogAccessHandle _access;    //   Accessibility within scope.
 FogEnumeratorListOfRef _list;   //   Enumerators in emission order.
 FogEnumeratorMapOfRef _map;    //   Enumerators for ambiguity detection.
 const FogEnumerator *_current;   //   Most recent addition.
private:
 virtual FogUsage *make_interface_usage_finish();
 FogEnum& mutate() const { return *(FogEnum *)this; }
protected:
 FogEnum();
 virtual ~FogEnum();
public:
 FogEnum(FogMakerContext& makerContext);
 virtual const FogAccess& access() const;
 void add(FogEnumerator& anEnumerator);
 virtual void create_usages();
 virtual bool do_compile(FogCompileContext& inScope);
 virtual void emit_forward_reference(FogStream& s);
 virtual void emit_interface(FogStream& s);
 const FogEnumeratorListOfRef& enumerators() { return _list; }
 virtual FogUsage& interface_usage_start();
 virtual bool is_active() const;
 virtual FogEnum *is_enum();
 virtual bool is_type() const;
 virtual const FogMetaType& meta_type() const;
 virtual FogUsage& name_usage();
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 FogScope& scope() { return non_virtual_scope(); }
 const FogScope& scope() const { return non_virtual_scope(); }
 virtual void set_access(const FogAccess& anAccess);
 void set_default_access(const FogAccess& anAccess);
 void set_is_named_anon_type() { _is_named_anon_type = true; }
 void set_is_unnamed_anon_type() { _is_unnamed_anon_type = true; }
public:
 static int compare(const FogEnumRefToConst *p1, const FogEnumRefToConst *p2);
};
#endif
 
