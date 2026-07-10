# F Implementation

The presentations of the FOG extensions to C++ in Chapter 3, their semantics in Chapter 4, and a novel parsing approach in Chapter 5 are all fairly substantial and so a number secondary issues are relegated to this appendix.

A brief discussion of the difﬁculties of enhancing C++ syntax is followed by a description of some syntax extensions that were considered and why they were not implemented.

We then describe how the superset grammar approach resolves speciﬁc C++ parsing difﬁculties and outline the activities needed during the semantic processing necessary to recover lost syntactic resolution.

Finally the syntax extensions to support ﬁle palcement and include ﬁle dependencies are presented.

## F.1 Syntax Implementation

```
Providing additional syntax in C++ without introducing new reserved words or
totally esoteric meanings for punctuation is rather difﬁcult, since most simple
syntax using non-reserved words is covered by a simple-declaration.
The multi-pass implementation used set rather than export, and use rather than
using for the ﬁle syntaxes. This caused no ambiguity at the declaration level since
/ cannot appear except as an initializer in a declaration.
set / implementation = "file" ;
The above of course is a syntactically valid expression, and a little provocative
given the presence of set as a template name in the Standard Template Library.
Migration to the superset grammar resulted in a generalised parser being used for
declarations and so set became difﬁcult to disambiguate syntactically. The syntax
was therefore changed to its current form. export and using are not so very far
away from the intended meanings of specify-output-ﬁle, and specify-input-
dependency.
Overloading reserved words is undesirable and confusing as exempliﬁed by the
many meanings of
static. The reuse of
auto to mean
meta is equally
unsatisfactory. Other new overloadings are relatively clear, since the reserved
word is followed by a switch.
```

### F.1.1 !const and !volatile

```
Provision of !static for more explicit control of composition suggests
that !const and !volatile should also be provided. However the situation is not
quite the same. const and volatile form part of a function signature and so
there is no possibility that composition should ever interpret a missing const as
const. A missing const always means !const. This reasoning makes !const
unnecessary but does not preclude its provision as a documentation aid.
Provision of !const causes implementation problems too, since an ambiguity
arises in a generalised parse between
(type) ! a
// Cast of complement
(type) ! const // Very degenerate parameter-declaration-clause and cv-qualiﬁers
```

Two tokens of lookahead are required to resolve the ambiguity.

```
Supporting !const as an extension of type-speciﬁer rather than cv-qualiﬁer solves
this problem allowing usage everywhere except following parameter-declaration-
clauses. However this support was a little irregular and since the sole purpose was
as a documentation aid, it was decided to omit !const and !volatile.
```

### F.1.2 Member variable delegation

Larger objects may be built from smaller objects using inheritance or aggregation. Inheritance has the convenient property that the entire interface of the base object is visible as part of the larger object, whereas aggregation makes none of the interface available. The implementor is faced with an all or nothing choice for delegation.

```
A re-using-declaration that mentions a member-variable could be interpreted as a
directive to support delegation so that:
class Proxy
{
Client *_member_variable;
using _member_variable;
};
```

automatically synthesises delegation routines such as

```
int f(double b) { return _member_variable->f(b); }
for every accessible function of _member_variable. More selective synthesis
could be achieved by naming functions
using _member_variable->f;
```

A further extension was considered whereby the client could group a number of functions to establish a view:

```
class Client
{
namespace/view ProxyView
{
int f(double b);
};
};
so that all functions identiﬁed as part of the ProxyView would automatically be
delegated by:
using _member_variable->ProxyView;
This
extension
then
ensures
that
addition
of
a
further
function
to
Client::ProxyView automatically adds a delegating function to the Proxy.
```

This is useful, but vulnerable to practical considerations:

- it may be desirable to handle null tests in the delegation routines

```
•
it may be necessary to add *this as an extra argument during delegation
```

Customized formatting of the synthesised routine is not easily handled by a standardised approach. Customized formatting is available via meta-programming and so the concept of using a member variable and delimiting part of an interface through a view is no longer supported. The effect can be achieved by:

```
class Client
{
int f(double b);
int g(double b);
void g();
auto declaration ProxyView[] = { f, g(double) };
};
class Proxy : auto Client
{
Client *_member_variable;
};
auto Proxy::Proxy()
{
$delegate(_member_variable, Client::ProxyView);
};
An appropriate implementation of delegate can then be written or accessed from
a meta-library to iterate over the declarations in
Client::ProxyView to
synthesize delegation routines via _member_variable.
[ The meta-inheritance of Proxy from Client establishes a meta-compilation
order dependency so that meta-construction of Client occurs before Proxy,
ensuring that any member-functions declared by Client’s meta-constructor exist
before Proxy’s meta-constructor synthesises its delegation functions. ]
This example is far from bomb-proof; functions added to Client during meta-main
execution or by later meta-constructors will not receive delegate treatment. An
implementation proof against arbitrary composition requires Client to provide a
meta-function to register contributions to ProxyView, and to register classes
interested in viewing the contents of ProxyView. The registration function can
notify registered viewers when any change occurs, and the meta-destructor for the
client can verify that no functions have appeared without passing through the
registration function. This approach is reliable but slow and it is readily supported
by standard meta-functions from a meta-library. Alternative metaobject protocols
such as those available with CLOS or OpenC++ provide more direct hooks. FOG
could support the option for a user-deﬁnable meta-function:
auto declaration Client::add_function(declaration);
```

which would be invoked for each addition and return a possibly modiﬁed declaration or even a completely different set of declarations. The modest complexity and inefﬁciency of detecting, maintaining and invoking such functions does not seem to be justiﬁed for simple applications.

### F.1.3 Pattern names

AspectJ [Lopes98] supports the addition of code to all functions whose name matches some pattern, which is very useful for adding tracing code, since an “entering function x” diagnostic is easily attached to everything. It is not clear whether a more partial pattern match is useful without imposing a potentially awkward lexical convention on function names just to satisfy the pattern match.

```
Support for pattern matching in AspectJ is relatively easy since it is Java-based
and so there is no overloading and the * character is free for use in patterns.
Direct adoption of the same policy in FOG would not be possible since overloads
need resolution and * is used for pointers. The problem of pattern syntax is
soluble by expressing pattern names as strings. Thus
void "print*"(ostream&, "*")
might select all functions whose name starts with print, that return void and
have an ostream& as a ﬁrst parameter.
```

An alternative solution is available by meta-programming. A pattern-matching routine ﬁlters the set of all member function names and invokes a customised meta-function for each of the ﬁltered names. Much of this functionality can be provided by a meta-library, and would be considerably assisted by a built-in metafunction to support an elemental pattern match.

```
auto::~auto()
{
$std::map(customMetaFunction,
$std::filter($functions(), "print*"));
}
for which
std::map() and
std::filter() represent to-be-implemented
components
of
a
meta-library,
functions()
is
built-in
to
FOG.
customMetaFunction performs the per-function meta-programming, and might
also be a standard meta-library component. Invocation from the meta-destructor
of auto ensures that the custom functionality is applied to all classes.
```

This an area for further research. It would appear that most of the solution lies in the domain of a meta-library rather than fundamental FOG functionality.

### F.1.4 #line directive

