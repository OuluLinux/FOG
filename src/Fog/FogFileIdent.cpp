#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogFileIdent, Super)

FogFileIdent::FogFileIdent(const char *fileStem, const PrimId& templateArguments)
		:
		_is_implementation(false),
		_is_interface(false),
		_is_template(false),
		_explicit_name(false),
		_explicit_guard(false),
		_utility(FogUtility::pool_utility()) {
	if (templateArguments) {
		PrimOstrstream fileName;
		fileName << fileStem;
		
		for (const char *p = templateArguments.str(); *p; p++)
			fileName << (isalnum(*p) ? *p : '_');
			
		_name = fileName.str();
	}
	
	else
		_name = fileStem;
}

FogFileIdent::~FogFileIdent() {}

FogTargetFile *FogFileIdent::make(FogScopeContext& scopeContext) const {
	FogFileManager& fileManager = scopeContext.file_manager();
	return make_target_file(fileManager);
}

FogTargetFile *FogFileIdent::make_target_file(FogFileManager& fileManager) const {
	if (!_path) {
		const PrimString& thePath = *Fog::path(_is_interface ? Fog::IS_INTERFACE : Fog::IS_IMPLEMENTATION,
				_is_template ? Fog::IS_TEMPLATE : Fog::NOT_TEMPLATE);
		        
		if (thePath)
			mutate()._path = thePath.str();
	}
	
	if (!_prefix) {
		const PrimString& thePrefix = *Fog::prefix(_is_interface ? Fog::IS_INTERFACE : Fog::IS_IMPLEMENTATION,
				_is_template ? Fog::IS_TEMPLATE : Fog::NOT_TEMPLATE);
		        
		if (thePrefix)
			mutate()._prefix = thePrefix.str();
	}
	
	if (!_suffix) {
		const PrimString& theSuffix = *Fog::suffix(_is_interface ? Fog::IS_INTERFACE : Fog::IS_IMPLEMENTATION,
				_is_template ? Fog::IS_TEMPLATE : Fog::NOT_TEMPLATE);
		        
		if (theSuffix)
			mutate()._suffix = theSuffix.str();
	}
	
	PrimOstrstream fileName;
	
	if (_prefix && (_is_interface || _is_implementation))
		fileName << _prefix;
		
	fileName << _name;
	
	if (_suffix && (_is_interface || _is_implementation))
		fileName << _suffix;
		
	const PrimId& fileNameId = fileName.id();
	
	FogTargetFile *aFile = fileManager.make_target_file(*_path, fileNameId);
	
//  	if (!aFile && !_path)
//  		aFile = fileManager.make_target_file(*Fog::path(
//  							_is_interface ? Fog::IS_INTERFACE : Fog::IS_IMPLEMENTATION,
//  						 	 _is_template ? Fog::IS_TEMPLATE : Fog::NOT_TEMPLATE), fileNameId);
	if (aFile) {
		if (is_input())
			aFile->set_is_input();
			
		if (_explicit_guard)
			aFile->set_guard(*_guard);
			
		aFile->set_max_utility(*this);
	}
	
	else
		if (_path)
			ERRMSG("BUG - Failed to create " << c_string(fileNameId.str())
				   << " on path " << c_string(_path->str()));
		else {
			PrimOstrstream s;
			s << "Failed to resolve " << c_string(fileNameId.str());
			static bool doneMessage = false;
			
			if (!doneMessage) {
				doneMessage = true;
				s << "\n\tuse -I <path> to specify access path to an existing file"
				<< "\n\tor use /path= to specify an explicit path"
				<< "\n\tor use -I. to enable creation in current directory";
			}
			
			ERRMSGZ(s.str());
		}
		
	return aFile;
}

std::ostream& FogFileIdent::print_this(std::ostream& s) const {
	if (_path)
		s << _path << ' ' << Fog::separator() << ' ';
		
	if (_prefix)
		s << _prefix;
		
	s << _name << _suffix;
	
	return s;
}

std::ostream& FogFileIdent::print_viz(std::ostream& s) const {
	s << "\"file \"";
	print_this(s);
	s <<'\"';
	return s;
}

void FogFileIdent::set_guard(const PrimId& anId) {
	if (_guard && (*_guard != anId))
		ERRMSG("Should not change file guard of " << viz(*this)
			   << " from " << c_string(_guard.str()) << " to " << c_string(anId.str()));
		       
	_guard = anId;
	
	_explicit_guard = true;
}

void FogFileIdent::set_name(const PrimId& anId) {
	if (_name && (*_name != anId) && !_explicit_name)
		ERRMSG("Should not change file name of " << viz(*this)
			   << " from " << c_string(_name.str()) << " to " << c_string(anId.str()));
		       
	_name = anId;
	
	_explicit_name = true;
}

void FogFileIdent::set_new_utility(const FogUtility& aUtility) {
	if (Fog::debug_utility())
		DIAMSG("Utility of " << viz(*this) << " changing from " << utility().str() << " to " << aUtility.str());
		
	_utility = aUtility;
}

void FogFileIdent::set_path(const PrimId& anId) {
	if (_path && (*_path != anId))
		ERRMSG("Should not change file path of " << viz(*this)
			   << " from " << c_string(_path.str()) << " to " << c_string(anId.str()));
		       
	_path = anId;
}

void FogFileIdent::set_prefix(const PrimId& anId) {
	if (_prefix && (*_prefix != anId))
		ERRMSG("Should not change file prefix of " << viz(*this)
			   << " from " << c_string(_prefix.str()) << " to " << c_string(anId.str()));
		       
	_prefix = anId;
}

void FogFileIdent::set_suffix(const PrimId& anId) {
	if (_suffix && (*_suffix != anId))
		ERRMSG("Should not change file suffix of " << viz(*this)
			   << " from " << c_string(_suffix.str()) << " to " << c_string(anId.str()));
		       
	_suffix = anId;
}
