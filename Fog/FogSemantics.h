
#ifndef FOGSEMANTICS_HXX
#define FOGSEMANTICS_HXX




class FogSemantics_Missing {
public:
	bool missing(const char* metaName) const;
public:
	static const FogSemantics& missing_value(const char* metaName);
};

#define FOG_SEMANTICS_BIT_TO_MASK(Name) name2(IS_,Name) = 1 << name2(Name,_BIT)
#define FOG_SEMANTICS_BIT(Name) name2(Name,_BIT)
#define FOG_SEMANTICS_MISSING(Name) bool Name() const { return missing(#Name); }
#define FOG_SEMANTICS_MISSING_VALUE(Name) static const FogSemantics& Name() { return missing_value(#Name); }

#define FOG_SEMANTICS_DECLARATIONS(Macro) \
	Macro(BIT_FIELD_DECLARATION), \
	Macro(BIT_FIELD_DECLARATION_ELEM), \
	Macro(COMPOUND_DECLARATION), \
	Macro(CONSTRUCTOR_DECLARATIVE), \
	Macro(CONSTRUCTOR_DECLARATOR), \
	Macro(CONSTRUCTOR_TYPE_ID), \
	Macro(DECLARATION), \
	Macro(DESTRUCTOR_DECLARATIVE), \
	Macro(DESTRUCTOR_DECLARATOR), \
	Macro(DESTRUCTOR_TYPE_ID), \
	Macro(FILE_DEPENDENCY_SPECIFIER), \
	Macro(FILE_PLACEMENT_SPECIFIER), \
	Macro(FUNCTION_DECLARATOR), \
	Macro(FUNCTION_DECLARATIVE), \
	Macro(FUNCTION_DEFINITION), \
	Macro(FUNCTION_TYPE_ID), \
	Macro(INIT_DECLARATION_LIST), \
	Macro(OBJECT_DECLARATION), \
	Macro(OBJECT_TYPE_ID), \
	Macro(PARAMETER_DECLARATION_CLAUSE), \
	Macro(TYPEDEF_DECLARATION), \
	Macro(TYPE_ID_LIST), \
	Macro(USING_DECLARATION), \
	Macro(USING_DECLARATIVE)

class FogSemantics_Declaration : private FogSemantics_Missing {
private:
	enum DeclarationBit { FOG_SEMANTICS_DECLARATIONS(FOG_SEMANTICS_BIT) };
public:
	enum Declaration { FOG_SEMANTICS_DECLARATIONS(FOG_SEMANTICS_BIT_TO_MASK) };
private:
	enum DeclarationMasks {
		FILE_DEPENDENCY_SPECIFIER_MASK = IS_FILE_DEPENDENCY_SPECIFIER,
		FILE_PLACEMENT_SPECIFIER_MASK = IS_FILE_PLACEMENT_SPECIFIER,
		TYPEDEF_DECLARATION_MASK = IS_TYPEDEF_DECLARATION,
		USING_DECLARATIVE_MASK = IS_USING_DECLARATIVE,
		USING_DECLARATION_MASK = IS_USING_DECLARATION | USING_DECLARATIVE_MASK,
		BIT_FIELD_DECLARATION_ELEM_MASK = IS_BIT_FIELD_DECLARATION_ELEM,
		BIT_FIELD_DECLARATION_MASK = IS_BIT_FIELD_DECLARATION | BIT_FIELD_DECLARATION_ELEM_MASK,
		CONSTRUCTOR_DECLARATOR_MSK = IS_CONSTRUCTOR_DECLARATOR,  //   | CONSTRUCTOR_NAME_MASK
		CONSTRUCTOR_TYPE_ID_MSK = IS_CONSTRUCTOR_TYPE_ID | CONSTRUCTOR_DECLARATOR_MSK,  //   | CONSTRUCTOR_NAME_MASK
		CONSTRUCTOR_DECLARATIVE_MSK = IS_CONSTRUCTOR_DECLARATIVE | CONSTRUCTOR_TYPE_ID_MSK,  //   | CONSTRUCTOR_NAME_MASK
		DESTRUCTOR_DECLARATOR_MASK = IS_DESTRUCTOR_DECLARATOR,
		DESTRUCTOR_TYPE_ID_MASK = IS_DESTRUCTOR_TYPE_ID,
		DESTRUCTOR_DECLARATIVE_MASK = IS_DESTRUCTOR_DECLARATIVE | DESTRUCTOR_TYPE_ID_MASK,
		FUNCTION_DECLARATOR_MASK = IS_FUNCTION_DECLARATOR,
		FUNCTION_TYPE_ID_MASK = IS_FUNCTION_TYPE_ID | FUNCTION_DECLARATOR_MASK,
		FUNCTION_DECLARATIVE_MASK = IS_FUNCTION_DECLARATIVE | FUNCTION_TYPE_ID_MASK,
		FUNCTION_SPECIFIER_MASK = FUNCTION_DECLARATIVE_MASK,
		FUNCTION_DEFINITION_MASK = IS_FUNCTION_DEFINITION,
		FUNCTION_DECLARATION_MSK = FUNCTION_DECLARATIVE_MASK | CONSTRUCTOR_DECLARATIVE_MSK
		                           | DESTRUCTOR_DECLARATIVE_MASK | FUNCTION_DEFINITION_MASK,  //   | CONSTRUCTOR_NAME_MASK
		OBJECT_TYPE_ID_MASK = IS_OBJECT_TYPE_ID,
		OBJECT_DECLARATION_MASK = IS_OBJECT_DECLARATION | OBJECT_TYPE_ID_MASK,
		INIT_DECLARATION_MASK = OBJECT_DECLARATION_MASK | BIT_FIELD_DECLARATION_MASK,
		VARIABLE_SPECIFIER_MASK = INIT_DECLARATION_MASK,
		INIT_DECLARATION_LIST_MASK = IS_INIT_DECLARATION_LIST | INIT_DECLARATION_MASK,
		PARAMETER_DECLARATION_CLAUSE_MASK = IS_PARAMETER_DECLARATION_CLAUSE,
		DECLARATION_MSK = IS_DECLARATION | INIT_DECLARATION_LIST_MASK | IS_COMPOUND_DECLARATION
		                  | FUNCTION_DECLARATION_MSK | USING_DECLARATION_MASK
		                  | FILE_DEPENDENCY_SPECIFIER_MASK | FILE_PLACEMENT_SPECIFIER_MASK
		                  //   DECL_SPECIFIER_SEQ_MASK | META_DECLARATION_MASK | CONSTRUCTOR_NAME_MASK
	};
	
private:
	Declaration _declaration;
protected:
	FogSemantics_Declaration(Declaration declarationSemantics = Declaration(0)) : _declaration(declarationSemantics) {}
	bool operator!() const { return _declaration == Declaration(0); }
	void assign(Declaration declarationSemantics) { _declaration = declarationSemantics; }
	bool is_constructor_declarativeDeclaration() const { return (_declaration & CONSTRUCTOR_DECLARATIVE_MSK) != 0; }
	bool is_constructor_declaratorDeclaration() const { return (_declaration & CONSTRUCTOR_DECLARATOR_MSK) != 0; }
	bool is_constructor_type_idDeclaration() const { return (_declaration & CONSTRUCTOR_TYPE_ID_MSK) != 0; }
	bool is_declarationDeclaration() const { return (_declaration & DECLARATION_MSK) != 0; }
	bool is_function_declarationDeclaration() const { return (_declaration & FUNCTION_DECLARATION_MSK) != 0; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _declaration = Declaration(0); }
	void set(Declaration declarationSemantics) { _declaration = Declaration(_declaration | declarationSemantics); }
public:
	bool is_bit_field_declaration() const { return (_declaration & BIT_FIELD_DECLARATION_MASK) != 0; }
	bool is_bit_field_declaration_elem() const { return (_declaration & BIT_FIELD_DECLARATION_ELEM_MASK) != 0; }
	bool is_destructor_declarative() const { return (_declaration & DESTRUCTOR_DECLARATIVE_MASK) != 0; }
	bool is_destructor_declarator() const { return (_declaration & DESTRUCTOR_DECLARATOR_MASK) != 0; }
	bool is_destructor_type_id() const { return (_declaration & DESTRUCTOR_TYPE_ID_MASK) != 0; }
	bool is_file_dependency_specifier() const { return (_declaration & FILE_DEPENDENCY_SPECIFIER_MASK) != 0; }
	bool is_file_placement_specifier() const { return (_declaration & FILE_PLACEMENT_SPECIFIER_MASK) != 0; }
	bool is_function_declarative() const { return (_declaration & FUNCTION_DECLARATIVE_MASK) != 0; }
	bool is_function_declarator() const { return (_declaration & FUNCTION_DECLARATOR_MASK) != 0; }
	bool is_function_definition() const { return (_declaration & FUNCTION_DEFINITION_MASK) != 0; }
	bool is_function_specifier() const { return (_declaration & FUNCTION_SPECIFIER_MASK) != 0; }
	bool is_function_type_id() const { return (_declaration & FUNCTION_TYPE_ID_MASK) != 0; }
	bool is_init_declaration() const { return (_declaration & INIT_DECLARATION_MASK) != 0; }
	bool is_init_declaration_list() const { return (_declaration & INIT_DECLARATION_LIST_MASK) != 0; }
	bool is_object_declaration() const { return (_declaration & OBJECT_DECLARATION_MASK) != 0; }
	bool is_object_type_id() const { return (_declaration & OBJECT_TYPE_ID_MASK) != 0; }
	bool is_parameter_declaration_clause() const { return (_declaration & PARAMETER_DECLARATION_CLAUSE_MASK) != 0; }
	bool is_typedef_declaration() const { return (_declaration & TYPEDEF_DECLARATION_MASK) != 0; }
	bool is_using_declaration() const { return (_declaration & USING_DECLARATION_MASK) != 0; }
	bool is_using_declarative() const { return (_declaration & USING_DECLARATIVE_MASK) != 0; }
	bool is_variable_specifier() const { return (_declaration & VARIABLE_SPECIFIER_MASK) != 0; }
public:
	static const FogSemantics& file_dependency_specifier_value();
	static const FogSemantics& file_placement_specifier_value();
	static const FogSemantics& function_specifier_value();
	FOG_SEMANTICS_MISSING_VALUE(using_declaration_value)
	static const FogSemantics& variable_specifier_value();
};

