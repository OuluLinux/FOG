# 4 FOG Semantics

Chapter 3 presented the FOG grammar and discussed many of the minor semantic issues, but deferred the more major ones for resolution in this chapter.

We ﬁrst describe the compilation stages that transform a FOG source program into C++ ﬁles, and distinguish between the potential declarations appearing in the FOG source and the actual declarations that are ﬁnally emitted. This leads on to a discussion of the meta-types and meta-objects used to represent these declarations, and the type-constructors used to support iterators and trees.

We then describe alternative models for substitution, justify the choice of syntaxlevel substitution in FOG, and examine the constraints imposed by supporting context free parsing of arbitrary syntax.

The distinction between the conventional name-space used for C++ declarations and the meta-name-space of meta-declarations is examined. Consideration of the context in which substitution occurs shows the need for more than one substitution operator, and a requirement to use lexically nested scopes rather than structurally nested scopes.

The ﬂexibility that FOG provides for redeclarations is a critical distinction between FOG and C++ that is resolved by composing the multiple contributions to a single composite C++ declaration. The composition policies for different categories of declaration are described.

FOG has a simple meta-class model. Other languages have different models. We review the alternate models and show that the FOG meta-class model is a natural and consistent extension applying the run-time programming perspective to (meta-)compile-time.

User-deﬁned code may contribute to decisions made at (meta-)compile-time through static meta-programming either through direct invocation from source code or through provision of meta-programs. Meta-programming by direct invocation is discussed in Sections 4.2, 4.3 and 4.4. Independent meta-programs are discussed in Section 4.6.

Finally the behaviour of syntax macros is described.

## 4.1 Meta-compilation stages

The signiﬁcant processing stages of meta-compilation are shown in the left-hand column of Figure 4.1.

The right-hand column identiﬁes the distinct representations used, with solid arrows indicating the ﬂow of data between the processing stages. As will shortly be explained, the anomalous arrow direct to an actual declaration from a potential declaration is the result of automatic creation of an actual declaration from a potential declaration in a determined context.

Chapter 5 describes the successive stages of lexical, syntactic and semantic analysis that steadily increase the precision of the internal representation of the following trivial program

```
int x;
```

as

```
•
six source characters for int x;
•
three tokens for identiﬁer int, identiﬁer x and punctuation ;
•
a syntax tree for the generalised expression int x
•
a potential declaration for the variable-speciﬁer int x
•
an actual declaration for the global variable int ::x
```

Actual declarations are emitted to the C++ output ﬁles, after construction of the usage dependency graph by the compilation stage as discussed in Chapter 6.

The distinction between potential declarations (in an unresolved context) and actual declarations (in a resolved context) is not necessary for C++, since every declaration has a well-deﬁned context.

FOG supports passing of declarations to meta-functions, and the regeneration of declarations in derived contexts by derivation rules. Potential declarations therefore describe a declaration after semantic analysis. Transformation to an actual declaration occurs as soon as the declaration is associated with an actual context.

Static meta-programming may occur during

- semantic analysis

- meta-construction

- meta-main

- meta-destruction

- compilation

Meta-programming during semantic analysis occurs through interpretation of meta-statements:

```
auto if (generateDebugCode)
/* debug code */
```

or more generally to service meta-function calls

```
$CachedString::flyweight("pointer");
```

The meta-construction, meta-main and meta-destruction stages are described in Section 4.6. They consist exclusively of application functionality.

The ﬁnal opportunity for meta-programming occurs during the compilation stage. At this point meta-functions and meta-variables used within the derivation predicates and bodies of declarations are evaluated, and meta-programming must be restricted to the acquisition of information from the pool of declarations. New

lexical analysis

semantic analysis

syntax analysis

**Figure 4.1 FOG Meta-compilation Stages**

FOG source

meta-main

meta-construction

meta-destruction

emission

compilation

potential declarations

tokens

syntax trees

actual declarations

usage nodes

C++ ﬁles

declarations cannot be added, although previously declared declarations can be extended by composition and meta-variables updated.

### 4.1.1 Potential and Actual Declarations

In C++, each (non-template) class, function, type or variable declaration provided as source text is analysed in, and contributes once to, its surrounding context.

Templates provide a macro facility that enables certain parametric references to be resolved automatically, but each template reference contributes at most once to its surrounding context.

In FOG, derivation rules and more generally meta-programming may cause one source declaration to make multiple contributions to a variety of contexts. Since there is no longer a one to one relationship between source text declarations and implemented declarations, we need to introduce new terminology.

A declaration appearing as source text is a potential declaration. A potential declaration has the potential to be resolved to an arbitrary number of actual declarations.

A declaration resolved to its eventual context is an actual declaration. Actual declarations correspond to the declarations emitted for subsequent compilation by the C++ compiler.

```
The two forms of declaration have signiﬁcant differences. Consider the possible
speciﬁers for an enum, which may occur in source text and consequently as a
potential declaration for:
```

- a deﬁnition

```
enum Enum { ENUMERATOR }
```

corresponding to

```
enum-speciﬁer:
enum identiﬁeropt { enumerator-listopt }
```

- or a to be resolved reference

```
enum Enum
```

corresponding to

```
elaborated-type-speciﬁer:
// Part of
enum ::opt nested-name-speciﬁeropt identiﬁer
Neither of these can be resolved without knowledge of the prevailing context,
whereas an actual declaration has already been resolved and known to be
perhaps enum ::MyNameSpace::EnumScope::Enum.
```

There is no point in instantiating an actual declaration at the source level, since this can only create a duplicate of the resolved declaration. Conversely, it is meaningless to probe the unresolved context of a potential declaration.

There are therefore necessarily three distinct concepts to be represented by the meta-type system:

- a potential deﬁnition

- a (potential) unresolved reference

- an (actual) reference resolved to an actual deﬁnition

```
The distinctions between potential and actual declarations can be seen in the
following example, which makes use of the built-in meta-types, class_specifier,
declaration, and identifier. Meta-objects with these types store constructs
that comply with the class_speciﬁer, declaration, and identiﬁer syntax. The more
detailed semantics of meta-types are explained in the next section.
class Actual {};
auto declaration nest(identifier anId, class_specifier aClass)
{
class $anId
{
$aClass;
};
}
auto declaration cached = $nest(Nested, class Potential {});
class ReNested
{
$cached;
};
The ﬁrst potential declaration, for class Actual, can be resolved immediately
and so results in an actual declaration of class ::Actual.
The meta-function deﬁnition similarly results in the actual declaration of the meta-
function ::nest. Analysis of the meta-function body creates a potential
declaration for the class-speciﬁer (class $anId) using the ﬁrst formal parameter
(anId), for which preliminary semantic analysis can verify compatibility of the
identifier meta-type. Syntactic analysis of the tree-literal ($aClass) in the class-
speciﬁer encounters the usage of the second formal parameter. For reasons
explained in Section 4.2.3, this is presumed to be an identiﬁer which is a
degenerate declaration. Again a preliminary semantic analysis can verify the meta-
type compatibility of a class_specifier.
Analysis of the invocation of the nest meta-function creates potential declarations
for the two arguments: Nested and class Potential {}. The ﬁrst is an identiﬁer,
for which there is no distinction between potential and actual declarations. The
second is a class-speciﬁer whose context cannot be resolved, so the potential
declaration cannot be converted to an actual. Further analysis veriﬁes that the
arguments can be converted to satisfy the identifier and class_specifier
syntaxes of the formal parameters.
```

Interpretation of the meta-function replaces the formal parameters and returns the meta-function-body for execution in the calling context as if the source were:

```
auto declaration cached = class Nested
{
class Potential {};
};
Further potential declarations for class Nested and class Nested::Potential
are created as the initializer for the potential and actual declarations of ::cached.
A potential and actual declaration for class ::ReNested are then created and the
cached variable replaced as if the source were:
class ReNested
{
class Nested
{
class Potential {};
};
};
so that potential and actual declarations are also created for
class
ReNested::Nested and class ReNested::Nested::Potential.
```

### 4.1.2 Meta-types

Execution of a meta-program involves manipulation of declarations that have some syntactical (meta-)type.

```
A very simple form of meta-execution could be supported by deﬁning just a single
meta-type such as syntax_element, which could exhibit polymorphic behaviour
for all possible syntax elements. However source syntax checking is normally a
rigorous activity and type-less meta-execution is not philosophically consistent
with C++, which is (in principle) a strongly typed language.
FOG should therefore support strongly typed meta-execution. The obvious set of
types correspond to the grammar productions:
class_name,
expression,
statement, template_parameter, etc. The set of types provided by FOG will be
described shortly.
```

**Casting**

C++ supports type-widening but requires some form of cast to narrow a type.

```
class A {};
class B : public A {};
void f()
{
B b;
A *pa = &b;
B *pb = &b;
pa = pb;
// Widening ok.
pb = static_cast<B *>(pa);
// Narrowing needs a cast.
}
```

This is necessary since it is not generally possible to determine at compile-time that a type-narrowing is safe. Static meta-programs execute at compile-time and so it is always possible to determine safety at compile-time. Precise replication of run-time behaviour at compile-time is therefore unnecessary and undesirable: casts are not available for meta-programming. Assignments are checked dynamically and if possible the assigned value is converted to a value compatible with the target meta-type.

```
auto identifier i = label;
// Ok label is an identiﬁer
auto statement s = $i;
// Ok identiﬁer to expression-statement
auto expression e = $s;
// Ok expression-statement to expression
No error occurs on the assignment of a statement to an expression, although not
all statements are expressions, because the conversion is performed on the value
which is an expression-statement for which there is a safe conversion. The permissible
conversions are described in Section 4.1.6.
```

**Meta-type as syntax predicate**

It is appropriate to regard the meta-type as a syntax predicate, since the polymorphism available at compile-time can be exploited to convert the result of an assignment to the speciﬁed meta-type.

```
In order to keep the FOG grammar context-free, it is necessary to be able to parse
the argument of a meta-function, or the initializer of a meta-variable without
knowledge of the required syntax (or meta-type). A single syntax must cover all
possibilities. This syntax is the tree-statement syntax described in Section 3.1.1.6. It
satisﬁes the weakest predicate of the token meta-type, and also the stronger
predicates corresponding to the meta-types used to create the AST.
```

Since each meta-type corresponds to a syntax predicate, a meta-type could be deﬁned for each non-terminal in the FOG grammar.

There are three problems:

Excluding the lexical productions, there are about 150 non-terminals in the C++ grammar (more in FOG).

```
The non-terminals are not orthogonal. For instance after parsing the source text
containing the isolated letter a, what is its syntactic type? It could be an identiﬁer,
template-argument, expression, abstract-declarator etc. Choosing the more primitive
meta-type would be possible if the grammar was completely context-free and so
the productions corresponded to nodes of a tree, unfortunately
a < b > ( c )
could be a postﬁx-expression or a declarator, for which postﬁx-expression is a shared
common syntax type in the superset grammar implementation of FOG. However
there is no shared syntax type in the C++ (or FOG) grammar.
```

Distinct meta-types are needed to describe potential and actual declarations.

```
We therefore need extra meta-types to capture the potential/actual distinction, but
want fewer of them to reduce the deﬁnition and documentation effort. There seems
little point introducing meta-types to distinguish between a logical-and-expression and
a logical-or-expression.
```

A more pragmatic approach is therefore taken, deﬁning meta-types only for those concepts for which a meta-program may reasonably need to make a distinction.

**Implemented meta-types**

```
Each meta-type corresponds to a non-terminal, whose production rules deﬁne the
valid syntax for syntax trees with the meta-type. Many non-terminals are
specialisation of others, thus, identiﬁer is a (specialisation of) id-expression which is
a primary-expression which is an expression which is a token.
```

This specialisation hierarchy resembles an Object Oriented inheritance hierarchy. The UML [Booch99] inheritance diagram in Figure 4.3 shows the externally visible general structure and some of the more signiﬁcant meta-types.

Figure 4.2 shows some of the intrinsic meta-types. Intrinsic meta-types deﬁne a

syntax that can be directly identiﬁed and require no distinction between potential and actual declarations. A fuller listing of the intrinsic meta-types is provided in Table 4.1, with specialisation indicated by the partitioning into more speciﬁc metatypes across the “intrinsic meta-types” columns.

```
Figure 4.3 shows some potential and actual meta-types. Meta-objects of a
potential meta-type more specialised than
specifier are created by the
(syntactic and) semantic analysis to describe potential declarations. These Meta-
objects are converted to have an actual meta-type more specialised than entity
once the context supports the conversion from a potential to an actual declaration.
A fuller listing of the potential and actual meta-types is shown in Table 4.2 again
using partitioning to denote specialisation.
```

**Declarations and Statements**

```
The syntax generalisations in FOG remove the principal distinctions between
declarations and statements, however both concepts are widely used and so both
declaration and statement meta-types should be provided. Implementation of
```

token

declaration expression

**Figure 4.2 Intrinsic Meta-type inheritance**

punctuation

identiﬁer

number is more specialised than name

```
these types within a simple inheritance hierarchy requires one to be more
specialised than the other. But within a function, a declaration-statement is a
specialised statement, whereas in a class a meta-expression-statement is a specialised
declaration.
The dilemma is resolved from the external perspective by treating the two names
as synonyms for the same meta-type. A subtle, and almost irrelevant, distinction
is
made
that
a
declaration
resolves
an
ambiguous
labeled-
statement/anonymous-bit-ﬁeld to the bit-ﬁeld, whereas a statement resolves to
the label.
```

### 4.1.3 Meta-objects

```
Potential and actual declarations and their constituent elements are represented
by meta-objects within the meta-compiler, and many of these meta-objects are
available for manipulation by meta-programs. Meta-objects are created by the
presence of the corresponding declaration, and are managed automatically as
part of a hierarchical pool of meta-objects maintained by the meta-compiler. There
is no need or facility for explicit creation or deletion of meta-objects; there is no
meta-operator new or meta-operator delete. Garbage collection of objects
that are not required during the code emission phase occurs automatically.
```

Each meta-object has a corresponding meta-type, with distinct meta-types for potential and actual declarations. When

```
enum Enum { E1, E@Second };
is parsed to create a potential declaration, a meta-object of enum_specifier
meta-type is created with two child meta-objects: an identiﬁer of identifier meta-
type and a list of enumerator-deﬁnitions. The list in turn comprises two child objects
of enumerator_definition meta-type, each of which has a child meta-object to
deﬁne the name. The unresolved name of the second enumerator is represented
by an expression tree to capture the deferred resolution. The root of the tree has
the name meta-type. The meta-types (and some values) for the meta-objects
deﬁning the potential declaration are shown in Figure 4.4.
When the potential declaration is converted to an actual declaration, the
unresolved name is evaluated in its actual context to yield a meta-object of
identifier meta-type, and the enum_specifier and enumerator_definition
meta-objects are converted to the meta-objects of enum and enumerator meta-
type shown in Figure 4.5.
```

Access to these automatically managed meta-objects occurs by establishing a reference. At the time of creation:

token

declaration

speciﬁer entity

scope object scope_speciﬁer object_speciﬁer

variable_speciﬁer

enum_speciﬁer enum

variable

**Figure 4.3 Potential and Actual Meta-type inheritance**

is more specialised than

type type_speciﬁer

```
auto enum_specifier potentialDef = enum Enum { E1, E@Second };
$potentialDef;
```

by subsequent direct reference

```
auto enum_specifier potentialRef = enum Enum;
auto enum actualRef = enum Enum;
// See Section 4.1.6.2
```

or by indirect reference

```
auto enumerator enum2a = $Enum::E2;
auto enumerator enum2b = $Enum.enumerators()[1];
```

