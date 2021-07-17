#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogTargetFile, Super)
PRIMREF_NULL_CLAS(FogTargetFile)
virtual void add_external(FogUsage& aUsage);
virtual void add_include(FogTargetFile& aFile);
virtual void add_internal(FogUsage& aUsage);
virtual void compile_bottom();
virtual void compile_top();
virtual void precompile(FogFileManager& fileManager);
virtual void set_file_number(size_t aNumber);
virtual void set_guard(const PrimId& aGuard);
virtual void set_is_input();
virtual void set_new_utility(const FogUtility& aUtility);
};

PRIMREF_DERIVED_NULL_IMPL(FogTargetFile)
PRIMREFS_IMPL(FogTargetFile)
PRIMLISTOFREFS_IMPL(FogTargetFile)
PRIMMAPOFREFS_IMPL(FogTargetFile)
FOGTOKEN_ACTUAL_IMPL(FogTargetFile)
TMPL_HACK_FIX_DO(FogTargetFile)

FogTargetFile::FogTargetFile()
	:
	_source_file(0),
	_explicit_guard(false),
	_is_interface(false),
	_is_input(false),
	_file_number(0),
	_min_usage_number(0),
	_max_usage_number(0),
	_usage(0) {}

FogTargetFile::FogTargetFile(const FogSourceFile& sourceFile)
	:
	_pathed_id(sourceFile.unique_id()),
	_short_id(sourceFile.short_id()),
	_source_file(&sourceFile),
	_utility(FogUtility::pool_utility()),
	_explicit_guard(false),
	_is_interface(false),
	_is_input(false),
	_file_number(0),
	_min_usage_number(0),
	_max_usage_number(0),
	_usage(0) {
	if (!sourceFile.source_type().is_hash() && !sourceFile.source_type().is_unread())
		ERRMSG("BUG - should not make " << viz(*this) << " for " << viz(sourceFile));
}

FogTargetFile::FogTargetFile(const PrimId& pathedId, const PrimId& fileIdent)
	:
	_pathed_id(pathedId),
	_short_id(fileIdent),
	_source_file(0),
	_utility(FogUtility::pool_utility()),
	_explicit_guard(false),
	_is_interface(false),
	_is_input(false),
	_file_number(0),
	_min_usage_number(0),
	_max_usage_number(0),
	_usage(0) {}

void FogTargetFile::add_external(FogUsage& aUsage) {
	_external_map.add_filtered(aUsage);
	FogTargetFile* aFile = aUsage.file();
	
	if (!aFile)
		_visible_names |= aUsage;
	else if (aFile != this)
		add_include(*aFile);
}

void FogTargetFile::add_include(FogTargetFile& aFile) {
	if (aFile.is_null())
		;
	else if (&aFile == this)
		ERRMSG("BUG - should not add_include " << viz(aFile) << " to itself.");
	else if (aFile._source_file && !aFile._source_file->source_type().is_hash()
	         && !aFile._source_file->source_type().is_unread())
		ERRMSG("BUG - should not add_include " << viz(aFile) << " to " << viz(*this));
	else if (_use_of.add_filtered(aFile)) {
		aFile._used_by.add(*this);
		CONDMSG(Fog::debug_file(), "Adding use of " << viz(aFile) << " by " << viz(*this));
	}
}

void FogTargetFile::add_internal(FogUsage& aUsage) {
	if (_source_file && !_source_file->source_type().is_hash() && !_source_file->source_type().is_unread())
		ERRMSG("BUG - should not add " << viz(aUsage) << " to " << viz(*this));
	else
		_internals.add(aUsage);
}

int FogTargetFile::compare(const FogTargetFileRefToConst* p1, const FogTargetFileRefToConst* p2) {
	return (*p1)->unique_id().compare((*p2)->unique_id());
}

