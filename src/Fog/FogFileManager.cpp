#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogFileManager, Super)
PrimText FogFileManager::_include_paths;

TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogSourceFile_ListSetOfRefElement)
PRIMLISTOFREFS_IMPL(FogSourceFile_ListSetOfRefElement)
PRIMSETELEMENT_IMPL(FogSourceFile_ListSetOfRefElement)
PRIMREFS_IMPL(FogSourceFile_ListSetOfRefElement)
TMPL_HACK_FIX_DO_TEMPLATE(FogSourceFile_ListSetOfRefElement)

TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogSourceFile_NameSetOfRefElement)
PRIMMAPOFREFS_IMPL(FogSourceFile_NameSetOfRefElement)
PRIMSETELEMENT_IMPL(FogSourceFile_NameSetOfRefElement)
TMPL_HACK_FIX_DO_TEMPLATE(FogSourceFile_NameSetOfRefElement)

FogFileManager::FogFileManager() {}
FogFileManager::~FogFileManager() {}

//  
//  	Add the sourceType of file whose name is the concatenation of aPath a separator and fileIdent,
//  	making use of the pre-caclculated concatenation by the caller as pathedId.
//  
const FogSourceFile *FogFileManager::add_source_file(const PrimString& aPath, const PrimId& fileIdent,
  const PrimId& pathedId, const FogSourceFileType& sourceType)
{
 const FogSourceFile *theFile = 0;
 const PrimIdHandle pathId(aPath.str());
 FogSourceFile_NameSetOfRefElement *nameList = _source_files.find(fileIdent);
 if (!nameList)
 {
  PrimAdopted<FogSourceFile_ListSetOfRef> aList(new FogSourceFile_ListSetOfRef);
  if (aList)
  {
   FogSourceFileRef aFile(new FogSourceFile(pathedId, fileIdent, sourceType), FogSourceFileRef::ADOPT);
   aList->adopt(new FogSourceFile_ListSetOfRefElement(*pathId, aFile));
   _source_files.adopt(new FogSourceFile_NameSetOfRefElement(fileIdent, *aList));
   theFile = aFile.pointer();
  }
 }
 else
 {
  FogSourceFile_ListSetOfRef theList(nameList->value());
  for (PrimConstListOfRefToConstIterator<FogSourceFile_ListSetOfRefElement> p(theList); p; ++p)
   if (p->id() == *pathId)
   {
    WRNMSG("BUG - should not encounter duplicate " << c_string(aPath.str())
     << " " << c_string(fileIdent.str()));
    return &*p->value();
   }
  if (theList.tally() == 1)
   WRNMSG("Multiple versions of " << c_string(fileIdent.str()) << " found along paths\n\t"
    << c_string(theList[0]->id().str()) << " and " << c_string(aPath.str()));
  else if (theList.tally() > 1)
   WRNMSG("Further version of " << c_string(fileIdent.str()) << " found along path\n\t"
    << c_string(aPath.str()));
  FogSourceFileRef aFile(new FogSourceFile(pathedId, fileIdent, sourceType), FogSourceFileRef::ADOPT);
  theList.adopt(new FogSourceFile_ListSetOfRefElement(*pathId, aFile));
  theFile = aFile.pointer();
 }
 return theFile;
}

void FogFileManager::compile()
{
//  	_source_files.vacate();
 for (FogTargetFileConstMapOfRefIterator p1(_target_files); p1; ++p1)
 {
  FogTargetFile& aFile = *p1;
  aFile.precompile(*this);
 }
 size_t fileNumber = 0;
 while (true)           //   Terminates when all files independent.
 {
  while (fileNumber < _independents.tally())   //   Terminates because fileNumber++ to finite limit.
  {
   FogTargetFile& inDependency = *_independents[fileNumber++];
   inDependency.remove_all_uses(*this, fileNumber);
  }
  FogTargetFileConstMapOfRefIterator p(_dependents);
  if (p)            //   Always breaks one loop.
  {             //   Eventually broken loops create independencies.
   FogTargetFileListOfRefToConst tracedList(64);
   FogTargetFileMapOfRefToConst tracedMap(128);
   FogTargetFile *endFile = &*p;
   while (true)
   {
    FogTargetFile *nextFile = endFile->pick_cyclic_use_of(*this);
    if (!nextFile)        //   Only occurs if a bug has left endFile acyclic
     break;         //    in which case there is a new _independents.
    if (!tracedMap.find(nextFile->unique_id()))
    {
     tracedList.add(*nextFile);
     tracedMap.add(*nextFile);
     endFile = nextFile;
    }
    else
    {
     show_cycle(tracedList, *nextFile);
     endFile->remove_use_of(*nextFile, *this);
     break;
    } 
   }
  }
  else
   break;
 }
 for (FogTargetFileConstListOfRefIterator p4(_independents); p4; ++p4) //   Scan in first line first order.
 {
  FogTargetFile& aFile = *p4;
  aFile.compile_top();
 }
 _independents.sort(FogTargetFile::compare_most_dependent_first);
 for (FogTargetFileConstListOfRefIterator p5(_independents); p5; ++p5) //   Scan in last line last order.
 {
  FogTargetFile& aFile = *p5;
  aFile.compile_bottom();
 }
 _dependents.vacate();
 _independents.sort(FogTargetFile::compare);
}

