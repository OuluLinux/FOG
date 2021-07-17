#include <Fog/FogIncludeAll.h>












struct FogDefine : public PrimPrint {
	typedef PrimPrint Super;
	typedef FogDefine This;
	TYPEDECL_SINGLE(This, Super)
	PRIMSHARES_DECLS(This)
	PRIMREF_DECLS(This)
	
private:
	const PrimIdHandle _id;
	const PrimStringHandle _string;
	
protected:
	FogDefine() {}
	
public:
	FogDefine(const PrimId& anId, const PrimString& aString) : _id(anId), _string(aString) {}
	
	const PrimId& id() const {
		return *_id;
	}
	
	const PrimString& string() const {
		return *_string;
	}
};

TYPEINFO_SINGLE(FogDefine, Super)
PRIMREF_NULL_CLASS_AND_IMPL(FogDefine)
PRIMSHARES_IMPL(FogDefine)
PRIMREFS_IMPL(FogDefine)
PRIMMAPOFREFS_IMPL(FogDefine)
TMPL_HACK_FIX_DO(FogDefine)

int Fog::_indent_size = 4;
bool Fog::_indent_size_ok = false;
int Fog::_inline_threshold = 4;
bool Fog::_inline_threshold_ok = false;
int Fog::_multi_byte_count = 2;
bool Fog::_multi_byte_count_ok = false;
int Fog::_tab_size = 8;
bool Fog::_tab_size_ok = false;

PrimStringHandle& Fog::anon_prefix() {
	static const char *defaultText = "_anon_";
	static PrimGetEnv<const char *> someText("Fog::anon_prefix", defaultText);
	static PrimStringHandle aString(someText);
	return aString;
}

const PrimId& Fog::assign_id() {
	static const PrimIdHandle anId("operator=");
	return *anId;
}

const PrimId& Fog::at_id() {
	static const PrimIdHandle anId("@");
	return *anId;
}

const PrimId& Fog::at_this_id() {
	static const PrimIdHandle anId("@This");
	return *anId;
}

const PrimId& Fog::auto_id() {
	static const PrimIdHandle anId("auto");
	return *anId;
}

bool& Fog::comment_line_numbers() {
	static PrimGetEnv<bool> commentLines("Fog::comment_line_numbers", false);
	return commentLines;
}

bool& Fog::cplusplus() {
	static PrimGetEnv<bool> cPlusPlus("Fog::cplusplus", false);
	return cPlusPlus;
}

bool& Fog::debug_activity() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_activity", false);
	return debugFlag;
}

bool& Fog::debug_actual() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_actual", false);
	return debugFlag;
}

bool& Fog::debug_file() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_file", false);
	return debugFlag;
}

bool& Fog::debug_input() {
	return PrimLexerContext::debug_input();
}

bool& Fog::debug_lex() {
	return PrimLexerContext::debug_lex();
}

bool& Fog::debug_lex2dollar() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_lex2dollar", false);
	return debugFlag;
}

bool& Fog::debug_lex2hash() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_lex2hash", false);
	return debugFlag;
}

bool& Fog::debug_lex2locate() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_lex2substitute", false);
	return debugFlag;
}

bool& Fog::debug_lex2main() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_lex2hash", PrimLexerContext::debug_lex2yacc());
	return debugFlag;
}

bool& Fog::debug_lex2replace() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_lex2replace", false);
	return debugFlag;
}

bool& Fog::debug_lex2substitute() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_lex2substitute", false);
	return debugFlag;
}

bool& Fog::debug_purity() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_purity", false);
	return debugFlag;
}

bool& Fog::debug_repeat() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_repeat", false);
	return debugFlag;
}

bool& Fog::debug_scope() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_scope", false);
	return debugFlag;
}

bool& Fog::debug_utility() {
	static PrimGetEnv<bool> debugFlag("Fog::debug_utility", false);
	return debugFlag;
}