int FogTargetFile::compare_least_dependent_first(const FogTargetFileRefToConst* p1,
        const FogTargetFileRefToConst* p2) {
	//  	size_t firstFileNumber = (*p1)->_file_number;
	//  	size_t secondFileNumber = (*p2)->_file_number;
	size_t firstFileNumber = (*p1)->_min_usage_number;
	size_t secondFileNumber = (*p2)->_min_usage_number;
	return int(firstFileNumber) - int(secondFileNumber);
}

int FogTargetFile::compare_most_dependent_first(const FogTargetFileRefToConst* p1,
        const FogTargetFileRefToConst* p2) {
	//  	size_t firstFileNumber = (*p1)->_file_number;
	//  	size_t secondFileNumber = (*p2)->_file_number;
	size_t firstFileNumber = (*p1)->_max_usage_number;
	size_t secondFileNumber = (*p2)->_max_usage_number;
	return int(firstFileNumber) - int(secondFileNumber);
}

//
//  	Compilation of the bottom of a file occurs in file independence order sorted by the last content,
//  	to enable this file to determine properties that are true after inclusion of this file
//  	(and all its nested files).
//
void FogTargetFile::compile_bottom() {
	_external_files.sort(&FogTargetFile::compare_least_dependent_first);
	
	for (FogTargetFileConstListOfRefToConstIterator p1(_external_files); p1; ++p1) {
		_visible_files |= p1->_visible_files;
		_visible_names |= p1->_visible_names;
	}
	
	for (FogUsageConstListOfRefToConstIterator p2(_internals); p2; ++p2) {
		_visible_names |= p2->precursors();
		//  		set_max_utility(p2->entity());		-- more elegant to pull, but much too late.
	}
	
	for (FogUsageConstMapOfRefToConstIterator p3(_visible_names); p3; ++p3) {
		const FogTargetFile* usedFile = p3->used_file();
		
		if (usedFile) {
			_visible_files |= usedFile->unique_id();
			_visible_names |= usedFile->_visible_names;
		}
	}
}

//
//  	Compilation of the top of a file occurs in file independence order sorted by the first content,
//  	to enable this file to determine properties that are true at the top of this file.
//
void FogTargetFile::compile_top() {
	if (_source_file && !_source_file->needs_reading())
		set_max_utility(FogUtility::frozen_utility());
		
	if (!_guard && !_explicit_guard) {
		PrimOstrstream guardName;
		
		for (const char* p = short_id().str(); *p; p++)
			guardName << char(isalnum(*p) ? toupper(*p) : '_');
			
		_guard = guardName.str();
	}
	
	_visible_files |= unique_id();
	_visible_names |= _internals;
}

void FogTargetFile::destroy() {
	_external_map.vacate();
	_internals.vacate();
	_used_by.vacate();
	_use_of.vacate();
	_external_files.vacate();
	_visible_files.vacate();
	_visible_names.vacate();
}

void FogTargetFile::diagnose_conflicts() const {
	PrimIdMap utilityEntities(_internals.tally());
	PrimIdMap nonUtilityEntities(_internals.tally());
	
	for (FogUsageConstListOfRefToConstIterator p1(_internals); p1; ++p1) {
		const FogEntity* anEntity = p1->entity();
		
		if (!anEntity || anEntity->is_instantiation())
			continue;
			
		const FogScope* aScope = anEntity->is_scope();
		const FogUtility& aUtility = anEntity->utility();
		
		if (aUtility.is_not_emitted())
			utilityEntities.add_filtered(anEntity->unique_id());
		else {
			anEntity->is_instantiation();
			nonUtilityEntities.add_filtered(anEntity->unique_id());
		}
	}
	
	if (nonUtilityEntities.tally()) {
		PrimOstrstream s;
		s << "Cannot emit " << viz(*this) << " containing both utility and non-utility contributions.";
		//  		static doneMessage = false;
		//  		if (!doneMessage)
		//  		{
		//  			doneMessage = true;
		//  			s << "\n\tFile has been declared for use as the implementation or interface for some entity";
		//  			s << "\n\tthat is in a frozen file, presumably a #included file. Examination of the file";
		//  			s << "\n\tmay reveal the offending entity, but only if it has the corresponding";
		//  			s << "\n\timplementation or interface.";
		//  			s << "\n\tProblem probably arises from use of a using include within a #include.";
		//  		}
		s << "\n\tUtility contributions from " << utilityEntities.tally() << " entities :";
		//  		PrimIdMapIterator p2(utilityEntities);
		PrimIdList utilityList(utilityEntities);
		utilityList.sort(&PrimId::sort_compare);
		PrimIdListIterator p2(utilityList);
		
		for (int i2 = 0; p2 && (i2 < 10); ++p2, ++i2)
			s << "\n\t\t" << *p2;
			
		if (p2)
			s << "\n\t\t...";
			
		s << "\n\tNon-utility contributions from " << nonUtilityEntities.tally() << " entities :";
		//  		PrimIdMapIterator p3(nonUtilityEntities);
		PrimIdList nonUtilityList(nonUtilityEntities);
		nonUtilityList.sort(&PrimId::sort_compare);
		PrimIdListIterator p3(nonUtilityList);
		
		for (int i3 = 0; p3 && (i3 < 10); ++p3, ++i3)
			s << "\n\t\t" << *p3;
			
		if (p3)
			s << "\n\t\t...";
			
		ERRMSGZ(s.str());
	}
}