void FogFileManager::destroy()
{
 for (FogTargetFileConstMapOfRefIterator p(_target_files); p; ++p)
  p->destroy();
 _source_files.vacate();
 _target_files.vacate();
 _independents.vacate();
 _dependents.vacate();
}

void FogFileManager::emit(FogRoot& aRoot) const
{
 for (FogTargetFileConstListOfRefToConstIterator p(_independents); p; ++p)
  if (!p->is_input())
   p->emit_file(aRoot);
}

void FogFileManager::emit_dependencies(std::ostream& s, const PrimIdList& sourceList) const
{
 for (FogTargetFileConstListOfRefToConstIterator p(_independents); p; ++p)
  if (!p->is_input())
   p->emit_dependencies(s, sourceList);
}

//  
//  	Locate the file defined by fileIdent, as required to service #include or using/include,
//  	returning the controlling file object.
//  
//  	Creates a list of the fileIdent across all possible paths as a side effect.
//  
//  	sourceType may be invalid to suppress an error message arising from mismatching source types.
//  
const FogSourceFile *FogFileManager::find_source_file(const PrimId& fileIdent,
  const FogSourceFileType& sourceType)
{
 FogSourceFile_NameSetOfRefElement *nameList = _source_files.find(fileIdent);
 const FogSourceFile *theFile = 0;
 if (!nameList)
 {
  PrimTextIterator p(_include_paths);
  if (!p || sourceType.is_top())
  {
   std::ifstream s(fileIdent.str());
   if (s)
    theFile = add_source_file(PrimString::null(), fileIdent, fileIdent, sourceType);
  }
  for ( ; p; p++)
  {
   PrimOstrstream fileName;
   fileName << *p << Fog::separator() << fileIdent;
   std::ifstream s(fileName.str());
   if (s)
   {
    const FogSourceFile *aFile = add_source_file(**p, fileIdent, fileName.id(), sourceType);
    if (!theFile)
     theFile = aFile;
   }
  }
 }
 else 
 {
  FogSourceFile_ListSetOfRef theList(nameList->value());
  if (theList.tally())
   theFile = theList[0]->value().pointer();
 }
 if (theFile && sourceType.is_read() && (theFile->source_type() != sourceType))
  ERRMSG("Should not attempt to use " << theFile->source_type() << " " << viz(*theFile)
      << " as a " << sourceType);
 return theFile;
}

//  
//  	Create a source file anyway. Used to satisfy #line references only.
//  
const FogSourceFile *FogFileManager::make_source_file(const PrimId& fileName,
 const FogSourceFileType& sourceType)
{
 return add_source_file(PrimString::null(), fileName, fileName, sourceType);
}

//  
//  	Make a file from the path and name fields of a FogFileIdent, typically resulting in a generated file.
//  	If aPath is 0, and no file exists on the empty path, the first file on any path is returned.
//  
FogTargetFile *FogFileManager::make_target_file(const PrimId& aPath, const PrimId& fileIdent)
{
 PrimOstrstream fileName;
 if (aPath)
  fileName << aPath << Fog::separator();
 fileName << fileIdent;
 FogTargetFile *targetFile = _target_files.find(fileName.id());
 if (targetFile)
  return targetFile;
 const FogSourceFile *sourceFile = find_source_file(fileIdent, FogSourceFileType::unread_input());
 if (sourceFile)
  return make_target_file(*sourceFile);
 targetFile = new FogTargetFile(fileName.id(), fileIdent);
 if (targetFile)
  _target_files.adopt(*targetFile);
 return targetFile;  
}