```
The #line directive is not used in source programs generated by human beings.
It provides a very simple but useful mechanism for automatic source code
generators to ensure that compilers and debuggers refer to the original source
lines rather than some scrambled intermediate.
#line performs this role
adequately and needs no replacement, although an extension with a more cryptic
free format spelling could be considered to free the # token once Cpp has been
discontinued. The line-literal would be discarded along with whitespace in
translation phase 7.
line-literal:
~ { line-context-seq }
line-context-seq:
line-context
line-context-seq line-context
line-context:
domainopt ﬁle-lineopt line-number
domain:
identiﬁer
ﬁle-line:
string-literal
line-number:
decimal-literal
The optional domain supports deﬁnition of line numbers for more than one source
domain, with the list of contexts supporting multiple contexts. For instance code
passed ﬁrst through yacc++ and then cfront needs to report both yacc input and
yacc output line numbers, so that an enhanced cfront might include a line-literal
such as
~{"Grammar.y" 21 cxx "yacc.tab.c" 127}
```

The ﬁrst domain is unspeciﬁed and defaults to source. With this information, enhanced debugging systems and their users can select the appropriate ﬁle upon which to perform source-level debugging.

## F.2 Resolution of parsing difﬁculties

This section reviews the speciﬁc problems that arise in parsing C++ and shows how they are resolved using the multi-pass or superset grammar approach.

### F.2.1 Context-free problems

```
The C++ (and C) grammar violates the requirement for a context-free grammar,
since “New context-dependent keywords are introduced into a program by
typedef, namespace, class, enumeration and template declarations” (§A.1-1).
```

When an identiﬁer is encountered, semantic information is needed, since there is a context-dependency on type names and on template names.

```
typedef-name:
identiﬁer
namespace-name:
original-namespace-name
namespace-alias
original-namespace-name:
identiﬁer
namespace-alias:
identiﬁer
class-name:
identiﬁer
template-id
enum-name:
identiﬁer
template-name:
identiﬁer
template-id:
template-name < template-argument-list >
If the grammar deﬁned by the standard is to be followed very closely, semantic
information is apparently needed to classify identiﬁers into one of
•
class-name
•
enum-name
•
identiﬁer
(anything else)
•
namespace-alias
•
original-namespace-name
•
template-name
•
typedef-name
```

However quite what constitutes context-dependency in the grammar depends on how much the grammar is intended to specify. A complete grammar might include all the language constraints on deﬁnition/reference ordering, template instantiation and function overloading. From such a strict perspective almost all languages are context-dependent (at least when implemented using a ﬁrst order grammar).

When a grammar is solely concerned with the conversion of a token stream into an Abstract Syntax Tree to support a subsequent semantic analysis, the requirements on the grammar are much less stringent. In Section 5.7.1 it was shown that type information was not necessary. In Section 5.6.2 it was shown that a lack of template name information could also be accommodated, although the consequent complexity might not justify that approach.

The context-dependencies are therefore reviewed from the less ambitious standpoint of AST creation.

#### F.2.1.1 #include anomaly

```
The arguments of the Cpp #include directive use non-standard forms. In the
#include "string"
```

form, there is no recognition of escape sequences. And in the

```
#include <file>
```

form, the angle brackets act as string delimiters, rather than template delimiters or arithmetic operators.

**Resolution**

```
This context-dependence is readily resolved within the lexical analysis processing
by switching the lexer into an alternate state starting at the recognition of a
#include and continuing to the end of the line. While in this alternate state a
different tokenization policy is adopted,
```

#### F.2.1.2 Type information

A full semantic interpretation of a C++ program obviously requires a knowledge of the types. Unfortunately this information is also needed for a complete syntactic disambiguation of

- declaration/expression ambiguity (Section F.2.5.1)

- parenthesised-call/cast-parenthesis ambiguity (Section F.2.5.2)

- parenthesised-binary/cast-unary ambiguity (Section F.2.5.2)

- call/functional-cast ambiguity (Section F.2.5.3)

```
•
new placement/initializer ambiguity (Section F.2.5.5)
•
sizeof type/value ambiguity (Section F.2.5.6)
•
typeid type/value ambiguity (Section F.2.5.7)
```

- template argument type/value ambiguity (Section F.2.5.8)

#### F.2.1.3 < as template-start or greater than (§14.2-3)

A misparse resulting from the lack of template context is difﬁcult to resolve because the two meanings of < and > do not result in localised errors to the tree structure. The arithmetic operators are inﬁx binary operators and have no requirements for associated punctuation, whereas the template brackets must be paired.

A problem arises for an expression such as

```
V < W < X > ( Y ) > ( Z )
```

which, in C, would be four unambiguous comparisons. However in C++, there are alternate meanings depending upon which of V or W are template names.

Correct determination of template names requires that names be resolved in the correct scope and may require a template to be instantiated.

```
a->b<int>::c<...
a in current scope.
b in scope of a.
c in scope of b<int>.
```

**Resolution**

Correcting a template misparse is an inconvenient but not a particularly difﬁcult AST rearrangement. Although a back-tracking search for a syntactically consistent interpretation is of exponential complexity, the implementation and results presented in Section 5.8.2 show that this does not arise in practice.

#### F.2.1.4 > as template-end or greater than (§14.2-3)

```
Within a template, the meaning of an unnested > changes to close the template
rather than perform an arithmetic operation. This is not a context-dependency,
since the interpretation is dependent on the preceding parse context. The parser
knows whether it is in a template and so the grammar can be written to resolve the
conﬂict.
```

**Resolution**

```
The expression rules with higher precedence than > are duplicated to omit the
arithmetic > behaviour. This duplicated behaviour is used whenever parsing within
a template. As a result the parser keeps track of in/out of template context and
distinguishes between > nested within parentheses or brackets as part of its
normal operation. The cost is about 25 extra rules.
```

#### F.2.1.5 Meta-types

The use of meta-types in Section 3.1.5.5 introduces a form of context dependency. In the declaration

```
auto expression e = a + b & c;
the syntax used following the = is determined by the expression meta-type. This
dependency could be eliminated by ﬂattening the grammar to support each meta-
type individually in every relevant rule.
```

However when the same dependency exists in parsing a function argument

```
auto bool meta_function(expression e) { ... }
if (meta_function(a + b & c))
```

it appears that the semantic knowledge of the meta-type of the meta-function parameter must inﬂuence the syntactic parsing.

**Resolution (multi-pass)**

The multi-pass implementation of FOG was syntax-driven, using the meta-type of a meta-function or meta-variable to guide the parse. This only required loose coupling between syntactic and semantic processing, since meta-function and meta-variable deﬁnitions can change only at the end of a statement or declaration.

**Resolution (superset)**

The superset implementation of FOG parses for generic syntax elements, and so syntactic and semantic processing are isolated and there is no meta-type context dependency.

### F.2.2 Trivial Ambiguities

The C++ grammar “accepts a superset of valid C++ constructs” (§A-1). Two simple examples of the ambiguities that arise from the overlap between subgrammars are described below.

#### F.2.2.1 Empty statement

In the syntax for a statement:

```
statement:
expression-statement
simple-declaration
// As part of a declaration-statement
...
expression-statement:
expressionopt ;
simple-declaration:
decl-speciﬁer-seqopt init-declarator-listopt ;
both
expression-statement and
simple-declaration provide a cover for the empty
statement comprising just a semicolon. The ambiguity is trivial, but must be
eliminated in order to create an unambiguous grammar for an automated parser
tool.
```

#### F.2.2.2 Template parameter

In the syntax for template parameters