void FogTargetFile::diagnose_non_emission() const {
	FogProgressMonitor contextMonitor("Validating non-utility contents of utility", *this, ".");
	FogUniqueMapOfRefToConst missedImplementations;
	FogUniqueMapOfRefToConst missedInterfaces;
	
	for (FogUsageConstListOfRefToConstIterator p(_internals); p; ++p) {
		const FogEntity* anEntity = p->entity();
		
		if (!anEntity || !anEntity->utility().is_emit())
			continue;
			
		const FogUnique& anUnique = *anEntity;
		
		if (p->is_interface()) {
			if (!anEntity->is_instantiation())
				missedInterfaces.add_filtered(anUnique);
		}
		else if (p->is_implementation())
			missedImplementations.add_filtered(anUnique);
	}
	
	if (missedInterfaces.tally()) {
		PrimOstrstream s;
		s << "The following interfaces were not emitted:";
		FogUniqueListOfRefToConst aList(missedInterfaces);
		aList.sort(&FogUnique::compare);
		
		for (FogUniqueConstListOfRefToConstIterator p(aList); p; ++p)
			s << "\n\t" << p->unique_id();
			
		WRNMSGZ(s.str());
	}
	
	if (missedImplementations.tally()) {
		PrimOstrstream s;
		s << "The following implementations were not emitted:";
		FogUniqueListOfRefToConst aList(missedImplementations);
		aList.sort(&FogUnique::compare);
		
		for (FogUniqueConstListOfRefToConstIterator p(aList); p; ++p)
			s << "\n\t" << p->unique_id();
			
		WRNMSGZ(s.str());
	}
}

void FogTargetFile::diagnose_non_inline_implementations() const {
	if (false && _is_interface)
		WRNMSG("Non-inline implementations within " << viz(*this) << " may violate One Definition Rule.");
		
	//  .bugbug no code
}

