#include <Fog/FogIncludeAll.h>



//  %import Fog.hxx






TYPEINFO_SINGLE(FogRaws, Super)

FogRaws::FogRaws() {}
FogRaws::~FogRaws() {}

bool FogRaws::compile_declaration(FogParseContext& parseContext) const {
	//      FogDynamicParseContext childParseContext(parseContext, *this);
	bool allOk = true;
	
	for (FogRawConstListOfRefIterator p(_raws); p; ++p) {
		FogDynamicParseContext childParseContext(parseContext, *p);
		
		if (!p->compile_declaration(childParseContext))
			allOk = false;
	}
	
	return allOk;
}

bool FogRaws::compile_parsed_case(FogParseContext& parseContext, const FogToken& theCase) {
	FogBreakableParseContext switchContext(parseContext, *this);
	
	for (FogRawConstListOfRefIterator p(_raws); p; ++p) {
		FogRaw* rawCase = p->is_case(switchContext, theCase);
		
		if (rawCase) {
			if (!rawCase->compile_declaration(switchContext)) {
				if (switchContext.do_break())
					return true;
					
				return false;
			}
			
			for (++p; p; ++p) {
				if (!p->compile_declaration(switchContext)) {
					if (switchContext.do_break())
						return true;
						
					return false;
				}
			}
			
			return true;
		}
	}
	
	for (FogRawConstListOfRefIterator q(_raws); q; ++q) {
		FogRaw* rawCase = q->is_case(switchContext, FogToken::immutable_null());
		
		if (rawCase) {
			if (!rawCase->compile_declaration(switchContext)) {
				if (switchContext.do_break())
					return true;
					
				return false;
			}
			
			for (++q; q; ++q) {
				if (!q->compile_declaration(switchContext)) {
					if (switchContext.do_break())
						return true;
						
					return false;
				}
			}
			
			return true;
		}
	}
	
	WRNMSG("No default case found.");
	return true;
}

void FogRaws::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
		p->create_usage(aContext);
}

//  bool FogRaws::emit(FogEmitContext& emitContext) const
//  {
//      for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
//      {
//          p->emit(emitContext);
//          if (p.to_go() > 1)
//              emitContext.emit_space_and_text(" ");
//      }
//      return true;
//  }

bool FogRaws::emit(FogEmitContext& emitContext) const {
	const char* oldSeparator = emitContext.separator();
	const char* oldTerminator = emitContext.terminator();
	
	if (!oldSeparator)
		ERRMSG("INVESTIGATE -- no separator defined for FogRaws::emit " << viz(*this));
		
	bool gotSomething = true;
	
	if (is_exposed()) {
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : " ", oldTerminator);
		gotSomething = false;
		
		for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p) {
			if (!p->emit(separatedContext)) {
				if (emitContext.do_jump())
					return false;
			}
			else {
				gotSomething = true;
				
				if ((p.to_go() > 1) && separatedContext.separate())
					gotSomething = true;
			}
		}
	}
	else if (!_raws.tally())
		emitContext.emit_space_and_text(" {}");
	else {
		emitContext.start();
		emitContext.emit_space_and_text("{");
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator, "\n");
		{
			FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
			
			for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p) {
				indentedContext.start();
				p->emit(indentedContext);
				
				if (p.to_go() > 1)
					indentedContext.separate();
			}
		}
		separatedContext.terminate();
		emitContext.emit_space_and_text("}");
	}
	
	return gotSomething;
}