#define FOG_SEMANTICS_DECLARATORS(Macro) \
	Macro(ASSIGNMENT_EXPRESSION), \
	Macro(BASIC_EXPRESSION),                            /*   logical-or-expression */ \
	      Macro(BIT_FIELD_DECLARATOR_ID_OPT), \
	      Macro(BIT_FIELD_DECLARATOR), \
	      Macro(BIT_FIELD_DECLARATOR_ELEM), \
	      Macro(CHARACTER), \
	      Macro(CONDITIONAL_EXPRESSION), \
	      Macro(CONVERSION_DECLARATOR), \
	      Macro(EPSILON), \
	      Macro(EXPRESSION), \
	      Macro(FUNCTION_DECLARATOR_ID), \
	      Macro(GLOBAL_ID_EXPRESSION), \
	      Macro(IDENTIFIER), \
	      Macro(INITIALIZER_CLAUSE), \
	      Macro(MEMBER_ID_EXPRESSION), \
	      Macro(NAME_DECL_SPECIFIER_SEQ), \
	      Macro(NUMBER), \
	      Macro(OBJECT_DECLARATOR), \
	      Macro(OBJECT_DECLARATOR_ID), \
	      Macro(OPERATOR_FUNCTION_ID), \
	      Macro(PARAMETER_DECLARATOR_OPT), \
	      Macro(PRIMARY_EXPRESSION), \
	      Macro(QUALIFIED_ID), \
	      Macro(SCOPED_IDENTIFIER), \
	      Macro(STRING), \
	      Macro(TEMPLATE_ARGUMENT), \
	      Macro(TEMPLATE_ARGUMENT_LIST), \
	      Macro(TEMPLATE_ID), \
	      Macro(TYPE_ID_DECLARATOR_OPT), \
	      Macro(UNQUALIFIED_ID)

class FogSemantics_Declarator : private FogSemantics_Missing {
private:
	enum DeclaratorBit { FOG_SEMANTICS_DECLARATORS(FOG_SEMANTICS_BIT) };
public:
	enum Declarator {
		FOG_SEMANTICS_DECLARATORS(FOG_SEMANTICS_BIT_TO_MASK),
		IS_CONVERSION_FUNCTION_ID = IS_UNQUALIFIED_ID,
		IS_DESTRUCTOR = IS_UNQUALIFIED_ID,
		IS_THIS = IS_PRIMARY_EXPRESSION
	};
private:
	enum DeclaratorMasks {
		EPSILON_MASK = IS_EPSILON,
		IDENTIFIER_MASK = IS_IDENTIFIER,
		IDENTIFIER_OPT_MASK = EPSILON_MASK | IDENTIFIER_MASK,
		SCOPED_IDENTIFIER_MASK = IS_SCOPED_IDENTIFIER | IDENTIFIER_MASK,
		SCOPED_IDENTIFIER_OPT_MASK = EPSILON_MASK | SCOPED_IDENTIFIER_MASK,
		NAME_DECL_SPECIFIER_SEQ_MSK = IS_NAME_DECL_SPECIFIER_SEQ | SCOPED_IDENTIFIER_MASK,  //   ENTITY_MASK
		OPERATOR_FUNCTION_ID_MASK = IS_OPERATOR_FUNCTION_ID,
		BIT_FIELD_DECLARATOR_ID_OPT_MASK = IS_BIT_FIELD_DECLARATOR_ID_OPT | SCOPED_IDENTIFIER_OPT_MASK,
		BIT_FIELD_DECLARATOR_ELEM_MASK = IS_BIT_FIELD_DECLARATOR_ELEM,
		BIT_FIELD_DECLARATOR_MASK = IS_BIT_FIELD_DECLARATOR | BIT_FIELD_DECLARATOR_ELEM_MASK,
		CONVERSION_DECLARATOR_MASK = IS_CONVERSION_DECLARATOR,
		CONVERSION_DECLARATOR_OPT_MASK = EPSILON_MASK | CONVERSION_DECLARATOR_MASK,
		OBJECT_NAME_MASK = SCOPED_IDENTIFIER_MASK,
		OBJECT_DECLARATOR_ID_MASK = IS_OBJECT_DECLARATOR_ID | OBJECT_NAME_MASK,
		OBJECT_DECLARATOR_MASK = IS_OBJECT_DECLARATOR | OBJECT_DECLARATOR_ID_MASK,
		INIT_DECLARATOR_MASK = OBJECT_DECLARATOR_MASK | BIT_FIELD_DECLARATOR_MASK,
		PARAMETER_DECLARATOR_ID_OPT_MASK = IDENTIFIER_OPT_MASK,
		PARAMETER_DECLARATOR_OPT_MASK = IS_PARAMETER_DECLARATOR_OPT | PARAMETER_DECLARATOR_ID_OPT_MASK,
		TYPE_ID_DECLARATOR_OPT_MASK = IS_TYPE_ID_DECLARATOR_OPT | EPSILON_MASK,
		TEMPLATE_ID_MASK = IS_TEMPLATE_ID,
		UNQUALIFIED_ID_MASK = IS_UNQUALIFIED_ID | IDENTIFIER_MASK | TEMPLATE_ID_MASK | OPERATOR_FUNCTION_ID_MASK,
		QUALIFIED_ID_MASK = IS_QUALIFIED_ID,
		ID_EXPRESSION_MASK = UNQUALIFIED_ID_MASK | QUALIFIED_ID_MASK,
		GLOBAL_ID_EXPRESSION_MASK = IS_GLOBAL_ID_EXPRESSION,
		MEMBER_ID_EXPRESSION_MSK = IS_MEMBER_ID_EXPRESSION | GLOBAL_ID_EXPRESSION_MASK | ID_EXPRESSION_MASK, //   PSEUDO_DESTRUCTOR_NAME
		FUNCTION_NAME_MASK = ID_EXPRESSION_MASK | GLOBAL_ID_EXPRESSION_MASK,
		FUNCTION_DECLARATOR_ID_MASK = IS_FUNCTION_DECLARATOR_ID | FUNCTION_NAME_MASK,
		DECLARATION_MSK = EPSILON_MASK | FUNCTION_DECLARATOR_ID_MASK,
		LITERAL_MASK = IS_CHARACTER | IS_NUMBER | IS_STRING,
		PRIMARY_EXPRESSION_MASK = IS_PRIMARY_EXPRESSION | ID_EXPRESSION_MASK | SCOPED_IDENTIFIER_MASK | LITERAL_MASK,
		BASIC_EXPRESSION_MASK = IS_BASIC_EXPRESSION | PRIMARY_EXPRESSION_MASK,
		CONDITIONAL_EXPRESSION_MASK = IS_CONDITIONAL_EXPRESSION | BASIC_EXPRESSION_MASK,
		CONSTANT_EXPRESSION_MASK = CONDITIONAL_EXPRESSION_MASK,
		CONSTANT_EXPRESSION_OPT_MASK = CONSTANT_EXPRESSION_MASK | EPSILON_MASK,
		ASSIGNMENT_EXPRESSION_MASK = IS_ASSIGNMENT_EXPRESSION | CONDITIONAL_EXPRESSION_MASK,
		EXPRESSION_MASK = IS_EXPRESSION | ASSIGNMENT_EXPRESSION_MASK,
		EXPRESSION_LIST_MASK = EXPRESSION_MASK,
		EXPRESSION_LIST_OPT_MASK = EXPRESSION_LIST_MASK | EPSILON_MASK,
		INITIALIZER_CLAUSE_MASK = IS_INITIALIZER_CLAUSE | ASSIGNMENT_EXPRESSION_MASK,
		TEMPLATE_ARGUMENT_MSK = IS_TEMPLATE_ARGUMENT | ASSIGNMENT_EXPRESSION_MASK, //   TYPE_ID_MASK | IS_TEMPLATE_NAME
		TEMPLATE_ARGUMENT_LIST_MSK = IS_TEMPLATE_ARGUMENT_LIST | TEMPLATE_ARGUMENT_MSK //   TYPE_ID_MASK | IS_TEMPLATE_NAME
	};
	
private:
	Declarator _declarator;
protected:
	FogSemantics_Declarator(Declarator declaratorSemantics = Declarator(0)) : _declarator(declaratorSemantics) {}
	bool operator!() const { return _declarator == Declarator(0); }
	void assign(Declarator declaratorSemantics) { _declarator = declaratorSemantics; }
	bool is_declarationDeclarator() const { return (_declarator & DECLARATION_MSK) != 0; }
	bool is_member_id_expressionDeclarator() const { return (_declarator & MEMBER_ID_EXPRESSION_MSK) != 0; }
	bool is_name_decl_specifier_seqDeclarator() const { return (_declarator & NAME_DECL_SPECIFIER_SEQ_MSK) != 0; }
	bool is_template_argumentDeclarator() const { return (_declarator & TEMPLATE_ARGUMENT_MSK) != 0; }
	bool is_template_argument_listDeclarator() const { return (_declarator & TEMPLATE_ARGUMENT_LIST_MSK) != 0; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _declarator = Declarator(0); }
	void set(Declarator declaratorSemantics) { _declarator = Declarator(_declarator | declaratorSemantics); }
public:
	bool is_assignment_expression() const { return (_declarator & ASSIGNMENT_EXPRESSION_MASK) != 0; }
	bool is_basic_expression() const { return (_declarator & BASIC_EXPRESSION_MASK) != 0; }
	bool is_bit_field_declarator_id_opt() const { return (_declarator & BIT_FIELD_DECLARATOR_ID_OPT_MASK) != 0; }
	bool is_bit_field_declarator() const { return (_declarator & BIT_FIELD_DECLARATOR_MASK) != 0; }
	bool is_bit_field_declarator_elem() const { return (_declarator & BIT_FIELD_DECLARATOR_ELEM_MASK) != 0; }
	bool is_character() const { return (_declarator & IS_CHARACTER) != 0; }
	bool is_conditional_expression() const { return (_declarator & CONDITIONAL_EXPRESSION_MASK) != 0; }
	bool is_constant_expression() const { return (_declarator & CONSTANT_EXPRESSION_MASK) != 0; }
	bool is_constant_expression_opt() const { return (_declarator & CONSTANT_EXPRESSION_OPT_MASK) != 0; }
	bool is_conversion_declarator() const { return (_declarator & CONVERSION_DECLARATOR_MASK) != 0; }
	bool is_conversion_declarator_opt() const { return (_declarator & CONVERSION_DECLARATOR_OPT_MASK) != 0; }
	bool is_enumerator() const { return is_identifier(); }
	bool is_epsilon() const { return (_declarator & EPSILON_MASK) != 0; }
	bool is_expression() const { return (_declarator & EXPRESSION_MASK) != 0; }
	bool is_expression_list() const { return (_declarator & EXPRESSION_LIST_MASK) != 0; }
	bool is_expression_list_opt() const { return (_declarator & EXPRESSION_LIST_OPT_MASK) != 0; }
	bool is_function_declarator_id() const { return (_declarator & FUNCTION_DECLARATOR_ID_MASK) != 0; }
	bool is_function_name() const { return (_declarator & FUNCTION_NAME_MASK) != 0; }
	bool is_global_id_expression() const { return (_declarator & GLOBAL_ID_EXPRESSION_MASK) != 0; }
	bool is_id_expression() const { return (_declarator & ID_EXPRESSION_MASK) != 0; }
	bool is_identifier() const { return (_declarator & IDENTIFIER_MASK) != 0; }
	bool is_identifier_opt() const { return (_declarator & IDENTIFIER_OPT_MASK) != 0; }
	bool is_init_declarator() const { return (_declarator & INIT_DECLARATOR_MASK) != 0; }
	bool is_initializer_clause() const { return (_declarator & INITIALIZER_CLAUSE_MASK) != 0; }
	bool is_number() const { return (_declarator & IS_NUMBER) != 0; }
	bool is_object_declarator() const { return (_declarator & OBJECT_DECLARATOR_MASK) != 0; }
	bool is_object_declarator_id() const { return (_declarator & OBJECT_DECLARATOR_ID_MASK) != 0; }
	bool is_operator_function_id() const { return (_declarator & OPERATOR_FUNCTION_ID_MASK) != 0; }
	bool is_parameter_declarator_id_opt() const { return (_declarator & PARAMETER_DECLARATOR_ID_OPT_MASK) != 0; }
	bool is_parameter_declarator_opt() const { return (_declarator & PARAMETER_DECLARATOR_OPT_MASK) != 0; }
	bool is_qualified_id() const { return (_declarator & QUALIFIED_ID_MASK) != 0; }
	bool is_scoped_identifier() const { return (_declarator & SCOPED_IDENTIFIER_MASK) != 0; }
	bool is_scoped_identifier_opt() const { return (_declarator & SCOPED_IDENTIFIER_OPT_MASK) != 0; }
	bool is_string() const { return (_declarator & IS_STRING) != 0; }
	bool is_type_id_declarator_opt() const { return (_declarator & TYPE_ID_DECLARATOR_OPT_MASK) != 0; }
	bool is_unqualified_id() const { return (_declarator & UNQUALIFIED_ID_MASK) != 0; }
public:
	static const FogSemantics& bool_value() { return number_value(); }
	static const FogSemantics& character_value();
	static const FogSemantics& double_value() { return number_value(); }
	FOG_SEMANTICS_MISSING_VALUE(enumerator_value)
	FOG_SEMANTICS_MISSING_VALUE(expression_value)
	static const FogSemantics& identifier_value();
	FOG_SEMANTICS_MISSING_VALUE(initializer_clause_value)
	static const FogSemantics& number_value();
	static const FogSemantics& signed_value() { return number_value(); }
	static const FogSemantics& string_value();
	static const FogSemantics& unsigned_value() { return number_value(); }
};

