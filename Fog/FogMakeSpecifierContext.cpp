#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogMakeSpecifierContext, Super)
TYPEINFO_SINGLE(FogBaseMakeSpecifierContext, Super)
TYPEINFO_SINGLE(FogDecoratedMakeSpecifierContext, Super)
TYPEINFO_SINGLE(FogMetaMakeSpecifierContext, Super)
TYPEINFO_SINGLE(FogTemplatedMakeSpecifierContext, Super)

bool FogMakeSpecifierContext::compile_specifier()
{
    FogSpecifier& aSpecifier = specifier();
    if (aSpecifier.is_null())
        return true;
    return aSpecifier.compile_specifier(*this);
}

bool FogMakeSpecifierContext::compile_type_specifier()
{
    FogSpecifier& aSpecifier = specifier();
    if (aSpecifier.is_null())
        return true;
    FogObjectSpecifier *objectSpecifier = aSpecifier.is_object_specifier();
    if (objectSpecifier)
    {
        FogName& typeName = objectSpecifier->type();
        FogSpecifier *typeSpecifier = typeName.is_specifier();
        if (typeSpecifier && (typeSpecifier != objectSpecifier))
            typeSpecifier->compile_specifier(*this);            
    }
    return true;
}

const FogAccess& FogMakeSpecifierContext::access() const { return parse_context().access(); }
const FogDeclSpecifierValue& FogMakeSpecifierContext::decl_specifiers() const
    { return base_make_specifier_context().decl_specifiers(); }
const FogRaw& FogMakeSpecifierContext::declaration() const { return parse_context().declaration(); }
FogFunctionSpecifier *FogMakeSpecifierContext::get_function_specifier(const FogToken& inToken)
    { return specifier().get_function_specifier(inToken); }
FogMetaFunctionSpecifier *FogMakeSpecifierContext::get_meta_function_specifier(const FogToken& inToken)
    { return specifier().get_meta_function_specifier(inToken); }
FogMetaParameterSpecifier *FogMakeSpecifierContext::get_meta_parameter_specifier(const FogToken& inToken)
    { return specifier().get_meta_parameter_specifier(inToken); }
FogObjectSpecifier *FogMakeSpecifierContext::get_object_specifier(const FogToken& inToken)
    { return specifier().get_object_specifier(inToken); }
FogParameterSpecifier *FogMakeSpecifierContext::get_parameter_specifier(const FogToken& inToken)
    { return specifier().get_parameter_specifier(inToken); }
FogTemplateParameterSpecifier *FogMakeSpecifierContext::get_template_parameter_specifier(const FogToken& inToken)
    { return specifier().get_template_parameter_specifier(inToken); }
bool FogMakeSpecifierContext::is_nested() const { return base_make_specifier_context().is_nested(); }
FogLine& FogMakeSpecifierContext::line() const { return parse_context().declaration().line(); }
bool FogMakeSpecifierContext::make_specifier(FogRaw& aSpecifier)
    { return base_make_specifier_context().make_specifier(aSpecifier); }

FogParseContext& FogMakeSpecifierContext::parse_context()
{
    FogBaseMakeSpecifierContext& baseMakeSpecifierContext = base_make_specifier_context();
    return baseMakeSpecifierContext.parse_context();
}

std::ostream& FogMakeSpecifierContext::print_viz(std::ostream& s) const
{
    if (!specifier().is_null())
        s << viz(specifier()) << " in " << viz(dynamic_token());
    else
        s << viz(declaration()) << " in " << viz(dynamic_token());
    return s;
}

FogScopeContext::Resolution FogMakeSpecifierContext::resolution() const { return RESOLVE_DOLLARS; }

//  
//    Resolve the template arguments within primaryEntity and assign to templateArgs, returning false if not possible
//    as is the case for resolution of a function name whose templae arguments must be deduced.
//  
bool FogMakeSpecifierContext::resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs)
{
 return false;
}

const FogMakeSemantics& FogMakeSpecifierContext::semantics() const
    { return base_make_specifier_context().semantics(); }
void FogMakeSpecifierContext::set_nested_scope(FogToken& inToken)
    { base_make_specifier_context().set_nested_scope(inToken); }
FogSpecifier& FogMakeSpecifierContext::specifier() { return base_make_specifier_context().specifier(); }
FogTargetFile *FogMakeSpecifierContext::target_file() const { return parse_context().target_file(); }

const FogUtility& FogMakeSpecifierContext::utility() const
{
    const FogParseContext& parseContext = parse_context();
    return parseContext.utility();
}
 
//  
//    Construct a specifier context from FogExpr, FogListOfExpr, FogRawTemplate::compile_parsed.
//  
//    FogMakeSemantics::DECLARATION           FogDerivable::make_used
//    FogMakeSemantics::META_DECLARATION      FogExpr::compile_parsed
//    FogMakeSemantics::DECLARATION           FogExpr::compile_parsed
//    FogMakeSemantics::DECLARATION_LIST      FogListOfExpr::compile_parsed
//    FogMakeSemantics::SUBSPACE              FogObjectSpecifier::compile_subspace
//    FogMakeSemantics::TEMPLATE_DECLARATION  FogRawTemplate::compile_parsed
//  
FogBaseMakeSpecifierContext::FogBaseMakeSpecifierContext(FogParseContext& parseContext,
    const FogSemanticsContext& theSemantics, const FogDeclSpecifierValue& declSpecifiers,
    FogMakeSemantics::Context aContext)
:
    Super(parseContext),
    _semantics(theSemantics, declSpecifiers, aContext),
    _decl_specifiers(declSpecifiers),
    _token(&parseContext.dynamic_token())
{}

