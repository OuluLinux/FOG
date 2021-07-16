#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogDestructorId, Super)
FOGTOKEN_LEAF_IMPL(FogDestructorId)

FogDestructorId::FogDestructorId(FogName& aName)
:
 _name(aName)
{
 if (aName.is_actual(FogScopeContext::weak()))
  resolve_id(_resolved_id, FogScopeContext::weak());
}

FogDestructorId::FogDestructorId(const This& anId)
:
 Super(anId),
 _name(anId._name),
 _resolved_id(anId._resolved_id)
{}

FogDestructorId::~FogDestructorId() {}
void FogDestructorId::create_usage(FogUsageContext& usageContext) const { Super::create_usage(usageContext); }
bool FogDestructorId::emit(FogEmitContext& emitContext) const { return Super::emit(emitContext); }
size_t FogDestructorId::executable_tokens() const { return _name->executable_tokens() + 1; }
void FogDestructorId::install(FogInstallContext& installContext) const { Super::install(installContext); }

bool FogDestructorId::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _name->is_actual(scopeContext);
}

FogDestructorId *FogDestructorId::is_destructor_id() { return this; }
const PrimId *FogDestructorId::is_resolved() const { return _resolved_id ? _resolved_id.pointer() : 0; }

void FogDestructorId::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 _name->make_actual(_name.to_const(), makeActualContext);
}

void FogDestructorId::merge_from(FogMergeContext& mergeContext, const This& thatId)
{
 Super::merge_from(mergeContext, thatId);
 thatId._name->merge_into(mergeContext, _name.to_const());
}

const FogMerge& FogDestructorId::needs_merge_from(FogMergeContext& mergeContext, const This& thatId) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatId));
 needsMerge |= _name->needs_merge(mergeContext, *thatId._name);
 return needsMerge;
}
std::ostream& FogDestructorId::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 if (_name)
  _name->print_deep(s, aDepth);
 return s;
}

std::ostream& FogDestructorId::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 if (_name)
  _name->print_on(s, aDepth);
 return s;
}

char FogDestructorId::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
 if (scopeId)
 {
  tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
  tailChar = FogStream::space_and_emit(s, tailChar, "::");  
 }
 if (_resolved_id)
  return FogStream::space_and_emit(s, tailChar, _resolved_id.str());
 tailChar = FogStream::space_and_emit(s, tailChar, "~");
 return Super::print_named(s, 0, tailChar);
}

std::ostream& FogDestructorId::print_viz(std::ostream& s) const
{
 s << "\"destructor-id: ";
 print_this(s);
 return s << '\"';
}

bool FogDestructorId::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
{
 if (_resolved_id) 
  returnId = _resolved_id;
 else
 {
  FogScope& aScope =  inScope.dynamic_scope();
  FogStream s(aScope);
  FogStaticEmitContext emitContext(s, aScope, FogEmitContext::DESTRUCTOR_ID);
  emitContext.emit_space_and_text("~");
  _name->emit(emitContext);
  size_t aSize = s.pcount();
  returnId = PrimIdHandle(s.str(), aSize);
 }
 return true;
}

bool FogDestructorId::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext nameSemantics(theSemantics);
 if (!_name->resolve_semantics(nameSemantics))
  return false;
 theSemantics.reset();
 if (nameSemantics.is_class_name())
  theSemantics.set(FogSemantics::IS_DESTRUCTOR);
 if (nameSemantics.is_type_name())
  theSemantics.set(FogSemantics::IS_PSEUDO_DESTRUCTOR_ID);
 if (nameSemantics.is_meta_type_name())
  theSemantics.set(FogSemantics::IS_META_DTOR_NAME);
 return true;
}

void FogDestructorId::set_destructor_name(FogNameRef& aName)
{
 ERRMSG("Cannot interpret ~ " << viz(*this) << " as the name of a destructor.");
 aName = FogFailure::make();
}

FogToken::NameSort FogDestructorId::sort_class() const { return DESTRUCTOR_NAME; }
