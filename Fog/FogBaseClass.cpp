#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogBaseClass, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBaseClass)
PRIMREFS_IMPL(FogBaseClass)
PRIMLISTOFREFS_IMPL(FogBaseClass)
PRIMMAPOFREFS_IMPL(FogBaseClass)
FOGTOKEN_LEAF_IMPL(FogBaseClass)
TMPL_HACK_FIX_DO(FogBaseClass)

FogBaseClass::FogBaseClass()
		:
		_base(Fog::null_scope()),
		_compiled(false) {}
		
FogBaseClass::FogBaseClass(FogScope& baseClass, const FogAccess& anAccess, const FogVirtual& aVirtual)
		:
		_base(baseClass),
		_access(anAccess),
		_virtual(FogVirtual::VIRTUAL),
		_base_id(baseClass.global_signature_id()),
		_compiled(false) {
	set_virtual(aVirtual);
	_base.set_not_leaf();
}

FogBaseClass::~FogBaseClass() {}

void FogBaseClass::compile(const FogScope& inScope) {
	if (!access())
		set_access(FogAccess::private_access());
		
	if (!get_virtual())
		set_virtual(FogVirtual::not_virtual());
		
	if (_virtual->is_virtual())
		_base_id = id();
	else {
		PrimOstrstream s;
		s << inScope.global_signature_id() << "::" << id();
		_base_id = s.id();
	}
	
	_compiled = true;
}

void FogBaseClass::do_compile(FogCompileContext& inScope) {
	if (!_base.done_compile()) {
		ERRMSG("INVESTIGATE - unexpected non-compilation of " << viz(*this));
		_base.do_compile(inScope);
	}
	
	if (!_compiled)
		compile(inScope.dynamic_scope());
}

void FogBaseClass::emit_interface(FogEmitContext& emitContext) const {
	if (!_compiled)
		ERRMSG("BUG -- should not emit un-compiled " << viz(*this));
		
	const FogVirtual& theVirtual = get_virtual();
	
//  	if (!theVirtual.is_redundant())
	if (theVirtual.is_virtual())
		emitContext.emit_space_and_text(theVirtual.str());
		
	emitContext.emit_space_and_text(access().str());
	
	emitContext.emit_space_and_text(_base.global_signature_id().str());
}

const FogScope *FogBaseClass::find_bases(const FogScope& inScope, PrimIdMap& visitMap,
		FogScopeListOfRef& nonVirtualBases, FogScopeListOfRef& virtualBases) const {
	if (!_compiled)
		mutate().compile(inScope);
		
	const FogScope *multipleBase = visitMap.add_filtered(*_base_id) ? 0 : &_base;
	
	bool isVirtual = _virtual->is_virtual() || _access->is_auto();
	
	if (!multipleBase) {
		multipleBase = _base.find_bases(visitMap, nonVirtualBases, virtualBases);
		
		if (!isVirtual)
			nonVirtualBases.add(_base);
		else
			virtualBases.add(_base);
	}
	
	return isVirtual ? 0 : multipleBase;
}

const PrimId& FogBaseClass::id() const {
	return _base.global_signature_id();
}

FogBaseClass *FogBaseClass::is_base_class() {
	return this;
}

void FogBaseClass::merge_from(FogMergeContext& mergeContext, const This& anExpr) {
	if (_compiled)
		ERRMSG("BUG -- should not merge un-compiled " << viz(*this));
		
	Super::merge_from(mergeContext, anExpr);
	
	set_virtual(get_virtual().merge_virtual(anExpr.get_virtual()));
	
	set_access(access().merge_access(anExpr.access()));
}

const FogMetaType& FogBaseClass::meta_type() const {
	return FogMetaType::base_type();
}

const FogMerge& FogBaseClass::needs_merge_from(FogMergeContext& mergeContext, const This& baseClass) const {
	if (&_base != &baseClass._base)
		return FogMerge::set_incompatible(FogMerge::incompatible_type_identifier());
		
	FogMergeHandle needsMerge = get_virtual().needs_merge_virtual(baseClass.get_virtual());
	
	if (needsMerge->is_incompatible())
		return needsMerge;
		
	needsMerge |= access().needs_merge_access(baseClass.access());
	
	return needsMerge;
}

std::ostream& FogBaseClass::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_base.print_deep(s, aDepth+1);
	return s;
}

std::ostream& FogBaseClass::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_base.print_on(s, aDepth+1);
	return s;
}

std::ostream& FogBaseClass::print_this(std::ostream& s) const {
	_base.print_this(s);
	s << ", " << access().str() << ", " << get_virtual().str();
	return s;
}

std::ostream& FogBaseClass::print_long_id(std::ostream& s) const {
	return _base.print_long_id(s);
}

std::ostream& FogBaseClass::print_viz(std::ostream& s) const {
	s << "\"base: " << get_virtual().str() << ' ' << access().str() << ' ';
	_base.print_this(s);
	return s << '\"';
}

void FogBaseClass::set_access(const FogAccess& anAccess) {
	if (anAccess == *_access)
		return;
		
	if (_compiled)
		ERRMSG("BUG -- should not change compiled access of " << viz(*this));
		
	_access = anAccess;
}

void FogBaseClass::set_virtual(const FogVirtual& aVirtual) {
	if (aVirtual == *_virtual)
		return;
		
	if (_compiled)
		ERRMSG("BUG -- should not change compiled virtuality of " << viz(*this));
		
	_virtual = aVirtual;
}

//  FogTokenType::TokenType FogBaseClass::token_type_enum() const { return FogTokenType::BaseSpecifier; }
