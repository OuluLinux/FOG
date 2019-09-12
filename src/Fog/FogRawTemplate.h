
#ifndef FOGRAWTEMPLATE_HXX
#define FOGRAWTEMPLATE_HXX




class FogRawTemplate : public FogRaw
{
 typedef FogRaw Super;
 typedef FogRawTemplate This;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
 FOGTOKEN_DERIVED_DECLS
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 IsExportTemplate _is_export;     //   True if export keyword used.
 IsUsingTemplate _is_using;      //   True if using keyword used.
 FogRawRef _declaration;       //   The templated declaration.
 FogListOfExprRef _parameters;     //   Template parameters.
private:
//  	FogRawTemplate(const This& rawTemplate);
 This& operator=(const This& rawTemplate);
protected:
 FogRawTemplate();
 virtual ~FogRawTemplate();
public:
 FogRawTemplate(FogListOfExpr& parameterList, FogRaw& aDeclaration);
 virtual bool compile_parsed(FogParseContext& parseContext);
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
 virtual void set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers);
 virtual void set_explicit_instantiation();
 virtual void set_line(FogLine& aLine);
};
#endif
 
