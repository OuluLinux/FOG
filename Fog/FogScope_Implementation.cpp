#include <Fog/FogIncludeAll.h>










FogScope_Implementation::FogScope_Implementation() :
	_no_implementation(false) {
}

void FogScope_Implementation::create_usages(FogScope& inScope) {
	//  	if (_code)
	//  		inScope.interface_usage_finish().add_use_by(inScope.implementation_usage());
	if (_used_files.tally()) {
		FogUsage& inlineUsage = inScope.inline_usage();
		FogUsageManager& usageManager = inScope.usage_manager();
		
		for (FogTargetFileConstListOfRefIterator p(_used_files); p; p++) {
			FogTargetFile& aFile = *p;
			aFile.usage(usageManager).add_use_by(inlineUsage);
		}
	}
}

void FogScope_Implementation::do_compile(FogCompileContext& inScope, FogTargetFile& implementationFile) {
	for (FogTargetFileConstListOfRefIterator p(_used_files); p; p++) {
		FogTargetFile& aFile = *p;
		
		if (&aFile != &implementationFile)
			implementationFile.add_include(aFile);
	}
}

void FogScope_Implementation::emit_implementation(FogEmitContext& emitContext) const {
	//  	if (_code)
	//  		_code->emit(emitContext);
}

std::ostream& FogScope_Implementation::print_depth(std::ostream& s, int aDepth) const {
	if (_used_files.tally()) {
		s << indent(aDepth) << "files used for implementation\n";
		_used_files.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Implementation::print_members(std::ostream& s, int aDepth) const {
	if (_used_files.tally()) {
		s << indent(aDepth) << "files used for implementation\n";
		_used_files.print_members(s, aDepth + 1);
	}
	
	return s;
}

//  void FogScope_Implementation::set_code(const FogList& someCode)
//  {
//  	if (_code)
//  		ERRMSG("BUG -- No support for multiple code contributions to implementation.");
//  	_code = someCode;
//  }