### 4.1.4 Working meta-variables

Temporary meta-objects may be used for numeric calculations and iterators.

```
class X
{
auto int i = 0;
auto for (iterator p = $Enum.enumerators(); p; ++p)
auto /* ... */ ++i /* ... */
};
The declaration of i has no syntactical indication that i is a temporary variable,
so it isn’t. The meta-variable X::i is therefore not temporary.
```

FOG establishes local scopes for meta-programming in the same way that C++ establishes local scopes for normal programming. The iterator is therefore a

```
enum_specifier
name
enumerator_definition
enumerator_definition[]
enumerator_definition
tree_literal
identifier : Second
identifier : E1
identifier : ENUM
identifier : Enum
```

**Figure 4.4 Example Potential Declaration Tree**

```
enum
enumerator
enumerator[]
enumerator
identifier : E1
identifier : Enum
```

**Figure 4.5 Example Actual Declaration Tree**

```
identifier : E2
temporary meta-variable since it is declared within a for loop. However, so are any
other meta-variables declared within the loop. This problem may be circumvented
by preﬁxing the declaration with This to start name resolution beyond the local
scopes.
auto for (iterator p = $Enum.enumerators(); p; ++p)
auto if (p->value() > 255)
auto number This.has_big_enum = true;
```

### 4.1.5 Scalars, Arrays, Lists and Trees

The elemental meta-types and their use for compile-time calculation and syntax validation has been described. Use of scalar types alone is inadequate for metaprogramming, since many declarations involve lists of child declarations, and in some cases trees of descendant declarations. Traversal of these structures must be supported.

Intrinsic Meta-Type

Grammar

example non-terminal

```
punctuation
=
‘punctuation’
modifier
array_modifier
.. []
ptr-operator
function_modifier
.. (int x) throw y
pointer_modifier
*T::Q const ..
reference_modifier
& ..
name
keyword
reserved
case
‘reserved-word’
meta_type
function
meta-type
class_key
class
class-key
decl_specifier
static
(non-type) decl-specifier
cv_qualifier
volatile
cv-qualifier
identifier
i
identifier
X::i
declarator-id
expression
assignment_
expression
a = b ^ c
assignment-expression
constant_
expression
5
constant-expression
character
'a'
character-literal
number
4.0
number-literal
string
"string"
string-literal
a,b
throw (a)
int
x(5)
expression
... ... ...
throw-expression
template-argument
mem-initializer
statement
declaration
label: x;
if (a) ;
while (1) ;
goto label;
try {} catch ... {}
labeled-statement
selection-statement
iteration-statement
jump-statement
try-block
a = b;
int a;
template T<Q> a;
asm "";
using namespace X;
{}
{}
{}
expression-statement
declaration-statement
explicit-instantiation
explicit-specialization
asm-definition
using-directive
compound-statement
compound-declaration
function-body
handler
catch (a) ;
handler
exception_specification
throw (a)
exception-specification
```

**Table 4.1 Intrinsic Meta-types**

C++ supports constructed types through pointer-to, array-of, function-returning and record-of type constructors, and perhaps the same type constructors should be available for meta-types.

```
The C++ pointer system inherited from C is necessary to support uncontrolled
memory access and arbitrary memory allocation through the type-less malloc().
This lack of discipline causes many problems through the use of null, dangling or
stale pointers and allows memory to leak. C++ introduced operator new and
references and thereby alleviated some of the problems. Java supports only
references and consequently has no comparable memory access problems. It is
not clear that there is any need for genuine dynamic memory allocation at meta-
compile time, since FOG allocates and manages meta-objects automatically. A
more pragmatic set of type constructors is therefore implemented, exploiting the
freedom to deﬁne all meta-types and lists of meta-types as specialisations of the
token meta-type.
```

Potential Meta-Type

Grammar

Actual Meta-Type example non-terminal

```
specifier
namespace_alias_definition
namespace X = Y
namespace-alias-
definition
namespace_alias
entity
scope_
specifier
namespace_definition
namespace X {}
namespace-definition
namespace
scope
linkage_specification
extern "C" {}
linkage-specification
linkage
filespace_specifier
namespace/file
X {}
filespace-specifier
filespace
meta_class_specifier
auto class X {}
meta-class-specifier
meta_class
type_
specifier
class_specifier
class X {}
class-specifier
class
type
elaborated_
type_specifier
class X
elaborated-type-
specifier
typename X
typename
enum X
enum
enum_
specifier
enum X {}
enum-specifier
built_in_
type_specifier
long int
‘built-in-type-
specifier’
built_in
base_specifier
public X
base-specifier
base
enumerator_definition
E = 5
enumerator-definition
enumerator
object_
specifier
using_declaration
using X::y
using-declaration
using
object
function_specifier
void f() {}
function-definition
function
void f()
simple-declaration
init-declarator
declarator
abstract-declarator
member-declarator
variable_specifier
int v = 0
variable
int errno
typedef_specifier
typedef a b
typedef
parameter_specifier
int * = 0
parameter-declaration
parameter
meta_function_specifier
type f() {}
meta-function-
definition
meta_
function
meta_variable_specifier
name n = n
meta-variable-
declaration
meta_
variable
meta_parameter_specifier
identifier i
meta-parameter
meta_
parameter
exception_declaration
int * = 0
exception-declaration
exception
template_parameter_
specifier
class T = X
template-parameter
template_
parameter
```

**Table 4.2 Potential and Actual Meta-types**

**Meta-Array-Of**

```
C++ deﬁnes a compound-statement to be a statement, and an expression-list to be an
expression. FOG extends this by deﬁning a compound-declaration to be a declaration and
a
compound-tree-statement to be a tree-statement. There is clearly a signiﬁcant
polymorphism between an element and a list of those elements. It is therefore
appropriate to deﬁne a meta-type as polymorphic to a list of the same or more
specialised meta-type, so that we may use nested lists to handle arbitrary tree
structures.
The list has many similarities to a C++ array and so we can safely re-use the
nested {} syntax for list initialization and [] for indexing.
auto string s = { "a", { "b", { "c", { "d" }}}};
```

deﬁnes a tree with a ﬁrst, second, third and fourth generation descendant, as shown in Figure 4.6.

```
s
is the element
{ "a", { "b", { "c", { "d" }}}}
s[0]
is the element
"a"
s[1]
is the element
{ "b", { "c", { "d" }}}
s[1][1][1]
is the element
{ "d" }
```

There is no provision for uninitialized meta-variables and so no need to declare tree shapes independent of initializers. The initializers deﬁne the shape: explicitly as in the above example, implicitly when a declaration tree is assigned to a metavariable.

Meta-programs need to be able to iterate over lists of declarations. A polymorphic iterator meta-type is therefore provided that acts as a pointer to a list.

With this system an iterator could be used as:

```
auto for (iterator i = $bases(); i; ++i)
$i->do_something();
bases() returns a list of base class speciﬁers. The iterator is initialized by the
elements of the iteration domain, but why does iterator i = $bases() establish
an iteration over the domain comprising the elements of the list rather than the
domain comprising the list as a single element? While it is convenient to treat the
list as polymorphic to an element, there must be a predictable mechanism for
deciding when and by how much to ﬂatten a tree. The simplest algorithm, which
never ﬂattens implicitly, does not support the above example.
```

An operator is necessary to expose the contents of the list, so that continuing the earlier example

```
{ }
"b"
{ }
{ }
{ }
"a"
"c"
"d"
```

**Figure 4.6 Example Tree initializer**

```
s[]
is the two element list
"a", { "b", { "c", { "d" }}}
s[1][1][1][]
is the elemental list
"d"
```

allowing the iteration to be written:

```
auto for (iterator i = $bases()[]; i; ++i)// Non-FOG example
The extra [] changes the initialization to the elements of the list ensuring an
iteration over each base class. This is clumsy and prone to errors, since the [] is
too easily omitted.
```

We therefore extend the system to distinguish exposed and encapsulated lists.

An encapsulated list variable comprises a single element, which may be a list.

```
auto identifier encapsulatedList = { a, b, c, d };
An exposed list variable identiﬁed by a [] declarator sufﬁx comprises the arbitrary
number of elements of a list.
auto identifier exposedList[] = a, b, c, d;
The {} and [] encapsulate and expose symmetrically, so that
exposedList is-the-same-as encapsulatedList[]
encapsulatedList is-the-same-as { exposedList }
Deﬁning the initialization of an iterator and the return from built-in meta-functions
as exposed lists eliminates the need for the clumsy [] and allows the natural
programming style:
auto for (iterator i = $bases(); i; ++i)
$i->do_something();
```

or even

```
auto for (iterator i = $A::bases(), $B::bases(); i; ++i)
$i->do_something();
```

to achieve iteration over the concatenation of two exposed lists.

```
The hypothetical built-in declaration for bases is therefore
auto base class::bases() [];
and for iterator initialization:
auto iterator::iterator(token []);
auto void iterator::operator=(token []);
```

**Meta-Pointer-To**

This approach supports scalars, arrays and more generally trees, with all names behaving as references. The limited need for a pointer is handled by the polymorphic iterator.

**Meta-Function-Returning**

```
There is no direct support for pointers to functions, however functions and meta-
functions can be manipulated under the guise of the function or meta_function
meta-types, or more generally as declarations.
auto function_specifier add_constant(number fixedValue)
{
inline double ${unique_name()}(double functionArgument)
{
return functionArgument + $fixedValue;
}
}
auto function plusTwo = $add_constant(2);
a = ${plusTwo.name()}(b);
// a = b + 2;
add_constant is a meta-function that returns the function-speciﬁer (potential
declaration)
for
an
inline
function
which
adds
fixedValue
to
the
functionArgument. The name of the returned function is determined by a unique
name-generating meta function.
A potential declaration for a function that adds 2 is created by the invocation to
initialize the plusTwo meta-variable. Since the required meta-type is for an actual
function the inline function is created in the current (global) context.
```

The name of the function is then used in the ﬁnal line.

**Meta-Record-Of**

User deﬁned meta-types are not supported in FOG. This seems a very natural extension when more substantial compile-time programming is required. A little careful thought is necessary to distinguish (or unify) the conﬂicting perspectives of user-deﬁned meta-types as types constructed from existing meta-types, and as predicates upon extended syntax.

### 4.1.6 Meta-type conversions

When a meta-object is used as an intermediate term in a meta-expression, the meta-object may be suitable for direct use:

```
$metaObject.meta_function();
auto int i = $metaObject1 + $metaObject2;
```

Alternatively, a conversion to a more suitable form may be required, which may involve:

- conversion of a potential declaration to an actual one

- resolution of the value of a meta-object

- conversion to a synonym

```
•
conversion to a character, identifier, number or string meta-type
•
conversion of an expression to an expression_statement
•
conversion of an expression_statement to an expression
•
conversion of the break keyword to a statement
•
conversion of the continue keyword to a statement
•
conversion of the return keyword to a statement
```

#### 4.1.6.1 Meta-type Synonyms

```
The declaration and statement meta-types may be interchanged, as may the
expression and initializer_clause meta-types.
```

#### 4.1.6.2 Potential to Actual meta-type conversion

When a potential meta-type occurs in a context where an actual is required, an actual declaration is created from the potential in the prevailing context.

```
auto enum_specifier potentialEnum = enum E;// No conversion
auto enum actualEnum = enum E;
// Creates ::E
```

#### 4.1.6.3 Resolution of the value of a meta-object

```
A tree-literal deﬁnes an expression to be resolved in the meta-name-space resulting
in a meta-object whose value replaces the tree-literal. The replacement value must
be a meta-object that has meaning beyond the meta-level; it must therefore be a
character-literal, identiﬁer, number-literal or string-literal, since these represent all
possible alternatives for a C++ literal. This value is obtained by invoking the built-
in conversion meta-function, operator identifier(), unless the tree-literal forms
part of a concatenation in which case one of operator character(), operator
number() or operator string() may be invoked instead.
For the
character,
identifier,
number and
string meta-objects these
conversion meta-functions perform conversions as described in Section 3.1.1.3.
For named meta-objects, the unqualiﬁed name is ﬁrst expressed as an
identifier, and then further converted if necessary. This supports the idiomatic
const char *class_name = ""$Scope;
```

rather than the more explicit

```
const char *class_name = ""$name();
```

or even more explicit

```
const char *class_name = ""$Scope.name();
Meta-variables, meta-iterators and meta-functions have no run-time object and so
the appropriate conversion operator is applied to their value.
```

The value of a meta-function is the function body.

The value of a meta-iterator is a boolean meta-object, whose value is true while the iterator is valid. This supports:

```
auto for (iterator p = $functions(); p; ++p)
/* ... */
```

## 4.2 Substitution

Substitution is triggered when a reference such as

```
$name
is detected that satisﬁes the tree-literal syntax described in Section 3.1.1.6. The
reference clearly cannot be fully resolved until the deﬁnition is available, which
requires that the semantic processing of the deﬁnition has completed.
The C preprocessor has an independent deﬁnition syntax using #define that can
easily be fully analysed before processing continues. In FOG, the deﬁnition syntax
is integrated with the rest of the language, where alternative substitution
semantics impose distinct requirements on the overall processing.
```

### 4.2.1 Substitution levels

Substitution of a segment of source code by a replacement is often called macro processing. A variety of models and classiﬁcations of macro processing are discussed in Section 2.1.2. The most appropriate for discussing substitution in FOG is the distinction between character-, token-, and syntax-level substitution.

These alternate levels will be considered while attempting to implement the following example meta-program. in which a loop populates an array with the value of the meta-variable used as the loop counter.

```
auto for (unsigned i = 0; i < 10; ++i)
static const char *digits[] = { ""$i };
```

The example involves repeated interleaved deﬁnition and reference of a metavariable. The statement should be equivalent to the following C++ declaration, for which the loop provides the values which are automatically comma-separated as FOG emits the composed list.

```
static const char *digits[] =
{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
In each iteration, the identiﬁer i must be resolved to the appropriate meta-
variable, and evaluation must use the prevailing value of the meta-variable. (In
most examples, such as this one, i resolves to the same meta-variable on each
iteration, however this cannot be assumed.)
```

While maintenance of the loop counter is often restricted to the loop header, it is unsafe to assume that the loop body is clean:

```
auto for (unsigned i = 0; i < 10; ++i)
{
auto if (i == 5)
auto i += 2;
static const char *digits[] = { ""$i };
}
```

#### 4.2.1.1 Character-level substitution

Character-level substitution supports replacement of a text macro by an arbitrary run of characters, allowing perverse programs such as:

```
auto string s = "= 5;";
x +$s
// x += 5;
(Not valid in FOG)
```

The original K&R C preprocessor used this form of substitution.

```
Character-level substitution requires lexical analysis of the replacement to be (at
least partially) deferred until the adjoining characters are available, and so
requires the lexical analysis to be repeated in each substitution context. Therefore
preliminary syntactic analysis of the example iteration-statement cannot identify more
structure than:
auto for (unsigned i = 0; test-chars; step-chars)
```

body-chars

```
The preliminary analysis of the xxx-chars character sequences for each of the
three sub-clauses must invoke a special scanner that is sensitive only to syntax
involving the structuring tokens: ( ) { } ; if else do while. Resolution of
replacements cannot occur immediately, or even in lexical order, since the
replacement for use in step-chars is not necessarily known until body-chars has
been interpreted.
```

This imposes the restriction that replacement character streams involving structuring tokens may behave unpredictably, since the assumptions about embedded punctuation may be unjustiﬁed.