bool FogRaws::emit_case(FogEmitContext& emitContext, const FogToken& theCase) const {
	FogBreakableEmitContext switchContext(emitContext, *this);
	
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p) {
		const FogRaw* rawCase = p->is_case(switchContext, theCase);
		
		if (rawCase) {
			if (!rawCase->emit(switchContext)) {
				if (switchContext.do_break())
					return true;
					
				return false;
			}
			
			for (++p; p; ++p) {
				if (!p->emit(switchContext)) {
					if (switchContext.do_break())
						return true;
						
					return false;
				}
			}
			
			return true;
		}
	}
	
	for (FogRawConstListOfRefToConstIterator q(_raws); q; ++q) {
		const FogRaw* rawCase = q->is_case(switchContext, FogToken::immutable_null());
		
		if (rawCase) {
			if (!rawCase->emit(switchContext)) {
				if (switchContext.do_break())
					return true;
					
				return false;
			}
			
			for (++q; q; ++q) {
				if (!q->emit(switchContext)) {
					if (switchContext.do_break())
						return true;
						
					return false;
				}
			}
			
			return true;
		}
	}
	
	WRNMSG("No default case found.");
	return false;
}

size_t FogRaws::executable_tokens() const {
	size_t executableTokens = 0;
	
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
		executableTokens += p->executable_tokens();
		
	return executableTokens;
}

void FogRaws::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
		p->install(installContext);
}

bool FogRaws::is_actual(const FogScopeContext& scopeContext) const {
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return Super::is_actual(scopeContext);
}

bool FogRaws::is_compound() const { return is_exposed() == IS_ENCAPSULATED; }
FogToken::IsExposed FogRaws::is_exposed() const { return IS_EXPOSED; }
FogRaws* FogRaws::is_raw_list() { return this; }

FogLine& FogRaws::line() const {
	if (_raws.tally())
		return _raws[0]->line();
	else
		return Super::line();
}

void FogRaws::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogRawListOfRefToConstIterator p(_raws); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
}

void FogRaws::merge_from(FogMergeContext& mergeContext, const This& thatDeclaration) {
	ERRMSG("BUG -- did not expect to FogRaws::merge_from.");
	Super::merge_from(mergeContext, thatDeclaration);
	Super::merge_short_into(mergeContext, _raws, thatDeclaration._raws);
}

const FogMerge& FogRaws::needs_merge_from(FogMergeContext& mergeContext, const This& aRaw) const {
	ERRMSG("BUG -- did not expect to FogRaws::needs_merge_from.");
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, aRaw));
	FogRawConstListOfRefToConstIterator p1(_raws);
	FogRawConstListOfRefToConstIterator p2(aRaw._raws);
	
	if (p1.to_go() != p2.to_go())
		return FogMerge::set_incompatible(FogMerge::incompatible_dimension());
		
	for ( ; p1 && p2; ++p1, ++p2)
		needsMerge |= p1->needs_merge(mergeContext, *p2);
		
	return needsMerge;
}

std::ostream& FogRaws::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_raws.tally()) {
		s << indent(aDepth) << "raws\n";
		_raws.print_depth(s, aDepth);
	}
	
	return s;
}

std::ostream& FogRaws::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_raws.tally()) {
		s << indent(aDepth) << "raws\n";
		_raws.print_members(s, aDepth);
	}
	
	return s;
}

char FogRaws::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (_raws.tally()) {
		tailChar = _raws[0]->print_named(s, scopeId, tailChar);
		s << " ";
	}
	
	s << "...";
	return '.';
}

std::ostream& FogRaws::print_this(std::ostream& s) const {
	print_named(s, 0, 0);
	return s;
}

std::ostream& FogRaws::print_viz(std::ostream& s) const {
	s << "\"raws : ";
	print_named(s, 0, 0);
	return s << '\"';
}

void FogRaws::set_explicit_instantiation() {
	for (FogRawConstListOfRefIterator p(_raws); p; ++p)
		p->set_explicit_instantiation();
}

size_t FogRaws::source_columns() const {
	size_t sourceCols = 0;
	
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
		sourceCols += p->source_columns();
		
	return sourceCols;
}

size_t FogRaws::source_lines() const {
	size_t sourceLines = 0;
	
	for (FogRawConstListOfRefToConstIterator p(_raws); p; ++p)
		sourceLines += p->source_lines();
		
	return sourceLines;
}
