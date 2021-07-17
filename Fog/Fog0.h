
#ifndef FOG_HXX
#define FOG_HXX



class Fog {

public:
	enum IsInterface { IS_IMPLEMENTATION = false, IS_INTERFACE = true };
	enum IsTemplate { NOT_TEMPLATE = false, IS_TEMPLATE = true };
	enum MaxMultiByteCount { MAX_MULTI_BYTE_COUNT = sizeof(unsigned long) };
	
private:
	static int _indent_size;     //   Raw indentation size.
	static int _inline_threshold;    //   Token threshold for inline decisions.
	static int _multi_byte_count;    //   Raw multi-byte count.
	static int _tab_size;      //   Raw tab size.
	static bool _indent_size_ok;    //   True if _indent_size validated.
	static bool _inline_threshold_ok;   //   True if _inline_threshold validated.
	static bool _multi_byte_count_ok;   //   True if _multi_byte_count validated.
	static bool _tab_size_ok;     //   True if _tab_size validated.
	
private:
	static size_t validate_indent_size();
	static size_t validate_multi_byte_count();
	static size_t validate_tab_size();
	
public:
	static PrimStringHandle& anon_prefix();
	static const PrimId& at_id();
	static const PrimId& assign_id();
	static const PrimId& at_this_id();
	static const PrimId& auto_id();
	static bool& comment_line_numbers();
	static bool& cplusplus();
	static bool& debug_activity();
	static bool& debug_actual();
	static bool& debug_file();
	static bool& debug_input();
	static bool& debug_lex();
	static bool& debug_lex2dollar();
	static bool& debug_lex2hash();
	static bool& debug_lex2locate();
	static bool& debug_lex2main();
	static bool& debug_lex2replace();
	static bool& debug_lex2substitute();
	static bool debug_make() {
		return false;
	}
	
	static bool& debug_purity();
	static bool& debug_repeat();
	static bool& debug_scope();
	static bool& debug_utility();
	static bool& debug_yacc();
	static bool& emit_preheader();
	static bool& use_test_breakers();
	static PrimText& define_strings();
	static PrimStringHandle& extern_prefix();
	static bool& force();
	static size_t get_indent_size() {
		return _indent_size_ok ? _indent_size : validate_indent_size();
	}
	
	static size_t get_multi_byte_count() {
		return _multi_byte_count_ok ? _multi_byte_count : validate_multi_byte_count();
	}
	
	static size_t get_tab_size() {
		return _tab_size_ok ? _tab_size : validate_tab_size();
	}
	
	static PrimStringHandle& implementation_path() {
		return path(IS_IMPLEMENTATION, NOT_TEMPLATE);
	}
	
	static PrimStringHandle& implementation_prefix() {
		return prefix(IS_IMPLEMENTATION, NOT_TEMPLATE);
	}
	
	static PrimStringHandle& implementation_suffix() {
		return suffix(IS_IMPLEMENTATION, NOT_TEMPLATE);
	}
	
	static int& indent_size();
	static int& inline_threshold();
	static PrimStringHandle& interface_path() {
		return path(IS_INTERFACE, NOT_TEMPLATE);
	}
	
	static PrimStringHandle& interface_prefix() {
		return prefix(IS_INTERFACE, NOT_TEMPLATE);
	}
	
	static PrimStringHandle& interface_suffix() {
		return suffix(IS_INTERFACE, NOT_TEMPLATE);
	}
	
	static bool& long_long_type();
	static bool& microsoft_extensions();
	static int& multi_byte_count();
	static const PrimId& name_space_id();
	static bool& nobanner();
	static bool& no_access();
	static bool& no_bool_type();
	static bool& no_explanations();
	static bool& no_line_numbers();
	static bool& no_namespace();
	static bool& no_specialisation_prefix();
	static bool& no_using();
	static bool& no_wchar_t_type();
	static bool& notify_create();
	static bool& notify_equivalence();
	static FogRoot& null_root();
	static FogScope& null_scope();
	static PrimStringHandle& path(IsInterface isInterface, IsTemplate isTemplate);
	static PrimStringHandle& prefix(IsInterface isInterface, IsTemplate isTemplate);
	static bool& readonly();
	static const PrimId& scope_id();
	static PrimStringHandle& separator();
	static const PrimId& std_id();
	static PrimStringHandle& suffix(IsInterface isInterface, IsTemplate isTemplate);
	static const PrimId& super_id();
	static int& tab_size();
	static PrimStringHandle& template_implementation_path() {
		return path(IS_IMPLEMENTATION, IS_TEMPLATE);
	}
	
	static PrimStringHandle& template_implementation_prefix() {
		return prefix(IS_IMPLEMENTATION, IS_TEMPLATE);
	}
	
	static PrimStringHandle& template_implementation_suffix() {
		return suffix(IS_IMPLEMENTATION, IS_TEMPLATE);
	}
	
	static PrimStringHandle& template_interface_path() {
		return path(IS_INTERFACE, IS_TEMPLATE);
	}
	
	static PrimStringHandle& template_interface_prefix() {
		return prefix(IS_INTERFACE, IS_TEMPLATE);
	}
	
	static PrimStringHandle& template_interface_suffix() {
		return suffix(IS_INTERFACE, IS_TEMPLATE);
	}
	
	static PrimStringHandle& template_parameter_prefix();
	static const PrimId& that_id();
	static const PrimId& this_id();
	static const PrimId& twiddle_at_this_id();
	static bool& unnest();
	static const PrimId& void_meta_type();
};

#endif