```
Once the three character sequences have been cached, interpretation may then
proceed by repeated invocation of lexical, syntactic and semantic analyses for
each of the cached test-chars, body-chars and step-chars. Eventually the
interrupted lexical analysis may resume, following on from the for statement.
```

Since character-level substitution during lexical analysis requires semantic interpretation, it is necessary to perform each of the lexical, syntactic and semantic analyses re-entrantly for each level of statement nesting.

An early version of FOG used this form of substitution, concurrently with Cpp substitution at the cost of considerable implementation difﬁculty and rather illdeﬁned behaviour.

FOG now supports explicit invocation of character-level substitution from the semantic analysis, rather than re-entrant semantic analysis within the lexical analysis. The perverse example can be realised as

```
auto string s = "= 5;";
$std::parse("x +"$s);
```

#### 4.2.1.2 Token-level substitution

Token-level substitution performs substitution after lexical analysis has completed the conversion of source text into a token stream. This improves efﬁciency and provides for a more predictable environment but loses ﬂexibility.

```
Operation at the token-level prevents the formation of composite punctuation
tokens such as +=. This is probably beneﬁcial. However, losing the ability to create
extended identiﬁers and the ability to convert character sequences to strings is
restrictive. The ANSI C preprocessor uses this substitution model and introduces
the # and ## operators to provide a slightly clumsy way around the restrictions.
```

Token-level substitution prohibits syntactic analysis of the replacement tokens in isolation, since adjacent tokens may inﬂuence the meaning; syntactic analysis must be repeated for each replacement and error diagnostics associated with instantiation rather than deﬁnition.

Token-level substitution simpliﬁes the example only slightly, requiring the preliminary parse to resolve:

```
auto for (unsigned i = 0; test-tokens; step-tokens)
```

body-tokens

The special scanner now caches token sequences rather than character sequences, but is still vulnerable to unexpected replacement behaviour that involves the structuring tokens.

Re-entrant invocation of syntactic and semantic processing is again necessary to interpret the cached token sequences during syntactic analysis of the loop.

```
A form of this token-level substitution was used by the multi-pass grammar
implementation of FOG, which performs syntax-driven re-entrant analysis. The
difﬁculty of deﬁning the token { (for use in a replacement) rather than the
punctuation { (for deﬁning program structure) was at one point assisted by
introducing the additional lexemes \{ and \}. This fudge was then replaced by
an identiﬁer to punctuation conversion fudge in the phase 7 tokenization grammar.
Neither fudge is required in the next approach:
```

#### 4.2.1.3 Syntax-level substitution

Syntax-level substitution uses a syntax tree from an earlier syntax analysis as the replacement. The replacement is therefore constrained to satisfy an explicit syntactical type, rather than the much weaker constraints of sequence-ofcharacter or sequence-of-token.

Syntax level substitution

- further improves efﬁciency since no source code experiences repeated lexical or syntactic analysis

- supports improved error diagnosis since the deﬁnition of a replacement can be checked syntactically

- avoids the syntactical interpretation changing in response to its instantiation context (the parenthesis problem of Section 3.4.1).

The informal restriction on the occurrence of structuring tokens is removed, or rather becomes intrinsic to the substitution model; it is difﬁcult to deﬁne structuring tokens as replacements, since partial syntax does not satisfy syntactical requirements.

```
Direct syntactic analysis of the entire example is supported by incorporating an
AST node for $i. This node is then resolved and evaluated during each iteration
of the loop. With syntax-level substitution it is possible to complete syntactic
analysis of the entire program before any semantic analysis starts. Re-entrancy
occurs, but only within the conﬁnes of the semantic analysis.
```

The superset grammar implementation of FOG uses syntax-level substitution and avoids all context dependencies. Substitution below syntax-level is supported by deferred character-level substitution:

```
auto string braced_else = "} else";
$std::parse("if (a) { b; " $braced_else " c;");
// if (a) { b; } else c;
Although this supports dubious concatenation, the prevailing syntax type must
always be syntactically valid. The mismatching } preceding the else is only
mismatched while represented as a string. Conversion to a useful syntactic
element must wait until string concatenation provides valid source for std::parse
to convert to a generic syntax element: in this case a selection-statement.
```

For the C preprocessor, there is no looping and so references always occur after their deﬁnitions.

```
#define SOME_THING
...
...
SOME_THING
...
```

There is no ﬂexibility in the location of a resolved deﬁnition. Replacements occur directly, with complexity arising only for formal parameters of function-like macros, for which replacement text is affected by the later actual argument.

```
#define DO_SOME_THING(ResolveLater) ... ResolveLater ...
...
...
DO_SOME_THING(WITH_THIS)
...
```

The same is not true for FOG.

While syntax-level substitution is efﬁcient and predictable, it inhibits characterlevel and token-level substitution.

```
In practice, many substitutions would satisfy the constraint that an invariant
deﬁnition lexically precede a reference from a determined context. So it is
tempting to allow such backward reference substitutions in which replacement
characters or tokens could be substituted while the source was at character or
token level. However this would be a special behaviour that could change
unexpectedly as a program evolved. Provision of std::parse to allow an explicit
recursion from semantic level back to the lexical level seems to satisfy the
potential requirements for lower level substitution without introducing irregular
behaviour. Other requirements are satisﬁed by the concatenation mechanism.
```

#### 4.2.1.4 Semantic-level substitution

The three preceding levels of substitution correspond to different degrees of validity for the deﬁnition of a replacement:

A character-level replacement is lexically indeterminate:

- an arbitrary sequence of valid characters

A token-level replacement is lexically valid but syntactically indeterminate:

- an arbitrary sequence of valid tokens

A syntax-level replacement is syntactically valid but semantically indeterminate:

- an arbitrary tree satisfying a valid syntax

A fourth semantic-level substitution is possible using a replacement that is semantically as well as syntactically valid. This level is not possible in FOG, since semantic analysis requires type information which may come from the surrounding context. The replacement cannot be semantically analysed in isolation. Overall semantic validity is determined where the replacement is used, although some checks can be made at the deﬁnition site.

### 4.2.2 Syntactic Polymorphism

Replacement of syntactically consistent trees allows some rather dubious programs to be written:

```
auto expression decl0 = i;
auto declaration decl1 = int i;
auto for (unsigned j = 0; j < 2; ++j)
$decl$j = k;
On the ﬁrst iteration decl0 is selected1 and so the loop body is an expression:
i = k;
On the second iteration using decl1, the loop body is a declaration:
int i = k;
```

The variation in syntactical type is legal, since there is no requirement for uniformity.

```
The declaration form is only legal in FOG because int i satisﬁes the generalised
naming that resolves int i as a name before assigning k to it, resulting in the
syntax tree shown in Figure 4.7.
This is not completely consistent with the exposition of the C++ grammar in the
standard, for which i = k is resolved as an init-declarator, before the int preﬁx is
applied as the decl-speciﬁer-seq of a simple-declaration. However, since C++ ascribes
no semantics to partial declarations, FOG does not create an incompatibility by
deﬁning them.
```

The ordering in the FOG grammar is necessary to support context free syntactic analysis. The FOG grammar should therefore be expressed in a way more closely resembling the practical implementation in Appendix C rather than the changed C++ grammar of Appendix A.

### 4.2.3 FOG substitution

The C++ reference model (§2.1) provides for each of the minor translation phases to be performed in sequence. However, the many responsibilities of phase 7 (the main compilation phase) are just bundled together, reﬂecting the apparent need for syntactic and semantic analyses to be tightly coupled.

```
In FOG, resolution of a tree-literal is detected during the phase 6 (string
concatenation) processing but requires the use of phase 7 syntactic processing to
identify the tree-expression syntax and any meta-function arguments. FOG originally
used syntax-driven parsing to resolve each argument according to its known
syntax. This imposed tight semantic coupling and required deferred analysis in
contexts where the syntax was initially unknown. FOG is now context-free and so
no syntactical knowledge or deferral is required.
1. The right to left evaluation of nested $-expressions is explained in Section 3.1.1.6.
=
i
int
k
typed-name
```

**Figure 4.7 int i = k**

```
The early translation stages naturally operate in a demand-driven fashion with the
phase 7 syntactical analysis, in the main grammar, making repeated requests to
a phase 6 procedure to provide the next token for syntactic analysis. When phase
6 detects a tree-literal, phase 7 processing is activated re-entrantly to acquire and
analyze the source tokens of the tree-literal. These are returned to the interrupted
phase 7 processing as a tree-literal token, which describes the unresolved tree-
expression. Recognition of tree-literals may occur re-entrantly to arbitrary depth.
```

FOG therefore introduces a need for re-entrant invocation of phase 7 syntactic processing, but achieves an overall simpliﬁcation through the separation of syntactic and semantic processing. All syntactic processing can be completed before any semantic processing starts2.

```
This is important since, in general, semantic information is not available. When the
tree-literal is encountered in the meta-function body of:
auto declaration defer(class clientClass)
{
${${clientClass}::deferred($Scope)};
}
it is only known that clientClass identiﬁes a class. It is not known which class,
so the existence or signature of the
deferred meta-function cannot be
determined, and the required syntax type of the parameter is unresolvable. The
invocation context is unknown, so the syntactical type of the $Scope argument is
also unknown.
```

Syntax-driven parsing is not possible when the syntactical type of either a formal parameter or an actual argument cannot be determined. In the example, neither can be known. The FOG grammar must therefore be context-free.

The example is resolved by initially parsing only for a generic syntax element. Eventually the syntax element is used in a deterministic context where semantic interpretation ensures compliance with the syntax types.

**tree-literal deﬁnition**

```
Deﬁnition of a tree-literal in one context, generally involves names that are resolved
in another context.
auto expression e = a(*b);
The initializer a(*b) is therefore parsed against the generic tree-statement syntax,
which accepts almost any C++ sentence, including many that are not expressions.
Association of the initializer with the meta-variable performs only a weak semantic
validation to verify that the initializer could satisfy the syntax of an expression. It
does not matter that the initializer could satisfy more than one syntax: a function
call or a function parameter with a redundantly parenthesised parameter.
```

When the meta-variable is used in a determined context:

```
class X
{
typedef int b;
class $e;
// class a(*int);
};
two errors can be detected. First that the value of e does not satisfy a strong
semantic check of the syntax of an expression, and secondly that this particular
expression is not a valid name for a class.
```

The ﬁrst of these errors can be subverted by use of a syntactically weaker metatype.

2. Semantic processing of syntax macros must occur at the syntactic level.

```
auto token f = $e;
class $f;
The original initializer is checked to verify that it could satisfy the syntax of a token
(which everything satisﬁes). When class $f is checked, the strong check then
detects that this particular token is not a valid name for a class.
```

**tree-literal syntactical analysis**

The deferred invocation example

```
auto declaration defer(class clientClass)
{
${${clientClass}::deferred($Scope)};
}
showed that the syntactic type of tree-literals is not necessarily known during
syntactic analysis of sentences involving tree-literals. This would appear to
preclude syntactic analysis of the meta-function body at the deﬁnition site. Even
if the syntax type could be known, its use introduces a tight coupling between the
semantic analysis that determines syntax types and the syntactic analysis that
uses these types.
```

Revising the C++ grammar to incorporate indeterminate contributions is not possible. The result would be totally ambiguous:

```
$a $b $c $d ;
could be a namespace-alias-deﬁnition:
namespace x = y ;
```

amongst very many alternatives.

```
A useful compromise leaves the C++ grammar almost untouched, but supports
most practical replacements. The compromise makes the simple assumption that
each occurrence of a tree-literal represents an identiﬁer. Syntactical analysis and the
grammar is therefore only affected by the categorisation of a tree-literal as an
identiﬁer.
This assumption imposes minor limitations. tree-literals cannot be used to source
keywords or punctuation where these deﬁne program structure. The use of $a and
$c for the namespace-alias-deﬁnition example is therefore impossible.
The assumption is much less restrictive than it might appear. It does not require
the tree-literal to be an identiﬁer, merely to be used where an identiﬁer could be used.
identiﬁer is a degenerate sentence for many syntaxes:
```

- a type name (since there is no type/non-type discrimination)

```
•
any form of expression (identiﬁer is a primary-expression)
•
a parameter-declaration(-clause) (identiﬁer is a type)
•
a decl-speciﬁer(-seq) (identiﬁer is a type)
identiﬁer ; is a degenerate sentence for most other syntaxes:
•
any form of statement (identiﬁer ; is a degenerate expression-statement)
•
any form of declaration (identiﬁer ; is a degenerate simple-declaration)
```

It is only for very narrow syntaxes involving just reserved words or punctuation that a substitution cannot be allowed, for example:

```
•
access-speciﬁer
•
class-key
•
cv-qualiﬁer
•
function-speciﬁer
•
operator
•
ptr-operator
•
storage-class-speciﬁer
•
unary-operator
These limitations do not prevent deﬁnition and usage of these syntactic types,
however they do prevent parameterisation of syntax that uses them. For instance,
a class/struct/union tag cannot easily be used directly:
auto class_key ClassKey = struct;
// Ok
$ClassKey Class { /* ... */ };
// Illegal
Some of these restrictions can be worked around by using a wider syntactical type
such as decl-speciﬁer rather than cv-qualiﬁer. Others could be worked around by
relaxing the assumption that all tree-literals are identiﬁers so that a functional cast
can specify the built-in meta-type.
$class_key($ClassKey) Class { /* ... */ };
However it seems perverse to cast something to its own type, and even more
perverse to do so without validation. It seems that some variant of a $-expression
is needed that uses the semantic knowledge where the programmer can
guarantee that the type satisﬁes ‘appropriate’ constraints on being ‘adequately’
resolvable. Choosing a satisfactory syntax and resolving ‘appropriate’ and
‘adequate’ is a matter for further research.
```

Restrictions involving punctuation or reserved words acting as punctuation cannot sensibly avoided. There is no point introducing single-valued ‘casts’ to reserved words or punctuation, when the reserved words or punctuation could be more easily used directly:

```
$'namespace'($a) $b $'='($c) $d;
// Silly non-FOG example
```

## 4.3 Name Resolution

```
In C++, there are three categories of name-space (as distinct from namespace).
```

- the hierarchical name-space of (run-time) program declarations

- template-names - type-names - non-type-names

```
•
the per-function name-space of labels available for use by gotos.
```

- the C preprocessor macro deﬁnition name-space

FOG adds

- the hierarchical meta-name-space of (compile-time) program declarations.

FOG removes the distinction between template, type and non-type names at the meta-programming level, since meta-programming may occur before such distinctions are possible. The distinction is preserved where C++ semantics apply.

The C++ name-spaces are used for resolving names of conventional and extended C++ declarations, and for the scopes of meta-names.

```
The meta-name-space is explicitly used when resolving tree-expressions
$Scope
@bases()[0].is_virtual()
```

and implicitly by the left-hand side of meta-expressions (within meta-statements)

```
auto if (meta_variable) /* ... */;
Resolution of a tree-expression involves four phases
```

- location of an object in the run-time name-space

- location of the meta-object that describes it

- evaluation of expression operators that use meta-objects

- conversion of the resultant meta-object to suit the invoking context

```
A meta-object for use in tree-expression is identiﬁed by a meta-scoped-id
meta-id:
id
meta-type
auto
meta-nested-id:
meta-id
~ meta-id
meta-id :: meta-nested-id
meta-scoped-id:
::opt meta-nested-id
The names are resolved in the conventional name-space, with the minor
generalisation that meta-classes such as unsigned int or auto, which have no
conventional classes are treated as having empty classes.
```

Thus, considering the meta-objects in Figure 4.8, the tree-literal

