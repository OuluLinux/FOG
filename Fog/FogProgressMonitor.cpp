#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogProgressMonitor, Super)

FogProgressMonitor::FogProgressMonitor(const char* aPrefix, const FogToken& aToken, const char* aSuffix)
	:
	_prefix(aPrefix),
	_token(aToken),
	_suffix(aSuffix) {
	//set_redirect(monitor());
	VERBOSE(
	        print_this(std::cout);
	        std::cout << std::endl;)
}

bool FogProgressMonitor::post_error(const PrimNotice_Error& aNotice) const {
	const PrimNotifier* thisNotifier = this;     //   Visual C++ V6 workaround.
	
	if (thisNotifier != global_notice_board())
		return global_notice_board()->post_error(aNotice);
	else
		return Super::post_error(aNotice);
}

bool FogProgressMonitor::post_warning(const PrimNotice_Warning& aNotice) const {
	const PrimNotifier* thisNotifier = this;     //   Visual C++ V6 workaround.
	
	if (thisNotifier != global_notice_board())
		return global_notice_board()->post_warning(aNotice);
	else
		return Super::post_warning(aNotice);
}

std::ostream& FogProgressMonitor::print_message_line(std::ostream& s, int aDepth) const {
	s << indent(aDepth);
	
	if (_prefix)
		s << _prefix << " ";
		
	//  	if (_token.is_list())
	//  		_token.print_source(s, aDepth + (_prefix ? (strlen(_prefix) + 1)/PrimIndent::spaces_per_indent() : 0));
	//  	else
	s << viz(_token);
	
	if (_suffix)
		s << _suffix;
		
	s << std::endl;
	return s;
}

std::ostream& FogProgressMonitor::print_this(std::ostream& s) const {
	if (_prefix)
		s << _prefix << " ";
		
	s << viz(_token);
	
	if (_suffix)
		s << _suffix;
		
	return s;
}
