# E Built-In Functionality

This appendix describes the built-in functionality, or more accurately the potential built-in functionality of FOG, since only about half of what is described has actually been implemented and because usage of FOG for a variety of practical applications will probably reveal requirements for further built-in support.

## E.1 Built-in Meta-classes

### E.1.1 auto

```
All meta-classes (and meta-namespaces) ultimately inherit from the ::auto meta-
class, which has no functionality. Its positioning at the root enables meta-program
code to affect all classes by composition with its initially empty meta-constructor
or meta-destructor. For instance:
auto auto::~auto()
{
std::diagnostic(@This);
}
```

generates a diagnostic message for every meta-class in an application, since the meta-destructor of the root meta-class is inherited by and consequently executed during meta-destruction of every meta-class including those for built-in types.

## E.2 Built-in Meta-namespaces

### E.2.1 std

```
A variety of generic support facilities are provided by built-in meta-functions.
These built-in functions are incorporated as part of the std meta-namespace. This
avoids cluttering the global meta-namespace directly, or indirectly through
introduction of a new namespace. The std meta-namespace already exists and is
otherwise empty since C++ reserves std for language support but has no meta-
functionality.
auto bool std::ambiguous(expression aName)
auto bool std::defined(expression aName)
```

These two meta-functions test for the presence and multi-presence of declarations. They return non-zero if the expression is ambiguous (has multiple deﬁnitions) or is deﬁned (has at least one deﬁnition).

These meta-functions may be used as predicates to avoid errors in subsequent code. They take no account of whether a declaration is enabled or not. Thus a declaration for use only in leaf classes is regarded as deﬁned at the root class and all its derived classes, even though the declaration is disabled at non-leaves.

```
auto token std::find(expression aName) []
```

Returns a possibly empty list of all declarations visible in the meta-name-space

```
auto void std::diagnostic(string aString)
auto void std::error(string aString)
auto void std::warning(string aString)
```

These meta-functions provide the only method for communication between a meta-program and the programmer. The string argument is emitted to standard out (and any log-ﬁle) classiﬁed as either a diagnostic warning or error message.

Warning messages are preﬁxed by “WARNING --” and increment the overall warning count.

Error messages are preﬁxed by “ERROR --” and increment the overall error count.

```
Obviously an extension to support meta-streams and std::cerr would be more
powerful.
auto string std::get_cpp(string aString)
```

An almost universally available preprocessor extension is the ability to pass macro deﬁnitions with an invoking command line such as

```
cc -DDEBUG_LEVEL=4 ...
FOG also supports -D as a command line option (see Appendix D).
FOG provides access to the preprocessor deﬁnition namespace via the get_cpp
built-in meta-function, which takes the name to be looked up in the Cpp name-
space as an argument and returns its value.
auto int debugLevel = $std::get_cpp("DEBUG_LEVEL");
auto if (debugLevel > 4 ) /* ... */;
auto string std::get_env(string aString)
Deﬁnitions may be acquired from the programming environment by using
std::get_env, which just invokes the POSIX getenv routine.
const char *logName = $std::get_env("LOGNAME");
auto string std::date()
auto string std::file()
auto string std::time()
These functions return the current date, ﬁle and time and replace the ANSI C
preprocessor symbols __DATE__, __FILE__ and __TIME__.
auto token std::parse(string aString)
auto token std::parse_tokens(token someTokens[])
auto token std::tokenize(string aString) []
The parse meta-function provides necessary support for character- and token-
level substitution. It performs lexical and syntactical analysis of aString to return
the equivalent syntax tree.
parse is equivalent to successive calls of tokenize and then parse_tokens, to
perform lexical and syntactic analysis respectively.It is not clear how much, if any
of this functionality is necessary or even desirable.
```

## E.3 Built-in Meta-variables

```
The following meta-variables are built-in (to the token meta-type and so inherited
by all meta-types).
Namespace
```

Identiﬁes the current namespace, which is necessary to ensure that declarations occurring nominally within one class can be rescoped to be placed elsewhere.

