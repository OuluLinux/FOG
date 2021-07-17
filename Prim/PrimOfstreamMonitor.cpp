#include <Prim/PrimIncludeAll.h>





TYPEINFO_SINGLE(PrimOfstreamMonitor, PrimMonitor)

PrimOfstreamMonitor::PrimOfstreamMonitor(const char* aPrefix, const PrimString& fileName, const char* aSuffix,
        std::ostream* sDefault, const char* fDefault)
	:
	_prefix(aPrefix),
	_file_name(fileName),
	_suffix(aSuffix),
	_s(sDefault ? sDefault : & null_stream()),
	_ok(false) {
	if (!fileName) {
		_file_name = fDefault;
		_ok = _s != 0;
		return;
	}
	
#ifdef NEEDS_STD
	_file.reset(new std::ofstream(fileName.str(), std::ios::out, ofstream_open_mode()));
#else
	_file.reset(new std::ofstream(fileName.str(), std::ios::out));
#endif
	PrimError anError;
	
	if (!_file.get()) {
		ERRMSG("Failed to allocate output file buffer for \"" << fileName << "\"");
		_s = &null_stream();
		return;
	}
	
	if (!*_file.get()) {
		ERRMSG("Failed to create \"" << fileName << "\", " << anError);
		_s = &null_stream();
		return;
	}
	
	_s = _file.get();
	_ok = _s != 0;
}

std::ostream& PrimOfstreamMonitor::null_stream() {
	static std::ofstream nullStream("/dev/null");
	return nullStream;
}

std::ostream& PrimOfstreamMonitor::print_message(std::ostream& s) const {
	if (_prefix)
		s << _prefix << " ";
		
	s << c_string(_file_name.str());
	
	if (_suffix)
		s << _suffix;
		
	return s;
}
