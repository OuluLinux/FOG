#include <Fog/FogIncludeAll.h>








class FogFileDependencySpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogFileDependencySpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	
private:
	FogFileIdRefToConst _file_id;
	
protected:
	FogFileDependencySpecifier(const FogFileId& fileId) : _file_id(fileId) {}
	
	virtual ~FogFileDependencySpecifier() {}
	
	const FogFileId& file_id() const {
		return *_file_id;
	}
	
public:
	virtual const FogMetaType& meta_type() const {
		return FogMetaType::file_dependency_specifier_type();
	}
	
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const {
		theSemantics.assign(FogSemantics::IS_FILE_DEPENDENCY_SPECIFIER);
		return true;
	}
};

class FogFilePlacementSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogFilePlacementSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	
protected:
	FogFilePlacementSpecifier() {}
	
	virtual ~FogFilePlacementSpecifier() {}
	
public:
	virtual const FogMetaType& meta_type() const {
		return FogMetaType::file_placement_specifier_type();
	}
	
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const {
		theSemantics.assign(FogSemantics::IS_FILE_PLACEMENT_SPECIFIER);
		return true;
	}
};

class FogExportFileSpecifier : public FogFilePlacementSpecifier {
	typedef FogFilePlacementSpecifier Super;
	typedef FogExportFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	
private:
	FogFileIdRefToConst _file_id;
	
protected:
	FogExportFileSpecifier(const FogFileId& fileId) : _file_id(fileId) {}
	
	virtual ~FogExportFileSpecifier() {}
	
	const FogFileId& file_id() const {
		return *_file_id;
	}
	
public:
	virtual std::ostream& print_this(std::ostream& s) const {
		return _file_id->print_this(s);
	}
};

#if 0

class FogExportImplementationCodeSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogExportImplementationCodeSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_ACTUAL_DECLS
	
private:
	const FogList& _code;
	
protected:
	virtual ~FogExportImplementationCodeSpecifier() {}
	
public:
	FogExportImplementationCodeSpecifier(const FogList& someCode) : _code(someCode) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext) {
		parseContext.scope().set_implementation_code(_code);
		return true;
	}
	
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/implementation {...}");
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-implementation-code :\"";
	}
};

#endif

class FogExportAllFileSpecifier : public FogExportFileSpecifier {
	typedef FogExportFileSpecifier Super;
	typedef FogExportAllFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
protected:
	virtual ~FogExportAllFileSpecifier() {}
	
public:
	FogExportAllFileSpecifier(const FogFileId& fileId) : Super(fileId) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/all");
//         tailChar = FogStream::space_and_emit(s, tailChar, file_id().str());
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-all-file : " << viz(file_id()) << '\"';
	}
};

class FogExportImplementationFileSpecifier : public FogExportFileSpecifier {
	typedef FogExportFileSpecifier Super;
	typedef FogExportImplementationFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
protected:
	virtual ~FogExportImplementationFileSpecifier() {}
	
public:
	FogExportImplementationFileSpecifier(const FogFileId& fileId) : Super(fileId) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/implementation");
//         tailChar = FogStream::space_and_emit(s, tailChar, file_id().str());
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-implementation-file : " << viz(file_id()) << '\"';
	}
};

#if 0

class FogExportInterfaceCodeSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogExportInterfaceCodeSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_ACTUAL_DECLS
	
private:
	const FogList& _code;
	
protected:
	virtual ~FogExportInterfaceCodeSpecifier() {}
	
public:
	FogExportInterfaceCodeSpecifier(const FogList& someCode) : _code(someCode) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext) {
		parseContext.scope().set_interface_code(_code);
		return true;
	}
	
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/interface {...}");
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-interface-code :\"";
	}
};

#endif

class FogExportInterfaceFileSpecifier : public FogExportFileSpecifier {
	typedef FogExportFileSpecifier Super;
	typedef FogExportInterfaceFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
protected:
	virtual ~FogExportInterfaceFileSpecifier() {}
	
public:
	FogExportInterfaceFileSpecifier(const FogFileId& fileId) : Super(fileId) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/interface");
//          tailChar = FogStream::space_and_emit(s, tailChar, file_id().str());
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-interface-file : " << viz(file_id()) << '\"';
	}
};