```
$X::Y::f.is_static()
is resolved by successively locating X, Y and f using the conventional name-space
to identify the «function meta-object» that describes the «function» X::Y::f.
Invocation of is_static() upon this «function meta-object» tests whether the
«function» X::Y::f has a static qualiﬁer.
```

Whereas in:

```
$X::Y.g.is_static()
```

«meta-function meta-object»

X::Y::g

«function»

X::Y::f

«function meta-object»

X::Y::f

«describedBy»

«instance meta-object»

X::Y

member-names meta-member-names

**Figure 4.8 Meta-function and function meta-object distinction**

«instance» «describedBy»

«meta-function»

X::Y::g

«describedBy» «describedBy»

meta-level meta-meta-level

«class»

«class meta-object»

X::Y «meta-class»

«instance meta-meta-object»

an X::Y

```
X, Y are successively located using the conventional name-space to identify the
«class meta-object» that describes the «class» X::Y (not «instance», since X::Y
is a class-name not an instance-name). Selection of the g member selects the
child «meta-function meta-object» named g. Invocation of is_static() upon this
«meta-function meta-object» tests whether the «meta-function» X::Y::g has a
static qualiﬁer.
These two mechanisms can coexist without conﬂict, since the «function meta-
object» describing a «function» is not a «meta-function». This is shown in
Figure 4.8 and discussed in greater detail in Section 4.5. The meta-objects of the
members of a «class» are not members of their corresponding «meta-class».
Therefore :: always performs resolution of references in the class/namespace
hierarchy, and . or -> are resolved within the prevailing name-space.
Note that this has the corollary that when deﬁning a meta-variable or meta-
function, either :: or . may be used for the ﬁnal scoping:
auto int Class.meta_variable = 0;
// Correct
auto int Class::meta_variable = 0;
// Also correct
```

Note also that, as might be expected,

```
$v.is_static()
```

is a short form of

```
$the-current-context3.v.is_static()
so that v is located in the meta-object describing the current context. The meta-
object describing v is not the start of the resolution.
The meta-name-space is used for meta-class, meta-function and meta-variable
names. It is not used for meta-function arguments or the right-hand side of a meta-
assignment, which, in the absence of further $ or @ operators, represents a literal
AST to be interpreted in the usage context.
```

Search for a name is always restricted to the expected name-space, however it should be noted that template parameters are present in both name-spaces.

```
A $ or @ preﬁx is necessary to access the meta-name-space where the run-time
name-space would normally be used.
```

### 4.3.1 Search Locations

C++ has complicated rules (§3.4) for determining when and where names should be resolved:

A name may be resolved within

- the enclosing local block hierarchy

- the local object

- a parameter list

- the local class

- the base class hierarchy

- the enclosing (structural) class hierarchy

- the enclosing (structural) namespace hierarchy

```
3. the-current-context is This, and so $This.v.is_static() may be used.
However, it is still necessary to use the meta-object describing the current context
to locate This, since This is deﬁned only in the meta-name-space.
```

FOG adds further complications. The resolution of a name may be available within

- the meta-name-space

- the enclosing lexical hierarchy

- the local block hierarchy

- the root or derived scope of an actual declaration

- the deﬁnition or invocation context of a meta-function

```
The C++ name resolution rules are complicated, and so arbitrary extension to
cover new possibilities would cause considerable confusion. Resolution of names
within different name-spaces in conventional contexts is therefore signalled
explicitly by the @ or $ operators.
```

### 4.3.2 Meta-name-space contents

```
Resolution of a name within the conventional run-time name-space proceeds
according to the rules deﬁned in the C++ standard. Resolution within the meta-
name-space is triggered by the use of a $ or @ preﬁx to establish a tree-literal, or
by the use of a meta-scoped-id on the left-hand side of a meta-expression.
```

Resolution of a name within the compile-time meta-name-space proceeds according to similar principles as run-time resolution, but without the contextdependent visibility of type-names. All names in a meta-name-space context are visible to all searches of that context. The search for a name proceeds in stages, with different candidate sets of locations considered in each stage. The search terminates at the end of the ﬁrst search stage in which a deﬁnition is found, or when all stages have been completed. It is an error for more than one distinct resolution to be found in a search stage.

The search stages are in order:

**Local scope**

The search for a meta-name in a meta-program may ﬁnd:

- meta-variables declared in a local scope established by a loop or compound-statement.

**Local object-scope**

The search for a meta-name in a meta-object may ﬁnd (using a function as an example):

```
•
meta-functions declared in an object-statements-clause
•
meta-variables declared in an object-statements-clause
•
the built-in meta-variables of the function meta-type
-
This resolving to the meta-object of the function
•
the built-in meta-functions of the function meta-type
```

**Meta-class**

The search for a meta-name in a class meta-object occurs either through use of the class, or through failure to resolve the name in a local-object scope. The search for a meta-name in a class meta-object may ﬁnd:

```
•
meta-functions declared in a class-speciﬁer
•
meta-variables declared in a class-speciﬁer
•
the built-in meta-variables of the class meta-type
-
This resolving to the (meta-)class
-
Scope resolving to the (meta-)class
-
OuterScope resolving to the less nested (meta-)class
•
the built-in meta-functions of the class meta-type
```

- the formal template parameters.

Search of a class meta-object does not ﬁnd

- the class name or base class names

- nested types (classes, enums or typedefs)

- member functions

- member variables

- enumerators

or their meta-objects.

**Base Meta-classes**

If a meta-name is not found directly in a meta-class, the search proceeds recursively by searching each base meta-class and recurses for each of the base classes, stopping individual searches when a name is found or after the root base meta class has been searched. Repeated resolutions with the same value are discarded. Ambiguous resolutions are an error.

**Less nested base meta-classes**

If a meta-name is not found in a meta-class or its base meta-class, the search is repeated successively for the meta-classes of each hierarchically less nested class stopping once resolved, or when the namespace scope is reached.

**Meta-namespace**

The search for a meta-name in a meta-namespace may ﬁnd:

```
•
meta-functions declared in a namespace-body
•
meta-variables declared in a namespace-body
•
the built-in meta-variables of the namespace meta-type
-
This resolving to the (meta-)namespace name
-
Scope resolving to the (meta-)namespace name
-
OuterScope resolving to the (meta-)namespace name
-
Namespace resolving to the (meta-)namespace name
-
OuterNamespace
resolving
to
the
less
nested
(meta-)namespace name
•
the built-in meta-functions of the namespace meta-type
```

**Less nested meta-namespaces**

If a meta-name is not found in the namespace, the search is repeated successively for each structurally less nested namespace stopping once resolved, or after the unnested global namespace has been searched.

The built-in functions and variables for the meta-types are summarised in Appendix E.

### 4.3.3 The Substituted Value

```
When a substitution requested by a $ or @-expression is resolved, the expression
is resolved in the invoking context to identify the meta-object whose value
replaces the expression.
$i[1]
```

The meta-object must necessarily have already been deﬁned, at which point any $-expressions that reference the deﬁning context are resolved.

```
auto token i[] = { ""$Scope, ""$Super };
Once the meta-object has identiﬁed any $-expressions referring to formal
arguments are replaced. This involves not only the explicit parameters of a meta-
function, but also the implicit Static, Dynamic and template parameter names for
meta-functions or meta-variables.
Each replacement takes the form of an already parsed syntax tree, albeit with
residual $ and @-expressions which are then resolved after substitution into the
invoking context. Execution of a meta-function body therefore occurs within the
invoking context, since:
Multi-$ expressions are resolved when the meta-function is deﬁned (see
Section 4.3.6).
Single-$ expressions are replaced as the meta-function body is substituted within
the invoking context.
@-expressions are resolved, and meta-statements executed each time the
expression is analysed within the invocation context.
```

The following example

```
auto statement nextDay(string dayText)
{
static const char *dayTable[] = { $dayText };
auto dayCount++;
}
auto int dayCount = 0;
$nextDay("Sunday");
$nextDay("Monday");
$nextDay("Tuesday");
$nextDay("Wednesday");
$nextDay("Thursday");
$nextDay("Friday");
$nextDay("Saturday");
static const int maxDay = $dayCount;
```

results in the equivalent C++ code:

```
static const char *dayTable[] =
{ "Sunday", "Monday", "Tuesday", "Wednesday",
"Thursday", "Friday", "Saturday" };
static const int maxDay = 7;
showing how the dayCount++ expression is executed seven times within the
invoking context. The array initializer is similarly executed seven times, with the
```

initializer resolved from the formal argument; the redeclarations compose as described in Section 4.4.7 to give the composite array declaration.

### 4.3.4 Derived context resolution

```
A declaration speciﬁed using a derivation rule may need to use names referring
to the root scope or the derived class. The following example (elaborated in
Section 7.1.4) deﬁnes a clone method for a RootClass and a derivation rule to
implement the protocol automatically for all derived concrete classes.
public virtual $Scope *RootClass::clone() const = 0
:{
derived(!Scope.is_pure())
{ new @{Scope}(*this); }
};
The body of the clone method must refer to the derived class, but the return type
must use the root scope (on pre-standard compilers).
```

The two requirements are resolved by the different substitution operators.

```
A $-expression is resolved within the deﬁning context of a declaration. This is
RootClass in the above example.
An @-expression is resolved as late as possible, which is generally within the
actual context of a declaration. This is the derived class in the above example. ‘As
late as possible’ may arise at three distinct times, depending upon the context of
the @-expression:
```

**@-expression in scope**

```
An @-expression forming part of the scope of a declaration is resolved when the
declaration is interpreted in a determined context in order to install the declaration
in the determined scope. This applies to each @-expression in:
class @Outer
{
int @Inner::i;
};
Since this form of usage that creates an actual declaration directly, there is no
distinction between a (single $) $-expression and an @-expression. However a
distinction does arise when the conversion to actual is deferred; the @-expression
is resolved in the actual context, whereas a $-expression is resolved in the
deﬁning context.
auto identifier Outer = "Outside";
auto identifier Inner = "Inside";
auto class_specifier cachedClass1 = class $Outer
{
int $Inner::i;
};
auto class_specifier cachedClass2 = class @Outer
{
int @Inner::i;
};
This deﬁnes the meta-variable cachedClass1 with the value
class Outside { int Inside::i; };
and the meta-variable cachedClass2 with the value
class @Outer { int @Inner::i; };
```

Immediate usage in which the invoking context is the same as the deﬁning context

```
$cachedClass2;
```

results in

```
class Outside { int Inside::i; };
as would also be the result for $cachedClass1. However cachedClass2 can be
invoked from an alternate context:
class AnotherContext
{
auto identifier Outer = "Without";
auto identifier Inner = "Within";
$cachedClass1;
// class Outside { ... }
$cachedClass2;
// class Without { ... }
};
with the result that the deferred resolution of $cachedClass2 uses the distinct
invocation context.
```

**@-expression in name**

```
An @-expression occurring as part of the naming of a declaration is resolved when
the signature is resolved to install a declaration in its actual scope. Resolution of
the @-expression may then respond to any change of scope caused by derivation
rules, or meta-programming.
The naming comprises the name, types and parameter names, but not default
initializers. (Parameter names do not form part of the unique signature of a
declaration.) This therefore applies to each @-expression in:
class X
{
auto string prefix = "p";
@Scope *accept_@{Scope}(@Scope& @{prefix}@Scope)
:{ derived(true) { return &@{prefix}@Scope; } };
};
class Y : public X
{
auto prefix = "q";
};
```

which consequentially generates the functions:

```
X *X::accept_X(X& pX) { return &pX; }
Y *Y::accept_Y(Y& qY) { return &qY; }
The lexical concatenation of accept_ and @{Scope} although syntactically
analysed as an identiﬁer remains unresolved until the declaration is installed in its
actual scope.
```

**@-expression in body**

```
An @-expression occurring as part of the body of a declaration is resolved when
the body is compiled in the ﬁnal compilation phase prior to emission. This differs
only slightly from the earlier example, by deferring resolution till all meta-
programming for all declarations has completed, except for side effects caused by
other @-expressions also in declaration bodies.
```

The body of a declaration comprises

- the body of a function

```
•
the handler of a function-try-block
•
the initializer of a ctor-initializer
•
the parameters of exception-speciﬁcations
```

- the initializer of a variable

- the default-initializer of a function argument

- the predicate of a derivation rule

```
static int X::member_variables = @variables().size();
static bool X::f(int i = @default_init())
:{
derived(@predicate()) { return i == @threshold(); }
};
```

Thus:

```
class HeapAllocated
{
protected virtual ~@{Scope}() :{ derived(true) {} };
};
enforces protected access on the destructor throughout a class hierarchy. The
derivation rule ensures regeneration in all derived classes, and ~@{Scope}
speciﬁes the correct name in each derived class.
```

Note that:

```
class Base
{
@{Scope}(const $Scope&) :{ derived(true) { /* ... */ } };
};
class Derived : public Base {};
provides a copy constructor in the root scope where @Scope and $Scope are the
same class, and so generates:
Base::Base(const Base&) { /* ... */ }
```

but an ordinary constructor in derived classes, where the failure to defer resolution results in:

```
Derived::Derived(const Base&) { /* ... */ }
The ﬁnal $Scope must be replaced by @Scope to specify a copy constructor
throughout the class hierarchy.
```

### 4.3.5 Lexical scope resolution

In C++, there is little need to use lexical nesting for name resolution since structural nesting is substantially the same.

```
class Outer
// ::Outer
{
public:
/*...*/ name /*...*/;
// ::Outer::name
class Inner
// ::Outer::Inner
{
/*...*/ name /*...*/
// resolved in structural scope.
};
};
Resolution of name searches in ::Outer::Inner, then ::Outer (and ﬁnally ::).
```

The FOG generalisation to support interleaved declarations and arbitrary scopings means that a lexically enclosing scope is not necessarily a structurally enclosing scope.

```
namespace Domain
{
class Outer
// ::Outer
{
public:
/*...*/ name /*...*/;
// ::Outer::name
class ${Namespace}::Sibling
// ::Sibling
{
/*...*/ $name /*...*/
// Error - unresolved.
/*...*/ $Outer::name /*...*/ // Ok - resolvable.
};
};
}
Resolution of $name searches in Domain::Sibling and Domain and ::, but not
in Domain::Outer, so the deﬁnition is only visible when explicitly qualiﬁed as
$Outer::name. The lack of visibility introduces signiﬁcant problems for a meta-
function such as:
auto declaration create_sibling()
{
/*...*/ name /*...*/;
public class ${Namespace}::Sibling
{
/*...*/ $name /*...*/
};
}
The meta-function establishes some declarations, such as name, in its invocation
context. These should be visible to other declarations in lexically nested contexts.
The surrounding context is available as $Scope, which could be used to explicitly
qualify the reference, except that the reference occurs within a lexical nesting that
redeﬁnes Scope. Caching the value of Scope in a temporary meta-variable across
the lexical boundary is possible but hardly elegant.
A solution is provided by extending the $ operator.
$x resolves x in the prevailing context.
$$x resolves x in the surrounding lexical context.
$$$x resolves x in the surrounding surrounding lexical context.
```

etc.

The meta-function can therefore be written in re-usable fashion as

```
auto declaration create_sibling()
{
/*...*/ name /*...*/;
public class ${Namespace}::Sibling
{
/*...*/ $$name /*...*/
};
}
```

### 4.3.6 Deﬁning or invoking resolution

Name references within C++ functions normally occur as if resolved within the runtime execution context. With the exception of virtual functions, these resolutions actually occur at compile time, and therefore make use of the deﬁning context which is known at compile-time.