```
class ThisClass
{
class NestedClass {};
class ${Namespace}::SiblingClass {};
};
This works whether ThisClass is a class in the traditional unnamed global
namespace or a class in a named namespace.
When invoked directly within a namespace, Namespace identiﬁes the namespace,
not its enclosing namespace.
OuterNamespace
OuterNamespace differs from Namespace when invoked for a namespace by
returning the immediately enclosing namespace, if there is one or the global
namespace otherwise.
Thus ${Namespace}::${OuterNamespace} ﬁrst locates the current namespace
and then locates its immediately enclosing namespace. Eventually after sufﬁcient
iterations OuterNamespace always returns the global namespace.
```

It is an error to traverse potential declarations beyond their deﬁned ancestry.

```
OuterScope
Similarly OuterScope differs from Scope when invoked for a scope (class, struct,
union) by returning the immediately enclosing scope, if there is one or the
namespace otherwise.
Thus ${Scope}::${OuterScope} ﬁrst locates the current scope and then locates
its immediately enclosing scope. Eventually after sufﬁcient iterations OuterScope
always returns the namespace.
```

It is an error to traverse potential declarations beyond their deﬁned ancestry.

```
Scope
```

Identiﬁes the current scope, which may be a class, ﬁlespace, linkage, namespace, struct or union.

```
When invoked directly within a scope, Scope identiﬁes the scope, not its enclosing
scope.
Super
Super identiﬁes the primary base class. It is a short form for (and much more
efﬁcient than) Scope::bases()[0].
Use of Super for a class without a base-class is an error. The base-class
determined by Super ignores base classes declared using auto, and so is not
exactly equivalent to Scope::bases()[0], which could resolve a base meta-
class.
class Y : auto X1, public X2
{
// Super is X2
// bases()[0] is X1
};
This
This identiﬁes the current declarative region which is the same as Scope, when
invoked within the context of a class or namespace. However, when invoked within
the context of a variable or function, This refers to the variable or function, and
provides access to object-scoped meta-declarations in preference to occluded
meta-declarations from the class scope.
```

## E.4 Built-in Meta-functions

The inheritance relationships between the built-in meta-types are described in Section 4.1.2 and shown below using indentation.

```
token
expression
character
number
double
signed
unsigned
bool
string
token
expression
name
keyword
decl_specifier
cv_qualifier
identifier
meta_type
class_key
reserved
using_directive
initializer_clause
template_argument
token
declaration/statement
expression_statement
specifier
base_specifier
enumerator_definition
file_dependency_specifier
file_placement_specifier
namespace_alias_definition
object_specifier
function_specifier
meta_function_specifier
meta_parameter_specifier
meta_variable_specifier
parameter_specifier
template_parameter_specifier
templatde_parameter_specifier
type_parameter_specifier
value_parameter_specifier
typedef_specifier
using_declaration
variable_specifier
scope_specifier
filespace_specifier
linkage_specification
meta_class_specifier
namespace_definition
type_specifier
built_in_type_specifier
class_specifier
elaborated_type_specifier
enum_specifier
token
entity
base
enumerator
namespace_alias
object
exception
function
meta_function
meta_parameter
meta_variable
parameter
typedef
using
variable
token
entity
scope
filespace
linkage
meta_class
namespace
type
built_in
class
enum
struct
typename
union
token
object_statement
exception_specification
handler
iterator
modifier
array_modifier
function_modifier
pointer_modifier
reference_modifier
scoped_modifier
void
punctuation
tree_literal
All meta-types inherit from token, where
```

- polymorphism between scalars and lists is established

- default implementations of all built-in functions provided

- meta-type type testing predicates are implemented

The built-in meta-functions are described in the following sections, with the description at the lowest level in the inheritance hierarchy at which use of the meta-function is meaningful. Use below that level for a predicate testing metafunction just returns false. For other meta-functions an error message is generated.

### E.4.1 array_modiﬁer

### E.4.2 base and base_speciﬁer

