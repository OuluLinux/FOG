
#ifndef FOGPARSERVALUE_HXX
#define FOGPARSERVALUE_HXX

#define YACC_BANG_TYPE int
#define YACC_MARK_TYPE void *






typedef FogToken CxxToken;
typedef FogTokens CxxTokens;

typedef FogAccess CxxAccessSpecifier;
typedef FogBaseSpecifier CxxBaseSpecifier;
typedef FogBaseSpecifiers CxxBaseSpecifiers;
typedef FogBuiltInTypeId CxxBuiltInId;
typedef FogCharacter CxxCharacterLiteral;
typedef FogClassSpecifier CxxClass;
typedef FogTag CxxClassKey;
typedef FogExpr CxxCondition;
typedef FogCv CxxCvQualifiers;
typedef FogDeclSpecifier CxxDeclSpecifierId;
typedef FogRaw CxxDeclaration;
typedef FogRawDeclarations CxxDeclarations;
typedef FogModifier CxxDeclarator;
typedef FogDeleteExpr CxxDeleteExpression;
typedef FogEnumSpecifier CxxEnum;
typedef FogEnumeratorSpecifier CxxEnumerator;
typedef FogEnumeratorSpecifiers CxxEnumerators;
typedef FogExpr CxxExceptionDeclaration;
typedef FogListOfExpr CxxExceptionSpecification;
typedef FogExpr CxxExpression;
typedef FogListOfExpr CxxExpressions;
typedef FogFileId CxxFileId;
typedef FogFileIds CxxFileIds;
typedef FogFileIdent CxxFileName;
typedef FogNumber CxxFloatingLiteral;
typedef FogCode CxxFunctionBody;
typedef FogExpr CxxInitializerClause;
typedef FogCatchStatement CxxHandler;
typedef FogTokens CxxHandlers;
typedef FogKeyword CxxIdentifier;
typedef FogExpr CxxInitializerClause;
typedef FogListOfExpr CxxInitializerClauses;
typedef FogNumber CxxIntegerLiteral;
typedef FogKeyword CxxKeyword;
typedef FogLine CxxLine;
typedef FogRawCtorInit CxxMemInitializer;
typedef FogRawCtorInits CxxMemInitializers;
typedef FogRawDeclarations CxxMemberDeclarations;
typedef FogClassSpecifier CxxMetaClass;
typedef FogExpr CxxMetaFunction;
typedef FogExpr CxxMetaInitializer;
typedef FogListOfExpr CxxMetaInitializers;
typedef FogMetaObjectSpecifier CxxMetaObject;
typedef FogRaw CxxMetaStatement;
typedef FogMetaType CxxMetaType;
typedef FogExpr CxxMetaVariable;
typedef FogName CxxName;
typedef FogClassSpecifier CxxNamespace;
typedef FogScopeEnums::IsExposed CxxIsExposed;
typedef FogScopeEnums::IsTemplate CxxIsTemplate;
typedef FogNewExpr CxxNewExpression;
typedef FogNumber CxxNumberLiteral;
typedef FogExpr CxxParameter;
typedef FogListOfExpr CxxParameters;
typedef FogParenthesisExpr CxxParenthesised;
typedef FogPointerModifier CxxPointerDeclarator;
typedef FogPosition CxxPosition;
typedef FogScope CxxScope;
typedef FogSegment CxxSegment;
typedef FogExpr CxxSimpleTypeParameter;
typedef FogString CxxStringLiteral;
typedef FogRaw CxxStatement;
typedef FogRawStatements CxxStatements;
//  typedef FogExpr CxxStrings;
typedef FogString CxxStrings;
typedef FogRawSubspace CxxSubspace;
typedef CxxToken CxxSyntaxMacroParameter;
typedef CxxTokens CxxSyntaxMacroParameters;
typedef FogExpr CxxTemplateArgument;
typedef FogListOfExpr CxxTemplateArguments;
typedef FogExpr CxxTemplateParameter;
typedef FogListOfExpr CxxTemplateParameters;
typedef FogExpr CxxTemplatedTypeParameter;
typedef FogToken CxxTreeArgument;
typedef FogTokenStatements CxxTokenStatements;
typedef FogTokens CxxTreeArguments;
typedef FogExpr CxxTreeExpression;
typedef FogListOfExpr CxxType1Parameters;
typedef FogUtility CxxUtility;