Name references within FOG meta-functions occur when the meta-function deﬁned in a (deﬁnition) context is invoked from another (invocation) context. Both contexts are known at compile-time, and since meta-programming is manipulating declarations, resolution within the invocation context as well as the deﬁnition context can be useful.

```
The semantics of meta-function execution is that formal parameters are replaced
and then the entire body of the meta-function is returned for interpretation within
the invocation context (see Section 4.3.3). This ensures that the invocation
context can be manipulated but prevents the deﬁnition of a meta-function reacting
to its deﬁning context. The following example attempts to conditionalise the
behaviour of a meta-function in accordance with a DEBUG command line variable:
auto bool debug = $std::get_cpp("DEBUG");
auto declaration f()
{
if ($debug)
// Not the way to do it
/* ... */
/* ... */
}
class X
{
auto declaration debug() { /* ... */ }
$f();
};
Resolution of
$debug occurs within the invocation context and so the
conditionalisation is resolved by the non-zero body of the meta-function
X::debug.
```

This problem is well known to Lisp programmers who call it the functional argument or FUNARG problem. A segment of code with free symbols is passed to a context where the free symbols may obtain an unexpected resolution.

Although this is a different problem to the lexical scoping problem, it is amenable to the same solution. Deﬁning a meta-function declaration as establishing a nested lexical scope, causes the rewritten meta-function:

```
auto declaration f()
{
if ($$debug)
/* ... */
/* ... */
}
to perform the resolution of debug in the surrounding lexical context ensuring the
intended resolution to ::debug.
```

### 4.3.7 Multi-$-expression resolution

```
The generalisation of $-expressions is logical and predictable. The following
example has nested meta-functions deﬁned within nested classes. The outer
meta-function A::B::f() is invoked within D::E to deﬁne the inner meta-function
D::E::A::g(), which is invoked within F::G.
//6
class A
{//5
class B
{//4
auto declaration f()
// A::B::f
{//3
class A
{//2
auto declaration g()
{//1
/*reference*/
}
};
}
};
};
class D
{
class E
{
$A::B::f();
};
};
class F
{
class G
{
$D::E::A::g();
};
};
Considering alternative invocations at /*reference*/, for which Scope is a
convenient built-in meta-variable to demonstrate the context in which name
resolution occurs:
$Scope resolves within //1, which is the meta-function body that replaces the
invocation as $D::E::A::g() from ::F::G. It therefore resolves to ::F::G.
$$Scope resolves within //2 which is nested scope within meta-function body that
replaces the invocation as $A::B::f() within
::D::E. It therefore resolves
to ::D::E::A.
$$$Scope resolves within //3, which is the meta-function body that replaces the
invocation as $A::B::f() within ::D::E. It therefore resolves to ::D::E.
$$$$Scope resolves within //4, which is a nested scope and resolves to ::A::B.
$$$$$Scope resolves within //5, which is a scope and resolves to ::A.
$$$$$$Scope resolves within //6 which is the global namespace ::,.
$$$$$$$Scope is an error since there is no surrounding context for the global
namespace.
```

And for completeness:

```
Scope is an identiﬁer.
@Scope resolves to ::F::G, or a class derived from ::F::G, if the reference
occurs within a declaration that is regenerated by a derivation rule or meta-
program.
This is clearly not a complete set of all contexts, since neither D nor F appear
although they constitute lexically surrounding scopes of contexts that do appear.
However the existence of D and F are implementation details that cannot be known
to the author of the meta-functions. A more elaborate 2-dimensional scheme
supporting access to the third surrounding lexical context of the second
surrounding meta-function invocation would therefore be inappropriate.
The relevant lexical context is readily identiﬁed by counting back through braces
surrounding the /*reference*/ invocation expression. It is independent of the
subsequent usage. Each additional $ causes resolution in a less nested context:
the invocation context for a meta-function body or the internal context of a class
body.
```

Nested lexical contexts are established:

```
•
between { and } of a class (or namespace) body
•
between { and } of a meta-function body
•
between :{ and } of an object-statements-clause
```

Nested lexical contexts are not established for:

```
•
{ and } or :{ and } within :{ and }
•
structural nesting established by ::
```

- compound statements

- compound declarations

- ﬁlespaces (see Appendix F.4.4)

- initializer clauses

### 4.3.8 Transferred lexical scope for object-statements-clauses

```
When meta-function or class bodies nest within an object-statements-clause, the object-
statements-clause establishes a lexical context. The function bodies do not establish
a further context.
auto statement f()
:{
// Outer grouping :{} is a lexical scope
derived(true)
:{
// Grouping :{} is not a lexical scope
entry
{
// Grouped {} is not a lexical scope
/* ... */
};
};
};
```

### 4.3.9 No lexical scope for :: nesting

```
It might seem that :: should establish a nested lexical scope so that there is no
difference between the following pair of meta-functions:
auto identifier A::B::f() { $$Scope; }
class A
{
class B
{
auto identifier g() { $$Scope; }
};
};
```

However this would introduce an inconsistency for

```
auto $X A::B::h() /* ... */
```

which by analogy with the C++ declaration

```
X A::B::h() /* ... */
would suggest that X be resolved in its surrounding (global) context rather than the
nested A::B context. There is no strong reason why f() and g() should behave
in the same way. C++ consistency requires that they do not. Therefore :: does not
establish a lexical scope.
```

### 4.3.10 No Lexical scope for initializers and arguments

Not establishing lexical contexts for initializer clauses seems obvious since in

```
int scalingMatrix[2][2] = { { $Scale, 0 }, { 0, $Scale } };
```

the braces are a grouping operator, they do not create a lexical context in which declarations could be differently resolved.

```
It is less clear whether the = and ; of a variable initializer or the ( or , and , or )
surrounding a meta-function argument should establish a nested lexical scope.
```

At ﬁrst sight it is clear that in

```
class X
{
auto identifier p = $Scope;
$f($Scope);
};
$Scope should be resolved as X using the prevailing context. However, the
semantics of meta-function execution return the body of a meta-function for
interpretation in the invoking context. Perhaps the initializers and arguments
should also be interpreted within their usage context. It is then necessary to use
$$Scope to ensure that the above example has the obvious behaviour.
Considering a more complicated meta-function call in which a class-speciﬁer is
passed as a meta-function parameter to create the class To::X.
auto declaration f(class_specifier c)
{
class ::To
{
$c;
};
}
class Call::From
{
$f(class X
{
... $Scope ...
... $$Scope ....
});
};
```

which expands to

```
class Call::From
{
class ::To
{
class X
{
... $Scope ...
... $$Scope ....
};
};
};
$Scope should clearly resolve to ::To::X, rather than Call::From (which is not
visible from ::To::X), or Call::From::X (which may not ever exist).
$$Scope resolves to a surrounding context, but is it Call::From (that surrounding
the deﬁnition of the argument) or ::To (that surrounding the instantiation
as ::To::X)?
Either resolution is tenable, however resolution as To in the invoking context
involves the use of a surrounding context that is not lexically identiﬁed where a
meta-variable is initialized or where a meta-function argument speciﬁed. It does
not seem necessary to make this scope easily accessible. This invisibility of
remote contexts mirrors the invisibility of the remote lexical scopes for D and F in
the nested class and meta-function example of Section 4.3.7. Therefore
initializers and meta-function arguments do not establish a nested lexical scope:
resolution of $-expressions has the obvious behaviour.
Although the remote surrounding lexical context is not directly accessible, it can
be accessed indirectly. When X is structurally as well as lexically nested, Scope
can be resolved in the surrounding structural scope by $OuterScope. Even when
X is not structurally nested, access is possible by creating a dummy lexical and
structural nesting:
class From
{
$f(auto class ExtraNesting
{
class ::X
{
... $Scope ...
... $$OuterScope ....
};
});
};
The structurally and lexically nested ExtraNesting meta-class establishes an
additional lexical context surrounding X, which can be reached using $$, and from
which OuterScope can traverse to the surrounding structural (and lexical) context.
```

### 4.3.11 Formal parameters

```
Formal parameters are visible throughout a meta-function-body, including any
nested lexical contexts, so it is unnecessary to use additional $’s to access formal
parameters from within nested lexical contexts.
auto declaration X::f(identifier outerParameter)
{
$resolved_in_invocation_context;
$$reaches_out_to_defining_context;
auto declaration h(identifier innerParameter)
{
class $outerParameter
{
$innerParameter;
// one $ is enough
$outerParameter;
// one $ is enough
};
}
}
The semantics of meta-function execution, that resolves tree-literals primarily in
the invocation context, makes the deﬁnition context less accessible and requires
the use of a multi-$-expression. This is not adequate for all forms of access. For
instance in the above example, deﬁnition as X::f rather than class X { ...
f }; prevents access to X using a built-in meta-variable. If the meta-function is
invoked in a derived class,
neither form of deﬁnition supports access to the
derived deﬁnition class name. Two built-in formal parameters are therefore
supplied to circumvent the problems.
The Static built-in formal parameter resolves to the static deﬁnition scope of the
meta-function.
The Dynamic built-in formal parameter resolves to the dynamic deﬁnition scope of
the meta-function.
auto class MetaBase
{
auto static declaration static_mf()
{
public static char *static_dynamic = ""$Dynamic;
public static char *static_static = ""$Static;
public static char *static_scope = ""$Scope;
}
auto declaration nonstatic_mf()
{
public static char *nonstatic_dynamic = ""$Dynamic;
public static char *nonstatic_static = ""$Static;
public static char *nonstatic_scope = ""$Scope;
}
};
auto class DerivedMetaBase : auto MetaBase {};
class Invoking
{
$DerivedMetaBase::static_mf();
$DerivedMetaBase::nonstatic_mf();
};
Within the invocation of DerivedMetaBase::static_mf, $Dynamic resolves to
MetaBase, whereas within the invocation of DerivedMetaBase::nonstatic_mf,
$Dynamic resolves to DerivedMetaBase. In both cases $Static resolves to
MetaBase and $Scope to Invoking.
```

Formal template parameters are treated as formal parameters supporting use of the deﬁnition parameterisation in the invocation context.

```
These additional built-in parameters may hide names in the invocation context,
necessitating the use of $This.Static to resolve Static in the invocation
context rather than as a built-in formal.
```

### 4.3.12 Meta-function and substitution semantics

Declarations within meta-function bodies are analysed in three contexts

- when the meta-function is deﬁned

```
-
to resolve (multi-)$-expressions that use formal parameters
-
to resolve multi-$-expressions in the deﬁnition context
```

- each time the meta-function is invoked

```
-
to resolve residual $-expressions in the invocation context
```

- when any generated declarations are compiled

```
-
to resolve @-expressions in the actual context
Formal parameters may hide declarations in nested classes, and may be hidden
by formal parameters of nested meta-functions. A formal parameter occluded in
this way may be accessed by using sufﬁcient $’s to reach out to a lexical context
in which the occluding formal is not visible.
auto declaration g(identifier id)
{
auto declaration h(identifier id)
{
$j($$id, $id);
// j(outer-id, inner-id)
}
}
It is an error if a $-expression reaches out to an external context where it cannot
be resolved.
The usage of the formal parameter names is identiﬁed during analysis in the
deﬁning context. The formal parameter names are therefore not visible while
resolving $-expressions in the invocation context.
auto declaration k(identifier aParameter)
{
${$nestedInvocation};
${$$nestedDefinition};
}
Therefore, if in the above example
$nestedInvocation has the value
aParameter, the subsequent access to aParameter is resolved in the invocation
context, ignoring the formal parameter. Furthermore, since $$nestedDefinition
reaches out to the external context, it is resolved in the deﬁning context and so if
nestedDefinition has the value aParameter, the $-expression is resolved to the
formal parameter.
Upon invocation of a meta-function, all accesses to formal parameters within the
meta-function body are replaced by their corresponding actual arguments before
the entire body and residual $-expressions are interpreted as part of the
invocation context.
```

## 4.4 Composition

The C++ One Deﬁnition Rule (§3.2) permits only one appearance of each declaration, and requires the declaration to have the same meaning in all compilation sessions in which it is used. A few exceptions to the rule permit repeated forward references and typedefs.

Violations of the rule within a single compilation should be trapped by the compiler. Violations between compilation sessions may go undetected and lead to unpredictable program behaviour. Some of these inter-session violations are diagnosed by the practice of using name mangling for function declarations.

Most opportunities for inter-session violations are eliminated by the practice of placing interfaces within include ﬁles that are shared between compilation sessions. As a result a program malfunction due to violation of the ODR arises mainly through the undue enthusiasm of some incremental compilers.

The ODR in combination with the hierarchical nature of C++ class declarations prevents declarations from more than one class being interleaved. This is a severe impediment to the implementation of patterns or Aspect Oriented Programming and so FOG relaxes the ODR with respect to FOG source code, requiring only that it be possible to satisfy the ODR after translation has been completed.

Multiple declarations are permitted and the contributions from each are combined hierarchically to form composite declarations. If the contributions are incompatible, the inconsistency is diagnosed and the resulting behaviour is unpredictable.

Composition is performed for the actual declarations of classes (including namespaces, structs and unions), enums, arrays, variables, functions, metavariables and meta-functions.

When any potential declaration is associated with a determined context, an existing actual declaration is ﬁrst located. If one is found, the new potential declaration is composed with the existing actual declaration. When searching for such a declaration, the search for the declared name is restricted to the speciﬁed scope, which is located conventionally.

```
Thus a declaration for A::B::f creates f within the scope visible as B within the
scope visible as A. It may compose within an existing f. It hides, rather than
composes with, an f that is visible in (by inheritance) but not part of A::B. This
provides consistency with the single deﬁnition in C++.
This implies that A::B::A::f is legal and probably refers to A::f since the more
nested A is visible as the less nested A. The unlikely alternative with an occluding
nested class is demonstrated by:
class A;
public class A::B;
private class A::B::A;
in which case A::B::A::f and A::f would be distinct entities.
Names must be made visible before they are used. In particular scopes must be
forward declared as in the cascade of declarations leading to A::B::A. Direct
declaration of a nested scope without its less nested scopes could be interpreted
as an implied forward declaration, but it leaves class/struct/union distinctions
and nested access constraints unclear. It also requires assumptions to be made
about the nature of the intervening names. These assumptions may be invalidated
by typedefs or additional base classes.
```

### 4.4.1 Class composition

Most class composition occurs hierarchically: the class grows to accommodate distinct member declarations, or to compose repeated member declarations.

#### 4.4.1.1 Nested contexts

```
Nested
class-speciﬁers,
namespace-deﬁnitions and
linkage-speciﬁcations compose
hierarchically.
```

Compound declarations are not nested contexts. They are just a syntactical grouping of multiple declarations as a single declaration. Declarations within a compound-declaration are therefore composed individually ignoring any compound structure.

Nested ﬁlespaces, described in Appendix F.4.4, are not nested contexts. The ﬁlespace just associates ﬁle-placement with its enclosed declarations. Declarations within a ﬁlespace are therefore composed without reference to the ﬁlespace beyond retention of their required placement. It is an error for composed declarations to have conﬂicting placement requirements.

#### 4.4.1.2 Base classes

```
A base-speciﬁer-list comprises an ordered list of base-speciﬁers. Composition occurs as
one or more lists of potential
base-speciﬁer declarations (each being a
base_specifier meta-object) are transformed into the single composed list of
actual base-speciﬁer declarations (each being a base meta-object). Additional
potential base-speciﬁers whose class-name is already on the list compose with the
existing actual. Additional potential
base-speciﬁers for new class-names are
converted to actuals and appended to the list.
Composition of a base-speciﬁer involves merging the access-speciﬁer and the virtual
keywords. An error arises if a conﬂict arises such as private with protected, or
virtual with !virtual. A conﬂict does not arise when merging a speciﬁed
access with an unspeciﬁed access or virtual with unspeciﬁed virtual.
class A : B, C, B, !virtual D {};
class A : E, protected B, F, private D, public E {};
```

