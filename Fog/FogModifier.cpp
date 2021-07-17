#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogModifier, Super)
PRIMREF_NULL_CLAS(FogModifier)
FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogModifier)
PRIMREFS_IMPL(FogModifier)
TMPL_HACK_FIX_DO(FogModifier)

//
//    Compilation of a decl returns true if ok. A derived FunctionDecl compiles its parameters too.
//
bool FogModifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext) { return true; }

//
//    Contribute to the deduction of the equivalence tree for a template parameter by updating aValue to reflect
//    the compatible application of this formal modifier and actualModifier to aValue..
//
void FogModifier::deduce_template_parameter(FogExprRef& deducedExpr) const {
	ERRMSG("BUG -- should not invoke FogModifier::deduce_template_parameter for " << viz(*this));
}
void FogModifier::deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const {
	ERRMSG("BUG -- should not invoke FogModifier::deduce_template_parameter for " << viz(*this));
}

bool FogModifier::emit(FogEmitContext& emitContext) const {
	bool doneSomething = emit_prefix(emitContext, false);
	doneSomething |= emit_suffix(emitContext, false);
	return doneSomething;
}

//
//    Emit text that prefixes the rest of a declarator, returning true if anything emitted.
//
bool FogModifier::emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const { return false; }

//
//    Emit text that suffixes the rest of a declarator, returning true if anything emitted.
//
bool FogModifier::emit_suffix(FogEmitContext& emitContext, bool needsBrackets) const { return false; }

//
//    Perform the template-specific comnparison of thsi formal modifier with actualModifier...
//
bool FogModifier::is_formal_template_parameter_for(const FogModifier& actualModifier) const {
	ERRMSG("BUG -- should not invoke FogModifier::is_formal_template_parameter_for for " << viz(*this));
	return false;
}

//
//    Return true if this declarator contains an outer indirection such as T &p, T N::*p, T *a[],
//    T *f() or T *X::a but not T (*a)[] or T (*f)().
//
bool FogModifier::is_indirect() const { return false; }

FogModifier* FogModifier::is_modifier() { return this; }

//
//    Return true if this declarator contains just a & and so is suitable for arithmetic interpretation.
//
bool FogModifier::is_reference() const { return false; }

//
//    Return true if this declarator contains just a * and so is suitable for arithmetic interpretation.
//
bool FogModifier::is_star() const { return false; }

//  void FogModifier::merge_from(FogMergeContext& mergeContext, const This& aToken)
//  {
//    Super::merge_from(mergeContext, aToken);
//    _cv = _cv->merge_cv(aToken.cv());
//  }

//  const FogMerge& FogModifier::needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const
//  {
//    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, aToken));
//    needsMerge |= cv().needs_merge_cv(aToken.cv());
//    return needsMerge;
//  }

//
//    Generate a formatted print-out of this declarator, returning an updated version of tailChar that
//    defines the previous character for pretty printing. If scopeId is non-0 it prefixes the declarator.
//
char FogModifier::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = print_prefix(s, tailChar, false);
	
	if (scopeId)
		tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
		
	//    if (!is_null())
	//        tailChar = FogStream::space_and_emit(s, tailChar, "?");
	tailChar = print_suffix(s, tailChar, false);
	return tailChar;
}

//
//    Print text that follows tailChar to s that prefixes the declarator, returning the final character.
//
char FogModifier::print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const {
	return tailChar;
}

//
//    Print text that follows tailChar to s that suffixes the declarator, returning the final character.
//
char FogModifier::print_suffix(std::ostream& s, char tailChar, bool needsBrackets) const {
	return tailChar;
}

std::ostream& FogModifier::print_this(std::ostream& s) const {
	print_named(s, 0, 0);
	return s;
}

std::ostream& FogModifier::print_viz(std::ostream& s) const {
	s << "\"decl-modifier: ";
	print_named(s, 0, 0);
	return s << '\"';
}