#define FOG_SEMANTICS_EXPRESSIONS(Macro) \
	Macro(CLASS_KEY), \
	Macro(CONVERSION_TYPE_ID),                          /*   {pointer}* epsilon */ \
	      Macro(ELLIPSIS), \
	      Macro(ENUMERATOR_DEFINITION), \
	      Macro(FUNCTION_BODY), \
	      Macro(FUNCTION_DECLARATION_ID), \
	      Macro(INIT_DECLARATOR_LIST), \
	      Macro(MEM_INITIALIZER), \
	      Macro(MISPARSED_EXPRESSION), \
	      Macro(PSEUDO_DESTRUCTOR_ID), \
	      Macro(PSEUDO_DESTRUCTOR_NAME), \
	      Macro(PUNCTUATION), \
	      Macro(RESERVED), \
	      Macro(TEMPLATE_UNQUALIFIED_ID), \
	      Macro(TYPE_CAST),                                   /*   ( type-id ) */ \
	      Macro(USING_DECL_SPECIFIER_SEQ), \
	      Macro(USING_NAME_DECL_SPECIFIER_SEQ), \
	      Macro(ZERO)                                         /*   0 (single character) */

class FogSemantics_Expression : private FogSemantics_Missing {
private:
	enum ExpressionBit { FOG_SEMANTICS_EXPRESSIONS(FOG_SEMANTICS_BIT) };
public:
	enum Expression { FOG_SEMANTICS_EXPRESSIONS(FOG_SEMANTICS_BIT_TO_MASK) };
private:
	enum ExpressionMasks {
		CONVERSION_TYPE_ID_MSK = IS_CONVERSION_TYPE_ID, //   TYPE_SPECIFIER_SEQ
		FUNCTION_BODY_MASK = IS_FUNCTION_BODY,
		FUNCTION_DECLARATION_ID_MASK = IS_FUNCTION_DECLARATION_ID,
		TEMPLATE_UNQUALIFIED_ID_MASK = IS_TEMPLATE_UNQUALIFIED_ID,
		PSEUDO_DESTRUCTOR_ID_MASK = IS_PSEUDO_DESTRUCTOR_ID,
		PSEUDO_DESTRUCTOR_NAME_MASK = IS_PSEUDO_DESTRUCTOR_NAME | PSEUDO_DESTRUCTOR_ID_MASK,
		INIT_DECLARATOR_LIST_MSK = IS_INIT_DECLARATOR_LIST //  | INIT_DECLARATOR_MASK | EPSILON_MASK,
	};
private:
	Expression _expression;
protected:
	FogSemantics_Expression(Expression expressionSemantics = Expression(0)) : _expression(expressionSemantics) {}
	bool operator!() const { return _expression == Expression(0); }
	void assign(Expression expressionSemantics) { _expression = expressionSemantics; }
	bool is_conversion_type_idExpr() const { return (_expression & CONVERSION_TYPE_ID_MSK) != 0; }
	bool is_enumerator_definitionExpr() const { return (_expression & IS_ENUMERATOR_DEFINITION) != 0; }
	bool is_init_declarator_listExpr() const { return (_expression & INIT_DECLARATOR_LIST_MSK) != 0; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _expression = Expression(0); }
	void set(Expression expressionSemantics) { _expression = Expression(_expression | expressionSemantics); }
public:
	bool is_class_key() const { return (_expression & IS_CLASS_KEY) != 0; }
	bool is_ellipsis() const { return (_expression & IS_ELLIPSIS) != 0; }
	bool is_function_body() const { return (_expression & FUNCTION_BODY_MASK) != 0; }
	bool is_function_declaration_id() const { return (_expression & FUNCTION_DECLARATION_ID_MASK) != 0; }
	bool is_mem_initializer() const { return (_expression & IS_MEM_INITIALIZER) != 0; }
	bool is_misparsed_expression() const { return (_expression & IS_MISPARSED_EXPRESSION) != 0; }
	bool is_pseudo_destructor_id() const { return (_expression & PSEUDO_DESTRUCTOR_ID_MASK) != 0; }
	bool is_pseudo_destructor_name() const { return (_expression & PSEUDO_DESTRUCTOR_NAME_MASK) != 0; }
	bool is_punctuation() const { return (_expression & IS_PUNCTUATION) != 0; }
	bool is_reserved() const { return (_expression & IS_RESERVED) != 0; }
	bool is_template_unqualified_id() const { return (_expression & TEMPLATE_UNQUALIFIED_ID_MASK) != 0; }
	bool is_type_cast() const { return (_expression & IS_TYPE_CAST) != 0; }
	bool is_using_decl_specifier_seq() const { return (_expression & IS_USING_DECL_SPECIFIER_SEQ) != 0; }
	bool is_using_name_decl_specifier_seq() const { return (_expression & IS_USING_NAME_DECL_SPECIFIER_SEQ) != 0; }
	bool is_zero() const { return (_expression & IS_ZERO) != 0; }
public:
	FOG_SEMANTICS_MISSING_VALUE(class_key_value)
	static const FogSemantics& function_body_value();
	FOG_SEMANTICS_MISSING_VALUE(punctuation_value)
	FOG_SEMANTICS_MISSING_VALUE(reserved_value)
};

#define FOG_SEMANTICS_METAS(Macro) \
	Macro(META_CTOR_NAME), \
	Macro(META_DECL_SPECIFIER_SEQ), \
	Macro(META_DTOR_NAME), \
	Macro(META_NAME), \
	Macro(META_OBJECT_NAME), \
	Macro(META_PARAMETER_DECLARATION), \
	Macro(META_PARAMETER_DECLARATION_CLAUSE), \
	Macro(META_PARAMETER_DECLARATION_ELEM), \
	Macro(META_PARAMETER_DECLARATOR), \
	Macro(META_PARAMETER_DECLARATOR_ELEM), \
	Macro(META_PARAMETER_TYPE_ID), \
	Macro(META_TYPE_NAME), \
	Macro(META_VARIABLE_DECLARATION), \
	Macro(META_VARIABLE_DECLARATION_ELEM), \
	Macro(META_VARIABLE_DECLARATOR), \
	Macro(META_VARIABLE_DECLARATOR_ELEM), \
	Macro(META_VARIABLE_TYPE_ID), \
	Macro(SCOPED_META_NAME), \
	Macro(SCOPED_META_TYPE_NAME), \
	Macro(UNMETA_DECL_SPECIFIER_SEQ)