composes to:

```
class A : protected B, C, private !virtual D, public E, F {};
The use of unspeciﬁed access is deprecated in C++ and defaults to private. This
remains the case in FOG, but only after composition, which may provide a
speciﬁed access.
```

#### 4.4.1.3 Miscellaneous declarations

**accessibility-speciﬁer**

```
An accessibility-speciﬁer changes defaults for subsequent declarations, within the
prevailing context. An accessibility-speciﬁer does not affect and is not composed with
other contributions to the same context.
```

**meta-expressions and meta-control-declarations**

Meta-programs are interpreted directly and so there is nothing to compose.

**include-declaration**

```
An include-declaration is interpreted directly and so there is nothing to compose.
```

syntax-macro-deﬁnition (Section 4.7)

Syntax macros with matching argument lists compose in the same way as metafunctions. Syntax macros with distinct argument list are overloaded and so do not compose.

using-directive namespace-alias-deﬁnition ﬁle-dependency-declaration (see Appendix F.4.5) explicit-instantiation explicit-specialization

Multiple declarations are gathered together. Duplicates are eliminated.

**asm-deﬁnition**

Multiple declarations are gathered together. Duplicates are preserved.

ﬁle-placement-declaration (see Appendix F.4.3)

Multiple declarations are gathered together. Only one distinct location may be speciﬁed for each interface and implementation ﬁle.

### 4.4.2 Object statement composition

Object-statement-scopes provide a limited form of scope at meta-compile time for functions and variables. Composition of declarations proceeds in the same way as composition for equivalent concepts in classes.

The presence of derivation rules supports multiple bodies in a potential declaration. Composition of these bodies is deferred until the compile compilation stage at which point only those bodies that are enabled by derivation predicates are retained.

### 4.4.3 Enum composition

```
In C++, an enum-speciﬁer comprises a list of enumerator-deﬁnitions each of which is a
name-value pair. The value part may be omitted, in which case it assumes the
value zero for the ﬁrst enumerator, or the preceding value plus 1 for subsequent
values.
enum Enums
{
ZERO,
// Implicitly 0
TWO = 2,
// Explicitly 2
THREE
// Implicitly TWO + 1
};
In FOG, composition occurs as one or more potential declarations comprising
meta-objects of
enum_specifier meta-type and lists of meta-objects of
enumerator_definition meta-type are converted to an actual declaration of
enum meta-type and an actual list of enumerator meta-type.
As each additional potential enum-speciﬁer is composed, each additional potential
enumerator-deﬁnition is appended to the list of actual enumerators. The missing
enumerator value is resolved as zero for the ﬁrst enumerator or one plus the value
of the most recent addition to the list for all subsequent values.
enum Enums
// Composing with above
{
FOUR,
// Implicitly THREE + 1
FIVE,
// Implicitly FOUR + 1
TWO = 2,
// Explicitly 2
THREE,
// Implicitly TWO + 1
ONE = TWO - 1
// Explicitly TWO - 1
};
```

Enumerator names may be repeated provided the value associated with the enumerator is the same for each repetition. Repeated names are discarded so that a meta-program traversal of the enumerators and the ﬁnal code emission iterates over the domain

```
{ ZERO = 0, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, ONE = 1 }
```

The “same value” involves a direct comparison of numeric values, whose evaluation may use the already resolved enumerators. There is no support for deferred evaluation and comparison of Abstract Syntax Trees.

### 4.4.4 Construct composition

Composition of constructs ﬁrst identiﬁes the construct (function, typedef or variable) to be composed using its unique signature, and then composes the remaining parts of the construct.

The unique signature of a typedef or variable involves the scope, template arguments and the name of the typedef or variable.

```
static const int X<A>::v = 5;
// Signature ::X<A>::v
typedef int (*PFunc)() const;
// Signature ::PFunc
The unique signature of a function additionally involves the function arguments
and those cv-qualiﬁers that resolve overloads.
virtual inline int f(const size_t& p = 5, const int q) volatile
// Signature ::f(const unsigned int &, int) volatile
The unique signature does not include other decl-speciﬁers such as static,
virtual or inline, the type-speciﬁer-seq, or function parameter names or default
function arguments4. Type names using typedefs are resolved. Redundant cv-
qualiﬁers for by-value arguments are ignored (§13.1-3).
The type-speciﬁer-seq, e.g. function return type, is not part of the unique signature,
since overloading of functions (or variables or typedefs) on the type is not
4. Exclusion of static from the signature is possible since the standard speciﬁcally
excludes overloading static and non-static member functions (§13.1-2).
supported. When multiple contributions are composed, each type-speciﬁer-seq must
refer to the same type to avoid an error.
The friend, typedef and using
decl-speciﬁers are not composed. friend and
typedef distinguish between different categories of construct. They must be
present for each declaration of a name.
using, in the context of the re-using-declaration described in Section 3.1.4.4,
indicates that only part of the declaration is provided, and that the remainder is
obtained from the re-used declarations. Composition of using in the context of a
using-directive has been described in Section 4.4.1.3.
The private, protected and public access-speciﬁers are composed using a four-
valued algorithm. Matching values compose to preserve the value. An omitted
value composes with an explicit value to preserve the explicit value. Conﬂicting
explicit values are an error.
```

A missing value for a new declaration speciﬁed within class braces automatically acquires the prevailing access for declarations in its scope. Repeated declarations within class braces, or any declaration outside class braces retain unspeciﬁed access until composed with a declaration that has a deﬁned access.

```
It is an error for a declaration forming part of a class to have no access-speciﬁer from
any source.
access-speciﬁers for declarations forming part of a namespace-body or linkage-speciﬁcation
are discarded.
The static and !static decl-speciﬁers are composed using a three-valued
algorithm. Matching values compose to preserve the value. An omitted value
composes with an explicit value to preserve the explicit value. Conﬂicting explicit
values are an error.
The
explicit,
export,
extern,
mutable and
register decl-speciﬁers are
composed using a two-valued algorithm. Matching values compose to preserve
the value. An omitted value composes with an explicit value to preserve the
explicit value. There are no conﬂicting values to cause an error.
```

### 4.4.5 Value composition

Values may need to be composed as initializers for variables, dimensions of arrays, or default initializers for function arguments. Successful composition veriﬁes that all alternative contributions have the same or no value, with the speciﬁc value being used rather than the no value. In the case of trees of values this policy is applied recursively to build a composite tree, by overlaying the lists at each level to form the longer list with consistent values. At any stage composition of a value with a different value or list of values is an error.

Conﬂict is determined by a syntax tree comparison after any tree-literals and constant values have been resolved. No symbolic interpretation of the syntax tree is performed, and so a redundant pair of parentheses or the interchange of binary operands is sufﬁcient to cause a composition failure.

### 4.4.6 Variable composition

```
Declarations of (non-array) variables are composed by composing the residual
decl-speciﬁers as outlined above in Section 4.4.4, and initializers as described in
Section 4.4.5. Any conﬂict is an error.
```

### 4.4.7 Array composition

```
Declarations of array variables are composed by composing the residual
decl-speciﬁers as outlined above in Section 4.4.4, and dimensions as described in
Section 4.4.5. Composition of array initializers is performed by ﬁlling up a multi-
dimensional array from [0][0]..., in the same way that a normal C++ initializer
deﬁnes its initializers. Each composition continues where the previous one left off;
it does not restart from [0][0]....
```

The gcc [Stallman98] indexed initializer syntax (Section 3.1.4.2) may be used to deﬁne a speciﬁc placement for an initialization value. As a result that it, and subsequent values in adjacent locations, may provide multiple initializers for the same array index. These are composed provided the values are consistent as described in Section 4.4.5. Missing initializers are given a zero value when the equivalent C++ declaration is emitted for the composite initializer.

The default incremental composition is most useful within the idiomatic loop

```
class X
{
public static const char *variableNames[];
auto for (iterator p = $variables(); p; ++p)
using variableNames = { ""$p->name() };
using variableNames = { 0 };
};
The ﬁrst line fully declares an array variable but without a dimension or any
initializers. The loop iterates over the member variables of class X. The body of
the loop contains a re-using-declaration to re-use the array declaration with a single
initializer comprising the concatenation of the empty string and the member
variable name. Each iteration composes the additional initializer with the existing
declaration, so that the array gradually builds up a list of strings. Finally the last
line adds a null terminator to the list.
```

Re-use of the declaration avoids a potentially conﬂicting redeclaration, but is not readily recognisable to C++ programmers. The loop can be speciﬁed more explicitly, saving a line:

```
auto for (iterator p = $variables(); p; ++p)
public static const char *variableNames[] = { ""$p->name() };
public static const char *variableNames[] = { 0 };
```

Indexed initializers are useful for applications such as automatically creating an array of debug text strings from an enumerator.

```
auto declaration EnumTextArray(name textsName, enum enumDecl)
{
auto for (iterator p = $enumDecl.enumerators(); p; ++p)
using $textsName = { [$p->value()] ""$p->name() };
}
class EnumClass
{
public enum Enums { A, E = 4, F, B = 1 };
};
static const char *enum_names[];
$EnumTextArray(enum_names, EnumClass::Enums);
```

is equivalent to and eventually emitted as

```
class EnumClass
{
public:
enum Enums { A, E = 4, F, B = 1 };
};
static const char *enum_names[] = { "A", "B", 0, 0, "E", "F" };
```

### 4.4.8 Function composition

```
Function composition involves composition of residual decl-speciﬁers, parameter
names, default arguments and function bodies. Composition of non-function-speciﬁer
decl-speciﬁers proceeds as above. The composition policies speciﬁc to functions
are:
The
inline,
!inline,
inline/interface
or
inline/implementation
decl-speciﬁers are composed using a ﬁve-valued algorithm. Matching values
compose to preserve the value. An omitted value composes with an explicit value
to preserve the explicit value. Conﬂicting explicit values are an error, except that
composition of plain inline with the more speciﬁc inline/interface or
inline/implementation composes to preserve the more speciﬁc value.
The virtual, !virtual and virtual/pure decl-speciﬁers are composed using a
four-valued algorithm. Matching values compose to preserve the value. An omitted
value composes with an explicit value to preserve the explicit value. Conﬂicting
explicit values are an error. For the above algorithm virtual/pure may be
explicitly speciﬁed as virtual/pure or by use of virtual and a subsequent = 0
pure-speciﬁer. The existence of a corresponding virtual function in a base class is
ignored during composition, however during the compilation phase the virtual
attribute is propagated to derived functions, resulting in an error message for a
conﬂicting requirement for a !virtual function derived from a virtual function,
and a warning message for a virtual function derived from a non-virtual
function.
```

Function parameter types do not compose, since distinct types represent distinct overloaded functions.

Function parameter names compose using a two-valued algorithm. Matching names compose to preserve the name. An omitted name composes with an explicit name to preserve the explicit name. Conﬂicting names are currently an error. This is a necessary constraint imposed by the potential for re-use of parameter names by multiple function bodies or derived functions. A less restrictive implementation should tolerate a local respeciﬁcation of parameter names in function bodies and automatic renaming to those in the ﬁrst declaration.

Default function arguments compose as described in Section 4.4.5.

Constructor initializers compose by gathering all initializers together. Multiple initializations for the same member variable compose as described in Section 4.4.5, however since the initializers form part of the function body, they are guarded by derivation predicates. Disabled initializations are ignored. When the constructor is emitted, the constructor initializers are emitted in constructor initialization order using explicit values, where available, with implicit values from member variable initializers as defaults.

**Exceptions**

```
Composition of exception-speciﬁcations and function-try-blocks goes well beyond the
considered policies for FOG, and so is an area for further work. It would appear
that exception-speciﬁcations for a particular function should just be gathered together
with duplicates discarded. It seems that exception-speciﬁcations should propagate up
the inheritance hierarchy to extend non-empty exception-speciﬁcations, so that the
exception-speciﬁcation of a derived virtual function should never be wider than its
inherited exception-speciﬁcation. This may lead to errors when inheriting from library
classes, whose speciﬁcations cannot be changed, but these errors diagnose a
problem rather than imposing a restriction. function-try-blocks should probably just
be concatenated on a per-handled type basis, with handlers organised to ensure
handlers for more derived types precede those for less derived types. It may be
necessary to extend the syntax for a default member-variable initializer to wrap a
try-block around it.
```

**Body**

The overall function body is formed from the concatenation of contributions to ﬁve named segments within which contributions are independently composed.

```
return-type function-name(function-arguments) cv-qualifiers
exception-specification
{
entry-segment-contribution
pre-segment-contribution
body-segment-contribution
post-segment-contribution
exit-segment-contribution
}
Contributions to each segment are concatenated and by default, contributions are
made to the body segment, so that
void f() { i++; }
void f() { i--; }
```

composes to

```
void f()
{
i++;
i--;
}
More explicit control of contributions requires the use of an object-statements-clause
in which the extra annotation syntax does not cause conﬂicts. The entry and exit
segments are useful for establishing a function framework:
class Manager
{
public Manager() {}
public bool do_it()
:{
entry { bool exitStatus = true; };
exit { return exitStatus; };
};
};
```

Other sources modules may contribute code. An independent module could specify:

```
class Manager
{
private MyContext& _context = MyContext::make(*this);
using do_it { if (!_context.do_it()) exitStatus = false; }
};
Composition of the framework established by entry and exit segments together
with the extra body results in the composed function:
bool Manager::do_it()
{
bool exitStatus = true;
if (!_context.do_it())
exitStatus = false;
return exitStatus;
}
```

The framework can be extended by any number of independent contributions. The safety of such extensions is dependent upon their orthogonality. If the contributions interact, then the meta-compilation source ﬁles must be carefully sequenced to ensure the intended result.

```
As well as extending the do_it function, an additional member variable was
speciﬁed with a default initializer. The composed class therefore looks like:
class Manager
{
private:
MyContext& _context;
public:
Manager();
bool do_it();
};
```

The default initializer is automatically supplied for all non-copy constructors:

```
Manager::Manager()
:
_context(MyContext::make(*this))
{}
The ﬁve segments provide sufﬁcient ﬂexibility for many applications, but are not
adequate for all. For instance, wrapping an if around a function body:
void f()
:{
pre { if(...) { }
// Not valid in FOG
post { } }
// Not valid in FOG
};
requires partial syntax that violates the syntactical requirement for statements.
There is also a parsing ambiguity between { and } as partial syntax and as syntax
structuring. (An earlier version of FOG used \{ and \} for partial syntax and
allowed any list of tokens as a composed contribution.)
```

The structure of a function body is deﬁned with two levels of indirection through meta-variables:

```
auto statement function::value[] = { @function_structure };
auto static const statement token::function_structure[] =
{ @entry; @pre; @body; @post; @exit; };
auto statement function::entry[] = {};
auto statement function::pre[] = {};
auto statement function::body[] = {};
auto statement function::post[] = {};
auto statement function::exit[] = {};
```

The segment-name/body syntax

```
derived(is_root()) entry { bool exitStatus = false; };
```

is therefore syntactic sugar for:

```
derived(is_root()) auto entry += { bool exitStatus = false; };
```

in which the internal meta-expression-statement of the form