//  
//    Construct a child specifier context for parameters of a function
//        (from FogListOfExpr::make_parameters and FogMetaFunctionExpr::make_specifier)
//  
//    FogMakeSemantics::MAKE_PARAMETER        FogListOfExpr::make_parameters
//    FogMakeSemantics::MAKE_META_PARAMETER   FogMetaFunctionExpr::make_specifier
//  
FogBaseMakeSpecifierContext::FogBaseMakeSpecifierContext(FogMakeSpecifierContext& makeSpecifierContext,
    const FogDeclSpecifierValue& declSpecifiers, FogMakeSemantics::Semantics makeSemantics)
:
    Super(makeSpecifierContext.parse_context()),
    _semantics(makeSemantics),
    _decl_specifiers(declSpecifiers),
    _token(&makeSpecifierContext.dynamic_token())
{}

FogBaseMakeSpecifierContext::~FogBaseMakeSpecifierContext() {}

bool FogBaseMakeSpecifierContext::adopt_specifier(FogSpecifier *aSpecifier)
{
    _specifier.adopt(aSpecifier);
    return aSpecifier != 0;
}

FogBaseMakeSpecifierContext& FogBaseMakeSpecifierContext::base_make_specifier_context() { return *this; }
FogToken& FogBaseMakeSpecifierContext::dynamic_token() { return *_token; }
const FogToken& FogBaseMakeSpecifierContext::dynamic_token() const { return *_token; }
bool FogBaseMakeSpecifierContext::find_slots(FogMetaSlotFinder& theFinder) { return _token->find_slots(theFinder); }

bool FogBaseMakeSpecifierContext::is_nested() const
{
    bool isNested = _token != &scope_context().static_token();
    return isNested;
}

bool FogBaseMakeSpecifierContext::make_specifier(FogRaw& aSpecifier)
{
    if (!_semantics.make_specifier(*this, aSpecifier))
        return false;
    FogLine& sourceLine = aSpecifier.line();
    if (!sourceLine.is_null())
        _specifier->set_line(sourceLine);       //  .bugbug Move via MakeSemantics::make_specifier to ctors
    return true;
}

std::ostream& FogBaseMakeSpecifierContext::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    s << indent(aDepth) << "semantics = ";
    _semantics.print_this(s);
    s << '\n';
    s << indent(aDepth) << "decl-specifiers = " << _decl_specifiers << '\n';
    return s;
}

std::ostream& FogBaseMakeSpecifierContext::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
//    s << indent(aDepth) << "semantics = ";
//    _semantics.print_this(s);
//    s << '\n';
//    s << indent(aDepth) << "decl-specifiers = " << _decl_specifiers << '\n';
    return s;
}

void FogBaseMakeSpecifierContext::set_nested_scope(FogToken& inToken) { _token = &inToken; }

const FogTemplateParameterSpecifiers *FogBaseMakeSpecifierContext::template_parameters() const
{
    const FogTemplateParameterSpecifiers *templateParameterSpecifiers = Super::template_parameters();
    FogEntity *anEntity = _token ? _token->is_entity() : 0;
    return templateParameterSpecifiers;
}

//  ---------------------------------------------------------------------------------------------------------------------

FogBaseMakeSpecifierContext& FogDecoratedMakeSpecifierContext::base_make_specifier_context()
    { return make_specifier_context().base_make_specifier_context(); }

//  ---------------------------------------------------------------------------------------------------------------------

FogMetaMakeSpecifierContext::FogMetaMakeSpecifierContext(FogMakeSpecifierContext& makeSpecifierContext)
:
    Super(makeSpecifierContext)
{}

const FogScopeContext *FogMetaMakeSpecifierContext::meta_context() const { return this; }

//  ---------------------------------------------------------------------------------------------------------------------

FogTemplatedMakeSpecifierContext::FogTemplatedMakeSpecifierContext(FogMakeSpecifierContext& makeSpecifierContext,
                                                                   const FogTemplatedName& templatedName)
:
 Super(makeSpecifierContext),
 _templated_name(templatedName)
{}

void FogTemplatedMakeSpecifierContext::find_entities(FogEntityFinder& theFinder)
{
    ERRMSG("INVESTIGATE -- use of suspected dead FogTemplatedMakeSpecifierContext::find_entities.");
//  	FogEntityFinding theFinding(theFinder.strategy());
//  	FogEntityFinder nestedFinder(theFinding, theFinder);
 Super::find_entities(theFinder);
//  	FogEntity *primaryEntity = theFinding.get_unambiguous_finding(dynamic_token(), *this);
//  	if (primaryEntity)
//  	{
//  		FogTemplateArgsRefToConst templateArgs;
//  		if (_templated_name.resolve_template(*primaryEntity, templateArgs, *this))
//  		{
//  			FogMakeTemplateContext makeContext(*this, *primaryEntity, *templateArgs, IS_DEFINITION);
//  			FogEntity *secondaryEntity = primaryEntity->find_template(makeContext);
//  			if (secondaryEntity)
//  				theFinder.add_find(*secondaryEntity);
//  		}
//  	}
}

std::ostream& FogTemplatedMakeSpecifierContext::print_resolution(std::ostream& s) const
{
 Super::print_resolution(s);
 s << " < ";
 _templated_name.exprs().print_named(s, 0, 0);
 s << " >, ";
 return s;
}

bool FogTemplatedMakeSpecifierContext::resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs)
{
 return _templated_name.resolve_template(primaryEntity, templateArgs, *this);
}

//  const FogExpr *FogTemplatedMakeSpecifierContext::template_arguments() const
//  {
//  	return &_templated_name.exprs();
//  }
