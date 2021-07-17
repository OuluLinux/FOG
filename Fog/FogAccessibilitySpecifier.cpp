#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogAccessibilitySpecifier, Super)
FOGTOKEN_LEAF_IMPL(FogAccessibilitySpecifier)

FogAccessibilitySpecifier::FogAccessibilitySpecifier(const FogAccess& anAccess)
	:
	_access(anAccess) {}

bool FogAccessibilitySpecifier::compile_parsed(FogParseContext& parseContext) {
	parseContext.set_access(*_access);
	return true;
}

char FogAccessibilitySpecifier::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, _access->str());
	tailChar = FogStream::space_and_emit(s, tailChar, ": ");
	return tailChar;
}

std::ostream& FogAccessibilitySpecifier::print_this(std::ostream& s) const {
	return s << _access->str();
}

std::ostream& FogAccessibilitySpecifier::print_viz(std::ostream& s) const {
	return s << "\"accessibility-declaration: " << _access->str() << '\"';
}

void FogAccessibilitySpecifier::set_line(FogLine& aLine) {}