```
template-parameter:
type-parameter
parameter-declaration
class name
is valid as both a type-parameter and a parameter-declaration.
```

### F.2.3 Syntactic Ambiguities

[Roskind91] identiﬁed the major C++ ambiguities that existed prior to templates. This section provides an update to the list and shows how each can be resolved using a superset parse. This section describes only those ambiguities that are syntactically ambiguous. The next two sections discuss further apparent ambiguities: one that is not ambiguous at all, and others that may be deferred for resolution after syntactic analysis.

#### F.2.3.1 Dangling else (§6.4.1)

```
The dangling else ambiguity arises in languages with no end of if marker:
if (a)
if (b)
... ;
else
// else (!a) or (!b) ?
... ;
The ambiguity is resolved by deﬁnition to the inner-most if, requiring the parser
to shift the else on to the parser stack and to continue parsing the if (b)
statement, rather than reducing the stack, completing the if (b) statement, and
continuing the if (a) statement.
```

**Resolution**

```
Resolution of the dangling else ambiguity is implemented by using a %prec rule.
selection_statement:
"if" '(' condition ')' statement
%prec SHIFT_THERE
|
"if" '(' condition ')' statement "else" statement
|
"switch" '(' condition ')' statement
The SHIFT_THERE precedence speciﬁed for the reduction of the shorter rule is
lower than that of the else token and so the shift of the else is favoured.
```

#### F.2.3.2 < as template-start or less than (§14.2-3)

[Roskind91] does not report this ambiguity because he did not implement templates.

The template-name problem is a context-dependency and has been described in Section F.2.1.3.

#### F.2.3.3 Multiply nested scope (§7.1-2)

[Roskind91] does not report this ambiguity because he did not implement arbitrary scope nesting.

There is an ambiguity between

```
A::B ::C
```

and

```
A ::B::C
```

and

```
A::B::C
which is resolved by language deﬁnition to favour the longest possible
decl-speciﬁer-seq as the type.
```

**Resolution**

```
A single %prec rule in the FOG grammar resolves the conﬂict that arises from this
ambiguity.
id_scope:
id "::"
nested_id:
id
%prec SHIFT_THERE
|
id_scope nested_id
scoped_id:
nested_id
|
"::" nested_id
```

#### F.2.3.4 new-type-id (§5.3.4-2), conversion-function-id (§12.3.2-4)

```
The name of a new-type-id in a new-expression
new-expression:
::opt new new-placementopt new-type-id new-initializeropt
::opt new new-placementopt ( type-id ) new-initializeropt
new-type-id:
type-speciﬁer-seq new-declaratoropt
new-declarator:
ptr-operator new-declaratoropt
direct-new-declarator
direct-new-declarator:
[ expression ]
direct-new-declarator [ constant-expression ]
and of a conversion-function-id
conversion-function-id:
operator conversion-type-id
conversion-type-id:
type-speciﬁer-seq conversion-declaratoropt
conversion-declarator:
ptr-operator conversion-declaratoropt
may each end in a * or &, which can cause an ambiguity with respect to a
subsequent expression.
new int ** * p;
// (new (int **)) * (p)
// (new (int *)) * (*p)
// (new (int)) * (**p)
&operator int ** + p;
```

Each is resolved by deﬁnition to maximise the length of the type name.

**Resolution**

```
A %prec resolves the ambiguity in accordance with the language speciﬁcation,
using the one production
ptr_operator_seq.opt to implement
conversion-
declaratoropt and part of new-declaratoropt.
ptr_operator_seq.opt:
/* empty */
%prec SHIFT_THERE  /* Maximise type length */
|
ptr_operator ptr_operator_seq.opt
```

#### F.2.3.5 Array of operator ambiguity

```
operator new and operator new[] (and operator delete and operator
delete[]) are valid declarator-ids. It is unclear whether
int operator new [ ];
```

declares an array or a scalar.

**Resolution**

The ambiguity is removed by

```
•
excluding operator new[] and operator delete[] from the grammar
```

- accepting a missing array dimension in an expression

Semantic processing identiﬁes the array form from the parsed array of scalar form.

```
Alternatively, unnecessary semantic effort can be avoided by retaining the array
forms and using two %precs to resolve the two consequent shift-reduce conﬂicts.
```

### F.2.4 Deep Ambiguities

There are some C++ constructs that require a signiﬁcant amount of lookahead to determine which of two alternative syntaxes is in use.

#### F.2.4.1 Bit-ﬁeld or Inheritance

[Roskind91] identiﬁes an ambiguity following

```
class A { class B :
```

which could form part of an anonymous bit ﬁeld

```
const int C = 3;
class A { class B : C, D, E = 5; };
// C is a bit-field width, D,E are variables
```

or a base class

```
class C {};
class D {};
class E {};
class A { class B : C, D, E {}; };
// A::B inherits privately from C, D and E
class A is not really part of the ambiguity. It just serves to avoid the semantic
quibble that there are no bit-ﬁelds at global scope. There is in fact no ambiguity
anyway, because the inheritance declaration must eventually lead on to an open
brace whereas the bit-ﬁeld can never be followed by an open brace.
The problem is the need to lookahead through an arbitrary long comma-separated
list of names until some keyword (such as public) or punctuation (such as *)
clariﬁes the name list, or until eventually the trailing punctuation resolves the
ambiguity.
```

**Resolution**

```
The superset grammar assumes that class A : is the start of a class declaration,
and back-tracks to the : if the base-speciﬁer-list is not terminated by a {. This incurs
only a very minor performance loss, since the use of redundantly qualiﬁed
anonymous bit-ﬁelds is surely rare, and so back-tracking may never occur in
practice.
class_specifier:
class_key scoped_id '{' member_specification.opt '}'
|
class_key scoped_id ':' mark base_specifier_list
'{' unmark member_specification.opt '}'
|
class_key '{' member_specification.opt '}'
|
class_key ':' base_specifier_list
'{' member_specification.opt '}'
|
"template" class_specifier
elaborated_type_specifier:
class_key scoped_id ':' mark error { rewind_colon(); }
|
class_key scoped_id
%prec SHIFT_THERE
|
"enum" scoped_id
%prec SHIFT_THERE
|
"typename" scoped_id
|
"template" elaborated_type_specifier
The %prec on class_key scoped_id
resolves two conﬂicts. It forces a
following : to go through the inheritance lookahead test, and suppresses the
spurious interpretation as a function-name when followed by a { (Section F.2.6.2).
Attempting to resolve this problem by parsing for a shared preﬁx with a more
generalised expression syntax proves to be rather difﬁcult, since the
generalisation to share a preﬁx allows the constructor initializer list to provide a
third alternative. This construct also ends in a { and so
class A : name {};
would satisfy the generalised syntax of both constructor and class inheritance.
Generalising the syntax further is not possible since a constructor takes a list of
statements whereas a class takes a list of declarations. A statement and a
declaration cannot be uniﬁed since the syntax for the label of a goto statement is
highly ambiguous with respect to an anonymous bit ﬁeld (see Appendix F.2.7.2).
```

#### F.2.4.2 Type I functions

The original form of C function declarations is not normally supported by C++ compilers. The syntax presents challenges in avoiding conﬂicts, and does not tie in well with the generalised name solution of the superset.

**Resolution**

The grammars in Appendix B and Appendix C implement Type I function declarations but only at a severe (25%) cost to the parsing efﬁciency. To avoid ambiguities, a lookahead parse is performed following almost any closing parenthesis not at statement level. The severe inefﬁciencies most commonly follow typedefed pointers to functions:

