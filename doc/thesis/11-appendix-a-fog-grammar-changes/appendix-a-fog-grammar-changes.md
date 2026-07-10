# A FOG Grammar changes

This summary of the FOG syntax follows the example of Appendix A of the [C++96] standard. It is intended to be an aid to comprehension, identifying all changes in a way that is easily compared to C++. Completely unchanged grammar productions are omitted, changed rules and terms are indicated by a strikethrough for removal and underline for addition.

## A.1 Keywords

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
punctuation:
one of
{
}
[
]
(
)
;
:
?
:: .
.*
+
-
*
/
%
^
&
|
~
!
=
<
>
+= -= *= /= %= ^= &= |= <<=>>=
<< >> == != <= >= && || ++ -- ,
-> ->* ...
reserved-words are not identiﬁers.
reserved-word:
one of
and
and_eq
asm
auto
bitand
bitor
bool
break
case
catch
char
class
compl
const
const_cast
continue
default
delete
do
double
dynamic_cast
else
enum
explicit
export
extern
false
float
for
friend
goto
if
inline
int
long
mutable
namespace
new
not
not_eq
operator
or
or_eq
private
protected
public
register
reinterpret_cast
return
short
signed
sizeof
static
static_cast
struct
switch
template
this
throw
true
try
typedef
typeid
typename
unsigned
using
virtual
void
volatile
wchar_t
while
xor
xor_eq
non-reserved-words are identiﬁers.
non-reserved-word:
one of
derived
emit
entry
exit
file
frozen
guard
implementation
include
interface
noguard
noimplementation
path
pool
post
pre
prefix
pure
suffix
utility
meta-type-names are reserved-words or identiﬁers.
meta-type-name:
intrinsic-meta-type-name
actual-meta-type-name
potential-meta-type-name
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
actual-meta-type-name:
one of
// 1
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
```

## A.2 Lexical conventions

### A.2.1 Phase 6 Concatenation Grammar

```
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
```

1. Some meta-type names are also reserved words. The usage as a meta-type name augments usage as a reserved word.

```
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
tree-literalpp:
at-literal
dollar-literal
syntax-macro-literal
// See Section 4.7
at-literal:
@ tree-expression
@ { tree-expression }
dollar-literal:
$ tree-expression
$ { tree-expression }
$ dollar-literal
```

### A.2.2 Phase 7 Tokenization Grammar

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
```

## A.3 Basic concepts

## A.4 Expressions

```
identiﬁer:
other-identiﬁer
meta-type-name
non-reserved-word
tree-literal
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
special-function-id:
~ id
conversion-function-id
operator-function-id
nested-special-function-id:
special-function-id
id :: nested-function-special-id
scoped-special-function-id:
::opt nested-special-function-id
primary-expression:
literal
this
:: identiﬁer
:: operator-function-id
:: qualiﬁed-id
( expression )
id-expression
declarator-id
```

## A.5 Statements

```
statement:
control-statement
expression-statement
compound-statement
declaration-statement
try-block
auto control-statement
auto meta-expression-statement
control-statement:
// 3
labeled-statement
selection-statement
iteration-statement
jump-statement
2. Resolution of the identiﬁer < context-dependency is discussed in Section 5.8.2.
```

3. New non-terminal but no changed functionality

## A.6 Declarations

```
declaration:
block-declaration
function-deﬁnition
template-declaration
explicit-instantiation
explicit-specialization
linkage-speciﬁcation
namespace-deﬁnition
namespace-declaration
accessibility-speciﬁer
compound-declaration
meta-control-declaration
auto meta-control-declaration
meta-expression-statement
auto meta-expression-statement
auto meta-class-declaration
auto meta-function-deﬁnition
auto meta-variable-declaration
syntax-macro-deﬁnition
include-declaration
ﬁle-dependency-declaration
ﬁle-placement-declaration
ﬁlespace-declaration
block-declaration:
simple-declaration
asm-deﬁnition
namespace-alias-deﬁnition
using-declaration
// 4
using-directive
compound-declaration:
{ declaration-seqopt }
simple-declaration:
decl-speciﬁer-seqopt init-declarator-listopt ;
decl-speciﬁer:
storage-class-speciﬁer
type-speciﬁer
function-speciﬁer
friend
typedef
access-speciﬁer
using
storage-class-speciﬁer:
auto
// 5
register
static
extern
mutable
static:
// 3
static
! static
function-speciﬁer:
inline
! inline
inline / implementation
inline / interface
virtual
! virtual
virtual / pure
explicit
4. using-declaration is generalised and covered by simple-declaration
5. Compatibility can be retained by allowing auto within a function-body.
type-speciﬁer:
simple-type-speciﬁer
class-speciﬁer
enum-speciﬁer
elaborated-type-speciﬁer
cv-qualiﬁer
simple-type-speciﬁer:
::opt nested-name-speciﬁeropt type-name scoped-id
built-in-type-speciﬁer
// 6
built-in-type-speciﬁer:
char
wchar_t
bool
short
int
long
signed
unsigned
float
double
void
elaborated-type-speciﬁer:
class-key ::opt nested-name-speciﬁeropt identiﬁer scoped-id
enum ::opt nested-name-speciﬁeropt identiﬁer scoped-id
typename ::opt nested-name-speciﬁeropt identiﬁer scoped-id
typename ::opt nested-name-speciﬁeropt identiﬁer < template-argument-list >
enum-speciﬁer:
enum identiﬁeropt scoped-idopt { enumerator-listopt }
namespace-declaration:
namespace scoped-id ;
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
using-declaration:
// 4
using typenameopt ::opt nested-name-speciﬁer unqualiﬁed-id ;
using :: unqualiﬁed-id ;
linkage-speciﬁcation:
extern string-literal { declaration-seqopt }
extern string-literal declaration
```

