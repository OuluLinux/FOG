#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogLexerHash, Super)

//
//  	Acquire the next token for derived lexers. Context is necessarily not within a # directive.
//
const char* FogLexerHash::get(FogTokenContext& tokenContext) {
	const char* stateName = 0;
	
	while ((stateName = Super::get(tokenContext)) != 0) {
		if (!_truth.is_enabled()) {
			switch (tokenContext.token().pp_token_type_enum()) {
			default:
				break;
				
			case FogTokenType::HASH_BLANK:
			case FogTokenType::HASH_DEFINE:
			case FogTokenType::HASH_ELIF:
			case FogTokenType::HASH_ELSE:
			case FogTokenType::HASH_ENDIF:
			case FogTokenType::HASH_ERROR:
			case FogTokenType::HASH_IF:
			case FogTokenType::HASH_IFDEF:
			case FogTokenType::HASH_IFNDEF:
			case FogTokenType::HASH_INCLUDE:
			case FogTokenType::HASH_LINE:
			case FogTokenType::HASH_PRAGMA:
			case FogTokenType::HASH_UNDEF: {
				switch (tokenContext.token().pp_token_type_enum()) {
				case FogTokenType::HASH_ELIF:
					if (_truth.is_enabled_below()) {
						FogHashParser hashParser(*this, tokenContext);
						int parseStatus = hashParser.yyparse();
						
						if ((parseStatus == 0) && hashParser.truth())
							_truth.enable();
					}
					
					break;
					
				case FogTokenType::HASH_ELSE:
					if (_truth.is_enabled_below())
						_truth.enable();
						
					_truth.is_enabled();
					break;
					
				case FogTokenType::HASH_ENDIF:
					_truth.pop();
					break;
					
				case FogTokenType::HASH_IF:
				case FogTokenType::HASH_IFDEF:
				case FogTokenType::HASH_IFNDEF:
					_truth.push(false);
					break;
					
				default: break;
				}
				
				set_truth(_truth.is_enabled());
				break;
			}
			}
		}
		else {           /*   _is_enabled */
			switch (tokenContext.token().pp_token_type_enum()) {
			case FogTokenType::MacroId: {    //   Don't need or want to know difference any more.
				const PrimId* anId = tokenContext.token().is_word();
				
				if (anId)
					tokenContext.assign(FogTokenType::make_identifier(*anId));
			}
			
			/*   drop through */
			default:
				if (Fog::readonly())
					break;
					
				return stateName;
				
			case FogTokenType::HASH_BLANK:
			case FogTokenType::HASH_DEFINE:
			case FogTokenType::HASH_ELIF:
			case FogTokenType::HASH_ELSE:
			case FogTokenType::HASH_ENDIF:
			case FogTokenType::HASH_ERROR:
			case FogTokenType::HASH_IF:
			case FogTokenType::HASH_IFDEF:
			case FogTokenType::HASH_IFNDEF:
			case FogTokenType::HASH_INCLUDE:
			case FogTokenType::HASH_LINE:
			case FogTokenType::HASH_PRAGMA:
			case FogTokenType::HASH_UNDEF: {
				{
					FogHashParser hashParser(*this, tokenContext);
					int parseStatus = hashParser.yyparse();
					
					if ((parseStatus == 0) && !Fog::readonly()) {
						switch (tokenContext.token().pp_token_type_enum()) {
						case FogTokenType::HASH_DEFINE:
							define(hashParser.id(), hashParser.formals(), hashParser.tokens());
							break;
							
						case FogTokenType::HASH_ELIF:
						case FogTokenType::HASH_ELSE:
							_truth.disable();
							break;
							
						case FogTokenType::HASH_ENDIF:
							_truth.pop();
							break;
							
						case FogTokenType::HASH_ERROR:
							ERRMSGZ(hashParser.stream() ? hashParser.stream()->str() : 0);
							break;
							
						case FogTokenType::HASH_IF:
						case FogTokenType::HASH_IFDEF:
						case FogTokenType::HASH_IFNDEF:
							_truth.push(hashParser.truth());
							break;
							
						case FogTokenType::HASH_INCLUDE:
							hash_include(hashParser.string_id());
							break;
							
						case FogTokenType::HASH_LINE: {
							const PrimId& fileStem = hashParser.string_id();
							set_line(fileStem ? make_source_file(fileStem) : 0, hashParser.line_number());
							break;
						}
						
						case FogTokenType::HASH_UNDEF:
							undefine(hashParser.id());
							break;
							
						default:
							break;
						}
					}
				}
				set_truth(_truth.is_enabled());
				break;
			}
			}
		}
	}
	
	return 0;
}

std::ostream& FogLexerHash::print_depth(std::ostream& s, int aDepth) const {
	s << indent(aDepth) << "FogLexerHash(" << _truth.is_enabled() << ")\n";
	return Super::print_depth(s, aDepth);
}

std::ostream& FogLexerHash::print_members(std::ostream& s, int aDepth) const {
	s << indent(aDepth) << "FogLexerHash(" << _truth.is_enabled() << ")\n";
	return Super::print_members(s, aDepth);
}

bool FogLexerTruth::pop() {
	if (!_next)
		ERRMSG("#endif has no enclosing #if.");
	else {
		_is_enabled = _next->is_enabled();
		_next = _next->_next;
	}
	
	return _is_enabled;
}

bool FogLexerTruth::push(bool aTruth) {
	_next.reset(new FogLexerTruth(_next, _is_enabled));
	_is_enabled = aTruth;
	return _is_enabled;
}
