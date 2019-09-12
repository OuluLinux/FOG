
#ifndef FOGSTRING_HXX
#define FOGSTRING_HXX




class FogString : public FogExpr
{
 typedef FogExpr Super;
 typedef FogString This;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
 FOGTOKEN_SHAREDLEAF_DECLS
public:
 enum StringType
 {
  NARROW,         //   e.g. \011 from "\011"
  WIDE,         //   e.g. \011 from L"\011"
  LITERAL         //   characters already translated (used as # encoding).
 };
private:
 enum SizeofStringType { SIZEOF_STRINGTYPE = LITERAL+1 };
private:
 const PrimIdHandle _raw_id;     //   The raw text, such as \ u 4 4 4 5 h \ 1 2 \ u 4 6 4 7
 StringType _string_type;
 mutable PrimIdHandle _bin_id;    //   The binary formatted such D E \0 h \0 \12 F G
 mutable PrimIdHandle _put_id;    //   The ASCII formatted reformatting \ u 4 4 4 5 h \ n \ u 4 6 4 7
private:
 FogString(const This&);
 This& operator=(const This&);
private:
 const PrimId& id() const { return *_raw_id; }
 friend struct PrimMapOfRefToConstFunctions<FogString>;
private:
 FogString(const PrimId& rawId, const PrimId& binId, StringType stringType);
 This& mutate() const { return *(This *)this; }
 const PrimId& put_id() const;
 const PrimId& raw_id() const { return *_raw_id; }
protected:
 FogString();
 virtual ~FogString();
public:
 const PrimId& bin_id() const;
 virtual bool emit(FogEmitContext& emitContext) const;
 virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
 virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
//  	virtual const PrimNumber& get_number(const FogScopeContext& inScope) const;
 virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
 virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
 virtual const FogString *is_string() const;
 virtual bool is_wide() const;
 virtual const FogMetaType& meta_type() const;
 virtual FogTokenType::TokenType pp_token_type_enum() const;
 virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
 virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const;
 virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_stringized(std::ostream& s) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
 virtual size_t source_columns() const;
 virtual FogTokenType::TokenType token_type_enum() const;
public:
 static const FogString& get(const char *& p);
 static FogString& make(const PrimId& rawId, StringType stringType);
 static FogString& make(const PrimId& rawId, const PrimId& binId, StringType stringType);
 static FogString& make(const FogString& firstString, const FogString& secondString);
 static FogString& make(const char *aBuffer, size_t aLength, StringType stringType);
 friend void operator==(const FogString& firstNumber, const FogString& secondNumber);
//  		{ return &firstNumber == &secondNumber; }
 friend std::ostream& operator<<(std::ostream& s, const FogString& aString) { return aString.print_this(s); }
};
#endif
 
