
#ifndef FOGTOKENTYPE_HXX
#define FOGTOKENTYPE_HXX

#undef CONST
#undef DELETE
#undef FALSE
#undef TRUE
#undef THIS
#undef PURE
#undef VOID

class FogTokenType {

public:
	//
	//  	The TokenType enum enumerates the formatting style to be used.
	//
	enum TokenType {
#include "FogTokenTypeEnum.h"
	};
	
protected:

	friend class FogDeclSpecifier;
	
	friend class FogAccess;
	
	friend class FogUtility;
	
	friend class FogTag;
	
	friend class FogVirtual;
	
	friend class FogSegment;
	
	friend class FogCv;
	
	friend class FogInline;
	
	friend class FogStaticMask;
	
	friend class FogStatic;
	static void initialise();
	
public:
	static void add_built_in(const PrimId& anId);
	static TokenType character(unsigned char aChar) {
		return TokenType(aChar);
	}
	
	static FogToken& error();
	static FogToken& get(TokenType tokenType);
	static FogAccess& get_access(TokenType tokenType);
	static FogToken& get_character(unsigned char aChar) {
		return get(TokenType(aChar));
	}
	
	static FogCv& get_cv(TokenType tokenType);
	static FogDeclSpecifier& get_decl_specifier(TokenType tokenType);
	static FogInline& get_inline(TokenType tokenType);
	static FogSegment& get_segment(TokenType tokenType);
	static FogStatic& get_static(TokenType tokenType);
	static FogTag& get_tag(TokenType tokenType);
	static FogUtility& get_utility(TokenType tokenType);
	static FogVirtual& get_virtual(TokenType tokenType);
	static TokenType init() {
		return TokenType(0);
	}
	
	static FogToken& make_di_open_brace(size_t endColumn);
	static FogKeyword& make_identifier(const char* aBuffer);
	static FogToken& make_identifier(const char* aBuffer, size_t aLength);
	static FogKeyword& make_identifier(const PrimId& anId);
	static FogToken& make_literal_character(const char* aBuffer, size_t aLength);
	static FogToken& make_literal_string(const char* aBuffer, size_t aLength);
	static FogToken& make_macro_identifier(const PrimId& anId);
	static FogToken& make_narrow_character(const char* aBuffer, size_t aLength);
	static FogToken& make_narrow_string(const char* aBuffer, size_t aLength);
	static FogToken& make_number(const char* aBuffer, size_t aLength);
	static FogToken& make_open_brace(size_t endColumn);
	static FogToken& make_spacing(int someSpaces);
	static FogToken& make_wide_character(const char* aBuffer, size_t aLength);
	static FogToken& make_wide_string(const char* aBuffer, size_t aLength);
	static FogToken& nil();
	static FogToken& null();
	static void set_no_bool_type();
	static void set_no_wchar_t_type();
	static FogToken& zero();
};

#endif

