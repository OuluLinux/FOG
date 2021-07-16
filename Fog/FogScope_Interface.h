
#ifndef FOGSCOPE_INTERFACE_HXX
#define FOGSCOPE_INTERFACE_HXX




class FogScope_Interface {

private:
	FogTargetFileListOfRef _used_files;
//  	FogListRefToConst _code;						// Text to be added near the head of interface file.

	bool _no_interface : 1;       //   True if generation of an interface suppressed.
	bool _has_own_interface : 1;     //   True if scope has its own interface file.
	
private:
	FogScope_Interface(const FogScope_Interface&);
	
public:
	FogScope_Interface();
	void add_use_for_interface(FogTargetFile& aFile) {
		_used_files.add(aFile);
	}
	
	void create_usages(FogScope& inScope);
	void destroy() {
		_used_files.vacate(); /*   _code.reset(); */
	}
	
	void do_compile(FogCompileContext& inScope, FogTargetFile& interfaceFile);
	void emit_interface(FogEmitContext& emitContext) const;
	const FogTargetFileListOfRefToConst& files_used_for_interface() const {
		return _used_files;
	}
	
	bool has_own_interface() const {
		return _has_own_interface;
	}
	
	bool no_interface() const {
		return _no_interface;
	}
	
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
//  	void set_code(const FogList& someCode);
	void set_no_interface() {
		_no_interface = true;
	}
};

#endif