```
typedef A (*B)(C);
for which (C) is a valid generalised ﬁrst parameter, and it is only after parsing
many subsequent declarations, which may include complete class deﬁnitions, that
the missing function-body is eventually detected.
```

Initiating a lookahead search after every close parenthesis interacts very badly with the initiation of binary searches to resolve template ambiguities. It is advisable to constrain the generality by maintaining a type I enabled ﬂag and type I active ﬂag so that type I lookahead is initiated less often and so that incompatible syntaxes (such as templates) terminate the lookahead more rapidly.

### F.2.5 Semantic Ambiguities

The traditional parsing approach needs to resolve semantics during syntactic analysis and so encounters ambiguities that need type information. These ambiguities are all deferred until the semantic analysis by the superset parse.

#### F.2.5.1 Declaration/Expression ambiguity (§6.8)

Section 5.5.3.2 discussed the ambiguity whereby

```
T(a);
```

could be:

```
•
an expression-statement invoking the function or constructor T with argument a
•
a declaration of a variable of type T and redundantly parenthesised name a
```

**Resolution**

The superset grammar eliminates the ambiguity but requires semantic processing to resolve type-dependent problems identiﬁed in Section F.2.1.2 and Section F.2.6.

[ The multi-pass FOG parser resolved the ambiguity by parsing declarations in the ﬁrst pass and expressions in the second pass. Statements within functions were not parsed and so no semantic corrections were required. ]

#### F.2.5.2 Parenthesised-call / cast-parenthesis parenthesised-binary / cast-unary ambiguity

A full semantic interpretation of a C++ program obviously requires a knowledge of the types. Unfortunately this information is also needed for a correct syntactic interpretation of an expression using a C cast followed by a unary operator or call.

```
(T)-5
// This is a cast if T is a type
(t)-5
// This is a subtraction if t is not a type
(T)(5)
// This is a cast if T is a type
(t)(5)
// This is a function call if t is not a type
```

**Resolution for binary operator**

Without type information, the above cases cannot be distinguished. At most one of the two possibilities can be parsed correctly. The misparse must be detected later and corrected. The superset grammar misparses the unary operator as the binary operator, since the subsequent semantic correction to change a binary operator into a cast is simpler than changing a unary into a binary:

- The change from binary requires replacing the binary node by a cast node and inserting a unary operator on the leading child node. This involves only the erroneous node and its children.

- A change from unary to cast would require inserting a cast node in the parent hierarchy of the erroneous node, and parent traversal is not normally supported by tree algorithms.

Preferring the binary operation is probably slightly more efﬁcient. The use of C casts is discouraged in C++ and so the need to make a correction to a cast should be rare.

The misparse resulting from the lack of type information is tractable because the incorrect parse results in a small easily resolved error in the parse tree.

**Resolution for parenthesis**

When a possible cast is followed by a parenthesised expression, the resolution has to be in the opposite direction, favouring the cast, since the presumption of a function-call would preclude the possibility of a subsequent non-parenthesised term:

```
(a)(b)(c)(d)e(f)(g)(h);
```

[The multi-pass implementation of FOG used back-tracking to resolve ambiguities. The cast ambiguity was resolved by establishing a mark following any open parenthesis and then attempting to parse a cast. If the cast failed a nested expression was parsed. This use of back-tracking was inefﬁcient, required a syntax rearrangement to isolate the leading parenthesis and resolved the

ambiguity in the opposite direction. Since expressions were only used within the context of declarations, inadequacies were not signiﬁcant.]

#### F.2.5.3 Call/functional-cast ambiguity

As described in Section 5.7.1.4, the functional-cast is totally subsumed by a call and so has been eliminated from the superset grammar.

#### F.2.5.4 Destructor name/one’s complement ambiguity (§5.3.1-9)

An unqualiﬁed destructor name cannot appear in an expression because of the ambiguity with a complement operator.

```
~ X ();
// This is ~(X()) even if X is a class
```

Unqualiﬁed destructors are therefore omitted from the superset parse.

**Resolution**

The missing unqualiﬁed destructor name in a declaration must be recovered from its complement expression form. The semantic processing must check all complement nodes to detect the misparse.

#### F.2.5.5 new-placement/new-initializer ambiguity

```
Type information is needed to distinguish between an omitted new-placement and an
omitted new-initializer when
new (a)(b)
```

is parsed against

```
new-expression:
::opt new new-placementopt new-type-id new-initializeropt
::opt new new-placementopt ( type-id ) new-initializeropt
new-placement:
( expression-list )
new-initializer:
( expression-listopt )
```

**Resolution**

```
The parser creates an AST node with two child expressions for the ambiguous
case. Semantic processing determines that if the ﬁrst child is a type, then the new-
placement has been omitted, or alternately if non-type that the new-initializer defaults.
[The multi-pass implementation of FOG parsed for new-placement present on a ﬁrst
pass and absent on a second.]
```

#### F.2.5.6 sizeof ambiguity

```
Type information is required to distinguish the overlap between the sizeof a value
and of a type. The syntactic ambiguity resulting from the lack of type information
may be resolved by recognising that the generalised syntax accepts a
parenthesised
parameter-declaration
as
a
unary-expression.
The
syntax
for
parenthesised type-id is therefore covered by the syntax accepted as a unary-
expression.
unary-expression:
...
sizeof unary-expression
sizeof ( type-id )
...
```

**Resolution**

The parser creates an AST node with a child expression. Semantic processing determines which syntax is in use.

[The multi-pass implementation of FOG used two passes to parse the two alternatives without changing the grammar.]

#### F.2.5.7 typeid ambiguity

```
Type information is required to distinguish the overlap between the typeid of a
value and that of a type. In the absence of type information, the superset parse of
expression and type-id is available as a parameter-declaration-clause.
postﬁx-expression:
...
typeid ( expression )
typeid ( type-id )
typeid ( parameter-declaration-clause )
...
```

**Resolution**

The parser creates an AST node with a child expression. Semantic processing determines which syntax is in use.

[The multi-pass implementation of FOG used two passes to parse the two alternatives without changing the grammar.]

#### F.2.5.8 Template argument type/value ambiguity

The appropriate template specialisation cannot be selected without knowledge of inheritance.

Successful parsing of the arguments of a template require type information to distinguish type and value parameters.

**Resolution**

```
Templates do not need to be specialised during the primary parse. An AST node
describing the template-argument-list is created which the subsequent semantic
processing elaborates.
```

A superset grammar that covers

```
template-argument:
assignment-expression
type-id
template-id
can be used to create AST nodes during the syntax analysis that can be
interpreted later once type information is available. The generalised parameter-
declaration covers template-argument.
```

[The multi-pass implementation of FOG used two passes to attempt value and type parsing. Value and type were tried independently for each argument of a multi-argument template resulting in exponential complexity.]

### F.2.6 New C++ ambiguities

A strict superset of the declaration and expression syntaxes should introduce no new ambiguities. However parsing is eased by taking a rather larger superset. In particular a very general policy is adopted for names. As a result some new ambiguities are created.

#### F.2.6.1 ctor-initializer or Named Bit-ﬁeld

The generalised name makes

```
name
valid as the declarator in a function-deﬁnition with the result that there is deep
ambiguity between
type name : m(i), n(j), p(l) {}
// A constructor
type name : m(i), n(j), p(l);
// A bit-field, and variables
```

