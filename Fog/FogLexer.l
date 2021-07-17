//
//	  Title:			Flex++ lexer for Flexible Object Generator.
//
//	  File Name:		FogLexer.l
//
//	  Author:			E.D.Willink
//
//	Externally the full 8-bit character set is used with \n line separation.
//	All incoming \r characters are suppressed.
//
//	Error handling is resolved through two different regimes. For low level lexing such as resolution
//	of # expressions, error messages are not context dependent and so can and should be emitted promptly.
//	Higher level error messages are context dependent and should not be emitted if the parsing path provoking
//	the error proves to have been misguided. Low level errors are handled promptly by installing the
//	FogLexerContext as the notice board to handle messages. High level messages are caught by the
//	FogParserBase which is installed as the notice board and deferred on the FogLexerInput or FogLexerRecorder
//	mark context stacks.
//END
//
% {

#ifdef ECHO
#undef ECHO							// Before PrimManip gets confused.
#endif

#include <Fog/FogIncludeAll.h>

#define INITIAL 0

#define ECHO { ERRMSG("BUG - Unexpected character " << c_string(yytext[0]) << " in " << state_name()); }
	%
}



% name FogLexer
//
//	Parsing within /* ... */
//
% x COMMENT_STATE
//
//	INITIAL			Waiting for first line.
//	HASH_STATE		Parsing between the # and subsequent directive
//	INCLUDE_STATE	Parsing after the # in a #include line (detect <...> as a string)
//	OFF_STATE		Passing a multiplicity of pp-tokens after a disabling #if
//	SOAK_STATE		Passing a multiplicity of pp-tokens for #error or #pragma
//
% s INCLUDE_STATE SOAK_STATE
% x HASH_STATE OFF_STATE

% define INHERIT : public FogLexerContext
% define CONSTRUCTOR_PARAM \
std::istream& inputStream, \
const FogSourceFile& sourceFile, \
std::ostream* outputStream
% define CONSTRUCTOR_INIT : \ FogLexerContext(sourceFile), \
_state(&initial_state), \
_pre_comment_state(&initial_state), \
_pre_off_state(&initial_state)

% define CONSTRUCTOR_CODE yyin = &inputStream; yyout = outputStream;
% define ECHO_NOCODE
% define INPUT_NOCODE
% define WRAP_NOCODE
% define LEX_PARAM FogTokenRef& yyValue
% define LEX_RETURN bool
% define IOSTREAM

% define MEMBERS \
private: \
typedef FogLexerContext Super; \
TYPEDECL_SINGLE(FogLexer, Super) \
const FogLexerState* _state; \
const FogLexerState* _pre_comment_state; \
const FogLexerState* _pre_off_state; \
private: \
static const FogLexerState comment_state; \
static const FogLexerState hash_state; \
static const FogLexerState include_state; \
static const FogLexerState initial_state; \
static const FogLexerState off_state; \
static const FogLexerState soak_state; \
public: \
static size_t _tokens; \
static size_t _body_tokens; \
private: \
size_t columns(const char* aString, size_t aSize); \
void got_character(FogTokenRef& yyValue, const char* someText, size_t aLength); \
void got_string(FogTokenRef& yyValue, const char* aString, size_t aSize, bool isPrefixed); \
const char* state_name() const; \
const char* text() const { return (const char*)yytext; } \
protected: \
const FogLexerState& begin(const FogLexerState& aState); \
const FogLexerState& begin_include() { return begin(include_state); } \
const FogLexerState& begin_soak() { return begin(soak_state); } \
const char* get(FogTokenContext& tokenContext); \
void include_file(const FogSourceFile& sourceFile, const FogUtility& aUtility); \
void set_truth(bool isEnabled); \
public: \
virtual std::ostream& print_depth(std::ostream& s, int aDepth) const; \
virtual std::ostream& print_members(std::ostream& s, int aDepth) const; \
virtual std::ostream& print_this(std::ostream& s) const; \
const FogLexerState& state() const { return *_state; } \
public: \
static std::ostream& print_statistics(std::ostream& s);

