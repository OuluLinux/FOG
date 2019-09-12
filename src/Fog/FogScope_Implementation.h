
#ifndef FOGSCOPE_IMPLEMENTATION_HXX
#define FOGSCOPE_IMPLEMENTATION_HXX




class FogScope_Implementation {

private:
	FogTargetFileListOfRef _used_files;
	bool _no_implementation : 1;
	
private:
	FogScope_Implementation(const FogScope_Implementation&);
	
public:
	FogScope_Implementation();
	void add_use_for_implementation(FogTargetFile& aFile) {
		_used_files.add(aFile);
	}
	
	void create_usages(FogScope& inScope);
	void destroy() {
		_used_files.vacate(); /*   _code.reset(); */
	}
	
	void do_compile(FogCompileContext& inScope, FogTargetFile& implementationFile);
	void emit_implementation(FogEmitContext& emitContext) const;
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
	void set_no_implementation() {
		_no_implementation = true;
	}
	
	bool is_no_implementation() const {return _no_implementation;}
	
//  	void set_code(const FogList& someCode);
};

#endif

