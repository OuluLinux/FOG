#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogBaseSpecifiers, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBaseSpecifiers)
PRIMREFS_IMPL(FogBaseSpecifiers)
FOGTOKEN_LEAF_IMPL(FogBaseSpecifiers)
TMPL_HACK_FIX_DO(FogBaseSpecifiers)

FogBaseSpecifiers::FogBaseSpecifiers() {}

FogBaseSpecifiers::FogBaseSpecifiers(const This& baseSpecifiers)
		:
		Super(baseSpecifiers),
		_bases(baseSpecifiers._bases) {}
		
FogBaseSpecifiers::~FogBaseSpecifiers() {}

void FogBaseSpecifiers::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	
	for (FogBaseSpecifierConstListOfRefToConstIterator p(_bases); p; ++p)
		p->create_usage(aContext);
}

bool FogBaseSpecifiers::emit(FogEmitContext& emitContext) const {
	for (FogBaseSpecifierConstListOfRefToConstIterator p(_bases); p; ++p)
		p->emit(emitContext);
		
	return true;
}

void FogBaseSpecifiers::install(FogInstallContext& installContext) const {
	Super::install(installContext);
//  	for (FogBaseSpecifierConstListOfRefToConstIterator p(_bases); p; ++p)
//  		p->install(installContext);
}

bool FogBaseSpecifiers::is_actual(const FogScopeContext& scopeContext) const {
	if (!Super::is_actual(scopeContext))
		return false;
		
	for (FogBaseSpecifierConstListOfRefToConstIterator p(_bases); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return true;
}

void FogBaseSpecifiers::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogBaseSpecifierListOfRefToConstIterator p(_bases); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
}

void FogBaseSpecifiers::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	FogBaseSpecifier::merge_long_into(mergeContext, _bases, thatExpr._bases);
}

const FogMerge& FogBaseSpecifiers::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
#if 0
	FogBaseSpecifierConstListOfRefToConstIterator p1(_bases);
	FogBaseSpecifierConstListOfRefToConstIterator p2(anExpr._bases);
	
	for (; p1 && p2; ++p1, ++p2) {
		if (p1->is_null()) {
			if (!p2->is_null())
				needsMerge |= FogMerge::left_invalid();
		}
		
		else {
			if (!p2->is_null())
				needsMerge |= p1->needs_merge(mergeContext, *p2);
			else
				needsMerge |= FogMerge::right_invalid();
		}
	}
	
	if (p1)
		needsMerge |= FogMerge::right_invalid();
	else
		if (p2)
			needsMerge |= FogMerge::left_invalid();
			
#endif
	return needsMerge;
}

std::ostream& FogBaseSpecifiers::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_bases.tally()) {
		s << indent(aDepth) << "raw-bases\n";
		_bases.print_depth(s, aDepth+1);
	}
	
	return s;
}

std::ostream& FogBaseSpecifiers::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_bases.tally()) {
		s << indent(aDepth) << "raw-bases\n";
		_bases.print_members(s, aDepth+1);
	}
	
	return s;
}

char FogBaseSpecifiers::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	for (FogBaseSpecifierConstListOfRefToConstIterator p(_bases); p; ++p) {
		tailChar = p->print_named(s, 0, tailChar);
		
		if (p.to_go() > 1)
			tailChar = FogStream::space_and_emit(s, tailChar, ", ");
	}
	
	return tailChar;
}

std::ostream& FogBaseSpecifiers::print_viz(std::ostream& s) const {
	s << "\"base-specifiers: ";
	print_this(s);
	return s << '\"';
}