close_brace1					("}")
close_brace2					("%>")
close_brace						({close_brace1} | {close_brace2})
close_square1					("]")
close_square2					(":>")
close_square					({close_square1} | {close_square2})
hash1							("#")
hash2							("%:")
hash							({hash1} | {hash2})
hash_hash1						("##")
hash_hash2						("%:%:")
hash_hash						({hash_hash1} | {hash_hash2})
open_brace1						("{")
open_brace2						("<%")
open_brace						({open_brace1} | {open_brace2})
open_square1					("[")
open_square2					("<:")
open_square						({open_square1} | {open_square2})
/* pp_ws is implementation defined as to whether to convert to a space. Here we convert */
pp_ws							[ \f\v\t]
ws								[ \f\v\t]
sq_sq							({open_square} {open_square})

digit							[0 - 9]
hex								[0 - 9A - Fa - f]
letter							[A - Z_a - z] simple_escape_sequence			(\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v)
        octal_escape_sequence			(\\[0 - 7] | \\[0 - 7][0 - 7] | \\[0 - 7][0 - 7][0 - 7])
        hexadecimal_escape_sequence		(\\x{hex} +)
        escape_sequence					({simple_escape_sequence} | {octal_escape_sequence} | {hexadecimal_escape_sequence})
universal_character_name		(\\u{hex} {hex} {hex} {hex} | \\U{hex} {hex} {hex} {hex} {hex} {hex} {hex} {hex})
non_digit						({letter} | {universal_character_name})
identifier						({non_digit}({non_digit} | {digit})*)