void FogTargetFile::emit_contents(FogStream& s, const FogRoot& aRoot) const {
	FogProgressMonitor contextMonitor("Generating", *this, ".");
	aRoot.emit_banner(s);
	
	if (_guard && *_guard.str()) {
		s << "#ifndef " << *_guard << '\n';
		s << "#define " << *_guard << '\n';
		next(s);
	}
	
	if (Fog::emit_preheader()) {
		s << "PREHEADER\n";
	}
	
	PrimIdMap visibleFiles(_visible_files.tally());
	visibleFiles |= unique_id();        //   Inhibit a self inclusion
	FogUsageMapOfRefToConst visibleNames(_visible_names.tally());
	FogTargetFileConstListOfRefToConstIterator pFile(_external_files);
	FogUsage::TypeEnum lastType = FogUsage::INTERFACE_FINISH; //   Fortuitously avoids a blank line pre-};
	
	for (FogUsageConstListOfRefIterator pInternal(_internals); pInternal; ++pInternal) {
		FogUsage& internalUsage = *pInternal;
		
		if (!internalUsage.is_weak()) {
			visibleNames |= internalUsage;      //   No need to forward declare this usage.
			const FogEntity* usageEntity = internalUsage.entity();
			const FogUsage* nameUsage = usageEntity ? usageEntity->raw_name_usage() : 0;
			
			if (nameUsage && (nameUsage != &internalUsage))
				visibleNames |= *nameUsage;      //   No need to forward declare name of this usage.
		}
		
		size_t usageNumber = internalUsage.usage_number();
		
		//  #if 0
		
		if (pFile && (pFile->min_usage_number() <= usageNumber)) { //   If some include files pending.
			s.change_to_emit_scope(aRoot);
			next(s);
			
			while (pFile && (pFile->min_usage_number() < usageNumber)) { //   Loop till all pending files done/
				//   Minimal output requires that the most dependent file be output first, since its
				//   inclusion may make a less dependent file inclusion redundant. The algorithm here
				//   is quadratic, but could be improved by preparing a work list. Since the expected
				//   number of files to emit is low, the overheads to achieve the algorithmic efficiency
				//   is unlikely to be justified in practice.
				const FogTargetFile* theFile = 0;
				
				for (; pFile && (pFile->min_usage_number() < usageNumber); ++pFile)
					if (!visibleFiles.find(pFile->unique_id())) { //   Pick first dependent file.
						theFile = &*pFile;
						break;
					}
					
				if (theFile) {
					FogTargetFileConstListOfRefToConstIterator p(pFile);
					
					for (++p; p && (p->min_usage_number() < usageNumber); ++p)
						if (!visibleFiles.find(p->unique_id())   //   Pick more dependent file
						        //  						 && (p->max_usage_number() > theFile->max_usage_number())
						        && (!theFile->_visible_files.find(p->unique_id())))
							theFile = &*p;
							
					theFile->emit_guarded_include(s);
					visibleFiles |= theFile->_visible_files;
					visibleNames |= theFile->_visible_names;
				}
			}
			
			lastType = FogUsage::INTERFACE_FINISH;
			next(s);
		}
		
		//  #endif
		
		if (internalUsage.precursors().tally()) {
			FogUsageMapOfRef preCursors(internalUsage.precursors());
			preCursors -= (FogUsageMapOfRef&)visibleNames;   //  .bugbug cast - ok since subtraction
			
			if (preCursors.tally()) {
				s.change_to_emit_scope(aRoot);
				next(s);
				FogUsageListOfRef preCursorList(preCursors);
				preCursorList.sort(&FogUsage::compare_least_dependent_first);
				
				for (FogUsageConstListOfRefIterator p(preCursorList); p; ++p) {
					p->emit(s);
					visibleNames |= *p;
				}
				
				lastType = FogUsage::INTERFACE_FINISH;
			}
		}
		
		if (!internalUsage.is_weak()) {
			if ((lastType != internalUsage.type()) && (internalUsage.type() != FogUsage::INTERFACE_FINISH)) {
				lastType = internalUsage.type();
				s.change_to_access(FogAccess::invalid_access());
			}
			
			const FogTargetFile* usageFile = internalUsage.used_file();
			
			if (usageFile) {
				if (visibleFiles.find(usageFile->unique_id()))
					continue;
					
				visibleFiles |= usageFile->unique_id();
			}
			
			internalUsage.emit(s);
		}
	}
	
	s.change_to_emit_scope(aRoot);
	
	if (_guard && *_guard.str()) {
		next(s);
		s << "#endif\n";
	}
}

void FogTargetFile::emit_dependencies(std::ostream& s, const PrimIdList& sourceList) const {
	if (is_null())
		return;
		
	if (_source_file) {
		const FogSourceFileType& sourceType = _source_file->source_type();
		
		if (!_utility->is_emit())
			return;
		else if (sourceType.is_unread() && _source_file->needs_reading())
			;
		else
			return;
	}
	
	if (!utility().is_emit())
		return;
		
	s << '\n' << full_file_name().str() << " :";
	
	for (PrimIdConstListIterator p(sourceList); p; p++)
		s << " \\\n\t" << *p;
		
	s << '\n';
}

