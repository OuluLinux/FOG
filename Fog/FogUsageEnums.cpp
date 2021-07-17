#include <Fog/FogIncludeAll.h>






static const char* const FogUsageType_texts[FogUsageEnums::INDEXES + 1] = {
	"interface-finish",
	"name",
	"friend",
	"public typedef",
	"protected typedef",
	"private typedef",
	"public enum",
	"protected enum",
	"private enum",
	"public variable",
	"protected variable",
	"private variable",
	"public function",
	"protected function",
	"private function",
	"static public variable",
	"static protected variable",
	"static private variable",
	"static public function",
	"static protected function",
	"static private function",
	"interface-start",
	"file",
	"inline",
	"implementation",
	"invalid"
};

const PrimEnum& FogUsageEnums::values() {
	static const PrimEnum theValues("FogUsageType", FogUsageType_texts, PRIM_ARRAY_SIZE(FogUsageType_texts));
	return theValues;
}

std::ostream& operator<<(std::ostream& s, const FogUsageEnums::TypeEnum& aUsage) {
	return s << FogUsageEnums::values()[aUsage];
}

std::istream& operator>>(std::istream& s, FogUsageEnums::TypeEnum& aUsage) {
	aUsage = FogUsageEnums::TypeEnum(FogUsageEnums::values()[s]);
	return s;
}