```
auto list-name += tree-statement
is used to append the requisite code to the list using the built-in operator+= which
is valid for lists.
An if may therefore be wrapped around the existing body without violating syntax
constraints by:
void f()
:{
auto body = { if (...) @body; };
};
Braces are optional around @body since it is treated as a single statement. The
use of @ rather than $ is very important to defer evaluation until the compilation
phase. A $ would be evaluated when the potential declaration is associated with
an actual function, allowing further potential declarations or meta-programs to
extend the body following, rather than within, the if.
The
global
structure
of
functions
cannot
be
changed
by
redeﬁning
token::function_structure, since this is a const meta-variable.
The structure of an individual function can be changed by deﬁning
function_structure within an object-statements-clause. Code segments may be
added and removed, but the syntactic sugar for the ﬁve built-in segments is
unaffected; user-deﬁned segments can only be extended using an expression
operator such as +=.
Statements can be appended to a segment using += or replaced by just using =.
as in the examples above.
Declaration of a meta-variable as const precludes multiple assignments, and so
may be used to provide some protection against unforeseen activities by
interfering meta-programs. Thus
auto const statement body[] = {};
```

(re)deﬁnes the body segment to be empty, detects an error if the existing content is anything other than the built-in empty default, and detects an error when any subsequent attempt is made to change the content.

```
As semantic analysis and meta-programming proceeds, the meta-objects for each
function build a list of object-statements with their associated derivation
predicates. @-expressions in these object-statements remain unevaluated.
```

During the compilation stage, classes are compiled in least derived ﬁrst order, and individual members of each class are compiled in an unspeciﬁed order. The list of object-statements is scanned in the order in which potential declarations contributed to actual declarations, with the further inherited lists of objectstatements applied in destruction order: a base-class can therefore wrap code around its derived implementations predictably.

**Historical Note**

The multi-pass implementation of FOG was able to, but did not, parse function bodies which were therefore treated as an arbitrary list of brace-delimited tokens. Code composition was supported by arbitrary concatenation of such lists giving total ﬂexibility and anarchy since there was no syntactical constraint upon the composition, and no discovery of error until a subsequent compilation failed.

The superset implementation parses function bodies and imposes syntactical consistency for each composition. These constraints provide much needed integrity and respond to some of the hazards outlined by [Ossher98] and impose some of the discipline discussed by [Mulet95].

### 4.4.9 Meta-variable composition

```
Meta-variables are working variables for use at meta-compile-time. Any
redeclaration of a meta-variable must therefore have the same meta-type and
current value, which in the case of a const meta-variable must be the initial value.
Redeclaration of a const meta-variable with the same value is permitted, although
reassignment of a const meta-variable with the same value is an error.
```

### 4.4.10 Meta-function composition

```
Meta-functions compose in the same way as functions. By default, contributions
are gathered into the body list of declarations. When a meta-function is invoked,
the body returned for incorporation in the invoking context comprises the
hierarchical composition of segment contributions with the formal parameters
replaced by actual arguments.
Declaration of a const meta-function can be used to inhibit a redeﬁnition:
auto const declaration f()
:{
auto const statement body[] = { /* ... */ };
};
```

## 4.5 Meta-classes

Conventional programming involves programs that operate on application entities. Meta-programming involves programs that operate on program entities: the class, function and variable declarations that deﬁne a program.

In the same way that newcomers to Object-Orientation are easily confused by loose usage of the terms class, instance and object, newcomers to metaprogramming are easily confused by loose usage of the term meta-class. One confusion arises because, in Object-Oriented Programming, the phrase is-a denotes an inheritance relationship, but in meta-programming is-a can alternatively be used to denote an instantiation relationship. Further confusion arises because the object models available to the programmer do not correspond to the underlying abstraction.

The exact one to one relationship between classes and meta-classes in FOG is easy for the programmer to appreciate and natural to use, but apparently in conﬂict with some of the more traditional perspectives of meta-classes and metameta-classes. We will therefore describe the very pure object model exempliﬁed by ObjVlisp [Cointe87], before describing the C++ model and the enhancements provided by FOG in order to justify the FOG model.

A very simple three-class hierarchy is shown in the central column of Figure 4.9 in which class X inherits from class Y, and class Y inherits from class Z. Three Instances of each class are shown stacked to its right with the top instance named respectively anX, aY and aZ.

An Object Oriented program performs computation as a result of the interaction of its object instances at run-time, and in the simplest object model, instances have meaning only as object instances at run-time; classes exist solely as an abstraction at compile time.

A more sophisticated object model enables the run-time objects to make use of class information, and in a pure object model, this information is provided by a runtime object for each class. Each such class object provides a description of the instances of its class.

Every object must be an instance of some class, so it is necessary to deﬁne metaclasses that are instantiated as the class objects. The meta-classes are labelled

Instances Classes

aZ

aY

anX

Z

Y

_X X

_Y

_Z

inherits from described by

Meta-Classes

**Figure 4.9 Pure OO object model**

_X, _Y and _Z in the ﬁgure. (If the same description applies to each of X, Y and Z, then _Z alone is sufﬁcient to describe all classes.)

The pure object model requires a run-time object to describe each of these metaclasses, and so there are corresponding meta-class objects to describe each class. A potentially inﬁnite recursion is avoided by ensuring that the instance of _Z is also a valid description of _Z. Every box in Figure 4.9 corresponds to a runtime object; vertical arrows denote an inheritance relationship; horizontal arrows denote an instance-of or described-by relationship from right to left or a describes relationship from left to right.

The pure object model just described is that of ObjVlisp [Cointe87]. Few other languages comply to it. Smalltalk provides a similar but more restrictive model; the meta-classes of meta-classes are not visible [Briot89]. CLOS and SOM provide considerably more generality. The inheritance relationships of meta-classes and classes need not correspond. This leads to a number of signiﬁcant compatibility issues [Graube89], whose resolution seems to create further problems [Danforth94], [Forman94].

Meta-classes were originally introduced for languages such as CLOS to assist in the construction of instances whose layout was entirely deﬁned at run-time. More efﬁcient languages such as C++ or Eiffel deﬁne object layouts at compile-time and compiler writers have no need to provide meta-classes. The available facilities in C++ are limited.

The C++ components corresponding to Figure 4.9 are shown in Figure 4.10. The instances in the right hand column comprise

- a contiguous piece of memory for the member variables

```
•
a hidden pointer to the instance description often called vptr,
which has been arbitrarily labelled -rtti-
Each class (and meta-class) ‘object’ comprises a potentially contiguous area of
memory containing the compiler generated run-time type information as an
instance of std::type_info, and generally discontiguous areas of memory for
the static member variables and functions.
Most of the behaviour of std::type_info is implementation deﬁned, but it will
typically comprise
•
a pointer to the class description (-rtti-)
•
a list of base-class instance descriptions (-bases-)
```

- a class name

```
•
a dispatch table for virtual functions (-vtable-).
Very limited functionality is exposed for the std::type_info class, whose sole
instance both describes and is an instance of the std::type_info class. This is
the only meta-class in ISO C++, although a more substantive facility was proposed
by [Buschmann92] during the standardisation process.
The class object of any suitable type T is returned by the typeid(T) operator. The
solitary
meta-class
object
is
returned
by
typeid(typeid(T))
or
just
typeid(std::type_info).
```

**Class variables**

The symmetry of Figure 4.9 and the presentation in Figure 4.10 implies that the class variables (static member variables) form a logical part of their corresponding class object. This follows postulate 6 of [Cointe87] that the class variables of an instance are the instance variables of its meta-class. However, in common with many programming languages, C++ does not require the programmer to distinguish between use of class variables and instance variables. This programming convenience leads to some confusion between class and instance

member

-rtti-

variables

static

member

functions

static

member

variables

-rtti- -bases-

-vtable-

“X”

run time type

info

member

functions

static

member

functions

static

member

variables

-rtti- -bases-

-vtable-

“X”

run time type

info

member

functions

static

member

functions

static

member

variables

-rtti- -bases-

-vtable-

“X”

run time type

info

member

functions

static

member

variables

static

member

functions

-rtti- -bases-

-vtable- “typeinfo”

run time type

info

member

-rtti-

variables virtual function

inherits from

described by

**Figure 4.10 C++ object model (Memory Perspective)**

member

-rtti-

variables

meanings, and undermines the pure perspective of the meta-class structure. The class variables are logically shared parts of each instance. The alternative detailed presentation of Figure 4.11 and its simpliﬁed presentation in Figure 4.12

is more appropriate. All visible names now appear in one rather than two columns.

At the source level, a member of a class or instance is accessed by name. The access is resolved by consulting the instance description5 to convert the name into the address of a variable or function. In C++, this conversion process is almost

static member variables

member

member functions

-rtti-

variables

-rtti- -bases-

-vtable-

“Z”

run time type

info

static member functions

static member variables

member

member functions

-rtti-

variables

-rtti- -bases-

-vtable-

“Y”

run time type

info

static member functions

**Figure 4.11 C++ object model (Naming Perspective)**

static member variables

member

member functions

-rtti-

variables

virtual function

inherits from

described by

-rtti- -bases-

-vtable-

“X”

run time type

info

static member functions

static member variables

-rtti- -bases-

-vtable- “typeinfo”

run time type

info

static member functions

Instance (Shared part)

Instance Description

Instance (Unique part)

```
entirely performed at compile-time; those few conversions that cannot be
completely resolved at compile-time are partially resolved as ﬁxed indexes that
index the -vtable- at run-time. The -vtable- is all that remains of the more
general name to address conversion table required by less compiled languages.
```

Seen from this perspective, class variables and instance variables differ in their access policy, but share the same name-space. Each class variable exists with a 1:1 relationship to a class, and is accessible by name from that class, and derived classes. However, there is no reason for class variables to be grouped as an object, and in C++ they are not. It is in fact impossible to group class variables as a contiguous object, since shared class variables cannot be adjacent for more than two distinct derived classes.

When class variables are accessible with respect to instances, there is no reason for them to be accessible with respect to their classes as well, and again in C++ they are not. However, the symmetrical interpretation, in which class variables are the instance variables of their meta-classes, requires that they are accessible with respect to instances and classes and so blurs the distinction between meta-levels [Maes87].

Therefore names of class variables (and functions) are visible in the instance description (the conventional name-space for access with respect to instances), but are omitted from the class description (the meta-namespace for access relative to instance meta-objects (classes)).

**Compile-time object model**

The run-time object model of Figure 4.11 is redrawn without the clutter and without inheritance in Figure 4.12.

```
Programming normally operates using the multiple instance objects of each class
shown in the right-hand column. Static and non-static member functions and
variables share the same name-space. The hidden -rtti- pointer identiﬁes the
run-time type information description of the instance. Invocation of typeid()
```