void FogTargetFile::emit_file(FogRoot& aRoot) const {
	if (is_null())
		return;
		
	if (_source_file) {
		const FogSourceFileType& sourceType = _source_file->source_type();
		
		if (!_utility->is_emit()) {
			//  			DIAMSG("Emission of " << viz(*this) << " does not occur to overwrite " << viz(*_source_file));
			return;
		}
		else if (sourceType.is_unread() && _source_file->needs_reading()) {
			DIAMSG("Emission of " << viz(*this) << " can safely overwrite " << viz(*_source_file));
		}
		//  		else if (!_utility->is_frozen())
		//  		{
		//  			WRNMSG("Emission of " << viz(*this) << " suppressed by existence of " << viz(*_source_file));
		//  			return;
		//  		}
		else {
			WRNMSG("Emission of " << viz(*this) << " ignored to " << viz(*_source_file));
			return;
		}
	}
	
	if (utility().is_not_emitted())
		diagnose_conflicts();
		
	if (!utility().is_emit()) {
		diagnose_non_emission();
		return;
	}
	
	FogStream s(aRoot, this);
	emit_contents(s, aRoot);
	FogFileComparator::Status aStatus = Fog::force() ?
	                                    FogFileComparator::NEW_TEXT : FogFileComparator::equivalent(s.str(), unique_id());
	                                    
	switch (aStatus) {
	case FogFileComparator::NEW_TEXT: {
		if (Fog::notify_create())
			DIAMSG("Writing " << c_string(unique_id().str()));
			
		PrimOfstreamMonitor fileMonitor("Creating", *PrimStringHandle(unique_id().str()), ".");
		fileMonitor.s() << s.str();
		break;
	}
	
	case FogFileComparator::CHANGED_TEXT: {
		if (Fog::notify_create())
			DIAMSG("Over-writing " << c_string(unique_id().str()));
			
		PrimOfstreamMonitor fileMonitor("Updating", *PrimStringHandle(unique_id().str()), ".");
		fileMonitor.s() << s.str();
		break;
	}
	
	case FogFileComparator::SAME_TEXT:
		if (Fog::notify_equivalence())
			DIAMSG("No need to update " << c_string(unique_id().str()));
			
		break;
		
	case FogFileComparator::NON_FOG_TEXT:
		ERRMSG("Missing '/*!$@FOG@$!'  marker at start of " << c_string(unique_id().str())
		       << ".\n\tFile not overwritten. Delete manually and reinvoke fog, or use fog -f.");
		break;
	}
}

void FogTargetFile::emit_guarded_include(FogStream& s) const {
	const bool hasGuard = _guard && *_guard.str();
	
	if (hasGuard)
		s << "#ifndef " << *_guard << '\n';
		
	s << "#include <" << short_id() << ">\n";
	
	if (hasGuard) {
		if (_explicit_guard) {
			s << "#ifndef " << *_guard << '\n';
			s << "#define " << *_guard << '\n';
			s << "#endif\n";
		}
		
		s << "#endif\n";
	}
	
	diagnose_non_inline_implementations();
}

bool FogTargetFile::is_frozen_input() const {
	if (!_source_file)
		return false;
		
	return _source_file->source_type().is_read();
}

bool FogTargetFile::is_input() const {
	return _is_input;
}

//
//  	Pick an arbitrary use-of as part of fileManager's identification of a detected cycle.
//  	Returns that use-of. In the impossible event that none is available, an error has occurred
//  	and this usage is changed to independent breaking a cycle. 0 is then returned.
//
FogTargetFile* FogTargetFile::pick_cyclic_use_of(FogFileManager& fileManager) {
	FogTargetFileConstMapOfRefIterator p(_use_of);
	
	if (p)
		return &*p;
		
	WRNMSG("BUG - " << viz(*this) << " was independent.");
	fileManager.promote_to_independent(*this);
	return 0;
}