class FogExportNoImplementationSpecifier : public FogFilePlacementSpecifier {
	typedef FogFilePlacementSpecifier Super;
	typedef FogExportNoImplementationSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
protected:
	virtual ~FogExportNoImplementationSpecifier() {}
	
public:
	FogExportNoImplementationSpecifier() {}
	
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/noimplementation");
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-no-implementation :\"";
	}
};

class FogExportNoInterfaceFileSpecifier : public FogFilePlacementSpecifier {
	typedef FogFilePlacementSpecifier Super;
	typedef FogExportNoInterfaceFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_ACTUAL_DECLS
	
protected:
	virtual ~FogExportNoInterfaceFileSpecifier() {}
	
public:
	FogExportNoInterfaceFileSpecifier() {}
	
	virtual bool compile_parsed(FogParseContext& parseContext) {
		parseContext.dynamic_scope().set_no_interface();
		return true;
	}
	
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
		makeSpecifierContext.dynamic_scope().set_no_interface();
		return true;
	}
	
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/nointerface");
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-no-interface :\"";
	}
};

class FogExportUtilitySpecifier : public FogFilePlacementSpecifier {
	typedef FogFilePlacementSpecifier Super;
	typedef FogExportUtilitySpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_ACTUAL_DECLS
	
private:
	const FogUtility& _utility;
	
protected:
	virtual ~FogExportUtilitySpecifier() {}
	
public:
	FogExportUtilitySpecifier(const FogUtility& aUtility) : _utility(aUtility) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext) {
		parseContext.set_utility(_utility);
		return true;
	}
	
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
		makeSpecifierContext.parse_context().set_utility(_utility);
		return true;
	}
	
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "export/utility");
		return tailChar;
	}
	
	virtual std::ostream& print_this(std::ostream& s) const {
		return s << _utility;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"export-utility : " << _utility << '\"';
	}
};

class FogUsingImplementationFileSpecifier : public FogExportFileSpecifier {
	typedef FogExportFileSpecifier Super;
	typedef FogUsingImplementationFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
protected:
	virtual ~FogUsingImplementationFileSpecifier() {}
	
public:
	FogUsingImplementationFileSpecifier(const FogFileId& fileId) : Super(fileId) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "using/implementation");
//          tailChar = FogStream::space_and_emit(s, tailChar, file_id().str());
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"using-implementation-file : " << viz(file_id()) << '\"';
	}
};

class FogUsingInterfaceFileSpecifier : public FogExportFileSpecifier {
	typedef FogExportFileSpecifier Super;
	typedef FogUsingInterfaceFileSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
protected:
	virtual ~FogUsingInterfaceFileSpecifier() {}
	
public:
	FogUsingInterfaceFileSpecifier(const FogFileId& fileId) : Super(fileId) {}
	
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
		tailChar = FogStream::space_and_emit(s, tailChar, "using/interface");
//          tailChar = FogStream::space_and_emit(s, tailChar, file_id().str());
		return tailChar;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"using-interface-file : " << viz(file_id()) << '\"';
	}
};

//  TYPEINFO_SINGLE(FogSetSpecifier, Super)
TYPEINFO_SINGLE(FogExportFileSpecifier, Super)
TYPEINFO_SINGLE(FogExportAllFileSpecifier, Super)
//  TYPEINFO_SINGLE(FogExportImplementationCodeSpecifier, Super)
TYPEINFO_SINGLE(FogExportImplementationFileSpecifier, Super)
//  TYPEINFO_SINGLE(FogExportInterfaceCodeSpecifier, Super)
TYPEINFO_SINGLE(FogExportInterfaceFileSpecifier, Super)
TYPEINFO_SINGLE(FogExportNoImplementationSpecifier, Super)
TYPEINFO_SINGLE(FogExportNoInterfaceFileSpecifier, Super)
TYPEINFO_SINGLE(FogExportUtilitySpecifier, Super)
TYPEINFO_SINGLE(FogFileDependencySpecifier, Super)
TYPEINFO_SINGLE(FogFilePlacementSpecifier, Super)
TYPEINFO_SINGLE(FogUsingImplementationFileSpecifier, Super)
TYPEINFO_SINGLE(FogUsingInterfaceFileSpecifier, Super)