class FogSemantics_Meta : private FogSemantics_Missing {
private:
	enum MetaBit { FOG_SEMANTICS_METAS(FOG_SEMANTICS_BIT) };
public:
	enum Meta { FOG_SEMANTICS_METAS(FOG_SEMANTICS_BIT_TO_MASK) };
private:
	enum MetaMasks {
		META_NAME_MSK = IS_META_NAME,                   //   | IS_CLASS_NAME ..
		META_TYPE_NAME_MSK = IS_META_TYPE_NAME | META_NAME_MSK, //   | IS_ACTUAL_META_TYPE_NAME
		SCOPED_META_NAME_MSK = IS_SCOPED_META_NAME | META_NAME_MSK,
		SCOPED_META_TYPE_NAME_MSK = IS_SCOPED_META_TYPE_NAME | META_TYPE_NAME_MSK, //   | IS_ACTUAL_META_TYPE_NAME
		META_DECL_SPECIFIER_SEQ_MSK = IS_META_DECL_SPECIFIER_SEQ | SCOPED_META_TYPE_NAME_MSK, //   | IS_ACTUAL_META_TYPE_NAME
		META_OBJECT_NAME_MSK = IS_META_OBJECT_NAME | SCOPED_META_NAME_MSK,
		META_PARAMETER_NAME_MSK = META_NAME_MSK,
		META_PARAMETER_DECLARATOR_ELEM_MSK = IS_META_PARAMETER_DECLARATOR_ELEM | META_PARAMETER_NAME_MSK,
		META_PARAMETER_DECLARATION_ELEM_MASK = IS_META_PARAMETER_DECLARATION_ELEM,
		META_PARAMETER_TYPE_ID_MASK = IS_META_PARAMETER_TYPE_ID | META_PARAMETER_DECLARATION_ELEM_MASK,
		META_PARAMETER_DECLARATOR_MSK = IS_META_PARAMETER_DECLARATOR | META_PARAMETER_DECLARATOR_ELEM_MSK,
		META_PARAMETER_DECLARATION_MASK = IS_META_PARAMETER_DECLARATION | META_PARAMETER_TYPE_ID_MASK,
		META_PARAMETER_DECLARATION_CLAUSE_MASK = IS_META_PARAMETER_DECLARATION_CLAUSE,
		META_VARIABLE_DECLARATOR_ELEM_MSK = IS_META_VARIABLE_DECLARATOR_ELEM | META_OBJECT_NAME_MSK,
		META_VARIABLE_DECLARATION_ELEM_MASK = IS_META_VARIABLE_DECLARATION_ELEM,
		META_VARIABLE_TYPE_ID_MASK = IS_META_VARIABLE_TYPE_ID | META_VARIABLE_DECLARATION_ELEM_MASK,
		META_VARIABLE_DECLARATOR_MSK = IS_META_VARIABLE_DECLARATOR | META_VARIABLE_DECLARATOR_ELEM_MSK,
		META_VARIABLE_DECLARATION_MASK = IS_META_VARIABLE_DECLARATION | META_VARIABLE_TYPE_ID_MASK,
		META_CTOR_NAME_MSK = IS_META_CTOR_NAME | SCOPED_META_TYPE_NAME_MSK, //   | IS_ACTUAL_META_TYPE_NAME
		META_DTOR_NAME_MASK = IS_META_DTOR_NAME,
		UNMETA_DECL_SPECIFIER_SEQ_MASK = IS_UNMETA_DECL_SPECIFIER_SEQ
	};
private:
	Meta _meta;
protected:
	FogSemantics_Meta(Meta metaSemantics = Meta(0)) : _meta(metaSemantics) {}
	bool operator!() const { return _meta == Meta(0); }
	void assign(Meta metaSemantics) { _meta = metaSemantics; }
	bool is_meta_ctor_nameMeta() const { return (_meta & META_CTOR_NAME_MSK) != 0; }
	bool is_meta_decl_specifier_seqMeta() const { return (_meta & META_DECL_SPECIFIER_SEQ_MSK) != 0; }
	bool is_meta_nameMeta() const { return (_meta & META_NAME_MSK) != 0; }
	bool is_meta_object_nameMeta() const { return (_meta & META_OBJECT_NAME_MSK) != 0; }
	bool is_meta_parameter_declaratorMeta() const { return (_meta & META_PARAMETER_DECLARATOR_MSK) != 0; }
	bool is_meta_parameter_declarator_elemMeta() const { return (_meta & META_PARAMETER_DECLARATOR_ELEM_MSK) != 0; }
	bool is_meta_parameter_nameMeta() const { return (_meta & META_PARAMETER_NAME_MSK) != 0; }
	bool is_meta_type_nameMeta() const { return (_meta & META_TYPE_NAME_MSK) != 0; }
	bool is_meta_variable_declaratorMeta() const { return (_meta & META_VARIABLE_DECLARATOR_MSK) != 0; }
	bool is_meta_variable_declarator_elemMeta() const { return (_meta & META_VARIABLE_DECLARATOR_ELEM_MSK) != 0; }
	bool is_scoped_meta_nameMeta() const { return (_meta & SCOPED_META_NAME_MSK) != 0; }
	bool is_scoped_meta_type_nameMeta() const { return (_meta & SCOPED_META_TYPE_NAME_MSK) != 0; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _meta = Meta(0); }
	void set(Meta metaSemantics) { _meta = Meta(_meta | metaSemantics); }
public:
	//      bool is_meta_ctor_id() const { return (_meta & META_CTOR_ID_MASK) != 0; }
	//      bool is_meta_dtor_id() const { return (_meta & META_DTOR_ID_MASK) != 0; }
	bool is_meta_dtor_name() const { return (_meta & META_DTOR_NAME_MASK) != 0; }
	bool is_meta_parameter_declaration() const { return (_meta & META_PARAMETER_DECLARATION_MASK) != 0; }
	bool is_meta_parameter_declaration_clause() const { return (_meta & META_PARAMETER_DECLARATION_CLAUSE_MASK) != 0; }
	bool is_meta_parameter_declaration_elem() const { return (_meta & META_PARAMETER_DECLARATION_ELEM_MASK) != 0; }
	bool is_meta_parameter_type_id() const { return (_meta & META_PARAMETER_TYPE_ID_MASK) != 0; }
	bool is_meta_variable_declaration() const { return (_meta & META_VARIABLE_DECLARATION_MASK) != 0; }
	bool is_meta_variable_declaration_elem() const { return (_meta & META_VARIABLE_DECLARATION_ELEM_MASK) != 0; }
	bool is_meta_variable_type_id() const { return (_meta & META_VARIABLE_TYPE_ID_MASK) != 0; }
	bool is_unmeta_decl_specifier_seq() const { return (_meta & UNMETA_DECL_SPECIFIER_SEQ_MASK) != 0; }
};

#define FOG_SEMANTICS_AUTOS(Macro) \
	Macro(AUTO_CTOR_DECLARATION), \
	Macro(AUTO_CTOR_DEFINITION), \
	Macro(AUTO_CTOR_NAME), \
	Macro(AUTO_DECL_SPECIFIER_SEQ), \
	Macro(AUTO_DTOR_DECLARATION), \
	Macro(AUTO_DTOR_DEFINITION), \
	Macro(AUTO_DTOR_NAME), \
	Macro(AUTO_FUNCTION_DECLARATION), \
	Macro(AUTO_FUNCTION_DECLARATION_ELEM), \
	Macro(AUTO_FUNCTION_DECLARATOR), \
	Macro(AUTO_FUNCTION_DECLARATOR_ELEM), \
	Macro(AUTO_FUNCTION_DEFINITION), \
	Macro(AUTO_FUNCTION_DEFINITOR), \
	Macro(AUTO_META_DECL_SPECIFIER_SEQ), \
	Macro(AUTO_VARIABLE_DECLARATION), \
	Macro(AUTO_VARIABLE_DECLARATION_ELEM), \
	Macro(AUTO_VARIABLE_DECLARATOR), \
	Macro(AUTO_VARIABLE_TYPE_ID), \
	Macro(META_CTOR_DECLARATION), \
	Macro(META_CTOR_DEFINITION), \
	Macro(META_DTOR_DECLARATION), \
	Macro(META_DTOR_DEFINITION), \
	Macro(META_FUNCTION_DECLARATION), \
	Macro(META_FUNCTION_DECLARATION_ELEM), \
	Macro(META_FUNCTION_DECLARATOR), \
	Macro(META_FUNCTION_DECLARATOR_ELEM), \
	Macro(META_FUNCTION_DEFINITION), \
	Macro(META_FUNCTION_DEFINITOR)