//  
//  	Make a file from the path and name fields of a FogFileIdent, typically resulting in a generated file.
//  	If aPath is 0, and no file exists on the empty path, the first file on any path is returned.
//  
FogTargetFile *FogFileManager::make_target_file(const FogSourceFile& sourceFile)
{
 if (!sourceFile.source_type().is_hash() && !sourceFile.source_type().is_unread())
  return 0;
 FogTargetFile *targetFile = _target_files.find(sourceFile.unique_id());
 if (targetFile)
  return targetFile;
 targetFile = new FogTargetFile(sourceFile);
 if (targetFile)
  _target_files.adopt(*targetFile);
 return targetFile;  
}

std::ostream& FogFileManager::print_depth(std::ostream& s, int aDepth) const
{
 if (_source_files.tally())
 {
  s << indent(aDepth) << "source file names\n";
  for (PrimConstMapOfRefToConstIterator<FogSourceFile_NameSetOfRefElement> p(_source_files); p; ++p)
   if (!p->value().tally())
    s << indent(aDepth+1) << "? " << c_string(p->id().str()) << '\n';
   else
    for (PrimConstListOfRefToConstIterator<FogSourceFile_ListSetOfRefElement> q(p->value()); q; ++q)
     s << indent(aDepth+1) << c_string(q->id().str()) << ' ' << c_string(p->id().str()) << '\n';
 }
 if (_target_files.tally())
 {
  s << indent(aDepth) << "target files\n";
  _target_files.print_members(s, aDepth+1);
 }
 if (_independents.tally())
 {
  s << indent(aDepth) << "independent files\n";
  _independents.print_members(s, aDepth+1);
 }
 if (_dependents.tally())
 {
  s << indent(aDepth) << "dependent files\n";
  for (FogTargetFileConstMapOfRefToConstIterator p1(_dependents); p1; ++p1)
  {
   s << indent(aDepth+1) << *p1 << '\n';
   for (FogTargetFileConstMapOfRefToConstIterator p2(p1->use_of()); p2; ++p2)
    s << indent(aDepth+2) << *p2 << '\n';
  }
 }
 return s;
}

std::ostream& FogFileManager::print_members(std::ostream& s, int aDepth) const
{
 if (_source_files.tally())
 {
  s << indent(aDepth) << "source file names\n";
  _source_files.print_members(s, aDepth+1);
 }
 if (_target_files.tally())
 {
  s << indent(aDepth) << "target files\n";
  _target_files.print_members(s, aDepth+1);
 }
 if (_independents.tally())
 {
  s << indent(aDepth) << "independent files\n";
  _independents.print_members(s, aDepth+1);
 }
 if (_dependents.tally())
 {
  s << indent(aDepth) << "dependent files\n";
  _dependents.print_members(s, aDepth+1);
 }
 return s;
}

void FogFileManager::promote_to_independent(FogTargetFile& aFile)
{
 add_independent(aFile);
 if (!_dependents.remove(aFile.unique_id()))
  ERRMSG("BUG - should not remove dependency on independent " << viz(aFile));
}

//  
//  	Generate a diagnostic to explain the detected dependency cycle comprising the elements of thePath
//  	from cycleStart to the end of thePath. (Preceeding elements represent a non-cyclic pre-amble).
//  
void FogFileManager::show_cycle(const FogTargetFileListOfRefToConst& thePath,
         const FogTargetFile& cycleStart) const
{
 FogTargetFileConstListOfRefToConstIterator p(thePath);
 for ( ; p; ++p)
  if (&*p == &cycleStart)
   break;
 static const char *anExplanation = 
  "\n"
  "FOG has failed to identify a legal file inclusion order.\n"
  "A dependency cycle has been identified whereby file A directly or indirectly\n"
  "requires file B to be included first, but file B requires the reverse order.\n"
  "FOG will arbitrarily discard ordering constraints until all cycles have been eliminated.\n"
  "Output files can then be emitted, but compilation errors are to be expected.\n"
  "\n"
  "The problem may be resolved by using constructs such as:\n"
  "\tclass B\n"
  "\t{\n"
  "\t\texport/interface A; export/implementation A;\n"
  "\t};\n"
  "to co-locate the interface and implementation of B with A.\n"
  "\n"
  "The problem may arise from inappropriate use of file-declarations.\n";
 if (anExplanation)
 {
  DIAMSGZ(anExplanation);
  anExplanation = 0;
 }
 PrimOstrstream s;
 s << "Cyclic dependency identified and arbitrarily broken involving:";
 for ( ; p; ++p)
  s << "\n\t" << c_string(p->unique_id().str());
 ERRMSGZ(s.str());
}
