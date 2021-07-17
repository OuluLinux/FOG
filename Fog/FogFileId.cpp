#include <Fog/FogIncludeAll.h>










class FogFileImplementation : public FogFileId {
	typedef FogFileId Super;
	TYPEDECL_SINGLE(FogFileImplementation, Super)
private:         //   Defining contents.
	const FogNameRefToConst _decl;
protected:
	virtual ~FogFileImplementation() {}
public:
	FogFileImplementation(const FogName& aDecl) : _decl(aDecl) {}
	virtual FogTargetFile* make(FogScopeContext& scopeContext) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};

class FogFileInterface : public FogFileId {
	typedef FogFileId Super;
	TYPEDECL_SINGLE(FogFileInterface, Super)
private:         //   Defining contents.
	const FogNameRefToConst _decl;
protected:
	virtual ~FogFileInterface() {}
public:
	FogFileInterface(const FogName& aDecl) : _decl(aDecl) {}
	virtual FogTargetFile* make(FogScopeContext& scopeContext) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};

TYPEINFO_SINGLE(FogFileId, Super)
FOGTOKEN_ACTUAL_IMPL(FogFileId)
TYPEINFO_SINGLE(FogFileImplementation, Super)
TYPEINFO_SINGLE(FogFileInterface, Super)
PRIMREF_NULL_CLAS(FogFileId)
virtual FogTargetFile* make(FogScopeContext& scopeContext) const { return 0; }
};
PRIMREF_DERIVED_NULL_IMPL(FogFileId)
PRIMREFS_IMPL(FogFileId)
TMPL_HACK_FIX_DO(FogFileId)

FogFileIdent* FogFileId::new_id(const PrimId& anId) {
	return new FogFileIdent(anId.str(), PrimId::null());
}

FogFileId* FogFileId::new_implementation(const FogName& aDecl) {
	return new FogFileImplementation(aDecl);;
}

FogFileId* FogFileId::new_interface(const FogName& aDecl) {
	return new FogFileInterface(aDecl);;
}

FogTargetFile* FogFileImplementation::make(FogScopeContext& scopeContext) const {
	//  	FogEntity *anEntity = _decl->find_entity_in(scopeContext.dynamic_scope());
	FogEntity* anEntity = _decl->find_entity_in(scopeContext);
	FogTargetFile* aFile = anEntity ? anEntity->implementation_file() : 0;
	
	if (!aFile) {
		aFile = anEntity ? anEntity->interface_file_source() : 0;
		
		if (aFile && aFile->utility().is_hash())
			;          //   Don't winge if interface was a #include.
		else if (anEntity && anEntity->utility().is_hash())
			;          //   Don't winge if entity from a #include.
		else if (anEntity)
			ERRMSG("Failed to find an implementation file for " << viz(*anEntity));
		else
			ERRMSG("Failed to find an implementation file in " << viz(*_decl));
	}
	else if (is_input())
		aFile->set_is_input();
		
	return aFile;
}

std::ostream& FogFileImplementation::print_this(std::ostream& s) const {
	return _decl->print_this(s);
}

std::ostream& FogFileImplementation::print_viz(std::ostream& s) const {
	s << "\"file/implementation ";
	_decl->print_this(s);
	s << '\"';
	return s;
}

FogTargetFile* FogFileInterface::make(FogScopeContext& scopeContext) const {
	//  	FogEntity *anEntity = _decl->find_entity_in(scopeContext.dynamic_scope());
	FogEntity* anEntity = _decl->find_entity_in(scopeContext);
	FogTargetFile* aFile = anEntity ? anEntity->interface_file_source() : 0;
	
	if (!aFile)
		ERRMSG("Failed to find an interface file in " << viz(*_decl));
	else if (is_input())
		aFile->set_is_input();
		
	return aFile;
}

std::ostream& FogFileInterface::print_this(std::ostream& s) const {
	return _decl->print_this(s);
}

std::ostream& FogFileInterface::print_viz(std::ostream& s) const {
	s << "\"file/interface ";
	_decl->print_this(s);
	s << '\"';
	return s;
}
