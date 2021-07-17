
#ifndef FOGLEXERSTATE_HXX
#define FOGLEXERSTATE_HXX



struct FogLexerState {
	const char* _name;
	/*   const */ int _state;     //   DevStudio thinks const means not-POD
	friend std::ostream& operator<<(std::ostream& s, const FogLexerState& aState);
};
#endif

