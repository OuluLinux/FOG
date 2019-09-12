
#ifndef FOGSOURCEFILE_HXX
#define FOGSOURCEFILE_HXX





class FogSourceFile : public FogToken
{
 typedef FogToken Super;
 typedef FogSourceFile This;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
 FOGTOKEN_ACTUAL_DECLS
private:
 const PrimIdHandle _pathed_id;    //   Complete external file name.
 const PrimIdHandle _short_id; 
 const FogSourceFileType& _source_type;
 mutable bool _has_been_read : 1;   //   True once read.
private:
 FogSourceFile(const This&);
 This& operator=(const This&);
 This& mutate() const { return *(This *)this; }
private:
 const PrimId& id() const { return unique_id(); }
 friend struct PrimMapOfRefToConstFunctions<FogSourceFile>;
protected:
 FogSourceFile();
public:
 FogSourceFile(const PrimId& pathedId, const PrimId& fileIdent, const FogSourceFileType& sourceType);
 const PrimId& full_file_name() const { return *_pathed_id; }
//  	virtual const FogMetaType *meta_type() const;
 bool needs_reading() const { return !_has_been_read; }
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 void set_read() const { mutate()._has_been_read = true; }
 const PrimId& short_id() const { return *_short_id; }
 const FogSourceFileType& source_type() const { return _source_type; }
 const PrimId& unique_id() const { return *_pathed_id; }
};
#endif
 
