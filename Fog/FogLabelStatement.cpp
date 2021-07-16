#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogLabelStatement, Super)
FOGTOKEN_LEAF_IMPL(FogLabelStatement)

FogLabelStatement::FogLabelStatement(FogName& aLabel, FogRaw& aStatement)
:
 _label(aLabel),
 _statement(aStatement)
{}

FogLabelStatement::FogLabelStatement(const This& aStatement)
:
 Super(aStatement),
 _label(aStatement._label),
 _statement(aStatement._statement)
{}

FogLabelStatement::~FogLabelStatement() {}

void FogLabelStatement::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 if (_label)
  _label->create_usage(aContext);
 _statement->create_usage(aContext);
}

bool FogLabelStatement::emit(FogEmitContext& emitContext) const
{
 emitContext.emit_hash_line(line());
 _label->emit(emitContext);
 emitContext.emit_space_and_text(":\n");
 FogIndentEmitContext indentedContext(emitContext, 1, FogEmitContext::INDENTS);
 _statement->emit(indentedContext);
 return true;
}

size_t FogLabelStatement::executable_tokens() const { return _statement->executable_tokens(); }

void FogLabelStatement::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 _statement->install(installContext);
}

bool FogLabelStatement::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext)
  && (!_label || _label->is_actual(scopeContext))
  && _statement->is_actual(scopeContext);
}

void FogLabelStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 if (_label)
  _label->make_actual(_label.to_const(), makeActualContext);
 _statement->make_actual(_statement.to_const(), makeActualContext);
}

void FogLabelStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
 Super::merge_from(mergeContext, thatExpr);
 thatExpr._label->merge_into(mergeContext, _label.to_const());
 thatExpr._statement->merge_into(mergeContext, _statement.to_const());
}

const FogMerge& FogLabelStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
 needsMerge |= _label->needs_merge(mergeContext, *thatExpr._label);
 needsMerge |= _statement->needs_merge(mergeContext, *thatExpr._statement);
 return needsMerge;
}

std::ostream& FogLabelStatement::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 _label->print_deep(s, aDepth);
 _statement->print_deep(s, aDepth);
 return s;
}

std::ostream& FogLabelStatement::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 _label->print_on(s, aDepth);
 _statement->print_on(s, aDepth);
 return s;
}

char FogLabelStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
 tailChar = _label->print_named(s, 0, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, ": ");
 tailChar = _statement->print_named(s, 0, tailChar);
 return tailChar;
}

void FogLabelStatement::set_is_meta()
{
    ERRMSG("Meta-label-statements are not supported.");
}