//  FOGTOKEN_ACTUAL_IMPL(FogExportImplementationCodeSpecifier)
//  FOGTOKEN_ACTUAL_IMPL(FogExportInterfaceCodeSpecifier)
FOGTOKEN_LEAF_IMPL(FogExportAllFileSpecifier)
FOGTOKEN_LEAF_IMPL(FogExportImplementationFileSpecifier)
FOGTOKEN_LEAF_IMPL(FogExportInterfaceFileSpecifier)
FOGTOKEN_LEAF_IMPL(FogExportNoImplementationSpecifier)
FOGTOKEN_ACTUAL_IMPL(FogExportNoInterfaceFileSpecifier)
FOGTOKEN_ACTUAL_IMPL(FogExportUtilitySpecifier)
FOGTOKEN_LEAF_IMPL(FogUsingImplementationFileSpecifier)
FOGTOKEN_LEAF_IMPL(FogUsingInterfaceFileSpecifier)

//  FogRaw *FogRaw::new_set_implementation(const FogList& someCode)
//  {
//    return new FogExportImplementationCodeSpecifier(someCode);
//  }

FogRaw *FogRaw::new_set_all(const FogFileId& fileId) {
	return new FogExportAllFileSpecifier(fileId);
}

FogRaw *FogRaw::new_set_implementation(const FogFileId& fileId) {
	return new FogExportImplementationFileSpecifier(fileId);
}

//  FogRaw *FogRaw::new_set_interface(const FogList& someCode)
//  {
//    return new FogExportInterfaceCodeSpecifier(someCode);
//  }

FogRaw *FogRaw::new_set_interface(const FogFileId& fileId) {
	return new FogExportInterfaceFileSpecifier(fileId);
}

FogRaw *FogRaw::new_set_no_implementation() {
	return new FogExportNoImplementationSpecifier();
}

FogRaw *FogRaw::new_set_no_interface() {
	return new FogExportNoInterfaceFileSpecifier();
}

FogRaw *FogRaw::new_set_utility(const FogUtility& aUtility) {
	return new FogExportUtilitySpecifier(aUtility);
}

FogRaw *FogRaw::new_use_implementation(const FogFileId& fileId) {
	return new FogUsingImplementationFileSpecifier(fileId);
}

FogRaw *FogRaw::new_use_interface(const FogFileId& fileId) {
	return new FogUsingInterfaceFileSpecifier(fileId);
}

void FogExportFileSpecifier::create_usage(FogUsageContext& aContext) const {
	ERRMSG("BUG - should not FogExportFileSpecifier::create_usage");
}

bool FogExportFileSpecifier::emit(FogEmitContext& emitContext) const {
	ERRMSG("BUG - should not FogExportFileSpecifier::emit");
	return false;
}

void FogExportFileSpecifier::install(FogInstallContext& installContext) const {
	ERRMSG("BUG - should not FogExportFileSpecifier::install");
}

bool FogExportFileSpecifier::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _file_id->is_actual(scopeContext);
}

void FogExportFileSpecifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_file_id->make_actual(_file_id.to_const(), makeActualContext);
}

void FogExportFileSpecifier::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._file_id->merge_into(mergeContext, _file_id.to_const());
}

const FogMerge& FogExportFileSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	needsMerge |= _file_id->needs_merge(mergeContext, *thatExpr._file_id);
	return needsMerge;
}

char FogExportFileSpecifier::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	return Super::print_named(s, fullId, tailChar);
}

bool FogExportAllFileSpecifier::compile_parsed(FogParseContext& parseContext) {
	parseContext.dynamic_entity().set_implementation_file(file_id().make(parseContext));
	parseContext.dynamic_entity().set_interface_file(file_id().make(parseContext));
	return true;
}