## A.7 Declarators

```
init-declarator-list:
init-declarator
init-declarator-list , init-declarator
```

6. The built-in types are split off to a distinct non-terminal.

```
init-declarator:
// 7
declarator pure-speciﬁeropt object-statements-clauseopt
declarator initializeropt object-statements-clauseopt
identiﬁeropt : constant-expression object-statements-clauseopt
cv-qualiﬁer:
const
volatile
declarator-id:
::opt id-expression
::opt nested-name-speciﬁeropt type-name
scoped-id
scoped-special-function-id
function-deﬁnition:
decl-speciﬁer-seqopt declarator pure-speciﬁeropt ctor-initializeropt function-body
decl-speciﬁer-seqopt declarator pure-speciﬁeropt function-try-block
initializer:
= initializer-clause
( expression-list )
initializer-clause:
assignment-expression
[ constant-expression ] assignment-expression
{ initializer-list ,opt }
{ }
```

## A.8 Classes

```
class-head:
class-key identiﬁeropt scoped-idopt base-clauseopt
class-key nested-name-speciﬁer identiﬁer base-clauseopt
class-speciﬁer:
class-head { member-speciﬁcationopt declaration-seqopt }
accessibility-speciﬁer:
access-speciﬁer :
member-speciﬁcation:
member-declaration member-speciﬁcationopt
access-speciﬁer : member-speciﬁcationopt
member-declaration:
decl-speciﬁer-seqopt member-declarator-listopt ;
function-deﬁnition ;opt
qualiﬁed-id ;
using-declaration
template-declaration
member-declarator-list:
member-declarator
member-declarator-list , member-declarator
member-declarator:
// 7
declarator pure-speciﬁeropt
declarator constant-initializeropt
identiﬁeropt : constant-expression
pure-speciﬁer:
= 0
constant-initializer:
= constant-expression
7. The distinction between init-declarator and member-declarator is removed. The ambigu-
ity of a bit-ﬁeld with a labeled-statement is resolved to the label.
```

## A.9 Derived Classes

```
base-speciﬁer:
::opt nested-name-speciﬁeropt class-name
virtual access-speciﬁeropt ::opt nested-name-speciﬁeropt class-name
access-speciﬁer virtualopt ::opt nested-name-speciﬁeropt class-name
scoped-id
built-in-type-id
virtual base-speciﬁer
// 8
! virtual base-speciﬁer
access-speciﬁer base-speciﬁer
// 8
auto base-speciﬁer
built-in-type-id:
built-in-type-speciﬁer
built-in-type-id built-in-type-speciﬁer
```

## A.10 Special member functions

```
mem-initializer-id:
::opt nested-name-speciﬁeropt class-name
identiﬁer
scoped-id
```

## A.11 Overloading

## A.12 Templates

```
template-declaration:
exportopt usingopt template < template-parameter-list > declaration
explicit-specialization:
usingopt template < > declaration
```

## A.13 Exception Handling

## A.14 Tree Literals

```
primary-tree-expression:
meta-scoped-id
( tree-expression )
postﬁx-tree-expression:
primary-tree-expression
postﬁx-tree-expression ( tree-argument-listopt )
postﬁx-tree-expression [ expression ]
postﬁx-tree-expression . scoped-id
postﬁx-tree-expression -> scoped-id
tree-expression:
postﬁx-tree-expression
* tree-expression
tree-argument-list:
tree-argument
// 9
tree-argument-list , tree-argument
tree-argument:
tree-statement
unterminated-tree-argument
tree-statement:
terminated-tree-argument
unterminated-tree-argumentopt ;
compound-tree-statement:
// 10
{ tree-statement-seqopt }
```

8. A more general rather than changed syntax.

