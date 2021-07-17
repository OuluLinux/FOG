
#ifndef FOGMAKESEMANTICS_HXX
#define FOGMAKESEMANTICS_HXX




#define FOGMAKESEMANTICS(Name, Text) Name

#define FOGMAKESEMANTICS_DECLARATIONS(Macro) \
	Macro(MAKE_INVALID, "unclassifiable declaration"), \
	Macro(MAKE_FRIEND_FUNCTION, "declaration of a friend function"), \
	Macro(MAKE_FRIEND_TYPE, "declaration of a friend class"), \
	Macro(MAKE_FUNCTION, "declaration or definition of a function"), \
	Macro(MAKE_FUNCTION_BODY, "definition of a function-body"), \
	Macro(MAKE_META_CONSTRUCTOR, "declaration of a meta-constructor"), \
	Macro(MAKE_META_DESTRUCTOR, "declaration of a meta-destructor"), \
	Macro(MAKE_META_FUNCTION, "declaration of a meta-function"), \
	Macro(MAKE_META_PARAMETER, "declaration of meta-parameter"), \
	Macro(MAKE_META_TYPE, "declaration of meta-type"), \
	Macro(MAKE_META_VARIABLE, "declaration of a meta-variable"), \
	Macro(MAKE_PARAMETER, "declaration of a parameter"), \
	Macro(MAKE_SUBSPACE, "declaration in a subspace"), \
	Macro(MAKE_TEMPLATED_PARAMETER, "declaration of a templated type template parameter"), \
	Macro(MAKE_TYPE, "declaration of a type"), \
	Macro(MAKE_TYPE_PARAMETER, "declaration of a type template parameter"), \
	Macro(MAKE_TYPEDEF, "declaration of a typedef"), \
	Macro(MAKE_USING, "declaration of a using declaration"), \
	Macro(MAKE_VALUE_PARAMETER, "declaration of a non-type template parameter"), \
	Macro(MAKE_VARIABLE, "declaration of a variable")

class FogMakeSemantics : public PrimPrint {
	typedef FogMakeSemantics This;
	typedef PrimPrint Super;
	TYPEDECL_SINGLE(This, Super)
public:
	enum Semantics { FOGMAKESEMANTICS_DECLARATIONS(FOGMAKESEMANTICS) };
	enum Context { DECLARATION, DECLARATION_LIST, META_DECLARATION, SUBSPACE, TEMPLATE_DECLARATION };
private:
	const Semantics _semantics;
public:
	explicit FogMakeSemantics(Semantics theSemantics) : _semantics(theSemantics) {}
	FogMakeSemantics(const FogSemanticsContext& theSemantics, const FogDeclSpecifierValue& declSpecifiers,
	                 Context aContext);
	//    FogMakeSemantics(const FogMakeSemantics& aSemantics);               // Default ok.
	bool is_meta() const { return (MAKE_META_CONSTRUCTOR <= _semantics) && (_semantics <= MAKE_META_VARIABLE); }
	bool is_meta_constructor() const { return _semantics == MAKE_META_CONSTRUCTOR; }
	bool is_meta_destructor() const { return _semantics == MAKE_META_DESTRUCTOR; }
	bool is_parameter() const { return _semantics == MAKE_PARAMETER; }
	bool make_specifier(FogBaseMakeSpecifierContext& makeSpecifierContext, FogRaw& aSpecifier) const;
	virtual std::ostream& print_this(std::ostream& s) const;
private:
	friend void operator==(const FogMakeSemantics& firstSemantics, const FogMakeSemantics& secondSemantics);
	static Semantics semantics(const FogSemantics& theSemantics, const FogDeclSpecifierValue& declSpecifiers,
	                           Context aContext, bool isDiagnosed);
};
#endif

