# C FOG Grammar

The complete FOG grammar implemented using the superset approach outlined for FOG is presented in this appendix.

The presented grammar has been derived automatically from FogParser.y by a sed script to

- remove C++ speciﬁc grammar

- remove irrelevant action rules

- simplify relevant action rules

```
•
remove implementation speciﬁc $ clutter
```

The resulting text is acceptable to yacc and has 0 unresolved conﬂicts.

It is available from

http://www.ee.surrey.ac.uk/Research/CSRG/fog/FogGrammar.y

```
/* This is a yacc-able parser for the entire FOG grammar with no unresolved conflicts. */
/* The parse is SYNTACTICALLY consistent and requires no template or type name assistance.
* The grammar in the C++ standard notes that its grammar is a superset of the true
* grammar requiring semantic constraints to resolve ambiguities. This grammar is a really big
* superset unifying expressions and declarations, eliminating the type/non-type distinction,
* and iterating to find a consistent solution to the template/arith,metoic < ambiguity.
* As a result the grammar is much simpler, but requires the missing semantic constraints to be
* performed in a subsequent semantic pass, which is of course where they belong. This grammar will
* support conversion of C++ tokens into an Abstract Syntax Tree. A lot of further work is required to
* The principles behind this grammar are described in my thesis on Meta-Compilation for C++, which
* may be found via http://www.computing.surrey.ac.uk/research/dsrg/fog/FogThesis.html.
*  Author:         E.D.Willink             Ed.Willink@rrl.co.uk
* The lexer (and/or a preprocessor) is expected to identify the following
%type <keyword> ‘+’ ‘-’ ‘*’ ‘/’ ‘%’ ‘^’ ‘&’ ‘|’ ‘~’ ‘!’ ‘<‘  ‘>’ ‘=’ ‘:’ ‘[‘ ‘]’ ‘{‘ ‘}’ ‘(‘ ‘)’
%type <keyword> ‘?’ ‘.’ ‘\’’ ‘\”’ ‘\\’ ‘@’ ‘$’ ‘;’ ‘,’
%term <keyword> ARROW ARROW_STAR DEC EQ GE INC LE LOG_AND LOG_OR NE SHL SHR
%term <keyword> ASS_ADD ASS_AND ASS_DIV ASS_MOD ASS_MUL ASS_OR ASS_SHL ASS_SHR ASS_SUB ASS_XOR
%term <access_specifier> PRIVATE PROTECTED PUBLIC
%term <built_in_id> BOOL CHAR DOUBLE FLOAT INT LONG SHORT SIGNED UNSIGNED VOID WCHAR_T
%term <class_key> CLASS ENUM NAMESPACE STRUCT TYPENAME UNION
%term <decl_specifier_id> AUTO EXPLICIT EXPORT EXTERN FRIEND INLINE MUTABLE REGISTER STATIC TEMPLATE TYPEDEF USING VIRTUAL
%term <keyword> ASM BREAK CASE CATCH CONST_CAST CONTINUE DEFAULT DELETE DO DYNAMIC_CAST
%term <keyword> ELSE FALSE FOR GOTO IF NEW OPERATOR REINTERPRET_CAST RETURN
%term <keyword> SIZEOF STATIC_CAST SWITCH THIS THROW TRUE TRY TYPEID WHILE
*  FOG non-reserved word identifier extensions
%term <name> DERIVED FILE GUARD IMPLEMENTATION INCLUDE INTERFACE
%term <name> NOGUARD NOIMPLEMENTATION OVERLOAD PATH
*  The lexer need not treat ‘0’ as distinct from IntegerLiteral in the hope that pure-specifier can
*  be distinguished, It isn’t. Semantic rescue from = constant-expression is necessary.
*  The lexer is not required to distinguish template or type names, although a slight simplification to the
*  grammar and elaboration of the action rules could make good use of template name information.
*  In return for not needing to use semantic information, the lexer must support back-tracking, which
*  is easily achieved by a simple linear buffer, a reference implementation of which may be found in the
*  accompanying CxxParsing.cxx. Back-tracking is used to support:
*  Binary search for a consistent parse of the template/arithmetic ambiguity.
*      template_test() maintains context during a search
*  Lookahead to resolve the inheritance/anonymous bit-field similarity
*      rewind_colon() restores the context and forces the missing :
*  Lookahead to resolve type 1 function parameter ambiguities
*      mark_type1() potentially marks the starting position
*      remark() rewinds to the starting position
*  The parsing philosophy is unusual. The major ambiguities are resolved by creating a unified superset
*  grammar rather than non-overlapping subgrammars. Thus the grammar for parameter-declaration covers an
*  assignment-expression. Minor ambiguities whose resolution by supersetting would create more
*  ambiguities are resolved the normal way with partitioned subgrammars.
*  This eliminates the traditional expression/declaration and constructor/parenthesised declarator
*  ambiguities at the syntactic level. A subsequent semantic level has to sort the problems out.
*  The generality introduces four bogus ambiguities and defers the cast ambiguity for resolution
*  The FOG grammar comprises 958 rules and uses 1585 states in yacc, with 0 unresolved conflicts.
*  31 conflicts from 15 ambiguities are resolved by 15 %prec’s, so that yacc and bison report 0 conflicts.
*  1) dangling else resolved to inner-most if
*  3) a :: b :: c resolved to favour a::b::c rather than a::b ::c or a ::b::c
*  4) pointer operators maximised at end of conversion id/new in preference to binary operators
*  5a) (a)@b resolved to favour binary a@b rather than cast unary (a)(@b)
*  5b) (a)(b) resolved to favour cast rather than call
*      8 conflicts in 1 state for the 8 prefix operators: 6 unaries and ( and [.
*  6) enum name { resolved to enum-specifier rather than function
*  7) class name { resolved to class-specifier rather than function
*  8) extern “C” resolved to linkage-specification rather than declaration
*      1 conflict in 1 state on StringLiteral
*  9) class X : forced to go through base-clause look-ahead
*  10) id : forced to label_statement rather than constructor_head
*  11) access-specifier : forced to access-declaration rather than anon bit-field
*  12) inline/ and virtual/ forced to switch rather than divide treatment
*  13) using StringLiteral forced to include_declaration not simple_declaration
*      1 conflict in 1 states on StringLiteral
*  14) handler_seq maximised avoiding ambiguity in compound_tree_statement
*  15) built_in_type_id maximised resolving ambiguity for auto unsigned int :: a
*      1 conflict in 1 states on BuiltInTypeSpecifier
*      1 is a fundamental C conflict - always correctly resolved
*          can be removed - see the Java spec
*          2 always consistently resolved by iteration
*      5 is a result of not using type information - deferred for semantic repair
*      6,7 are caused by parsing over-generous superset - always correctly resolved
*      8 is caused by parsing over-generous superset - always correctly resolved
*          can be removed at the expense of 7 rules and 5 states.
*      9 is a look-ahead trick - always correctly resolved
*          could be removed by marking one token sooner
*      10 is caused by parsing over-generous superset - always correctly resolved
*      11 is caused by parsing over-generous superset - always correctly resolved
*      12 is caused by parsing over-generous superset - always correctly resolved
*      13 is caused by parsing over-generous superset - always correctly resolved
*      14 is a genuine conflict - always correctly resolved by definition
*          more enthusiastic parsing of the } or ; statement end could fix this
*      15 is a fundamental FOG conflict comparable to 3
*      class A { class B : C, D, E {           -- A::B privately inherits C, D and E
*      class A { class B : C, D, E ;           -- C is width of anon bit-field
*  is resolved by using a lookahead that assumes inheritance and rewinds for the bit-field.
*  The potential shift-reduce conflict on > is resolved by flattening part of the expression grammar
*  to know when the next > is template end or arithmetic >.
*  The grammar is SYNTACTICALLY context-free with respect to type. No semantic assistance is required
*  during syntactic analysis. However the cast ambiguity is deferred and must be recovered
*  after syntactic analysis of a statement has completed.
*  The grammar is SYNTACTICALLY context-free with respect to template-names. This is achieved by
*  organising a binary search over all possible template/arithmetic ambiguities with respect to
*  the enclosing statement. This is potentially exponentially inefficient but well-behaved in practice.
*  Approximately 1% of statements trigger a search and approximately 1% of those are misparsed,
*  requiring the semantic analysis to check and correct once template information is available.
*  1.5 parse attempts are required on average per ambiguous statement.
*  The grammar supports type I function declarations at severe impediment to efficiency. A lookahead
*  has to be performed after almost every non-statement close parenthesis. A one-line plus corollary
*  change to postfix_expression is commented and strongly recommended to make this grammar as
*  efficient as the rather large number of reduction levels permits.
*  Error recovery occurs mostly at the statement/declaration level. Recovery also occurs at
*  the list-element level where this poses no hazard to statement/declaration level recovery.
*  Note that since error propagation interacts with the lookaheads for template iteration or
*  type 1 function arguments, introduction of finer grained error recovery may repair a false
*  The following syntactic analysis errors occur, but are correctable semantically:
*  (cast)unary-op expr         is parsed as (parenthesised)binary-op expr
*      The semantic test should look for a binary/call with a (type) as its left child.
*  (parenthesised)(arguments)  is parsed as (cast)(parenthesised)
*      The semantic test should look for a cast with a non-type as its left child.
*  template < and arithmetic < may be cross-parsed (unless semnatic help is provided)
*      approximately 0.01% are misparsed, and must be sorted out - not easy.
*  The syntactic analysis defers the following ambiguities for semantic resolution:
*  declaration/expression is parsed as a unified concept
*      Use type and context to complete the parse.
*  ~class-name                 is parsed as unary~ name
*      The semantic test should look for ~ with a type as its child.
*  delete[] expr               is parsed as delete []expr
*      The semantic test should look for delete with a [] cast of its child.
*  operator new/delete[]       are parsed as array of operator new/delete
*      The semantic test should look for array of operator new/delete
*      or activate the two extra commented rules in operator
*  template of an explicit_instantiation is buried deep in the tree
*  pure-specifier and constant-initializer are covered by assignment-expression
*      just another of the deferred declaration/expression ambiguities
*  sizeof and typeid don’t distinguish type/value syntaxes
*      probably makes life polymorphically easier
%nonassoc SCOPE ELSE INC DEC ‘+’ ‘-’ ‘*’ ‘&’ ‘[‘ ‘{‘ ‘<‘ ‘:’ StringLiteral
*  The %prec resolves a conflict in identifier_word : which is forced to be a shift of a label for
*  a labeled-statement rather than a reduction for the name of a bit-field or generalised constructor.
*  This is pretty dubious syntactically but correct for all semantic possibilities.
*  The shift is only activated when the ambiguity exists at the start of a statement. In this context
*  a bit-field declaration or constructor definition are not allowed.
identifier_word:                    Identifier
|                               DERIVED | FILE | GUARD | IMPLEMENTATION
|                               INCLUDE | INTERFACE | NOGUARD | NOIMPLEMENTATION
|                               OVERLOAD | PATH | PREFIX | PURE | SUFFIX
identifier:                         identifier_word                     %prec SHIFT_THERE
|                               TreeLiteral
*  Identifier ‘<‘ is forced to go through the is-it-a-template-name test
*  All names absorb TEMPLATE with the name, so that no template_test is performed for them.
*  This requires all potential declarations within an expression to perpetuate this policy
*  and thereby guarantee the ultimate coverage of explicit_instantiation.
id:                                 identifier                          %prec SHIFT_THERE       /* Force < through test */
|                               identifier template_test ‘+’ template_argument_list ‘>’
|                               identifier template_test ‘+’ ‘>’                            { ERRMSG(“Empty template-argument-list”); }
|                               identifier template_test ‘-’                                /* requeued < follows */
|                               template_id
template_test:                      ‘<‘             /* Queue ‘+’ or ‘-’ < as follow on */       { template_test(); }
global_scope:                       SCOPE                                                       { IS_DEFAULT; }
|                               TEMPLATE global_scope                                       { IS_TEMPLATE; }
*  A :: B :: C; is ambiguous How much is type and how much name ?
*  The %prec maximises the (type) length which is the 7.1-2 semantic constraint.
nested_id:                          id                                  %prec SHIFT_THERE       /* Maximise length */
|                               id_scope nested_id
|                               global_scope nested_id
*  destructor_id has to be held back to avoid a conflict with a one’s complement as per 5.3.1-9,
*  It gets put back only when scoped or in a declarator_id, which is only used as an explicit member name.
*  Declarations of an unscoped destructor are always parsed as a one’s complement.
|                               TEMPLATE destructor_id
special_function_id:                conversion_function_id
|                               operator_function_id
|                               TEMPLATE special_function_id
nested_special_function_id:         special_function_id
|                               id_scope destructor_id
|                               id_scope nested_special_function_id
scoped_special_function_id:         nested_special_function_id
|                               global_scope nested_special_function_id
/* declarator-id is all names in all scopes, except reserved words */
|                               scoped_special_function_id
|                               destructor_id
/*  The standard defines pseudo-destructors in terms of type-name, which is class/enum/typedef, of which
*  class-name is covered by a normal destructor. pseudo-destructors are supposed to support ~int() in
*  templates, so the grammar here covers built-in names. Other names are covered by the lack of
built_in_type_id:                   built_in_type_specifier
|                               built_in_type_id built_in_type_specifier
pseudo_destructor_id:               built_in_type_id SCOPE ‘~’ built_in_type_id
|                               ‘~’ built_in_type_id
|                               TEMPLATE pseudo_destructor_id
nested_pseudo_destructor_id:        pseudo_destructor_id
|                               id_scope nested_pseudo_destructor_id
scoped_pseudo_destructor_id:        nested_pseudo_destructor_id
|                               global_scope scoped_pseudo_destructor_id
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
*  String concatenation is a phase 6, not phase 7 activity so does not really belong in the grammar.
*  However it may be convenient to have it here to make this grammar fully functional.
*  Unfortunately it introduces a conflict with the generalised parsing of extern “C” which
*  is correctly resolved to maximise the string length as the token source should do anyway.
string:                             StringLiteral
/*string:                           StringLiteral                           %prec SHIFT_THERE */
/*  |                               StringLiteral string  -- Perverse order avoids conflicts -- */
literal:                            IntegerLiteral
|                               CharacterLiteral
|                               FloatingLiteral
|                               boolean_literal
|                               NumberLiteral
|                               TreeLiteral
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
translation_unit:                   declaration_seq.opt
|                               ‘$’ tree_expression                                         { YYACCEPT; }
|                               ‘$’ ‘{‘ tree_expression ecarb                               { YYACCEPT; }
|                               ‘$’ bang error                                              { UNBANG(“Bad tree-expression.”); YYABORT; }
|                               ‘$’ ‘{‘ bang error ecarb                                    { UNBANG(“Bad tree-expression}.”); YYABORT; }
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------
*  primary_expression covers an arbitrary sequence of all names with the exception of an unscoped destructor,
*  which is parsed as its unary expression which is the correct disambiguation (when ambiguous).
*  This eliminates the traditional A(B) meaning A B ambiguity, since we never have to tack an A onto
*  the front of something that might start with (. The name length got maximised ab initio. The downside
*  is that semantic interpretation must split the names up again.
*  Unification of the declaration and expression syntax means that unary and binary pointer declarator operators:
*  are parsed as binary and unary arithmetic operators (int) * (*name). Since type information is not used
*  are resolved to favour the binary rather than the cast unary to ease AST clean-up.
*  The cast-call ambiguity must be resolved to the cast to ensure that (a)(b)c can be parsed.
*  The problem of the functional cast ambiguity
*  as call or declaration is avoided by maximising the name within the parsing kernel. So
*  as an assignment to the syntax parsed as “extern long int const var”. The presence of two names is
*  parsed so that “extern long into const” is distinguished from “var” considerably simplifying subsequent
*  The generalised name is a concatenation of potential type-names (scoped identifiers or built-in sequences)
*  plus optionally one of the special names such as an operator-function-id, conversion-function-id or
|                               suffix_decl_specified_ids
/*  |                               SCOPE identifier                                        -- covered by suffix_decl_specified_ids */
/*  |                               SCOPE operator_function_id                              -- covered by suffix_decl_specified_ids */
/*  |                               SCOPE qualified_id                                      -- covered by suffix_decl_specified_ids */
|                               abstract_expression           %prec REDUCE_HERE_MOSTLY  /* Prefer binary to unary ops, cast to call */
/*  |                               id_expression                                           -- covered by suffix_decl_specified_ids */
*  Abstract-expression covers the () and [] of abstract-declarators.
abstract_expression:                parenthesis_clause
|                               ‘[‘ expression.opt ‘]’
|                               TEMPLATE parenthesis_clause
/*  Type I function parameters are ambiguous with respect to the generalised name, so we have to do a lookahead following
*  any function-like parentheses. This unfortunately hits normal code, so kill the -- lines and add the ++ lines for efficiency.
*  Supporting Type I code under the superset causes perhaps 25% of lookahead parsing. Sometimes complete class definitions
*  get traversed since they are valid generalised type I parameters!
type1_parameters:       /*----*/    parameter_declaration_list ‘;’
|                   /*----*/    type1_parameters parameter_declaration_list ‘;’
mark_type1:                         /* empty */                                             { mark_type1(); yyclearin; }
postfix_expression:                 primary_expression
/*  |                   /++++++/    postfix_expression parenthesis_clause */
|                   /*----*/    postfix_expression parenthesis_clause mark_type1 ‘-’
|                   /*----*/    postfix_expression parenthesis_clause mark_type1 ‘+’ type1_parameters mark ‘{‘ error
/*----*/                    { yyerrok; yyclearin; remark_type1(); unmark(); unmark(); }
|                   /*----*/    postfix_expression parenthesis_clause mark_type1 ‘+’ type1_parameters mark error
/*----*/                    { yyerrok; yyclearin; remark_type1(); unmark(); unmark(); }
|                   /*----*/    postfix_expression parenthesis_clause mark_type1 ‘+’ error
/*----*/                    { yyerrok; yyclearin; remark_type1(); unmark(); }
|                               postfix_expression ‘[‘ expression.opt ‘]’
/*  |                               destructor_id ‘[‘ expression.opt ‘]’                    -- not semantically valid */
/*  |                               destructor_id parenthesis_clause                        -- omitted to resolve known ambiguity */
/*  |                               simple_type_specifier ‘(‘ expression_list.opt ‘)’       -- simple_type_specifier is a primary_expression */
|                               postfix_expression ‘.’ declarator_id
/*  |                               postfix_expression ‘.’ TEMPLATE declarator_id           -- TEMPLATE absorbed into declarator_id. */
|                               postfix_expression ‘.’ scoped_pseudo_destructor_id
|                               postfix_expression ARROW declarator_id
/*  |                               postfix_expression ARROW TEMPLATE declarator_id         -- TEMPLATE absorbed into declarator_id. */
|                               postfix_expression ARROW scoped_pseudo_destructor_id
|                               postfix_expression INC
|                               postfix_expression DEC
|                               DYNAMIC_CAST ‘<‘ type_id ‘>’ ‘(‘ expression ‘)’
|                               STATIC_CAST ‘<‘ type_id ‘>’ ‘(‘ expression ‘)’
|                               REINTERPRET_CAST ‘<‘ type_id ‘>’ ‘(‘ expression ‘)’
|                               CONST_CAST ‘<‘ type_id ‘>’ ‘(‘ expression ‘)’
|                               TYPEID parameters_clause
/*  |                               TYPEID ‘(‘ expression ‘)’                               -- covered by parameters_clause */
/*  |                               TYPEID ‘(‘ type_id ‘)’                                  -- covered by parameters_clause */
expression_list.opt:                /* empty */
|                               expression_list
expression_list:                    assignment_expression
|                               expression_list ‘,’ assignment_expression
unary_expression:                   postfix_expression
|                               INC cast_expression
|                               DEC cast_expression
|                               ptr_operator cast_expression
/*  |                               ‘*’ cast_expression                                     -- covered by ptr_operator */
/*  |                               ‘&’ cast_expression                                     -- covered by ptr_operator */
/*  |                               decl_specifier_seq ‘*’ cast_expression                  -- covered by binary operator */
/*  |                               decl_specifier_seq ‘&’ cast_expression                  -- covered by binary operator */
|                               suffix_decl_specified_scope star_ptr_operator cast_expression   /* covers e.g int ::type::* const t = 4 */
|                               ‘+’ cast_expression
|                               ‘-’ cast_expression
|                               ‘!’ cast_expression
|                               ‘~’ cast_expression
|                               SIZEOF unary_expression
/*  |                               SIZEOF ‘(‘ type_id ‘)’                                  -- covered by unary_expression */
|                               new_expression
|                               global_scope new_expression
|                               delete_expression
|                               global_scope delete_expression
/*  |                               DELETE ‘[‘ ‘]’ cast_expression       -- covered by DELETE cast_expression since cast_expression covers ... */
/*  |                               SCOPE DELETE ‘[‘ ‘]’ cast_expression //  ... abstract_expression cast_expression and so [] cast_expression */
delete_expression:                  DELETE cast_expression                                  /* also covers DELETE[] cast_expression */
new_expression:                     NEW new_type_id new_initializer.opt
|                               NEW parameters_clause new_type_id new_initializer.opt
|                               NEW parameters_clause
/*  |                               NEW ‘(‘ type-id ‘)’                                     -- covered by parameters_clause */
|                               NEW parameters_clause parameters_clause new_initializer.opt
/*  |                               NEW ‘(‘ type-id ‘)’ new_initializer                     -- covered by parameters_clause parameters_clause */
/*  |                               NEW parameters_clause ‘(‘ type-id ‘)’                   -- covered by parameters_clause parameters_clause */
/* ptr_operator_seq.opt production reused to save a %prec */
new_type_id:                        type_specifier ptr_operator_seq.opt
|                               type_specifier new_declarator
|                               type_specifier new_type_id
new_declarator:                     ptr_operator new_declarator
|                               direct_new_declarator
direct_new_declarator:              ‘[‘ expression ‘]’
|                               direct_new_declarator ‘[‘ constant_expression ‘]’
new_initializer.opt:                /* empty */
|                               ‘(‘ expression_list.opt ‘)’
/*  cast-expression is generalised to support a [] as well as a () prefix. This covers the omission of DELETE[] which when
*  followed by a parenthesised expression was ambiguous. It also covers the gcc indexed array initialisation for free.
cast_expression:                    unary_expression
|                               abstract_expression cast_expression
/*  |                               ‘(‘ type_id ‘)’ cast_expression                             -- covered by abstract_expression */
pm_expression:                      cast_expression
|                               pm_expression DOT_STAR cast_expression
|                               pm_expression ARROW_STAR cast_expression
multiplicative_expression:          pm_expression
|                               multiplicative_expression star_ptr_operator pm_expression
|                               multiplicative_expression ‘/’ pm_expression
|                               multiplicative_expression ‘%’ pm_expression
additive_expression:                multiplicative_expression
|                               additive_expression ‘+’ multiplicative_expression
|                               additive_expression ‘-’ multiplicative_expression
shift_expression:                   additive_expression
|                               shift_expression SHL additive_expression
|                               shift_expression SHR additive_expression
relational_expression:              shift_expression
|                               relational_expression ‘<‘ shift_expression
|                               relational_expression ‘>’ shift_expression
|                               relational_expression LE shift_expression
|                               relational_expression GE shift_expression
equality_expression:                relational_expression
|                               equality_expression EQ relational_expression
|                               equality_expression NE relational_expression
and_expression:                     equality_expression
|                               and_expression ‘&’ equality_expression
exclusive_or_expression:            and_expression
|                               exclusive_or_expression ‘^’ and_expression
inclusive_or_expression:            exclusive_or_expression
|                               inclusive_or_expression ‘|’ exclusive_or_expression
logical_and_expression:             inclusive_or_expression
|                               logical_and_expression LOG_AND inclusive_or_expression
logical_or_expression:              logical_and_expression
|                               logical_or_expression LOG_OR logical_and_expression
conditional_expression:             logical_or_expression
|                               logical_or_expression ‘?’ expression ‘:’ assignment_expression
/*  assignment-expression is generalised to cover the simple assignment of a braced initializer in order to contribute to the
*  coverage of parameter-declaration and init-declaration.
assignment_expression:              conditional_expression
|                               logical_or_expression assignment_operator assignment_expression
|                               logical_or_expression ‘=’ braced_initializer
|                               throw_expression
assignment_operator:                ‘=’ | ASS_ADD | ASS_AND | ASS_DIV | ASS_MOD | ASS_MUL | ASS_OR | ASS_SHL | ASS_SHR | ASS_SUB | ASS_XOR
/*  expression is widely used and usually single-element, so the reductions are arranged so that a
*  single-element expression is returned as is. Multi-element expressions are parsed as a list that
*  may then behave polymorphically as an element or be compacted to an element. */
expression.opt:                     /* empty */
|                               expression
expression:                         assignment_expression
|                               expression_list ‘,’ assignment_expression
constant_expression:                conditional_expression
/*  The grammar is repeated for when the parser stack knows that the next > must end a template.
templated_relational_expression:    shift_expression
|                               templated_relational_expression ‘<‘ shift_expression
|                               templated_relational_expression LE shift_expression
|                               templated_relational_expression GE shift_expression
templated_equality_expression:      templated_relational_expression
|                               templated_equality_expression EQ templated_relational_expression
|                               templated_equality_expression NE templated_relational_expression
templated_and_expression:           templated_equality_expression
|                               templated_and_expression ‘&’ templated_equality_expression
templated_exclusive_or_expression:  templated_and_expression
|                               templated_exclusive_or_expression ‘^’ templated_and_expression
templated_inclusive_or_expression:  templated_exclusive_or_expression
|                               templated_inclusive_or_expression ‘|’ templated_exclusive_or_expression
templated_logical_and_expression:   templated_inclusive_or_expression
|                               templated_logical_and_expression LOG_AND templated_inclusive_or_expression
templated_logical_or_expression:    templated_logical_and_expression
|                               templated_logical_or_expression LOG_OR templated_logical_and_expression
templated_conditional_expression:   templated_logical_or_expression
|                               templated_logical_or_expression ‘?’ templated_expression ‘:’ templated_assignment_expression
templated_assignment_expression:    templated_conditional_expression
|                               templated_logical_or_expression assignment_operator templated_assignment_expression
|                               templated_throw_expression
templated_expression:               templated_assignment_expression
|                               templated_expression_list ‘,’ templated_assignment_expression
templated_expression_list:          templated_assignment_expression
|                               templated_expression_list ‘,’ templated_assignment_expression
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------
*  Parsing statements is easy once simple_declaration has been generalised to cover expression_statement.
looping_statement:                  start_search looped_statement                               { end_search(); }
|                               advance_search ‘+’ looped_statement
|                               advance_search ‘-’
statement:                          control_statement
/*  |                               expression_statement                                        -- covered by declaration_statement */
|                               compound_statement
|                               declaration_statement
|                               AUTO control_statement
|                               AUTO meta_expression_statement
control_statement:                  labeled_statement
|                               selection_statement
|                               iteration_statement
|                               jump_statement
labeled_statement:                  identifier_word ‘:’ looping_statement
|                               CASE constant_expression ‘:’ looping_statement
|                               DEFAULT ‘:’ looping_statement
/*expression_statement:             expression.opt ‘;’                                          -- covered by declaration_statement */
compound_statement:                 ‘{‘ statement_seq.opt ‘}’
|                               ‘{‘ statement_seq.opt looping_statement ‘#’ bang error ‘}’  { UNBANG(“Bad statement-seq.”); }
statement_seq.opt:                  /* empty */
|                               statement_seq.opt looping_statement
|                               statement_seq.opt looping_statement ‘#’ bang error ‘;’      { UNBANG(“Bad statement.”); }
*  The dangling else conflict is resolved to the innermost if.
selection_statement:                IF ‘(‘ condition ‘)’ looping_statement    %prec SHIFT_THERE
|                               IF ‘(‘ condition ‘)’ looping_statement ELSE looping_statement
|                               SWITCH ‘(‘ condition ‘)’ looping_statement
condition.opt:                      /* empty */
condition:                          parameter_declaration_list
/*  |                               expression                                                  -- covered by parameter_declaration_list */
/*  |                               type_specifier_seq declarator ‘=’ assignment_expression     -- covered by parameter_declaration_list */
iteration_statement:                WHILE ‘(‘ condition ‘)’ looping_statement
|                               DO looping_statement WHILE ‘(‘ expression ‘)’ ‘;’
|                               FOR ‘(‘ for_init_statement condition.opt ‘;’ expression.opt ‘)’ looping_statement
for_init_statement:                 simple_declaration
/*  |                               expression_statement                                        -- covered by simple_declaration */
|                               CONTINUE ‘;’
|                               RETURN expression.opt ‘;’
|                               GOTO identifier ‘;’
declaration_statement:              block_declaration
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
compound_declaration:               ‘{‘ nest declaration_seq.opt ‘}’                            { unnest(); }
|                               ‘{‘ nest declaration_seq.opt util looping_declaration ‘#’ bang error ‘}’
{ unnest(); UNBANG(“Bad declaration-seq.”); }
declaration_seq.opt:                /* empty */
|                               declaration_seq.opt util looping_declaration
|                               declaration_seq.opt util looping_declaration ‘#’ bang error ‘;’ { UNBANG(“Bad declaration.”); }
looping_declaration:                start_search1 looped_declaration                            { end_search(); }
looped_declaration:                 declaration
|                               advance_search ‘+’ looped_declaration
|                               advance_search ‘-’
lined_declaration:                  line declaration
declaration:                        block_declaration
|                               function_definition
|                               template_declaration
/*  |                               explicit_instantiation                                      -- covered by relevant declarations */
|                               explicit_specialization
|                               specialised_declaration
|                               accessibility_specifier
|                               compound_declaration
|                               meta_control_statement
|                               AUTO meta_control_statement
|                               AUTO meta_class_specifier semi
|                               AUTO meta_expression_statement
|                               AUTO meta_function_definition
|                               syntax_macro_definition
|                               include_declaration semi
|                               file_dependency_declaration
|                               file_placement_declaration
|                               filespace_specifier semi
specialised_declaration:            linkage_specification
|                               namespace_declaration
|                               namespace_definition
|                               TEMPLATE specialised_declaration
block_declaration:                  simple_declaration
|                               specialised_block_declaration
specialised_block_declaration:      asm_definition
|                               namespace_alias_definition
/*  |                               using_declaration                                           -- covered by simple_declaration */
|                               using_directive
|                               TEMPLATE specialised_block_declaration
|                               init_declaration ‘;’
|                               constructor_head ‘,’ assignment_expression ‘;’
|                               init_declarations ‘;’
|                               decl_specifier_prefix simple_declaration
/*  A decl-specifier following a ptr_operator provokes a shift-reduce conflict for
*  which is resolved in favour of the pointer, and implemented by providing versions
*  of decl-specifier guaranteed not to start with a cv_qualifier.
*  decl-specifiers are implemented type-centrically. That is the semantic constraint
*  that there must be a type is exploited to impose structure, but actually eliminate
*  very little syntax. built-in types are multi-name and so need a different policy.
*  non-type decl-specifiers are bound to the left-most type in a decl-specifier-seq,
*  by parsing from the right and attaching suffixes to the right-hand type. Finally
suffix_built_in_decl_specifier.raw: built_in_type_specifier
|                               suffix_built_in_decl_specifier.raw built_in_type_specifier
|                               suffix_built_in_decl_specifier.raw decl_specifier_suffix
suffix_built_in_decl_specifier:     suffix_built_in_decl_specifier.raw
|                               TEMPLATE suffix_built_in_decl_specifier
|                               elaborate_type_specifier
|                               suffix_named_decl_specifier decl_specifier_suffix
suffix_named_decl_specifier.bi:     suffix_named_decl_specifier
|                               suffix_named_decl_specifier suffix_built_in_decl_specifier.raw
suffix_named_decl_specifiers:       suffix_named_decl_specifier.bi
|                               suffix_named_decl_specifiers suffix_named_decl_specifier.bi
suffix_named_decl_specifiers.sf:    scoped_special_function_id          /* operators etc */
|                               suffix_named_decl_specifiers
|                               suffix_named_decl_specifiers scoped_special_function_id
suffix_decl_specified_ids:          suffix_built_in_decl_specifier
|                               suffix_built_in_decl_specifier suffix_named_decl_specifiers.sf
|                               suffix_named_decl_specifiers.sf
suffix_decl_specified_scope:        suffix_named_decl_specifiers SCOPE
|                               suffix_built_in_decl_specifier suffix_named_decl_specifiers SCOPE
|                               suffix_built_in_decl_specifier SCOPE
decl_specifier_affix:               storage_class_specifier
|                               function_specifier
|                               cv_qualifier
/*  The bogus conflict between public: as an anonymous bit-field and member-specification is resolved to the member-specification.*/
|                               access_specifier            %prec SHIFT_THERE
/*  using-declaration is generalised to cover a much more general concept of re-use, so using treated like typedef.
*  Unfortunately this gives the same conflict on string as for linkage_specification, so the %prec forces using followed
*  by a string to be treated as an include rather than a declaration. */
|                               USING                       %prec SHIFT_THERE
decl_specifier_suffix:              decl_specifier_affix
decl_specifier_prefix:              decl_specifier_affix
|                               TEMPLATE decl_specifier_prefix
storage_class_specifier:            REGISTER | STATIC | MUTABLE
|                               EXTERN                  %prec SHIFT_THERE                   /* Prefer linkage specification */
|                               ‘!’ STATIC
|                               INLINE                  %prec SHIFT_THERE                   /* Prefer INLINE / IMPLEMENTATION */
|                               VIRTUAL                 %prec SHIFT_THERE                   /* Prefer VIRTUAL / PURE */
|                               ‘!’ INLINE
|                               INLINE ‘/’ IMPLEMENTATION
|                               INLINE ‘/’ INTERFACE
|                               ‘!’ VIRTUAL
|                               VIRTUAL ‘/’ PURE
type_specifier:                     simple_type_specifier
|                               elaborate_type_specifier
|                               cv_qualifier
/* The following augment type_specifier rather than cv_qualifier to avoid a conflict on ! between
*      a * ! const b     and    a * ! b    which requires a 2-token lookahead to resolve. */
|                               ‘!’ VOLATILE
elaborate_type_specifier:           class_specifier
|                               enum_specifier
|                               elaborated_type_specifier
|                               TEMPLATE elaborate_type_specifier
|                               built_in_type_specifier
built_in_type_specifier:            BuiltInTypeSpecifier
*  The over-general use of declaration_expression to cover decl-specifier-seq.opt declarator in a function-definition means that
*  could be a function-definition or a class-specifier.
*  could be a function-definition or an enum-specifier.
*  The function-definition is not syntactically valid so resolving the false conflict in favour of the
elaborated_type_specifier:          elaborated_class_specifier
|                               elaborated_enum_specifier
|                               TYPENAME scoped_id
elaborated_enum_specifier:          ENUM scoped_id               %prec SHIFT_THERE
enum_specifier:                     ENUM scoped_id enumerator_clause
|                               ENUM enumerator_clause
enumerator_clause:                  ‘{‘ enumerator_list_ecarb
|                               ‘{‘ enumerator_list enumerator_list_ecarb
|                               ‘{‘ enumerator_list ‘,’ enumerator_definition_ecarb
|                               bang error ‘}’                                              { UNBANG(“Bad enumerator-list.”); }
|                               bang error ‘}’                                              { UNBANG(“Bad enumerator-definition.”); }
enumerator_definition_filler:       /* empty */
|                               bang error ‘,’                                              { UNBANG(“Bad enumerator-definition.”); }
enumerator_list_head:               enumerator_definition_filler
|                               enumerator_list ‘,’ enumerator_definition_filler
enumerator_list:                    enumerator_list_head enumerator_definition
enumerator_definition:              enumerator
|                               enumerator ‘=’ constant_expression
enumerator:                         identifier
namespace_definition:               NAMESPACE scoped_id compound_declaration
|                               NAMESPACE compound_declaration
namespace_alias_definition:         NAMESPACE scoped_id ‘=’ scoped_id ‘;’
namespace_declaration:              NAMESPACE scoped_id ‘;’
using_directive:                    USING NAMESPACE scoped_id ‘;’
asm_definition:                     ASM ‘(‘ string ‘)’ ‘;’
linkage_specification:              EXTERN string looping_declaration
/*   |                              EXTERN string compound_declaration                          -- covered by declaration */
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
/*init-declarator is named init_declaration to reflect the embedded decl-specifier-seq.opt*/
init_declarations:                  assignment_expression ‘,’ init_declaration
|                               init_declarations ‘,’ init_declaration
|                               init_object_declaration ‘,’ init_declaration
|                               constructor_head ‘,’ bit_field_init_declaration
|                               constructor_head ‘,’ init_object_declaration
init_declaration:                   assignment_expression
/*  |                               assignment_expression ‘=’ initializer_clause                -- covered by assignment_expression */
/*  |                               assignment_expression ‘(‘ expression_list ‘)’               -- covered by another set of call arguments */
|                               bit_field_init_declaration
|                               init_object_declaration
init_object_declaration:            assignment_expression object_statements_clause
|                               bit_field_init_declaration object_statements_clause
/*declarator:                                                                                   -- covered by assignment_expression */
/*direct_declarator:                                                                            -- covered by postfix_expression */
|                               star_ptr_operator cv_qualifier
nested_ptr_operator:                star_ptr_operator
|                               id_scope nested_ptr_operator
|                               nested_ptr_operator
|                               global_scope nested_ptr_operator
ptr_operator_seq:                   ptr_operator
|                               ptr_operator ptr_operator_seq
/* Independently coded to localise the shift-reduce conflict: sharing just needs another %prec */
ptr_operator_seq.opt:               /* empty */                         %prec SHIFT_THERE       /* Maximise type length */
|                               ptr_operator ptr_operator_seq.opt
cv_qualifier_seq.opt:               /* empty */
|                               cv_qualifier_seq.opt cv_qualifier
cv_qualifier:                       CONST | VOLATILE /* | CvQualifier */
/*type_id                                                                                       -- also covered by parameter declaration */
type_id:                            type_specifier abstract_declarator.opt
|                               type_specifier type_id
/*abstract_declarator:                                                                          -- also covered by parameter declaration */
abstract_declarator.opt:            /* empty */
|                               ptr_operator abstract_declarator.opt
|                               direct_abstract_declarator
direct_abstract_declarator.opt:     /* empty */
|                               direct_abstract_declarator
direct_abstract_declarator:         direct_abstract_declarator.opt parenthesis_clause
|                               direct_abstract_declarator.opt ‘[‘ ‘]’
|                               direct_abstract_declarator.opt ‘[‘ constant_expression ‘]’
/*  |                               ‘(‘ abstract_declarator ‘)’                                 -- covered by parenthesis_clause */
parenthesis_clause:                 parameters_clause cv_qualifier_seq.opt
|                               parameters_clause cv_qualifier_seq.opt exception_specification
parameters_clause:                  ‘(‘ parameter_declaration_clause ‘)’
/* parameter_declaration_clause also covers init_declaration, type_id, declarator and abstract_declarator. */
parameter_declaration_clause:       /* empty */
|                               parameter_declaration_list
|                               parameter_declaration_list ELLIPSIS
parameter_declaration_list:         parameter_declaration
|                               parameter_declaration_list ‘,’ parameter_declaration
* looks like a multiply, so pointers are parsed as their binary operation equivalents that
* ultimately terminate with a degenerate right hand term.
abstract_pointer_declaration:       ptr_operator_seq
|                               multiplicative_expression star_ptr_operator ptr_operator_seq.opt
abstract_parameter_declaration:     abstract_pointer_declaration
|                               and_expression ‘&’
|                               and_expression ‘&’ abstract_pointer_declaration
special_parameter_declaration:      abstract_parameter_declaration
|                               abstract_parameter_declaration ‘=’ assignment_expression
parameter_declaration:              assignment_expression
|                               special_parameter_declaration
|                               decl_specifier_prefix parameter_declaration
/*  The grammar is repeated for use within template <>
templated_parameter_declaration:    templated_assignment_expression
|                               templated_abstract_declaration
|                               templated_abstract_declaration ‘=’ templated_assignment_expression
|                               decl_specifier_prefix templated_parameter_declaration
templated_abstract_declaration:     abstract_pointer_declaration
|                               templated_and_expression ‘&’
|                               templated_and_expression ‘&’ abstract_pointer_declaration
/*  function_definition includes constructor, destructor, implicit int definitions too.
*  A local destructor is successfully parsed as a function-declaration but the ~ was treated as a unary operator.
*  constructor_head is the prefix ambiguity between a constructor and a member-init-list starting with a bit-field.
func_definition:            assignment_expression function_try_block
|                       assignment_expression function_body
|                       decl_specifier_prefix func_definition
ctor_definition:            constructor_head function_try_block
|                       constructor_head function_body
|                       decl_specifier_prefix ctor_definition
constructor_head:           bit_field_init_declaration
|                       constructor_head ‘,’ assignment_expression
function_try_block:         TRY function_block handler_seq
function_block:             ctor_initializer.opt function_body
function_body:              compound_statement
/*  An = initializer looks like an extended assignment_expression.
*  An () initializer looks like a function call.
*  initializer is therefore flattened into its generalised customers.
*initializer:              ‘=’ initializer_clause                                      -- flattened into caller
*  |                       ‘(‘ expression_list ‘)’                                     -- flattened into caller */
initializer_clause:         assignment_expression
|                       braced_initializer
braced_initializer:         ‘{‘ initializer_list ‘}’
|                       ‘{‘ initializer_list ‘,’ ‘}’
|                       ‘{‘ looping_initializer_clause ‘#’ bang error ‘}’           { UNBANG(“Bad initializer_clause.”); }
|                       ‘{‘ initializer_list ‘,’ looping_initializer_clause ‘#’ bang error ‘}’
{ UNBANG(“Bad initializer_clause.”); }
initializer_list:           looping_initializer_clause
|                       initializer_list ‘,’ looping_initializer_clause
looping_initializer_clause: start_search looped_initializer_clause                      { end_search(); }
looped_initializer_clause:  initializer_clause
|                       advance_search ‘+’ looped_initializer_clause
|                       advance_search ‘-’
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------
*  An anonymous bit-field declaration may look very like inheritance:
*  The two usages are too distant to try to create and enforce a common prefix so we have to resort to
*  a parser hack by backtracking. Inheritance is much the most likely so we mark the input stream context
*  and try to parse a base-clause. If we successfully reach a { the base-clause is ok and inheritance was
*  the correct choice so we unmark and continue. If we fail to find the { an error token causes back-tracking
*  to the alternative parse in elaborated_class_specifier which regenerates the : and declares unconditional success.
colon_mark:                 ‘:’                                                         { mark(); }
elaborated_class_specifier: class_key scoped_id                    %prec SHIFT_THERE
|                       class_key scoped_id colon_mark error                        { rewind_colon(); }
class_specifier_head:       class_key scoped_id colon_mark base_specifier_list ‘{‘      { unmark(); }
|                       class_key ‘:’ base_specifier_list ‘{‘
|                       class_key scoped_id ‘{‘
class_key:                  CLASS | STRUCT | UNION
class_specifier:            class_specifier_head nest declaration_seq.opt ‘}’           { unnest(); }
|                       class_specifier_head nest declaration_seq.opt util looping_declaration ‘#’ bang error ‘}’
{ unnest(); UNBANG(“Bad member_specification.opt.”); }
accessibility_specifier:    access_specifier ‘:’
bit_field_declaration:      assignment_expression ‘:’ bit_field_width
|                       ‘:’ bit_field_width
bit_field_width:            logical_or_expression
/*  |                       logical_or_expression ‘?’ expression ‘:’ assignment_expression  -- has SR conflict w.r.t later = */
|                       logical_or_expression ‘?’ bit_field_width ‘:’ bit_field_width
bit_field_init_declaration: bit_field_declaration
|                       bit_field_declaration ‘=’ initializer_clause
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
/*base_clause:              ‘:’ base_specifier_list                                     -- flattened */
|                       base_specifier_list ‘,’ base_specifier
|                       access_specifier base_specifier
|                       VIRTUAL base_specifier
|                       ‘!’ VIRTUAL base_specifier
|                       AUTO base_specifier
access_specifier:           PRIVATE | PROTECTED | PUBLIC
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
conversion_function_id:     OPERATOR conversion_type_id
conversion_type_id:         type_specifier ptr_operator_seq.opt
|                       type_specifier conversion_type_id
*  Ctor-initialisers can look like a bit field declaration, given the generalisation of names:
*      NonClass(bit_field) : int(2), second_variable, ...
*  The grammar below is used within a function_try_block or function_definition.
*  See simple_member_declaration for use in normal member function_definition.
ctor_initializer:           ‘:’ mem_initializer_list
|                       ‘:’ mem_initializer_list bang error                         { UNBANG(“Bad ctor-initializer.”); }
|                       mem_initializer_list_head mem_initializer
mem_initializer_list_head:  mem_initializer_list ‘,’
|                       mem_initializer_list bang error ‘,’                         { UNBANG(“Bad mem-initializer.”); }
mem_initializer:            mem_initializer_id ‘(‘ expression_list.opt ‘)’
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
*  It is not clear from the ANSI standard whether spaces are permitted in delete[]. If not then it can
*  be recognised and returned as DELETE_ARRAY by the lexer. Assuming spaces are permitted there is an
*  ambiguity created by the over generalised nature of expressions. operator new is a valid delarator-id
*  which we may have an undimensioned array of. Semantic rubbish, but syntactically valid. Since the
*  array form is covered by the declarator consideration we can exclude the operator here. The need
*  for a semantic rescue can be eliminated at the expense of a couple of shift-reduce conflicts by
*  removing the comments on the next four lines.
/*  |                 / ---- /      NEW                 %prec SHIFT_THERE
/*  |                 / ---- /      DELETE              %prec SHIFT_THERE
/*  |                 / ---- /      NEW ‘[‘ ‘]’                                                 -- Covered by array of OPERATOR NEW */
/*  |                 / ---- /      DELETE ‘[‘ ‘]’                                              -- Covered by array of OPERATOR DELETE */
|                               ARROW_STAR
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
template_declaration:               template_parameter_clause declaration
|                               EXPORT template_declaration
/*  This extension is only defined for USING, but we need to use decl_specifier_prefix to avoid conflicts. */
|                               decl_specifier_prefix template_declaration
template_parameter_clause:          TEMPLATE ‘<‘ template_parameter_list ‘>’
template_parameter_list:            template_parameter
|                               template_parameter_list ‘,’ template_parameter
template_parameter:                 simple_type_parameter
|                               simple_type_parameter ‘=’ type_id
|                               templated_type_parameter
|                               templated_type_parameter ‘=’ identifier
|                               templated_parameter_declaration
|                               bang error                                                  { UNBANG(“Bad template-parameter.”); }
/*  |                               CLASS identifier                                            -- covered by parameter_declaration */
/*  |                               TYPENAME identifier                                         -- covered by parameter_declaration */
templated_type_parameter:           template_parameter_clause CLASS
|                               template_parameter_clause CLASS identifier
template_id:                        TEMPLATE identifier ‘<‘ template_argument_list ‘>’
|                               TEMPLATE template_id
*  template-argument is evaluated using a templated...expression so that > resolves to end of template.
template_argument_list:             template_argument
|                               template_argument_list ‘,’ template_argument
template_argument:                  templated_parameter_declaration
/*  |                               type_id                                                     -- covered by templated_parameter_declaration */
/*  |                               template_name                                               -- covered by templated_parameter_declaration */
/*  |                               error                                                       -- must allow template failure to re-search */
*  Generalised naming makes identifier a valid declaration, so TEMPLATE identifier is too.
*  The TEMPLATE prefix is therefore folded into all names, parenthesis_clause and decl_specifier_prefix.
/*explicit_instantiation:           TEMPLATE declaration */
explicit_specialization:            TEMPLATE ‘<‘ ‘>’ declaration
/*  This extension is only defined for USING, but we need to use decl_specifier_prefix to avoid conflicts. */
|                               decl_specifier_prefix explicit_specialization
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
try_block:                          TRY compound_statement handler_seq
/*function_try_block:                                                                           -- moved near function_block */
/* A handler_seq may follow a try_block in a compound_tree_statement such as:
*      if (a) try { } catch(a) {} catch(b) {} catch(c) {} ...
*  we resolve the conflict by maximising the handler sequence. */
handler_seq:                        handler                                 %prec SHIFT_THERE   /* Maximise length */
|                               handler handler_seq
handler:                            CATCH ‘(‘ exception_declaration ‘)’ compound_statement
exception_declaration:              parameter_declaration
/*                                  ELLIPSIS                                                    -- covered by parameter_declaration */
|                               THROW assignment_expression
|                               THROW templated_assignment_expression
exception_specification:            THROW ‘(‘ ‘)’
|                               THROW ‘(‘ type_id_list ‘)’
|                               type_id_list ‘,’ type_id
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
primary_tree_expression:            meta_scoped_id
|                               ‘(‘ tree_expression ‘)’
postfix_tree_expression:            primary_tree_expression
|                               postfix_tree_expression ‘[‘ ‘]’
|                               postfix_tree_expression ‘[‘ constant_expression ‘]’
|                               postfix_tree_expression ‘(‘ tree_argument_list.opt ‘)’
|                               postfix_tree_expression ‘.’ scoped_id
|                               postfix_tree_expression ARROW scoped_id
tree_expression:                    postfix_tree_expression
|                               ‘*’ tree_expression
/* tree_argument_list.opt are carefully coded to avoid conflicts between the components of a constructor_head at the start of a function_definition
* and the equivalent discrete elements. There is no need to resolve a conflict on “,”!, which is fortunate because it couldn’t work. */
tree_argument_list.opt:             tree_arguments.head
|                               tree_arguments.head ‘,’ tree_argument_list.opt
|                               tree_argument.ctors
tree_argument.ctors:                constructor_head
|                               decl_specifier_prefix tree_argument.ctors
tree_arguments.head:                /* empty */
|                               tree_argument.most
|                               assignment_expression
|                               func_definition
|                               tree_argument.ctors_comma_most
tree_argument.ctors_comma_most:     constructor_head ‘,’ tree_argument.most
|                               decl_specifier_prefix tree_argument.ctors_comma_most
tree_argument.most:                 terminated_tree_argument
|                               ctor_definition
|                               unterminated_tree_argument.most
|                               unterminated_tree_argument.most ‘;’
|                               tree_argument.misc
|                               tree_argument.misc ‘;’
tree_argument.misc:                 decl_specifier_prefix
/*  |                               assignment_expression                                       -- separated out */
/*  |                               bit_field_init_declaration                                  -- separated out into tree_argument.ctors */
/*  |                               function_definition                                         -- split into ctor/func_definition */
|                               init_object_declaration
|                               special_parameter_declaration
|                               decl_specifier_prefix assignment_expression
|                               decl_specifier_prefix tree_argument.misc
looping_unterminated_tree_argument: start_search looped_unterminated_tree_argument              { end_search(); }
looped_unterminated_tree_argument:  unterminated_tree_argument
|                               advance_search ‘+’ looped_unterminated_tree_argument
/*  |                               advance_search ‘-’ */
/* Omission of the preceding line which causes two reduce/reduce conflicts is justified provided the
* looped_unterminated_tree_argument rules are only used within a compound_tree_statement, where the alternate
* looping search for a tree_statement precedes and dominates this search. Since the cascaded advance_search ‘-’
* is only used to terminate a total failure of the search for a plausible template/arithmetic syntax, it doesn’t
* matter, apart from minor error reporting niceties, whether it is the statement or unterminated argument search
looping_tree_statement:             start_search looped_tree_statement                          { end_search(); }
looped_tree_statement:              tree_statement
|                               advance_search ‘+’ looped_tree_statement
|                               advance_search ‘-’
|                               terminated_tree_argument
|                               unterminated_tree_argument ‘;’
|                               function_definition
compound_tree_statement:            ‘{‘ tree_statement_seq.opt ‘}’
|                               ‘{‘ tree_statement_seq.opt looping_unterminated_tree_argument ‘}’
|                               ‘{‘ tree_statement_seq.opt looping_unterminated_tree_argument ‘#’ bang error ‘}’
{ UNBANG(“Bad compound-tree-statement.”); }
|                               ‘{‘ tree_statement_seq.opt looping_tree_statement ‘#’ bang error ‘}’
{ UNBANG(“Bad compound-tree-statement.”); }
tree_statement_seq.opt:             /* empty */
|                               tree_statement_seq.opt looping_tree_statement
|                               tree_statement_seq.opt looping_tree_statement ‘#’ bang error ‘;’
{ UNBANG(“Bad tree-statement.”); }
/*  Terminated syntax has an unambiguous end and does not need a ; as a meta-variable initializer. */
terminated_tree_argument:           asm_definition
|                               compound_tree_statement
/*  |                               declaration_statement ‘;’                   -- covered by simple_tree_declaration ; */
/*  |                               explicit_instantiation                      -- covered by simple_tree_declaration ; */
|                               explicit_specialization
/*  |                               expression_statement ‘;’                    -- covered by simple_tree_declaration ; */
|                               file_dependency_declaration
|                               file_placement_declaration
|                               include_declaration semi
|                               iteration_statement
/*  |                               jump_statement                              -- covered by BREAK ; */
|                               labeled_statement
|                               linkage_specification
|                               namespace_alias_definition
|                               namespace_declaration
|                               namespace_definition
/*  |                               parameter_declaration ‘;’                   -- covered by simple_tree_declaration ; */
|                               selection_statement
|                               template_declaration
|                               using_directive
|                               AUTO meta_control_statement
|                               AUTO meta_expression_statement
|                               AUTO meta_function_definition
|                               OPERATOR ‘;’
/*  Unterminated syntax has no obvious end and/or must have a ; as a meta-variable initializer. */
unterminated_tree_argument:         unterminated_tree_argument.most
|                               simple_tree_declaration
unterminated_tree_argument.most:    accessibility_specifier
/*  |                               access_specifier                            -- covered by decl_specifier_affix */
/*  |                               base_specifier                              -- covered by simple_tree_declaration */
/*  |                               built_in_type_specifier                     -- covered by simple_tree_declaration */
/*  |                               class_specifier                             -- covered by simple_tree_declaration */
/*  |                               condition                                   -- covered by simple_tree_declaration */
/*  |                               cv_qualifier                                -- covered by simple_tree_declaration */
/*  |                               decl_specifier                              -- covered by simple_tree_declaration */
/*  |                               enum_specifier                              -- covered by simple_tree_declaration */
/*  |                               enumerator_definition                       -- covered by simple_tree_declaration */
/*  |                               exception_declaration                       -- covered by simple_tree_declaration */
/*  |                               exception_specification                     -- covered by simple_tree_declaration */
|                               filespace_specifier
/*  |                               function_definition                         -- not part of .most */
|                               function_try_block
|                               handler_seq
/*  |                               initializer_clause                          -- covered by simple_tree_declaration, compound_statement */
/*  |                               mem_initializer                             -- covered by simple_tree_declaration */
|                               AUTO meta_class_specifier
/*  |                               operator                                    -- mostly covered by token.punct */
/*  |                               parameter_declaration                       -- not part of .most */
/*  |                               simple_tree_declaration                     -- not part of .most */
|                               simple_type_parameter
/*  |                               storage_class_specifier                     -- covered by simple_tree_declaration */
/*  |                               template_argument                           -- covered by simple_tree_declaration */
/*  |                               template_parameter                          -- covered by simple_tree_declaration */
/*  |                               try_block                                   -- covered by function_try_block */
/*  |                               type_id                                     -- covered by simple_tree_declaration */
/*  |                               type_parameter                              -- covered by simple_tree_declaration, template_declaration */
|                               reserved_id
|                               token.punct
/*  |                               CATCH                                       -- awkward function-definition at end of terminated  */
/*  |                               CLASS                                       -- covered by simple_type_parameter */
/*  |                               DO                                          -- DO ; awkward */
/*  |                               OPERATOR                                    -- OPERATOR , awkward */
/*  |                               THROW                                       -- covered by throw-expression */
/*  |                               TYPENAME                                    -- covered by simple_type_parameter */
/*  |                               ‘*’ | ‘&’ | ELLIPSIS                        -- covered by simple_tree_declaration */
/*  |                               ‘#’                                         -- used as error iteration flag */
/*  |                               ‘,’ | ‘{‘ | ‘}’ | ‘(‘ | ‘)’ | ‘;’           -- awkward - major punctuation */
/*  |                               ‘/’                                         -- awkward looks like switch */
reserved_id:                        ASM | BREAK | CASE | CONST_CAST | CONTINUE | DEFAULT | DELETE | DYNAMIC_CAST | ELSE | FOR
|                               GOTO | IF | NEW | REINTERPRET_CAST | RETURN | SIZEOF | STATIC_CAST | SWITCH | TRY | TYPEID | WHILE
token.punct:                        SCOPE | SHL | SHR | EQ | NE | LE | GE | LOG_AND | LOG_OR | INC | DEC | ARROW | ARROW_STAR | DOT_STAR
|                               ASS_ADD | ASS_AND | ASS_DIV | ASS_MOD | ASS_MUL | ASS_OR | ASS_SHL | ASS_SHR | ASS_SUB | ASS_XOR
|                               ‘[‘ | ‘]’ | ‘:’ | ‘?’ | ‘.’
|                               ‘+’ | ‘-’ | ‘%’ | ‘^’ | ‘|’ | ‘~’ | ‘!’ | ‘=’ | ‘<‘ | ‘>’
|                               ‘\’’ | ‘\”’ | ‘\\’
simple_tree_declaration:            decl_specifier_prefix
|                               init_declaration
|                               constructor_head ‘,’ assignment_expression
|                               init_declarations
|                               special_parameter_declaration
|                               decl_specifier_prefix simple_tree_declaration
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
object_statements_clause:           ‘:’ ‘{‘ object_statement_seq.opt ‘}’
|                               ‘:’ ‘{‘ object_statement_seq.opt looping_object_statement ‘#’ bang error ‘}’
{ UNBANG(“Bad object-statements-clause.”);}
object_statement_seq.opt:           /* empty */
|                               object_statement_seq.opt looping_object_statement
|                               object_statement_seq.opt looping_object_statement ‘#’ bang error ‘;’
{ UNBANG(“Bad object-statement.”); }
looping_object_statement:           start_search looped_object_statement                        { end_search(); }
looped_object_statement:            object_statement
|                               advance_search ‘+’ looped_object_statement
|                               advance_search ‘-’
|                               function_used_block
|                               ‘=’ initializer_clause ‘;’
|                               ‘(‘ expression_list ‘)’ ‘;’
|                               file_dependency_declaration
|                               file_placement_declaration
|                               filespace_specifier semi
|                               meta_control_statement
|                               AUTO meta_control_statement
|                               AUTO meta_expression_statement
|                               AUTO meta_function_definition
|                               derived_clause object_statement
|                               derived_clause ‘:’ ‘{‘ object_statement_seq.opt ‘}’
function_used_block:                function_block
|                               function_try_block
|                               ctor_initializer ‘;’
|                               USING file_id_list function_used_block
|                               segment function_used_block
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
derived_clause:                     DERIVED ‘(‘ meta_conditional_expression ‘)’
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
|                               meta_simple_type
meta_scope:                         meta_id SCOPE
|                               meta_scope ‘~’ meta_id
|                               meta_scope meta_nested_id
meta_scoped_id:                     meta_nested_id
|                               global_scope meta_nested_id
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
meta_class_specifier:               meta_class_key meta_nested_id compound_declaration
|                               meta_class_key meta_nested_id ‘:’ base_specifier_list compound_declaration
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------
*  The MetaType names are not reserved words so form part of identifier and consequently scoped_id */
/* The %prec maximises the length of e.g. unsigned int when followed by e.g int::a */
|                               built_in_type_id                %prec SHIFT_THERE
meta_simple_type:                   meta_class_key
|                               meta_non_class_key
|                               meta_simple_type
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
/*meta_variable_declaration:                                                                    -- covered by meta_expression_statement */
/*---------------------------------------------------------------------------------------------------
* A 17.5 meta-functions, meta-constructors and meta-destructors
*---------------------------------------------------------------------------------------------------*/
/*          meta_postfix_expression covers the function name, tree_argument_list.opt covers the parameter list */
/*          meta_postfix_expression also covers the function name(tree_argument_list.opt) for exposed list */
meta_function_definition:           meta_postfix_expression ‘(‘ tree_argument_list.opt ‘)’ compound_tree_statement
|                               meta_postfix_expression ‘[‘ ‘]’ compound_tree_statement
|                               ‘~’ meta_postfix_expression ‘(‘ tree_argument_list.opt ‘)’ compound_tree_statement
|                               CONST meta_function_definition
|                               STATIC meta_function_definition
|                               ‘!’ STATIC meta_function_definition
/*  |                               meta_postfix_expression ‘(‘ tree_argument_list.opt ‘)’ object_statements_clause
-- covered by meta_expression_statement */
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
meta_control_statement:             line meta_control_statement1
meta_control_statement1:            CASE constant_expression ‘:’ lined_declaration
|                               DEFAULT ‘:’ lined_declaration
|                               DO lined_declaration WHILE ‘(‘ expression ‘)’ semi
|                               IF ‘(‘ condition ‘)’ lined_declaration     %prec SHIFT_THERE
|                               IF ‘(‘ condition ‘)’ lined_declaration ELSE lined_declaration
|                               SWITCH ‘(‘ expression ‘)’ lined_declaration
|                               WHILE ‘(‘ condition ‘)’ lined_declaration
|                               FOR ‘(‘ for_init_statement condition.opt ‘;’ expression.opt ‘)’ lined_declaration
|                               jump_statement
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
meta_primary_head:                  meta_scoped_id
|                               MetaType meta_nested_id
|                               meta_non_class_key meta_nested_id
meta_primary_id:                    meta_primary_head
|                               meta_class_key meta_nested_id
|                               meta_primary_id
|                               ‘(‘ tree_argument_list.opt ‘)’
meta_postfix_expression:            meta_primary_expression
|                               meta_postfix_expression ‘(‘ tree_argument_list.opt ‘)’
|                               meta_postfix_expression ‘[‘ ‘]’
|                               meta_postfix_expression ‘[‘ expression ‘]’
|                               meta_postfix_expression ‘.’ declarator_id
|                               meta_postfix_expression ARROW declarator_id
|                               meta_postfix_expression INC
|                               meta_postfix_expression DEC
meta_unary_expression:              meta_postfix_expression
|                               INC meta_cast_expression
|                               DEC meta_cast_expression
|                               star_ptr_operator meta_cast_expression
|                               ‘&’ meta_cast_expression
|                               ‘+’ meta_cast_expression
|                               ‘-’ meta_cast_expression
|                               ‘!’ meta_cast_expression
|                               ‘~’ meta_cast_expression
|                               SIZEOF unary_expression
meta_cast_expression:               meta_unary_expression
meta_pm_expression:                 meta_cast_expression
meta_multiplicative_expression:     meta_pm_expression
|                               meta_multiplicative_expression star_ptr_operator meta_pm_expression
|                               meta_multiplicative_expression ‘/’ meta_pm_expression
|                               meta_multiplicative_expression ‘%’ meta_pm_expression
meta_additive_expression:           meta_multiplicative_expression
|                               meta_additive_expression ‘+’ meta_multiplicative_expression
|                               meta_additive_expression ‘-’ meta_multiplicative_expression
meta_shift_expression:              meta_additive_expression
|                               meta_shift_expression SHL meta_additive_expression
|                               meta_shift_expression SHR meta_additive_expression
meta_relational_expression:         meta_shift_expression
|                               meta_relational_expression ‘<‘ meta_shift_expression
|                               meta_relational_expression ‘>’ meta_shift_expression
|                               meta_relational_expression LE meta_shift_expression
|                               meta_relational_expression GE meta_shift_expression
meta_equality_expression:           meta_relational_expression
|                               meta_equality_expression EQ meta_relational_expression
|                               meta_equality_expression NE meta_relational_expression
meta_and_expression:                meta_equality_expression
|                               meta_and_expression ‘&’ meta_equality_expression
meta_exclusive_or_expression:       meta_and_expression
|                               meta_exclusive_or_expression ‘^’ meta_and_expression
meta_inclusive_or_expression:       meta_exclusive_or_expression
|                               meta_inclusive_or_expression ‘|’ meta_exclusive_or_expression
meta_logical_and_expression:        meta_inclusive_or_expression
|                               meta_logical_and_expression LOG_AND meta_inclusive_or_expression
meta_logical_or_expression:         meta_logical_and_expression
|                               meta_logical_or_expression LOG_OR meta_logical_and_expression
meta_conditional_expression:        meta_logical_or_expression
|                               meta_logical_or_expression ‘?’ meta_conditional_expression ‘:’ meta_conditional_expression
meta_expression_statement:          meta_conditional_expression semi
|                               meta_primary_head object_statements_clause semi
|                               meta_class_key meta_nested_id object_statements_clause semi
|                               meta_postfix_expression ‘(‘ tree_argument_list.opt ‘)’ object_statements_clause semi
|                               meta_postfix_expression ‘[‘ ‘]’ object_statements_clause semi
|                               meta_postfix_expression ‘[‘ expression ‘]’ object_statements_clause semi
|                               meta_logical_or_expression assignment_operator line tree_statement
|                               CONST meta_expression_statement
|                               STATIC meta_expression_statement
|                               ‘!’ STATIC meta_expression_statement
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
syntax_macro_definition:            EXPLICIT AUTO meta_type identifier ‘(‘ syntax_macro_parameter_list ‘)’ compound_tree_statement
|                               EXPLICIT AUTO meta_type identifier ‘(‘ syntax_macro_parameter_list ‘)’ ‘[‘ ‘]’ compound_tree_statement
|                               EXPLICIT AUTO meta_type identifier ‘(‘ ‘)’ compound_tree_statement
|                               EXPLICIT AUTO meta_type identifier  ‘(‘ ‘)’ ‘[‘ ‘]’ compound_tree_statement
syntax_macro_parameter_list:        syntax_macro_parameter
|                               syntax_macro_parameter_list ‘,’ syntax_macro_parameter
syntax_macro_parameter:             meta_type identifier
|                               meta_type identifier ‘[‘ ‘]’
|                               identifier
|                               reserved_id
|                               token.punct
|                               bang error                                                  { UNBANG(“bad syntax-macro-parameter.”); }
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
include_declaration:                USING string
|                               USING ‘/’ INCLUDE string_expr
|                               USING ‘/’ INCLUDE ‘/’ utility string_expr
|                               USING ‘/’ utility string_expr
file_dependency_declaration:        using_implementation semi
|                               using_interface semi
using_implementation:               USING ‘/’ IMPLEMENTATION file_use
|                               USING ‘/’ IMPLEMENTATION ‘=’ file_use
using_interface:                    USING ‘/’ INTERFACE file_use
|                               USING ‘/’ INTERFACE ‘=’ file_use
|                               file_entity
file_placement_declaration:         export_implementation semi
|                               export_interface semi
|                               EXPORT ‘/’ NOIMPLEMENTATION semi
|                               EXPORT ‘/’ UTILITY semi
export_implementation:              EXPORT ‘/’ IMPLEMENTATION implementation_file
|                               EXPORT ‘/’ IMPLEMENTATION ‘=’ implementation_file
export_interface:                   EXPORT ‘/’ INTERFACE interface_file
|                               EXPORT ‘/’ INTERFACE ‘=’ interface_file
|                               file_entity
|                               file_entity
|                               file_name ‘/’ INTERFACE
|                               file_name ‘/’ IMPLEMENTATION
|                               file_name ‘/’ TEMPLATE
|                               file_name ‘/’ utility
|                               file_name ‘/’ GUARD ‘=’ string_expr
|                               file_name ‘/’ NOGUARD
|                               file_name ‘/’ PATH ‘=’ string_expr
|                               file_name ‘/’ PREFIX ‘=’ string_expr
|                               file_name ‘/’ SUFFIX ‘=’ string_expr
file_entity:                        declarator_id
|                               elaborated_type_specifier
|                               NAMESPACE scoped_id
|                               file_entity ‘/’ IMPLEMENTATION
|                               file_entity ‘/’ INTERFACE
|                               file_id_list ‘,’ file_id
filespace_specifier:                NAMESPACE ‘/’ FILE file_name compound_declaration
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
|                               bang error ‘}’                                              { UNBANG(“Extra text ignored before ‘}’.”); }
|                               bang error ‘)’                                              { UNBANG(“Missing ‘}’.”); }
|                               bang error ‘;’                                              { UNBANG(“Extra text ignored before ‘;’.”);}
/*---------------------------------------------------------------------------------------------------
*---------------------------------------------------------------------------------------------------*/
advance_search:                     error               { yyerrok; yyclearin; advance_search(); } /* Rewind and queue ‘+’ or ‘-’ ‘#’ */
bang:                               /* empty */         { BANG(); }   /* set flag to suppress “parse error” */
line:                               /* empty */                   /* Get current line context */
mark:                               /* empty */         { mark(); }        /* Push lookahead and input token stream context onto a stack */
nest:                               /* empty */         { nest(); }        /* Push a declaration nesting depth onto the parse stack */
start_search:                       /* empty */         { start_search(false); }    /* Create/reset binary search context */
start_search1:                      /* empty */         { start_search(true); }     /* Create/reset binary search context */
util:                               /* empty */           /* Get current utility mode */
```
