#include <Fog/FogIncludeAll.h>









FogScope_Interface::FogScope_Interface()
	:
	_no_interface(false),
	_has_own_interface(false) {}

void FogScope_Interface::create_usages(FogScope& inScope) {
	//  	const void *const codeOk = _code;			// DevStudio V5 workaround
	//  	if (!no_interface() && codeOk)
	//  		inScope.interface_usage_start().add_use_by(inScope.interface_usage_finish());
	if (_used_files.tally()) {
		FogUsage& interfaceUsage = inScope.interface_usage_start();
		FogUsageManager& usageManager = inScope.usage_manager();
		
		for (FogTargetFileConstListOfRefIterator p(_used_files); p; p++) {
			FogTargetFile& aFile = *p;
			aFile.usage(usageManager).add_use_by(interfaceUsage);
		}
	}
}

void FogScope_Interface::do_compile(FogCompileContext& inScope, FogTargetFile& interfaceFile) {
	for (FogTargetFileConstListOfRefIterator p(_used_files); p; p++) {
		FogTargetFile& aFile = *p;
		
		if (&aFile != &interfaceFile)
			interfaceFile.add_include(aFile);
	}
}

void FogScope_Interface::emit_interface(FogEmitContext& emitContext) const {
	//  	const void *const codeOk = _code;			// DevStudio V5 workaround
	//  	if (!no_interface() && codeOk)
	//  		_code->emit(emitContext);
}

std::ostream& FogScope_Interface::print_depth(std::ostream& s, int aDepth) const {
	if (_used_files.tally()) {
		s << indent(aDepth) << "files used for interface\n";
		_used_files.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Interface::print_members(std::ostream& s, int aDepth) const {
	if (_used_files.tally()) {
		s << indent(aDepth) << "files used for interface\n";
		_used_files.print_members(s, aDepth + 1);
	}
	
	return s;
}

//  void FogScope_Interface::set_code(const FogList& someCode)
//  {
//  	if (_code)
//  		ERRMSG("BUG -- No support for multiple code contributions to interface.");
//  	_code = someCode;
//  }
