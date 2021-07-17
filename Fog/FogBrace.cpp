#include <Fog/FogIncludeAll.h>









class FogDiOpenBrace : public FogBrace {
	typedef FogBrace Super;
	typedef FogDiOpenBrace This;
	TYPEDECL_SINGLE(This, Super)
	
public:
	FogDiOpenBrace(size_t endColumn = 0) : Super(endColumn) {}
	
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_stringized(std::ostream& s) const;
	virtual std::ostream& print_tokens(std::ostream& s, int aDepth) const;
	virtual size_t source_columns() const;
};

class FogOpenBrace : public FogBrace {
	typedef FogBrace Super;
	typedef FogOpenBrace This;
	TYPEDECL_SINGLE(This, Super)
	
public:
	FogOpenBrace(size_t endColumn = 0) : Super(endColumn) {}
	
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_stringized(std::ostream& s) const;
	virtual std::ostream& print_tokens(std::ostream& s, int aDepth) const;
	virtual size_t source_columns() const;
};

TYPEINFO_SINGLE(FogBrace, Super)
TYPEINFO_SINGLE(FogDiOpenBrace, Super)
TYPEINFO_SINGLE(FogOpenBrace, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBrace)
PRIMREFS_IMPL(FogBrace)
PRIMLISTOFREFS_IMPL(FogBrace)
FOGTOKEN_SHAREDLEAF_IMPL(FogBrace)
TMPL_HACK_FIX_DO(FogBrace)

size_t FogBrace::executable_tokens() const {
	return 0;
}

FogBrace& FogBrace::make_di_open_brace(size_t endColumn) {
	static FogBraceListOfRef tokenTable(32);
	
	if (tokenTable.tally() <= endColumn)
		tokenTable.set_tally(endColumn + 16);
		
	if (endColumn >= tokenTable.capacity())
		return FogBrace::mutable_null();
		
	if (!tokenTable[endColumn])
		tokenTable[endColumn].adopt(new FogDiOpenBrace(endColumn));
		
	if (tokenTable[endColumn])
		return *tokenTable[endColumn];
	else
		return FogBrace::mutable_null();
}

FogBrace& FogBrace::make_open_brace(size_t endColumn) {
	static FogBraceListOfRef tokenTable(32);
	
	if (tokenTable.tally() <= endColumn)
		tokenTable.set_tally(endColumn + 16);
		
	if (endColumn >= tokenTable.capacity())
		return FogBrace::mutable_null();
		
	if (!tokenTable[endColumn])
		tokenTable[endColumn].adopt(new FogOpenBrace(endColumn));
		
	if (tokenTable[endColumn])
		return *tokenTable[endColumn];
	else
		return FogBrace::mutable_null();
}

FogTokenType::TokenType FogBrace::pp_token_type_enum() const {
	return FogTokenType::character('{');
}

std::ostream& FogBrace::print_this(std::ostream& s) const {
	return s << _end_column;
}

FogTokenType::TokenType FogBrace::token_type_enum() const {
	return FogTokenType::character('{');
}

bool FogDiOpenBrace::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text("<%");
	return true;
}

std::ostream& FogDiOpenBrace::print_diagnostic(std::ostream& s, int aDepth) const {
	return s << "DiOpenBrace " << end_column();
}

char FogDiOpenBrace::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	return FogStream::space_and_emit(s, tailChar, "{");
}

std::ostream& FogDiOpenBrace::print_source(std::ostream& s, int aDepth) const {
	return s << "<%";
}

std::ostream& FogDiOpenBrace::print_stringized(std::ostream& s) const {
	return s << "<%";
}

std::ostream& FogDiOpenBrace::print_tokens(std::ostream& s, int aDepth) const {
	return s << "DiOpenBrace " << end_column();
}

size_t FogDiOpenBrace::source_columns() const {
	return 2;
}

bool FogOpenBrace::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text("{");
	return true;
}

std::ostream& FogOpenBrace::print_diagnostic(std::ostream& s, int aDepth) const {
	return s << "OpenBrace " << end_column();
}

char FogOpenBrace::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	return FogStream::space_and_emit(s, tailChar, "{");
}

std::ostream& FogOpenBrace::print_source(std::ostream& s, int aDepth) const {
	return s << "{";
}

std::ostream& FogOpenBrace::print_stringized(std::ostream& s) const {
	return s << "{";
}

std::ostream& FogOpenBrace::print_tokens(std::ostream& s, int aDepth) const {
	return s << "OpenBrace " << end_column();
}

size_t FogOpenBrace::source_columns() const {
	return 1;
}
