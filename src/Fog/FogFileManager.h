
#ifndef FOGFILEMANAGER_HXX
#define FOGFILEMANAGER_HXX








typedef PrimSetElem<FogSourceFileRef> FogSourceFile_ListSetOfRefElement;
typedef PrimListOfRef<FogSourceFile_ListSetOfRefElement> FogSourceFile_ListSetOfRef;

typedef PrimSetElem<FogSourceFile_ListSetOfRef> FogSourceFile_NameSetOfRefElement;
typedef PrimMapOfRef<FogSourceFile_NameSetOfRefElement> FogSourceFile_NameSetOfRef;

class FogFileManager : public PrimPrint
{
 typedef PrimPrint Super;
 typedef FogFileManager This;
 TYPEDECL_SINGLE(This, Super)
private:        //   Defining contents.
 FogSourceFile_NameSetOfRef _source_files; //   Map of list of files indexed by file-stem sub-indexed by path.
            //    contains all files on all paths, even if not needed.
 FogTargetFileMapOfRef _target_files;  //   Map of those files that need processing..
private:        //   Working contents.
 FogTargetFileListOfRef _independents;  //   List of files created in the order in which independence arises.
 FogTargetFileMapOfRef _dependents;   //   Map of files dependent upon some dependent file.
private:
 FogFileManager(const This&);
 This& operator=(const This&);
private:
 const FogSourceFile *add_source_file(const PrimString& aPath, const PrimId& fileIdent,
  const PrimId& pathedId, const FogSourceFileType& sourceType);
 void show_cycle(const FogTargetFileListOfRefToConst& thePath, const FogTargetFile& cycleStart) const;
public:
 FogFileManager();
 virtual ~FogFileManager();
 void add_dependent(FogTargetFile& aFile) { _dependents.add(aFile); }
 void add_independent(FogTargetFile& aFile) { _independents.add(aFile); }
 void compile();
 void destroy();
 void emit(FogRoot& aRoot) const;
 void emit_dependencies(std::ostream& s, const PrimIdList& sourceList) const;
 const FogSourceFile *find_source_file(const PrimId& fileIdent, const FogSourceFileType& sourceType);
 const FogSourceFile *make_source_file(const PrimId& fileIdent, const FogSourceFileType& sourceType);
 FogTargetFile *make_target_file(const PrimId& aPath, const PrimId& fileIdent);
 FogTargetFile *make_target_file(const FogSourceFile& sourceFile);
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 void promote_to_independent(FogTargetFile& aFile);

private:
 static PrimText _include_paths;
public:
 static PrimText& include_paths() { return _include_paths; }
};
#endif
 