#define FOGPARSERVALUE_ENUM(T,N) \
 const T *name2(u_,N); \
 const T& N() const { return *name2(u_,N); } \
 const T* & N() { return name2(u_,N); }
#define FOGPARSERVALUE_POINTER(T,N) T *N;
#define FOGPARSERVALUE_VALUE(T,N) T N;

union FogParserValue
{
  FogToken *_token;

  FOGPARSERVALUE_VALUE(bool, _bool)
  FOGPARSERVALUE_VALUE(long, _long)
  FOGPARSERVALUE_POINTER(FogBrace, brace)
  FOGPARSERVALUE_POINTER(FogSpacing, spacing)

  FOGPARSERVALUE_POINTER(CxxAccessSpecifier, access_specifier)
  FOGPARSERVALUE_POINTER(CxxBaseSpecifier, base_specifier)
  FOGPARSERVALUE_POINTER(CxxBaseSpecifiers, base_specifiers)
  FOGPARSERVALUE_POINTER(CxxBuiltInId, built_in_id)
  FOGPARSERVALUE_POINTER(CxxCharacterLiteral, character_literal)
  FOGPARSERVALUE_POINTER(CxxClass, _class)
  FOGPARSERVALUE_POINTER(CxxClassKey, class_key)
  FOGPARSERVALUE_POINTER(CxxCondition, condition)
  FOGPARSERVALUE_POINTER(CxxCvQualifiers, cv_qualifiers)
  FOGPARSERVALUE_POINTER(CxxDeclSpecifierId, decl_specifier_id)
  FOGPARSERVALUE_POINTER(CxxDeclaration, declaration)
  FOGPARSERVALUE_POINTER(CxxDeclarations, declarations)
  FOGPARSERVALUE_POINTER(CxxDeclarator, declarator)
  FOGPARSERVALUE_POINTER(CxxDeleteExpression, delete_expression)
  FOGPARSERVALUE_POINTER(CxxEnumerator, enumerator)
  FOGPARSERVALUE_POINTER(CxxEnumerators, enumerators)
  FOGPARSERVALUE_POINTER(CxxExceptionDeclaration, exception_declaration)
  FOGPARSERVALUE_POINTER(CxxExceptionSpecification, exception_specification)
  FOGPARSERVALUE_POINTER(CxxExpression, expression)
  FOGPARSERVALUE_POINTER(CxxExpressions, expressions)
  FOGPARSERVALUE_POINTER(CxxFileId, file_id)
  FOGPARSERVALUE_POINTER(CxxFileIds, file_ids)
  FOGPARSERVALUE_POINTER(CxxFileName, file_name)
  FOGPARSERVALUE_POINTER(CxxFloatingLiteral, floating_literal)
  FOGPARSERVALUE_POINTER(CxxFunctionBody, function_body)
  FOGPARSERVALUE_POINTER(CxxHandler, handler)
  FOGPARSERVALUE_POINTER(CxxHandlers, handlers)
  FOGPARSERVALUE_POINTER(CxxIdentifier, identifier)
  FOGPARSERVALUE_POINTER(CxxInitializerClause, initializer_clause)
  FOGPARSERVALUE_POINTER(CxxInitializerClauses, initializer_clauses)
  FOGPARSERVALUE_POINTER(CxxIntegerLiteral, integer_literal)
  FOGPARSERVALUE_POINTER(CxxKeyword, keyword)
  FOGPARSERVALUE_POINTER(CxxLine, line)
  FOGPARSERVALUE_POINTER(CxxMemInitializer, mem_initializer)
  FOGPARSERVALUE_POINTER(CxxMemInitializers, mem_initializers)
  FOGPARSERVALUE_POINTER(CxxMemberDeclarations, member_declarations)
  FOGPARSERVALUE_POINTER(CxxMetaClass, meta_class)
  FOGPARSERVALUE_POINTER(CxxMetaFunction, meta_function)
  FOGPARSERVALUE_POINTER(CxxMetaInitializer, meta_initializer)
  FOGPARSERVALUE_POINTER(CxxMetaInitializers, meta_initializers)
  FOGPARSERVALUE_POINTER(CxxMetaObject, meta_object)
  FOGPARSERVALUE_POINTER(CxxMetaStatement, meta_statement)
  FOGPARSERVALUE_POINTER(CxxMetaType, meta_type)
  FOGPARSERVALUE_POINTER(CxxMetaVariable, meta_variable)
  FOGPARSERVALUE_POINTER(CxxName, name)
  FOGPARSERVALUE_POINTER(CxxNewExpression, new_expression)
  FOGPARSERVALUE_POINTER(CxxNumberLiteral, number_literal)
  FOGPARSERVALUE_POINTER(CxxParameter, parameter)
  FOGPARSERVALUE_POINTER(CxxParameters, parameters)
  FOGPARSERVALUE_POINTER(CxxParenthesised, parenthesised)
  FOGPARSERVALUE_POINTER(CxxPointerDeclarator, pointer_declarator)
  FOGPARSERVALUE_POINTER(CxxPosition, position)
  FOGPARSERVALUE_POINTER(CxxSegment, segment)
  FOGPARSERVALUE_POINTER(CxxSimpleTypeParameter, simple_type_parameter)
  FOGPARSERVALUE_POINTER(CxxStatement, statement)
  FOGPARSERVALUE_POINTER(CxxStatements, statements)
  FOGPARSERVALUE_POINTER(CxxStringLiteral, string_literal)
  FOGPARSERVALUE_POINTER(CxxStrings, strings)
  FOGPARSERVALUE_POINTER(CxxSubspace, subspace)
  FOGPARSERVALUE_POINTER(CxxSyntaxMacroParameter, syntax_macro_parameter)
  FOGPARSERVALUE_POINTER(CxxSyntaxMacroParameters, syntax_macro_parameters)
  FOGPARSERVALUE_POINTER(CxxTemplateArgument, template_argument)
  FOGPARSERVALUE_POINTER(CxxTemplateArguments, template_arguments)
  FOGPARSERVALUE_POINTER(CxxTemplateParameter, template_parameter)
  FOGPARSERVALUE_POINTER(CxxTemplateParameters, template_parameters)
  FOGPARSERVALUE_POINTER(CxxTemplatedTypeParameter, templated_type_parameter)
  FOGPARSERVALUE_POINTER(CxxToken, token)
  FOGPARSERVALUE_POINTER(CxxTokenStatements, token_statements)
  FOGPARSERVALUE_POINTER(CxxTokens, tokens)
  FOGPARSERVALUE_POINTER(CxxTreeArgument, tree_argument)
  FOGPARSERVALUE_POINTER(CxxTreeArguments, tree_arguments)
  FOGPARSERVALUE_POINTER(CxxTreeExpression, tree_expression)
  FOGPARSERVALUE_POINTER(CxxType1Parameters, type1_parameters)
  FOGPARSERVALUE_POINTER(CxxUtility, utility)

  FOGPARSERVALUE_VALUE(int, bang)
  FOGPARSERVALUE_VALUE(CxxIsTemplate, is_template)
  FOGPARSERVALUE_VALUE(void *, mark)
  FOGPARSERVALUE_VALUE(size_t, nest)
};
#endif

