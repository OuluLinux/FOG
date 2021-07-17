#include <Prim/PrimIncludeAll.h>







TYPEINFO_SINGLE(PrimParserMonitor, PrimMonitor)

const PrimLineContext* PrimParserMonitor::_current_line_context = 0;

//
//  	The constructor caches the context message and currently installed monitor, installing this in its stead.
//
PrimParserMonitor::PrimParserMonitor() {}

PrimParserMonitor::PrimParserMonitor(Install doInstall)
	:
	Super(NO_INSTALL) {
	if (doInstall == INSTALL)
		install();
}

//
//  	The destructor reinstalls the less-nested monitor.
//
PrimParserMonitor::~PrimParserMonitor() {}

//
//  	Adopt lineContext as the current line context, establishing ownership in a local variable, and
//  	publishing the value via the static member for faster access.
//
void PrimParserMonitor::adopt_current_line_context(const PrimLineContext* lineContext) {
	PrimLineContextRefToConst aContext(lineContext, PrimLineContextRefToConst::ADOPT);
	set_current_line_context(lineContext);
}

//
//  	Set lineContext as the current line context, establishing shared ownership in a local variable, and
//  	publishing the value via the static member for faster access.
//
void PrimParserMonitor::set_current_line_context(const PrimLineContext* lineContext) {
	static PrimLineContextRefToConst currentLineContext;
	currentLineContext = lineContext;
	_current_line_context = lineContext;
}