void FogTargetFile::precompile(FogFileManager& fileManager) {
	if (!_use_of.tally())
		fileManager.add_independent(*this);
	else
		fileManager.add_dependent(*this);
		
	_external_files = _use_of;
	
	if (_internals.tally()) {
		_internals.sort(&FogUsage::compare_least_dependent_first);
		_max_usage_number = _internals[_internals.tally() - 1]->usage_number();
		_min_usage_number = _internals[0]->usage_number();
	}
}

std::ostream& FogTargetFile::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_use_of.tally()) {
		s << indent(aDepth) << "uses\n";
		_use_of.print_members(s, aDepth + 1);
	}
	
	if (_used_by.tally()) {
		s << indent(aDepth) << "used by\n";
		_used_by.print_members(s, aDepth + 1);
	}
	
	if (_external_files.tally()) {
		s << indent(aDepth) << "external files\n";
		_external_files.print_members(s, aDepth + 1);
	}
	else if (_external_map.tally()) {
		s << indent(aDepth) << "externals\n";
		_external_map.print_members(s, aDepth + 1);
	}
	
	if (_internals.tally()) {
		s << indent(aDepth) << "internals\n";
		_internals.print_members(s, aDepth + 1);
	}
	
	if (_visible_files.tally()) {
		s << indent(aDepth) << "visible files\n";
		_visible_files.print_members(s, aDepth + 1);
	}
	
	if (_visible_names.tally()) {
		s << indent(aDepth) << "visible names\n";
		_visible_names.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogTargetFile::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_use_of.tally()) {
		s << indent(aDepth) << "uses\n";
		_use_of.print_members(s, aDepth + 1);
	}
	
	if (_used_by.tally()) {
		s << indent(aDepth) << "used by\n";
		_used_by.print_members(s, aDepth + 1);
	}
	
	if (_external_files.tally()) {
		s << indent(aDepth) << "external files\n";
		_external_files.print_members(s, aDepth + 1);
	}
	else if (_external_map.tally()) {
		s << indent(aDepth) << "externals\n";
		_external_map.print_members(s, aDepth + 1);
	}
	
	if (_internals.tally()) {
		s << indent(aDepth) << "internals\n";
		_internals.print_members(s, aDepth + 1);
	}
	
	if (_visible_files.tally()) {
		s << indent(aDepth) << "visible files\n";
		_visible_files.print_members(s, aDepth + 1);
	}
	
	if (_visible_names.tally()) {
		s << indent(aDepth) << "visible names\n";
		_visible_names.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogTargetFile::print_this(std::ostream& s) const {
	s << _file_number << ", " << _min_usage_number << ':' << _max_usage_number;
	s << ", " << unique_id() << ", " << utility();
	return s;
}

std::ostream& FogTargetFile::print_viz(std::ostream& s) const {
	//  	return s << "\"file: " << c_string(_pathed_id.str()) << '"';
	return s << c_string(_pathed_id.str());
}

//
//  	Eliminate all uses of _file, once _file has been allocated a file independence number.
//
void FogTargetFile::remove_all_uses(FogFileManager& fileManager, size_t fileNumber) {
	_file_number = fileNumber;
	CONDMSG(Fog::debug_file(), *this << " assigned file independence number " << _file_number);
	
	for (FogTargetFileConstMapOfRefIterator p(_used_by); p; p++)
		p->remove_just_use_of(*this, fileManager);
		
	_used_by.vacate();
}

//
//  	Eliminate the use-of and used-by relationship for the use of aFile._file by _file.
//  	This is a sub-method for remove_use_of and remove_all_uses. In order to avoid unnecessary
//  	map activity the used-by relationship is maintained separately so that a bulk elimination
//  	can be performed by remove_all_uses.
//
void FogTargetFile::remove_just_use_of(FogTargetFile& aFile, FogFileManager& fileManager) {
	if (!_use_of.remove(aFile.unique_id()))
		ERRMSG("BUG - should not remove un-use-of " << viz(aFile) << " from " << viz(*this));
		
	CONDMSG(Fog::debug_file(), "Removed use of " << aFile << " by " << *this);
	
	if (!_use_of.tally())
		fileManager.promote_to_independent(*this);
}

//
//  	Eliminate the use-of and used-by relationship for the use of aFile._file by _file, once
//  	aDependency._file has been allocated a file independence number, removing an impediment on
//  	allocating a file independence number to _file.
//
void FogTargetFile::remove_use_of(FogTargetFile& aFile, FogFileManager& fileManager) {
	if (!aFile._used_by.remove(unique_id()))
		ERRMSG("BUG - should not remove un-used-by unused " << viz(*this) << " from " << viz(aFile));
		
	remove_just_use_of(aFile, fileManager);
}

void FogTargetFile::set_file_number(size_t aNumber) {
	_file_number = aNumber;
}

void FogTargetFile::set_guard(const PrimId& aGuard) {
	if (_guard && (_guard != aGuard))
		ERRMSG("Should not change guard of " << viz(*this)
		       << " from " << c_string(_guard.str()) << " to " << c_string(aGuard.str()));
		       
	_guard = aGuard;
	_explicit_guard = true;
}

void FogTargetFile::set_is_input() {
	_is_input = true;
}

void FogTargetFile::set_max_utility(const FogEntity& anEntity) {
	const FogUtility& aUtility = anEntity.utility();
	
	if (aUtility > utility())
		set_new_utility(aUtility);
}

void FogTargetFile::set_max_utility(const FogFileIdent& fileIdent) {
	const FogUtility& aUtility = fileIdent.utility();
	
	if (aUtility > utility())
		set_new_utility(aUtility);
}

void FogTargetFile::set_max_utility(const FogUtility& aUtility) {
	if (aUtility > utility())
		set_new_utility(aUtility);
}

void FogTargetFile::set_new_utility(const FogUtility& aUtility) {
	if (Fog::debug_utility())
		DIAMSG("Utility of " << viz(*this) << " changing from " << utility().str() << " to " << aUtility.str());
		
	_utility = aUtility;
}

FogUsage& FogTargetFile::usage(FogUsageManager& usageManager) {
	if (!_usage) {
		_usage = usageManager.make_file_usage(*this);
		
		if (!_usage)
			_usage = &FogUsage::mutable_null();
	}
	
	return *_usage;
}

void FogTargetFileNull::add_external(FogUsage& aUsage) { //   No error message since null-file used to black-hole an unwanted global name-space.
	//  	ERRMSG("BUG - should not add_external " << viz(aUsage) << " to null file.");
}

void FogTargetFileNull::add_include(FogTargetFile& aFile) {
	ERRMSG("BUG - should not add_include " << viz(aFile) << " to null file.");
}

void FogTargetFileNull::add_internal(FogUsage& aUsage) { //   No error message since null-file used to black-hole an unwanted global name-space.
	//  	ERRMSG("BUG - should not add_internal " << viz(aUsage) << " to null file.");
}

void FogTargetFileNull::compile_bottom() {
	ERRMSG("BUG - should not compile bottom of null file.");
}

void FogTargetFileNull::compile_top() {
	ERRMSG("BUG - should not compile top of null file.");
}

void FogTargetFileNull::precompile(FogFileManager& fileManager) {
	ERRMSG("BUG - should not precompile null file.");
}

void FogTargetFileNull::set_file_number(size_t aNumber) {
	ERRMSG("BUG - should not set_file_number " << aNumber << " for null file.");
}

void FogTargetFileNull::set_guard(const PrimId& aGuard) {
	ERRMSG("BUG - should not set_guard << " << aGuard << " for  null file.");
}

void FogTargetFileNull::set_is_input() {
	ERRMSG("BUG - should not set_is_input null file.");
}

void FogTargetFileNull::set_new_utility(const FogUtility& aUtility) {
	ERRMSG("BUG - should not set_new_utility " << aUtility << " for  null file.");
}

