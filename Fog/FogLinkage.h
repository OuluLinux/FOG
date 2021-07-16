
#ifndef FOGLINKAGE_HXX
#define FOGLINKAGE_HXX




class FogLinkage : public FogScope
{
 typedef FogScope Super;
 typedef FogLinkage This;
 TYPEDECL_SINGLE(This, Super)
private:
 const PrimIdHandle _string;
private:
 This& mutate() const { return *(This *)this; }
 FogScope& scope() { return non_virtual_scope(); }
 const FogScope& scope() const { return non_virtual_scope(); }
protected:
 FogLinkage();
 virtual ~FogLinkage();
public:
 FogLinkage(FogScope& aScope, const PrimId& anId, const PrimId& linkageString);
 virtual bool do_compile(FogCompileContext& inScope);
  virtual void emit_scope(FogEmitContext& emitContext, const FogScope& inScope) const;
 virtual void emit_scope_head(FogStream& s) const;
 virtual void emit_scope_tail(FogStream& s) const;
 virtual void find_entities(FogEntityFinder& theFinder);
 virtual FogEntity *find_entity(const PrimId& anId, InScope inScope, FindStrategy aStrategy);
 virtual FogTargetFile *implementation_file();
 virtual FogTargetFile *interface_file_source();
 virtual FogUsage& interface_usage_finish();
 virtual FogUsage& interface_usage_start();
 virtual const FogMetaType& meta_type() const;
 virtual FogScope& name_emit_scope();
 virtual FogScope& name_emit_transient_scope();
 virtual FogUsage& name_usage();
};
#endif
 
