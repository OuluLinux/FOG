
#ifndef FOGMAKERCONTEXT_HXX
#define FOGMAKERCONTEXT_HXX


//   Visual C++ needs to see FogScope to size FogEntity *(FogScope::*)(FogMakerContext&) properly!


//
//  	Decorate the context with a maker routine on behalf of numerous callers
//
class FogMakerContext : public FogDecoratedMakeEntityContext {
	typedef FogDecoratedMakeEntityContext Super;
	typedef FogMakerContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	//  	FogMakerContext(const This& aContext);
	This& operator=(const This& aContext);
protected:
	FogMakerContext(FogMakeEntityContext& makeEntityContext) : Super(makeEntityContext) {}
	FogMakerContext(FogMakerContext& makerContext) : Super(makerContext) {}
	PrimIdHandle make_scoped_id(const PrimId& unscopedId) const;
	PrimIdHandle make_templated_id(const PrimId& unscopedId) const;
public:
	virtual FogEntity* find_entity(FindStrategy aStrategy);
	FogEntity* find_entity() { return find_entity(FIND_ENTITY); }
	FogEntity* find_name() { return find_entity(FIND_NAME); }
	FogEntity* find_scope() { return find_entity(FIND_SCOPE); }
	FogEntity* find_type() { return find_entity(FIND_TYPE); }
	virtual const PrimId& global_id() const = 0;
	virtual PrimIdHandle global_signature_id() const = 0;
	virtual const PrimId& local_id() const = 0;
	virtual PrimIdHandle local_signature_id() const = 0;
	virtual const PrimId& long_id() const = 0;
	virtual FogEntity* make_entity(FogMakerContext& makerContext, const PrimId& anId) = 0;
	FogEntity* make_entity(const PrimId& anId) { return make_entity(*this, anId); }
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual Resolution resolution() const;
	virtual bool resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs);
	virtual const PrimId& short_id() const = 0;
	virtual const FogSpecifier& specifier() const = 0;
	virtual const FogUtility& utility() const;
};

//
//  	Decorate the context with a maker routine on behalf of numerous callers
//
class FogEntityMakerContext : public FogMakerContext {
	typedef FogMakerContext Super;
	typedef FogEntityMakerContext This;
	TYPEDECL_SINGLE(This, Super)
public:
	typedef FogEntity* (FogScope::*MakerFunction)(FogMakerContext&);
private:
	const FogSpecifierRefToConst _specifier;   //   Specifier describing wanted declaration.
	const MakerFunction _maker;       //   Routine to make the  entity.
private:
	PrimIdHandle _short_id;        //   Untemplated name of declarator.
private:
	mutable PrimIdHandle _long_id;
private:
	FogEntityMakerContext(const This& aContext);
	This& operator=(const This& aContext);
	This& mutate() const { return *(This*)this; }
public:
	FogEntityMakerContext(FogMakeEntityContext& makeEntityContext,
	                      const FogSpecifier& aSpecifier, MakerFunction aMaker);
	virtual FogEntity* make_entity(FogMakerContext& makerContext, const PrimId& anId);
	virtual const PrimId& global_id() const;
	virtual PrimIdHandle global_signature_id() const;
	virtual const PrimId& local_id() const;
	virtual PrimIdHandle local_signature_id() const;
	virtual const PrimId& long_id() const;
	virtual const PrimId& short_id() const;
	virtual const FogSpecifier& specifier() const;
};

class FogDecoratedMakerContext : public FogMakerContext {
	typedef FogMakerContext Super;
	typedef FogDecoratedMakerContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogDecoratedMakerContext(const This& aContext);
	This& operator=(const This& aContext);
protected:
	FogDecoratedMakerContext(FogMakerContext& makerContext) : Super(makerContext) {}
	FogMakerContext& maker_context() { return (FogMakerContext&)make_entity_context(); }
	const FogMakerContext& maker_context() const { return (const FogMakerContext&)make_entity_context(); }
public:
	virtual FogEntity* make_entity(FogMakerContext& makerContext, const PrimId& anId);
	virtual const PrimId& global_id() const;
	virtual PrimIdHandle global_signature_id() const;
	virtual const PrimId& local_id() const;
	virtual PrimIdHandle local_signature_id() const;
	virtual const PrimId& long_id() const;
	virtual Resolution resolution() const;
	virtual const PrimId& short_id() const;
	virtual const FogSpecifier& specifier() const;
};

class FogFunctionMakerContext : public FogDecoratedMakerContext {
	typedef FogDecoratedMakerContext Super;
	typedef FogFunctionMakerContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const PrimIdHandle _local_signature_id;   //   Unscoped, name with template arguments and parameter lists.
	const PrimIdHandle _global_signature_id;  //   Scoped version of _local_signature_id.
private:
	FogFunctionMakerContext(const This& aContext);
	This& operator=(const This& aContext);
public:
	FogFunctionMakerContext(FogMakerContext& makerContext,
	                        const PrimId& localSignatureId, const PrimId& globalSignatureId);
	virtual PrimIdHandle global_signature_id() const;
	virtual PrimIdHandle local_signature_id() const;
};

//
//  	Decorate the context with a scope nesting on behalf of
//  		FogNestedName::make_entity(FogMakerContext& makerContext)
//
class FogNestedMakerContext : public FogDecoratedMakerContext {
	typedef FogDecoratedMakerContext Super;
	typedef FogNestedMakerContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogToken& _dynamic_token;       //   Current token.
private:
	mutable PrimIdHandle _global_id;
	mutable PrimIdHandle _long_id;
private:
	FogNestedMakerContext(const This& makeEntityContext);
	This& operator=(const This& makeEntityContext);
	This& mutate() const { return *(This*)this; }
public:
	FogNestedMakerContext(FogMakerContext& makerContext, FogToken& inToken);
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const PrimId& global_id() const;
	virtual PrimIdHandle global_signature_id() const;
	virtual InScope in_scope() const;
	virtual const PrimId& long_id() const;
};

//
//  	A FogTemplatedScopeContext decorates a FogScopeContext to establish template arguments.
//
class FogTemplatedMakerContext : public FogDecoratedMakerContext {
	typedef FogDecoratedMakerContext Super;
	typedef FogTemplatedMakerContext This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogTemplatedName& _templated_name;    //   Raw template argument expressions.
private:
	FogTemplatedMakerContext(const This& scopeContext);
	This& operator=(const This& scopeContext);
public:
	FogTemplatedMakerContext(FogMakerContext& makerContext, const FogTemplatedName& templatedName);
	virtual void find_entities(FogEntityFinder& theFinder);
	virtual std::ostream& print_resolution(std::ostream& s) const;
	virtual bool resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs);
};
#endif

