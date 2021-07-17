#include <Fog/FogIncludeAll.h>


TYPEINFO_SINGLE(FogCode, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogCode)
PRIMREFS_IMPL(FogCode)
PRIMLISTOFREFS_IMPL(FogCode)
FOGTOKEN_LEAF_IMPL(FogCode)
TMPL_HACK_FIX_DO(FogCode)

FogCode::FogCode()
	:
	_is_exclusive(false),
	_is_not_implemented(false),
	_is_redundant(false),
	_is_replacement(false) {}

//
//  	Create a code segment from { aString }, for which the { occured in braceColumn at lineContext.
//
//  	The text of aString is normalised according to whether a compact source representation has been used.
//  	Compact source is a one line format in which aString is new-line free, typically used for inlines.
//  	Normalisation is to remove the leading whitespace present to pretty format with respect to the
//  	defining brace. Subserquent denormalisation occurs during emission to preserve the layout but
//  	with respect to the position of the emitted defininbg brace.
//
//  	Compact source just has leading and trailing whitespace stripped, then a leading tab added
//  	so that it indents with respect to the defining brace.
//
//  	Normal source just has leading and trailing whitespace stripped that matches that present
//  	in the source. Extra leading and trailing whitespace is preserved for re-use on emission.
//
FogCode::FogCode(FogRaw& codeBody)
	:
	_is_exclusive(false),
	_is_not_implemented(false),
	_is_redundant(false),
	_is_replacement(false),
	_body(codeBody) {}

FogCode::FogCode(const FogCode& thatCode)
	:
	Super(thatCode),
	_position(thatCode._position),
	_segment(thatCode._segment),
	_used_files(thatCode._used_files),
	_is_exclusive(thatCode._is_exclusive),
	_is_not_implemented(thatCode._is_not_implemented),
	_is_redundant(thatCode._is_redundant),
	_is_replacement(thatCode._is_replacement),
	_ctor_inits(thatCode._ctor_inits),
	_body(thatCode._body) {}

FogCode::~FogCode() {}

void FogCode::compile() {
	//  	if (!_position.is_valid())
	//  		_position = FogPosition::EXCLUSIVE;
	if (!_segment->is_valid())
		_segment = FogSegment::BODY;
		
	if (is_redundant() && !segment().is_body())
		WRNMSG("Only code for a body segment should be redundant in " << viz(*this));
}

bool FogCode::compile_parsed(FogParseContext& parseContext) {
	ERRMSG("BUG should not FogCode::compile_parsed.");
	return true;
}

//  void FogCode::create_usages(const FogCompileContext& compileContext, FogTargetFile& byFile) const
//  {
//  	if (compileContext.derive_at(_derivation))
//  		for (FogFileIdConstListOfRefToConstIterator p(_used_files); p; ++p)
//  			/*byFile.add_include(*p*/;				//.bugbug - transfer to actual at some point.
//  }

void FogCode::create_usage(FogUsageContext& aContext) const {
	ERRMSG("BUG -- should not FogCode::create_usage.");
}

size_t FogCode::executable_tokens() const {
	return _body->executable_tokens();
}

bool FogCode::emit(FogEmitContext& emitContext) const {
	//  	ERRMSG("BUG -- should not FogCode::emit.");
	emitContext.emit_space_and_text(" /*...*/ ");
	return true;
}

void FogCode::install(FogInstallContext& installContext) const {
	ERRMSG("BUG -- should not FogCode::install.");
}

bool FogCode::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _used_files->is_actual(scopeContext)
	       && _ctor_inits->is_actual(scopeContext) && _body->is_actual(scopeContext);
}

void FogCode::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	//  	_used_files->make_actual(_used_files.to_const(), makeActualContext);
	_ctor_inits->make_actual(_ctor_inits.to_const(), makeActualContext);
	_body->make_actual(_body.to_const(), makeActualContext);
}