**Resolution**

```
The superset grammar uses a shared preﬁx for the two cases, so that no decision
is made until the disambiguating punctuation is reached. Use of a shared preﬁx
requires considerable generalisation, tolerating assignment-expressions as each
ctor-initializer, and an assignment-expression rather than an identiﬁer as the bit-ﬁeld
name.
```

All valid syntaxes are parsed correctly. Many invalid syntaxes are accepted and need diagnosis at the semantic level.

```
The shared preﬁx appears as constructor_head in the grammar of Appendix B.
The preﬁx causes greater problems in the implementation of tree_statement,
since, as described at the end of Section 3.1.1.6, there is an ambiguity between
the use of a comma to separate multiple components of a single element, and its
use to separate multiple elements. The implementation in Appendix C is carefully
structured to avoid shift-reduce conﬂicts.
```

#### F.2.6.2 class-speciﬁer or enum-speciﬁer as function-deﬁnition name

The generalised name makes

```
enum X
valid as the declarator in a function-deﬁnition with the result that
enum X {};
is accepted as both an enum-speciﬁer and function-deﬁnition.
```

**Resolution**

The resolution of the bit-ﬁeld or inheritance ambiguity (Appendix F.2.4.1) has the beneﬁcial side effect of solving this ambiguity for

```
class X {};
The ambiguity for enum produces a shift-reduce conﬂict, which is resolved in
favour of the only valid possibility: the enum-speciﬁer. No semantic repair is
necessary.
Elaboration of the grammar to be more restrictive on function names might be
possible for C++, although there is a risk of introducing conﬂicts between function-
deﬁnitions and simple-declarations. Such elaboration is not possible in FOG where the
extended re-using-declaration syntax accepts function names without parentheses.
```

#### F.2.6.3 delete[] ambiguity

```
Introduction of the abstract array declarator [] as part of a primary-expression
creates an ambiguity in
delete[](p)
```

between

- the intended array operation

- deletion of an abstract array of functions.

**Resolution**

The array form is removed from the grammar and supported by generalising a cast expression to accept a bracketed as well as a parenthesised preﬁx. The resulting bracketed cast ambiguities are resolved in exactly the same way as parenthesised cast ambiguities, save for the beneﬁt that the resolution is correct except for the array delete which must be detected semantically.

```
cast_expression:
unary_expression
|
abstract_expression cast_expression
abstract_expression:parenthesis_clause
|
'[' expression.opt ']'
parenthesis_clause:parameters_clause cv_qualifier_seq.opt
exception_specification.opt
```

Generalisation of the cast also covers the gcc indexed array initializer extension described in Section 3.1.4.2.

#### F.2.6.4 linkage-speciﬁcation ambiguity

The generalised name parsing supports interpretation of

```
extern "C";
as a conventional declaration since a string-literal satisﬁes the syntax of a
generalised name.
```

**Resolution**

```
This is a false ambiguity resolved by unconditional treatment of the keyword
extern followed by a string-literal as the pre-amble for a linkage-speciﬁcation.
```

### F.2.7 Extra FOG ambiguities

The presentation of the FOG extensions in Chapter 3 identiﬁed many of the ambiguities associated with the extensions and parsing approaches to avoid them. All ambiguities that cannot easily be avoided are identiﬁed in this section.

#### F.2.7.1 built-in-type-id maximised

```
The lack of distinction between meta-type-names and meta-names leads to two
ambiguities when a meta-name follows a meta-type as in a meta-variable-declaration.
auto short long :: int :: a = 0;
```

This could be

```
•
a meta-declaration of ::a with meta-type short long::int
•
a meta-declaration of ::int::a with meta-type short long
•
a meta-declaration of long::int::a with meta-type short
•
a meta-expression assigning to short long::int::a
```

**Resolution**

These ambiguities are resolved by two of the C++ disambiguation principles: maximise to the left, and prefer declarations to expressions.

```
The elemental meta-type-name is therefore maximised syntactically as short
long even though this is a semantic error. The entire name is maximised as short
long::int::a to select the expression interpretation.
```

#### F.2.7.2 label preferred to anonymous bit-ﬁeld

There is an ambiguity between a label and bit-ﬁeld:

```
label: int(value);
// label then simple-declaration
type : int(value);
// anonymous bit field
```

which does not arise in C++ since labels and bit-ﬁelds are syntactically exclusive. Syntax generalisation in FOG removes this exclusivity introducing a parsing conﬂict.

**Resolution**

```
The conﬂict is resolved to preserve C++ syntax, by preferring the label
interpretation as part of a statement. The same %prec that forces an identiﬁer <
to be shifted for a template test also forces a shift to prefer a labeled-statement
whenever a labeled-statement is a syntactic option.
id:
identifier
%prec SHIFT_THERE
...
labeled_statement:
identifier ':' looping_statement
...
```

#### F.2.7.3 handler-seq maximised

```
The generic syntax parsing for a tree statement supports a handler-seq following a
try-block.
void f() try {} catch (a) {} catch (b) {} catch (c) {}
leading to an ambiguity as to where the function ends and a subsequent handler-seq
begins.
```

**Resolution**

The ambiguity is resolved by deﬁnition to maximise the length of the left hand syntax element.

```
handler_seq:
handler
%prec SHIFT_THERE
|
handler handler_seq
```

#### F.2.7.4 access-speciﬁer

The false ambiguity resulting from the generalised name interpretation of

```
public : x(y);
as an implicit int anonymous bit-ﬁeld was discussed in Section 3.1.3.2.
```

**Resolution**

```
The ambiguity is resolved by a %prec to favour the accessibility-speciﬁer.
decl_specifier_affix:               ...
|
access_specifier
%prec SHIFT_THERE
accessibility_specifier: access_specifier ':'
```

#### F.2.7.5 inline/ and virtual/ cannot be expressions

The false ambiguities resulting from the generalised name parsing of

```
name1 inline / interface ( name2 ) ;
```

were discussed in Sections 3.1.3.5 and 3.1.3.6.

**Resolution**

```
The ambiguities are resolved by %precs to favour the switch.
function_specifier:
"explicit"
|
"inline"
%prec SHIFT_THERE
|
"virtual"
%prec SHIFT_THERE
|
'!' "inline"
|
"inline" '/' "implementation"
|
"inline" '/' "interface"
|
'!' "virtual"
|
"virtual" '/' "pure"
```

#### F.2.7.6 using string-literal is not an expression

A false ambiguity arises from the generalised name interpretation of

```
using "string";
as a re-using-declaration since a string-literal satisﬁes the generalised syntax of a
name and a
re-using-declaration is parsed as a generalised declaration or
expression.
```

**Resolution**

```
The ambiguity is resolved by treating the keyword using followed by a string-literal
as an include-declaration unconditionally.
decl_specifier_affix:
...
|
"using"
%prec SHIFT_THERE
include_declaration:
"using" string
...
```

## F.3 Semantic checks

The syntactic analysis tolerates a very generalised syntax that merges declarations and expressions and requires no knowledge of template or type names. The analysis builds an Abstract Syntax Tree that must then be processed to incorporate semantic information.

The semantic processing comprises four parts; resolution of syntax ambiguities, correction of misparses, validation of semantics and implementation. Each part is naturally performed in a distinct pass over the AST adding little to the substantial amount of processing needed for a complex language such as C++.

