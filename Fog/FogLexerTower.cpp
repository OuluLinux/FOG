#include <Fog/FogIncludeAll.h>




TYPEINFO_SINGLE(FogLexerTower, Super)

FogLexerTower::FogLexerTower(FogReader& aReader)
	:
	Super(FogToken::immutable_null()),
	_reader(aReader)
{}

FogLexerTower::FogLexerTower(FogReader& aReader, const FogToken& aToken)
	:
	Super(aToken),
	_reader(aReader)
{}

FogLexerTower::~FogLexerTower() {}

void FogLexerTower::print_token(const char* aState, const FogToken* aValue) const {
	_reader.print_token(aState, aValue);
}
