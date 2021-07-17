#include <Fog/FogIncludeAll.h>













struct FogLexerMacro_Element : public PrimPrint {
	typedef PrimPrint Super;
	typedef FogLexerMacro_Element This;
	TYPEDECL_SINGLE(This, Super)
	
public:
	PrimAdopted<FogLexerMacro_Element> _next;  //   Pushed macro invocation.
	const PrimIdHandle _id;       //   The invoked macro.
	FogTokenListOfRef _tokens;      //   The tokens to parse.
	size_t _index;         //   Index of next token to get.
	
private:
	FogLexerMacro_Element(const This&);
	This& operator=(const This&);
	
public:
	FogLexerMacro_Element(const PrimId& anId) : _id(anId), _index(0) {}
	
	virtual ~FogLexerMacro_Element() {}
	
	virtual std::ostream& print_this(std::ostream& s) const {
		return s << *_id;
	}
	
	void push(PrimAdopted<FogLexerMacro_Element>& topOfStack) {
		_next = topOfStack;
	}
	
	FogTokenListOfRef& tokens() {
		return _tokens;
	}
};

TYPEINFO_SINGLE(FogLexerMacro, Super)
TYPEINFO_SINGLE(FogLexerMacro_Element, Super)

FogLexerMacro::FogLexerMacro(std::istream& inputStream, const FogSourceFile& sourceFile, std::ostream* outputStream)
	:
	Super(inputStream, sourceFile, outputStream) {}

FogLexerMacro::~FogLexerMacro() {}

//
//  	Acquire the next token.
//
const char* FogLexerMacro::get(FogTokenContext& tokenContext) {
	while (true) {
		const char* stateName = 0;
		
		for (FogLexerMacro_Element * p; (p = _stack.get()) != 0; _stack = _stack->_next) {
			if (p->_index < p->_tokens.tally()) {
				tokenContext.assign(*p->_tokens[p->_index++]);
				stateName = "<MACRO_GET>";
				break;
			}
		}
		
		if (!stateName && _unget.is_full() && _unget.get(tokenContext))
			stateName = "<MACRO-REGET>";
			
		if (!stateName)
			stateName = Super::get(tokenContext);
			
		if (!stateName)
			return 0;
			
		if ((tokenContext.token().pp_token_type_enum() != FogTokenType::Identifier) || Fog::readonly())
			return stateName;
			
		const PrimId* anId = tokenContext.token().is_word();
		
		if (!anId) {
			ERRMSG("INVESTIGATE - unexpected 0 keyword in FogLexerMacro::get.");
			return stateName;
		}
		
		if (!hash_push(*anId))
			return stateName;
	}
}

bool FogLexerMacro::hash_push(const PrimId& anId) {
	const FogMacro* aMacro = is_defined(anId);
	
	if (!aMacro)
		return false;
		
	PrimAdopted<FogLexerMacro_Element> macroElement(new FogLexerMacro_Element(anId));
	
	if (!macroElement)
		return false;
		
	if (aMacro->is_function_like()) {
		FogMacroListOfRefToConst locatedArguments;
		{
			FogLocateParser locateParser(*this, locatedArguments, *aMacro);
			int parseStatus = locateParser.yyparse();
		}
		invoke_function(macroElement->tokens(), *aMacro, locatedArguments);
	}
	else
		invoke_object(macroElement->tokens(), *aMacro);
		
	macroElement->push(_stack);
	_stack = macroElement;
	return true;
}

void FogLexerMacro::invoke_function(FogTokenListOfRef& replacedTokens, const FogMacro& aMacro,
                                    const FogMacroListOfRefToConst& locatedArguments) {
	if (!aMacro.is_function_like()) {
		ERRMSG("BUG -- expected " << viz(aMacro) << " to be function-like.");
		return;
	}
	
	if (locatedArguments.tally() != aMacro.formal_tally()) {
		ERRMSG("Cannot invoke " << viz(aMacro) << " with " << locatedArguments.tally()
		       << " arguments when " << aMacro.formal_tally() << " required.");
		return;
	}
	
	FogReplaceParser replaceParser(*this, replacedTokens, aMacro.id());
	int replaceStatus = replaceParser.parse_function(aMacro, locatedArguments);
}

void FogLexerMacro::invoke_object(FogTokenListOfRef& replacedTokens, const FogMacro& aMacro) {
	if (aMacro.is_function_like())
		ERRMSG("BUG -- expected " << viz(aMacro) << " to be object-like.");
		
	FogReplaceParser replaceParser(*this, replacedTokens, aMacro.id());
	int replaceStatus = replaceParser.parse_object(aMacro.tokens());
}

std::ostream& FogLexerMacro::print_depth(std::ostream& s, int aDepth) const {
	s << indent(aDepth) << "FogLexerMacro(" << ")\n";
	
	for (const FogLexerMacro_Element* p = _stack.get(); p; p = p->_next.get())
		p->print_deep(s, aDepth + 1);
		
	return Super::print_depth(s, aDepth);
}

std::ostream& FogLexerMacro::print_members(std::ostream& s, int aDepth) const {
	s << indent(aDepth) << "FogLexerMacro(" << ")\n";
	
	for (const FogLexerMacro_Element* p = _stack.get(); p; p = p->_next.get())
		p->print_on(s, aDepth + 1);
		
	return Super::print_members(s, aDepth);
}

//
//  	Acquire the next token without substituting identifiers.
//
void FogLexerMacro::raw_get(FogTokenContext& tokenContext) {
	if (_unget.is_full())
		ERRMSG("BUG - did not expect to unget for FogLexerMacro::raw_get.");
		
	for (FogLexerMacro_Element * p; (p = _stack.get()) != 0; _stack = _stack->_next) {
		if (p->_index < p->_tokens.tally()) {
			tokenContext.assign(*(p->_tokens)[p->_index++]);
			return;
		}
	}
	
	Super::get(tokenContext);
}

//
//  	Signal that the most recent input token was not consumed by the parser and so should be
//  	returned by the next get() or peek().
//
void FogLexerMacro::raw_unget(const FogTokenContext& tokenContext) {
	if (Fog::debug_lex2main()) {
		PrimOstrstream s;
		s << "<MACRO-UNGET> ";
		tokenContext.value()->print_diagnostic(s, 0);
		DIAMSGZ(s.str());
	}
	
	FogLexerMacro_Element* p = _stack.get();
	
	if (!p)
		_unget.unget(tokenContext, false);
	else if (p->_index <= 0)
		ERRMSG("BUG -- cannot unget from empty macro stack " << *this);
	else if ((p->_tokens)[--p->_index].pointer() != &tokenContext.token())
		ERRMSG("BUG -- should not unget " << tokenContext.value()
		       << " when " << (p->_tokens)[p->_index] << " expected.");
}