The origin of most of the deferred ambiguities has been described in Sections F.2.5, F.2.6 and F.2.7. Their resolution during semantic analysis is considerably eased by operation on the AST where the whole of the construct to be analysed is available, whereas the more conventional approach must struggle with constraints of parser lookahead and shift-reduce conﬂicts. Both approaches ultimately require the same decision code.

### F.3.1 Resolution

A conventional C++ parse uses semantic information to resolve most if not all ambiguities during the syntax parse. The superset parse uses a generalised syntax to avoid semantic leakage. As a result the parse is incomplete and requires ambiguities deferred from the syntactic analysis to be resolved during semantic analysis. The most signiﬁcant ambiguity is the declaration/expression ambiguity, but there are a number of other minor type related problems to resolve.

#### F.3.1.1 Declaration/Expression (§6.8)

The usage of the same AST nodes for declarations and expressions must be resolved. The superset grammar parses declarations as a slightly generalised expression, and so when appropriate, the declaration must be determined from an expression tree. The context in which a tree is used sometimes determines whether a declaration or expression is required. For instance, an initializer for a default argument can only be an expression. In most situations, such as a statement within a function, an ambiguity must be resolved.

```
Some aspects of the ambiguity can and must be resolved be resolved by an
accurate syntax check. For instance * const can only occur in a declaration,
whereas && can only occur in an expression. Further aspects can be resolved
when type information is available as it is for actual declarations. The syntax
resolution for an actual declaration can therefore perform a strong check, whereas
only a weak check can be performed for a potential declaration.
```

Resolution occurs by a dataﬂow propagation of a set of boolean ﬂags that indicate whether the AST satisﬁes a number of syntactic hypotheses. These hypotheses are propagated from the leaves to the root, applying the constraints applicable at each node so that the syntaxes satisﬁed by the tree can be determined.

The current partially functional implementation propagates a bit vector of 217 hypotheses ranging from EPSILON, IDENTIFIER, CLASS_NAME, ELABORATED_TYPE_SPECIFIER, via PARAMETER_DECLARATOR and PARAMETER_DECLARATION_CLAUSE, to BIT_FIELD_DECLARATION or META_FUNCTION_DECLARATION.

```
Maintenance of these hypotheses at the 20 or so interesting tree nodes is
relatively straightforward since few hypotheses propagate through each node
type. Most operator nodes propagate only the BASIC_EXPRESSION hypothesis.
Unary operators such as & * and ~ that have meaning in declarators also
propagate the various DECLARATOR hypotheses. Tree nodes for the binary
operators * and & resolve their alternate parse for a left-hand type and right-hand
declarator. Tree nodes involving parentheses are where the conventional
ambiguities are resolved and where there is some implementation complexity, but
only in the number of ifs that map incoming to outgoing hypotheses.
```

The overall operation of the tree nodes of course restores the grammar which was folded into the expression grammar; there are separate hypotheses for DECLARATOR and DIRECT_DECLARATOR. However, because of the very different semantic constraints associated with different names, it is necessary to maintain independent hypotheses for ABSTRACT, CONVERSION, FUNCTION, PURE_FUNCTION, INIT, META, PARAMETER or BIT_FIELD DECLARATIONs and DECLARATORs and sometimes DIRECT_DECLARATORs, DECLARATION_IDs and DECLARATOR_IDs. It is not appropriate to merge all DECLARATOR hypotheses as a single parameterised hypothesis, since each hypothesis is potentially independent and propagated up the tree in parallel. Sharing state between hypotheses could cause cross-talk and therefore fail to achieve sufﬁcient precision to resolve very ﬁnely balanced declaration/expression ambiguities correctly.

The same propagation algorithm is used for weak and strong hypotheses. The difference lies at the leaves. For a strong determination, an identiﬁer node is assessed to determine whether the identiﬁer satisﬁes each of a class, enum namespace, typedef and template hypothesis. For a weak determination, all hypotheses are satisﬁed, since there is no context to contradict the hypothesis.

All hypotheses are propagated in parallel, and so a single pass over the tree identiﬁes all satisﬁed syntaxes. Ambiguity resolution amongst those of interest determines whether to

- correct misparses in the expression metaobject

- create a potential declaration speciﬁer metaobject

The corrections that may be required are outlined in Appendix F.3.2. Creation of a potential declaration speciﬁer involves a further tree traversal towards the naming node where the appropriate metaobject can be created, and then decorated as it is returned through intervening nodes.

#### F.3.1.2 new-placement/new-initializer ambiguity

See Section F.2.5.5.

#### F.3.1.3 sizeof and typeid ambiguity

The reuse of the same AST nodes for types and values probably simpliﬁes the implementation.

#### F.3.1.4 pure-speciﬁer

The

```
pure-speciﬁer:
= 0
syntax is covered by
constant-initializer:
= constant-expression
so that if even the lexer distinguishes the lexeme '0' from the number 0, there is
little possibility of identifying the distinction in a generalised parse.
```

**Resolution**

```
The superset parse does not resolve pure-speciﬁers or even constant-initializer. The
semantic pass must identify each from the more general assignment-expression,
taking care to distinguish character streams such as 00 or 0x0 from 0.
```

#### F.3.1.5 explicit-instantiation

```
The generalised name binds template close to the name, rather than as a preﬁx
to a declaration. Semantic processing must therefore locate the keyword deeper in
the AST than might be expected. explicit-instantiation does not appear as a distinct
production.
```

#### F.3.1.6 Implicit int

```
[Roskind91] questions the enthusiasm with which compilers have stopped
supporting the deprecated implicit int for C functions with no declared return
type.
```

**Resolution**

```
The purely syntactic superset parse does not distinguish implicit int from a
constructor. Implicit int is therefore parsed successfully. The semantic pass may
easily distinguish implicit int from a constructor, since a constructor has a name
that matches its scope. Any other name is implicit int.
```

### F.3.2 Correction

Misparses resulting from incorrect assumptions during the syntactic analysis must be corrected, or accounted for during subsequent processing.

#### F.3.2.1 Template/arithmetic ambiguity

```
The binary search to identify a consistent syntax for the template or arithmetic
interpretation of identiﬁer followed by < identiﬁes the wrong syntax for
approximately 0.01% of statements (Section 5.8.2). These errors must be
corrected. Detection of the errors just requires the semantic test that should have
been performed during lexical analysis to be performed upon the AST. If the test
result is inconsistent with the tree structure, then the tree must be rearranged.
This is not particularly easy, since the two interpretations have distinctly different
tree structures, and rearrangements of nodes associated with arithmetic operators
must account for arithmetic precedences. The complexity is reduced a little by the
use of generic syntax elements, and so there is no difference between the sub-
tree
for
an
expression
and
that
for
a
template-argument.
There
is
no
template_argument meta-type.
```

#### F.3.2.2 Parenthesised-call / cast-parenthesis parenthesised-binary / cast-unary ambiguity

Semantic correction of the AST is required where a binary operator that is also a unary operator has a type as its ﬁrst child. This indicates that the binary operator should be replaced by a C-style cast, and that the equivalent unary operator be applied to the ﬁrst term in the tree headed by the second child.

The reverse correction occurs where the parenthesised function name in a function call was misparsed as a cast. An apparent cast to a non-type should be corrected to a parenthesised function call.

#### F.3.2.3 Call/functional-cast ambiguity

A function call invoking a non-class type is recognised as a functional-cast.

