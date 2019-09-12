#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogScopeSpecifier, Super)
PRIMREF_NULL_CLAS(FogScopeSpecifier)
 virtual FogToken *clone() const { return error_clone(); }
};
PRIMREF_DERIVED_NULL_IMPL(FogScopeSpecifier)
PRIMREFS_IMPL(FogScopeSpecifier)
TMPL_HACK_FIX_DO(FogScopeSpecifier)

FogScopeSpecifier::FogScopeSpecifier()
:
 _tag(FogTag::invalid_tag())
{}

FogScopeSpecifier::FogScopeSpecifier(const FogTag& aTag, FogName& aName)
:
 Super(aName),
 _tag(aTag)
{}

FogScopeSpecifier::FogScopeSpecifier(const This &thatSpecifier)
:
 Super(thatSpecifier),
 _tag(thatSpecifier._tag)
{}

bool FogScopeSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 if (!decl_specifiers().is_explicit_access() && !decl_specifiers().is_friend())
  set_decl_specifier(makeSpecifierContext.access());
 return Super::compile_specifier(makeSpecifierContext);
}

bool FogScopeSpecifier::emit(FogEmitContext& emitContext) const
{
 emitContext.emit_space_and_text(_tag->str());
 Super::emit(emitContext);
 return true;
}

FogScopeSpecifier *FogScopeSpecifier::is_scope_specifier() { return this; }

FogEntity *FogScopeSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
 return &FogEntity::mutable_null();
}

bool FogScopeSpecifier::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 makeSpecifierContext.specifier().set_decl_specifiers(decl_specifiers());
 FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
 if (objectSpecifier)
  objectSpecifier->set_type(*this);
 return true;
}

const FogMetaType& FogScopeSpecifier::meta_type() const { return FogMetaType::scope_specifier_type(); }

char FogScopeSpecifier::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
 if (scopeId) //   ? Tacky/pragmatic way of detecting decl_specifiers() emit by caller ?
  tailChar = decl_specifiers().print_named(s, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, tag().str());
 tailChar = FogStream::space_and_emit(s, tailChar, " ");
 return Super::print_named(s, scopeId, tailChar);
}

const FogTag& FogScopeSpecifier::tag() const { return *_tag; }
