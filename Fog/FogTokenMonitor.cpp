#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogTokenMonitor, Super)

FogTokenMonitor::FogTokenMonitor(const FogToken& aToken)
	:
	_token(aToken)
{}

bool FogTokenMonitor::post_error(const PrimNotice_Error& aNotice) const {
	const PrimNotifier* thisNotifier = this;     //   Visual C++ V6 workaround.
	
	if (thisNotifier != global_notice_board())
		return global_notice_board()->post_error(aNotice);
	else
		return Super::post_error(aNotice);
}

bool FogTokenMonitor::post_warning(const PrimNotice_Warning& aNotice) const {
	const PrimNotifier* thisNotifier = this;     //   Visual C++ V6 workaround.
	
	if (thisNotifier != global_notice_board())
		return global_notice_board()->post_warning(aNotice);
	else
		return Super::post_warning(aNotice);
}

std::ostream& FogTokenMonitor::print_message(std::ostream& s) const {
	const FogLine& aLine = _token.line();
	return s << "At " << viz(aLine) << ": " << viz(_token) << '.';
}

std::ostream& FogTokenMonitor::print_this(std::ostream& s) const {
	const FogLine& aLine = _token.line();
	s << viz(aLine) << ", " << viz(_token);
	return s << viz(aLine) << ", " << viz(_token);
}
