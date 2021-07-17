#include <Fog/FogIncludeAll.h>



//  %import FogLine.hxx




TYPEINFO_SINGLE(FogStatement, Super)

FogStatement::FogStatement()
	:
	_is_meta(false)
{}

FogStatement::FogStatement(const This& aStatement)
	:
	Super(aStatement),
	//    _line(aStatement._line)
	_is_meta(aStatement._is_meta)
{}

FogStatement::~FogStatement() {}

bool FogStatement::compile_parsed(FogParseContext& parseContext) {
	ERRMSG("No meta-level support for a " << class_name());
	return false;
}

FogStatement* FogStatement::is_statement() { return this; }
//  FogLine& FogStatement::line() const { return *_line; }
const FogMetaType& FogStatement::meta_type() const { return FogMetaType::statement_type(); }

std::ostream& FogStatement::print_viz(std::ostream& s) const {
	s << "\"statement: ";
	print_this(s);
	return s << '\"';
}

bool FogStatement::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.reset();
	//    theSemantics.set(FogSemantics::IS_STATEMENT);
	theSemantics.set(FogSemantics::IS_DECLARATION);
	return true;
}

void FogStatement::set_is_meta() { _is_meta = true; }

//  void FogStatement::set_line(FogLine& aLine)
//  {
//    if (is_null())
//        ERRMSG("BUG -- should not set_line for  " << viz(*this));
//    else
//        _line = aLine;
//  }