class FogSemantics_Auto : private FogSemantics_Missing {
private:
	enum AutoBit { FOG_SEMANTICS_AUTOS(FOG_SEMANTICS_BIT) };
public:
	enum Auto { FOG_SEMANTICS_AUTOS(FOG_SEMANTICS_BIT_TO_MASK) };
private:
	enum AutoMasks {
		AUTO_CTOR_DECLARATION_MASK = IS_AUTO_CTOR_DECLARATION,
		AUTO_CTOR_DEFINITION_MASK = IS_AUTO_CTOR_DEFINITION,
		AUTO_CTOR_NAME_MASK = IS_AUTO_CTOR_NAME,
		AUTO_DECL_SPECIFIER_SEQ_MASK = IS_AUTO_DECL_SPECIFIER_SEQ,
		AUTO_DTOR_DECLARATION_MASK = IS_AUTO_DTOR_DECLARATION,
		AUTO_DTOR_DEFINITION_MASK = IS_AUTO_DTOR_DEFINITION,
		AUTO_DTOR_NAME_MASK = IS_AUTO_DTOR_NAME,
		AUTO_FUNCTION_DECLARATOR_ELEM_MASK = IS_AUTO_FUNCTION_DECLARATOR_ELEM,
		AUTO_FUNCTION_DECLARATION_ELEM_MASK = IS_AUTO_FUNCTION_DECLARATION_ELEM,
		AUTO_FUNCTION_DECLARATOR_MASK = IS_AUTO_FUNCTION_DECLARATOR | AUTO_FUNCTION_DECLARATOR_ELEM_MASK,
		AUTO_FUNCTION_DECLARATION_MASK = IS_AUTO_FUNCTION_DECLARATION | AUTO_FUNCTION_DECLARATION_ELEM_MASK,
		AUTO_FUNCTION_DEFINITOR_MASK = IS_AUTO_FUNCTION_DEFINITOR,
		AUTO_FUNCTION_DEFINITION_MASK = IS_AUTO_FUNCTION_DEFINITION | IS_AUTO_CTOR_DEFINITION | IS_AUTO_DTOR_DEFINITION,
		AUTO_META_DECL_SPECIFIER_SEQ_MASK = IS_AUTO_META_DECL_SPECIFIER_SEQ,
		AUTO_VARIABLE_DECLARATION_ELEM_MASK = IS_AUTO_VARIABLE_DECLARATION_ELEM,
		AUTO_VARIABLE_TYPE_ID_MASK = IS_AUTO_VARIABLE_TYPE_ID,
		AUTO_VARIABLE_DECLARATOR_MASK = IS_AUTO_VARIABLE_DECLARATOR,
		AUTO_VARIABLE_DECLARATION_MASK = IS_AUTO_VARIABLE_DECLARATION | AUTO_VARIABLE_TYPE_ID_MASK,
		META_CTOR_DECLARATION_MASK = IS_META_CTOR_DECLARATION,
		META_CTOR_DEFINITION_MASK = IS_META_CTOR_DEFINITION,
		META_DTOR_DECLARATION_MASK = IS_META_DTOR_DECLARATION,
		META_DTOR_DEFINITION_MASK = IS_META_DTOR_DEFINITION,
		META_FUNCTION_DECLARATOR_ELEM_MASK = IS_META_FUNCTION_DECLARATOR_ELEM,
		META_FUNCTION_DECLARATION_ELEM_MASK = IS_META_FUNCTION_DECLARATION_ELEM,
		META_FUNCTION_DECLARATOR_MASK = IS_META_FUNCTION_DECLARATOR | META_FUNCTION_DECLARATOR_ELEM_MASK,
		META_FUNCTION_DECLARATION_MASK = IS_META_FUNCTION_DECLARATION | META_FUNCTION_DECLARATION_ELEM_MASK,
		META_FUNCTION_DEFINITOR_MASK = IS_META_FUNCTION_DEFINITOR,
		META_FUNCTION_DEFINITION_MASK = IS_META_FUNCTION_DEFINITION | IS_META_CTOR_DEFINITION | IS_META_DTOR_DEFINITION
	};
private:
	Auto _auto;
protected:
	FogSemantics_Auto(Auto autoSemantics = Auto(0)) : _auto(autoSemantics) {}
	bool operator!() const { return _auto == Auto(0); }
	void assign(Auto autoSemantics) { _auto = autoSemantics; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _auto = Auto(0); }
	void set(Auto autoSemantics) { _auto = Auto(_auto | autoSemantics); }
public:
	bool is_auto_ctor_declaration() const { return (_auto & AUTO_CTOR_DECLARATION_MASK) != 0; }
	bool is_auto_ctor_definition() const { return (_auto & AUTO_CTOR_DEFINITION_MASK) != 0; }
	bool is_auto_ctor_name() const { return (_auto & AUTO_CTOR_NAME_MASK) != 0; }
	bool is_auto_decl_specifier_seq() const { return (_auto & AUTO_DECL_SPECIFIER_SEQ_MASK) != 0; }
	bool is_auto_dtor_declaration() const { return (_auto & AUTO_DTOR_DECLARATION_MASK) != 0; }
	bool is_auto_dtor_definition() const { return (_auto & AUTO_DTOR_DEFINITION_MASK) != 0; }
	bool is_auto_dtor_name() const { return (_auto & AUTO_DTOR_NAME_MASK) != 0; }
	bool is_auto_function_declaration() const { return (_auto & AUTO_FUNCTION_DECLARATION_MASK) != 0; }
	bool is_auto_function_declaration_elem() const { return (_auto & AUTO_FUNCTION_DECLARATION_ELEM_MASK) != 0; }
	bool is_auto_function_declarator() const { return (_auto & AUTO_FUNCTION_DECLARATOR_MASK) != 0; }
	bool is_auto_function_declarator_elem() const { return (_auto & AUTO_FUNCTION_DECLARATOR_ELEM_MASK) != 0; }
	bool is_auto_function_definition() const { return (_auto & AUTO_FUNCTION_DEFINITION_MASK) != 0; }
	bool is_auto_function_definitor() const { return (_auto & AUTO_FUNCTION_DEFINITOR_MASK) != 0; }
	bool is_auto_meta_decl_specifier_seq() const { return (_auto & AUTO_META_DECL_SPECIFIER_SEQ_MASK) != 0; }
	bool is_auto_variable_declaration() const { return (_auto & AUTO_VARIABLE_DECLARATION_MASK) != 0; }
	bool is_auto_variable_declaration_elem() const { return (_auto & AUTO_VARIABLE_DECLARATION_ELEM_MASK) != 0; }
	bool is_auto_variable_declarator() const { return (_auto & AUTO_VARIABLE_DECLARATOR_MASK) != 0; }
	bool is_auto_variable_type_id() const { return (_auto & AUTO_VARIABLE_TYPE_ID_MASK) != 0; }
	bool is_meta_ctor_declaration() const { return (_auto & META_CTOR_DECLARATION_MASK) != 0; }
	bool is_meta_ctor_definition() const { return (_auto & META_CTOR_DEFINITION_MASK) != 0; }
	bool is_meta_dtor_declaration() const { return (_auto & META_DTOR_DECLARATION_MASK) != 0; }
	bool is_meta_dtor_definition() const { return (_auto & META_DTOR_DEFINITION_MASK) != 0; }
	bool is_meta_function_declaration() const { return (_auto & META_FUNCTION_DECLARATION_MASK) != 0; }
	bool is_meta_function_declaration_elem() const { return (_auto & META_FUNCTION_DECLARATION_ELEM_MASK) != 0; }
	bool is_meta_function_declarator() const { return (_auto & META_FUNCTION_DECLARATOR_MASK) != 0; }
	bool is_meta_function_declarator_elem() const { return (_auto & META_FUNCTION_DECLARATOR_ELEM_MASK) != 0; }
	bool is_meta_function_definition() const { return (_auto & META_FUNCTION_DEFINITION_MASK) != 0; }
	bool is_meta_function_definitor() const { return (_auto & META_FUNCTION_DEFINITOR_MASK) != 0; }
};

#define FOG_SEMANTICS_NAMES(Macro) \
	Macro(BIT_FIELD_TYPE_ID), \
	Macro(CLASS_NAME), \
	Macro(CLASS_SPECIFIER), \
	Macro(CLASS_TYPE_PARAMETER), \
	Macro(CV_QUALIFIER), \
	Macro(CV_QUALIFIER_SEQ), \
	Macro(ELABORATED_TYPE_SPECIFIER), \
	Macro(ENUM_NAME), \
	Macro(ENUM_SPECIFIER), \
	Macro(LINKAGE_SPECIFIER), \
	Macro(NAMESPACE_NAME), \
	Macro(PARAMETER_DECLARATION), \
	Macro(PARAMETER_TYPE_ID), \
	Macro(PRIMARY_EXPRESSION_ID), \
	Macro(SCOPED_CLASS_NAME), \
	Macro(SCOPED_TYPE_NAME), \
	Macro(SCOPE_SPECIFIER), \
	Macro(SIMPLE_TYPE_SPECIFIER), \
	Macro(TEMPLATED_PARAMETER), \
	Macro(TEMPLATE_PARAMETER_LIST), \
	Macro(TEMPLATE_TYPE_NAME), \
	Macro(TEMPLATE_VALUE_NAME), \
	Macro(TYPEDEF_NAME), \
	Macro(TYPED_DECL_SPECIFIER_SEQ), \
	Macro(TYPENAME_TYPE_PARAMETER), \
	Macro(TYPE_ID), \
	Macro(TYPE_PARAMETER), \
	Macro(TYPE_SPECIFIER), \
	Macro(TYPE_SPECIFIER_SEQ), \
	Macro(UNTYPED_DECL_SPECIFIER_SEQ)

