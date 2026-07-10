# 3 FOG Grammar

Sections 1.4 and 1.5 provided a brief overview of the FOG extensions to C+ including

- substitution

- concatenation

- composition

- meta-scopes

- derivation rules

- meta-programming

In this chapter these language extensions are presented in detail and the semantics speciﬁc to each extension are discussed. More general semantic issues are discussed in Chapter 4.

Description of the necessary but rather peripheral extensions required to support multiple intermediate ﬁles between the FOG translator and the C++ compiler is deferred till Chapter 6.

It is a moot point whether this chapter should appear early, or late or be relegated to an appendix. On the one hand, it contains too much important material to be an appendix and lays the foundation for the subsequent chapters. On the other hand some of the detailed descriptions become very detailed. The material is therefore presented here, and the reader is invited to skip directly to the summary in Section 3.5 on page 80, if satisﬁed by the overview in Chapter 1, or to skip to a following sub-section if a description becomes too detailed.

The changes are presented one at a time in this chapter. The summary of the changed grammar in Appendix A shows all changes combined and is structured to ease comparison with Annex A of [C++98].

The ﬁnal section justiﬁes the claim that FOG renders the C preprocessor redundant.

## 3.1 Grammar Extensions

Most of the FOG extensions contribute extra grammar, however the substitution and concatenation functionality is white-space sensitive and so must be performed earlier. This occurs during phase 6 of the C++ translation process summarised in Figure 3.1.

### 3.1.1 Substitution, Concatenation and Tokenization

The C++ standard deﬁnes (§2.1-6) translation phase 6 as:

Adjacent ordinary string literals are concatenated. Adjacent wide string literals are concatenated.

and (§2.1-7) phase 7 as:

White-space characters separating tokens are no longer signiﬁcant. Each preprocessing token is converted to a token. The resulting tokens are syntactically and semantically analyzed and translated.

```
FOG generalises phase 6 processing to support more extensive concatenation
and also the recognition of substitution operators. The parsed result of a
substitution or non-trivial concatenation is referred to as a tree-literal, since it
comprises a pre-parsed AST.
FOG generalises phase 7 to support tokenization of a tree-literal as an identiﬁer, and
to defer treatment of non-reserved words as identiﬁers.
```

The enhanced behaviour of FOG is most easily explained by ﬁrst elaborating the simple C++ descriptions into grammars before showing the revised grammars.

#### 3.1.1.1 C++ Phase 6 Concatenation Grammar

The C++ Phase 6 translations can be expressed as

```
string-literalcat:
string-literalpp
string-literalcat whitespaceopt string-literalpp
‘anything-else’cat:
‘anything-else’pp
pp denotes the preprocessor token input to phase 6 from phase 5 and cat the
concatenated output production passed from phase 6 to phase 7.
Single quotes as in ‘anything-else’ surround a production whose meaning is obvious
though difﬁcult to express compactly.
```

#### 3.1.1.2 C++ Phase 7 Tokenization Grammar

Phase 7 tokenization can be expressed by the following ‘grammar’

```
‘discard’:
// Token is discarded
whitespace
‘punctuation’:
// e.g. , or += or ...
‘punctuation’cat
‘reserved-word’:
// e.g. true or unsigned or if
identiﬁercat
// If identiﬁercat is a reserved word
character-literal:
character-literalcat
ﬂoating-literal:
number-literalcat
// If number-literalcat is ﬂoating point
```

Phase 1 : character decoding

Phase 2 : continuation lines

Phase 3 : tokenization

Phase 5 : character set mapping

Phase 4 : preprocessing

Phase 6 : concatenation

Phase 7 : syntactic and semantic analysis

Source Program

**Figure 3.1 C++ translation**

```
integer-literal:
number-literalcat
// If number-literalcat is ﬁxed point
string-literal:
string-literalcat
identiﬁer:
identiﬁercat
// If identiﬁercat is anything else
```

#### 3.1.1.3 FOG Phase 6 Concatenation Grammar

FOG replaces phase 6 by a white-space sensitive grammar to augment the C++

- concatenation of adjacent string-literals

by

- concatenation of unseparated character-literals

- concatenation of unseparated identiﬁers

- concatenation of unseparated number-literals

- concatenation of unseparated tree-literals

```
As in C++, the distinction between ordinary and wide-string literals can be
ignored, since their adjacency leads to undeﬁned behaviour (§2.13.4-3).
Behaviour is only deﬁned for a sequence of same-width string-literals or character-
literals.
text-literalpp:
character-literalpp
identiﬁerpp
// Including all reserved words
number-literalpp
string-literalpp
tree-literalpp
// a $ or @ expression
character-literalcat:
character-literalpp
character-literalcat text-literalpp
identiﬁercat:
identiﬁerpp
identiﬁercat text-literalpp
number-literalcat:
number-literalpp
number-literalcat text-literalpp
string-literalcat:
string-literalpp
string-literalcat text-literalpp
string-literalcat whitespaceopt string-literalpp
tree-literalcat:
tree-literalpp
tree-literalcat text-literalpp
‘anything-else’cat:
‘anything-else’pp
Translation of source tokens involves three signiﬁcant textual representations1:
```

- the original source code spelling

Any escape sequences and digraphs in this representation are replaced during translation phase 5 to give

- an internal textual (multi-)byte sequence

1. There is a momentary fourth true source code spelling before trigraphs are replaced in translation phase 1.

that comprises straightforward binary encoding of each character. This representation is in turn converted during the emission phase to

- a representation suitable for output

which may require regeneration of escape sequences. This regeneration propagates the original source spelling when the output corresponds directly (no concatenation) to a source token.

```
Concatenation operates by concatenation of internal sequences without regard to
the character-literal, identiﬁer, number-literal or string-literal categorisation.
The textual byte sequence of a numeric value is the source spelling, if the value
originated from source. Otherwise the textual sequence is generated from the
numeric value using a numeric to ASCII conversion. The default formatting is
speciﬁed only to require precision not less than long for an integer-literal or float
for a ﬂoating-literal. Speciﬁc built-in meta-functions may be used for more precisely
controlled formatting.
The textual byte sequence of a tree-literal comprises the byte sequence of the main
unqualiﬁed name in the tree-literal, such as the class-name of a class-speciﬁer:
auto class_specifier c = class Scope::Class {};
auto identifier i = $c;
// i = Class
Preservation of unchanged format and conversion between identiﬁer and string-
literal perspectives provides most of the functionality of Cpp # operator stringizing.
Generalisation to character-literals and number-literals provides consistency rather
than signiﬁcant new functionality.
```

It is surprising that this fundamental lexical extension causes only very minor incompatibilities, mandating protective spaces where only a perverse coding style would omit them:

```
•
around the string-literal in a linkage-speciﬁcation: extern "C" declaration
•
around alternative tokens: and, and_eq, bitand, bitor, ..., xor, xor_eq
•
following expression preﬁxes: return, sizeof, throw
```

This is described in Section 3.3.4.

```
There is no incompatibility regarding the L preﬁx for wide-characters or wide-
strings since the L" and " are stripped during phase 3. Phase 6 processes the
contents using a parallel rather than preﬁx annotation to signify a wide-string.
```

#### 3.1.1.4 FOG Phase 7 Tokenization Grammar

The concatenated preprocessor tokens are tokenized by the following ‘grammar’

```
‘discard’:
// Token is discarded
whitespace
‘reserved-word’:
identiﬁercat
// If identiﬁercat is a reserved word
‘punctuation’:
‘punctuation’pp
character-literal:
character-literalcat
ﬂoating-literal:
number-literalcat
// If number-literalcat is ﬂoating point
integer-literal:
number-literalcat
// If number-literalcat is ﬁxed point
string-literal:
string-literalcat
meta-type-name:
// If identiﬁercat is a meta-type name
identiﬁercat
//
(and not a reserved word)
‘non-reserved-word’:
identiﬁercat
// If identiﬁercat is a non-reserved word
identiﬁer:
other-identiﬁer:
identiﬁercat
// If identiﬁercat is anything else
tree-literal:
tree-literalcat
Identiﬁers are categorised as one of reserved-word, meta-type-name (name of a built-
in meta-type), non-reserved-word (word used in a FOG syntax extension) or other-
identiﬁer. The latter three and tree-literal are combined in the main grammar for
uniform treatment where an identiﬁer is expected.
identiﬁer:
other-identiﬁer
meta-type-name
non-reserved-word
tree-literal
The results of concatenation in phase 6 may not yield consistent tokens. For
instance:
auto identifier plus = '+';
x ${plus}${plus};
```

could be perceived as

```
x ++;
If the tokenization grammar recognises that the identiﬁer ++ can be tokenized as
the corresponding punctuation token, then the example comprises an expression-
statement that increments x.
The multi-pass grammar FOG implementation took this approach, reclassifying
the byte sequences masquerading as identiﬁers into reserved-words, punctuation,
number-literals or identiﬁers. However tree-literals are not necessarily resolvable
during phase 6 without invoking a very tight coupling with syntactic and semantic
analysis.
Retokenization of identiﬁers is no longer performed and so the only change in
FOG phase 7 processing is the propagation without interpretation of tree-literals to
the syntactic analysis, where they are treated, again without interpretation, as
identiﬁers. (The justiﬁcation for treating a tree-literal as an identiﬁer is given in
Section 4.2.3.)
```

The processing is therefore context-free and the example is interpreted as

```
•
identiﬁer with value x
•
identiﬁer with value ++
•
the punctuation ;
This is a syntactically valid deﬁnition of a global variable named ++ of type x. It
must be rejected during semantic analysis.
Retokenization can be achieved by using the std::parse built-in meta-function,
using which, the increment behaviour may be obtained by:
$std::parse("x "${plus}${plus}";");
```

Whether trivial concatenations are resolved during phase 6 is an implementation option. However, certain complicated concatenations involving substitutions can only be resolved during semantic analysis.

The lack of direct support for retokenization does not introduce any incompatibility with C++, merely a limitation to the language extension. Very few programs are likely to require retokenization, and those that do will presumably only do so in a very restricted context, where the subtle behaviour may beneﬁt from the need to expose it more clearly.

#### 3.1.1.5 character-literal

```
In C++, a character-literal should comprise a single character, whereas a string-literal
may contain any number of characters.
While C++ deﬁnes the concept of a multi-character-literal, the deﬁnition is of limited
utility since it is implementation deﬁned.
In order to support concatenation consistently, FOG generalises character-literals to
encompass
multi-character-literals and the zero-character-literal to treat them
equivalently to string-literals during lexical and syntactic analysis. A character-literal
may have any number of characters, however when a character-literal with other
than one character is used in a C++ context (as a literal in a primary-expression), the
behaviour is undeﬁned.
```

This change supports concatenation consistently, but offers only minor functionality enhancements: an empty character can be used as a concatenation join or as a concatenation cast.

```
auto for (int i = 0; i < 10; ++i)
{
static char digit_values[] = char($i);
// 0 to 9
static char digit_codes[] = ''$i;
// '0' to '9'
}
```

#### 3.1.1.6 FOG tree-literals

Tree-literals support the use of a parsed AST from a meta-variable or metafunction.

Deﬁnition occurs using the meta-function or meta-variable declarations described in Section 3.1.5.5 and Section 3.1.5.6.

```
auto enum_specifier enumTree = enum f { F };
Access occurs through use of a $ or @ expression to provide a tree-literalpp token.
typedef $enumTree E;
This syntactic usage exploits the treatment of a pre-parsed AST tree-literal as an
identiﬁer by the main FOG grammar as described in Section 4.2.3.
tree-literalpp:
at-literal
dollar-literal
syntax-macro
// Section 4.7
at-literal:
// Loosely referred to as an @-expression
@ tree-expression
@ { tree-expression }
dollar-literal:
// Loosely referred to as a $-expression
$ tree-expression
$ { tree-expression }
$ dollar-literal
The ﬁrst two forms of tree-literal are triggered by a $ or @ introducer with an optional
pair of braces to surround the actual expression. Syntax ambiguities are avoided
in the absence of braces by deﬁning the lexical expression as the longest possible
token sequence. The semantics of syntax-level substitution is described in
Section 4.2. The name-resolution rules are described in Section 4.3, and the
signiﬁcance of repeated $’s in Sections 4.3.5 and 4.3.6.
The third form of tree-literal supports a user-deﬁned syntax-macro comprising a
triggering identiﬁer and relatively arbitrary subsequent syntax. Syntax macros are
described in Section 4.7.
Analysis of the tree-literal invocation sequence is initiated by recognition of the
trigger token in the white-space sensitive phase 6 concatenation grammar. This
activates a nested whitespace insensitive syntactic analysis to identify the syntax
```

tree to be returned for use by the interrupted syntactic analysis. This syntax tree represents the invocation; it is not evaluated, since the invocation may not be directly or uniquely resolvable.