bool FogCode::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	compile();
	FogToken& inToken = makeSpecifierContext.dynamic_token();
	FogPotentialDeclaration* potentialDeclaration = inToken.is_potential_declaration();
	FogFunction* aFunction = potentialDeclaration ? potentialDeclaration->entity().is_function() : 0;
	
	if (!aFunction) {
		ERRMSG("Cannot compile code for non-function " << viz(inToken));
		return false;
	}
	
	FogCtorInitMapOfRefToConst& ctorInits = aFunction->raw_ctor_inits();
	FogCodeTokensRef* derivedCodes = aFunction->raw_derived_codes();
	FogCodeTokensRef* localCodes = aFunction->raw_local_codes();
	
	if (_is_not_implemented)
		aFunction->set_not_implemented();
		
	for (FogRawCtorInitConstListOfRefToConstIterator p(_ctor_inits->inits()); p; ++p) {
		FogCompileMakeEntityContext makeContext(makeSpecifierContext);
		p->compile_ctor(makeContext, ctorInits);
	}
	
	if (_body) {
		FogCodeTokensRef& theTokens = localCodes[_segment->value()];
		
		if (!theTokens)
			theTokens.adopt(new FogCodeTokens);
			
		if (theTokens)
			theTokens->add(*_body);
	}
	
	FogTargetFile* targetFile = aFunction->implementation_file();
	
	if (targetFile)
		for (FogFileIdConstListOfRefToConstIterator p(_used_files->ids()); p; ++p) {
			FogTargetFile* includeFile = p->make(makeSpecifierContext);
			
			if (includeFile)
				targetFile->add_include(*includeFile);
		}
		
	//  	if (_implementation)
	//  	{
	//  		FogTargetFile *targetFile = _implementation->make(parseContext);
	//  		aFunction.set_implementation_file(targetFile);
	//  	}
	return true;
}

void FogCode::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._used_files->merge_into(mergeContext, _used_files.to_const());
	thatExpr._ctor_inits->merge_into(mergeContext, _ctor_inits.to_const());
	thatExpr._body->merge_into(mergeContext, _body.to_const());
}

const FogMetaType& FogCode::meta_type() const {
	return FogMetaType::function_body_type();
}

const FogMerge& FogCode::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	needsMerge |= _used_files->needs_merge(mergeContext, *thatExpr._used_files);
	needsMerge |= _ctor_inits->needs_merge(mergeContext, *thatExpr._ctor_inits);
	needsMerge |= _body->needs_merge(mergeContext, *thatExpr._body);
	return needsMerge;
}

std::ostream& FogCode::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	s << indent(aDepth) << "ctor-inits\n";
	_ctor_inits->print_depth(s, aDepth + 1);
	_body->print_depth(s, aDepth);
	
	if (_used_files->tally()) {
		s << indent(aDepth) << "files used for implementation\n";
		_used_files->print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogCode::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	s << indent(aDepth) << "ctor-inits\n";
	_ctor_inits->print_members(s, aDepth + 1);
	_body->print_members(s, aDepth);
	
	if (_used_files->tally()) {
		s << indent(aDepth) << "files used for implementation\n";
		_used_files->print_members(s, aDepth + 1);
	}
	
	return s;
}

char FogCode::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, "{ /* ");
	tailChar = FogStream::space_and_emit(s, tailChar, segment().str());
	tailChar = FogStream::space_and_emit(s, tailChar, " */ }");
	return tailChar;
}

std::ostream& FogCode::print_this(std::ostream& s) const {
	return s << segment() << ", " << _body->source_lines();
}

std::ostream& FogCode::print_viz(std::ostream& s) const {
	return s << "\"code: " << segment() << '\"';
}

bool FogCode::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogSemantics exprSemantics;
	//  	if (!expr().resolve_semantics(exprSemantics, inScope))
	//  		return false;
	theSemantics.reset();
	//  	if (exprSemantics.is_basic_expression())
	theSemantics.set(FogSemantics::IS_FUNCTION_BODY);
	return true;
}