5. An instance description is a description of the names visible in an instance (on Figure 4.11 and Figure 4.12. A “class description” misleadingly refers to the wrong meta-level.

run-time meta-meta-level

instance description

instance meta-object (class object)

-rtti-

member variables

-rtti-

instance object

static member functions

static member variables

member functions

class description

instance meta-meta-object (meta-class object)

-rtti-

static member functions

static member variables

member functions

meta-level meta-meta-meta-level

**Figure 4.12 C++ Run-time object model**

```
shifts the programming perspective to the meta-level, where only the members of
the typeinfo instance describing the class are visible. A further invocation of
typeid() shifts to the meta-meta-level and since typeinfo is its own meta-class,
only its members remain visible.
```

The corresponding compile-time programming model for FOG is shown in Figure 4.13.

A meta-level instance at compile-time (Figure 4.13) has instance variables and functions, as well as class variables and functions in just the same way as an instance at run-time (Figure 4.12) has instance (member) variables and functions and class (static member) variables and functions. The class meta-object members are referred to as (non-static) meta-members and static meta-members by direct analogy with (non-static) members and static members.

Each class declaration forms part of a class meta-object that also comprises meta-variables and a description of the run-time instance objects. The very righthand ‘level’ corresponds to the normal run-time perspective, in which a small amount of the instance description maintained by the class meta-object is provided as the run-time-type-information of the class object. This level does not exist at compile-time.

```
Meta-programming occurs at the meta-level where the description of objects
(member names and types) is available as well as the meta-members of the meta-
class. The -ctti- counterpart of -rtti- identiﬁes the describing meta-object.
The class description describing an instance meta-object contains the full
mapping from names to built-in and user-deﬁned meta-functions and meta-
variables, rather than the highly optimised -vtable- in the run-time type
information.
Instance meta-objects are instances of class or class_specifier meta-types.
Class and meta-class meta-objects are instances of the meta_type meta-type.
In the same way that typeid() shifts the perspective to the left in the run-time
diagram, the meta_type() built-in member function shifts the compile-time
```

meta-meta-meta-level meta-meta-level meta-level

**Figure 4.13 FOG Compile-time object model**

instance meta-object

instance meta-meta-object (class meta-object)

-ctti- (class) description

meta-member variables

-ctti-

instance description

instance meta-object (class object)

instance meta-meta-object

instance meta-meta-meta-object (meta-class meta-object)

-ctti- (meta-class) description

static meta-member functions

static meta-member variables

meta-member functions

static meta-member functions

static meta-member variables

meta-member functions

run-time

```
perspective to the left. Whereas the typeid of a run-time class is type_info, the
meta-type of a compile-time meta-object is meta_type.
```

**The meta-class**

The compile-time representation of a class comprises two meta-objects, one class meta-object to describe the class, and one instance meta-object to describe runtime instances. Since these two meta-objects have matching inheritance and always exist as a pair, it is convenient to regard the pair of meta-objects as a single object, which may be safely but loosely referred to as the meta-class.

**Other meta-types**

Figure 4.13 is drawn for the meta-objects describing classes and their instances. The same diagram applies for all meta-objects, replacing ‘instance’ by the metaobject category. However, only variables, functions, meta-variables and metafunctions have their own user-deﬁned meta-functions and meta-variables.

```
A function meta-object, of meta-type function or function_specifier, contains
a function description (the parameters). The corresponding function meta-meta-
object contains the mapping of all meta-names applicable to the function including
any meta-variables deﬁned within an object-statements-clause. The function body is
indirectly deﬁned via the value meta-variable.
Similarly, but taking care to use distinct terminology, a meta-function meta-object,
of meta-type meta_function or meta_function_specifier, contains a meta-
function description (the parameters). The corresponding meta-function meta-
meta-object contains the mapping of all meta-names applicable to the meta-
function including any meta-variables deﬁned within an object-statement-clause. The
meta-function body is indirectly deﬁned via the value meta-variable.
```

The distinction between meta-functions and function meta-objects is shown in Figure 4.8 on page 104.

**Meta-inheritance**

```
C++ supports instance objects and, to a limited extent, class objects at run-time.
FOG extends C++ to support class objects and, to a limited extent, meta-class
objects at compile time. FOG provides a meta-class for every class and built-in
type. The inheritance of meta-classes mirrors that of the class hierarchy, so that
the base meta-classes of every meta-class are the meta-classes of the base
classes of the corresponding class. Every meta-class without any other base
meta-classes automatically inherits (virtually) from the built-in meta-class ::auto.
Additional meta-inheritance may be speciﬁed by using the auto keyword as an
access-speciﬁer. The inheritance and meta-inheritance for
class Base {};
class Derived : public Base, auto char {};
is shown in Figure 4.14. With the exception of meta-inheritance from ::auto
which is always virtual, meta-inheritance is deﬁned by the virtual keyword. A
virtual meta-class appearing more than once in the meta-inheritance hierarchy
only contributes one set of meta-variables.
```

## 4.6 Meta-programming

The description of compilation stages in Section 4.1 identiﬁed the different contexts in which meta-programming can occur.

A parasitic form of meta-programming occurs through augmented behaviour during semantic analysis and compilation as tree-literals are resolved. However, even allowing for the extra ﬂexibility provided by derivation rules, this behaviour constrains the actions of meta-programming to be closely correlated with the

corresponding timing of compiler activity. This is adequate for simple elaboration of declarations, but a poor foundation for a meta-program where the programmer, rather than the meta-compilation system should determine the sequencing.

FOG therefore provides additional compilation stages in which the programmer has greater control. The meta-construction provides an opportunity for algorithms that operate on meta-classes to be activated in a base-functionality ﬁrst order analogous to construction, with meta-destruction providing a similar opportunity in base-functionality last order analogous to destruction. In addition the metamain stage provides the programmer with no sequencing assistance or constraints.

**meta-construction**

During the meta-construction stage, the meta-constructor of each class (including built-in types) is invoked once in a least-derived ﬁrst order. Programs may supply code for execution during this stage by declaring a meta-constructor:

```
auto MyClass::MyClass()
{
/* meta-program */
}
This code is composed with any other deﬁnitions of the same meta-constructor.
Such composition includes inherited contributions, which are executed ﬁrst in
accordance with the normal principles of constructors. Since everything inherits
from the auto class, deﬁnition of a meta-constructor for auto provides a
mechanism for executing a meta-program in all classes.
```

**meta-main**

The meta-main stage consists of execution of all deﬁnitions of:

```
auto void main()
{
/* meta-program */
}
```

in the order in which the declarations are encountered.

**meta-destruction**

The meta-destruction stage mirrors the meta-construction stage. The metadestructor for each class is executed once, again in least-derived ﬁrst order, but

**Figure 4.14 Inheritance and Meta-inheritance**

```
auto
Base
char
Derived
Base
char
Derived
```

meta-inheritance inheritance

with inherited contributions executed after local contributions in accordance with destructor principles.

**Integrity**

It has been recognised [Chiba96] that the operation of two apparently independent meta-programs on the same program can lead to poorly deﬁned behaviour. For instance, a problem can arise even with two very simple meta programs which

- add a diagnostic print-out to every function to create a call-trace

```
•
add a check_invariant() and invocation from each non-const function
Whether a diagnostic print-out is added to the check_invariant() routine
depends upon the application order. Whether such a print-out should be added is
a subtle user preference.
```

The distinction is fairly trivial in this example. The distinction is critical for applications that involve synchronization or persistence, since functions or variables added by meta-program A after the class structure has been analysed by meta-program B may not be subjected to a re-analysis by meta-program B.

The problem is largely ignored in practical reﬂective systems. It is assumed that the meta-programmer will coordinate multiple meta-programs. The theoretical problem is addressed by the reﬂective tower [Smith84], in which each level6 of reﬂection deﬁnes a new language for the level above that hides the language of the level below. The rather impractical need for a distinct representation of each object at each level is described by [Chiba96].

The three stages offered by FOG perhaps represent a pragmatic compromise and symmetry with run-time concepts of static-construction, main-program and staticdestruction. During the meta-construction stage, actual declarations are in a highly unstable state, since further meta-programming may provide additional declarations. It is therefore unwise to place any code in a meta-constructor that browses child declarations. Meta-construction code should consist only of deﬁnitions. Browsing meta-programming should be implemented in metadestructors, and meta-programs should avoid creating new declarations during meta-destruction in order to support consistent behaviour by other metadestructors. Meta-destructors should only elaborate and compose with existing declarations. The meta-main stage is not strictly necessary, however it avoids the need for relatively arbitrary meta-programs to be constrained by the invocation mechanisms of meta-construction or meta-destruction.

The non-trivial examples in Chapter 7 make extensive use of the metaconstruction and meta-destruction phases to realise each example. It would appear that the two traversals of the tree of program declarations are insufﬁcient to support multiple meta-programs.

It is essential for all of one meta-program to execute before any of the next, since use of just the meta-construction and meta-destruction phases requires interleaved meta-program execution as the inheritance hierarchy is descended.

```
In principle, the problem of many meta-programs can be directly resolved by
multiple meta-main programs, each of which perform a hierarchical traversal
starting with an iteration over ::all_classes(). However this requires the
source declarations to be presented in the correct order. A slightly less direct
approach could support registration of activities as one or more lists of meta-
functions during meta-construction. These lists could then be serviced by meta-
```

6. A level of reﬂection, counting the number of layers of language elaboration provided by meta-programming, should not be confused with a meta-level, counting the number of levels of description for instance, meta-instance/class, meta-meta-instance/meta-class, ...

main. This indirection provides some opportunity for programmed prioritising, rather than source ﬁle sequencing, to determine the behaviour.

Identifying a more direct mechanism for specifying multiple meta-programs and their sequencing dependencies is an area for further work. This could tie in to consideration of user-deﬁned meta-types, for which the current use of metaconstructors and meta-destructors for compilation stages rather than meta-type maintenance could be embarrassing.

## 4.7 Syntax macros

The superset parsing approach described in Chapter 5 isolates the syntactic and semantic analysis stages. This supports an implementation in which all syntactic processing completes before any semantic processing starts. However, enforcing this isolation prohibits any syntax dependency on semantics, and unfortunately prevents the deﬁnition of syntax macros, since resolving the deﬁnition of a syntax macro is a semantic activity.

```
It is important to distinguish the C++ syntax macro problem from the equivalent
Lisp problem. Lisp has a very disciplined lexical structure, in which the program
tree structure is represented by parentheses in the source. It is therefore easy for
a Lisp preprocessor to manipulate its syntax trees, since they can easily be
identiﬁed. There is no equivalent underlying lexical structure in C++, not even {}
are predictable,
namespace X {}
// } completes a {} construct
struct X {} a;
// ; completes a {} construct
if (a) {}
// } sometimes completes a {}
if (a) {} else if {b}; else; //
construct except when ...
do {} while();
// {} may be mid-construct
for ( ; ; ) {}
// ; may be in a strange place
```

Identifying a C++ construct cannot be reliably performed by a simple preprocessor, unless that preprocessor has such a substantial understanding of C++ syntax, and indeed semantics, that it cannot really be regarded as a simple preprocessor. Section 5.4 describes how early attempts to implement FOG in preprocessor style failed through lack of adequate language comprehension.

In Lisp, a syntax macro can be deﬁned and exploited by a preprocessor. In C++, deﬁnition and exploitation of the syntax macro must be integrated with the language.

A syntax macro supports (or rather gives the illusion of supporting) user-deﬁned language extensions7. Such an extension could in its general form support arbitrary additional syntax so that:

```
with counter from 1 to 100 step 5 in { sum += $counter; }
```

could be recognised as an alternate way of writing

```
for (int counter = 1; counter < 100; counter += 5)
{ sum += $counter; }
More practically syntax macros should support what appear to be extra
decl-speciﬁers
synchronized class MyClass
{
persistent int _count;
};
```

so that programmers appear to use an extended language, although the synchronized and persistent extensions are realised by meta-programming in

7. The extensions are shown by italicizing the type-writer font.

a standard language. The example is clearly more readable than the functionally equivalent:

```
$synchronized(class MyClass
{
$persistent(int _count);
};)
```

In both cases application meta-functions are invoked to support the concepts of multi-process synchronisation or data-base persistence. The syntax macro approach has the advantage of offering a much more acceptable programming interface.

```
Implementation of a syntax macro requires the trigger word (persistent) to be
recognised in order to perform the appropriate syntactical parse.
```

Recognition of the trigger word in an unconstrained context requires the trigger word to become a new reserved word, introducing the problems of conﬂicting usage and unwanted replacement associated with the C preprocessor.

Alternatively, recognition of the trigger word within a restricted syntactical context imposes the implementation problem of executing yacc at compile-time to generate an updated syntax analyser, and the practical problem of enabling the application programmer to understand the shift-reduce conﬂicts associated with a proposed macro. Resolution of these conﬂicts is unlikely to be portable. The complexity of generating and diagnosing an updated analyser at compile-time are inappropriate for a language with as difﬁcult a syntax as C++.

```
A potential solution to the problem of conﬂicting name capture lies in the use of
the C++ name hierarchy. A syntax macro could be deﬁned within a namespace or
class and would only be a reserved word within that namespace or class.
However this approach has two problems:
```

A reserved word does not necessarily occur lexically within its class:

```
persistent int MyClass::_status;
```

would have to be written:

```
class MyClass
{
persistent int _status;
};
```

This is inelegant but could perhaps be tolerated.

The class may be indeterminate:

```
$do_something(class MyClass
{
persistent int _status;
};
);)
Since the context of MyClass is undetermined, it cannot be known whether
persistent is a scope-dependent reserved word and so syntactic analysis is not
possible.
```

This is also inelegant and could perhaps also be tolerated, but the ﬁrst example indicates that scope-speciﬁc syntax macros do not extend C++ comfortably. The second example shows an incompatibility with other FOG concepts.

A syntax macro must therefore be scope-independent, and so have the same status as any other reserved word. Syntax macros should be restricted to applications where the beneﬁt of the cleaner invocation far outweighs the hazards of the introduction of a global name.

Deﬁnition of a syntax macro should integrate with the rest of the language, and necessarily occurs at global scope. However, usage of a syntax macro requires

```
semantic analysis of its deﬁnition to have been completed. Therefore preserving
the implementation option of completing syntactical analysis before starting
semantic analysis requires semantic analysis of syntax-macro-deﬁnitions to occur
during syntax analysis. Syntax macro deﬁnitions therefore have a distinctive
syntax to facilitate this special treatment.
```

Although deﬁnition of a syntax macro can be regarded as a preprocessing activity, its exploitation occurs in conjunction with the subsequent compilation activities.

```
syntax-macro-deﬁnition:
explicit auto meta-type identiﬁer ( syntax-macro-parameter-listopt ) exposed-treeopt
compound-tree-statement
syntax-macro-parameter-list:
syntax-macro-parameter
syntax-macro-parameter-list , syntax-macro-parameter
syntax-macro-parameter:
meta-type identiﬁer exposed-treeopt
identiﬁer
reserved-word
punctuation
The further overloading of explicit and auto is unpleasant and only slightly
mnemonic. Introducing a new reserved word such as syntax would be better.
Declaration of a syntax-macro declares the identiﬁer to be a reserved trigger word
for a sentence that should satisfy the meta-type syntax. The syntax to be accepted
by the parser comprises this trigger word followed by the sequence of syntax-macro-
parameters, which comprise expected syntax elements and further words or
punctuation which are temporarily reserved between recognition of the trigger
word and detection of the end of the syntax.
It is a slightly surprising but fortunate accident that this syntax supports
speciﬁcation of any combination of intervening punctuation including , or ). Thus
the syntax-macro to intercept and pack a fractional coordinate such as
pt(0.5, -0.7)
```

could be speciﬁed as

```
explicit auto expression pt( (
, assignment_expression8 x
, ,
, expression y
, )
)
{ (int(32768 * $x) << 16) | (int(32768 * $y) & 0xFFFF); }
```

using an overline to distinguish tokens deﬁning the structure of the deﬁnition from those parameterising the deﬁnition.

```
The above deﬁnition makes pt a reserved word throughout the rest of the
program. Once the reserved word is recognised, its literal arguments are given a
temporarily reserved status. These are the punctuation comma and ), following pt
(or from, to, step, in following with in the earlier example). The temporarily
reserved status ensures that separators are treated as separators, giving a well-
deﬁned, if not necessarily ﬂexible behaviour. Premature recognition of the comma
as a separator in
pt(pow(1,2), 0)
will give a potentially confusing error diagnostics from arithmetic on "pow(1", even
if no earlier confusion was caused by the trailing ", 0)". Syntax macros are
perhaps best restricted to single arguments that comply with a C++ construct,.
8. assignment_expression rather than expression is necessary to ensure that
pt(0,0) is not treated as pt((0,0) *missing*), since 0,0 is an expression.
```

Overloading is permitted, subject to the constraint that the set of temporarily reserved tokens is the union of all syntaxes triggered by the overloaded trigger word. In the following example with single arguments, there are no separator tokens, so there are no temporarily reserved tokens to cause confusion.

```
explicit auto variable_specifier persistent(variable_specifier v)
{
$v;
/* additional meta-programming using $v */
}
explicit auto class_specifier persistent(class_specifier c)
{
$c;
/* additional meta-programming using $c */
}
Syntax-macro parameters are identiﬁed one at a time. For each parameter, a one
token lookahead is used to see whether the explicit identiﬁer, reserved-word or
punctuation requirement of a parameter can be satisﬁed. If satisﬁed, the lookahead
is discarded, overload alternatives without explicit requirements are discarded
and the scan continues looking for the next parameter. When no explicit
requirement can be satisﬁed and a meta-typed parameter is required, a recursive
syntactical analysis is invoked to locate a generic syntax element. Overload
alternatives that the generic element satisﬁes are retained. Eventually:
```

- no alternatives remain: a syntax error has been detected

- one alternative remains and it requires no further parameters: the syntax-macro arguments have been successfully identiﬁed

- more than one alternative remains: an ambiguous invocation has been detected

The temporarily reserved words are restored to their previous status after processing of the syntax macro. This may still be a reserved status since syntax macros can be invoked recursively.

```
Care should be exercised in the use of { } and ; as speciﬁc punctuation since
FOG uses these for recovery from syntax errors, which may be hampered by
unconventional usage of these tokens.
A syntax-macro is functionally the same as a meta-function. It differs only in its
invocation mechanism. A meta-function invocation has a trigger operator ($ or @)
and an expression identifying the meta-function name followed by parenthesised
comma-separated generic syntax elements. A syntax macro has a trigger word
identifying the macro followed by a sequence of user-deﬁned punctuation,
(temporarily) reserved words and generic syntax elements.
The foregoing description has been implemented in so far as FOG is able to
perform the syntactic analysis of syntax macro deﬁnitions, including the pt
example above. Meta-functions have also been implemented, so there is a
relatively small gap to bridge to activate the alternative invocation mechanism,
particularly for the simple case of single argument macros. Resolution of
overloads and multiple separators is not particularly difﬁcult, however it is clear
that the lack of an underlying lexical C++ structure imposes severe limits upon the
useful complexity of multi-argument syntax macros. Implementing this and
assessing the utility is an area for further work.
```

## 4.8 Summary

We have described the FOG compilation activities and the transformation of source declarations to potential declarations, at which level meta-programming and composition can operate before conversion to the actual declarations that are

emitted as C++ declarations. We have shown how meta-types act as a syntax predicate and justify the polymorphic treatment of a single meta-type with a tree of the same meta-type, supporting meta-programming over useful program structures.

Alternative models for macro substitution have been examined and the choice of syntax level substitution in FOG justiﬁed. The rationale for treatment of a treeliteral as an identiﬁer has been given.

```
The C++ name-spaces have been described. The new meta-name-space has
been related to the run-time name-space, with usage within auto statements or
tree-expressions. A multi-$ invocation has been provided to resolve lexical
scoping problems and the FUNARG problem. The need for the built-in meta-
function parameters Dynamic, Static has also been given.
```

Composition rules have been provided to deﬁne the behaviour once the C++ One Deﬁnition Rule has been relaxed so that it applies only at the output from FOG. Some of the hazards of ill-disciplined function body composition have been addressed by the use of token lists.

The general concepts of meta-classes have been reviewed in order to establish the C++ perspective and show how FOG provides a consistent compile-time generalisation of meta-classes and other meta-types.

The need for ﬂexible meta-programming has been motivated and solutions provided by meta-construction, meta-main and meta-destruction compilation stages.

Finally, the difﬁculties of implementing syntax macros in C++ have been considered, and a proposal described that works reasonably, at least for single argument macros.