```
auto class base::base()
auto class_specifier base_specifier::base()
auto bool base::is_auto()
auto bool base_specifier::is_auto()
auto bool base::is_private()
auto bool base_specifier::is_private()
auto bool base::is_protected()
auto bool base_specifier::is_public()
auto bool base::is_public()
auto bool base_specifier::is_public()
auto bool base::is_virtual()
auto bool base_specifier::is_virtual()
```

### E.4.3 built_in and built_in_type_speciﬁer

### E.4.4 character

```
auto character::operator character()
auto character::operator identifier()
auto character::operator number()
auto character::operator string()
```

### E.4.5 class and class_speciﬁer

### E.4.6 class_key

### E.4.7 cv_qualiﬁer

### E.4.8 decl_speciﬁer

### E.4.9 declaration

see statement

### E.4.10 elaborated_type_speciﬁer

```
auto class_key elaborated_type_specifier::class_key() []
auto bool elaborated_type_specifier::is_auto()
auto bool elaborated_type_specifier::is_class()
auto bool elaborated_type_specifier::is_namespace()
auto bool elaborated_type_specifier::is_struct()
auto bool elaborated_type_specifier::is_typename()
auto bool elaborated_type_specifier::is_union()
```

### E.4.11 entity and speciﬁer

```
auto string entity::implementation_file()
auto string specifier::implementation_file()
auto string entity::interface_file()
auto string specifier::interface_file()
```

### E.4.12 enum and enum_speciﬁer

```
auto enumerator enum::enumerators() []
auto enumerator_definition enum_specifier::enumerators() []
```

### E.4.13 enumerator and enumerator_deﬁnition

```
auto number enumerator::value()
auto expression enumerator_definition::value()
```

### E.4.14 exception and exception_declaration

### E.4.15 exception_speciﬁcation

### E.4.16 expression

```
auto expression expression::value()
```

### E.4.17 ﬁlespace and ﬁlespace_speciﬁer

### E.4.18 function, function_modiﬁer and function_speciﬁer

```
auto exception function::exceptions() []
auto exception_declaration function_modifier::exceptions() []
auto exception_declaration function_specifier::exceptions() []
auto parameter function::parameters() []
auto parameter_specifier function_modifier::parameters() []
auto parameter_specifier function_specifier::parameters() []
auto function_specifier function::signature() []
auto function_specifier function_modifier::signature() []
auto function_specifier function_specifier::signature() []
```

### E.4.19 handler

### E.4.20 identiﬁer

```
auto identifier::operator character()
auto identifier::operator identifier()
auto identifier::operator number()
```

returns the result of a text to numeric conversion, which usually involves an error message.

```
auto identifier::operator string()
```

### E.4.21 iterator

A typical idiomatic use of an iterator is shown in the following example

```
auto for (iterator i = $MyClass::variables(); i; ++i)
auto if (!i->is_static())
const char *MyClass::names[] = { ""$i->name() };
const char *MyClass::names[] = { 0 };
in which composition of array elements is used to build a null terminated list of the
names of the member variables of MyClass.
```

Note that an iterator maintains a copy of the identities of the elements of the iteration domain, but not of their contents. Therefore addition of an addition base class during a traversal of base classes will not be detected by the iteration. Modiﬁcation of an iteration element prior to traversal does affect the iteration.

```
auto iterator::iterator()
```

constructs an iterator already out-of-domain.

```
auto iterator::iterator(token [])
```

constructs an iterator to iterate over and from the start of the exposed list. The identities of the elements in the list are copied.

```
auto iterator::iterator(iterator)
```

constructs a copy of an iterator, which involves a copy of the identities of the elements in the iteration domain and of the position within the domain.

```
auto void iterator::operator=(token [])
```

assigns an iterator to iterate over and from the start of the exposed list. The identities of the elements in the list are copied.

```
auto void iterator::operator=(iterator)
```

assigns a copy of an iterator, which involves a copy of the identities of the elements in the iteration domain and of the position within the domain.

```
auto iterator::operator number()
```

returns true as long as the iterator remains within the iteration domain

```
auto void iterator::operator++()
```