bool& Fog::debug_yacc() {
	return PrimLexerContext::debug_yacc();
}

bool& Fog::emit_preheader() {
	static PrimGetEnv<bool> emit_preheader("Fog::emit_preheader", false);
	return emit_preheader;
}

bool& Fog::use_test_breakers() {
	static PrimGetEnv<bool> emit_preheader("Fog::use_test_breakers", false);
	return emit_preheader;
}

PrimText& Fog::define_strings() {
	static PrimText defineStrings;
	return defineStrings;
}

PrimStringHandle& Fog::extern_prefix() {
	static const char *defaultText = "_extern_";
	static PrimGetEnv<const char *> someText("Fog::extern_prefix", defaultText);
	static PrimStringHandle aString(someText);
	return aString;
}

bool& Fog::force() {
	static PrimGetEnv<bool> forceFlag("Fog::force", false);
	return forceFlag;
}

int& Fog::indent_size() {
	static bool initialised = false;
	
	if (!initialised) {
		initialised = true;
		static const PrimGetEnv<int> indentSize("Fog::indent_size", 4);
		static const int& indentSizeRef = indentSize;  //   DevStudio V5 workaround.
		_indent_size = indentSizeRef;
	}
	
	_indent_size_ok = false;    //   Every time since use of the reference is uncontrolled.
	
	return _indent_size;
}

int& Fog::inline_threshold() {
	static bool initialised = false;
	
	if (!initialised) {
		initialised = true;
		static const PrimGetEnv<int> inlineThreshold("Fog::inline_threshold", 10);
		static const int& inlineThresholdRef = inlineThreshold;  //   DevStudio V5 workaround.
		_inline_threshold = inlineThresholdRef;
	}
	
	_inline_threshold_ok = false;    //   Every time since use of the reference is uncontrolled.
	
	return _inline_threshold;
}

bool& Fog::long_long_type() {
	static PrimGetEnv<bool> longLong("Fog::long_long_type", true);
	return longLong;
}

bool& Fog::microsoft_extensions() {
	static PrimGetEnv<bool> microsoftExtensions("Fog::microsoft_extensions", false);
	return microsoftExtensions;
}

int& Fog::multi_byte_count() {
	static PrimGetEnv<int> multiByteCount("Fog::multi_byte_count", 2);
	return multiByteCount;
}

const PrimId& Fog::name_space_id() {
	static const PrimIdHandle anId("NameSpace");
	return *anId;
}

bool& Fog::nobanner() {
	static PrimGetEnv<bool> noBanner("Fog::nobanner", false);
	return noBanner;
}

bool& Fog::no_access() {
	static PrimGetEnv<bool> noAccess("Fog::no_access", false);
	return noAccess;
}

bool& Fog::no_bool_type() {
	static PrimGetEnv<bool> noBool("Fog::no_bool_type", false);
	return noBool;
}

bool& Fog::no_explanations() {
	static PrimGetEnv<bool> noExplanations("Fog::no_explanations", false);
	return noExplanations;
}

bool& Fog::no_line_numbers() {
	static PrimGetEnv<bool> noLines("Fog::no_line_numbers", false);
	return noLines;
}

bool& Fog::no_namespace() {
	static PrimGetEnv<bool> noNamespace("Fog::no_namespace", false);
	return noNamespace;
}

bool& Fog::no_specialisation_prefix() {
	static PrimGetEnv<bool> noPrefix("Fog::no_specialisation_prefix", false);
	return noPrefix;
}

bool& Fog::no_using() {
	static PrimGetEnv<bool> noUsing("Fog::no_using", false);
	return noUsing;
}

bool& Fog::no_wchar_t_type() {
	static PrimGetEnv<bool> noWcharT("Fog::no_wchar_t_type", false);
	return noWcharT;
}

bool& Fog::notify_create() {
	static PrimGetEnv<bool> notifyCreate("Fog::notify_create", false);
	return notifyCreate;
}

