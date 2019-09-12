#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogVariableSpecifier, Super)
FOGTOKEN_LEAF_IMPL(FogVariableSpecifier)

FogVariableSpecifier::FogVariableSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(makeSpecifierContext, nameName)
{}

//  
//    This constructor used for anaonlymous variables whose type is elkaborated. e.g class X : 3;
//  
FogVariableSpecifier::FogVariableSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogScopeSpecifier& typeName)
:
    Super(makeSpecifierContext, make_anon())
{
    set_type(typeName);
}

FogVariableSpecifier::~FogVariableSpecifier() {}

bool FogVariableSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!decl_specifiers().is_explicit_access())
        set_decl_specifier(makeSpecifierContext.access());
    if (_field_width)                                   //   If a bit-field
    {
        if (type().is_null())                           //   If anon bit-field
        {
            name().make_type_specifier(makeSpecifierContext);
            name_ref() = make_anon();
        }
    }
    else
    {                                                   //   Establish a unique name so that
        FogAnonId *anonId = type().is_anon();           //        enum {} c;
        if (anonId)                                     //   can be emitted as
            anonId->set_concrete();                     //        enum _anon_57 {};
    }                                                   //        _anon_57 c;
    return Super::compile_specifier(makeSpecifierContext);
}

void FogVariableSpecifier::create_usage(FogUsageContext& usageContext) const
{
    Super::create_usage(usageContext);
    _field_width->create_usage(usageContext);
    _initializer->create_usage(usageContext);
}

bool FogVariableSpecifier::emit(FogEmitContext& emitContext) const
{
    Super::emit(emitContext);
//    if (_field_width)                               -- in emit_interface
//    {
//        emitContext.emit_space_and_text(":");
//        _field_width->emit(emitContext);
//    }
//    if (_initializer)                               -- in emit_implementation
//    {
//        emitContext.emit_space_and_text("=");
//        _initializer->emit(emitContext);
//    }
    return true;
}

void FogVariableSpecifier::emit_implementation(FogEmitContext& emitContext) const
{
    bool isNameSpace = emitContext.dynamic_scope().is_name_space();
    const FogDeclSpecifierValue& declSpecifiers = decl_specifiers();
    if (declSpecifiers.is_extern())
        emitContext.emit_space_and_text("extern");
    if (declSpecifiers.is_static() && isNameSpace)
        emitContext.emit_space_and_text("static");
    emit(emitContext);
    if (_initializer)
    {
        emitContext.emit_space_and_text(" = ");
//        FogEmitContext::ChangedForUseAs localContext(emitContext, FogEmitContext::RAW); // RAW until names resolved 
        FogForUseAsEmitContext localContext(emitContext, FogEmitContext::EXPRESSION); 
        FogSeparatedEmitContext separatedContext(localContext, ",", 0);
        _initializer->emit(separatedContext);
    }
    emitContext.emit_space_and_text(";\n");
}

void FogVariableSpecifier::emit_interface(FogEmitContext& emitContext) const
{
    const FogDeclSpecifierValue& declSpecifiers = decl_specifiers();
    emitContext.emit_hash_line(line());
    if (declSpecifiers.is_extern())
        emitContext.emit_space_and_text("extern");
    if (declSpecifiers.is_static())
        emitContext.emit_space_and_text("static");
    emit(emitContext);
    if (_field_width)
    {
        emitContext.emit_space_and_text(" : ");
        _field_width->emit(emitContext);
    }
    emitContext.emit_space_and_text(";\n");
}

const FogExpr *FogVariableSpecifier::initializer() const { return _initializer ? _initializer.pointer() : 0; }

void FogVariableSpecifier::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _field_width->install(installContext);
    _initializer->install(installContext);
}

void FogVariableSpecifier::instantiate()
{
    Super::instantiate();
    _initializer.reset();
}

bool FogVariableSpecifier::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _initializer->is_actual(scopeContext) && _field_width->is_actual(scopeContext);
}

FogVariableSpecifier *FogVariableSpecifier::is_variable_specifier() { return this; }

void FogVariableSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _field_width->make_actual(_field_width.to_const(), makeActualContext);
    _initializer->make_actual(_initializer.to_const(), makeActualContext);
}

FogEntity *FogVariableSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
    if (makeEntityContext.decl_specifiers().is_using())
    {
        FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_using_entity);
        return make_entity(makerContext);
    }
    else
    {
        FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_variable_entity);
        FogEntity *anEntity = make_entity(makerContext);
        FogVariable *aVariable = anEntity ? anEntity->is_variable() : 0;
        if (aVariable && makerContext.has_context())
            aVariable->add(makerContext);
        return anEntity;
    }
}

void FogVariableSpecifier::merge_from(FogMergeContext& mergeContext, const This& aSpecifier)
{
    Super::merge_from(mergeContext, aSpecifier);
    if (!_field_width)
        _field_width = aSpecifier._field_width;
    else if (aSpecifier._field_width)
        aSpecifier._field_width->merge_into(mergeContext, _field_width.to_const());
    if (!_initializer)
        _initializer = aSpecifier._initializer;
    else if (aSpecifier._initializer)
        aSpecifier._initializer->merge_into(mergeContext, _initializer.to_const());
}

const FogMetaType& FogVariableSpecifier::meta_type() const { return FogMetaType::variable_specifier_type(); }

const FogMerge& FogVariableSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatSpecifier) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatSpecifier));
    if (!_field_width)
    {
        if (thatSpecifier._field_width)
            needsMerge |= FogMerge::left_invalid();
    }
    else
    {
        if (!thatSpecifier._field_width)
            needsMerge |= FogMerge::right_invalid();
        else
            needsMerge |= _field_width->needs_merge(mergeContext, *thatSpecifier._field_width);
    }
    if (!_initializer)
    {
        if (thatSpecifier._initializer)
            needsMerge |= FogMerge::left_invalid();
    }
    else
    {
        if (!thatSpecifier._initializer)
            needsMerge |= FogMerge::right_invalid();
        else
            needsMerge |= _initializer->needs_merge(mergeContext, *thatSpecifier._initializer);
    }
    return needsMerge;
}

std::ostream& FogVariableSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_field_width)
    {
        s << indent(aDepth) << "field width\n";
        _field_width->print_deep(s, aDepth+1);
    }
    if (_initializer)
    {
        s << indent(aDepth) << "initializer\n";
        _initializer->print_deep(s, aDepth+1);
    }
    return s;
}

std::ostream& FogVariableSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_field_width)
    {
        s << indent(aDepth) << "field width\n";
        _field_width->print_on(s, aDepth+1);
    }
    if (_initializer)
    {
        s << indent(aDepth) << "initializer\n";
        _initializer->print_on(s, aDepth+1);
    }
    return s;
}

char FogVariableSpecifier::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = Super::print_named(s, fullId, tailChar);
    if (_field_width)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, " : ");
        tailChar = _field_width->print_named(s, 0, tailChar);
    }
    if (_initializer)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, " = ");
        tailChar = _initializer->print_named(s, 0, tailChar);
    }
    return tailChar;
}

std::ostream& FogVariableSpecifier::print_viz(std::ostream& s) const
{
    s << "\"variable-specifier: ";
    print_this(s);
    return s << '\"';
}

void FogVariableSpecifier::set_colon_value(FogExpr& anExpr) { _field_width = anExpr; }
void FogVariableSpecifier::set_initializer(const FogExpr& anExpr) { _initializer = anExpr; }