advances the iterator through the iteration domain, setting out-of-domain once the top edge passed.

```
auto void iterator::operator--()
```

rewinds an iterator one step back through the iteration domain, setting out-ofdomain once the bottom edge passed.

```
auto token iterator::operator->()
```

returns the current element in the iteration domain, generating an error if out of domain.

```
auto token iterator::operator*()
```

returns the current element in the iteration domain, generating an error if out of domain.

### E.4.22 keyword

### E.4.23 linkage and linkage_speciﬁcation

```
auto string linkage::value()
auto string linkage_specification::value()
```

### E.4.24 meta_class and meta_class_speciﬁer

### E.4.25 meta_function and meta_function_speciﬁer

```
auto meta_parameter meta_function::meta_parameters() []
auto meta_parameter_specifier
meta_function_specifier::meta_parameters() []
```

### E.4.26 meta_parameter and meta_parameter_speciﬁer

### E.4.27 meta_type

### E.4.28 meta_variable and meta_variable_speciﬁer

```
auto token meta_variable::value()
auto token meta_variable_specifier::value()
```

### E.4.29 modiﬁer

```
auto bool modifier::is_array_modifier()
auto bool modifier::is_const()
auto bool modifier::is_function_modifier()
auto bool modifier::is_pointer_modifier()
auto bool modifier::is_reference_modifier()
auto bool modifier::is_scoped_modifier()
auto bool modifier::is_volatile()
```

### E.4.30 name

```
auto name name::full_name()
auto name name::name()
```

### E.4.31 namespace and namespace_deﬁnition

### E.4.32 namespace_alias and namespace_alias_deﬁnition

```
auto namespace namespace_alias::value()
auto name namespace_alias_definition::value()
```

### E.4.33 number

```
auto number::operator character()
auto number::operator identifier()
auto number::operator number()
auto number::operator string()
```

### E.4.34 object and object_speciﬁer

```
auto bool object::is_const()
auto bool object_specifier::is_const()
auto bool object::is_static()
auto bool object_specifier::is_static()
auto bool object::is_volatile()
auto bool object_specifier::is_volatile()
auto meta_function object::meta_functions() []
auto meta_function_specifier object_specifier::meta_functions() []
```

returns the immediate list of member meta-functions

```
auto meta_variable object::meta_variables() []
auto meta_variable_specifier object_specifier::meta_variables() []
```

returns the immediate list of member meta-variables

```
auto modifier object::modifiers() []
auto modifier object_specifier::modifiers() []
```

returns the list of declarator modiﬁers

```
auto type object::type()
auto type_specifier object_specifier::type()
```

### E.4.35 object_statement

```
auto bool object_statement::is_boundary()
auto bool object_statement::is_leaf()
auto bool object_statement::is_pure()
auto bool object_statement::is_root()
```

### E.4.36 parameter and parameter_speciﬁer

### E.4.37 pointer_modiﬁer

### E.4.38 punctuation

### E.4.39 reference_modiﬁer

### E.4.40 reserved

### E.4.41 scope and scope_speciﬁer

```
auto base scope::all_bases() []
auto base_specifier scope_specifier::all_bases() []
```

returns the transitive list of base-speciﬁers (including meta-bases)

```
auto scope scope::all_classes() []
auto scope_specifier scope_specifier::all_classes() []
```

returns the transitive list of nested classes

```
auto function scope::all_functions() []
auto function_specifier scope_specifier::all_functions() []
```

returns the transitive list of member functions

```
auto type scope::all_types() []
auto type_specifier scope_specifier::all_types() []
```

returns the transitive list of member types

```
auto typedef scope::all_typedefs() []
auto typedef_specifier scope_specifier::all_typedefs() []
```

returns the transitive list of member typedefs

```
auto variable scope::all_variables() []
auto variable_specifier scope_specifier::all_variables() []
```

returns the transitive list of member variables

```
auto base scope::bases() []
auto base_specifier scope_specifier::bases() []
```

returns the immediate list of base-speciﬁers (including meta-bases)