bool& Fog::notify_equivalence() {
	static PrimGetEnv<bool> notifyEquivalence("Fog::notify_equivalence", false);
	return notifyEquivalence;
}

FogRoot& Fog::null_root() {
	return FogRoot::mutable_null();
}

FogScope& Fog::null_scope() {
	return FogScope::mutable_null();
}

PrimStringHandle& Fog::path(IsInterface isInterface, IsTemplate isTemplate) {
	static const char *defaultPath = 0;
	
	if (isInterface) {
		static PrimGetEnv<const char *> someText1("Fog::interface_path", defaultPath);
		static PrimStringHandle aString1(someText1);
		
		if (!isTemplate)
			return aString1;
			
		static const char *defaultText1 = aString1 ? aString1.str() : 0;
		
		static PrimGetEnv<const char *> moreText1("Fog::template_interface_path", defaultText1);
		
		static PrimStringHandle bString1(moreText1);
		
		const void *const aStringOk1 = aString1;    //   DevStudio V5 workaround.
		
		if (!bString1 && aStringOk1)
			bString1 = aString1;
			
		return bString1;
	}
	
	else {
		static PrimGetEnv<const char *> someText2("Fog::implementation_path", defaultPath);
		static PrimStringHandle aString2(someText2);
		
		if (!isTemplate)
			return aString2;
			
		static const char *defaultText2 = aString2 ? aString2.str() : 0;
		
		static PrimGetEnv<const char *> moreText2("Fog::template_implementation_path", defaultText2);
		
		static PrimStringHandle bString2(moreText2);
		
		const void *const aStringOk2 = aString2;    //   DevStudio V5 workaround.
		
		if (!bString2 && aStringOk2)
			bString2 = aString2;
			
		return bString2;
	}
}

PrimStringHandle& Fog::prefix(IsInterface isInterface, IsTemplate isTemplate) {
	static const char *defaultText = 0;
	
	if (isInterface) {
		static PrimGetEnv<const char *> someText("Fog::interface_prefix", defaultText);
		static PrimStringHandle aString(someText);
		
		if (!isTemplate)
			return aString;
			
		static const char *defaultText = aString ? aString.str() : 0;
		
		static PrimGetEnv<const char *> moreText("Fog::template_interface_prefix", defaultText);
		
		static PrimStringHandle bString(moreText);
		
		const void *const aStringOk = aString;    //   DevStudio V5 workaround.
		
		if (!bString && aStringOk)
			bString = aString;
			
		return bString;
	}
	
	else {
		static PrimGetEnv<const char *> someText("Fog::implementation_prefix", defaultText);
		static PrimStringHandle aString(someText);
		
		if (!isTemplate)
			return aString;
			
		static const char *defaultText = aString ? aString.str() : 0;
		
		static PrimGetEnv<const char *> moreText("Fog::template_implementation_prefix", defaultText);
		
		static PrimStringHandle bString(moreText);
		
		const void *const aStringOk = aString;    //   DevStudio V5 workaround.
		
		if (!bString && aStringOk)
			bString = aString;
			
		return bString;
	}
}

bool& Fog::readonly() {
	static PrimGetEnv<bool> readOnly("Fog::readonly", false);
	return readOnly;
}

const PrimId& Fog::scope_id() {
	static const PrimIdHandle anId("Scope");
	return *anId;
}

PrimStringHandle& Fog::separator() {
	static const char *defaultText = "/";
	static PrimGetEnv<const char *> someText("Fog::separator", defaultText);
	static PrimStringHandle aString(someText);
	return aString;
}

const PrimId& Fog::std_id() {
	static const PrimIdHandle anId("std");
	return *anId;
}

