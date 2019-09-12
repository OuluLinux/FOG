#include <Fog/FogIncludeAll.h>















class FogTemplateTypeArg : public FogTemplateArg
{
    typedef FogTemplateArg Super;
    typedef FogTemplateTypeArg This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_LEAF_DECLS
public:
    FogTemplateTypeArg(FogExpr& aType);
    virtual bool matches(FogMergeContext& mergeContext, const FogTemplateParameterSpecifier& templateParameter) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
};

class FogTemplateValueArg : public FogTemplateArg
{
    typedef FogTemplateArg Super;
    typedef FogTemplateValueArg This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_LEAF_DECLS
public:
    FogTemplateValueArg(FogExpr& anExpr);
    virtual bool matches(FogMergeContext& mergeContext, const FogTemplateParameterSpecifier& templateParameter) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
};

TYPEINFO_SINGLE(FogTemplateArg, Super)
TYPEINFO_SINGLE(FogTemplateTypeArg, Super)
TYPEINFO_SINGLE(FogTemplateValueArg, Super)
PRIMREF_NULL_CLAS(FogTemplateArg)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogTemplateArg)
PRIMREFS_IMPL(FogTemplateArg)
PRIMLISTOFREFS_IMPL(FogTemplateArg)
TMPL_HACK_FIX_DO(FogTemplateArg)

FOGTOKEN_LEAF_IMPL(FogTemplateTypeArg)
//TMPL_HACK_FIX_DO(FogTemplateTypeArg)

FOGTOKEN_LEAF_IMPL(FogTemplateValueArg)
//TMPL_HACK_FIX_DO(FogTemplateValueArg)

bool FogTemplateArg::emit(FogEmitContext& emitContext) const
    { return expr().emit(emitContext); }
size_t FogTemplateArg::executable_tokens() const { return 1; }
bool FogTemplateArg::matches(FogMergeContext& mergeContext,
    const FogTemplateParameterSpecifier& templateParameter) const { return false; }
const FogMetaType& FogTemplateArg::meta_type() const { return FogMetaType::template_argument_type(); }

FogTemplateArg *FogTemplateArg::new_template_type_argument(FogExpr& anExpr) { return new FogTemplateTypeArg(anExpr); }
FogTemplateArg *FogTemplateArg::new_template_value_argument(FogExpr& anExpr)
    { return new FogTemplateValueArg(anExpr); }
std::ostream& FogTemplateArg::print_this(std::ostream& s) const { return expr().print_this(s); }

std::ostream& FogTemplateArg::print_viz(std::ostream& s) const
{
    s << "\"template-arg: ";
    print_this(s);
    return s << '\"';
}
 
FogTemplateTypeArg::FogTemplateTypeArg(FogExpr& aType)
:
    Super(aType)
{}

#if 0
void FogTemplateTypeArg::create_usage(FogUsageContext& usageContext) const
{
    if (usageContext, usageContext.of_use_by().is_of_name())
        _type->create_usage(usageContext);
    else                                        //   Use of tail must convert to head to support
    {                                           //    incomplete types (at least within typedef's)
//        FogUsageContext::ChangedOfUseBy changedContext(usageContext, FogOfUseBy::of_head_by_tail());
        //   The actual usage of a template type depends on the context of the external and internal use.
        //   The internal use can only be determined by template instantiation.
        //  .bugbug in the absence of instantiation, assume usage of name only, since this avoids creating
        //   bogus dependency cycles that could arise from stronger dependency. Application code must
        //   provide use declarations to strengthen the dependency as required.
//        FogUsageContext::ChangedOfUseBy changedContext(usageContext, usageContext.of_use_by().of_name());
        FogChangedUsageContext changedContext(usageContext, usageContext.of_use_by().of_name());
        _type->create_usage(changedContext);
    }
}
#endif

bool FogTemplateTypeArg::matches(FogMergeContext& mergeContext, const FogTemplateParameterSpecifier& templateParameter) const
{
    const FogTypeParameterSpecifier *typeParameter = templateParameter.is_type_parameter_specifier();
    if (!typeParameter)
        return false;
//    const FogTypeDecl *argType = 0; //.bugbug _type->find_scalar_type_decl();
    const FogName *argTypeName = 0; //  .bugbug argType ? argType->find_unqualified_name() : 0;
    if (!argTypeName || !argTypeName->is_resolved())
        return false;
    const PrimId& parameterId = typeParameter->short_id();
    const PrimId *argumentId = argTypeName->is_resolved();
    return &parameterId == argumentId;
}

std::ostream& FogTemplateTypeArg::print_viz(std::ostream& s) const
{
    s << "\"template-type-argument: ";
    print_this(s);
    return s << '\"';
}
 
FogTemplateValueArg::FogTemplateValueArg(FogExpr& anExpr)
:
    Super(anExpr)
{}

bool FogTemplateValueArg::matches(FogMergeContext& mergeContext,
                                  const FogTemplateParameterSpecifier& templateParameter) const
{           //  .bugbug Check this out.
    const FogTypeParameterSpecifier *typeParameter = templateParameter.is_type_parameter_specifier();
    if (!typeParameter)
        return false;
    const FogName *aName = expr().is_name();
    if (!aName)
        return false;
    const PrimId& parameterId = typeParameter->short_id();
    PrimIdHandle argumentId;
    if (!aName->resolve_id(argumentId, mergeContext))
        return false;
    return parameterId == argumentId;
}

std::ostream& FogTemplateValueArg::print_viz(std::ostream& s) const
{
    s << "\"template-value-argument: ";
    print_this(s);
    return s << '\"';
}