A function call invoking a class type is recognised as a constructor.

#### F.3.2.4 Destructor name/one’s complement ambiguity (§5.3.1-9)

A function call to a complemented class-name in a declaration is detected as a destructor declaration.

#### F.3.2.5 Array of operator ambiguity

```
Arrays of operator new or operator delete are recognised as the array forms
operator new[] and operator delete[].
```

#### F.3.2.6 delete[] ambiguity

```
An [] preﬁx following a delete operator is recognised as denoting delete[].
```

### F.3.3 Validation

The syntax generalisations to establish a simple superset grammar accept many nonsensical constructs.

```
double root2 = static sqrt(2) const throw();// declaration clutter
void b extern = int a char;
// unlimited generality
A::A() : this(0), 5+7, ~3, int(...) {}
// expression/name clutter
```

The form of each generalised expression and declaration must be carefully validated to diagnose the numerous anomalies that should normally be trapped by syntax analysis.

This has advantages in terms of error diagnosis. Because the syntax accepts so much, there is a good chance that many common semantic errors will survive the syntactic analysis, build a plausible tree, at which point the difference from a closely matching possibility can be reported. An accurate syntax parse is liable to encounter the ubiquitous unclassiﬁed “parse error” more often, or require extra elaboration in the grammar to cover probable user errors.

### F.3.4 Implementation

The semantic analysis of the AST for each parsed statement must update the symbol table to keep track of the enlarged program and make any additional declarations accessible to subsequent statements.

For some unpleasant statements such as typedefs containing more than one declarator, or function parameter lists, the semantic analysis update must perform the symbol table updates so that names introduced early in the statement have appropriate visibility later in the same statement. Achieving this during syntactic analysis, and before expression/declaration ambiguities have been resolved, requires very careful coding of the symbol table update to reﬂect the tentative semantic analysis of an incomplete syntactic analysis.

Again these are exactly the same decisions that need to be performed by any accurate C++ compiler, however their implementation is eased by operation in the context of the AST rather than within the straitjacket imposed by avoiding conﬂicts while looking inﬁnitely far ahead in the parser grammar.

## F.4 File Syntaxes

Additional syntax supports

```
•
include declarations (replacing #include)
```

- speciﬁcation of and allocation to output ﬁle names

- speciﬁcation of generated include ﬁle dependencies for scopes

- speciﬁcation of generated include ﬁle dependencies for code

This syntax contributes four new categories of declaration, only three of which are supported as parts of classes. Include declarations are only valid at global scope. The integration of these declarations with the main grammar is described in Section 3.1.5. The syntax to support function-speciﬁc and function-body-speciﬁc declarations is described in Section 3.1.4.7.

### F.4.1 Target File Names

The names of target ﬁles are used in two ways.

- to create the ﬁle in the ﬁle system

```
•
to reference the ﬁle in a generated #include directive
These two names need not be exactly the same. The name used to create the ﬁle
must be complete and speciﬁed with respect to the current user directory. Names
appearing in generated #include directives need only be resolvable by the
subsequent compiler, with respect to a potentially different current working
directory and using a search list of include ﬁle paths.
The ﬁle name used to refer to a ﬁle in a generated #include directive is composed
from 3 parts
```

prefix name suffix

The name is normally that of a class or namespace.

```
The way in which prefix and
suffix
components are determined is
summarised in Table F.1.
The suffix is normally one of .cxx, .hxx, .C, .H depending upon whether the
ﬁle is associated with the implementation or interface of a non-template or
template class. These defaults may be changed by deﬁning the corresponding
environment variable. This default setting may in turn be overridden by a
command line token. Finer-grained settings can be supplied using the appropriate
switch in the source code syntax of a ﬁle-name.
The prefix may be used to locate nested include ﬁles such as sys/stdlib.h.
The prefix has no default, but may be given a value from an environment
variable, command line option or source code switch. For template preﬁxes (and
paths) the environment variable option is determined ﬁrst from a template-speciﬁc
name, and if that is undeﬁned the non-template name is used.
```

When ﬁles are created, a further path preﬁx may be added. If no preﬁx has been deﬁned then the ﬁle is created as

prefix name suffix

otherwise the ﬁle is created as

path separator prefix name suffix

```
where path is the speciﬁed path, and separator is a ﬁle system dependent
joining character. Only / is implemented, which is suitable for any ﬁle system with
a C interface.
```

component ﬁle template default environment variable

command

-line token

source

code switch

path interface Fog::interface_path -hd /path

template Fog::template_interface_path

Fog::interface_path

-htd

implementa-

tion

Fog::implementation_path -cd

template Fog::template_implementation_path

Fog::implementation_path

-ctd

preﬁx interface Fog::interface_preﬁx -hp /preﬁx

template Fog::template_interface_preﬁx

Fog::interface_preﬁx

-htp

implementa-

tion

Fog::implementation_preﬁx -cp

template Fog::template_implementation_preﬁx

Fog::implementation_preﬁx

-ctp

sufﬁx interface .hxx Fog::interface_sufﬁx -hs /sufﬁx

template .H Fog::template_interface_sufﬁx -hts

implementa-

tion

.cxx Fog::implementation_sufﬁx -cs

template .C Fog::template_implementation_sufﬁx -cts

**Table F.1 File Name Component Contributions**

### F.4.2 Target File Identities

When the name of a ﬁle is speciﬁed, it may be qualiﬁed to override the default naming or structuring policies.

```
ﬁle-name:
string-literal
ﬁle-name / interface
ﬁle-name / implementation
ﬁle-name / template
ﬁle-name / utility
ﬁle-name / guard = string-literal
ﬁle-name / noguard
ﬁle-name / path = string-literal
ﬁle-name / prefix = string-literal
ﬁle-name / suffix = string-literal
The ﬁle name is speciﬁed as a normal string (or concatenation yielding a string),
rather than the subtly different syntax for a string in a #include directive. File
names containing escape sequences must therefore be appropriately escaped.
It is assumed that the string-literal incorporates prefix, name and suffix
components, unless /interface or /implementation is speciﬁed, in which case
the string-literal should comprise just the name component.
```

**/interface /implementation**

Specify that a prefix and a suffix should be applied to the name using values determined in accordance with Table F.1.

**/template**

Speciﬁes the use of the template, rather than non-template, policy for any path, prefix or suffix.

**/utility**

```
Speciﬁes a utility level of an output ﬁle. Only /utility (or /frozen) is
meaningful, as a further assurance that a ﬁle should not be emitted.
```

**/guard = string-literal**

Speciﬁes the spelling of the include ﬁle guard, overriding the default derived from the ﬁle name and its sufﬁx.

**/noguard**

Speciﬁes that there should be no include ﬁle guard in the target ﬁle.

**/path = string-literal**

```
speciﬁes the path to be preﬁxed to the name when creating the ﬁle. If neither
/interface nor /implementation is used, the implementation rather than
interface path is used in accordance with Table F.1.
```

**/preﬁx = string-literal /sufﬁx = string-literal**

```
specify an override for the prefix and/or suffix parts of the ﬁle name, but are
only used if one of /interface or /implementation has been used to specify an
algorithmic contribution to the name.
```

### F.4.3 Target File Placement

