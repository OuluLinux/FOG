#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogLine, Super)
FOGTOKEN_SHAREDLEAF_IMPL(FogLine)

PRIMREF_NULL_CLAS(FogLine)
};
PRIMREF_DERIVED_NULL_IMPL(FogLine)
PRIMREFS_IMPL(FogLine)
TMPL_HACK_FIX_DO(FogLine)

FogLine::FogLine(const FogSourceFile& sourceFile, size_t lineNumber, const PrimString& sourceText)
	:
	_file(sourceFile),
	_number(lineNumber),
	_text(sourceText)
{}

FogLine::FogLine()
	:
	_file(FogSourceFile::immutable_null()),
	_number(0)
{}

bool FogLine::emit(FogEmitContext& emitContext) const {
	emitContext.emit_hash_line(*this);
	return true;
}

size_t FogLine::executable_tokens() const { return 0; }
FogLine* FogLine::is_line() const { return &mutate(); }
const FogToken* FogLine::is_white() const { return this; }
FogLine& FogLine::line() const { return mutate(); }

FogLine* FogLine::new_line(const FogSourceFile* sourceFile, size_t lineNumber, const PrimString& sourceText) {
	return new FogLine(sourceFile ? *sourceFile : FogSourceFile::immutable_null(), lineNumber, sourceText);
}

FogTokenType::TokenType FogLine::pp_token_type_enum() const { return FogTokenType::NewLine; }

std::ostream& FogLine::print_source(std::ostream& s, int aDepth) const {
	s << '\n' << indent(aDepth);
	return s;
}

std::ostream& FogLine::print_this(std::ostream& s) const {
	s << _file.full_file_name().str() << "." << _number;
	return s;
}

std::ostream& FogLine::print_viz(std::ostream& s) const {
	//  	return s << "\"file: " << c_string(_pathed_id.str()) << '"';
	return s << '\"' << _file.full_file_name().str() << "\"." << _number;
}

size_t FogLine::source_columns() const { return 0; }
size_t FogLine::source_lines() const { return 1; }

FogTokenType::TokenType FogLine::token_type_enum() const { return FogTokenType::NewLine; }

