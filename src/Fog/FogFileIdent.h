
#ifndef FOGFILEIDENT_HXX
#define FOGFILEIDENT_HXX





class FogFileIdent : public FogFileId
{
 typedef FogFileId Super;
 TYPEDECL_SINGLE(FogFileIdent, Super)
private:         //   Defining contents.
 PrimIdHandle _path;      //   Optional file name path.
 PrimIdHandle _prefix;     //   Optional name prefix.
 PrimIdHandle _name;      //   File stem.
 PrimIdHandle _suffix;     //   Optional name suffix.
 PrimIdHandle _guard;     //   Upper cased _name used as guard.
 bool _is_implementation : 1;   //   True if file is intended to behave as an implementation.
 bool _is_interface : 1;     //   True if file is intended to behave as an interface.
 bool _is_template : 1;     //   True if file is intended to behave as a template.
 bool _explicit_name : 1;    //   True once set_name invoked.
 bool _explicit_guard : 1;    //   True once set_guard invoked.
 FogUtilityHandle _utility;    //   Status of contributions to file.
private:         //   Defining contents.
 FogFileIdent& mutate() const { return *(FogFileIdent *)this; }
 void set_new_utility(const FogUtility& aUtility);
protected:
 virtual ~FogFileIdent();
public:
 FogFileIdent(const char *fileStem, const PrimId& templateArguments);
 virtual FogTargetFile *make(FogScopeContext& scopeContext) const;
 FogTargetFile *make_target_file(FogFileManager& fileManager) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 void set_guard(const PrimId& anId);
 void set_implementation() { _is_implementation = true; }
 void set_interface() { _is_interface = true; }
 void set_name(const PrimId& anId);
 void set_path(const PrimId& anId);
 void set_prefix(const PrimId& anId);
 void set_suffix(const PrimId& anId);
 void set_template() { _is_template = true; }
 void set_utility(const FogUtility& aUtility) { if (aUtility != utility()) set_new_utility(aUtility); }
 const FogUtility& utility() const { return *_utility; }
};
#endif
 
