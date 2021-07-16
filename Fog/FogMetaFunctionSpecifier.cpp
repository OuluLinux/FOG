#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogMetaFunctionSpecifier, Super)
FOGTOKEN_UNIQUE_IMPL(FogMetaFunctionSpecifier)

FogMetaFunctionSpecifier::FogMetaFunctionSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(makeSpecifierContext, nameName),
    _is_constructor(makeSpecifierContext.semantics().is_meta_constructor()),
    _is_destructor(makeSpecifierContext.semantics().is_meta_destructor())
{
    if (_is_constructor)
    {
        PrimIdHandle actualId;
        const FogEntityName *actualName = name().is_entity_name();
        const FogEntity *scopeEntity = makeSpecifierContext.dynamic_token().is_entity();
        if (!scopeEntity)
            ERRMSG("BUG - expected entity as scope for " << viz(*this));
        else if (!actualName)
            ERRMSG("BUG - expected entity name for " << viz(*this));
        else if (!actualName->resolve_id(actualId, makeSpecifierContext))
            ERRMSG("BUG - failed to resolve name for " << viz(*this));
        else if (scopeEntity->short_id() != actualId)
            ERRMSG("Meta-constructor name is not the meta-class name for " << viz(*this));
        set_name(constructor_id());
        set_type(FogMetaType::token_type());
    }
    else if (_is_destructor)
    {
        PrimIdHandle actualId;
        const FogEntityName *actualName = name().is_entity_name();
        const FogDestructorId *dtorId = actualName ? actualName->is_destructor_id() : 0;
        const FogEntityName *dtorName = dtorId ? dtorId->name().is_entity_name() : 0;
        FogEntity *scopeEntity = makeSpecifierContext.dynamic_token().is_entity();
        if (!scopeEntity)
            ERRMSG("BUG - expected entity as scope for " << viz(*this));
        else if (!dtorName)
            ERRMSG("BUG - expected entity name for " << viz(*this));
        else if (!dtorName->resolve_id(actualId, makeSpecifierContext))
            ERRMSG("BUG - failed to resolve name for " << viz(*this));
        else if (scopeEntity->short_id() != actualId)
            ERRMSG("Meta-destructor name is not ~ the meta-class name for " << viz(*this));
        set_name(destructor_id());
        set_type(FogMetaType::token_type());
    }
}

FogMetaFunctionSpecifier::~FogMetaFunctionSpecifier() {}

void FogMetaFunctionSpecifier::add_parameter(FogMetaParameterSpecifier& metaParameter)
{
    _formals.add(metaParameter);
}

bool FogMetaFunctionSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    return Super::compile_specifier(makeSpecifierContext);
}

FogName& FogMetaFunctionSpecifier::constructor_id()
{
    static const FogKeywordRef aKeyword(FogIdentifier::make(FogMetaSlot::constructor_id()));
    return *aKeyword;
}

FogName& FogMetaFunctionSpecifier::destructor_id()
{
    static const FogKeywordRef aKeyword(FogIdentifier::make(FogMetaSlot::destructor_id()));
    return *aKeyword;
}

FogMetaFunctionSpecifier *FogMetaFunctionSpecifier::is_meta_function_specifier() { return this; }

bool FogMetaFunctionSpecifier::make_slot(FogMakerContext& makerContext) const
{
    FogToken& inToken = makerContext.dynamic_token();
    const FogMetaType& metaType = resolve_type_meta_type(makerContext);
    FogTokenRef metaEntityValue;
    FogInScopeContext localContext(makerContext, IN_THIS_SCOPE);
    if (name().find_meta_entity_in(metaEntityValue, localContext))
    {
        const FogMetaFunctionEntity *metaFunctionEntity = metaEntityValue->is_meta_function_entity();
        if (!metaFunctionEntity)
            ERRMSG("Cannot create meta-function when " << viz(*metaFunctionEntity) << " already defined.");
//          else if (&type() != &metaFunctionEntity->type())
        else if (&metaType != &metaFunctionEntity->type())
            ERRMSG("Cannot change type of " << viz(*metaFunctionEntity) << " to " << type());
//        else if (metaFunction->is_exclusive() || is_exclusive())
//            ERRMSG("Cannot compose exclusive meta-function " << viz(*this));
//        else if (!FogMetaArgument::matches(metaFunction->formals(), dollarContext.formals()))
//            ERRMSG("Must have consistent argument types and names for " << viz(*this));
        else
        {
            FogTokenStatementsRef newList(new FogTokenStatements(metaType), FogTokenStatementsRef::ADOPT);
//            FogConstTokensRef newList(new FogConstTokens, FogConstTokensRef::ADOPT);
//            if (_position->is_append())
//  //                newList->add_list(metaFunction->tokens());
//                newList->add(metaFunction->token());
//  //            newList->add_list(*_tokens);
//            newList->add(*_tokens);
//  //            newList->add(value());
//            if (_position->is_prepend())
//  //                newList->add_list(metaFunction->tokens());
//                newList->add(metaFunction->token());
            FogMetaFunction *metaFunction = new FogMetaFunction(makerContext,
                get_static(), metaType, is_exposed(), _formals, *newList);
            return inToken.adopt_slot(metaFunction) != 0;
        }
        return true;
    }
    else
    {
        FogMetaFunction *metaFunction = new FogMetaFunction(makerContext,
            get_static(), metaType, is_exposed(), _formals, *_body);
        return inToken.adopt_slot(metaFunction) != 0;
    }
}

const FogMetaType& FogMetaFunctionSpecifier::meta_type() const { return FogMetaType::meta_function_specifier_type(); }

std::ostream& FogMetaFunctionSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _formals.print_deep(s, aDepth);
    return s;
}

std::ostream& FogMetaFunctionSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _formals.print_on(s, aDepth);
    return s;
}

std::ostream& FogMetaFunctionSpecifier::print_viz(std::ostream& s) const
{
    s << "\"meta-function-specifier: ";
    print_this(s);
    return s << '\"';
}

void FogMetaFunctionSpecifier::set_body(const FogTokenStatements& theBody)
{
    _body = theBody;
}