class FogSemantics_Name : private FogSemantics_Missing {
private:
	enum NameBit { FOG_SEMANTICS_NAMES(FOG_SEMANTICS_BIT) };
public:
	enum Name {
		FOG_SEMANTICS_NAMES(FOG_SEMANTICS_BIT_TO_MASK)
	};
private:
	enum NameMasks {
		CLASS_NAME_MSK = IS_CLASS_NAME,    //    IS_ACTUAL_CLASS_NAME, TEMPLATE_ID applied by TemplateName
		NAMESPACE_NAME_MSK = IS_NAMESPACE_NAME,                 //   IS_ACTUAL_NAMESPACE_NAME
		CLASS_TYPE_PARAMETER_MASK = IS_CLASS_TYPE_PARAMETER,
		CV_QUALIFIER_MASK = IS_CV_QUALIFIER,
		TEMPLATE_TYPE_NAME_MASK = IS_TEMPLATE_TYPE_NAME,
		TEMPLATE_VALUE_NAME_MASK = IS_TEMPLATE_VALUE_NAME,
		TEMPLATED_PARAMETER_MASK = IS_TEMPLATED_PARAMETER,
		TYPENAME_TYPE_PARAMETER_MASK = IS_TYPENAME_TYPE_PARAMETER,
		UNTYPED_DECL_SPECIFIER_SEQ_MASK = IS_UNTYPED_DECL_SPECIFIER_SEQ,
		//
		TEMPLATE_NAME_MASK = TEMPLATE_TYPE_NAME_MASK | TEMPLATE_VALUE_NAME_MASK,
		TYPE_NAME_MSK = CLASS_NAME_MSK | IS_ENUM_NAME | IS_TYPEDEF_NAME | TEMPLATE_TYPE_NAME_MASK, //    IS_ACTUAL_CLASS_NAME
		SCOPED_CLASS_NAME_MSK = IS_SCOPED_CLASS_NAME | CLASS_NAME_MSK, //    IS_ACTUAL_CLASS_NAME
		SCOPED_TYPE_NAME_MSK = IS_SCOPED_TYPE_NAME | TYPE_NAME_MSK, //    IS_ACTUAL_CLASS_NAME
		SIMPLE_TYPE_SPECIFIER_MSK = IS_SIMPLE_TYPE_SPECIFIER | SCOPED_TYPE_NAME_MSK, //    IS_ACTUAL_CLASS_NAME
		ELABORATE_TYPE_SPECIFIER_MASK = IS_CLASS_SPECIFIER | IS_ENUM_SPECIFIER | IS_ELABORATED_TYPE_SPECIFIER,
		TYPED_TYPE_SPECIFIER_MSK = SIMPLE_TYPE_SPECIFIER_MSK | ELABORATE_TYPE_SPECIFIER_MASK, //    IS_ACTUAL_CLASS_NAME
		TYPE_SPECIFIER_MSK = IS_TYPE_SPECIFIER | TYPED_TYPE_SPECIFIER_MSK | CV_QUALIFIER_MASK, //    IS_ACTUAL_CLASS_NAME
		CV_QUALIFIER_SEQ_MASK = IS_CV_QUALIFIER_SEQ | CV_QUALIFIER_MASK,
		TYPE_SPECIFIER_SEQ_MSK = IS_TYPE_SPECIFIER_SEQ | TYPE_SPECIFIER_MSK | CV_QUALIFIER_SEQ_MASK, //   IS_ACTUAL_TYPE_NAME
		TYPED_DECL_SPECIFIER_SEQ_MSK = IS_TYPED_DECL_SPECIFIER_SEQ | IS_SCOPE_SPECIFIER | TYPE_SPECIFIER_SEQ_MSK, //   IS_ACTUAL_TYPE_NAME
		DECL_SPECIFIER_SEQ_MSK = TYPED_DECL_SPECIFIER_SEQ_MSK | UNTYPED_DECL_SPECIFIER_SEQ_MASK, //   IS_ACTUAL_TYPE_NAME
		MEM_INITIALIZER_ID_MSK = SCOPED_CLASS_NAME_MSK, //   IDENTIFIER_MASK, IS_ACTUAL_CLASS_NAME
		BIT_FIELD_TYPE_ID_MSK = IS_BIT_FIELD_TYPE_ID | TYPED_DECL_SPECIFIER_SEQ_MSK, //   IS_ACTUAL_TYPE_NAME
		TYPE_ID_MSK = IS_TYPE_ID | TYPE_SPECIFIER_SEQ_MSK, //   IS_ACTUAL_TYPE_NAME
		PARAMETER_TYPE_ID_MSK = IS_PARAMETER_TYPE_ID | TYPED_DECL_SPECIFIER_SEQ_MSK, //   IS_ACTUAL_TYPE_NAME
		PARAMETER_DECLARATION_MSK = IS_PARAMETER_DECLARATION | PARAMETER_TYPE_ID_MSK, //   IS_ACTUAL_TYPE_NAME
		SIMPLE_TYPE_PARAMETER_MASK = CLASS_TYPE_PARAMETER_MASK | TYPENAME_TYPE_PARAMETER_MASK,
		TYPE_PARAMETER_MASK = IS_TYPE_PARAMETER | SIMPLE_TYPE_PARAMETER_MASK | TEMPLATED_PARAMETER_MASK,
		TEMPLATE_PARAMETER_MSK = TYPE_PARAMETER_MASK | PARAMETER_DECLARATION_MSK, //   IS_ACTUAL_TYPE_NAME
		TEMPLATE_PARAMETER_LIST_MSK = IS_TEMPLATE_PARAMETER_LIST | TEMPLATE_PARAMETER_MSK, //   IS_ACTUAL_TYPE_NAME
		TYPE_DECLARATION_MSK = TYPE_SPECIFIER_MSK | IS_LINKAGE_SPECIFIER | IS_SCOPE_SPECIFIER, //    IS_ACTUAL_CLASS_NAME ...
		DECLARATION_MSK = DECL_SPECIFIER_SEQ_MSK | IS_LINKAGE_SPECIFIER //   IS_ACTUAL_TYPE_NAME ...
	};
private:
	Name _name;
protected:
	FogSemantics_Name(Name nameSemantics = Name(0)) : _name(nameSemantics) {}
	bool operator!() const { return _name == Name(0); }
	void assign(Name nameSemantics) { _name = nameSemantics; }
	bool is_bit_field_type_idName() const { return (_name & BIT_FIELD_TYPE_ID_MSK) != 0; }
	bool is_class_nameName() const { return (_name & CLASS_NAME_MSK) != 0; }
	bool is_decl_specifier_seqName() const { return (_name & DECL_SPECIFIER_SEQ_MSK) != 0; }
	bool is_declarationName() const { return (_name & DECLARATION_MSK) != 0; }
	bool is_mem_initializer_idName() const { return (_name & MEM_INITIALIZER_ID_MSK) != 0; }
	bool is_meta_nameName() const { return (_name & TYPE_NAME_MSK) != 0; }
	bool is_namespace_nameName() const { return (_name & NAMESPACE_NAME_MSK) != 0; }
	bool is_parameter_declarationName() const { return (_name & PARAMETER_DECLARATION_MSK) != 0; }
	bool is_parameter_type_idName() const { return (_name & PARAMETER_TYPE_ID_MSK) != 0; }
	bool is_scoped_class_nameName() const { return (_name & SCOPED_CLASS_NAME_MSK) != 0; }
	bool is_scoped_type_nameName() const { return (_name & SCOPED_TYPE_NAME_MSK) != 0; }
	bool is_simple_type_specifierName() const { return (_name & SIMPLE_TYPE_SPECIFIER_MSK) != 0; }
	bool is_template_parameterName() const { return (_name & TEMPLATE_PARAMETER_MSK) != 0; }
	bool is_template_parameter_listName() const { return (_name & TEMPLATE_PARAMETER_LIST_MSK) != 0; }
	bool is_type_declarationName() const { return (_name & TYPE_DECLARATION_MSK) != 0; }
	bool is_type_idName() const { return (_name & TYPE_ID_MSK) != 0; }
	bool is_type_nameName() const { return (_name & TYPE_NAME_MSK) != 0; }
	bool is_type_specifierName() const { return (_name & TYPE_SPECIFIER_MSK) != 0; }
	bool is_type_specifier_seqName() const { return (_name & TYPE_SPECIFIER_SEQ_MSK) != 0; }
	bool is_typed_decl_specifier_seqName() const { return (_name & TYPED_DECL_SPECIFIER_SEQ_MSK) != 0; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _name = Name(0); }
	void set(Name nameSemantics) { _name = Name(_name | nameSemantics); }
public:
	bool is_class_type_parameter() const { return (_name & CLASS_TYPE_PARAMETER_MASK) != 0; }
	bool is_cv_qualifier() const { return (_name & CV_QUALIFIER_MASK) != 0; }
	bool is_cv_qualifier_seq() const { return (_name & CV_QUALIFIER_SEQ_MASK) != 0; }
	bool is_scope_specifier() const { return (_name & IS_SCOPE_SPECIFIER) != 0; }
	bool is_simple_type_parameter() const { return (_name & SIMPLE_TYPE_PARAMETER_MASK) != 0; }
	bool is_template_name() const { return (_name & TEMPLATE_NAME_MASK) != 0; }
	bool is_template_type_name() const { return (_name & TEMPLATE_TYPE_NAME_MASK) != 0; }
	bool is_template_value_name() const { return (_name & TEMPLATE_VALUE_NAME_MASK) != 0; }
	bool is_templated_parameter() const { return (_name & TEMPLATED_PARAMETER_MASK) != 0; }
	bool is_type_parameter() const { return (_name & TYPE_PARAMETER_MASK) != 0; }
	bool is_untyped_decl_specifier_seq() const { return (_name & UNTYPED_DECL_SPECIFIER_SEQ_MASK) != 0; }
public:
	FOG_SEMANTICS_MISSING_VALUE(cv_qualifier_value)
	FOG_SEMANTICS_MISSING_VALUE(scope_specifier_value)
	//      FOG_SEMANTICS_MISSING_VALUE(template_parameter_value)
	//      FOG_SEMANTICS_MISSING_VALUE(type_parameter_value)
	FOG_SEMANTICS_MISSING_VALUE(type_specifier_value)
	//      FOG_SEMANTICS_MISSING_VALUE(value_parameter_value)
};

#define FOG_SEMANTICS_ENTITIES(Macro) \
	Macro(BASE), \
	Macro(BUILT_IN), \
	Macro(CLASS), \
	Macro(ENTITY), \
	Macro(ENUM), \
	Macro(EXCEPTION), \
	Macro(FILESPACE), \
	Macro(FUNCTION), \
	Macro(LINKAGE), \
	Macro(META_CLASS), \
	Macro(META_FUNCTION), \
	Macro(META_PARAMETER), \
	Macro(META_VARIABLE), \
	Macro(NAMESPACE), \
	Macro(NAMESPACE_ALIAS), \
	Macro(OBJECT), \
	Macro(PARAMETER), \
	Macro(SCOPE), \
	Macro(STRUCT), \
	Macro(TYPE), \
	Macro(TYPEDEF), \
	Macro(TYPENAME), \
	Macro(UNION), \
	Macro(USING), \
	Macro(VARIABLE)