```
The target ﬁle for a particular scope may be changed by a ﬁle-placement-declaration.
ﬁle-placement-declaration:
export / implementation =opt ﬁle-speciﬁer ;
export / interface =opt ﬁle-speciﬁer ;
export / noimplementation ;
ﬁle-speciﬁer:
ﬁle-name
ﬁle-entity
ﬁle-entity / implementation
ﬁle-entity / interface
ﬁle-entity:
declarator-id
elaborated-type-speciﬁer
namespace scoped-id
export/implementation speciﬁes that the implementation (non-inline function
bodies and initialised variables) should be located in ﬁle-speciﬁer. Similarly
export/interface speciﬁes that the interface should be located in ﬁle-speciﬁer.
Each ﬁle-speciﬁer may be a ﬁle-name (the qualiﬁed name of a ﬁle) or ﬁle-entity (the
name of some declared entity). In the case of a ﬁle-entity, the required ﬁle is either
the implementation or the interface ﬁle to which the declarations of the entity are
emitted. An explicit distinction is made by use of
/implementation or
/interface to qualify the entity name. An implicit distinction is made in the
absence of an explicit qualiﬁer, by using the implementation or /interface
qualifying the export keyword.
```

The default placement recursively locates declarations with their enclosing class. Top level classes and namespaces are located in a ﬁle named from the declaration name. This policy may be overridden to support

- arbitrary placement

```
class Base
{
export/implementation "Bases.cxx"; // emit implementation to Bases.cxx
export/interface "Bases.hxx";
// emit interface to Bases.hxx
};
```

- placement of a derived class with its base:

```
class Derived : public Base
{
export/implementation Base;
// emit implementation with
//
implementation of Base
export/interface Base;
// emit interface with interface of Base
};
```

- placement of a local class entirely within an implementation ﬁle:

```
class Local
{
export/implementation Base;
// emit implementation with
//
implementation of Base
export/interface Base/implementation;
// emit interface with
//
implementation of Base
};
export/noimplementation speciﬁes that there should be no implementation, as
is often intended to be the case for private constructors and assignment operators.
Speciﬁcation of export/noimplementation ensures that any violation of this
intent as a result of composition is detected.
```

### F.4.4 File-spaces

A ﬁle-space is a set of declarations grouped in order to share ﬁle placement. A ﬁle-space therefore supports placement of selected declarations in a speciﬁc pair

of implementation and interface ﬁles. A ﬁle-space is not a declarative region, and so declarations appearing within a ﬁle-space form part of the enclosing scope.

```
ﬁlespace-speciﬁer:
namespace / file ﬁle-name compound-declaration
ﬁlespace-declaration:
ﬁlespace-speciﬁer ;
The implementation and interface ﬁles are determined on the assumption that ﬁle-
name constitutes a suitable name for application of prefix and suffix components
in accordance with Table F.1. Since both interface and implementation ﬁle names
are determined algorithmically, the use of /interface or /implementation on
the ﬁle-name is redundant. A ﬁle-placement-declaration may be used to change one of
both of the ﬁle names. The interface ﬁle is only used for namespaces and function
deﬁnitions inlined in the interface, since the declarations of a class must
necessarily occur within the one class: they cannot be partitioned across multiple
interface ﬁles.
```

In

```
class A
{
namespace/file "FirstRegion"
{
// ...
};
namespace/file "SecondRegion"
{
export/implementation B;
// ...
};
};
declarations within FirstRegion form part of class A and are typically emitted to
A.hxx,
FirstRegion.hxx
and
FirstRegion.cxx.
Declarations
within
SecondRegion are similarly part of class A, but are typically emitted to A.hxx,
SecondRegion.hxx and B.cxx.
File-spaces may be used to compose a ﬁle structure upon the global namespace
or upon C source, and to place explicit-instantiations.
```

### F.4.5 Target File Dependencies

Files emitted by FOG have include ﬁle guards around declarations and ﬁle inclusions that are arranged to ensure that C++ requirements for forward references are satisﬁed. Include ﬁle references and forward declarations are generated by analysis, and as a result are often tighter (better) than those produced by hand.

The current implementation of FOG performs a very limited analysis of function bodies, so FOG may miss dependencies. A perfect analysis could not guarantee to catch all dependencies, given the inadequate type information available for template arguments of templates that remain uninstantiated.

Target ﬁle dependency declarations are therefore provided to allow missed declarations to be speciﬁed.

```
ﬁle-dependency-declaration:
using / implementation =opt ﬁle-speciﬁer ;
using / interface =opt ﬁle-speciﬁer ;
```

These concepts correspond to “uses (for interface)” and “uses (for implementation)” in [Booch91]. However, whereas a full model requires all dependencies to be speciﬁed, FOG only requires the speciﬁcation of those that it fails to deduce.

```
A dependency declaration may be used as a class member-declaration to specify a
dependency for all declarations within the class
class Base
{
using/implementation "stdlib.h"; // Entire implementation uses stdlib.h
using/interface string;
// Interface uses string
};
```

Class level speciﬁcation is very heavy handed, providing little indication of where the dependency is triggered and poor support for declarations originating from algorithm-centric source code. This may provoke spurious includes to be generated if the class implementation is spread into many ﬁles. FOF therefore supports declaration level speciﬁcation of dependencies:

```
As an object-statement to restrict the dependency to a single function, irrespective of
its derivation context
class Base
{
public void f()
:{
// N.B. Start of function scope not body.
using/implementation iostream;
{ ... };
// Base::f implementation uses iostream
};
};
or as part of a function-used-block to further restrict the dependency to the derivation
contexts in which the associated function-body is used.
class Base
{
public void f()
:{
// N.B. Start of function scope not body.
using iostream { ... };
// Function contribution uses iostream
};
};
```

### F.4.6 File inclusion

```
A replacement syntax for #include is provided by overloading the using keyword
and recognising that using followed by a string has no meaning in C++.
include-declaration:
using slash-includeopt slash-utilityopt string-literal ;
slash-include:
/ include
slash-utility:
/ utility
utility:
pool
emit
utility
frozen
The utility indicates whether the declaration belongs to a frozen free-standing
external class utility, to a pool of declarations, or whether the declaration is to be
emitted. The default behaviour is for all declarations arising from #include ﬁles
to be treated as frozen utility declarations that must not be changed or re-emitted.
The replacement syntax for #include provides qualiﬁers to select the nature of
included declarations. The replacement syntax is only valid as a global-scope
declaration.
```

Inclusion only occurs upon the ﬁrst encounter, and so include ﬁle guards are unnecessary.

```
using "file.h";
// Include file.h preserving prevailing utility
using/utility "string.h";
// Include string.h as utility declarations
using/pool "shared.h";
// Include shared.h as pooled declarations
```

The utility or pool attributes apply throughout the included ﬁle and its nested inclusions. The prevailing mode is restored after the include completes.

Declarations read while in utility mode provide information that enables FOG to correctly analyse and emit the wanted code, but do not directly cause emission of the utility code. However, utility classes may indirectly contribute to emitted code by providing derivation rules or meta-programs that do contribute to classes that are emitted. Any attempt to change the functionality of utility classes can be diagnosed and rejected.

```
The new include syntax is restricted to top-level declarations, unlike the #include
directive which could potentially appear in the middle of an expression. The
rationale behind this is a corollary of adopting an include-just-once policy to avoid
the need for include ﬁle guards. This implies that a new-style include cannot
contribute to multiple scopes and so should not contribute to any. Every included
declaration should be part of the global name-space. Perhaps a little of the old
behaviour should be restored by introducing using/reinclude as part of any
declaration or statement.
```