bool FogExportAllFileSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	makeSpecifierContext.dynamic_entity().set_implementation_file(file_id().make(makeSpecifierContext));
	makeSpecifierContext.dynamic_entity().set_interface_file(file_id().make(makeSpecifierContext));
	return true;
}

bool FogExportImplementationFileSpecifier::compile_parsed(FogParseContext& parseContext) {
	parseContext.dynamic_entity().set_implementation_file(file_id().make(parseContext));
	return true;
}

bool FogExportImplementationFileSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	makeSpecifierContext.dynamic_entity().set_implementation_file(file_id().make(makeSpecifierContext));
	return true;
}

bool FogExportInterfaceFileSpecifier::compile_parsed(FogParseContext& parseContext) {
	parseContext.dynamic_entity().set_interface_file(file_id().make(parseContext));
	return true;
}

bool FogExportInterfaceFileSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	makeSpecifierContext.dynamic_entity().set_interface_file(file_id().make(makeSpecifierContext));
	return true;
}

void FogFileDependencySpecifier::create_usage(FogUsageContext& aContext) const {
	ERRMSG("BUG - should not FogFileDependencySpecifier::create_usage");
}

bool FogFileDependencySpecifier::emit(FogEmitContext& emitContext) const {
	ERRMSG("BUG - should not FogFileDependencySpecifier::emit");
	return false;
}

void FogFileDependencySpecifier::install(FogInstallContext& installContext) const {
	ERRMSG("BUG - should not FogFileDependencySpecifier::install");
}

bool FogFileDependencySpecifier::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _file_id->is_actual(scopeContext);
}

void FogFileDependencySpecifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_file_id->make_actual(_file_id.to_const(), makeActualContext);
}

char FogFileDependencySpecifier::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	return Super::print_named(s, fullId, tailChar);
}

bool FogUsingImplementationFileSpecifier::compile_parsed(FogParseContext& parseContext) {
	FogTargetFile *aFile = file_id().make(parseContext);
	
	if (aFile)
		parseContext.dynamic_scope().add_use_for_implementation(*aFile);
		
	return true;
}

bool FogUsingImplementationFileSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogTargetFile *aFile = file_id().make(makeSpecifierContext);
	
	if (aFile)
		makeSpecifierContext.dynamic_scope().add_use_for_implementation(*aFile);
		
	return true;
}

bool FogExportNoImplementationSpecifier::compile_parsed(FogParseContext& parseContext) {
	const FogScopeContext* ctx = &parseContext;
	while (ctx) {
	    /*std::strstream str;
		ctx->print_viz(str);
		printf("NO_IMPLEMENTATION\n%s\n", str.str());*/
		if (ctx->function) {
			ctx->function->set_explicit_no_implementation();
			return true;
		}
		ctx = ctx->get_parent();
	}
	
	ERRMSG("export/noimplementation is not in function");
	return true;
}

bool FogExportNoImplementationSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
    const FogScopeContext* ctx = &makeSpecifierContext;
	while (ctx) {
	    /*std::strstream str;
		ctx->print_viz(str);
		printf("NO_IMPLEMENTATION\n%s\n", str.str());*/
		if (ctx->function) {
			ctx->function->set_explicit_no_implementation();
			return true;
		}
		ctx = ctx->get_parent();
	}
	
	ERRMSG("export/noimplementation is not in function");
	return true;
}

bool FogUsingInterfaceFileSpecifier::compile_parsed(FogParseContext& parseContext) {
	FogTargetFile *aFile = file_id().make(parseContext);
	
	if (aFile)
		parseContext.dynamic_scope().add_use_for_interface(*aFile);
		
	return true;
}

bool FogUsingInterfaceFileSpecifier::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogTargetFile *aFile = file_id().make(makeSpecifierContext);
	
	if (aFile)
		makeSpecifierContext.dynamic_scope().add_use_for_interface(*aFile);
		
	return true;
}