class FogSemantics_Entity : private FogSemantics_Missing {
private:
	enum EntitiesBit { FOG_SEMANTICS_ENTITIES(FOG_SEMANTICS_BIT) };
public:
	enum Entity {
		FOG_SEMANTICS_ENTITIES(FOG_SEMANTICS_BIT_TO_MASK)
	};
private:
	enum EntityMasks {
		BASE_MASK = IS_BASE,
		BUILT_IN_MASK = IS_BUILT_IN,
		CLASS_MASK = IS_CLASS,
		ENUM_MASK = IS_ENUM,
		EXCEPTION_MASK = IS_EXCEPTION,
		FILESPACE_MASK = IS_FILESPACE,
		FUNCTION_MASK = IS_FUNCTION,
		LINKAGE_MASK = IS_LINKAGE,
		META_CLASS_MASK = IS_META_CLASS,
		META_FUNCTION_MASK = IS_META_FUNCTION,
		META_PARAMETER_MASK = IS_META_PARAMETER,
		META_VARIABLE_MASK = IS_META_VARIABLE,
		NAMESPACE_MASK = IS_NAMESPACE,
		NAMESPACE_ALIAS_MASK = IS_NAMESPACE_ALIAS,
		PARAMETER_MASK = IS_PARAMETER,
		STRUCT_MASK = IS_STRUCT,
		TYPEDEF_MASK = IS_TYPEDEF,
		TYPENAME_MASK = IS_TYPENAME,
		UNION_MASK = IS_UNION,
		USING_MASK = IS_USING,
		VARIABLE_MASK = IS_VARIABLE,
		OBJECT_MASK = IS_OBJECT | EXCEPTION_MASK | FUNCTION_MASK
		              | META_FUNCTION_MASK | META_PARAMETER_MASK | META_VARIABLE_MASK
		              | PARAMETER_MASK | TYPEDEF_MASK | USING_MASK | VARIABLE_MASK,
		TYPE_MASK = IS_TYPE | BUILT_IN_MASK | CLASS_MASK | ENUM_MASK | STRUCT_MASK | UNION_MASK,
		SCOPE_MASK = IS_SCOPE | FILESPACE_MASK | LINKAGE_MASK | META_CLASS_MASK | NAMESPACE_MASK | TYPE_MASK,
		ENTITY_MASK = IS_ENTITY | BASE_MASK | NAMESPACE_ALIAS_MASK | OBJECT_MASK,
		ACTUAL_CLASS_NAME_MASK = CLASS_MASK | STRUCT_MASK | UNION_MASK,
		ACTUAL_META_TYPE_NAME_MASK = TYPE_MASK | META_CLASS_MASK,
		ACTUAL_NAMESPACE_NAME_MASK = NAMESPACE_MASK,
		ACTUAL_TYPE_NAME_MASK = TYPE_MASK,
		ACTUAL_NAME_MASK = ENTITY_MASK
	};
private:
	Entity _entity;
protected:
	FogSemantics_Entity(Entity entitySemantics = Entity(0)) : _entity(entitySemantics) {}
	bool operator!() const { return _entity == Entity(0); }
	void assign(Entity entitySemantics) { _entity = entitySemantics; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _entity = Entity(0); }
	void set(Entity entitySemantics) { _entity = Entity(_entity | entitySemantics); }
public:
	bool is_actual_class_name() const { return (_entity & ACTUAL_CLASS_NAME_MASK) != 0; }
	bool is_actual_name() const { return (_entity & ACTUAL_NAME_MASK) != 0; }
	bool is_actual_meta_type_name() const { return (_entity & ACTUAL_META_TYPE_NAME_MASK) != 0; }
	bool is_actual_namespace_name() const { return (_entity & ACTUAL_NAMESPACE_NAME_MASK) != 0; }
	bool is_actual_type_name() const { return (_entity & ACTUAL_TYPE_NAME_MASK) != 0; }
	bool is_base() const { return (_entity & BASE_MASK) != 0; }
	bool is_built_in() const { return (_entity & BUILT_IN_MASK) != 0; }
	bool is_class() const { return (_entity & CLASS_MASK) != 0; }
	bool is_entity() const { return (_entity & ENTITY_MASK) != 0; }
	bool is_enum() const { return (_entity & ENUM_MASK) != 0; }
	bool is_exception() const { return (_entity & EXCEPTION_MASK) != 0; }
	bool is_filespace() const { return (_entity & FILESPACE_MASK) != 0; }
	bool is_function() const { return (_entity & FUNCTION_MASK) != 0; }
	bool is_linkage() const { return (_entity & LINKAGE_MASK) != 0; }
	bool is_meta_class() const { return (_entity & META_CLASS_MASK) != 0; }
	bool is_meta_function() const { return (_entity & META_FUNCTION_MASK) != 0; }
	bool is_meta_parameter() const { return (_entity & META_PARAMETER_MASK) != 0; }
	bool is_meta_variable() const { return (_entity & META_VARIABLE_MASK) != 0; }
	bool is_namespace() const { return (_entity & NAMESPACE_MASK) != 0; }
	bool is_namespace_alias() const { return (_entity & NAMESPACE_ALIAS_MASK) != 0; }
	bool is_object() const { return (_entity & OBJECT_MASK) != 0; }
	bool is_parameter() const { return (_entity & PARAMETER_MASK) != 0; }
	bool is_scope() const { return (_entity & SCOPE_MASK) != 0; }
	bool is_struct() const { return (_entity & STRUCT_MASK) != 0; }
	bool is_type() const { return (_entity & TYPE_MASK) != 0; }
	bool is_typedef() const { return (_entity & TYPEDEF_MASK) != 0; }
	bool is_typename() const { return (_entity & TYPENAME_MASK) != 0; }
	bool is_union() const { return (_entity & UNION_MASK) != 0; }
	bool is_using() const { return (_entity & USING_MASK) != 0; }
	bool is_variable() const { return (_entity & VARIABLE_MASK) != 0; }
public:
	static const FogSemantics& base_value();
	static const FogSemantics& built_in_value();
	static const FogSemantics& class_value();
	static const FogSemantics& entity_value();
	static const FogSemantics& enum_value();
	static const FogSemantics& exception_value();
	static const FogSemantics& filespace_value();
	static const FogSemantics& function_value();
	static const FogSemantics& linkage_value();
	static const FogSemantics& meta_class_value();
	static const FogSemantics& meta_function_value();
	static const FogSemantics& meta_parameter_value();
	static const FogSemantics& meta_variable_value();
	static const FogSemantics& namespace_value();
	static const FogSemantics& namespace_alias_value();
	static const FogSemantics& object_value();
	static const FogSemantics& parameter_value();
	static const FogSemantics& scope_value();
	static const FogSemantics& struct_value();
	static const FogSemantics& type_value();
	static const FogSemantics& typedef_value();
	static const FogSemantics& typename_value();
	static const FogSemantics& union_value();
	static const FogSemantics& using_value();
	static const FogSemantics& variable_value();
};

class FogSemantics_Others : private FogSemantics_Missing {
public:
	FOG_SEMANTICS_MISSING(is_bool)
	FOG_SEMANTICS_MISSING(is_double)
	//      FOG_SEMANTICS_MISSING(is_exception_declaration)
	FOG_SEMANTICS_MISSING(is_exception_specification)
	FOG_SEMANTICS_MISSING(is_expression_statement)
	FOG_SEMANTICS_MISSING(is_handler)
	FOG_SEMANTICS_MISSING(is_iterator)
	FOG_SEMANTICS_MISSING(is_keyword)
	FOG_SEMANTICS_MISSING(is_list)
	FOG_SEMANTICS_MISSING(is_meta_type)
	FOG_SEMANTICS_MISSING(is_name)
	FOG_SEMANTICS_MISSING(is_object_statement)
	FOG_SEMANTICS_MISSING(is_signed)
	FOG_SEMANTICS_MISSING(is_statement)
	FOG_SEMANTICS_MISSING(is_tree_literal)
	FOG_SEMANTICS_MISSING(is_unsigned)
	FOG_SEMANTICS_MISSING(is_void)
public:
	//      FOG_SEMANTICS_MISSING_VALUE(exception_declaration_value)
	FOG_SEMANTICS_MISSING_VALUE(exception_specification_value)
	FOG_SEMANTICS_MISSING_VALUE(expression_statement_value)
	FOG_SEMANTICS_MISSING_VALUE(handler_value)
	FOG_SEMANTICS_MISSING_VALUE(iterator_value)
	FOG_SEMANTICS_MISSING_VALUE(keyword_value)
	FOG_SEMANTICS_MISSING_VALUE(list_value)
	FOG_SEMANTICS_MISSING_VALUE(meta_type_value)
	FOG_SEMANTICS_MISSING_VALUE(name_value)
	FOG_SEMANTICS_MISSING_VALUE(object_statement_value)
	FOG_SEMANTICS_MISSING_VALUE(statement_value)
	FOG_SEMANTICS_MISSING_VALUE(tree_literal_value)
	FOG_SEMANTICS_MISSING_VALUE(void_value)
public:
	FOG_SEMANTICS_MISSING_VALUE(declaration_value)
	FOG_SEMANTICS_MISSING_VALUE(enumerator_definition_value)
	FOG_SEMANTICS_MISSING_VALUE(template_argument_value)
	FOG_SEMANTICS_MISSING_VALUE(token_value)
};

#define FOG_SEMANTICS_SPECIFIERS(Macro) \
	Macro(ASM_DEFINITION), \
	Macro(NAMESPACE_ALIAS_DEFINITION), \
	Macro(USING_DIRECTIVE)