/* An empty '' is valid FOG but not C++. Escapes are simplified to \. since any residue is easy */
character_lit					(L ? \'([^\'\\\n]|\\.)*)
                                 character_literal				({character_lit}\')
                                         
                                         string_lit						(L ? \"([^\"\\\n]|\\.)*)
                                                 string_literal					({string_lit}\")
                                                         
                                                         pp_number						(\. ? {digit}({digit} | {non_digit} | [eE][- +] | \.)*)
                                                         
                                                         pp_punct0						({open_brace} | {close_brace} | {open_square} | {close_square} | {hash} | {hash_hash})
                                                         pp_punct1						("(" | ")" | ";" | ":" | "..." | "?" | "::" | "." | ".*")
                                                         pp_punct2						("+" | "-" | "*" | "/" | "%" | "^" | "&" | "|" | "~" | "!" | "=" | "<" | ">")
                                                         pp_punct3						("+=" | "-=" | "*=" | "/=" | "%=" | "^=" | "&=" | "|=" | "<<" | ">>" | ">>=" | "<<=")
                                                         pp_punct4						("==" | "!=" | "<=" | ">=" | "&&" | "||" | "++" | "--" | "," | "->*" | "->")
                                                         pp_other						([^\ \n\t\v\f] | \\. | {pp_punct0} | {pp_punct1} | {pp_punct2} | {pp_punct3} | {pp_punct4})
                                                         pp_token						({identifier} | {pp_number} | {pp_other} | {string_literal} | {character_literal})
                                                         
                                                         % %
                                                         /****************************************************************************************************
                                                          			Comments
                                                          ****************************************************************************************************/
                                                         <COMMENT_STATE>([^ / \n] | [^*\n]"/")*"*/"		{ begin(*_pre_comment_state);
                                                                 yyValue = FogTokenType::get_character(' '); return true;
                                                                                                     }
                                                         <COMMENT_STATE>([^ / \n] | [^*\n]"/") +			|
                                                         <COMMENT_STATE>.							{ advance_token(yyleng); }
                                                         <OFF_STATE>\n	                            |
                                                         <COMMENT_STATE>\n							{ advance_line(); }
                                                         
                                                         <HASH_STATE> {ws}*"//".*						|
<OFF_STATE> {ws}*"//".*						|
{ws}*"//".* 								{ advance_token(yyleng); }

<HASH_STATE> {ws}*"/*"						|
<OFF_STATE> {ws}*"/*"						|
{ws}*"/*"	 								{ advance_token(yyleng); _pre_comment_state = &begin(comment_state); }

/****************************************************************************************************
 			#, #error and #pragma pass all. ({pp_token}|{pp_ws})* unfortunately is more than flex++ can take.
 ****************************************************************************************************/
<SOAK_STATE> {pp_token}				{ yyValue = FogTokenType::make_literal_string(text(), yyleng); return true; }

/****************************************************************************************************
 			#include requires distinct recognition of ".." and <...>
 ****************************************************************************************************/
<INCLUDE_STATE>\"[^\"\n]*\"			|
<INCLUDE_STATE>"<"[^\ > \n]*">"		{ got_string(yyValue, text() + 1, yyleng - 2, false); return true; }

/****************************************************************************************************
 			# Directives			.bugbug comment preceeding # is not ignored.
 ****************************************************************************************************/
<OFF_STATE>^ {ws}* {hash}		|
^ {ws}* {hash}				{ CONDMSG(Fog::debug_lex(), "<LEXER-" << _state->_name << "> \""  << text() << '"');
	advance_token(yyleng); begin(hash_state);
}
<OFF_STATE> {ws} +	        |
<OFF_STATE> {pp_token}	    |
<OFF_STATE> {ws} +	        { CONDMSG(Fog::debug_lex(), "<LEXER-" << _state->_name << "> \""  << text() << '"');
                              advance_token(yyleng);
                           }
<HASH_STATE>"define"		{ yyValue = FogTokenType::get(FogTokenType::HASH_DEFINE); begin(initial_state); return true; }
<HASH_STATE>"elif"			{ yyValue = FogTokenType::get(FogTokenType::HASH_ELIF); begin(initial_state); return true; }
<HASH_STATE>"else"			{ yyValue = FogTokenType::get(FogTokenType::HASH_ELSE); begin(initial_state); return true; }
<HASH_STATE>"endif"			{ yyValue = FogTokenType::get(FogTokenType::HASH_ENDIF); begin(initial_state); return true; }
<HASH_STATE>"error"			{ yyValue = FogTokenType::get(FogTokenType::HASH_ERROR); begin(initial_state); return true; }
<HASH_STATE>"if"			{ yyValue = FogTokenType::get(FogTokenType::HASH_IF); begin(initial_state); return true; }
<HASH_STATE>"ifdef"			{ yyValue = FogTokenType::get(FogTokenType::HASH_IFDEF); begin(initial_state); return true; }
<HASH_STATE>"ifndef"		{ yyValue = FogTokenType::get(FogTokenType::HASH_IFNDEF); begin(initial_state); return true; }
<HASH_STATE>"include"		{ yyValue = FogTokenType::get(FogTokenType::HASH_INCLUDE); begin(initial_state); return true; }
<HASH_STATE>"line"			{ yyValue = FogTokenType::get(FogTokenType::HASH_LINE); begin(initial_state); return true; }
<HASH_STATE>"pragma"		{ yyValue = FogTokenType::get(FogTokenType::HASH_PRAGMA); begin(initial_state); return true; }
<HASH_STATE>"undef"			{ yyValue = FogTokenType::get(FogTokenType::HASH_UNDEF); begin(initial_state); return true; }
<HASH_STATE>.				{ yyless(0);
                              yyValue = FogTokenType::get(FogTokenType::HASH_BLANK); begin(initial_state); return true; }
<HASH_STATE>\n				{ yyValue = FogTokenType::get(FogTokenType::HASH_BLANK); begin(initial_state); return true; }

/****************************************************************************************************
 			Character literals
 ****************************************************************************************************/
{character_lit}\'					{ got_character(yyValue, text(), yyleng-1); return true; }
{character_lit}\\					{
	ERRMSG("End of line assumed to terminate character with trailing escape.");
	got_character(yyValue, text(), yyleng - 1); return true;
}
{character_lit}						{
	ERRMSG("End of line assumed to terminate character.");
	got_character(yyValue, text(), yyleng); return true;
}

/****************************************************************************************************
 			String literals
 ****************************************************************************************************/
{string_lit}\"						{ got_string(yyValue, text(), yyleng-1, true); return true; }
{string_lit}\\						{
	ERRMSG("End of line assumed to terminate string with trailing escape.");
	got_string(yyValue, text(), yyleng - 1, true); return true;
}
{string_lit}						{
	ERRMSG("End of line assumed to terminate string.");
	got_string(yyValue, text(), yyleng, true); return true;
}

/****************************************************************************************************
 			Number literals
 			'0' is special cased in FogNumber::token_type_enum
 ****************************************************************************************************/
{pp_number}							{ yyValue = FogTokenType::make_number(text(), yyleng); return true; }

/****************************************************************************************************
 			Identifiers
 				DEFINED is detected where allowable by FogHashParser
 				Id (and MacroId) are partitioned into MetaType, BuiltIn, keywords in FogLexerGarbage
 ****************************************************************************************************/
{identifier}					{ yyValue.assign(FogTokenType::make_identifier(text(), yyleng)); return true; }

/****************************************************************************************************
 			Punctuation
 ****************************************************************************************************/
{hash1}								{ yyValue = FogTokenType::get_character('#'); return true; }
{hash2}								{ yyValue = FogTokenType::get(FogTokenType::DI_HASH); return true; }
{hash_hash1}						{ yyValue = FogTokenType::get(FogTokenType::HASH_HASH); return true; }
{hash_hash2}						{ yyValue = FogTokenType::get(FogTokenType::DI_HASH_HASH); return true; }
{open_brace1}						{ yyValue = FogTokenType::make_open_brace(detabbed_col() + 1); return true; }
{open_brace2}						{ yyValue = FogTokenType::make_di_open_brace(detabbed_col() + 2); return true; }
{open_square1}						{ yyValue = FogTokenType::get_character('['); return true; }
{open_square2}						{ yyValue = FogTokenType::get(FogTokenType::DI_SQUARE); return true; }
{close_brace1}						{ yyValue = FogTokenType::get_character('}'); return true; }
{close_brace2}						{ yyValue = FogTokenType::get(FogTokenType::DI_ECARB); return true; }
{close_square1}						{ yyValue = FogTokenType::get_character(']'); return true; }
{close_square2}						{ yyValue = FogTokenType::get(FogTokenType::DI_ERAUQS); return true; }
"::"								{ yyValue = FogTokenType::get(FogTokenType::SCOPE); return true; }
"..."								{ yyValue = FogTokenType::get(FogTokenType::ELLIPSIS); return true; }
"<<"								{ yyValue = FogTokenType::get(FogTokenType::SHL); return true; }
">>"								{ yyValue = FogTokenType::get(FogTokenType::SHR); return true; }
"=="								{ yyValue = FogTokenType::get(FogTokenType::EQ); return true; }
"!="								{ yyValue = FogTokenType::get(FogTokenType::NE); return true; }
"<="								{ yyValue = FogTokenType::get(FogTokenType::LE); return true; }
">="								{ yyValue = FogTokenType::get(FogTokenType::GE); return true; }
"&&"								{ yyValue = FogTokenType::get(FogTokenType::LOG_AND); return true; }
"||"								{ yyValue = FogTokenType::get(FogTokenType::LOG_OR); return true; }
"++"								{ yyValue = FogTokenType::get(FogTokenType::INC); return true; }
"--"								{ yyValue = FogTokenType::get(FogTokenType::DEC); return true; }
"->*"								{ yyValue = FogTokenType::get(FogTokenType::ARROW_STAR); return true; }
"->"								{ yyValue = FogTokenType::get(FogTokenType::ARROW); return true; }
".*"								{ yyValue = FogTokenType::get(FogTokenType::DOT_STAR); return true; }
"+="								{ yyValue = FogTokenType::get(FogTokenType::ASS_ADD); return true; }
"-="								{ yyValue = FogTokenType::get(FogTokenType::ASS_SUB); return true; }
"*="								{ yyValue = FogTokenType::get(FogTokenType::ASS_MUL); return true; }
"/="								{ yyValue = FogTokenType::get(FogTokenType::ASS_DIV); return true; }
"%="								{ yyValue = FogTokenType::get(FogTokenType::ASS_MOD); return true; }
"^="								{ yyValue = FogTokenType::get(FogTokenType::ASS_XOR); return true; }
"&="								{ yyValue = FogTokenType::get(FogTokenType::ASS_AND); return true; }
"|="								{ yyValue = FogTokenType::get(FogTokenType::ASS_OR); return true; }
">>="								{ yyValue = FogTokenType::get(FogTokenType::ASS_SHR); return true; }
"<<="								{ yyValue = FogTokenType::get(FogTokenType::ASS_SHL); return true; }

{escape_sequence}					|
{universal_character_name}			{
	yyValue = FogTokenType::make_literal_character(text(), yyleng);
	return true;
}

/****************************************************************************************************
 			Whitespace, Single Characters and New Lines
 ****************************************************************************************************/
{ws} +								{
	yyValue = FogTokenType::make_spacing(columns(yytext, yyleng));
	return true;
}
.									{ yyValue = FogTokenType::get_character(yytext[0]);
                                      return true; }
\n									{ yyValue = current_line(); return true; }

/****************************************************************************************************
 			Backstops
 ****************************************************************************************************/
<COMMENT_STATE> {pp_token}			|
{pp_token}							{
	ERRMSG("BUG - Unexpected token " << c_string(text(), yyleng) << " in "
	       << state()); yyValue = FogTokenType::nil(); return true;
}

<COMMENT_STATE>.					|
.									{ ERRMSG("BUG - Unexpected character " << c_string(yytext[0]) << " in "
                                      << state()); yyValue = FogTokenType::nil(); return true; }

<COMMENT_STATE>\n					|
\n									{ ERRMSG("BUG - Unexpected end of line in " << state_name());
                                      advance_line(); yyValue = FogTokenType::nil(); return true; }

% %
TYPEINFO_SINGLE(FogLexer, Super)

const FogLexerState FogLexer::comment_state = { "<COMMENT>", COMMENT_STATE };
        const FogLexerState FogLexer::hash_state = { "<HASH>", HASH_STATE };
        const FogLexerState FogLexer::include_state = { "<INCLUDE>", INCLUDE_STATE };
        const FogLexerState FogLexer::initial_state = { "<INITIAL>", INITIAL };
        const FogLexerState FogLexer::off_state = { "<OFF>", OFF_STATE };
        const FogLexerState FogLexer::soak_state = { "<SOAK>", SOAK_STATE };
        
        size_t FogLexer::_tokens = 0;
        size_t FogLexer::_body_tokens = 0;
        
#include <Fog/FogIncludeAll.h>
        
        //
        //	Change lexing state to aState, invoking BEGIN appropriately. Returns the previous state.
        //
const FogLexerState & FogLexer::begin(const FogLexerState& aState) {
	const FogLexerState& oldState = state();
	_state = &aState;
	BEGIN(state()._state);
	return oldState;
}

//
//	Return the number of source character columns contributed by aString[aSize].
//
size_t FogLexer::columns(const char* aString, size_t aSize) {
	const char* p = aString;
	size_t tabSize = Fog::get_tab_size();
	size_t detabbedCol = detabbed_col();
	
	for (size_t i = aSize; i > 0; i--, p++) {
		if (*p == '\t') {
			size_t numTabs = detabbedCol / tabSize;
			detabbedCol = (numTabs + 1) * tabSize;
		}
		else
			detabbedCol++;
	}
	
	long aValue = detabbedCol - detabbed_col();
	return aValue;
}

//
//	Return the next token from the lexer.
//
const char* FogLexer::get(FogTokenContext& tokenContext) {
	advance_line();
	//	if ((yyleng != 1) || (yytext[0] != '\n'))
	//		advance(yyleng, columns(yytext, yyleng));
	const FogLexerState* preState = _state;
	
	if (!yylex(tokenContext.value())) {
		tokenContext.reset();
		return 0;
	}
	
	tokenContext.set_line(current_line(), detabbed_col());
	
	if (!tokenContext.token().is_white())
		_tokens++;
		
	//	advance_line();
	if ((yyleng != 1) || (yytext[0] != '\n'))
		advance_token(columns(yytext, yyleng));
	else
		advance_token(0);
		
	CONDMSG(Fog::debug_lex(), "<LEXER-" << preState->_name << "> " << _state->_name << " : " << tokenContext);
	return "<LEXER-GET>";
}

//
//	Configure the lexer to reflect successful parsing of a character value, assigning it to yyValue.
//
//	The source someText[aLength] should correspond to the parsed text including any L or ' prefix
//	but excluding any ' suffix. In this way the return can indicate whether a wide character has
//	been detected and the routine can accommodate a variety of erroneous terminations.
//
void FogLexer::got_character(FogTokenRef& yyValue, const char* someText, size_t aLength) {
	bool isWide = false;
	
	if (someText && aLength) {
		if (*someText == 'L') {
			isWide = true;
			someText++;
			aLength--;
		}
		
		if (!aLength || (*someText != '\''))
			ERRMSG("BUG - bad start of character literal.");
			
		if (aLength) {
			someText++;
			aLength--;
		}
	}
	
	if (isWide)
		yyValue = FogTokenType::make_wide_character(someText, aLength);
	else
		yyValue = FogTokenType::make_narrow_character(someText, aLength);
}

//
//	Configure the lexer to reflect successful parsing of a categorised string.
//
//	If !isPrefixed the required string is precisely semeText[aLenth].
//
//	If isPrefixed the source someText[aLength] should correspond to the parsed text including any
//	L or " prefix but excluding any " suffix. In this way the return can indicate whether a wide
//	character has been detected and the routine can accommodate a variety of erroneous terminations.
//
void FogLexer::got_string(FogTokenRef& yyValue, const char* someText, size_t aLength, bool isPrefixed) {
	bool isWide = false;
	
	if (isPrefixed && someText && aLength) {
		if (*someText == 'L') {
			isWide = true;
			someText++;
			aLength--;
		}
		
		if (!aLength || (*someText != '"'))
			ERRMSG("BUG - bad start of string literal.");
			
		if (aLength) {
			someText++;
			aLength--;
		}
	}
	
	if (isWide)
		yyValue = FogTokenType::make_wide_string(someText, aLength);
	else
		yyValue = FogTokenType::make_narrow_string(someText, aLength);
}

//
//	Configure the lexer to start processing sourceFile, setting the declaration mode to aUtility.
//
void FogLexer::include_file(const FogSourceFile& sourceFile, const FogUtility& aUtility) {
	const PrimId& fileName = sourceFile.full_file_name();
	CONDMSG(Fog::debug_lex() || Fog::debug_input(), state() << " Including " << c_string(fileName.str()));
	PrimAdopted<std::istream> aStream(new std::ifstream(fileName.str()));
	PrimError anError;
	
	if (!aStream)
		ERRMSG("Failed to allocate memory to read " << c_string(fileName.str()));
	else if (!*aStream)
		ERRMSG("Failed to to open " << c_string(fileName.str()) << ", " << anError);
	else {
		std::istream* newStream = push_buffer(fileName, aStream, YY_CURRENT_BUFFER, aUtility, sourceFile);
		yy_switch_to_buffer(yy_create_buffer(newStream, YY_BUF_SIZE));
		begin(initial_state);
	}
}

std::ostream & FogLexer::print_depth(std::ostream& s, int aDepth) const {
	s << indent(aDepth) << "FogLexer(" << state() << ")\n";
	return Super::print_depth(s, aDepth);
}

std::ostream & FogLexer::print_members(std::ostream& s, int aDepth) const {
	s << indent(aDepth) << "FogLexer(" << state() << ")\n";
	return Super::print_members(s, aDepth);
}

std::ostream & FogLexer::print_statistics(std::ostream& s) {
	s << "tokens = " << _tokens << '\n';
	s << "body-tokens = " << _body_tokens << '\n';
	return s;
}

std::ostream & FogLexer::print_this(std::ostream& s) const {
	s << state();
	s << ", ";
	return Super::print_this(s);
}

void FogLexer::set_truth(bool isEnabled) {
	if (isEnabled) {
		if (_state == &off_state)
			begin(*_pre_off_state);
	}
	else {
		if (_state != &off_state)
			_pre_off_state = &begin(off_state);
	}
}

//
//	Return the state name.
//
//	This method is supplied out-of-line to bypass a declaration ordering problem.
//
const char* FogLexer::state_name() const { return state()._name; }

//
//	Generate a warning diagnostic using msg to explain a problem with respect to the source context.
//
//void FogLexer::warning(const char *msg) { WRNMSGZ(msg); }

//
//	Catch the lexer default processing of an unintelligible input character.
//
void FogLexer::yy_echo() {
	ERRMSG("BUG - should not resort to invocation of yyecho() for " << c_string(yytext, yyleng));
}

//
//	Acquire some more input to buffer[max_size] returning the number of characters to result.
//	Returns result, which is -1 at end of input.
//
int FogLexer::yy_input(char* buffer, int& result, int max_size) {
	result = yyin ? get_line(*yyin, buffer, max_size) : 0;
	return result;
}

//
//	Restore the previous source file reading context from the stack.
//
int FogLexer::yy_wrap() {
	CONDMSG(Fog::debug_lex() || Fog::debug_input(),
	        state() << " End of " << c_string(current_line().file().full_file_name().str()));
	struct yy_buffer_state* poppedBuffer = pop_buffer();
	
	if (!poppedBuffer)
		return 1;
		
	struct yy_buffer_state* deadBuffer = YY_CURRENT_BUFFER;
	yy_switch_to_buffer(poppedBuffer);
	yy_delete_buffer(deadBuffer);
	//
	//	There is no need to restore any state, since input is a continuous flow process and so whatever
	//	state is in use at the end of one buffer is correct for the subsequent continuations. The only
	//	state change for buffer changes is enforcement of initial_state at the start of an include.
	//
	return 0;
}
