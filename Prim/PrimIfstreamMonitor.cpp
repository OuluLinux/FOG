#include <Prim/PrimIncludeAll.h>






TYPEINFO_SINGLE(PrimIfstreamMonitor, PrimMonitor)

PrimIfstreamMonitor::PrimIfstreamMonitor(const char* aPrefix, const PrimString& fileName, const char* aSuffix,
        std::istream* sDefault, const char* fDefault)
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
	
	_file.reset(new std::ifstream(fileName.str()));
	PrimError anError;
	
	if (!_file.get()) {
		ERRMSG("Failed to allocate input file buffer for \"" << fileName << "\"");
		_s = &null_stream();
		return;
	}
	
	if (!*_file.get()) {
		ERRMSG("Failed to open \"" << fileName << "\", " << anError);
		_s = &null_stream();
		return;
	}
	
	_s = _file.get();
	_ok = _s != 0;
}

std::istream& PrimIfstreamMonitor::null_stream() {
	static std::ifstream nullStream("/dev/null");
	return nullStream;
}

std::ostream& PrimIfstreamMonitor::print_message(std::ostream& s) const {
	if (_prefix)
		s << _prefix << " ";
		
	s << c_string(_file_name.str());
	
	if (_suffix)
		s << _suffix;
		
	return s;
}

std::ostream& PrimIfstreamMonitor::print_this(std::ostream& s) const {
	return s << _file_name;
}
