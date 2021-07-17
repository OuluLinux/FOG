#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogLexerCpp, Super)

FogLexerCpp::FogLexerCpp(FogReader& aReader, std::istream& inputStream, const FogSourceFile& sourceFile)
	:
	Super(inputStream, sourceFile, &std::cerr),
	_reader(aReader)
{}

FogLexerCpp::~FogLexerCpp() {}

void FogLexerCpp::define(const PrimId& macroId, const PrimIdList* formalList, FogTokens& someTokens) {
	_reader.define(macroId, formalList, someTokens);
}

//
//  	Acquire the next token, convertying preprocessor tokens to tokens.
//
const char* FogLexerCpp::get(FogTokenContext& tokenContext) {
	if (_unget.get(tokenContext))
		return "<CPP-REGET>";
		
	FogNullMonitor rawMonitor;
	
	if (Super::get(tokenContext))
		return "<CPP-GET>";
		
	return 0;
}

void FogLexerCpp::hash_include(const PrimId& fileStem) {
	const FogSourceFile* sourceFile = _reader.find_source_file(fileStem, FogSourceFileType::hash_input());
	
	if (!sourceFile)
		ERRMSG("Failed to to find #include file " << c_string(fileStem.str()) << " along any include path.");
		
	if (sourceFile)
		include_file(*sourceFile, FogUtility::frozen_utility());
}

//
//  	Configure the lexer to start processing the include file defined by fileStem, setting the declaration mode
//  	to aUtility.
//
void FogLexerCpp::include(const PrimId& fileStem, const FogUtility& aUtility) {
	const FogSourceFile* sourceFile = _reader.find_source_file(fileStem, FogSourceFileType::using_input());
	
	if (!sourceFile)
		ERRMSG("Failed to to find file " << c_string(fileStem.str()) << " along any include path.");
	else if (sourceFile->needs_reading())
		include_file(*sourceFile, aUtility);
	else
		CONDMSG(Fog::debug_lex(), "Suppressing reinclusion of " << c_string(sourceFile->full_file_name().str()));
}

//
//  	Configure the lexer to start processing sourceFile, setting the declaration mode to aUtility.
//
void FogLexerCpp::include_file(const FogSourceFile& sourceFile, const FogUtility& aUtility) {
	const PrimId& fileName = sourceFile.full_file_name();
	
	if (aUtility < _reader.utility_mode())
		ERRMSG("Should not include " << c_string(fileName.str()) << " as " << aUtility.str()
		       << " while at " << _reader.utility_mode().str());
		       
	sourceFile.set_read();
	Super::include_file(sourceFile, aUtility);
}

const FogMacro* FogLexerCpp::is_defined(const PrimId& anId) const { return _reader.is_defined(anId); }

const FogSourceFile* FogLexerCpp::make_source_file(const PrimId& fileName) {
	return _reader.make_source_file(fileName);
}

//
//  	Peek at the next character.
//
const char* FogLexerCpp::peek(FogTokenRef& tokenValue, bool skipWhite) {
	if (_unget.peek(tokenValue, skipWhite))
		return "<CPP-REPEEK>";
		
	const char* stateName = 0;
	FogTokenContext tokenContext;
	FogNullMonitor rawMonitor;
	
	do {
		stateName = Super::get(tokenContext);
		
		if (!stateName)
			return 0;
			
		tokenValue = tokenContext.token();
		
		if (Fog::debug_lex2main())
			_reader.print_token("<CPP-PEEKED>", &tokenContext.token());
			
		_unget.unget(tokenContext, false);
	} while (tokenContext.token().is_skip(skipWhite));
	
	return stateName;
}

void FogLexerCpp::popped_file(const FogUtility& utilityMode, const FogSourceFile* sourceFile) {
	_reader.set_utility_mode(utilityMode, sourceFile);
}

const FogUtility& FogLexerCpp::pushed_file(const FogUtility& utilityMode, const FogSourceFile& sourceFile) {
	_reader.set_max_utility_mode(utilityMode, &sourceFile);
	return _reader.utility_mode();
}

void FogLexerCpp::undefine(const PrimId& anId) { _reader.undefine(anId); }