PrimStringHandle& Fog::suffix(IsInterface isInterface, IsTemplate isTemplate) {
	if (isInterface) {
		static const char hxxText[] = ".hxx";
		static PrimGetEnv<const char *> someText("Fog::interface_suffix", hxxText);
		static PrimStringHandle aString(someText);
		
		if (!isTemplate)
			return aString;
			
		static const char *hText = ".H"; //   interface_suffix().str();
		
//  		static const char *hText = aString ? aString.str() : 0;
		static PrimGetEnv<const char *> moreText("Fog::template_interface_suffix", hText);
		
		static PrimStringHandle bString(moreText);
		
		return bString;
	}
	
	else {
		static const char cxxText[] = ".cxx";
		static PrimGetEnv<const char *> someText("Fog::implementation_suffix", cxxText);
		static PrimStringHandle aString(someText);
		
		if (!isTemplate)
			return aString;
			
		static const char *cText = ".C"; //   implementation_suffix().str();
		
//  		static const char *cText = aString ? aString.str() : 0;
		static PrimGetEnv<const char *> moreText("Fog::template_implementation_suffix", cText);
		
		static PrimStringHandle bString(moreText);
		
		return bString;
	}
}

const PrimId& Fog::super_id() {
	static const PrimIdHandle anId("Super");
	return *anId;
}

int& Fog::tab_size() {
	static bool initialised = false;
	
	if (!initialised) {
		initialised = true;
		static const PrimGetEnv<int> tabSize("Fog::tab_size", 4);
		static const int& tabSizeRef = tabSize;  //   DevStudio V5 workaround.
		_tab_size = tabSizeRef;
	}
	
	_tab_size_ok = false;      //   Every time since use of the reference is uncontrolled.
	
	return _tab_size;
}

PrimStringHandle& Fog::template_parameter_prefix() {
	static const char *defaultText = "_";
	static PrimGetEnv<const char *> someText("Fog::template_parameter_prefix", defaultText);
	static PrimStringHandle aString(someText);
	return aString;
}

const PrimId& Fog::that_id() {
	static const PrimIdHandle anId("That");
	return *anId;
}

const PrimId& Fog::this_id() {
	static const PrimIdHandle anId("This");
	return *anId;
}

const PrimId& Fog::twiddle_at_this_id() {
	static const PrimIdHandle anId("~@This");
	return *anId;
}

bool& Fog::unnest() {
	static PrimGetEnv<bool> unNest("Fog::unnest", false);
	return unNest;
}

size_t Fog::validate_indent_size() {
	if (!_indent_size_ok)       //   Ensure PrimGetEnv gets a first call.
		indent_size();
		
	if (_indent_size <= 1) {
		WRNMSG("Illegal indent size adjusted to 4.");
		_indent_size = 4;
	}
	
	if (_indent_size > 32) {
		WRNMSG("Excessive indent size adjusted to 32.");
		_indent_size = 32;
	}
	
	_indent_size_ok = true;
	
	return size_t(_indent_size);
}

size_t Fog::validate_multi_byte_count() {
	if (!_multi_byte_count_ok)       //   Ensure PrimGetEnv gets a first call.
		multi_byte_count();
		
	if (_multi_byte_count < 1) {
		WRNMSG("Illegal multi_byte_count adjusted to 1.");
		_multi_byte_count = 1;
	}
	
	if (_multi_byte_count > MAX_MULTI_BYTE_COUNT) {
		WRNMSG("Excessive multi_byte_count adjusted to " << MAX_MULTI_BYTE_COUNT << '.');
		_multi_byte_count = MAX_MULTI_BYTE_COUNT;
	}
	
	_multi_byte_count_ok = true;
	
	return size_t(_multi_byte_count);
}

size_t Fog::validate_tab_size() {
	if (!_tab_size_ok)       //   Ensure PrimGetEnv gets a first call.
		tab_size();
		
	if (_tab_size <= 1) {
		WRNMSG("Illegal tab-size adjusted to 8.");
		_tab_size = 8;
	}
	
	_tab_size_ok = true;
	
	return size_t(_tab_size);
}

const PrimId& Fog::void_meta_type() {
	static const PrimIdHandle anId("void");
	return *anId;
}
