#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogFailure, Super)
FOGTOKEN_SHAREDLEAF_IMPL(FogFailure)

FogFailure::FogFailure() {}
bool FogFailure::compile_parsed(FogParseContext& parseContext) { return false; }
bool FogFailure::emit(FogEmitContext& emitContext) const { return emitContext.emit_space_and_text("/*failure*/"); }
void FogFailure::find_entities(FogEntityFinder& theFinder) {}
void FogFailure::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const {}
FogEntity *FogFailure::find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const { return 0; }
const FogMetaArgument *FogFailure::find_formal_in(FogScopeContext& inScope) const { return 0; }
bool FogFailure::find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const { return false; }
bool FogFailure::get_character(PrimIdHandle& returnId, FogScopeContext& inScope)
    { returnId = PrimId::null(); return true; }

bool FogFailure::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const
{
    returnValue.assign(mutate());
    return false;
}

FogEntity *FogFailure::get_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const { return 0; }
bool FogFailure::get_identifier(PrimIdHandle& returnId, FogScopeContext&)
    { returnId = PrimId::null(); return true; }
bool FogFailure::get_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const { return false; }

bool FogFailure::get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType monadicOp) const
{
    returnValue.assign(mutate());
    return false;
}

bool FogFailure::get_number_token(FogTokenRef& returnValue, FogScopeContext&) const
{
    returnValue.assign(mutate());
    return false;
}

bool FogFailure::get_string(PrimIdHandle& returnId, FogScopeContext& inScope)
    { returnId = PrimId::null(); return true; }
FogScope& FogFailure::inner_scope() { return FogScope::mutable_null(); }
FogFailure *FogFailure::is_failure() { return this; }
const PrimId *FogFailure::is_resolved() const { return 0; }

FogName& FogFailure::make()
{
    static const FogNameRef theFailure(new FogFailure, FogNameRef::ADOPT);
    return *theFailure;
}

void FogFailure::make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId) { aName = *this; }
bool FogFailure::make_ctor_init(FogRawCtorInitRef& ctorInit) { return false; }
FogEntity *FogFailure::make_entity(FogMakerContext& makerContext) const { return 0; }
FogEntity *FogFailure::make_name_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }
bool FogFailure::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) { return false; }
FogEntity *FogFailure::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }
bool FogFailure::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) { return false; }
bool FogFailure::make_typed_expression(FogExprRef& theExpr, FogName& theType) { return false; }
const FogMetaType& FogFailure::meta_type() const { return FogMetaType::void_type(); }

bool FogFailure::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
    FogScopeContext& inScope) const
{
    returnValue = mutate();
    return false;
}

char FogFailure::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
    if (scopeId)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
        tailChar = FogStream::space_and_emit(s, tailChar, "::");        
    }
    return FogStream::space_and_emit(s, tailChar, "<|failure|>");
}

bool FogFailure::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const
{
    return false;
}

bool FogFailure::resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const
    { returnId = PrimId::null(); return true; }
bool FogFailure::resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext,
    const FogToken& objectValue) const { return false; }
void FogFailure::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {}
void FogFailure::set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers) { aName = *this; }
void FogFailure::set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers) { aRaw = *this; }
void FogFailure::set_destructor_expression(FogExprRef& anExpr) {}
void FogFailure::set_destructor_name(FogNameRef& aName) { aName = *this; }
FogToken::NameSort FogFailure::sort_class() const { return INVALID_NAME; }
size_t FogFailure::source_columns() const { return 0; }
const FogTag& FogFailure::tag() const { return FogTag::invalid_tag(); }