```
The nested analysis to identify the tree-literal invocation obtains tokens from the
same source as, and in the same way as, each token of the interrupted analysis.
Nested recognition of tree-literals is therefore supported with $a$b$c resolved as
${a${b${c}}}.
The tree-expression deﬁning the metaobject referred to by a $ or @ expression is
deﬁned by:
primary-tree-expression:
meta-scoped-id
// Section 3.1.5.1
( tree-expression )
postﬁx-tree-expression:
primary-tree-expression
postﬁx-tree-expression ( tree-argument-listopt )
// meta-function call
postﬁx-tree-expression [ expression ]
// meta-array index
postﬁx-tree-expression . scoped-id
// meta-member selection
postﬁx-tree-expression -> scoped-id
// meta-member selection via iterator
tree-expression:
postﬁx-tree-expression
* tree-expression
// iterator indirection
meta-scoped-id is the optionally scoped name of a metaobject (in the meta-name-
space).
```

A meta-function-call invokes a user-deﬁned or built-in meta-function, globally or on a selected metaobject.

Array indexing selects a list element.

```
Member-selection selects a named member of a metaobject. Indirect member
selection and indirection apply only to metaobjects of iterator meta-type, since
these are the only form of meta-pointer.
```

**tree-arguments and tree-statements**

```
Relatively arbitrary segments of program code may be passed as pre-parsed
syntax trees to meta-functions as in the meta-function call above and as
initializers for meta-variables. These arbitrary code segments must satisfy the
syntax of a tree-argument when used in a comma-separated context, such as a meta-
function call. In the more general whitespace-separated context the tree-statement
syntax must be satisﬁed.
tree-argument-list:
tree-argument
tree-argument-list , tree-argument
tree-argument:
tree-statement
unterminated-tree-argument
tree-statement:
terminated-tree-argument
unterminated-tree-argumentopt ;
compound-tree-statement:
{ tree-statement-seqopt }
tree-statement-seq:
tree-statement
tree-statement-seq tree-statement
The constructs that may contribute to a tree-argument or a tree-statement are split into
two categories.
terminated-tree-arguments have an inherent lexical termination, enabling parsing
without any lookahead
namespace X {}
// namespace-deﬁnition does not need a ;
unterminated-tree-arguments require lookahead to the following punctuation
enum Y {}
// unterminated and so indeterminate
enum Y {};
// ... enum-speciﬁer
enum Y {} a = 0;
// ... simple-declaration
terminated-tree-argument:
asm-deﬁnition
compound-tree-statement
control-statement
declaration-statement
explicit-instantiation
explicit-specialization
expression-statement
ﬁle-dependency-declaration
ﬁle-placement-declaration
ﬁlespace-declaration
function-deﬁnition
include-declaration
linkage-speciﬁcation
namespace-alias-deﬁnition
namespace-declaration
namespace-deﬁnition
template-declaration
using-declaration
using-directive
auto meta-class-declaration
auto meta-control-declaration
auto meta-expression-statement
auto meta-function-deﬁnition
auto meta-variable-declaration
unterminated-tree-argument:
access-speciﬁer
accessibility-speciﬁer
base-speciﬁer
built-in-type-id
class-speciﬁer
condition
cv-qualiﬁer
decl-speciﬁer
enum-speciﬁer
enumerator-deﬁnition
expression
ﬁlespace-speciﬁer
function-try-block
handler-seq
initializer-clause
mem-initializer
parameter-declaration
reserved-word
simple-type-parameter
storage-class-speciﬁer
template-argument
template-parameter
type-parameter
auto meta-class-speciﬁer
When used as a tree-argument in the comma-separated tree-argument-list the extra
semicolon is optional for the unterminated productions.
$f(int a;, int b, c, d;);
When used as a tree-statement as the initializer of a meta-variable the semicolon is
mandatory for the unterminated productions.
auto statement s = { a(); b(); c(); };
auto declaration d = int a;
auto identifier i = if;
The trailing semicolon satisﬁes syntactic requirements, but has no semantic
meaning. There is therefore no semantic difference between the expression
argument in:
$meta_something(a=5)
and the expression-statement argument in:
$meta_something(a=5;)
The argument is parsed as the more ﬂexible expression, which may be converted
to an expression-statement if required to satisfy some meta-type constraint, such
as the formal meta-type of the meta_something parameter. They may be used
interchangeably as tree-arguments.
```

**Ambiguity**

```
There is an ambiguity between multiple comma-separated components of a
speciﬁc tree-argument and multiple tree-arguments. The meta-function invocation
$f(int a, b, c, if (d) e, f; else g, h;)
comprises four arguments, the fourth of which is overlined and obviously a
selection-statement, however the earlier three could be an init-declaration-list, a
parameter-declaration-clause, or perhaps an exception-speciﬁcation followed by a two
element expression. The ambiguity is not resolved until the actual arguments are
associated with formal parameters. When semantic interpretation occurs, the
declaration of f may be found to be:
auto statement f(parameter_declaration p1,
identifier p2[],
statement p3)
The available arguments are then associated from left to right, associating as
many arguments as possible with one parameter before advancing to the next
parameter. int a is associated with the parameter-declaration p1. The p2 formal
parameter can associate with an exposed list of identiﬁers (see Section 4.1.5). The
second and third arguments are syntactically valid identiﬁers and so the list
comprising b and c is associated as the list for p2. Finally the fourth argument is
associated with p3.
The ambiguity resolution resulting from unconditionally associating arguments
with the left-most parameter is consistent with similar disambiguation policies
such as §5.3.4-2 for trailing *’s in new-declarators.
In practice, meta-functions with argument lists that comprise an exposed list,
followed by further arguments, should be avoided since left to right association
may prevent subsequent arguments being passed. For instance, it is not possible
to pass an identiﬁer representing a simple expression-statement as the fourth argument
to f above since the identiﬁer would be associated with the preceding exposed list
parameter.
```

It is not possible to solve the problem by introducing parentheses or braces, since this would further overload existing punctuation and introduce ambiguities with respect to their existing meaning.

**Syntax coverage**

```
The productions listed for terminated-tree-argument and unterminated-tree-argument cover
most of the C++ constructs and exhibit signiﬁcant redundancy. In practice it is
more helpful to identify what is not covered, why it is not covered, and how the
limitation can be worked around.
```

The limitations represent very minor limitations on the language extension. They do not introduce any C++ incompatibility.

```
•
an anonymous bit-ﬁeld cannot be speciﬁed because a labeled-statement is
unconditionally preferred
-
give the bit-ﬁeld a name or
-
provide an access-speciﬁer or
-
use a class or typename preﬁx for the type
•
do followed by semicolon is presumed to be the start of an iteration
statement, so do as a reserved-word must not be followed by semicolon
-
omit the semicolon in a tree-argument
-
use "do" and implicit (or explicit) string to identiﬁer conversion
•
operator followed by a comma is presumed to be the sequencing function
name, so operator as a reserved-word cannot be followed by a list-
separating comma
-
use operator;,
•
meta-declarations in tree-arguments cannot be speciﬁed without a leading
auto to avoid ambiguity with conventional declarations and statements.
-
use auto preﬁx for all meta-declarations
-
omit auto preﬁx for all normal declarations.
•
major punctuation deﬁnes lexical structure and so cannot be passed as a
punctuation argument. This affects: { } , ; " ' ( ) #
-
use $std::parse("{")
```

#### 3.1.1.7 Design Rationale

**Lists and Trees**

Support for list (or more strictly tree structured) arguments as described above is convenient and the disambiguation rule solves some inelegant practical problems, at the expense of being a little cute.

One simpler alternative policy identiﬁes maximal length sub-list elements unconditionally without reference to the invoking context. This unfortunately requires the additional semicolons (shown with an overline) to prevent unwanted grouping:

```
$f(int a;, b, c;, if (d) e, f; else g, h;)
```

and more unacceptably requires three identiﬁers to be passed as:

```
$g(a;, b;, c)
Another alternative policy prohibits transparent passing of list elements, which is
obviously less ﬂexible. ()’s or {}’s must then be used to encapsulate the lists for
those syntaxes where bracketing is permitted. A syntax extension is needed for
other syntaxes.
```

All three policies are difﬁcult to implement in practice because a generalised superset grammar needs to treat a potential constructor argument list

```
a : b(c), d(e)
```

as an unresolved preﬁx that could be a bit-ﬁeld followed by a variable, or a constructor and initializers. The solution in Appendix C exploits the generalised monomorphic characteristic of each argument to avoid premature parsing decisions.

Comma-separated lists of arguments that may themselves comprise commaseparated lists are therefore supported, since support imposes fewer programming constraints and no extra implementation difﬁculty.

```
auto declaration f(parameter_declaration_clause p);
```

can be invoked as:

```
$f(int a, char *b, ...);
// Ellipsis token as third argument
```

**Sequences**

Sequences (unseparated lists) of arguments are not supported to avoid a syntax ambiguity between a sequence and further syntax as the initializer of a metavariable list:

```
auto statement meta_variable[] = stmt1; stmt2; stmt3;
```

This is not a limitation since a sequence can be expressed in its compound form.

```
auto statement meta_variable[] = { stmt1; stmt2; } stmt3;
```

**Labels**

```
The goto form of a labeled-statement:
label : statement
```

and an anonymous bit-ﬁeld

```
type : 5 ;
```

are ambiguous and have dissimilar syntax tree structure; a label decorates a statement, whereas a bit-ﬁeld width decorates a declarator.

```
It does not seem worth signiﬁcant effort to unify these rare constructs. The label
form is therefore excluded from a meta-control-declaration to avoid changing the
semantics of a member-declaration. However, when a bit-ﬁeld is added to an init-
declarator and consequently to a statement, the syntactic conﬂict must be resolved to
the label. The same conﬂict arises for a
tree-argument and so the same
unconditional resolution of identiﬁer : as a label is made for compatibility with C++.
This behaviour is a little surprising and so the equivalent ambiguity for tree-literal :
is resolved to the more useful bit-ﬁeld.
```

### 3.1.2 Names

FOG syntactic analysis has to be context-free while operating on both potential and actual declarations, since it is inevitable that the context cannot be known for potential declarations, for which complete identiﬁcation of all enclosing scopes is missing.

Chapter 5 shows how the traditional template and type name context dependencies are eliminated using the superset grammar.