```
9. Ambiguities are resolved semantically by left to right maximisation of the length of
each tree-argument with respect to its required syntax.
tree-statement-seq:
tree-statement
tree-statement-seq tree-statement
terminated-tree-argument:
// 11
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
// 11
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
// 12
simple-type-parameter
storage-class-speciﬁer
template-argument
template-parameter
type-parameter
auto meta-class-speciﬁer
```

## A.15 Object statements

```
object-statements-clause:
: { object-statement-seqopt }
10. An ambiguity arising from a try-block at the end of a statement followed by a handler-
seq is resolved by maximising the length of the left-hand (try-block) element.
```

11. The presentation of these productions has huge ambiguities. They demonstrate some of the variety of C++ grammar that can be parsed in a context-free fashion. Very little cannot be. See Appendix C for an actual implementation that avoids the ambiguities.

```
12. do; is presumed to start an iteration-statement. operator, is presumed to be a de-
clarator-id.
object-statement-seq:
object-statement
object-statement-seqopt object-statement
object-statement:
;
initializer ;
function-used-block
ﬁle-dependency-declaration
ﬁle-placement-declaration
ﬁlespace-declaration
meta-control-declaration
auto meta-control-declaration
auto meta-expression-statement
auto meta-function-deﬁnition
auto meta-variable-declaration
derived-clause object-statement
derived-clause : { object-statement-seqopt }
function-used-block:
ctor-initializer ;
ctor-initializeropt function-body
function-try-block
using ﬁle-id-list function-used-block
segment function-used-block
segment:
entry
pre
body
post
exit
derived-clause:
derived ( meta-conditional-expression )
```

## A.16 Meta-Programming

### A.16.1 Meta-names

```
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
```

### A.16.2 Meta-classes

```
meta-class-id:
meta-id
meta-id :: meta-class-id
meta-class-speciﬁer:
meta-class-key meta-class-id
meta-class-key meta-class-id base-speciﬁer-clauseopt { declaration-seqopt }
meta-class-declaration:
meta-class-speciﬁer ;
meta-class-key:
class-key
namespace
```

### A.16.3 Meta-types

```
meta-type:
meta-type-name
built-in-type-id
meta-class-key
enum
typedef
typename
using
exposed-tree:
[ ]
```

### A.16.4 Meta-variables

```
meta-variable-declaration:
staticopt constopt meta-type meta-scoped-id exposed-treeopt = tree-statement
staticopt constopt meta-type meta-scoped-id exposed-treeopt object-statements-clause
staticopt constopt meta-type ( meta-scoped-id ) exposed-treeopt = tree-statement
staticopt constopt meta-type ( meta-scoped-id ) exposed-treeopt object-statements-clause
```

### A.16.5 Meta-functions, Meta-constructors and Meta-destructors

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
staticopt t meta-type meta-scoped-id ( meta-parameter-listopt ) exposed-treeopt
compound-tree-statement
staticopt tmeta-type meta-scoped-id ( meta-parameter-listopt ) exposed-treeopt
object-statements-clause
meta-scoped-constructor-id ( ) compound-tree-statement
meta-scoped-constructor-id ( ) object-statements-clause
meta-scoped-destructor-id ( ) compound-tree-statement
meta-scoped-destructor-id ( ) object-statements-clause
meta-parameter-list:
meta-parameter
meta-parameter-list , meta-parameter
meta-parameter:
meta-type identiﬁer exposed-treeopt
meta-type identiﬁer exposed-treeopt = tree-argument
```

### A.16.6 Meta-statements

```
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
```

### A.16.7 Meta-expressions

```
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
```

## A.17 Syntax macros

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
```

## A.18 Files

```
string-expression:
string-literal
tree-literal
include-declaration:
using slash-includeopt slash-utilityopt string-expression ;
slash-include:
/ include
slash-utility:
/ utility
utility:
emit
pool
utility
frozen
ﬁle-dependency-declaration:
using / implementation =opt ﬁle-speciﬁer ;
using / interface =opt ﬁle-speciﬁer ;
ﬁle-placement-declaration:
export / implementation =opt ﬁle-speciﬁer ;
export / interface =opt ﬁle-speciﬁer ;
export / noimplementation ;
export / utility ;
ﬁle-speciﬁer:
ﬁle-name
ﬁle-entity
ﬁle-entity / implementation
ﬁle-entity / interface
ﬁle-name:
string-literal
ﬁle-name / interface
ﬁle-name / implementation
ﬁle-name / template
ﬁle-name / utility
ﬁle-name / guard = string-expression
ﬁle-name / noguard
ﬁle-name / path = string-expression
ﬁle-name / prefix = string-expression
ﬁle-name / suffix = string-expression
ﬁle-entity:
declarator-id
elaborated-type-speciﬁer
namespace scoped-id
ﬁlespace-speciﬁer:
namespace / file ﬁle-name compound-declaration
ﬁlespace-declaration:
ﬁlespace-speciﬁer ;
```