```
auto class_key scope::class_key() []
auto class_key scope_specifier::class_key() []
auto scope scope::classes() []
auto scope_specifier scope_specifier::classes() []
```

returns the immediate list of nested classes

```
auto entity scope::friends() []
auto specifier scope_specifier::friends() []
auto function scope::functions() []
auto function_specifier scope_specifier::functions() []
```

returns the immediate list of member functions

```
auto bool scope::is_auto()
auto bool scope_specifier::is_auto()
auto bool scope::is_boundary()
```

returns true if there is a pure-virtual function in an immediate base class but no pure-virtual in this class

```
auto bool scope::is_class()
auto bool scope_specifier::is_class()
auto bool scope::is_leaf()
```

returns true if there are no derived classes

```
auto bool scope::is_pure()
```

returns true if there is a pure-virtual function

```
auto bool scope::is_struct()
auto bool scope_specifier::is_struct()
auto bool scope::is_union()
auto bool scope_specifier::is_union()
auto typedef scope::typedefs() []
auto typedef_specifier scope_specifier::typedefs() []
```

returns the immediate list of member typedefs

```
auto type scope::types() []
auto type_specifier scope_specifier::types() []
```

returns the immediate list of member types

```
auto variable scope::variables() []
auto variable_specifier scope_specifier::variables() []
```

returns the immediate list of member variables

### E.4.42 scoped_modiﬁer

### E.4.43 speciﬁer

### E.4.44 statement

### E.4.45 string

```
Identical strings are represented by the same metaobject, so content comparison
is performed by operator== and operator!=.
auto string::operator character()
auto string::operator identifier()
auto string::operator number()
auto string::operator string()
```

### E.4.46 template_parameter and template_parameter_speciﬁer

### E.4.47 token

```
The token meta-type provides a default implementation for all built-in meta-
functions. Predicate meta-functions such as is_virtual() that return a bool
value are implemented to return false. All other meta-functions generate an error
message.
auto entity token::find_entity(expression anExpression) []
```

returns all visible entities (names or types)

```
auto object token::find_name(expression anExpression) []
```

returns all visible names

```
auto type token::find_type(expression anExpression) []
```

returns all visible types)

```
auto token token::get(expression anExpression)
```

returns the meta-declaration visible in the meta-name-space, generating an error if undeﬁned or ambiguous.

```
auto entity token::get_entity(expression anExpression)
```

returns the visible entity (name or type), generating an error if undeﬁned or ambiguous.

```
auto object token::get_name(expression anExpression)
```

returns the visible name, generating an error if undeﬁned or ambiguous.

```
auto type token::get_type(expression anExpression)
```

returns the visible type, generating an error if undeﬁned or ambiguous.

```
auto bool token::is_exposed()
```

returns true if token comprises an exposed list.

```
auto bool token::is_list()
```

returns true if token comprises an encapsulated or exposed list.

```
auto bool token::is_meta_type()
```

returns true if token is at least as specialised as meta_type.

```
auto unsigned token::length()
```

returns the number of elements in an encapsulated or exposed list.

```
auto meta_type token::meta_type()
```

returns the describing meta-type.

```
auto token token::sub_list(unsigned start, signed length) []
returns an exposed list comprising the abs(length) elements from start to
start+length exclusive, generating an error if any element out-of range.
auto token token::operator[](unsigned index)
returns the index element, generating an error if out-of range.
auto token token::operator+(token tokens[]) []
```

return a new list comprising tokens appended to this list.

```
auto void token::operator+=(token tokens[]) []
```

appends tokens.

### E.4.48 type and type_speciﬁer

### E.4.49 typedef and typedef_speciﬁer

```
auto type typedef::value()
auto type_specifier typedef_specifier::value()
```

### E.4.50 typename

### E.4.51 using and using_declaration

### E.4.52 using_directive

### E.4.53 variable and variable_speciﬁer

```
auto expression variable::value()
auto expression variable_specifier::value()
```

### E.4.54 void

```
The void meta-type is used for invalid and zero values.
```