Since FOG does not use template or type name information, it is necessary to generalise the syntax of names to eliminate the productions dependent upon semantic name classiﬁcation:

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
```

Much simpler unclassiﬁed name productions are used instead:

```
id:
identiﬁer
identiﬁer < template-argument-list >
// 2
template identiﬁer < template-argument-list >
nested-id:
id
id :: nested-id
scoped-id:
::opt nested-id
A straightforward conversion to a context-free grammar replaces the C++ usage
of xxx-name by id.
A further syntax generalisation permits declarations to appear as interface style
declarations inside class braces or implementation style declarations outside.
This requires that xxx-name be replaced by scoped-id.
```

In addition to the regular names based upon identiﬁers and templates, there are the special function names that involve punctuation characters:

```
special-function-id:
~ id
// 3
conversion-function-id
operator-function-id
nested-special-function-id:
special-function-id
id :: nested-function-special-id
scoped-special-function-id:
::opt nested-special-function-id
```

These new non-terminals replace the existing names for declarators, expressions and declarations:

```
declarator-id:
::opt id-expression
::opt nested-name-speciﬁeropt type-name
scoped-id
scoped-special-function-id
primary-expression:
literal
this
:: identiﬁer
:: operator-function-id
:: qualiﬁed-id
( expression )
id-expression
declarator-id
using-declaration:
using typenameopt ::opt nested-name-speciﬁer unqualiﬁed-id declarator-id ;
using :: unqualiﬁed-id ;
Type speciﬁers are simpliﬁed:
simple-type-speciﬁer:
::opt nested-name-speciﬁeropt type-name scoped-id
char
...
enum-speciﬁer:
enum identiﬁeropt scoped-idopt { enumerator-listopt }
```

2. Resolution of the context-dependency for < is discussed in Section 5.8.

```
3. An unqualiﬁed destructor is ambiguous with respect to a complement expression,
and very rarely valid. In practice it is easier to exclude the local destructor from
special-function-id and introduce it only once nested in nested-special-function-id.
elaborated-type-speciﬁer:
class-key ::opt nested-name-speciﬁeropt identiﬁer scoped-id
enum ::opt nested-name-speciﬁeropt identiﬁer scoped-id
typename ::opt nested-name-speciﬁeropt identiﬁer scoped-id
typename ::opt nested-name-speciﬁeropt identiﬁer < template-argument-list >
class-head:
class-key identiﬁeropt scoped-idopt base-clauseopt
class-key nested-name-speciﬁer identiﬁer base-clauseopt
base-speciﬁer:
::opt nested-name-speciﬁeropt class-name scoped-id
...
Namespace deﬁnitions are simpliﬁed dramatically after eliminating semantic
distinctions from the syntactic grammar:
namespace-deﬁnition:
named-namespace-deﬁnition
unnamed-namespace-deﬁnition
namespace scoped-idopt { namespace-body }
named-namespace-deﬁnition:
original-namespace-deﬁnition
extension-namespace-deﬁnition
original-namespace-deﬁnition:
namespace identiﬁer { namespace-body }
extension-namespace-deﬁnition:
namespace original-namespace-name { namespace-body }
unnamed-namespace-deﬁnition:
namespace { namespace-body }
```

**Impact**

The simpliﬁed naming defers semantic constraints so that the grammar deﬁnes just syntax. As a result, the grammar covers much that is illegal; in particular, every occurrence of a templated-name requires semantic validation and very occasionally correction as well.

### 3.1.3 Syntax Generalisation

Perhaps the most signiﬁcant simple language change in FOG is the relaxation of the One Deﬁnition Rule (§3.2) described in Section 4.4, so that multiple declarations compose to give an extended declaration rather than an error. This change is entirely semantic.

A further generalisation is the uniﬁcation of syntax to combine interface and implementation declarations. This is almost entirely semantic, since the C++ grammar for functions and variables embraces most of the required FOG generalisations within syntax that is only semantically invalid in C++.

Some minor syntax generalisations to resolve anomalies are described in this section. Some slightly more signiﬁcant enhancements appear in the next section.

#### 3.1.3.1 Forward declaration for namespace

A FOG declaration may ﬁrst appear as:

```
int Scope::name;
name is added to Scope, which must be a previously declared namespace or class.
C++ already allows a prior declaration of a class to be provided by
class Scope;
```

A similar declaration for a namespace should be possible. So FOG adds:

```
declaration:
// Extension of
namespace-declaration
namespace-declaration:
namespace scoped-id ;
```

**Impact**

This is an unnecessary extension. It just provides consistency without introducing any problems. A namespace could alternatively be forward declared by:

```
namespace Scope {}
```

#### 3.1.3.2 access-speciﬁers as decl-speciﬁers

```
The
public/protected/private accessibility of a member declaration is
necessarily speciﬁed in C++ by a preceding and relatively independent access-
speciﬁer. Other characteristics such as inline or static are speciﬁed by decl-
speciﬁers.
class MyClass
{
protected:
inline static void protected_method();
};
In order to avoid declarations relying on surrounding context and causing
indeterminacies during composition, FOG allows the access-speciﬁer to be speciﬁed
as part of a declaration.
decl-speciﬁer:
// Extension of
access-speciﬁer
The presence of an access-speciﬁer as part of a decl-speciﬁer affects only the speciﬁed
declaration. Subsequent declarations continue to use the prevailing default
accessibility.
class MyClass
{
protected:
public virtual void public_method();
inline static void protected_method();
};
private void MyClass::private_method();
```

**Impact**

```
This generalisation introduces a syntax ambiguity whereby
public: could
introduce an implicitly int anonymous bit-ﬁeld. There is no such thing and the
ambiguity is resolved.
```

#### 3.1.3.3 Pure-virtual

C++ allows a pure-virtual function to be declared

```
class MyClass
{
virtual int f() = 0;
};
```

and then implemented

```
int MyClass::f() { return 0; }
```

but does not allow the two declarations to be combined, so that the implementation is inlined within the interface. This strange prohibition is removed, in order to allow a complete function deﬁnition in one FOG declaration.

```
function-deﬁnition:
decl-speciﬁer-seqopt declarator pure-speciﬁeropt ctor-initializeropt function-body
decl-speciﬁer-seqopt declarator pure-speciﬁeropt function-try-block
pure-speciﬁer:
= 0
The following is invalid C++, but valid FOG.
class MyClass
{
virtual int f() = 0 { return 0; }
};
```

**Impact**

The extra term poses no additional problem to the generalised superset grammar. It could pose signiﬁcant problems to a conventional grammar. This may be the reason for the current exclusion.

#### 3.1.3.4 !static

```
The
static keyword speciﬁes whether a member function or variable is
associated with a class or an instance. This is a fundamental programming
decision that should not normally be changed as a result of composing multiple
declarations.
```

In C++ the class usage is explicitly speciﬁed, the instance usage is implicit. In order to make the instance usage explicit as well, and so ensure that any inconsistent composition is detected, the inverse behaviour of certain keywords can be speciﬁed explicitly.

```
storage-class-speciﬁer:
// Extension of
static
! static
When composing declarations it is only necessary to supply enough of the
declaration name to identify the declaration unambiguously. The remaining parts
of multiple declarations compose. Unspeciﬁed static may compose with either
static or !static. However an attempted composition of static and !static
gives an error.
```

**Implementation**

```
The ! operator in an expression using a generalised parse of a name can be
ambiguous with only one token of look-ahead.
(type) ! static a
// Cast of non-static a
(type) ! static a
// Cast of complement of static a
Neither interpretation is semantically valid, so the generalised name parsing
excludes decl-speciﬁers appearing as preﬁxes.
!const and !volatile were originally supported but withdrawn for reasons
described in Appendix F.1.1.
```

#### 3.1.3.5 !inline

```
In C++, the request to inline function may be explicitly provided in the interface or
in the implementation, depending on whether the inline keyword is associated
with the interface or the implementation declaration. FOG merges these
declarations and so the presence or absence of inline in a FOG deﬁnition cannot
express the three C++ alternatives. The syntax of inline is therefore extended to
express all three intents explicitly.
function-speciﬁer:
// Extension of
inline
! inline
inline / implementation
inline / interface
```

no inline

In the absence of any form of inline keyword, FOG must decide whether functions are to be deﬁned in the interface or implementation ﬁle.

```
class X
{
int f(int a) { return a-1; }
};
```

Functions are placed in the implementation unless all the following criteria are satisﬁed:

The function is declared as above within class braces.

The function body code is simple, on the basis of a complexity estimate formed by counting the number of accesses and operations and comparing it with a command line threshold that has a default value of 10.

The function is not virtual.

The function is not static at namespace scope.

**inline**

```
The default form of inline is interpreted in a context sensitive fashion in order to
provide compatibility with common C++ coding styles:
class X
{
inline void f1();
};
inline void X::f2() { /* ... */ }
An inline appearing within class braces as in f1 is conditionally inlined within
the interface, whereas a function such as f2 with only an inline outside class
braces is conditionally inlined in the implementation.
```

**inline/interface**

Requests that the function be inlined in the interface.

**inline/implementation**

Requests that the function be inlined in the implementation (and therefore not in the interface).

**!inline**

Speciﬁes that the function is not to be inlined in the interface or in the implementation.

**Impact**

```
Parsing of a generalised name accepts a decl-speciﬁer as a sufﬁx and so
name1 inline / interface ( name2 ) ;
could be an expression involving a division and a function call or a more explicitly
positioned inline function declaration. This is a false conﬂict since inline can
never occur in an expression, so inline followed by / is unconditionally resolved
to an extended form of inline.
```

#### 3.1.3.6 !virtual

```
The syntax for virtual is expanded to support its negation and to give an
alternative and less cryptic way of specifying pure-virtual.
function-speciﬁer:
// Extension of
virtual
! virtual
virtual / pure
```

**Impact**

```
The same ambiguity arises and the same resolution is used as for inline in
Section 3.1.3.5.
```

### 3.1.4 Syntax Enhancements

In Section 3.1.3, generalisations and very minor enhancements were introduced to support composition consistently. In this section more signiﬁcant enhancements are introduced again in support of composition.

#### 3.1.4.1 Default member initializer

In C++, it is easy for a constructor to leave member variables of simple types uninitialized. For classes with a non-trivial number of variables or constructors this can be a maintenance problem.

FOG allows one set of compositions to add member variables and another to add constructors. There is therefore ample scope for two compositions to aggravate the problem of uninitialized member variables, so FOG allows a default initializer to be speciﬁed for member variables.

The (C++) syntax for the declaration of global variables and for the deﬁnition of any variable outside a class supports initialization:

```
simple-declaration:
decl-speciﬁer-seqopt init-declarator-listopt ;
init-declarator-list:
init-declarator
init-declarator-list , init-declarator
init-declarator:
declarator initializeropt
initializer:
= initializer-clause
( expression-list )
initializer-clause:
assignment-expression
{ initializer-list ,opt }
{ }
This is incorporated into the more restrictive syntax for deﬁnition of member
variables within classes.
member-declaration:
decl-speciﬁer-seqopt member-declarator-listopt ;
member-declarator-list:
member-declarator
member-declarator-list , member-declarator
member-declarator:
declarator pure-speciﬁeropt
declarator constant-initializeropt
declarator initializeropt
identiﬁeropt : constant-expression
constant-initializer:
= constant-expression
The default initialization of member variables may be declared:
class MyClass
{
bool _satisfies_predicate = false;
int _usages[3] = { 1, 2, 3 };
That _that(*this);
};
```

**Semantics**

The default initializer provides an explicit initial value for use in every constructor that does not provide an initializer. The value is never used in a copy constructor, since a copy constructor provides an implicit initializer for each member.

C++ does not support direct initialization of array members during construction. The array initialization must therefore be synthesised by code placed at the start the constructor body, and so the construction order of array members is not deﬁned.

**Syntax Ambiguity**

```
The constructor form of initializer introduces the function-declaration/constructor-
invocation ambiguity into a class (Section 5.5.3.2).
class A
{
int a(a_type);
// member function declaration
int b(not_a_type);
// member variable and initialization
};
```

It is amenable to exactly the same resolution as outside a class. Resolution favours the declaration perspective and so preserves upward compatibility with existing C++ code.

The assignment form is clearer but unable to express multi-argument construction directly. (The result of an explicit constructor call can be used less directly.)

#### 3.1.4.2 gcc indexed array initializer

In order to support composition of arrays usefully, it is necessary to be able to specify the location of array initializers. The gcc [Stallman98] indexed array initialization syntax is therefore supported:

```
initializer-clause:
assignment-expression
[ constant-expression ] assignment-expression
{ initializer-list ,opt }
{ }
Each array initializer may be preﬁxed by an expression specifying its position.
```

**Implementation**

```
Parsing of this syntax causes no problems in a precise grammar, however an
ambiguity arises in the superset grammar, since a preﬁx [] is already recognised
to avoid a conﬂict for delete [] cast-expression. Generalising the solution to the
delete[] conﬂict, so that [ constant-expression ] is parsed as another form of cast,
accepts the indexed initializer without extra syntax.
abstract_expression:
parenthesis_clause
// Like '(' expression.opt ')'
|
'[' expression.opt ']'
cast_expression:
unary_expression
|
abstract_expression cast_expression
```

#### 3.1.4.3 compound-declaration

```
compound-declaration:
{ declaration-seqopt }
compound-declaration is introduced to support polymorphic use of multiple
declarations as a single declaration, in the same way as multiple statements can
behave as a single statement. However this is a purely lexical grouping; no nested
declarative region is deﬁned. The new region has no name; treating it as a
declarative region would prevent access to names forming part of a declaration.
The analogy is therefore with an anonymous union, whose names are externally
visible, rather than with a function block, whose names are local.
```

#### 3.1.4.4 using

```
A C++ using-declaration supports the re-use of the name of a base-class declaration
in a derived class.
class Lock
{
//...
public:
bool is_locked() const;
};
class LockableWidget : public Widget, private Lock
{
// ...
public:
using Lock::is_locked;
// Make private base class name public.
};
FOG generalises this concept in a re-using-declaration to support re-use of the name
of any declaration and re-use (extension) of an existing declaration.
class Debug
{
//...
public:
static bool diagnose();
};
class LockableWidget : public Widget, private Lock
{
// ...
public:
// Incorporate Lock::is_locked() as is_locked()
using Lock::is_locked;
// Incorporate Debug::diagnose() as show_conflicts()
using Debug::diagnose show_conflicts;
};
```

The extended syntax supports:

- signature re-use

- function placement

- built-in functionality extension

```
The speciﬁc syntax for a using-declaration is removed and covered by adding using
to decl-speciﬁer.
using-declaration:
using typenameopt ::opt nested-name-speciﬁer unqualiﬁed-id
using :: unqualiﬁed-id
decl-speciﬁer:
// Extension of
using
This generalization covers the existing syntax as part of the declarations now
contributing to:
simple-declaration:
decl-speciﬁer-seqopt init-declarator-listopt ;
function-deﬁnition:
decl-speciﬁer-seqopt declarator pure-speciﬁeropt ctor-initializeropt function-body
decl-speciﬁer-seqopt declarator pure-speciﬁeropt function-try-block
A long form of the new re-using-declaration
using A::b C::d;
// Use signature of A::b to deﬁne C::d
```

is present when

```
•
decl-speciﬁer-seqopt includes using
•
decl-speciﬁer-seqopt includes a type-speciﬁer
A short form of the new re-using-declaration,
using A::b;
// Use signature of A::b to deﬁne b
that subsumes the existing syntax and functionality of using-declaration, is present
when
•
decl-speciﬁer-seqopt includes using
•
decl-speciﬁer-seqopt includes no type-speciﬁer
```

**Signature re-use**

In deep polymorphic, or wide isomorphic object hierarchies, it is common for the same function signature to recur in many, if not all, classes in the hierarchy. This incurs a little lexical redundancy, and acts as a barrier to code evolution; a change to a function signature may involve a very substantial amount of editing. FOG allows a function signature to be deﬁned once and re-used many times, so that changes to the function declaration can be made in one place.

```
class A
{
public void protocol(int a, double b) const { /* ... */ }
};
class B : public A {
{
using B::protocol { /* ... */ }
// Short form
};
using A::protocol B::protocol { /* ... */ }
// Long form
Both using lines contribute code to the function
public void B::protocol(int a, double b) const;
The short-form creates or extends the entity that must already be unambiguously
visible with the name
B::protocol Since
A::protocol is visible as
B::protocol, the function B::protocol is therefore created with the same
signature as A::protocol.
The long-form uses the signature of A::protocol (which could be a typedef) to
create or extend the function named B::protocol. Overload resolution of
B::protocol is performed using the signature from A::protocol to select one
precisely matching alternative. There is no overload resolution for A::protocol,
however distinct names can be associated with each overload by using typedefs
to deﬁne A::protocol.
With either form, the signature of A::protocol is being re-used, enabling a
change to the signature to be made in one place. However re-used signatures
must also re-use parameter names, since there is no need for them to be
respeciﬁed. This is a little inconvenient, since parameter names now have a more
```

global import. However there may also be some advantages to the requirement for consistent parameter names in closely related functions

**Function placement and tuning of built-in functionality**

The C++ compiler may automatically generate code for

- default constructor

- copy constructor

- assignment operator

- destructor

```
•
dereferencing operator (unary &)
```

With the exception of the destructor, whose functionality is only extensible, any attempt to modify the auto-generated code requires manual re-implementation of the entire functionality. Trivial modiﬁcations such as speciﬁcation of the access, inlining, virtual or placement in a speciﬁc ﬁle should be possible.

```
The extended re-using-declaration syntax supports this, since re-using a function
involves composition with its existing functionality. Therefore within a class
declaration, where @Scope resolves to the class name:
using virtual ~@Scope;
deﬁnes the destructor as virtual, without affecting any other declaration that
speciﬁes accessibility.
using !inline ~@Scope;
```

forces an out-of-line implementation of the destructor avoiding any unwanted include ﬁle dependencies that might result from the default inline version.

```
using protected operator=(const @Scope&) { _assigns++; }
```

speciﬁes that the assignment operator is to have protected access and adds a counter update to the existing (default) functionality.

```
using @Scope(const @Scope&) : _share_count(1) {}
```

overrides the initialization of one member in a copy constructor, leaving other members unaffected and therefore retaining their default member-wise copy, whereas:

```
@Scope(const @Scope&) : _share_count(1) {}
```

speciﬁes an explicit initialization of one member, and a default initialization of all other members.

**Semantics**

```
The re-using-declaration may be elaborated with an access-speciﬁer, decl-speciﬁers,
parameter-declaration-clause, default arguments, initializers, function-bodys and object-
statement-clauses. These compose with existing declarations as described in
Section 4.4.8.
The long form of
re-using-declaration applies functionality from the source
declaration identiﬁed in the decl-speciﬁer to the target declaration identiﬁed in
(each)
init-declarator or
declarator. Source and target declarations may be
independently scoped and resolved with respect to the surrounding declarative
region.
The short form of re-using-declaration subsumes the existing using-declaration. It
speciﬁes both source and target declarations as the init-declarator or declarator. For
compatibility with the existing using-declaration, any speciﬁed scope must serve to
locate the source declaration. The target declaration is therefore necessarily part
of the surrounding declarative region.
For both forms, the name provided as the source declaration must be visible within
the surrounding declarative region. decl-speciﬁers forming part of the source
declaration are copied to the target declaration, except that conﬂicting decl-
speciﬁers are discarded in favour of those forming part of the re-using-declaration.
```

**Implementation**

```
When an overloaded signature is re-used, it is not clear which signature is
required. The deprecated ARM C++ access-declaration suffered from this problem
too. In FOG, the problem was originally solved by introducing a nick-naming
capability so that overloads could be given alternate names:
MyClass::MyClass()/overload=default_constructor
MyClass::MyClass(const @Scope&)/overload=copy_constructor
MyClass& MyClass::operator=(const @Scope&)/overload=assign
using default_constructor { /* ... */ }
However it was realised that this was unnecessary as well as clumsy. A more
generalised use of a typedef is better:
auto namespace HandySignatures
//4
{
typedef default_constructor();
typedef copy_constructor(const @Scope& thatObject);
typedef @Scope assign(const @Scope& thatObject);
};
```

supports use as

```
using HandySignatures::assign MyClass::operator= { /* ... */ }
The typedefs stretch the syntactic legality of C++, but only deﬁne what was
previously meaningless. The constructor typedefs lack a type-speciﬁer, and so
there is a potential ambiguity for
typedef a(b());
```

between

```
•
the constructor typedef of a taking a pointer to function argument
•
the redundantly parenthesised function b returning a.
```

Only the latter is valid in C++. The former is a new alternative interpretation that must be ignored. The ambiguity does not arise when the missing parameter names are speciﬁed as is necessary for the deﬁnition of a constructor signature to be useful.

```
typedef a(b (*c));
```

The typedef approach supports sharing of signatures independent of inheritance. The nickname approach needed further elaboration to support this.

```
The parameter names are retained as part of the typedef so that they form part
of the re-used signature.
```

#### 3.1.4.5 using template

```
A using keyword may preﬁx a template-specialization to indicate that the
subsequent declarations re-use and so compose with, rather than replace, the
declarations from the less specialized template.
template-declaration:
exportopt usingopt template < template-parameter-list > declaration
4. class could be used rather than auto namespace. The use of a meta-namespace
just serves to eliminate unnecessary declarations emitted for compilation.
explicit-specialization:
usingopt template < > declaration
```

**Impact**

This usage introduces a parsing lookahead problem with respect to

```
using template name < args >;
The lookahead is eliminated by accepting any decl-speciﬁer rather than just using
during syntactical analysis and then rejecting the spurious alternatives during
semantic analysis.
```

#### 3.1.4.6 Object-statement-scopes

Specialized placement of a declaration in a speciﬁc ﬁle, or accurate resolution of dependencies may require use of FOG extensions to annotate the declaration. Adding the additional syntax to support these declarations is rather difﬁcult, see Appendix F.1.

When composing declarations, it may be necessary to share meta-context between contributions. Deﬁning meta-variables at class scope to share this context can lead to unpleasant interactions when similar composition policies or meta-programs affect more than one function.

```
Both of these problems are resolved by the introduction of the concept of an
object-statement-scope: a declarative region exclusively for use at meta-compile-
time. Meta-declarations may be placed in this scope, and shared between
contributions to the object. The scope is deﬁned by an object-statements-clause, within
which, annotations can be placed without introducing syntactical conﬂicts. It is
only necessary to identify one syntax extension that does not conﬂict with existing
syntax. This is achieved by using :{ and } to delimit the region.
init-declarator:
declarator initializeropt object-statements-clauseopt
member-declarator:
declarator pure-speciﬁeropt object-statements-clauseopt
declarator constant-initializeropt object-statements-clauseopt
identiﬁeropt : constant-expression object-statements-clauseopt
object-statements-clause:
: { object-statement-seqopt }
object-statement-seq:
object-statement
object-statement-seq object-statement
object-statement:
;
initializer ;
function-used-block
ﬁle-dependency-declaration
// Appendix F.4.5
ﬁle-placement-declaration
// Appendix F.4.3
ﬁlespace-declaration
// Appendix F.4.4
meta-control-declaration
// Section 3.1.5.8
auto meta-control-declaration
// Section 3.1.5.8
auto meta-expression-statement
// Section 3.1.5.10
auto meta-function-deﬁnition
// Section 3.1.5.6
auto meta-variable-declaration
// Section 3.1.5.5
derived-clause object-statement
// Section 3.1.4.8
derived-clause : { object-statement-seqopt }
// Section 3.1.4.8
The rule involving an initializer is only semantically valid for a variable-statement-
scope (an object-statement-scope associated with a variable).
The rule involving a function-used-block is only semantically valid for a function-
statement-scope (an object-statement-scope associated with a function).
```

Section 4.3.2 describes the revised search order for meta-name resolution within the object, then the object scope and then class scopes.

**Impact**

```
The syntax is unambiguous because colon is only followed by { in C++ in the
limited context of a label-statement of the form
identiﬁer : { statement-seqopt }
The usage as an object-statements-clause has a more substantial preﬁx.
```

Care is required to avoid a shift-reduce conﬂict with only one token of lookahead.

#### 3.1.4.7 Function-statement-scopes

Object-statement-scopes or more speciﬁcally function-statement-scopes are essential for annotating the contributions to a composed function. Each contribution may have its own constructor initializers and function body reinterpreted in derived classes in accordance with a derivation rule.

The contribution may be further annotated to deﬁne its include ‘ﬁle’ dependencies, its positioning relative to other contributions and a position in the overall function structure.

```
function-used-block:
ctor-initializer ;
ctor-initializeropt function-body
function-try-block
using ﬁle-id-list function-used-block
// Appendix F.4.5
segment function-used-block
Note that a function-statement-scope is a syntactic extension of init-declarator and
consequently requires a trailing semicolon to form a
simple-declaration.
(Implementation as a function-deﬁnition leads to challenging conﬂicts.)
friend ostream& operator<<(ostream& s, const MyClass& myClass)
:{
using ostream { /* ... */ return s; }
};
Usage of commas to separate multiple declarations with object-statements-clauses
should be considered extremely bad style, however an outright prohibition
appears to add a slight complexity rather than a simpliﬁcation.
```

**Segments**

Program segments identify ﬁve distinct domains of composition. Code contributions are composed independently for each segment, but emitted as one contiguous code body. The segments are emitted in the order listed for:

```
segment:
entry
pre
body
post
exit
The body is the default segment in which code is normally placed.
entry and exit segments bracket the rest of the code. The intention is that the
entry segment contain any required declarations and the exit segment a return
statement.
pre and post are intended for passive code that wraps pre-condition and post-
condition checks or diagnostics around the active part of the function.
```

These default policies are informal. Composition of function bodies and the redeﬁnition of function structure is described in Section 4.4.8.

#### 3.1.4.8 Derivation Rules

A derivation rule determines how a potential declaration is interpreted so as to automatically generate derived declarations. A declaration is conventionally supplied for a speciﬁc scope, which is referred to as the root scope of that potential declaration. Derivation rules consider the inheritance hierarchy at and below that root scope, evaluating a predicate expression in the meta-name-space of each class to determine whether the derivation rule is enabled.

```
derived-clause:
derived ( meta-conditional-expression )
The following pair of declarations
protected inline static int Class::static_inheritance_depth()
:{
derived (is_root())
{ return 0; }
derived (!is_root())
{ return @{Super}::static_inheritance_depth() + 1; }
};
public virtual int Class::dynamic_inheritance_depth() const
:{
derived (true)
{ return static_inheritance_depth(); }
};
deﬁne a pair of functions that are implemented for Class and all its derived
classes so that invocation of dynamic_inheritance_depth() on p, a pointer to a
Class object:
Class& p = ...
... = p.dynamic_inheritance_depth();
returns the actual inheritance depth of p. The implementation of the static inline
function static_inheritance_depth() for the root scope, where the is_root()
predicate is satisﬁed, provides a function body that just returns zero. The
implementation for derived classes, where the !is_root() predicate is satisﬁed,
returns the super-class depth +1. The true predicate is always satisﬁed, and so
dynamic_inheritance_depth() is a virtual function for the root scope and all
derived classes, which ensures that the correct depth is returned.
The :{ } object-statement-scope contains a number of object-statements. The
meta-function invoked within the derivation predicate is therefore resolved with
respect
to
the
prevailing
meta-object
to
locate
the
built-in
object_statement::is_root(). As will be seen in subsequent examples a
Scope. preﬁx may be used to resolve the meta-function with respect to the scope.
The predicate meta-conditional-expression may involve user-deﬁned or built-in meta-
functions and meta-variables. Two groups of built-in functions are provided
primarily for use as derivation rule predicates. One group deﬁnes structural
predicates upon the inheritance tree. The second group deﬁnes abstract
predicates dependent upon the position of pure virtual functions in the inheritance
hierarchy.
```

**Structural Predicates**

**true**

Speciﬁes that the declaration is to be applied throughout the inheritance hierarchy: in the root scope and all its derived classes.

**object_statement::is_root()**

Speciﬁes that the declaration is to be applied to the root scope. This is the default derivation rule and ensures upward compatibility with C++.

**object_statement::is_leaf() scope::is_leaf()**

Speciﬁes that the declaration is to be applied to all classes in the inheritance hierarchy that have no derived classes. In the degenerate case, this may be just the root scope.

Leaf-ness is a class (or struct or union) property and so the information is provided by scope::is_leaf(). object_statement::is_leaf() is provided for convenience; it just delegates the inquiry to the scope.

Leaf-ness is independently determined from the types declared during each metacompilation session. There is no overall global view across meta-compilation sessions, so if further derivation occurs unknown to one session, leaf-based decisions may be inaccurate in another.

More complicated conditions can be built-up using expression operators:

```
derived(!is_root() && !is_leaf())
```

selects all intermediate nodes in an inheritance hierarchy.

**Abstract Predicates**

```
Abstract predicates support efﬁcient and appropriate generation of code
depending on the presence of pure virtual functions. The !is_pure() predicate
may be used to avoid generation of code that illegally attempts to new an abstract
class. The is_boundary() predicate may place code just once at the inheritance
boundary between abstract and concrete classes.
```

**scope::is_pure()**

Speciﬁes that the declaration is to be applied to all classes in the inheritance hierarchy that have at least one pure virtual function.

**scope::is_boundary()**

Speciﬁes that the declaration is to be applied to the least derived class in the inheritance hierarchy derived from the root scope for which there are no pure virtual functions.

**function::is_boundary()**

Speciﬁes that the declaration is to be applied to the least derived class in the inheritance hierarchy derived from the root scope for which the virtual function is not pure.

**Semantics**

In the absence of a derivation rule, a declaration contributes to its root scope.

With a derivation rule, the declaration name is provisionally present in the root scope and all derived scopes. Once it can be determined that the derivation predicate cannot be satisﬁed, the declaration is disabled, and consequently is not emitted.

```
Some predicates, such as true or is_root(), can be evaluated immediately.
Other predicates, such as is_leaf() or Scope.is_pure() can be affected by
further declarations or meta-programming and so cannot be evaluated promptly.
Resolution is therefore automatically deferred until the code emission phase of
meta-compilation. Deferred evaluation of user-deﬁned predicates may be
enforced by use of an @ operator to delay evaluation until the body is resolved
during code emission.
```

The derivation predicate gates the body of a declaration

```
•
function-body and return type of a function
•
ctor-initializers of a constructor
•
initializer and type of a variable
```

- value of a typedef

```
The body is not evaluated until the predicate has been resolved, thereby avoiding
problems that might occur through use of invalid manipulations, such as a base-
class (@Super) of a root scope.
Deferred evaluation of a body is only possible if the body is not used. References
to the body must therefore also use the @ operator to defer evaluation. Direct use
of the body such as the use of a typedef to deﬁne a function signature are an error
if the body is not immediately resolvable.
```

**Implementation**

The presence of derivation rules dependent upon the abstract context of a class leads to a potential ambiguity:

```
class Root
{
virtual void f1() = 0
:{
derived(@is_boundary())
{}
};
};
class Branch : public Root {};
class Leaf : public Branch {};
Which, if any, of Root::f1, Branch::f1 and Leaf::f1 should be implemented to
deﬁne the boundary? The Root class has an explicit pure virtual, is clearly
abstract and so cannot constitute a boundary. Implementation of none or one of
Branch::f1 and Leaf::f1 gives a consistent behaviour.
FOG implements Branch::f1 as a result of compiling classes in a least-derived
order, and of making the presumption that an ambiguously pure/concrete class is
concrete while evaluating the derivation predicates of its members. This ensures
that practical problems yield a stable solution. Since the predicates may be
arbitrary expressions, a predicate may introduce a contradiction for which an error
message is produced.
```

**Historical Note**

Derivation rules were introduced in FOG before meta-programming. They offer little that cannot be achieved by meta-programming, but do so with a much more compact and manageable syntax for many common cases.

### 3.1.5 Meta-Programming

Meta-programming extends C++ run-time behaviour by providing very similar behaviour at (meta-)compile-time.

In order to introduce additional functionality without new reserved words, the existing syntax is heavily overloaded, which has the advantage of requiring very little new syntax to be learnt, but incurs the risk that the rather different behaviour may be overlooked.

```
The auto keyword is used to introduce meta-functionality. Its existing usage is
only valid as a decl-speciﬁer within a function and so all other usage of auto is
retracted, at no cost to semantically valid programs.
storage-class-speciﬁer:
// Part of
auto
The apparent loss of support for auto for local variables in functions is resolved
by broadening the replacement meta-expression-statement syntax to cover the old
usage and then recognising the old style usage during semantic analysis.
```

**Integration**

FOG introduces a number of new distinct syntaxes and generalises some existing syntaxes. In order to see how these integrate with the C++ grammar, the three contexts in which the C++ grammar supports a diverse range of syntaxes are presented.

**namespace statements**

```
Contributions to namespaces occur as a sequence of declarations within the top-
level (unnamed global namespace), within namespaces, and within external
linkages:
declaration-seq:
declaration
declaration-seq declaration
translation-unit:
declaration-seqopt
namespace-body:
declaration-seqopt
linkage-speciﬁcation:
extern string-literal { declaration-seqopt }
extern string-literal declaration
```

**member statements**

```
Contributions to classes occur as a sequence of member-declaration
member-speciﬁcation:
member-declaration member-speciﬁcationopt
access-speciﬁer : member-speciﬁcationopt
class-speciﬁer:
class-head { member-speciﬁcationopt }
```

**function statements**

```
Contributions to function bodies occur as a sequence of statement
function-body:
compound-statement
compound-statement:
{ statement-seqopt }
statement-seq:
statement
statement-seq statement
C++ contributions therefore arise as a declaration, member-declaration or statement.
FOG adds a fourth context in which object-statements contribute to an object-statement-
clause in order to qualify the behaviour of a function, typedef or variable (see
Section 3.1.4.6). A ﬁfth generic context arises when almost arbitrary syntax is
parsed as a tree-statement to form a tree-literal (see Section 3.1.1.6).
```

Meta-functionality extends each of the three existing contexts and contributes to the two new contexts. The capabilities of each context are difﬁcult to grasp from the grammar and so the contributions to each context are tabulated in Table 3.1.

The ﬁrst column identiﬁes the nature of a contribution and the ﬁve subsequent columns show how that contribution applies to each context.

```
a. access-declaration is deprecated and therefore now appears as just qualiﬁed-id in
```

the grammar.

```
b. access-speciﬁer : is syntactically a member-declaration since it is interchangeable in
its only usage which is as a sequence of member-declarations in a member-speciﬁ-
cation.
Non-terminal
declaration
member-
declaration
statement
object
statement
tree
statement
Compound Statements
compound-declaration
FOG
FOG
compound-statement
C++
compound-tree-statement
FOG
Control Statements and Declarations
control-statement
try-block
```

C++ FOG

```
meta-control-declaration
FOG
FOG
auto
FOG
auto
Expression Statements
expression-statement
C++
FOG
meta-expression-statement
FOG
FOG
auto
FOG
auto
Declaration Statements
function-deﬁnition
template-declaration
```

C++ C++ FOG

```
using-declaration
C++
C++
C++
FOG
simple-declaration
‘simple-’member-declaration
```

C++ C++ C++ FOG

```
=assignment-expression
C++
FOG
C++
FOG
={ ... }
C++
FOG
C++
FOG
(expression-list)
C++
FOG
C++
FOG
=constant-expression
(C++)
C++
(C++)
(FOG)
bit-ﬁeld
FOG
C++
FOG
meta-class-declaration
meta-function-deﬁnition
meta-variable-declaration
```

FOG FOG FOG FOG

```
Others
explicit-instantiation
explicit-specialization
linkage-speciﬁcation
namespace-deﬁnition
```

C++ FOG FOG

```
asm-deﬁnition
namespace-alias-deﬁnition
using-directive
```

C++ FOG C++ FOG

```
access-declarationa
access-speciﬁer :
```

FOG C++b FOG

```
ﬁle-dependency-declaration
ﬁle-placement-declaration
ﬁlespace-declaration
```

FOG FOG FOG FOG

```
include-declaration
namespace-declaration
```

FOG FOG FOG

```
syntax-macro-deﬁnition
FOG
FOG
‘most-other-things’
FOG
```

**Table 3.1 Statement and Declaration Grammar**

```
The contributions to simple-declaration and its member-declaration counterpart are
further split into additional rows that reﬂect the alternate forms of initialization:
scalar initializer (run-time evaluation), array initializer, constructor, scalar
initializer (compile-time evaluation) and bit-ﬁeld.
```

Boxes marked

```
•
C++ identify a contribution in the standard C++ grammar
•
FOG identify FOG extensions
•
auto identify FOG extensions disambiguated by an auto keyword
•
() identify degenerate grammar covered by another contribution
Thus a standard C++ member-declaration covers contributions from a function-
deﬁnition, template-declaration, using-declaration, member-declaration (without initializer,
with a constant-expression initializer, or with a bit-ﬁeld), access-declaration and access-
speciﬁer.
FOG removes the syntactic distinction between declaration and member-declaration,
and adds meta-functionality, resulting in the many further contributions shown by
the boxes annotated as FOG.
The syntax of declarations is extended by the addition of meta-expression-statements
and meta-control-declarations so that conditional and iterated compilation may
embrace declarations. The existing control-statement syntax is re-used as is, with the
result that declaration/expression ambiguities now occur at the declaration as well
as statement level; extension of the existing disambiguation rule (§6.8) to favour
declarations preserves compatibility.
gotos and associated statement labels are not re-used in meta-control-declarations,
primarily because the label syntax has a challenging ambiguity with respect to an
anonymous bit-ﬁeld.
Since meta-control-declarations re-use control-statement syntax, they cannot be added
directly to statement syntax. Boxes labelled auto therefore indicate where an auto
preﬁx is necessary to disambiguate between a standard C++ construct and
extended FOG meta-construct.
Meta-variables and meta-functions are added by overloading the auto keyword to
mean “meta”. Since the auto keyword has no meaning outside of functions this
change merely gives meaning to constructs that are meaningless in C++.
```

In lexical form, the FOG extensions are:

```
statement:
// Extension of
auto control-statement
auto meta-expression-statement
control-statement:
// New non-terminal with old functionality
labeled-statement
selection-statement
iteration-statement
jump-statement
declaration:
// Extension of
namespace-declaration
// Section 3.1.3.1
accessibility-speciﬁer
compound-declaration
// Section 3.1.4.3
meta-control-declaration
// Section 3.1.5.8
auto meta-control-declaration
// Section 3.1.5.8
expression-statement
// Section 3.1.5.10
auto meta-expression-statement
// Section 3.1.5.10
auto meta-class-declaration
// Section 3.1.5.2
auto meta-function-deﬁnition
// Section 3.1.5.6
auto meta-variable-declaration
// Section 3.1.5.5
syntax-macro-deﬁnition
// Section 4.7
include-declaration
// Appendix F.4.6
ﬁle-dependency-declaration
// Appendix F.4.5
ﬁle-placement-declaration
// Appendix F.4.3
ﬁlespace-declaration
// Appendix F.4.4
The syntactic difference between declaration and member-declaration is eliminated and
consequently the member-declarator syntax must be added to init-declarator and an
accessibility-speciﬁer to declaration. The changes are syntactic only; C++ declarations
that are not syntactically valid as member-declarations, and conversely, C++ member-
declarations that are not syntactically valid as declarations, are semantically invalid
in FOG. Accessibility and bit-ﬁelds may not be speciﬁed for namespaces and
explicit-instantiations or using-directives may not be speciﬁed within classes.
init-declarator:
declarator pure-speciﬁeropt object-statements-clauseopt
declarator initializeropt object-statements-clauseopt
identiﬁeropt : constant-expression object-statements-clauseopt
class-speciﬁer:
class-head { member-speciﬁcationopt declaration-seqopt }
accessibility-speciﬁer:
access-speciﬁer :
member-speciﬁcation:
member-declaration:
member-declarator-list:
member-declarator:
```

#### 3.1.5.1 Meta-names

```
The built-in types have meta-classes and the auto meta-class is the root of all
meta-classes. We therefore deﬁne names that incorporate these alternative
scopes, including their meta-constructors and meta-destructors.
built-in-type-id:
// e.g. unsigned int
built-in-type-speciﬁer
built-in-type-id built-in-type-speciﬁer
meta-id:
id
meta-type
auto
meta-nested-id:
meta-id
~ meta-id
meta-id :: meta-nested-id
meta-scoped-id:
// e.g. ::auto::symbol_table
::opt meta-nested-id
```

#### 3.1.5.2 Meta-classes

Section 4.5 describes how every user-deﬁned and built-in type has a meta-class with the same name. The meta-class is discarded after meta-compilation completes, so that the meta-class forms no part of the emitted code. The metaclass deﬁnes additional functionality for use at compile-time.

```
Meta-classes for classes that have no declarations may not be needed at compile-
time and so such classes may not need emission. In order to forward reference
such a meta-class, or to diagnose any inadvertent use of compile-time
declarations, an auto may preﬁx a very similar syntax to a class-speciﬁer. This
asserts that the meta-class alone is required, avoiding an empty class declaration
cluttering the generated output.
meta-class-id:
meta-id
meta-id :: meta-class-id
meta-class-key:
class-key
namespace
meta-class-speciﬁer:
meta-class-key meta-class-id
meta-class-key meta-class-id base-speciﬁer-clauseopt { declaration-seqopt }
meta-class-declaration:
meta-class-speciﬁer ;
declaration:
// Extension of
auto meta-class-declaration
access-speciﬁers are ignored for meta-declarations in a meta-class. All meta-
declarations are therefore public. The presumption is that meta-functionality
contributes to a tightly coupled pool of collaborating code, where access
restrictions would be an inconvenience rather than an asset.
```

**Impact**

```
Re-use of reserved-words such as class as meta-type-names creates some
inconvenient ambiguities with respect to meta-variable-declaration and meta-function-
deﬁnitions. Most of these can be resolved by careful implementation to share
common parsed preﬁxes. It is not obvious how to solve the problem of a global
name as a meta-class-id, for which the leading :: in
auto class ::MyClass { /* ... */ };
signiﬁes the nested meta-class class::MyClass of the class meta-type.
```

Introducing parentheses:

```
auto class (::MyClass) { /* ... */ };
satisﬁes a generalised syntax for the meta-constructor of the class meta-type.
However this cannot be uniﬁed because the body of a meta-constructor comprises
statements whereas the body of a meta-class comprises declarations.
Therefore FOG does not support explicitly global scoping in meta-class-speciﬁers. An
inelegant workaround is:
auto identifier globalScopeId = "";
auto class ${globalScopeId}::MyClass { /* ... */ };
```

#### 3.1.5.3 Base Meta-classes

```
Every user-deﬁned and built-in type has a corresponding meta-class, whose
meta-inheritance corresponds to the compile-time inheritance. The base meta-
classes are the meta-classes of the base-classes. The meta-inheritance is
augmented so that every meta-class without a base meta-class meta-inherits from
the built-in root meta-class named auto. Additional meta-inheritance may be
speciﬁed by using the auto keyword as an access-speciﬁer in a base-speciﬁer.
base-speciﬁer:
::opt nested-name-speciﬁeropt class-name
virtual access-speciﬁeropt ::opt nested-name-speciﬁeropt class-name
access-speciﬁer virtualopt ::opt nested-name-speciﬁeropt class-name
scoped-id
built-in-type-id
access-speciﬁer base-speciﬁer
virtual base-speciﬁer
! virtual base-speciﬁer
auto base-speciﬁer
The rewritten recursion removes the syntactic limitations on multiple access-
speciﬁers. It is therefore a semantic error for more than one of auto and the three
distinct access-speciﬁers or for both virtual and !virtual to be supplied.
The virtual keyword is ignored for base meta-classes whose behaviour is always
virtual; only one copy of a meta-class is inherited.
```

#### 3.1.5.4 Meta-types

The arguments and returns of meta-functions and the values of meta-variables are deﬁned by meta-types. All meta-types are built-in and there is no facility for userdeﬁned meta-types. (User-deﬁned classes are not meta-types, although extending meta-class deﬁnitions to support user-deﬁned assignment might be a logical extension.) The deﬁned set is

```
meta-type:
meta-type-name
built-in-type-id
meta-class-key
enum
typedef
typename
using
The direct mention of typedef, and indirect mention of class and signed re-
instates names that duplicate reserved words and which were consequently
tokenized as a reserved-word rather than a meta-type-name (Section 3.1.1.4). All the
built-in C++ numeric types are available for use as meta-types for compile-time
calculations. (The current FOG implementation maps the 20 distinct C++ types to
one of: bool, unsigned, signed or double.)
meta-type-name:
intrinsic-meta-type-name
actual-meta-type-name
potential-meta-type-name
The intrinsic meta-types deﬁne concepts that do not depend upon their program
context.
intrinsic-meta-type-name:
one of
array_modifier
assignment_expression
character
class_key
constant_expression
cv_qualifier
decl_specifier
declaration
expression
function_modifier
handler
identifier
initializer_clause
iterator
keyword
meta_type
modifier
name
number
pointer_modifier
punctuation
reference_modifier
reserved
scoped_modifier
statement
string
template_argument
token
tree_literal
using_directive
Potential meta-types deﬁne concepts that have limited meaning until associated
with some parent context.
potential-meta-type-name:
one of
base_specifier
built_in_type_specifier
class_specifier
elaborated_type_specifier
enum_specifier
file_dependency_specifier
file_placement_specifier
enumerator_definition
exception_specification
filespace_specifier
function_specifier
linkage_specification
meta_class_specifier
meta_function_specifier
meta_parameter_specifier
meta_variable_specifier
namespace_definition
namespace_alias_definition
object_specifier
object_statement
parameter_specifier
scope_specifier
specifier
template_parameter_specifier
templated_parameter_specifier type_parameter_specifier
type_specifier
typedef_specifier
using_declaration
value_parameter_specifier
variable_specifier
Actual meta-types deﬁne concepts that have been associated with a parent
context, and in many cases correspond to emitted declarations.
actual-meta-type-name:
one of
// 5
base
built_in
class
entity
enum
enumerator
exception
filespace
function
linkage
meta_class
meta_function
meta_parameter
meta_variable
namespace
namespace_alias
object
parameter
scope
struct
template_parameter
type
typedef
typename
union
using
variable
The semantics of these types is deﬁned in Section 4.1.2.
```

Although meta-types are built-in, their names are not reserved words. There is therefore no compatibility problem when migrating C++ code that makes use of some of the meta-type names for its own identiﬁers. It is just a little confusing to read:

```
void f()
{
bool moveable;
typedef bool variable;
variable t = moveable;
// Assign value to run-time type
auto variable t = moveable;
// Assign name to compile-time meta-type
}
```

The values stored in meta-variables may be scalars or trees exploiting the polymorphic characteristics available at compile that are described in Section 4.1.5.

#### 3.1.5.5 Meta-variables

```
Meta-variables support storage of values at compile-time. Their deﬁnition differs
from conventional variables through the use of an auto preﬁx, the requirement for
an initializer, and the acceptance of almost any syntactically valid construct as
that initializer.
meta-variable-declaration:
staticopt constopt meta-type meta-scoped-id exposed-treeopt = tree-statement
staticopt constopt meta-type meta-scoped-id exposed-treeopt object-statements-clause
staticopt constopt meta-type ( meta-scoped-id ) exposed-treeopt = tree-statement
staticopt constopt meta-type ( meta-scoped-id ) exposed-treeopt object-statements-clause
exposed-tree:
[ ]
declaration:
// Extension of
auto meta-variable-declaration
When []’s are omitted, the syntax deﬁnes a scalar meta-variable with a single
initializer. The single initializer may be a tree of initializers with compound-tree-
statements used as tree-statements to create the tree structure. All leaves in the
initializer tree must satisfy the syntax of the meta-type. When subsequently
assigned to an iterator, the iteration domain comprises the one root element.
When []’s are present, the syntax similarly deﬁnes a meta-variable but from a
compound initializer. When subsequently assigned to an iterator, the iteration
domain comprises the ﬁrst generation of children, thereby treating the tree as an
array.
```

The semantics of the composed list and tree types are discussed in Section 4.1.5.

5. Some meta-type names are also reserved words. The usage as a meta-type name augments usage as a reserved word.

```
static meta-variables have a single value shared by all derived meta-classes.
!static meta-variables have distinct copies for each derived class.
const meta-variables may have only a single unchanging value. Assignment or
redeclaration is illegal.
non-const meta-variables may change value either by assignment or by
composition with a further declaration.
Meta-variables are non-const and !static by default.
```

**Impact**

```
The
meta-expression-statement
and
meta-variable-declaration
syntaxes
exhibit
expression/declaration ambiguities. The existing disambiguation rule is extended
to resolve an ambiguity in favour of the meta-declaration.
```

Members of the meta-classes of the built-in types and members of globally scoped meta-classes can be difﬁcult to specify:

```
auto int int::a = 0;
auto int ::MyClass::a = 0;
In the ﬁrst case, the disambiguation rule for built-in types maximises the length of
built-in type speciﬁers, treats int int as a single type and so the example is an
assignment expression.
In the second case, extrapolation from the §7.1-2 disambiguation rule, that
maximises the length of a type-speciﬁer-seq, maximises the length of the alternating
names and scopes and so once again the example is an assignment to the
semantically illegal nested class of int.
```

Parentheses must be used to deﬁne valid declarations.

```
auto int (int::a) = 0;
auto int (::MyClass::a) = 0;
```

Each of these examples is now syntactically valid as both a meta-expression and a meta-declaration, so the disambiguation rule resolves in favour of the required meta-declaration.

There is no ambiguity with respect to meta-constructor declarations, since there are no pure virtual meta-functions and meta-constructors have no parameters.

#### 3.1.5.6 Meta-functions

Meta-functions provide code for execution during the meta-compilation process.

```
declaration:
// Extension of
auto meta-function-deﬁnition
meta-function-deﬁnition:
// Part of
staticopt meta-type meta-scoped-id ( meta-parameter-listopt ) exposed-treeopt
compound-tree-statement
staticopt meta-type meta-scoped-id ( meta-parameter-listopt ) exposed-treeopt
object-statements-clause
meta-parameter-list:
meta-parameter
meta-parameter-list , meta-parameter
meta-parameter:
meta-type identiﬁer exposed-treeopt
meta-type identiﬁer exposed-treeopt = tree-argument
static meta-functions have a single copy shared by all derived classes.
!static meta-functions have distinct copies for each derived class.
The distinction between the two is relatively subtle given that a single instance
always exists and so there is always a meta-object available. The non-static
meta-function operates in the derived scope and so any use of $Dynamic returns
```

the derived scope, and any access to a meta-variable is made with respect to the derived scope.

The syntax is very similar to a conventional function declaration, except for the use of the much simpler and restrictive meta-type system. Default parameter values are supported, but not overloading or exception speciﬁcations.

The lack of support for overloading was once necessary because resolution of the meta-function was necessary to identify the syntax with which each argument was parsed. The evolution to a context free grammar, as described in Chapter 5, removes this constraint. Overloading could now be supported. It is just a matter of deﬁning overload resolution rules that are in keeping with existing C++ overload resolution policies, but which are also appropriate for the dynamic rather than static type information available during meta-compilation.

An alternative form of deﬁnition and invocation is provided by the syntax-macro deﬁned in Section 4.7.

#### 3.1.5.7 Meta-constructor and Meta-destructor

Meta-constructors and meta-destructors provide for relatively independent metaprograms. They are invoked automatically during the meta-construction and metadestruction compilation phases. They therefore have no parameters.

```
meta-nested-constructor-id:
meta-id
meta-id :: meta-nested-constructor-id
meta-scoped-constructor-id:
::opt meta-nested-constructor-id
meta-nested-destructor-id:
~ meta-id
meta-id :: meta-nested-destructor-id
meta-scoped-destructor-id:
::opt meta-nested-destructor-id
meta-function-deﬁnition:
// Part of
meta-scoped-constructor-id ( ) compound-tree-statement
meta-scoped-constructor-id ( ) object-statements-clause
meta-scoped-destructor-id ( ) compound-tree-statement
meta-scoped-destructor-id ( ) object-statements-clause
```

#### 3.1.5.8 Meta-control-statements and meta-control-declarations

```
Meta-statements control compilation. The control part of a meta-control-statement or
meta-control-declaration is evaluated, as the source text is analysed or a meta-
program executed, to affect the interpretation of the child statement or declaration.
Within a statement-seq, the conventional program control statements retain their run-
time meaning. Additional meta-programming control applies when an auto preﬁx
is used.
statement:
// Extension of
control-statement
auto control-statement
control-statement:
labeled-statement
selection-statement
iteration-statement
jump-statement
Within a declaration-seq, C++ provides no program control and so the existing
statement syntax is re-used with an optional auto preﬁx to deﬁne a meta-control-
declaration.
declaration:
// Extension of
meta-control-declaration
auto meta-control-declaration
meta-control-declaration:
case constant-expression : declaration
default : declaration
do declaration while ( expression ) ;
for ( for-init-statement conditionopt ; expressionopt ) declaration
if ( condition ) declaration
if ( condition ) declaration else declaration
switch ( expression ) declaration
while ( condition ) declaration
jump-statement
The meta-control-declaration syntax repeats nearly all the program control syntax of
statement replacing child statements by child declarations. The goto form of labeled-
statement is omitted to avoid introduction of a syntax ambiguity with respect to an
anonymous bit-ﬁeld. goto is therefore not supported for meta-programming.
A return meta-statement has no meaning, since the semantics of meta-function
execution involve a return of the entire meta-function body as a tree for
interpretation in the calling context (Section 4.3).
```

**Impact**

Interspersing compile-time and run-time control apparently introduces more ambiguities:

```
do
{
if (...)
{
auto do
{
auto if (... ) {}
} while (...);
}
else {...}
} while (...);
How do the ifs, elses, dos and whiles pair up?
The meta-syntax is integrated with the language, and statements nest. The pairing
is therefore exactly the same as would be the case with the auto keywords
removed and all meta-statements changed to statements. The dangling else
ambiguity is resolved as always to the nearest if, or auto if.
It is unnecessary and highly undesirable to introduce an auto preﬁx for else or
the while of do...while. This would permit the meta-programming control ﬂow
to interleave programming control ﬂow. Although this is permitted by the C
preprocessor, it leads to difﬁcult to understand code. The following example
demonstrates how preprocessor directives can be used in a very unstructured
fashion.
if
{
a();
#if B
}
else
{
#endif
b();
#if !B
}
else
{
#endif
c();
}
```

The distinct preprocessor syntax makes the example comprehensible. If this were permitted in FOG, the use of almost identical syntax for meta-statements and statements would render interleaved code unintelligible.

```
There is also a pragmatic reason to avoid preﬁxing else with auto; 2 tokens of
lookahead are required to resolve a dangling auto else ambiguity.
The declarations in selection and iteration meta-statements have a data-dependent
interpretation. These productions must therefore be parsed with the appropriate
number of side-effects. Section 4.2.1 describes how this may be achieved either
by using a simple lookahead parser to cache the unparsed tokens for deferred
parsing under control of the parent statement. Alternatively, AST nodes may be
constructed directly, but without any resolution of tree-literals. Resolution of tree-
literals is initiated by the parent statement. The earlier multi-pass FOG
implementation was context-dependent and took the former approach. The
superset grammar is context-free and takes the latter.
```

#### 3.1.5.9 Meta-typedef

A typedef deﬁnes a more convenient name for a run-time type during compilation. That name has no meaning at run-time.

A meta-typedef could therefore deﬁne a more convenient name for a compile-time type during (meta-)compilation. That name also has no meaning at run-time.

```
meta-typedefs are not currently implemented in FOG. There is no obvious reason
why they should not be. Arguably, the typedefs in HandySignatures on page 54
are meta-typedefs.
```

#### 3.1.5.10 Meta-expression-statement

```
A meta-expression-statement takes two forms, each of which must be preceded by an
auto preﬁx to distinguish from the very similar syntax of an expression-statement in
contexts where an ambiguity could arise: function bodes and tree-literals. In other
contexts (declarations) an auto preﬁx is optional.
```

The ﬁrst form performs some operation as a side effect:

```
auto a++;
auto f();
```

The second form updates a meta-variable

```
auto a = 5;
auto listVariable += { "a", a * b, g[] };
meta-primary-expression:
literal
this
meta-scoped-id
meta-type meta-nested-id
( tree-argument-listopt )
meta-postﬁx-expression:
meta-primary-expression
meta-postﬁx-expression ( tree-argument-listopt )
meta-postﬁx-expression [ expressionopt ]
meta-postﬁx-expression . declarator-id
meta-postﬁx-expression -> declarator-id
meta-postﬁx-expression ++
meta-postﬁx-expression --
meta-unary-expression:
meta-postﬁx-expression
++ meta-unary-expression
-- meta-unary-expression
* meta-unary-expression
+ meta-unary-expression
- meta-unary-expression
! meta-unary-expression
~ meta-unary-expression
sizeof unary-expression
meta-multiplicative-expression:
meta-unary-expression
meta-multiplicative-expression * meta-unary-expression
meta-multiplicative-expression / meta-unary-expression
meta-multiplicative-expression % meta-unary-expression
meta-additive-expression:
meta-multiplicative-expression
meta-additive-expression + meta-multiplicative-expression
meta-additive-expression - meta-multiplicative-expression
meta-shift-expression:
meta-additive-expression
meta-shift-expression << meta-additive-expression
meta-shift-expression >> meta-additive-expression
meta-relational-expression:
meta-shift-expression
meta-relational-expression < meta-shift-expression
meta-relational-expression > meta-shift-expression
meta-relational-expression <= meta-shift-expression
meta-relational-expression >= meta-shift-expression
meta-equality-expression:
meta-relational-expression
meta-equality-expression == meta-relational-expression
meta-equality-expression != meta-relational-expression
meta-and-expression:
meta-equality-expression
meta-and-expression & meta-equality-expression
meta-exclusive-or-expression:
meta-and-expression
meta-exclusive-or-expression ^ meta-and-expression
meta-inclusive-or-expression:
meta-exclusive-or-expression
meta-inclusive-or-expression | meta-exclusive-or-expression
meta-logical-and-expression:
meta-inclusive-or-expression
meta-logical-and-expression && meta-inclusive-or-expression
meta-logical-or-expression:
meta-logical-and-expression
meta-logical-or-expression || meta-logical-and-expression
meta-conditional-expression:
meta-logical-or-expression
meta-logical-or-expression ? meta-conditional-expression : meta-conditional-expression
meta-expression-statement:
meta-conditional-expression ;
meta-logical-or-expression assignment-operator tree-statement
The expression syntax is repeated so that meta-primary-expression resolves meta-scoped-
id in the meta-name-space, whereas primary-expression resolves declarator-id in the
conventional name-space. The semantics of meta-assignment differ and some
inappropriate operators are omitted. A conventional assignment is right
associative. Therefore:
a = b = c = d;
```

is equivalent to:

```
a = (b = (c = d));
A meta-assignment does not associate. The entire right-hand side is analyzed as
a tree-statement and is assigned to the left-hand side as a literal syntactic element:
auto a = b = c = d;
assigns b = c = d to a. Of these names, only a is resolved in the meta-name-
space. b, c and d are not resolved in any namespace, since they form part of a
literal. Eventually when the value of a is used, the name-space in which to resolve
b, c and d may become clear.
$a;
// b = c = d;
// b, c, d in normal name-space.
auto $a;
// auto b = c = d;
// b resolved in meta-name-space
// c and d still literals
The generality required to parse the tree-statement necessitates a simpliﬁcation of
meta-conditional-expression to exclude assignments and comma-separated lists of
meta-expressions.
```

The meta-expression grammar has the following further incidental differences:

- no casts

```
•
no typeid
•
no new
•
no delete
•
no unary &
•
no .* or ->*
```

Removing explicit casts simpliﬁes the implementation. The remaining constructs are inappropriate for the current language deﬁnition.

**Impact**

```
The
meta-expression-statement
and
meta-variable-declaration
syntaxes
exhibit
expression/declaration ambiguities. The existing disambiguation rule (§6.8) is
extended to resolve an ambiguity in favour of the meta-declaration.
The tree-statement is initially analyzed without knowledge of the expected syntactic
type. When assigned to a meta-variable, used as a meta-parameter or returned
from a meta-function, a further analysis checks that the value is compatible with
the required meta-type. The program is ill-formed if it is incompatible.
void f()
{
int x;
int y;
y = x;
// Ok expression
auto class_specifier x = class X {};
// Ok class-speciﬁer init
auto class_specifier y = class Y {};
// Ok class-speciﬁer init
auto y = class X {};
// Ok class-speciﬁer assign
auto y = $x;
// Ok class-speciﬁer assign
auto y = x;
// Error x is integer
auto y = class X {};
// Ok class-speciﬁer assign
y = $x;
// Error not expression
$y z;
// Ok z of local class
$y x;
// Error redeclaration of x
$y = x;
// Error assign int to class
$y = $x;
// Error assign class to class
}
A meta-expression-statement replaces and must therefore cover the syntax for a local
function variable with an auto preﬁx. This requires the practical implementation of
the above grammar to add unary & to support local reference variables, and accept
cv-qualiﬁers following * to support local pointer variables.
```

## 3.2 Built-In Functionality

The built-in functionality is described at length in Appendix E, and so only a very brief summary is provided here.

### 3.2.1 Built-in Root Meta-class

```
auto is the root meta-class for all other meta-classes (including those for
namespaces and built-in types). It has no functionality, but user-deﬁned
functionality can be added and thereby affect all classes.
```

### 3.2.2 Built-in Meta-variables

Meta-type-speciﬁc meta-variables support access to declarations.

### 3.2.3 Built-in Meta-functions

Meta-type-speciﬁc meta-functions support access to declarations.

### 3.2.4 std meta-namespace

```
The std meta-namespace is used as a repository for useful language support
meta-functions.
```

static bool std::ambiguous(expression aName) static bool std::defined(expression aName) static token std::find(expression aName) []

```
interrogate the meta-name-space to determine whether a reference to aName is
ambiguous, deﬁned, or to return a list of all deﬁnitions.
```

static void std::diagnostic(string aString) static void std::error(string aString) static void std::warning(string aString)

generate diagnostic, error and warning messages.

**static string std::get_cpp(string aString)**

resolves a deﬁnition within the preprocessor/command line name-space.

**static string std::get_env(string aString)**

resolves a deﬁnition within the external environment.

static string std::date() static string std::file() static string std::time()

```
replace the ANSI C __DATE__, __FILE__ and __TIME__ macros.
```

static token std::parse(string aString) static token std::parse_tokens(token someTokens[]) static token std::tokenize(string aString) []

support character-level substitution and re-entrant analysis.

## 3.3 Incompatibilities

In principle the FOG grammar is a superset of the C++ grammar. In practice there are some very minor incompatibilities.

### 3.3.1 Semantic Errors

Most of the signiﬁcant FOG enhancements occur by deﬁning a meaning for constructs that are semantic (or syntactic) errors in C++. Therefore a C++ program with semantic errors may be error-free in FOG.

This is an inevitable consequence of enhanced semantics.

### 3.3.2 Transparency

```
FOG generates multiple output ﬁles with a default disposition to an interface and
an implementation ﬁle per top level class. If FOG is used to preprocess C++ code
requiring a more sophisticated ﬁle allocation, the ﬁle structuring may be lost. ﬁle-
placement-declarations may be needed to create the required structure, see
Appendix F.4.3.
This is an inconvenience that occurs when porting C++ code to FOG. C++ code
used via #include or using/utility is not regenerated and so retains its ﬁle
structure.
```

### 3.3.3 auto

```
FOG meta-constructs are deﬁned by overloading the auto keyword. This causes
no incompatibility outside functions, where the keyword is semantically invalid.
The keyword is required to disambiguate syntax within a function.
void f()
{
int i = 5;
// Assignment of 5 to (integer) i
auto int j = 5;
// As above with redundant auto
auto e = k = 5;
// Assignment of k = 5 to (expression) e
}
```

### 3.3.4 Incompatible concatenation

Extension of the ANSI C string concatenation policy to characters, identiﬁers, numbers and strings potentially introduces a major incompatibility for FOG. However the deﬁnition of the major syntax elements of C++ (whitespace, punctuation, characters, numbers, strings, identiﬁers and keywords) and accidental properties of the C++ grammar limit the problems.

An incompatibility arises wherever FOG concatenates but C++ does not. This may occur when characters, identiﬁers (including keywords), numbers and strings occur without intervening whitespace or punctuation. There are 16 combinations of adjacent characters, identiﬁers, numbers and strings to consider:

```
Character-anything only occurs in C++ when a character-literal arises as a literal in
a primary-expression and the subsequent operator is not punctuation. In ARM C++
[Ellis90] there were no non-punctuation operators, however the standard [C++98]
introduced the alternative tokens (and, and_eq, bitand, bitor, compl, not,
not_eq, or, or_eq, xor, xor_eq).
Anything-character occurs in C++ when a character-literal arises as a literal in a
primary-expression and is preceded by non-punctuation. This occurs for the
alternative tokens and following a return, sizeof or throw keyword.
```

The same possibilities occur for number-anything, anything-number, stringanything and anything-string.

String-string concatenates in FOG, and corresponds to ANSI C behaviour.

```
"This " "is"" ""a" " single"" string" // "This is a single string"
```

Identiﬁer-identiﬁer cannot occur since whitespace or punctuation is required to terminate the ﬁrst identiﬁer.

```
For identiﬁer-string and string-identiﬁer there is a further problem that arises in
the linkage-speciﬁcation syntax.
extern"C"size_t f;
```

must be written in FOG as

```
extern "C" size_t f;
```

This is unlikely to cause many problems since few programmers would choose to be so economical with whitespace in these cases.

```
The FOG concatenation extension therefore introduces minor incompatibilities.
Whitespace cannot be omitted around the string-literal of a linkage-speciﬁcation,
following return, sizeof or throw or around alternative tokens, if the omission
conﬂicts with a concatenation interpretation.
```

## 3.4 Cpp Replacement

Stroustrup, in The Design and Evolution of C++ [Stroustrup97], identiﬁes elimination of the preprocessor as a major goal for C++, devoting the ﬁnal chapter to a discussion of its weaknesses, and identifying some remedies that C++ provides. In the ﬁnal paragraph, Stroustrup writes

“I’d like to see Cpp abolished. However the only realistic and responsible way of doing that is ﬁrst to make it redundant ...”.

In order to justify the claim that FOG makes Cpp redundant, we must brieﬂy review the deﬁciencies and facilities of Cpp to determine to what extent FOG resolves and replaces them.

### 3.4.1 Cpp limitations

Cpp supports the deﬁnition of and replacement of object-like and function-like macros.

An object-like macro associates an identiﬁer with a replacement sequence of preprocessor tokens.

```
#define OCTAL_CASES \
'0': case '1': case '2': case '3': case \
'4': case '5': case '6': case '7'
```

The replacement tokens replace the macro identiﬁer wherever it occurs.

```
switch (c)
{
case OCTAL_CASES: /* ... */ break;
case ALPHABETIC_CASES: /* ... */ break;
case '%': /* ... */ break;
}
Substitution occurs at a very low level, offering the programmer considerable
ﬂexibility. In the above example, the replacement sequence omits an initial case
keyword and a trailing colon token. The missing tokens accompany the
instantiation. Readers may form their own opinion as to whether the unusual
deﬁnition leads to a dangerously obscure or aesthetically pleasing implementation
of the switch statement.
```

A function-like macro associates an identiﬁer and a list of formal parameters with a replacement sequence.

```
#define MAX(a,b) ((a) > (b) ? (a) : (b))
```

Invocation of the macro provides the actual arguments that replace the formal parameters in the replacement sequence.

Macro substitution is simple but prone to accidents. Substitution occurs at a lexical level and so ignores any logical structure that may be present in the source code.

```
The intention that MAX returns an expression from a pair of expression arguments
is only realised when the actual usage is appropriate.
```

The apparently redundant parentheses in the macro deﬁnition avoid the surprising evaluation that would otherwise result from the interpretation of

```
price + MAX(current_rate, fixed_rate) * commission
```

as

```
(price + current_rate) > fixed_rate)
? current_rate : (fixed_rate * commission)
```

#### 3.4.1.1 Unwanted substitution

Perhaps the most serious problem with the preprocessor is that of name capture. All names occur in a single namespace and so every conventional use of a name that is deﬁned as a macro can malfunction. For instance the enumeration

```
struct Options
{
enum { LEFT, RIGHT, UP, DOWN, MAX };
};
should operate quite satisfactorily with Options::MAX denoting the number of
options. This deﬁnition will typically be placed in some include ﬁle. However if
another include ﬁle contains the earlier deﬁnition of MAX, any reference to
Options::MAX will fail whenever both include ﬁles are used. If the macro is
deﬁned before the enumeration, a syntax error will spring up in the enumeration.
```

This form of error is obscure and confusing. It can appear to be intermittent since compilations that do not use both include ﬁles succeed. Novice programmers are bafﬂed. Experienced programmers may take a little time to detect the handiwork of the preprocessor.

```
This problem is resolved in FOG by changing to a policy of invited substitution.
The replacement functionality for MAX substitutes only as part of a $ or @
expression.
```

#### 3.4.1.2 Language independence

The independence of macros from the underlying language is resolved in FOG by use of meta-types to constrain the syntax of meta-variable values and of metafunction argument and return values. Parentheses are not required.

When a meta-function is invoked, the argument is represented by a parsed AST and eventually validated against the required syntax. The tree is substituted for each reference within the meta-function, and since the tree has already been parsed there is no possibility of re-interpretation in conjunction with surrounding tokens.

#### 3.4.1.3 Side-effects

```
When a macro such as MAX is invoked with an argument that causes a side effect
c = MAX(a++, ++b)
```

one of the arguments is evaluated twice, and so receives a double increment. If the ﬁrst argument is greater, the result is obtained after one increment has occurred. It is therefore unlikely that the program will function as required.

```
FOG does not resolve this problem, since FOG passes the syntax tree for a++ into
the meta-function and instantiates the tree for each reference. FOG does not
‘evaluate’ the argument once and then pass it. This is not possible because the
argument is a syntax tree, not a value. It may be that the syntax happens to
correspond to a value as in the MAX case, but not in general.
MAX is a poor example for a syntactic meta-function. MAX is a computational
operation and so can and should be implemented by some form of inlined function.
```

#### 3.4.1.4 Substitution level

```
Many implementations of the original Kernighan and Ritchie C preprocessor
[Kernighan78]
performed
character-based
substitution.
The
replacement
characters were inserted between the surrounding characters and the resulting
character stream was then re-analysed. This offered considerable ﬂexibility, but
different implementations varied in their treatment of obscure recursions and
encountered difﬁculties when a composite token such as += arose as a result of
character concatenation.
The ANSI C preprocessor changed to token-based substitution. The preprocessor
identiﬁes the tokens and substitution replaces a sequence of tokens. Composite
tokens can only arise through explicit use of the ## operator.
Although the FOG substitution is syntax-based, character-based substitution is
also supported through use of the std::parse built-in meta-function and the
token meta-type6. token is the most primitive and generic terminal of the FOG
grammar. Every number, string, identiﬁer or piece of punctuation such as >>= is a
token.
auto string OCTAL_CASES[] =
"'0': case '1': case '2': case '3': case "
"'4': case '5': case '6': case '7'";
The required tokens are represented as a concatenation of two strings to ﬁt the
available line length. This is then incorporated into its overall context by further
string concatenation before $std::parse is invoked to convert to a syntax tree.
$std::parse(
"switch (c)"
"{"
"
case " $OCTAL_CASES ": /* ... */ break;"
"
case " $ALPHABETIC_CASES ": /* ... */ break;"
"
case '%': /* ... */ break;"
"}"
);
```

A cleaner solution is available by meta-programming:

```
auto statement multi_case(signed lo, signed hi)
{
auto for (signed i = $lo; i <= hi; ++i)
case $i: ;
}
...
switch (i)
{
$multi_case('0', '7') /*...*/ break;
$multi_case('a', 'z') /*...*/ break;
case '%': /* ... */ break;
}
```

6. Token-based substitution could easily be supported, but it is difﬁcult to conceive of an example that is not better resolved by syntax-based substitution, or more clearly resolved by character-based substitution.

```
This exploits the non-structured deﬁnition of a case statement to treat
case 'a':;
as a statement that drops through to the next case. The extra
semicolon avoids the need for a case meta-type.
case '0': ;
... ... ...
case '7': ;
/* ... */ break;
```

#### 3.4.1.5 Backslash continuations

The C preprocessor requires all directive lines to comprise exactly one complete line. This restriction is alleviated by the ability to continue a line with a trailing backslash, but results in less readable and difﬁcult to maintain code.

FOG integrates replacement functionality into the main grammar and so the replacement facilities are free format.

### 3.4.2 Concatenation and Stringizing

The character-based substitution of the K&R C preprocessor enabled composite tokens (normally an extended identiﬁer) to be formed by causing the character sequences to abut.

```
begin/**/_and_/**/end
// begin_and_end
```

An identiﬁer could be converted to a string by substitution within a macro:

```
#define STRINGIZE(s) "s"
STRINGIZE(text)
// "text"
In the ANSI C preprocessor, the change to token-based substitution, and the
requirement that a comment be replaced by a whitespace character, lost this
ﬂexibility necessitating the introduction of the
## operator to request
concatenation, and the # operator to support stringizing.
#define CONC3(a,b,c) a ## b ## c
CONC3(begin,_and_,end)
// begin_and_end
#define STRINGIZE(s) #s
STRINGIZE(text)
// "text"
```

In FOG character-based concatenation occurs between characters, numbers, strings, identiﬁers (including keywords) without intervening whitespace. An empty string or character can be used to provide separation between elements that require concatenation.

```
begin""_and_''end
// begin_and_end
```

An empty string (or character) can be used as a meta-cast for stringizing. The subsequent text acquires the string (or character) characteristics of the start of the sequence.

```
""text
// "text"
```

Alternatively, meta-functions can be deﬁned with similar behaviour to the ANSI C approach

```
auto identifier CONC3(identifier a, identifier b, identifier c)
{ ${a}${b}$c }
auto string STRINGIZE(identifier s) { $s; }
$CONC3(begin,_and_,end)
// begin_and_end
$STRINGIZE(text)
// "text"
$ invocations in the body of CONC3 access the formal parameters. The ﬁrst two
invocations use the ${} form to ensure evaluation as ${a}${b}${c}, since the
default of taking the longest possible interpretation of $a$b$c would evaluate
${a${b${c}}}.
The stringize meta-function apparently does nothing, however the distinct
parameter and return meta-types arrange for an
identifier to
string
conversion, which also does nothing. The stringizing occurs at the point of usage;
when a string representation is required, the internal representation is formatted
as a valid string.
```

### 3.4.3 #define directive

Preprocessor object-like deﬁnitions

```
#define PI 3.14159
```

and function-like deﬁnitions

```
#define MAX(a, b) ((a) > (b) ? (a) : (b))
```

are replaced by meta-variable

```
auto double PI = 3.14159;
```

and meta-function

```
auto expression MAX(expression a, expression b)
{ $a > $b ? $a : $b }
```

deﬁnitions in FOG. The FOG deﬁnitions have syntactical types that can be checked and avoid the need for protective parentheses.

### 3.4.4 #include directive

```
The historical semantics of the #include directive permit an arbitrary sequence
of tokens to be incorporated more than once in almost any context. This ﬂexibility
is excessive and almost never needed. Most programmers have learnt to tame the
directive by placing an include ﬁle guard within each include ﬁle to inhibit multiple
inclusion:
#ifndef FRED_H_INCLUDED
#define FRED_H_INCLUDED
//...
#endif
```

although many have not learnt how much compile time can be saved by inhibiting the include as in

```
#ifndef FRED_H_INCLUDED
#include "fred.h"
#endif
FOG supports a more disciplined form of inclusion like the #import of Objective
C [Cox86]:
using "fred.h";
```

performs the inclusion provided the ﬁle has not already been included. Guards are unnecessary.

### 3.4.5 #if, #ifdef, #ifndef, #else, #elif, #endif directives

Preprocessor conditional compilation

```
#if defined(UNIX)
static const char *temp_path = "/tmp/";
#else
static const char *temp_path = "C:\\Temp\\";
#endif
```

is replaced by the use of meta-statements

```
auto bool unix = $std::get_cpp(UNIX);
auto if (unix)
static const char *temp_path = "/tmp/";
else
static const char *temp_path = "C:\\Temp\\";
with built-in meta-functions of the std meta-namespace providing support when
necessary. (The auto preceding the if is unnecessary if the example occurs
outside a function.)
```

### 3.4.6 #line directive

```
The #line directive is not used in source programs generated by human beings.
It provides a very simple but useful mechanism for automatic source code
generators to ensure that compilers and debuggers refer to the original source
lines rather than some scrambled intermediate.
#line performs this role
adequately and needs no replacement. Once Cpp has been discontinued, an
extension that uses a more cryptic free format spelling and releases the # token
could be considered. This is examined in Appendix F.1.4.
```

### 3.4.7 #error directive

FOG provides the built-in meta-functions

```
auto void std::diagnostic(string someText)
auto void std::error(string someText)
auto void std::warning(string someText)
```

to support emission of meta-compile-time messages.

### 3.4.8 #pragma directive

FOG provides no explicit counterpart for pragmas. However the effects of pragmas may be achieved by vendor-deﬁned built-in meta-functions.

A compilation system that supports intrinsic functions might recognise

```
#pragma intrinsic(memset)
to request use of the intrinsic rather than function call implementation of memset.
```

In a compatible meta-compilation system, a built-in meta-function to implement the pragma could be hypothetically declared:

```
auto void std::intrinsic(identifier anIdentifier);
```

and invoked as

```
$std::intrinsic(memset);
```

Invocation on a meta-compiler without support for the pragma could be dummied out by deﬁning the meta-function to have no functionality:

```
auto void std::intrinsic(identifier anIdentifier) {}
```

or to redirect to a different pragma:

```
auto void std::intrinsic(identifier anIdentifier)
{ $std::use_intrinsics(""$anIdentifier); }
```

## 3.5 Summary

We have shown how C++ string concatenation can be usefully generalised by simple lexical changes to phase 6 of the C++ translation. This eliminates the need for Cpp concatenation.

We have shown how an additional form of literal representing a pre-parsed AST may be incorporated into the C++ grammar as an identiﬁer to support the re-use of syntax trees. This eliminates the need for Cpp macros.

```
We have shown how the auto keyword can be re-used to make C++ control and
declaration constructs available for meta-programming. This eliminates the need
for Cpp conditionalisation.
```

We have introduced minor syntactic and semantic generalisations to provide greater consistency when multiple declarations are composed together in response to the relaxation of the One Deﬁnition Rule. This supports weaving of multiple code contributions together as required for Aspect Oriented Programming.

We have shown how these changes introduce minimal incompatibilities. The changes make Cpp redundant by providing replacement facilities that integrate with the main C++ language. In the next chapter we show how these changes facilitate meta-programming.