class FogSemantics_Specifier : private FogSemantics_Missing {
private:
	enum SpecifiersBit { FOG_SEMANTICS_SPECIFIERS(FOG_SEMANTICS_BIT) };
public:
	enum Specifier {
		FOG_SEMANTICS_SPECIFIERS(FOG_SEMANTICS_BIT_TO_MASK)
	};
private:
	enum SpecifierMasks {
		ASM_DEFINITION_MASK = IS_ASM_DEFINITION,
		NAMESPACE_ALIAS_DEFINITION_MASK = IS_NAMESPACE_ALIAS_DEFINITION,
		USING_DIRECTIVE_MASK = IS_USING_DIRECTIVE
	};
private:
	Specifier _specifier;
protected:
	FogSemantics_Specifier(Specifier specifierSemantics = Specifier(0)) : _specifier(specifierSemantics) {}
	bool operator!() const { return _specifier == Specifier(0); }
	void assign(Specifier specifierSemantics) { _specifier = specifierSemantics; }
	const char* print(std::ostream& s, const char* prefixCharacter = "") const;
	void reset() { _specifier = Specifier(0); }
	void set(Specifier specifierSemantics) { _specifier = Specifier(_specifier | specifierSemantics); }
public:
	bool is_asm_definition() const { return (_specifier & ASM_DEFINITION_MASK) != 0; }
	bool is_namespace_alias_definition() const { return (_specifier & NAMESPACE_ALIAS_DEFINITION_MASK) != 0; }
	bool is_using_directive() const { return (_specifier & USING_DIRECTIVE_MASK) != 0; }
public:
	FOG_SEMANTICS_MISSING(is_array_modifier)
	FOG_SEMANTICS_MISSING(is_base_specifier)
	FOG_SEMANTICS_MISSING(is_built_in_type_specifier)
	FOG_SEMANTICS_MISSING(is_class_specifier)
	FOG_SEMANTICS_MISSING(is_decl_specifier)
	FOG_SEMANTICS_MISSING(is_elaborated_type_specifier)
	FOG_SEMANTICS_MISSING(is_enum_specifier)
	FOG_SEMANTICS_MISSING(is_filespace_specifier)
	FOG_SEMANTICS_MISSING(is_function_modifier)
	FOG_SEMANTICS_MISSING(is_linkage_specification)
	FOG_SEMANTICS_MISSING(is_meta_class_specifier)
	FOG_SEMANTICS_MISSING(is_meta_function_specifier)
	FOG_SEMANTICS_MISSING(is_meta_parameter_specifier)
	FOG_SEMANTICS_MISSING(is_meta_variable_specifier)
	FOG_SEMANTICS_MISSING(is_modifier)
	FOG_SEMANTICS_MISSING(is_namespace_definition)
	FOG_SEMANTICS_MISSING(is_object_specifier)
	FOG_SEMANTICS_MISSING(is_parameter_specifier)
	FOG_SEMANTICS_MISSING(is_pointer_modifier)
	FOG_SEMANTICS_MISSING(is_reference_modifier)
	FOG_SEMANTICS_MISSING(is_scoped_modifier)
	FOG_SEMANTICS_MISSING(is_specifier)
	FOG_SEMANTICS_MISSING(is_template_parameter_specifier)
	FOG_SEMANTICS_MISSING(is_templated_parameter_specifier)
	FOG_SEMANTICS_MISSING(is_type_parameter_specifier)
	FOG_SEMANTICS_MISSING(is_typedef_specifier)
	FOG_SEMANTICS_MISSING(is_value_parameter_specifier)
public:
	FOG_SEMANTICS_MISSING_VALUE(array_modifier_value)
	static const FogSemantics& asm_definition_value();
	FOG_SEMANTICS_MISSING_VALUE(base_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(built_in_type_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(class_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(decl_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(elaborated_type_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(enum_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(filespace_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(function_modifier_value)
	FOG_SEMANTICS_MISSING_VALUE(linkage_specification_value)
	FOG_SEMANTICS_MISSING_VALUE(meta_class_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(meta_function_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(meta_parameter_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(meta_variable_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(modifier_value)
	static const FogSemantics& namespace_alias_definition_value();
	FOG_SEMANTICS_MISSING_VALUE(namespace_definition_value)
	FOG_SEMANTICS_MISSING_VALUE(object_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(parameter_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(pointer_modifier_value)
	FOG_SEMANTICS_MISSING_VALUE(reference_modifier_value)
	FOG_SEMANTICS_MISSING_VALUE(scoped_modifier_value)
	FOG_SEMANTICS_MISSING_VALUE(specifier_value);
	FOG_SEMANTICS_MISSING_VALUE(template_parameter_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(templated_parameter_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(type_parameter_specifier_value)
	FOG_SEMANTICS_MISSING_VALUE(typedef_specifier_value)
	static const FogSemantics& using_directive_value();
	FOG_SEMANTICS_MISSING_VALUE(value_parameter_specifier_value)
};

#undef FOG_SEMANTICS_BIT
#undef FOG_SEMANTICS_BIT_TO_MASK

class FogSemantics : public PrimPrint,
	public FogSemantics_Expression,
	public FogSemantics_Declaration,
	public FogSemantics_Declarator,
	public FogSemantics_Meta,
	public FogSemantics_Auto,
	public FogSemantics_Name,
	public FogSemantics_Entity,
	public FogSemantics_Others,
	public FogSemantics_Specifier {
	typedef PrimPrint Super;
	typedef FogSemantics_Declaration SuperDeclaration;
	typedef FogSemantics_Declarator SuperDeclarator;
	typedef FogSemantics_Expression SuperExpr;
	typedef FogSemantics_Meta SuperMeta;
	typedef FogSemantics_Auto SuperAuto;
	typedef FogSemantics_Name SuperName;
	typedef FogSemantics_Entity SuperEntity;
	typedef FogSemantics_Specifier SuperSpecifier;
	typedef FogSemantics This;
	TYPEDECL_SINGLE(This, Super)
public:
	enum IsPotential { ACTUAL, POTENTIAL };
private:
	//    FogSemantics& operator=(const FogSemantics& aSemantics);    // Default ok.
	
public:
	FogSemantics() {}
	FogSemantics(Auto autoSemantics) : SuperAuto(autoSemantics) {}
	FogSemantics(Declaration declarationSemantics) : SuperDeclaration(declarationSemantics) {}
	FogSemantics(Declarator declaratorSemantics) : SuperDeclarator(declaratorSemantics) {}
	FogSemantics(Entity entitySemantics) : SuperEntity(entitySemantics) {}
	FogSemantics(Expression expressionSemantics) : SuperExpr(expressionSemantics) {}
	FogSemantics(Meta metaSemantics) : SuperMeta(metaSemantics) {}
	FogSemantics(Name nameSemantics) : SuperName(nameSemantics) {}
	FogSemantics(Specifier specifierSemantics) : SuperSpecifier(specifierSemantics) {}
	//    FogSemantics(const FogSemantics& aSemantics);               // Default ok.
	bool operator!() const {
		return SuperName::operator!() && SuperMeta::operator!() && SuperAuto::operator!()
		       && SuperExpr::operator!() && SuperDeclaration::operator!() && SuperDeclarator::operator!()
		       && SuperSpecifier::operator!();
	}
	void assign(Auto autoSemantics)
	{ reset(); SuperAuto::assign(autoSemantics); }
	void assign(Declaration declarationSemantics)
	{ reset(); SuperDeclaration::assign(declarationSemantics); }
	void assign(Declarator declaratorSemantics)
	{ reset(); SuperDeclarator::assign(declaratorSemantics); }
	void assign(Entity entitySemantics)
	{ reset(); SuperEntity::assign(entitySemantics); }
	void assign(Expression expressionSemantics)
	{ reset(); SuperExpr::assign(expressionSemantics); }
	void assign(Meta metaSemantics)
	{ reset(); SuperMeta::assign(metaSemantics); }
	void assign(Name nameSemantics)
	{ reset(); SuperName::assign(nameSemantics); }
	void assign(Specifier specifierSemantics)
	{ reset(); SuperSpecifier::assign(specifierSemantics); }
	void assign_identifier(IsPotential isPotential);
	bool is_bit_field_type_id() const { return is_bit_field_type_idName() || is_actual_type_name(); }
	bool is_class_name() const { return is_class_nameName() || is_actual_class_name(); }
	bool is_class_or_namespace_name() const { return is_class_name() || is_namespace_name(); }
	bool is_constructor_declarative() const
	{ return is_constructor_declarativeDeclaration() || is_constructor_name(); }
	bool is_constructor_declarator() const
	{ return is_constructor_declaratorDeclaration() || is_constructor_name(); }
	bool is_constructor_name() const { return is_class_name(); }
	bool is_constructor_type_id() const { return is_constructor_type_idDeclaration() || is_constructor_name(); }
	bool is_conversion_type_id() const { return is_conversion_type_idExpr() || is_type_specifier_seq(); }
	bool is_decl_specifier_seq() const { return is_decl_specifier_seqName() || is_actual_type_name(); }
	bool is_declaration() const {
		return is_declarationDeclaration() || is_declarationDeclarator()
		       || is_declarationName() || is_auto_function_definition() || is_auto_variable_declaration()
		       || is_constructor_name() || is_actual_type_name();
	}
	bool is_enumerator_definition() const { return is_enumerator_definitionExpr() || is_enumerator(); }
	bool is_function_declaration() const { return is_function_declarationDeclaration() || is_constructor_name(); }
	bool is_init_declarator_list() const { return is_init_declarator_listExpr() || is_init_declarator(); }
	bool is_mem_initializer_id() const
	{ return is_mem_initializer_idName() || is_identifier() || is_actual_class_name(); }
	bool is_member_id_expression() const { return is_member_id_expressionDeclarator() || is_pseudo_destructor_name(); }
	bool is_meta_ctor_name() const { return is_meta_ctor_nameMeta() || is_meta_name() || is_meta_type_name(); }
	bool is_meta_decl_specifier_seq() const
	{ return is_meta_decl_specifier_seqMeta() || is_meta_name() || is_meta_type_name(); }
	bool is_meta_declaration() const { return is_meta_function_definition() || is_meta_variable_declaration(); }
	bool is_meta_name() const { return is_meta_nameMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_meta_object_name() const
	{ return is_meta_object_nameMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_meta_parameter_declarator() const
	{ return is_meta_parameter_declaratorMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_meta_parameter_declarator_elem() const
	{ return is_meta_parameter_declarator_elemMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_meta_parameter_name() const
	{ return is_meta_parameter_nameMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_meta_type_name() const
	{ return is_meta_type_nameMeta() || is_meta_nameName() || is_actual_class_name() || is_actual_meta_type_name(); }
	bool is_meta_variable_declarator() const
	{ return is_meta_variable_declaratorMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_meta_variable_declarator_elem() const
	{ return is_meta_variable_declarator_elemMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_name_decl_specifier_seq() const { return is_name_decl_specifier_seqDeclarator() || is_actual_name(); }
	bool is_namespace_name() const { return is_namespace_nameName() || is_actual_namespace_name(); }
	bool is_parameter_declaration() const { return is_parameter_declarationName() || is_actual_type_name(); }
	bool is_parameter_type_id() const { return is_parameter_type_idName() || is_actual_type_name(); }
	bool is_scoped_class_name() const { return is_scoped_class_nameName() || is_actual_class_name(); }
	bool is_scoped_meta_name() const
	{ return is_scoped_meta_nameMeta() || is_meta_nameName() || is_actual_class_name(); }
	bool is_scoped_meta_type_name() const { return is_scoped_meta_type_nameMeta() || is_meta_name(); }
	bool is_scoped_type_name() const { return is_scoped_type_nameName() || is_actual_class_name(); }
	bool is_simple_declaration() const { return is_init_declarator_list() || is_init_declaration_list(); }
	bool is_simple_type_specifier() const { return is_simple_type_specifierName() || is_actual_class_name(); }
	bool is_template_argument() const
	{ return is_template_argumentDeclarator() || is_type_id() || is_template_name(); }
	bool is_template_argument_list() const { return is_template_argument_listDeclarator() || is_template_argument(); }
	bool is_template_parameter() const { return is_template_parameterName() || is_actual_type_name(); }
	bool is_template_parameter_list() const { return is_template_parameter_listName() || is_actual_type_name(); }
	bool is_token() const { return true; }
	bool is_type_declaration() const { return is_type_declarationName() || is_actual_class_name(); }
	bool is_type_id() const { return is_type_idName() || is_actual_type_name(); }
	bool is_type_name() const { return is_type_nameName() || is_actual_class_name(); }
	bool is_type_specifier() const { return is_type_specifierName() || is_actual_class_name(); }
	bool is_type_specifier_seq() const { return is_type_specifier_seqName() || is_actual_type_name(); }
	bool is_typed_decl_specifier_seq() const { return is_typed_decl_specifier_seqName() || is_actual_type_name(); }
	bool is_value_parameter() const { return is_parameter_declaration(); }
	virtual std::ostream& print_this(std::ostream& s) const;
	void reset() {
		SuperName::reset(); SuperMeta::reset(); SuperExpr::reset();
		SuperDeclaration::reset(); SuperDeclarator::reset(); SuperEntity::reset();
	}
	void set(Auto autoSemantics) { SuperAuto::set(autoSemantics); }
	void set(Declaration declarationSemantics) { SuperDeclaration::set(declarationSemantics); }
	void set(Declarator declaratorSemantics) { SuperDeclarator::set(declaratorSemantics); }
	void set(Entity entitySemantics) { SuperEntity::set(entitySemantics); }
	void set(Expression expressionSemantics) { SuperExpr::set(expressionSemantics); }
	void set(Meta metaSemantics) { SuperMeta::set(metaSemantics); }
	void set(Name nameSemantics) { SuperName::set(nameSemantics); }
	void set(Specifier specifierSemantics) { SuperSpecifier::set(specifierSemantics); }
private:
	friend void operator==(const FogSemantics& firstSemantics, const FogSemantics& secondSemantics);
};
#endif

