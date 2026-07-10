# 7 Examples

This chapter provides example uses of FOG, starting with very simple idioms and progressing to more serious applications.

## 7.1 Idioms

An idiom is a simple coding construct that arguably could be part of the C++ language.

Although some of the simple idioms are little more than one-liners, they should not be dismissed as trivial programs. They demonstrate how real programming problems can have very simple solutions in FOG. The examples support direct expression and realisation of programming intent and consequently improve reliability and maintainability, trading off some declarative complexity against ease of instantiation.

### 7.1.1 InheritedTypedef

```
Using inheritance, a programmer may derive a specialisation from a base class.
This may involve a reﬁned implementation of a base class operation. Whereas the
derived implementation is constrained by the inner invocation from the base
class, in languages such as BETA [Madsen93], C++ imposes few limitations. The
programmer is free to provide a replacement implementation, or to incorporate the
derived functionality where appropriate. The derived functionality is incorporated
by invoking the base-class method explicitly.
void DerivedClass::method()
{
// some code
BaseClass::method();
// more code
}
When DerivedClass is derived directly from BaseClass
class DerivedClass : public BaseClass { /* ... */ };
```

the code fragment operates as might be expected.

```
However, if DerivedClass is indirectly derived
class IntermediateClass : public BaseClass { /* ... */ };
class DerivedClass : public IntermediateClass { /* ... */ };
it is unclear why IntermediateClass::method() was bypassed.
It may be that when the fragment was ﬁrst written, DerivedClass was directly
derived, and there was no IntermediateClass method to worry about. When
subsequent maintenance or evolution introduced
IntermediateClass and
IntermediateClass::method(), a bug was also introduced.
If IntermediateClass::method() is accidentally bypassed, then the use of the
BaseClass name may lead to incorrect behaviour.
If IntermediateClass::method() is deliberately bypassed, then the code is
obscure, since in some respect
DerivedClass is not a specialization of
IntermediateClass, or
IntermediateClass is not a specialization of
BaseClass. Such obscurity probably hides a design ﬂaw, and certainly merits a
comment to explain why the
IntermediateClass::method() has been
bypassed.
```

Summarising: The use of explicit type names to refer to inherited members leads to fragile code.

Stroustrup on page 292 of The Design & Evolution of C++ [Stroustrup94] describes discussions about a potential language extension to resolve this

problem, and credits Michael Tiemann for providing the following very simple solution:

```
class foreman : public employee {
typedef employee inherited;
//...
void print();
};
class manager : public foreman {
//1a
typedef foreman inherited;
//1b
//...
void print();
};
void manager::print()
{
inherited::print();
//...
}
The base class is referred to as inherited throughout the class, limiting the
knowledge of the inheritance hierarchy to lines //1a and //1b. The reader is
assured that the intent is to invoke the base class functionality, and may then
question any reference to class names other than inherited. The maintainer
does not risk breaking code when the inheritance hierarchy is reviewed.
```

FOG offers three alternative solutions that avoid the redundancy of line //1b, and consequently avoid the risk of an inconsistency between line //1a and //1b.

**Super meta-variable**

```
FOG provides a built-in meta-variable Super that refers to the ﬁrst base class.
There is therefore no need for a typedef. The built-in meta-variable can be used
instead.
void manager::print()
{
${Super}::print();
//...
}
This is not a very elegant solution. It requires a $-expression to appear in normal
application code.
```

**Super typedef**

```
The typedef can be reintroduced to hide the $-expression from application code.
class manager : public foreman {
typedef $Super inherited;
```

This clearly expresses the intent and is guaranteed to be locally correct. There is however a possibility that the typedef could be omitted for a derived class, and if the typedef is accessible no compilation error would arise.

**InheritedTypedef meta-function**

The problem of an inaccurate deﬁnition in a derived class, and the inconvenience of providing the deﬁnition in every class can be resolved by a derivation rule, and since the ﬁnal solution is of general utility it is presented as a meta-function.

```
auto declaration InheritedTypedef()
{
private typedef @Super inherited :{ derived(!is_root()); };
}
Expressing the solution as a meta-function introduces three changes to the
typedef declaration:
A private keyword is added to ensure that the typedef is not visible outside the
class. Omission of the keyword would leave the declaration vulnerable to
assuming a prevailing accessibility from the invocation context.
An object-statement-scope
:{...} follows the typedef so that additional
declarations can qualify the typedef. The derived(!is_root()) derivation rule,
causes the typedef declaration to be generated in all classes that derive from the
class for which the meta-function is invoked. Thus when invoked as
class employee
{
$InheritedTypedef();
};
no typedef is generated at the root (employee) for which Super may be
undeﬁned, but typedefs are generated for all classes derived from employee.
The use of @Super rather than $Super defers resolution of the meta-expression
until each actual declaration generated as a result of the derivation rule is
installed in the derived class. @Super therefore resolves to the super-class of the
actual derived class. In contrast, $Super would be resolved when the potential
declaration is deﬁned for the root class, and would therefore result in the typedef
always resolving to the super-class of the root class.
```

**Beneﬁt**

Use of the meta-function requires a single line of source code for each base class, and ensures consistency throughout an inheritance hierarchy.

The conventional implementation requires one line of source code per derived class in the inheritance hierarchy, and is vulnerable to typographic errors.

**Alternatives**

This problem cannot be solved reliably without access to the name of the baseclass.

The Tiemann approach localises the redundant declarations to a single place, potentially adjacent to the necessary declaration.

Applications that already make extensive use of preprocessor macros to deﬁne class scaffolding can implement the typedef with no additional application code:

<Interface-file>

```
class DerivedClass : public BaseClass
{
SCAFFOLDING_INTERFACE(DerivedClass, BaseClass)
//...
};
```

<Implementation-file>

```
SCAFFOLDING_IMPLEMENTATION(DerivedClass)
The typedef can be incorporated in the SCAFFOLDING_INTERFACE preprocessor
macro. The examples in Sections 7.3.3 and 7.5 show how other standard
declarations that may form part of SCAFFOLDING_INTERFACE can also be resolved
automatically, rendering both of the
SCAFFOLDING_IMPLEMENTATION and
SCAFFOLDING_INTERFACE macros obsolete.
```

### 7.1.2 NoAssign, NoCopy

C++ provides powerful facilities to support the deﬁnition of encapsulated data types. C++ also eases the deﬁnition of user deﬁned types by providing default

implementation for constructors, destructors and assignment operators. For classes involving pointers or allocated resources, the default implementations are often inappropriate. Replacement implementations may need to be provided.

For some classes, there is no possible replacement. Consider a class that deﬁnes properties of colours, with a single unique object for each used colour combination. Creating a copy of such an object is meaningless because that would break the uniqueness property. Assigning to such an object is illegal since assignment would involve creation of a new combination, and so could not be represented by the old object.

For other classes, the replacement implementation might never be used and so the development cost and code size of the replacement cannot be justiﬁed.

These problems are solved in C++ by declaring the relevant methods to be private and not providing an implementation for them [Coplien92] p45.

```
class UniqueColor
{
private:
UniqueColor(const UniqueColor&);
// not implemented
UniqueColor& operator=(const UniqueColor&);// not implemented
};
Any attempt to copy or assign to a UniqueColor object outside of the scope of
UniqueColor encounters a compilation error through the
private access
restriction. Within the scope of UniqueColor, compilation succeeds, but a linker
error results from the missing implementations.
```

This is a well-known idiom, but is obscure and so not always recognised by the novice programmer. An accidental implementation of the not implemented functions may break the informal coding convention.

Meta-functions may be provided in FOG to express the intent more clearly and to enforce the non-implementation constraint.

```
auto declaration NoAssign()
{
private $Scope& operator=(const $Scope&)
:{ export/noimplementation; };
};
auto declaration NoCopy()
{
private ${Scope}(const $Scope&) :{ export/noimplementation; };
};
```

The meta-functions may be invoked as

```
class UniqueColor
{
$NoCopy();
$NoAssign();
};
```

clearly expressing the programming intent.

```
The implementation of each meta-function makes extensive use of $Scope to
deﬁne declarations appropriate to the invoking scope. The :{} declares an object-
statements-clause, within which function annotations can occur. In this case the
function scope contains the single declaration export/noimplementation, which
ensures that FOG generates a compilation error if any attempt is made to
compose an implementation.
Use of $Scope and @Super are interchangeable in this example, since there is no
re-evaluation in derived contexts. The extra {} on ${Scope} is necessary to avoid
the interpretation ${Scope(const $Scope&)}.
```

**Alternatives**

```
NOCOPY and NOASSIGN preprocessor macros could be deﬁned, however the name
of the surrounding scope would have to be passed as a parameter.
class UniqueColor
{
NOCOPY(UniqueColor);
NOASSIGN(UniqueColor);
};
```

Enforcement of no-implementation requires the additional language support provided by FOG.

### 7.1.3 Mutate

```
ANSI C introduced the const qualiﬁer to types to deﬁne unchanging values. C++
extends const to apply to objects and consequently member functions. Use of
const ensures that any attempt to change an object is detected at compile time.
However, it may be appropriate for the implementation of an object to perform lazy
evaluation of some of its properties, caching the results to avoid a re-evaluation.
For example, the conventional complex class has a Cartesian representation, and
must therefore calculate a polar representation, each time the polar
representation is requested. A more sophisticated complex number class could
cache the polar representation lazily, so that no calculation cost was incurred
when the polar representation was unused, and avoid additional calculation cost
for uses after the ﬁrst. From an external perspective, the complex number object
is unchanged by the use of a polar representation and so the usage method should
be const. Internally the cached context changes and so the object is not const.
```

This distinction is referred to as physically-const and logically-const on p26 of [Coplien95b]. On p76, [Meyers92] uses the term conceptual constness.

```
Implementation of logically-const code requires that the const qualiﬁer to be cast
away.
((ComplexNumber *)this)->_polar_value = ...
```

Usage of casts in application code is considered poor style. They are prone to error: any kind of change can be performed, although only a very subtle change was intended. When reviewing code it is difﬁcult to locate casts with searching tools, and it is not always obvious what the intent of the cast is. Wrapping the necessary cast up in a private overloaded inline function makes the meaning clear, and simpliﬁes searching for the usage, and allows a stronger no-casts programming practice to be used elsewhere.

```
class ComplexNumber
{
/* ... */
private:
inline ComplexNumber& mutate() const
{ return *(ComplexNumber *)this; }
};
supports use within ComplexNumber member functions as:
mutate()._polar_value = ...
```

This can be implemented by a simple meta-function:

```
auto declaration Mutate()
{
private $Scope& mutate() const { return *($Scope *)this; }
}
```

and installed in a class by

```
class ComplexNumber
{
$Mutate();
};
```

**Alternatives**

```
The introduction of the mutable storage-class-speciﬁer resolves many of the problems
of logically-const const. In the above example, declaring _polar_value as
mutable would be a complete solution. However mutable is a storage-class-speciﬁer
and not a cv-qualiﬁer and so non-const methods cannot be invoked without some
form of cast.
mutate().set_polar_value(...);
The introduction of const_cast<T> resolves the danger of inadvertently casting
to a different class, since the type of T can be statically checked. However
const_cast<ComplexNumber>(*this)._polar_value = ...
is a little harder to read, contains a redundant typename, and does not work on
old compilers. The improved functionality of const_cast can be exploited to give
a more robust meta-function:
auto declaration Mutate()
{
private $Scope& mutate() const
{ return const_cast<$Scope>(*this); }
}
```

### 7.1.4 Clone, Prototype

In languages such as Smalltalk, creating a copy of an object presents no problem, since there is direct language support. In C++, creating a copy of an object whose type is known at compile-time makes use of the copy constructor. However when the type is not statically known, the programmer must provide support code.

Stroustrup on p424 of [Stroustrup97] refers to this support code as a virtual constructor. The technique is also referred to a cloning and forms part of the Prototype pattern [Gamma95].

The support code requires that every concrete1 class implements a virtual function to create a clone of itself.

```
class RootClass
{
//...
public:
virtual RootClass *clone() const = 0;
};
class IntermediateClass : public RootClass
{
//...
};
class ConcreteClass : public IntermediateClass
{
//...
public:
virtual RootClass *clone() const;
};
RootClass *ConcreteClass::clone() const
{
return new ConcreteClass(*this);
}
```

1. a class with no pure virtual methods

Invocation of the virtual function upon an object of unknown type therefore invokes the appropriate class-speciﬁc method to create the clone.

```
const RootClass& someObject = ...;
RootClass *clonedObject = someObject.clone();
Implementation of this idiom requires contributions to the interface and to the
implementation of each concrete class. These will often be in different ﬁles,
because of the need to avoid excess include ﬁle dependencies. Observance of the
protocol is largely enforced by the use of a pure virtual function. However an
inaccurate implementation can arise through failing to create an instance of the
correct class, or through failing to implement clone when one concrete class
inherits from another concrete class.
```

A FOG meta-function using a derivation rule can generate all derived class code automatically

```
auto declaration Prototype()
{
public virtual $Scope2 *clone() const = 0
:{
derived(!Scope.is_pure()) { return new @{Scope}(*this); };
};
}
The clone functionality is woven by a single invocation from the root class.
class RootClass
{
$Prototype();
};
The pure virtual function is deﬁned in the invoking class. The function body has
an associated derivation rule requiring implementation in all concrete classes.
The function body, and consequently its declaration, is therefore generated in
each concrete class. The use of @{Scope} within the function body ensures that
resolution of the name is deferred until the function body is installed in its actual
class, and so ensures that a new instance of the concrete class is created.
It is important to use derived(!is_pure()) rather than a less restrictive rule
such as derived(true) to avoid generation of code that creates instances of
abstract classes, since compilers are required to generate error messages if an
abstract class is constructed.
```

**Alternatives**

This problem is insoluble without an automatic code generator. Traditional approaches require extensive use of scaffolding macros.

## 7.2 Patterns

The general problem of providing implementations of patterns is not soluble, because patterns are too vague and require tailoring to suit the application context. However, implementations of patterns suitable for more restrictive contexts are possible. A few such implementations are presented in the following sections.

### 7.2.1 StaticFlyweight

Components are easier to use when components that exhibit similar functionality provide it in a predictable way. This can be achieved by providing an external

```
2. $Scope may be changed to @Scope to use the derived type as the return type, which
is permitted in standard C++ but not allowed in earlier implementations of C++.
```

interface for a standardised internal behaviour, often realised by the use of virtual functions to provide polymorphic behaviour at run-time. Classes (and more generally declarations) with this form of external interface compatibility are called isomorphic.

The concept of an isomorphic interface at compile-time is not normally used in C++, but lies at the heart of the Standard Template Library, where many of the templates operate on any type that complies with the deﬁned isomorphic interface. The concept of isomorphism is independent of polymorphism and inheritance: the templates work for a variety of independent inheritance trees; there is no need for a common base class.

Provision of meta-functions and meta-variables makes the advantages of isomorphism more visible to the programmer. Families of isomorphic metafunctions can be declared, one per class, to provide the same functionality for the programmer, but using a distinct implementation appropriate to each class.

The Flyweight pattern [Gamma95] describes how shared objects can be used to reduce allocation costs. The pattern comprises a Flyweight manager responsible for managing a pool of Flyweight objects, one of which is returned in response to a request from the client. The manager creates a new Flyweight when no existing one is available.

In the general implementation, the pool may be large and so the factory manager needs some form of map to locate the Flyweights.

A useful variation occurs when the Flyweight objects can be enumerated at compile-time and may have the same life-time as the program. The manager then degenerates to a set of static member functions one per ﬂyweight.

```
class DayOfWeek // ...
{
public:
static const String& monday()
{ static const String theDay("Mon"); return theDay; }
static const String& tuesday()
{ static const String theDay("Tue"); return theDay; }
// ...
};
The DayOfWeek class makes the textual representation of each day available as
ﬂyweight objects, using function scope to deﬁne the object lifetime and thereby
avoid the race conditions during object construction that can arise from the use of
static member variables.
```

Using a family of isomorphic meta-functions to capture this variant of the Flyweight pattern avoids the need for an application programmer to understand the construction protocols of the relevant ﬂyweight.

```
class String
{
/* ... */
auto declaration StaticFlyweight(identifier name,
string init = "")
{
public static const String& ${name}()
{
static const String staticInstance($init);
return staticInstance;
}
}
};
The DayOfWeek class may then be simpliﬁed to
class DayOfWeek // ...
{
$String::StaticFlyweight(monday, "Mon");
$String::StaticFlyweight(tuesday, "Tue");
// ...
};
The programming complexity is now partitioned appropriately. Provision of a
ﬂyweight requires just a single line. The two to ﬁve lines to implement ﬂyweight
construction appear just once as part of the ﬂyweight class, rather than repeated
throughout each static ﬂyweight manager. Since the construction protocol for the
ﬂyweight is encapsulated by the meta-function, different protocols may be used,
without affecting the callers, except in so far as additional initialisation arguments
might be required. For instance, if an implementation of String makes use of the
Flyweight pattern to share all identical strings across an application, the above
code might not work. The String class might deﬁne private constructors and/or
destructors to prohibit static instances. The following may be necessary:
auto declaration String::StaticFlyweight(identifier name,
string init = "")
{
public static const String& ${name}()
{
static const StringHandle staticInstance($init);
return *staticInstance;
}
}
where StringHandle is a smart pointer to a String.
```

**Alternatives**

It is possible to use the preprocessor to solve this problem

```
#define STRING_STATIC_FLYWEIGHT(name, init) \
static const String& name() \
{ \
static const String staticInstance(init); \
return staticInstance; \
}
```

However the preprocessor approach is unnatural and so programmers tend to avoid its use. Meta-functions ﬁt within the context of the language and so provide a useful addition to a programmer’s tool-box.

This kind of problem can sometimes be solved using templates. However in this case the initializer is a string value which is not a legal template parameter, so in the following example the initializer is changed to a character to produce legal code.

```
class String
{
/* ... */
public:
template <const char initString>
class StaticFlyweight
{
public:
const String& operator()() const
{ static const String iT(initString); return iT; }
};
};
class DayOfWeek
{
/*...*/
public:
static const String::StaticFlyweight<'M'> monday;
static const String::StaticFlyweight<'T'> tuesday;
};
```

The initializer is cached as the template parameter so that access of the ﬂyweight as

```
DayOfWeek::monday()
```

invokes

```
String::StaticFlyweight<'M'>::operator()()
```

which maintains the appropriate ﬂyweight instance. The above code compiles, but fails to link because no implementation objects were created. It is unfortunately necessary to duplicate the declarations of the ﬂyweights in the interface by deﬁning implementations of the functional objects, even though they are never used and have no content.

```
const String::StaticFlyweight<'M'> DayOfWeek::monday;
const String::StaticFlyweight<'T'> DayOfWeek::tuesday;
```

This example shows many of the limitations of templates:

- limited range of parameter types

- more than one line to express the invocation

- relatively obscure implementation

```
Note that use of operator()() is the only way in which instantiation of a template
can result in a user-deﬁned function name. Since it is necessary to provide a
corresponding deﬁnition, this approach requires two declarations for a user-
deﬁned name, just the same as for renaming with an inline function or a typedef.
```

When more than a single declaration is required, the template approach fails, although the preprocessor approach may remain viable.

### 7.2.2 Member

It is customary to declare member variables and associated support declarations individually, typically involving

- member declaration

- initialization

- get methods

and sometimes

- set methods

in addition to any actual application functionality. Any intended mode of behaviour of the member is left to be inferred from the miscellaneous declarations.

For variables with primitive types, coding errors can easily arise from missing initializers.

For variables with more complicated types, the associated declarations may be less obvious.

The standard declarations are easily provided by meta-functions, whose name demonstrates the programming intent, and whose use offers opportunities for uniform code evolution along the conventional development path, or for metaprogramming to compose additional functionality.

```
The three declarations required to deﬁne a member variable (name), a protected
set accessor (set_name) and a public get accessor (get_name) are provided by:
auto declaration ScalarMember(identifier type,
identifier name, expression init = 0)
{
private $type $name = $init;
protected void set_${name}(const $type& aValue)
{ $name = aValue; }
public const $type& get_${name}() const { return $name; }
};
```

A similar isomorphic meta-function can be provided to use a smart pointer as the member type:

```
template <class T>
class SmartPointer
{
/* ... */
public:
SmartPointer(T&);
SmartPointer(const SmartPointer&);
SmartPointer& operator=(T&);
SmartPointer& operator=(const SmartPointer&);
T& operator*();
const T& operator*() const;
};
auto declaration SmartPointerMember(identifier type,
identifier name, expression init = 0)
{
private SmartPointer<$type> $name = $init;
protected void set_${name}(const $type& aValue)
{ $name = aValue; }
public const $type& get_${name}() const { return *$name; }
};
```

Member variables and their standard accessors may then be deﬁned as

```
class Application
{
/* ... */
$ScalarMember(bool, is_valid, false);
$SmartPointerMember(Client, client);
};
```

The example uses multiple parameters to pass the type, name and initial values. A complete declaration may be passed as

```
$ScalarMember(bool is_valid = false);
```

at the expense of a little extra declaration effort:

```
auto declaration ScalarMember(variable_specifier var)
{
private $var.type() $var.name() = $var.value();
protected void set_${var.name()}(const $var.type()& aValue)
{ $var.name() = aValue; }
public const $var.type()& get_${var.name()}() const
{ return $var.name(); }
};
```

**Alternatives**

Much of this functionality can be provided by preprocessor macros, however FOG extensions are required to support member or static member initialisation from a single invocation. ISO C++ has relaxed the initialisation rules for static but not for non-static members.

Default initialisation can be enforced by a templated member such as

```
Initialized<bool> _is_valid(false);
and operator()() can be used to support a sensibly named get method.
However, further ﬂexibility cannot be provided by templates.
```

### 7.2.3 ReferenceCount3

C++ has no garbage collector. It is therefore the C++ application programmer’s responsibility to ensure that allocated memory is appropriately released. For simple forms of object use, allocated memory can be freed later in the same function. For slightly more complicated situations, objects may be organised in trees, with the parent objects assuming responsibility for releasing the resources of children; the application programmer’s responsibility is then only with the roots of the trees. In the general case, it is too difﬁcult, inconvenient or even impossible for the program structure to ensure that resources are released.

In the absence of a garbage collector, the problem is resolved by making each object responsible for releasing its own resources. Users of the object register their usage with the object. Then, when no users are registered, the object detects that it is no longer required and self-destructs, releasing the redundant resources. It is not necessary for the object to know the identities of its users, merely their number. This form of resource management is therefore implemented by reference counting: maintaining a count of the number of registered users [Coplien92].

Implementation of reference counting involves two collaborators: the reference counted object and its user. The reference counted object maintains a count of its users, that register their usage, often using a smart pointer. Construction and destruction of the pointer increment and decrement the reference count.

The reference count may be intrusive or non-intrusive.

An intrusive reference count adds an extra member variable to the reference counted object, it

- increases object size

- must be part of the class declaration

- is efﬁcient

A non-intrusive reference count, maintains the count in a disjoint area of memory, possibly accessed by an associative look-up from the object address, possibly at a location known to the user. It

- need not be part of the class declaration

- is inefﬁcient on space and/or time

The example here supports injection of the intrusive functionality. An isomorphic meta-function could be deﬁned for the non-intrusive behaviour.

**External Interface**

In order to support reference counting we need an application class to have an external interface such as

3. A version of this example that was tested using the multi-pass implementation of FOG was presented at TOOLS Eastern Europe [Willink99a]. The version presented here uses the improved syntax for derivation rules and clariﬁes the heap/non-heap allocation policies.

```
class ApplicationClass
{
protected:
virtual ~ApplicationClass();
public:
void annul() const;
void share() const;
friend inline void annul(const ApplicationClass *anObject)
{ if (anObject) anObject->annul(); }
};
share() and annul() increment and decrement the reference count, with
annul() provoking self-destruction once all references have been removed. Since
the object manages its own destruction, it is important to prevent deletion by any
other mechanism. In particular
ApplicationClass *countedObject = ...;
delete countedObject;
```

could cause premature deletion. Use of delete should therefore be trapped as a compile-time error whenever possible, with a run-time trap as well to double check. Making the destructor protected ensures a compile-time error

```
annul() acts as a replacement for delete and so the friend function is provided
to create a closer analogue for delete as
annul(countedObject);
(An exact analogue could be provided using a syntax macro, but at the expense
of deﬁning annul as a reserved word.)
```

**Internal implementation**

The internal representation involves a counter, which is constructed with value 1, so that decrementing to 0 triggers self-destruction. Care is required to ensure that copy construction and assignment preserve registrations; the copy constructor deﬁnes the count to one, and the assignment leaves the counts unchanged, since the number of referenced objects should increase by one on construction and be unchanged by assignment.

It is convenient to encapsulate this behaviour in a class, so that the unconventional implementation of copy construction and assignment can be automatically incorporated into otherwise conventional implementations of the same functions in client classes.

```
class ReferenceCount
{
private:
mutable unsigned int _shares;
public:
ReferenceCount() : _shares(1) {}
ReferenceCount(const ReferenceCount&) : _shares(1) {}
ReferenceCount& operator=(const ReferenceCount&)
{ return *this; }
~ReferenceCount() { /* ASSERT(_shares == 1); */ }
bool annul() const
{ return (_shares == 1) ? false : (_shares--, true); }4
bool heap_only_annul() const { return --_shares != 0; }
void share() const { _shares++; }
unsigned int shares() const { return _shares; }
};
```

4. This rather contrived form of conditional ensures that there is only one return statement, which is a prerequisite for inlining by some compilers.

```
The destructor may beneﬁcially validate that the share count is 1. This may detect
a premature deletion through the use delete, or some more obscure problem
such as double destruction through some unpleasant pointer recursion. The
implementation of share() performs the increment for registration of an additional
user. The implementation of annul() collaborates with the counted object to
decrement when a usage is removed. The return status indicates whether the
counted object should continue in existence. The count is not decremented if
destruction is due, in order to ensure consistent count behaviour for the three
following usage patterns
void test()
{
static ApplicationClass staticallyAllocatedObject;
ApplicationClass stackAllocatedObject;
ApplicationClass *heapAllocatedObject = new ApplicationClass();
// ...
annul(heapAllocatedObject);
}
For the statically and stack allocated objects, construction and destruction bound
the lifetime, any additional sharing through annul and share is optional. Only the
statically allocated object may live on beyond the function return. For the heap
allocated object the life-time of the object terminates with respect to the local
function with the annul, but any additional registration in the commented section
may prolong the lifetime. If only heap usage is required, the simpler
implementation of annul() as heap_only_annul() is adequate.
```

**Glue code**

```
Installing the ReferenceCount into an ApplicationClass class requires the
following code to convert the partial functionality of the ReferenceCount class
into the required external interface of the ApplicationClass class.
class ApplicationClass
{
private:
ReferenceCount _shares;
protected:
virtual ~ApplicationClass();
public:
void annul() const
{ if (!_shares.annul()) delete (ApplicationClass *)this; }
void share() const { _shares.share(); }
friend inline void annul(const ApplicationClass *anObject)
{ if (anObject) anObject->annul(); }
// ...
};
```

This is rather too much to be entered by the programmer. In the past, the author has used a preprocessor macro for all except the virtual destructor which has been dealt with manually. A meta-function provides a more powerful solution:

```
auto declaration ReferenceCount::install(bool heapOnly = false)
{
auto if (!defined(has_reference_count))
//1.1
{
//1.2
auto bool has_reference_count = true;
//1.3
private ReferenceCount _shares;
//2.1
public void share() const { _shares.share(); }
//2.2
auto if ($heapOnly)
//2.3a
{
public void annul() const
//2.4a
{ if (!_shares.heap_only_annul())
//2.5a
delete ($Scope *)this; }
protected ~${Scope}() :{ derived(true) {} };
//2.6a
}
else
//2.3b
public void annul() const
//2.4b
{ if (!_shares.annul())
//2.5b
delete ($Scope *)this; }
friend inline void annul(const $Scope *anObject)
//2.7
{ if (anObject) anObject->annul(); }
//2.8
}
//1.4
}
```

The meta-function may be invoked as

```
class ApplicationClass /* ... */
{
/* ... */
$ReferenceCount::install();
};
```

leaving little opportunity for error or misunderstanding.

```
Lines 2.1 to 2.8 of the meta-function closely follow the glue code above, save for
the use of per-declaration rather than preﬁx access-speciﬁers (Section 3.1.3.2) and
for the use of $Scope to access the invoking class name.
The meta-function contains an outer conditionalisation on lines 1.1 to 1.4. to guard
against double installation. When ﬁrst invoked, the has_reference_count meta-
variable is not deﬁned, and so the conditional succeeds. The meta-variable is then
deﬁned on line 1.3, and any reinvocation of the meta-function within the
ApplicationClass class or its derived classes is suppressed by the conditional.
The guard code is followed by the deﬁnition of the member variable (2.1), two
member functions (2.2, 2.4) and a friend function (2.7). The _shares member
variable provides the run-time storage needed by the share count, and interacts
via its constructors and destructors with the equivalent functions for the reference
counted class. The share() member function simply delegates the member
variable. The annul() member function completes the share counting protocol by
deleting the reference counted object when the ﬁnal share is removed. The
annul() friend function just provides a more convenient destruction option
avoiding the need to worry about null pointers.
The inner conditionalisation (2.3) upon the formal parameter heapOnly selects
between declarations that inhibit non-heap object construction, and the more
ﬂexible default behaviour allowing static as well as heap objects.
Line 2.6a deﬁnes the destructor as protected in the invoking class and as a
result of the derivation rule, in all derived classes. This provides a compile-time
check to catch most attempts to create counted objects statically or on the stack,
which might other wise have a lifetime extended either through a start-
up/shutdown race condition or beyond the return of a function. The virtual
keyword is omitted to avoid prejudicing the class designer’s decision on whether
the destructor should be virtual. Although the destructor should probably be
virtual, it is not appropriate for a meta-function to enforce ‘good’ style [Meyers92]
when there may be legitimate reasons for an alternate style.
```

Accidental double installation within the same class may seem unlikely for conventionally structured code, however when more than one meta-function is invoked as in

```
class ApplicationClass /* ... */
{
/* ... */
$NonIntrusiveList::install();
$NonIntrusiveMap::install();
};
a multiple installation could occur indirectly. Accidental double installation within
an inheritance tree can easily occur, and is partially trapped by the inheritance of
has_reference_count. The guard is not proof against a later installation of a
reference count into a base class, or against multiply inherited reference counts.
Solutions to these problems require installation to be split into two phases, ﬁrst to
determine the overall requirements, and then to generate the corresponding
declarations. The Monitor example in Section 7.4.2 shows how the more general
problem can be resolved, without affecting application or glue code.
```

**Alternatives**

There is an efﬁcient solution to this problem, for the case of unconstrained usage from a well-deﬁned class, using the Curiously Recurring Template Pattern [Coplien95a] in which a derived class parameterises its base class.

```
class ApplicationClass : /* ... */,
public ReferenceCounted<ApplicationClass>
{ /* ... */ };
In this application, the template parameter is required to support a static_cast
from a pointer to the base ReferenceCounted<ApplicationClass> to a pointer
to the derived ApplicationClass for use by the delete in a complete rather than
partial implementation of annul().
template <class T>
class ReferenceCounted
{
/* ... */
void annul() const
{
if (!--_shares)
delete static_cast<T *>(this);
}
};
Alternatively, if the compulsory expense of the virtual function table is acceptable,
a non-templated version of ReferenceCounted can be written using a virtual
destructor and no cast.
```

If protected destructors are used to enforce heap-only usage, an extra friend declaration is required to allow the base class access to the derived destructor, violating the goal of a single mention in the instantiation glue (unless a metafunction is used to deﬁne base class and friend).

A non-templated approach is more ﬂexible, and necessary when multiple inheritance is involved:

```
class ApplicationClass : /* ... */,
public virtual ReferenceCounted
{ /* ... */ };
```

The latter approach can always be used, but at the expense of an extra indirection for the much commoner single inheritance cases.

The meta-programming approach supports an arbitrary request for reference counting functionality, leaving the meta-program to choose an efﬁcient implementation strategy. The conventional approach requires the programmer to choose and implement the strategy directly.

### 7.2.4 WholePart

Soukup [Soukup94] makes a persuasive case for implementing patterns using pattern classes. A pattern class is just a grouping of static functions templated by the types of each collaborator as shown in Figure 7.1.

The pattern class contains no member variables and is a friend of each collaborator. A pattern is used by invoking the static member function of the pattern class with the appropriate collaborator instances as parameters.

```
class C1
{
/* ... */
friend class P1<C1, C2>;
};
template <class T1, class T2>
void P1::do_something(T1& c1, T2& c2)
{
c1.poke = c2.peek;
}
```

Since the pattern class is a friend of each collaborator, the function is free to peek and poke the working variables in the collaborator objects to perform the required actions. This approach achieves a very regular style of implementation, and has very beneﬁcial effects in reducing include ﬁle dependencies. However the extensive use of friend declarations runs counter to normal programming practice.

Instantiation of a pattern using a pattern class requires instantiation of the pattern class, and insertion of friendship declarations and working variables into the collaboration classes. Instantiation of the pattern class is readily resolved by conventional C++ template instantiation. Insertion of declarations into the collaborators could be performed by manual editing. [Soukup94] describes a custom preprocessor for the CodeFarms library that performs this insertion automatically, provided the programmer has left a hook in each class of the form.

```
class State
{
MEMBER_State
// Hook to enable Cpp to insert declarations
/*...*/
};
class Town
{
MEMBER_Town
// Hook to enable Cpp to insert declarations
/*...*/
};
```

The custom preprocessor scans pattern class instantiations such as

```
WHOLE_PART(State,Town) // Declaration that State has many Towns
```

to produce conventional Cpp deﬁnitions such as

```
#define MEMBER_Town \
friend class WholePart<State,Town>; \
State *_whole;
```

Pattern Classes

C1 C4 C3 C2

P1<T1,T2> P2<T2,T3,T4>

Collaboration

Classes

**Figure 7.1 Pattern Class Friendships**

Multiple patterns are readily accommodated; the generated Cpp macro just grows.

This approach demonstrates that implementation of a particular pattern solution requires that declarations be injected into the code for collaborator classes. Only for the degenerate case of a pattern involving a single class can injection be avoided. In the terminology of Aspect Orientation, the declarations associated with each aspect (or pattern) must be woven together to create composite declarations acceptable to the C++ compiler. FOG supports this weaving and eliminates the need for a custom preprocessor and for the preprocessor hooks that support the CodeFarms library.

```
template <class Whole, class Part>
auto declaration WholePart::install()
{
class $Whole
{
friend class $Dynamic;
private list<$Part> _parts;
/* optional construction, delegations and destruction */
};
class $Part
{
friend class $Dynamic;
private $Whole *_whole;
/* optional construction, delegations and destruction */
};
}
```

Invocation of the installation function as

```
$WholePart<State,Town>::install();
```

just adds the required friend declaration and member variable to each collaborator class identiﬁed by the template parameters.

```
The semantics of meta-function execution involve replacement only of formal
parameters in the scope of the meta-function, before returning the declarations in
the meta-function body for interpretation within the invocation scope. The meta-
function apparently has no formal parameters. It actually has four (see
Section 4.3.11). All meta-functions have two built-in formal parameters Static
and Dynamic corresponding to the declared scope of the meta-function and the
actual scope, which differs if invoked for a derived class. In addition, each
template parameter is also a formal parameter. The usage of $Whole rather than
Whole therefore ensures that a replacement occurs before the body is returned to
the calling scope where Whole may be undeﬁned or differently deﬁned.
```

The example shows only the minimum to activate the pattern solution. Additional declarations could enforce appropriate construction and destruction protocols, and provide delegation so that users are unaware that a pattern class is in use.

It is not necessary to use Soukup’s pattern class approach, although it has a pleasant symmetry. Installation can be organised with respect to a dominant collaborator, probably the Whole class in this case. It is then only necessary to perform code injection into the other collaborators. However, whatever approach is adopted, a pattern solution with more than one collaborator class requires either manual editing to spread the pattern solution or automatic code injection.

### 7.2.5 Visitor

An implementation of the Visitor pattern [Gamma95] provides a more complete example of some of the facilities of FOG.

```
The pattern involves a hierarchy of data classes (DerivedData1, DerivedData2,
etc.) and a number of algorithms that may be performed on the data. The
algorithms
are
realised
by
algorithm
classes
(DerivedAlgorithm1,
DerivedAlgorithm2,
etc.)
and
inherit
from
the
abstract
Visitor
class
AbstractAlgorithm. The data classes similarly inherit from an abstract class
AbstractData.
Usage of the pattern requires the appropriate data and algorithm dependent
action to be performed. This is achieved by an invocation of the virtual method
AbstractData::accept(AbstractAlgorithm&), whose derived implementation
invokes AbstractAlgorithm::accept_DerivedDatan(DerivedDatan&), which
can in turn have a derived implementation to perform the required action as
DerivedAlgorithmm::accept_DerivedDatan(DerivedDatan&).
```

In a system with A algorithms and D data classes, there may be as many as A*D functions to be declared and implemented. The implementations in the algorithm classes performs the required actions in response to the two dimensional dispatch.

The scaffolding code required in the data element classes can be generated automatically by FOG.

The pattern has two degrees of freedom, which contributes to the inconvenience of a conventional manual approach. Addition of an extra algorithm class is relatively benign, requiring just that the new algorithm class implements as many of the data functions as required. Addition of an extra data class requires that the data class complies with the inherited protocol and that an additional method be deﬁned for the abstract algorithm. It may also be necessary to implement this method in every derived algorithm class.

```
The example implementation uses two meta-functions, one to be invoked in the
root data class (AbstractData), and another to be invoked in each derived data
class (DerivedDatan).
A derivation rule cannot satisfy the derivation requirements of this pattern, since
a derivation rule can only regenerate potential declarations in derived contexts. In
this case, derivation of a data element class (DerivedDatan) needs to export a
declaration to the abstract visitor class (AbstractAlgorithm). A generalisation of
the rule could be considered, but appears to be necessary only for this pattern.
The cost of a one-line invocation in each derived data element class is small
compared to the associated response code in the algorithm classes, and provides
ﬂexibility for deliberate omission if abstract levels of the data element hierarchy do
not need visitor support.
```

AbstractData

accept(AbstractAlgorithm&)

AbstractAlgorithm

accept_DerivedData1(DerivedData1&)

**Figure 7.2 Visitor classes**

inherits

invokes

DerivedAlgorithm2

DerivedAlgorithm1

DerivedData1

DerivedData2

DerivedDerivedData

accept_DerivedData2(DerivedData2&)

accept_DerivedDerivedData(DerivedDerivedData&)

accept_AbstractData(AbstractData&)

```
auto declaration VisitorBaseElement(identifier V)
{
public typedef $V Visitor;
public virtual void accept($V& aVisitor) = 0;
};
auto declaration VisitorDerivedElement()
{
public virtual void accept(Visitor& aVisitor)
{ aVisitor.accept_${Scope}(*this); }
public virtual void Visitor::accept_${Scope}($Scope& aData)
{}
};
```

The abstract classes may invoke the pattern as

```
class AbstractAlgorithm /* ... */
{
// ...
};
class AbstractData /* ... */
{
$VisitorBaseElement(AbstractAlgorithm);
// ...
};
Invocation of VisitorBaseElement deﬁnes the AbstractData::Visitor typedef
with the value AbstractAlgorithm, avoiding the need to pass the identity of the
visitor class to invocations in derived classes:
class DerivedData1 : /* ... */ public AbstractData /* ... */
{
$VisitorDerivedElement();
//...
};
class DerivedData2 : /* ... */ public AbstractData /* ... */
{
$VisitorDerivedElement();
//...
};
class DerivedDerivedData : /* ... */ public DerivedData2 /* ... */
{
$VisitorDerivedElement();
//...
};
Deﬁnition of the derived algorithm classes, like the abstract algorithm classes
requires no explicit code. The declarations are provided automatically by the
second declaration in the VisitorDerivedElement meta-function, which also
provides a default empty algorithm implementation. Since the signatures of the
algorithm are deﬁned in the base class, implementation of the derived algorithm
code need only mention the name.
class DerivedAlgorithm1 : public AbstractAlgorithm
{
using accept_DerivedData1
{
// ...
}
};
The short form using-declaration requires usage within class braces. Independent
usage can be provided by a long form using-declaration at the expense of a double
mention:
using accept_DerivedData1 DerivedAlgorithm1::accept_DerivedData1
{
// ...
}
```

or directly at the cost of requiring distributed editing to change a signature:

```
void DerivedAlgorithm1::accept_DerivedData1(DerivedData1& aData)
{
// ...
}
```

The pattern is expressed compactly, and instantiated so that its use is clear. Compliance with the pattern is ensured because the pattern provides all the relevant declarations. The manually contributed code is reduced to that necessary to provide the actual implementation. The scaffolding is almost completely removed.

## 7.3 Meta-Programming

### 7.3.1 OS Traits

Conditional compilation is essential to support a variety of conﬁguration options, often to resolve distinctions between different operating systems. Control values are variously predeﬁned by the compiler, supplied by command line or deﬁned in header ﬁles.

```
#if defined(UNIX)
static const char *temp_path = "/tmp/";
#else
static const char *temp_path = "C:\\Temp\\";
#endif
```

C++ statements occur only within functions and express evaluations to be resolved at run-time. In FOG, meta-statements are declarations and so the example simpliﬁes to:

```
auto bool unix = $std::get_cpp("UNIX") != "";
auto if (unix)
static const char *temp_path = "/tmp/";
else
static const char *temp_path = "C:\\Temp\\";
The invocation of std::get_cpp looks up UNIX in the C preprocessor namespace,
providing controlled access to command line deﬁnitions.
```

Using an Object-Oriented perspective eliminates the need for conditionalisation. The characteristics of each conﬁguration option may be packaged as metavariables (and meta-functions) of a (meta-)class, extending the traits concepts of [Myers95].

```
auto5 class OsTraits_Abstract
{
auto static bool NT = false;
// default value
auto static bool UNIX = false;
//...
};
5. These autos were omitted from the presentation in Section 1.5.1, where, as a re-
sult, classes were deﬁned. The extra autos here declare that only the meta-classes
are required, so avoiding the eventual emission of unnecessary C++ declarations.
auto5 class OsTraits_Nt : auto5 OsTraits_Abstract
{
auto static bool NT = true;
// overriding value
auto static string temp_path = "C:\\Temp\\";
//...
};
auto5 class OsTraits_Unix : auto5 OsTraits_Abstract
{
auto static bool UNIX = true;
auto static string temp_path = "/tmp/";
//...
};
The appropriate conﬁguration may then be chosen using std::get_cpp("OS") to
resolve OS from the command line. Thus
auto5 class OsTraits : auto5 OsTraits_$std::get_cpp("OS") {};
maps the required conﬁguration to OsTraits. The appropriate operating system
may be speciﬁed on the FOG command line by:
fog ... -D OS=Nt ...
```

A ﬁle may then be opened on the operating system speciﬁc temporary path by:

```
std::ofstream s($OsTraits::temp_path "results.dat");
```

(The pair of strings concatenate to give the required ﬁle name).

Having isolated the conﬁguration in separate classes and an associated header ﬁle, a new operating system can be supported by providing a preﬁx ﬁle characterising the new system and invoking it with an appropriate command line. Existing source ﬁles need no change.

**Alternatives**

This could be achieved directly using multiple layers of name substitutions with C preprocessor, but it never is. Modularization is much easier when supported by the programming environment.

This cannot be achieved by templates, which lack the ability to perform string manipulations.

### 7.3.2 SynchronisedEnum

In Section 4.4.7 on page 123, an example was given showing how a meta-function could transform a list of enumerations into an array of text strings. An alternative approach may sometimes be preferable, deﬁning additional enumerators and text array elements incrementally.

```
For instance a compiler may be structured so that the sub-algorithms for each AST
traversal algorithm are placed with one complete algorithm per ﬁle, and an
enumeration may be used to identify which algorithm is in active. Addition of an
additional algorithm should then be possible with the minimum of disruption to
other code. The declaration of the enumerator for the constant folding pass is
beneﬁcially
performed
by
a
meta-function
invocation
such
as
$install_ast_traversal(constant_folding) that resolves as many of the
shared declarations as possible
class ApplicationClass
{
public enum Enum {};
public static const char *texts[] = {};
auto declaration define(identifier aName, expression aValue)
{
enum Enum { $aName = $aValue };
static const char *texts[] = { [$aValue] ""$aName };
}
};
```

Application code may then deﬁne enumerators with explicit values (and text elements) by:

```
$ApplicationClass::define(LABEL, 40);
```

The example can be usefully simpliﬁed and made more reliable by using the sequentially allocated enumerator values directly:

```
auto declaration define(identifier aName)
{
enum Enum { $aName };
static const char *texts[] = { ""$aName };
}
```

**Alternatives**

```
The above example cannot be expressed in C++. Conventional practice requires
that a maintainer update both enumeration and array of text strings consistently.
The FOG pattern updates both at once, and provides freedom for each invocation
of ApplicationClass::define to be located with code associated with the
invocation, avoiding the need to fragment code to satisfy compiler constraints.
```

### 7.3.3 RTTI

Prior to the adoption of RTTI into C++, it was necessary for large Object Oriented programmers to implement RTTI as part of the application code. Each third party library had its own policy, which imposed signiﬁcant compliance burdens upon consumers. RTTI within the language eliminates much of the add-on functionality, however it pursues the C++ philosophy of avoiding costs of unused functionality by providing only minimal functionality. Extra functionality must still be provided extra-lingually, using the unique RTTI type identiﬁer as an index to custom information. Applications may therefore still need to provide RTTI, albeit tailored to exploit the built-in functionality.

FOG meta-programming supports conversion of declarations into a format suitable for use at run-time.

A simple example supporting just class name diagnostics and inheritance tables is provided here. A more extensive example involving formatting of member variable names is provided to support marshalling in Section 7.4.1.

**Class-name information**

Class-name information may be provided by:

```
class Rtti;
auto declaration Rtti::class_names()
{
virtual const char *class_name() const
:{ derived (true) { return ""$Scope; } }
}
```

and invoked as

```
class ApplicationClass
{
$Rtti::class_names();
}
to have class_name() return the class name for ApplicationClass and all its
derived classes.
```

This does not work for template classes, whose full name is not known at metacompile-time. A rather more elaborate approach is necessary that concatenates the names of each argument during construction of a static instance within the invoked method.

**Inheritance information**

We are trying to generate code such as:

```
class MyClass : public Base1, protected Base2
{
public:
virtual const Rtti& dynamic_type_info() const
//1
{ return static_type_info(); }
static const Rtti& static_type_info()
//2
{
static const Rtti rtti("MyClass", _base_info);
//3
return rtti;
}
protected:
static const Rtti::BaseInfo _base_info[] =
//4
{
{ &Base1::static_type_info(),
//7
(char *)&(Base1&)(MyClass&)*(char *)0x1000-(char *)0x1000 },
{ &Base2::static_type_info(),
//7
(char *)&(Base2&)(MyClass&)*(char *)0x1000-(char *)0x1000 },
{ 0,
//9
0 }
};
};
The dynamic_type_info and static_type_info methods provide the class
description in a similar way to the get_info and info methods on p448 of
[Stroustrup91]. dynamic_type_info is a virtual function and so returns the
dynamic type of a pointer, whereas static_type_info is static and so returns the
declared type.
The detailed class description uses a null-terminated list of Rtti::BaseInfo base
class descriptors to deﬁne the context of a class with respect to its bases. The
descriptor provides two ﬁelds, one that points to the base class, and the other that
identiﬁes the offset of the particular base class object within the enclosing object.
The horrendous sequence of casts establish a phantom MyClass instance at
address 0x1000 upon which offset calculations are performed. (The phantom
object can be placed at any address other than the obvious 0x0 for which C++
mandates that all offset arithmetic returns 0.)
class Rtti
{
public:
struct BaseInfo
// List element in list of bases
{
const Rtti *_base_rtti;
// Base class description
int _offset_to_this;
// Base position
};
Rtti(const char *className, const BaseInfo baseList[]);
};
auto declaration Rtti::base_names()
{
public virtual const Rtti& dynamic_type_info() const
//1
:{ derived (true) { return static_type_info(); } }
public static const Rtti& static_type_info()
//2
:{ derived (true)
{
static const Rtti rtti(""@Scope, _base_info);
//3
return rtti;
}
}
protected static const Rtti::BaseInfo _base_info[];
//4
:{ derived (true); }
auto ~${Scope}()
//5
{
auto for (iterator b = $bases(); b; ++b)
//6
using _base_info =
{ {
&${b}::static_type_info(),
//7
(char *)&($b&)($Scope&)*(char *)0x1000 -
//8
(char *)0x1000
} };
using _base_info = { { 0, 0 } };
//9
};
};
```

The functionality is installed in the same way as before with a single line at the root of an inheritance hierarchy:

```
class ApplicationClass
{
$Rtti::base_names();
};
as a result of which ApplicationClass has two methods static_type_info (2)
and dynamic_type_info (1), a null-terminated array of base class descriptors (4)
and a meta-destructor (5). The methods and array have derivation rules with a true
predicate, these methods and the array are therefore regenerated in all derived
classes. Construction of the local Rtti object (3) uses @Scope ensuring that the
derived class name is used.
The meta-destructor executes for each derived class. It loops over all base
classes (6). Each iteration adds a list element comprising a pointer to the base-
class type information (7), and an offset of the base-class within the derived object
(8). Finally the list is null-terminated (9). Each using exploits the extended re-
using-declaration (Section 3.1.4.4) to refer to
protected
static
const
Rtti::BaseInfo _base_info[] as just _base_info before specifying an
additional array element to be composed (Section 4.4.7).
```

This technique generates predictable functionality in derived classes automatically, and so compares very favourably with traditional approaches relying on multiple preprocessor scaffolding macros per class.

## 7.4 Aspects

Aspect Oriented Programming [Mens97] seeks to isolate independent programming concerns as aspects, each of which can be implemented (and re-used) independently. The next two examples show partitioning of a problem into the application, some additional concern implemented as an aspect, and a very small amount of glue code which initiates weaving the code for each aspect into the main application code. The examples also demonstrate how application code

is simpliﬁed and consequently made more reliable by the initial expenditure of extra declarative effort to achieve re-use. This is a natural extension of OO philosophy, where a class can encapsulate difﬁcult concepts making use easy although deﬁnition difﬁcult. FOG provides additional declarative power supporting better encapsulation.

### 7.4.1 Marshalling6

Communication between programs requires messages to be passed between those programs. Each message is usefully represented as an object, and so the programmer is presented with the problem of transferring the contents of one object between programs. This is readily achieved using an Interface Deﬁnition Language and CORBA when such high level facilities are available, however when working at a lower level, as often occurs for embedded systems, the problem must be solved by the programmer.

A typical approach involves the conversion of each object into a sequence of bytes with a common header that describes the format and length of the subsequent bytes. The sending program must marshal the data elements of each object into the byte stream and the receiving program must perform the corresponding unmarshalling back into an object. Preparation of this marshalling code is straightforward, but not amenable to automation with conventional compilers. In order to show how this can be resolved by FOG, it is helpful to ﬁrst show one possible conventional solution. The exposition matches the subsequent automated solution. Numbered comments (//3.0) may assist the reader in correlating the two solutions.

```
All messages inherit from the Message class, which deﬁnes the marshalling and
unmarshalling interfaces and an enumeration, whose values distinguish between
possible message formats.
typedef unsigned char uchar;
// Short name for shorter lines
class Message
{
/* ... */
protected:
enum MessageTypes
//1.0
{
MESSAGE_StockReport /* , ... */
//1.1
};
public:
virtual size_t marshal(uchar data[]) const;
//2.0
static Message *unmarshal(uchar data[]);
//3.0
};
Invocation of the marshal function ﬁlls data[] with the byte stream and returns
the message size. The unmarshal function is passed a byte stream and returns a
pointer to an object if the message is valid, or 0 on failure. A single very simple
message comprising just two data elements is used for this example.
```

6. A version of this example that was tested using the multi-pass implementation of FOG was presented at TOOLS Eastern Europe [Willink99b]. That version used ad hoc token pasting within function bodies. The version presented here exploits the polymorphic behaviour of token lists available with the superset implementation.

```
class StockReport : public Message
{
/* ... */
private:
unsigned long _item_number;
short _stock_level;
private:
inline StockReport(uchar data[]);
//5.0
public:
static StockReport *make(uchar data[]);
//4.0
virtual size_t marshal(uchar data[]) const;
//2.1
};
The message-speciﬁc marshalling into data[] is performed by a virtual function:
size_t StockReport::marshal(uchar data[]) const
//2.2
{
uchar *p = data;
//2.3
*p++ = MESSAGE_StockReport;
// Message type
//2.4
*p++ = 6;
// Message body length
//2.5
*p++ = (_item_number >> 24) & 0xFF;
//2.6
*p++ = (_item_number >> 16) & 0xFF;
*p++ = (_item_number >> 8) & 0xFF;
*p++ = _item_number & 0xFF;
*p++ = (_stock_level >> 8) & 0xFF;
//2.7
*p++ = _stock_level & 0xFF;
return p - data;
//2.8
}
Unmarshalling from data[] selects the message-speciﬁc routine:
Message *Message::unmarshal(uchar data[])
//3.1
{
switch (data[0])
//3.2
{
//3.3
default:
//3.4
return 0;
// 0 for bad message type error.
case MESSAGE_StockReport:
//3.5
return StockReport::make(data);
/* ... */
}
//3.6
}
```

Then the message-speciﬁc object is created, but only if the length is valid:

```
StockReport *StockReport::make(uchar data[])
//4.1
{
if (data[1] != 6)
return 0;
// 0 for bad message length error.
else
return new StockReport(data);
}
```

Finally, construction performs the message-speciﬁc unmarshalling:

```
StockReport::StockReport(uchar data[])
//5.1
{
uchar *p = data+2);
//5.2
{
//5.3
unsigned long temp = *p++;
temp = (temp << 8) | *p++;
temp = (temp << 8) | *p++;
temp = (temp << 8) | *p++;
_item_number = temp;
}
{
//5.4
unsigned long temp = *p++;
temp = (temp << 8) | *p++;
_stock_level = short(temp);
}
}
```

The marshalling and unmarshalling code is very predictable and in principle easy to write, however when there are many messages, it is tedious and error prone. When a data type is changed or a member variable added, there are many places where updates are required. It is preferable to generate the code automatically. This requires a meta-program that can reﬂect upon the message class declarations and generate code accordingly.

**Application aspect**

The marshalling support may be separated completely from the application code. The message classes express their own inheritance relationships, their data contents, and any other application declarations that may be necessary.

```
typedef unsigned char uchar; // Short name for shorter lines
class Message { /* ... */ };
class StockReport : public Message
{
/* ... */
private:
unsigned long _item_number;
short _stock_level;
};
```

**Aspect weaving**

```
The marshalling aspect is added (woven) by invoking the installation meta function
of the Marshal meta-class.
using "Marshal.fog";
class Message
{
$Marshal::install();
};
```

**Marshalling aspect**

The installation meta-function is:

```
auto class Marshal {};
auto declaration Marshal::install()
{
auto type MessageClass = $Scope;
//10.1
auto static statement switchBody =
//10.2
{
default:
//3.4
return 0;
}
auto number byte_count = 0;
//10.3
protected enum MessageTypes {};
//1.0
public virtual size_t marshal(uchar data[]) const
//2.0/1/2
:{
derived(true) entry
{
uchar *p = data;
//2.3
*p++ = MESSAGE_@Scope;
//2.4
*p++ = @byte_count;
//2.5
}
derived(true) exit
{
return p - data;
//2.8
}
};
public static !inline $Scope *unmarshal(uchar data[]);//3.0/1
{
switch (dataBuffer[0])
//3.2
@switchBody;
}
public static @Scope *make(uchar data[])
//4.0/1
:{ derived(true)
{
if (*p++ != @byte_count)
return 0;
else
return new @{Scope}(data);
}
};
auto ${Scope}()
//11
{
protected enum ${MessageClass}::MessageTypes
{ MESSAGE_$Dynamic };
//1.1
auto switchBody +=
case MESSAGE_$Dynamic:
//3.5
return ${Dynamic}::make(data);
private inline/implementation
${Dynamic}(uchar data[])
//5.0/1
{
uchar *p = data+2;
//5.2
}
}
auto ~${Scope}()
//12.0
{
auto for (iterator i = $all_variables(); i; ++i)
//12.2
auto if (!i->is_static())
//12.3
$i->type().marshal($i->id());
//12.4
}
}
The install meta-function is invoked from a class declaration for Message and
executes as part of the source ﬁle reading and analysis compilation phase. All
lines declare declarations that are added to the Message class. Three meta-
variables (10.1, 10.2, 10.3), a meta-constructor (11) and a meta-destructor (12.0)
are declared in addition to more conventional declarations (1.0, 2.0, 3.0, 4.0).
The ﬁrst meta-variable (10.1) caches the name of the invocation scope in the
meta-variable MessageClass for access by the meta-constructor.
The second meta-variable (10.2) deﬁnes the switchBody meta-variable, which
will accumulate the switch cases for the unmarshal routine. It is initialized with a
list comprising the default case.
The third meta-variable (10.3) deﬁnes byte_count which will be incremented with
a count of the bytes required to express the marshalled data. It is initialized to
zero.
Note that byte_count is non-static since each derived class has a distinct size,
whereas switchBody is static because all derived classes should contribute to
the single base class list.
```

The enumeration of message types is declared (1), ready for extension by the meta-constructor.

The framework of each marshalling routine is deﬁned (2), with a derivation rule to ensure regeneration in each derived class. The framework deﬁnes three lines of code for the function entry, and one line for the function exit. The function body will be deﬁned during meta-destruction.

```
The unmarshalling routine is deﬁned (3) in its entirety, using a deferred @ to
reference the switchBody after meta-programming has deﬁned its content. There
is no need for braces around the meta-variable, since FOG automatically supplies
the appropriate brace/parenthesis/comma punctuation when emitting a list as part
of a C++ declaration.
The make routine is similarly deﬁned in its entirety (4) embedding the calculated
count, using a deferred reference to await its determination by meta-programming
and of the copy appropriate to the particular derived class, since the derivation
rules ensures that the make function appears in all derived classes.
The meta-constructor is invoked for Message and all its derived classes during the
meta-construction compilation phase. Invocations occur in least derived ﬁrst
order. The meta-constructor ﬁrst deﬁnes an enumerator and then appends a
switch case.
An enumerator is deﬁned (1.1) in the MessageTypes enumeration of the Message
class. The additional enumerator extends the enumeration and so acquires a
unique value for each message class. The enumerator name is formed by
concatenation of the preﬁx MESSAGE_ and $Dynamic, the class name of the
derived meta-constructor (the derived message class).
The additional switch case is deﬁned (3.5) and appended to the switchBody
meta-variable, thereby extending the
unmarshal routine that uses it. The
invocation of the Dynamic built-in variable ensures that the identity of the derived
rather than base message class is used. Note the absence of braces to ensure
that a case rather than a list of one case is appended.
```

The meta-constructor then deﬁnes the interface and the start of the implementation for the derived message class constructor, on whose behalf the meta-constructor is executing.

The meta-destructor is similarly invoked on behalf of each message class, during the meta-destruction phase, by which time all member variables have been deﬁned. It comprises a loop to resolve the member variable dependent code and count the number of bytes in the message.

The byte count is maintained in a meta-variable initialised to 0 (10.3). The loop (12.2) iterates over all member (and inherited member) variables of the derived class, and (12.3) skips static member variables. Within the loop (12.4), invocation

```
of the marshal meta-function for the data-type of each member variable causes
emission of member-speciﬁc marshalling code. The member variable name is
passed as a parameter to a type-speciﬁc implementation such as:
auto declaration unsigned long::marshal(expression name)
//13.0
{
byte_count += 4;
//13.1
public virtual size_t marshal(uchar data[]) const
//2.2
{
*p++ = ($name >> 24) & 0xFF;
//2.6
*p++ = ($name >> 16) & 0xFF;
*p++ = ($name >> 8) & 0xFF;
*p++ = $name & 0xFF;
}
private $(Scope)(uchar data[])
//5.1
{
{
//5.3
unsigned long temp = *p++;
temp = (temp << 8) | *p++;
temp = (temp << 8) | *p++;
temp = (temp << 8) | *p++;
$name = temp;
}
}
}
Meta-functions can be deﬁned for built-in types as well as user deﬁned types. The
above declaration for the unsigned long ‘class’ supports the marshalling of
unsigned long member variables. The formal parameter name is replaced
throughout the body before the body is interpreted in the invoking context, that of
the derived message class. The update of the byte_count (13.1) therefore
maintains the counter of the derived message class, and the two declarations (2.2,
5.1) provide additional code for the body region of the derived message class
routines. The member variable iteration is in declaration order, and the ordering
of function body contributions is preserved, so the ﬁnal ordering of the many
contributions is well-deﬁned. The contributed code (2.6, 5.3) just performs the
very simple operations appropriate to the data type.
A similar isomorphic meta-function for short is needed to complete the example
(2.7, 5.4), and further routines for every other primitive data type. Nested data
types can be resolved by a nested iteration, which can be speciﬁed as a general-
purpose meta-function, passing the nested member name to the nested call,
necessitating the use of expression rather than name or identifier for the
parameter type.
auto declaration Marshal::marshal(expression name)
{
auto for (iterator i = $all_variables(); i; ++i)
auto if (!i->is_static())
$i->type().marshal(${name}.$i->id());
};
```

The general purpose meta-function can be installed by meta-inheritance for use in a nested type

```
struct NestedDataType : auto Marshal { /* ... */ };
This declares Marshal as an additional base class of the NestedDataType, but
only at (meta-)compile time. The meta-names of Marshal are therefore visible to
the
derived
class,
providing
the
required
resolution
of
NestedDataType::marshal().
```

This example shows how application code can be generated in response to the actual application declarations. The code is fully under the programmer’s control. The programmer can freely choose an alternate implementation using data tables

to describe each message rather than monolithic functions. Inheritance of messages can be exploited to trade size for speed, by changing compile-time iterations to serve only the local member variables, and changing the run-time code to invoke base class methods for inherited members. More sophisticated code can be provided to support swizzling of pointer types for database applications.

```
The generated code is portable, since all members are referred to by name. The
example code for unsigned long::marshal has a portability problem for
processors with a greater than 32 bit unsigned long, but this is a limitation of the
example solution, not of the approach.
```

### 7.4.2 Monitor7

The ability to use FOG to separate different programming concerns is demonstrated by application of a synchronisation monitor to a stack. One line of glue code is necessary to weave the otherwise independent functionality of monitor and stack.

**Application Aspect**

We ﬁrst deﬁne a simple stack class.

```
template <class T>
class Stack
{
$NoCopy();
$NoAssign();
private:
T *_elements;
size_t _capacity;
// Allocated size of _elements[]
size_t _tally;
// Used size of _elements[]
public:
Stack() : _elements(0), _capacity(0), _tally(0) {}
~Stack() { /* ... */ }
bool is_empty() const volatile { return _tally == 0; }
T pop() { /* ... */ }
void push(const T&) { /* ... */ }
T top() const { return _elements[_tally-1]; }
};
The const qualiﬁer is used conventionally to indicate that no change occurs.
Concurrent readers are therefore permissible, but concurrent writing should not be
permitted once a monitor aspect has been added.
The volatile qualiﬁer is used to indicate that access may occur without the use
of a lock, allowing interleaved reading and writing by other threads8.
```

**The Monitor Aspect (run-time)**

```
The monitor functionality is provided by a Monitor class, whose detailed
implementation is not relevant to this example.
7. A version of this example, that was tested using the multi-pass implementation of
FOG, appears in a position paper for the AOP workshop at ECOOP’99 [Willink99c].
The version here resolves reverse and multiple inheritance conﬂicts. This version
has been adapted to use the revised syntax of derivation rules as a part of an object-
statement-clause.
class Monitor
{
friend class Monitor::ReadOnlyLock;
friend class Monitor::ReadWriteLock;
private:
void acquire_exclusive() { /* ... */ }
void acquire_shared() { /* ... */ }
void release() { /* ... */ }
};
acquire_exclusive and acquire_shared block until exclusive or shared access
is available to the resource(s) managed by the monitor. release terminates the
resource reservation.
Reservation of the monitored resource is managed by a pair of nested lock
classes,
ReadOnlyLock and
ReadWriteLock. They differ only in whether
acquire_shared or acquire_exclusive is invoked.
class Monitor::ReadOnlyLock
{
private:
Monitor& _monitor;
public:
ReadOnlyLock(Monitor& aMonitor)
: _monitor(aMonitor) { _monitor.acquire_shared(); }
~ReadOnlyLock() { _monitor.release(); }
};
A lock class invokes acquire_shared to acquire the resource during construction
and ensures its release from the destructor, whose invocation C++ guarantees.
```

**Aspect composition**

```
The Stack application code above is written independently of the synchronisation
code. The presence of the volatile keyword is an optional optimisation.
```

The monitor aspect is added to the application aspect by providing additional declarations that are woven into the application code.

```
using "monitor.fog";
template <class T>
class Stack
{
$Monitor::install();
// Invoke meta-function
};
8. The qualiﬁcation of is_empty() as volatile as well as const therefore goes
beyond conventional practice, but is safe because the implementation involves a
single read, whereas top() involves at least two reads.
Qualiﬁcation of is_empty() as volatile is of limited utility, since the return
accurately reﬂects a state that existed but that state may no longer exist when the
calling code interprets the result. It would seem that the volatile qualiﬁcation is
redundant since calling code must establish a lock to encompass both
a !is_empty() and a subsequent pop(). However, the volatile qualiﬁcation is
useful when is_empty() is invoked within a polling loop that can recover on the
next iteration. The presence of volatile avoids incurring locking costs for such
a loop.
The usage is consistent because volatile indicates that concurrent change may
occur and so inhibits any optimisation that could reorder the sequence of memory
accesses.
```

**The Monitor Aspect (compile-time)**

```
The remainder of the code for this example forms part of the monitor.fog include
ﬁle. There are two relatively independent code injections to be performed to install
the monitor. Class declarations must be updated to incorporate an instance of
Monitor, and function declarations must be updated to establish locks.
Direct installation of an instance of Monitor is relatively straightforward and could
be achieved by just adding a member variable. However, making the meta-function
work in a more general purpose fashion is harder. There are ﬁve problems to be
resolved:
•
Monitor::install may be invoked more than once on the same class
•
Monitor::install may be invoked later for a derived class
•
Monitor::install may be invoked later for a base class
•
Monitor::install may be invoked later for more than one base class
•
Monitor::install may be invoked for a derived monitor
```

There must be only one synchronisation monitor in each object, so multiples must be suppressed, retaining only the one monitor in the least derived class. If a monitor is multiply inherited, virtual inheritance must be used to share it. If multiple monitors use different implementation classes, we will generate a compiler diagnostic.

Since virtual inheritance must be used to resolve the multiple inheritance problem, it is convenient to implement the more conventional monitor by non-virtual multiple inheritance rather than as a member variable. Resolving multiple inheritance then just requires composing the virtual keyword on the simpler and much commoner inheritance.

```
Resolving
the
uniqueness
problem
when
the
invocation
order
of
Monitor::install cannot be known requires splitting the structural problem into
two passes. A third pass is required to update the functions. These three passes
are performed in turn during the semantic analysis phase, meta-construction
phase and meta-destruction phase.
```

**Pass 1, Semantic analysis phase**

```
The ﬁrst pass is executed directly by the during semantic analysis of the
$Monitor::install() meta-function invocation.
```

It sets ﬂag variables indicating the class requirements, and arranges for the second phase to occur later.

```
auto const class Monitor::needs_monitor = 0;
//1
auto declaration Monitor::install()
//2
{
class $Scope : auto $Dynamic {};
//3
auto const class needs_monitor = $Dynamic;
//4
}
//1
declares a meta-variable in the Monitor class whose 0 (nil) initialisation ﬂags that
a Monitor class does not need Monitor functionality inserted into it.
//2
declares the compile-time meta-function invoked by the application glue code.
//3
adds the Monitor class (as a meta-base class of Stack).
template <class T> class Stack : auto Monitor { ... };
$Scope is not a formal and so resolves to the prevailing scope in the invocation
context.
$Dynamic is a built-in formal that resolves to actual deﬁnition scope, typically
Monitor, but
DerivedFromMonitor if
Monitor::install is invoked as
DerivedFromMonitor::install.
class DerivedFromMonitor : public Monitor
{
//...
};
//...
$DerivedFromMonitor::install();
Installation of
Monitor as a meta-base-class provides an inherited meta-
constructor and meta-destructor for Stack, and so ensures that the second and
third passes are executed for Stack (and all its derived classes).
//4
declares the Stack::needs_monitor meta-variable with a non-0 value to signal
that Monitor functionality is required. The value of Dynamic is used as the non-0
value, so that in combination with the const, any attempt to install a different class
of monitor will be caught. Re-installation of the same monitor class is allowed.
```

**Pass 2, Meta-construction phase**

```
The ﬁrst pass sets the needs_monitor ﬂag non-0 in all classes that are speciﬁed
as requiring monitor functionality, and arranges for the meta-constructor to be
invoked. Invocation in the ﬁrst pass occurs in an unpredictable order. Invocation
during the meta-construction phase occurs in a least derived ﬁrst order, which can
be exploited to install the monitor in the least derived alternative. Resolution of the
multiple inheritance conﬂict requires a further pass, which is implemented by
performing an iteration over the multiple bases.
auto Monitor::Monitor()
{
auto if (needs_monitor)
//1
{
auto class baseMonitors[] = $find(has_monitor);
//2
auto if (baseMonitors.size() == 0)
//3
{
class $Scope : public $needs_monitor {};
auto const class has_monitor = $Scope;
}
else if (baseMonitors.size() == 1)
//4
{
auto const class needs_monitor = $baseMonitors[0];
}
else
//5
{
auto for (iterator m = $baseMonitors; m; ++m)
{
class $*m : virtual $needs_monitor {};
auto const class needs_monitor = $*m;
}
}
}
}
```

//1 Execution of the meta-construction code is guarded by a test for a non-0 ﬂag, thereby inhibiting installation of monitor functionality in monitor classes.

```
//2
std::find returns an exposed list of all visible declarations of the has_monitor
ﬂag, which is used to identify the location(s) where monitor functionality is already
installed.
```

//3 If there are no deﬁnitions visible, this must be a least-derived requirement and so the monitor class is speciﬁed as a public base class, composing with the existing

```
speciﬁcation as a meta-base class. The has_monitor ﬂag is deﬁned to indicate
the location of the monitor functionality.
//4
If there is exactly one deﬁnition visible, then the inherited functionality is adequate
and no further functionality is required in this class. (Re-)declaration of the
derived needs_monitor provokes an error message if conﬂicting monitor classes
are in use.
```

//5 If there is more than one deﬁnition visible, then a multiple inheritance conﬂict must be resolved. The iteration loops over all deﬁnitions and redeﬁnes the base-class to use virtual inheritance, and detects conﬂicting monitor classing.

**Pass 3, Meta-destruction phase**

The ﬁnal phase of monitor installation should occur after any concurrent metaprogramming has deﬁned additional member functions, so that all member functions may have locking code inserted.

```
auto Monitor::~Monitor()
{
auto if (needs_monitor)
// 1
{
auto for (iterator f = $functions(); f; ++f)
// 2
{
auto if (f->is_static())
// 3
;
else if (f->is_volatile())
// 4
;
else if (f->is_const())
// 5
{
$f->specifier()
:{
entry { ReadOnlyLock aLock(*this); }
};
}
else
// 6
{
$f->specifier()
:{
entry { ReadWriteLock aLock(*this); }
};
}
}
auto if (friends().size() != 0)
// 7
$std::error("friend of monitored " $Scope " detected.");
}
}
```

//1 Once again functionality is guarded to prevent operation on monitor classes.

//2 The loop over all functions uses the decl-speciﬁers to determine whether monitor code needs inserting.

```
//3
static member functions are not associated with any object and so have nothing
to monitor access to.
//4
volatile is recognised as a requirement to bypass locking.
//5
const member functions require a shared lock, which is provided by specifying an
entry code segment for the function whose full name is returned by
function::signature().
//6
Similarly non-const member functions require an exclusive lock.
```

//7 Finally the problem of friend functions and classes subverting the protection is resolved in a very heavy handed fashion by banning friends. (Direct access by friends must be changed to use access functions into which locks can be inserted

automatically. It very hard and probably impossible to analyze all code associated with a friend class or function to guarantee that it does not violate access constraints.)

**Function Weaving**

```
FOG performs function weaving by concatenating the code from multiple function
bodies, within the ﬁve named regions entry, pre, body, post and exit (see
Section 4.4.8). The entry region precedes the default body region and so the
above meta-program generates the additional contribution
template <class T>
T Stack::top() const
// from the meta-destructor
:{
entry { ReadOnlyLock aLock(_monitor); };
};
```

to be woven with the application function:

```
template <class T>
T Stack::top() const
// from application aspect
{ return _elements[_tally-1]; }
```

to generate the ﬁnal C++ result:

```
template <class T>
T Stack::top() const
{
#line ...
ReadOnlyLock aLock(_monitor);
#line ...
return _elements[_tally-1];
}
```

**References**

Monitor and Stack are fundamental concepts and consequently staples for numerous articles in many Computer Science ﬁelds.

[Stroud95] used OpenC++ version 1 to implement atomic data types by intercepting method calls at run-time.

[Hedin97b] considered the monitor from an Aspect Oriented perspective, and introduced an attribute extension language to enable a preprocessing stage to validate that the requisite coding constraints had been observed. In this example we use introspection to synthesise the required code directly.

[Bjarnason97] advocates an extensible language, so that the required monitor protocol can be incorporated into the extended language. Language extension involves manipulation of syntax trees, and it is not clear how practical this is for a language with as challenging a syntax as C++.

## 7.5 A Real Example - BURG

FOG grew out of work to improve productivity in a different ﬁeld of compilation technology.

It is difﬁcult to apply high level programming concepts to Digital Signal Processors because of the very poor quality of the available compilers [Willink97b]. This is in part due to lack of awareness of the need for better support and partly due to the extreme difﬁculty of matching the performance of hand-crafted assembler on rather challenging architectures. Research therefore started to apply modern Very Long Instruction Word (VLIW) scheduling concepts, using an intermediate representation supporting data parallelism [Muchnick93]. The intermediate

representation was extended to support user-characterised types [Willink97a], as part of a relatively general purpose compiler framework.

One of the activities of a compiler involves selection of appropriate machine instructions (such as ADD or MOVE) to implement the program, usually represented by a tree of Abstract Syntax Tree nodes [Aho86]. An effective approach to solving this problem involves a Bottom-Up Rewrite System [Proebsting95], which searches the tree from the leaves upwards identifying the lowest cost solution that has each node covered exactly once by a machine instruction. The tree may then be rewritten in terms of the selected machine instructions. In order to support multiple target architectures, alternative instruction sets must be covered. Implementation of this diversity is assisted by the use of a Bottom-Up Rewrite Generator to transform a description of each machine instruction into the form needed for an efﬁcient tree search. An example of this form of generator is lburg that forms part of the lcc C compiler [Fraser95].

lburg is a compact C program comprising just three ﬁles:

- lburg.c has 690 lines and 4652 tokens

- lburg.h has 66 lines and 259 tokens

- gram.y is a 19 rule, 37 state yacc parser grammar

(token counts are non-comment, non-whitespace preprocessor tokens.)

lburg supports single dispatch architectures (such as SPARC). An enhanced version was required in order to support less conventional processor architectures, and so a highly Object Oriented C++ rewrite was undertaken using reference counting and smart pointers to share common partial instructions. The resulting program was substantially larger, due to the extra declarations for encapsulated C++ classes, rather than the original free access to structure elements, and due to the added functionality. Preprocessor macros were used extensively to factor out common declarations.

A further revision to exploit FOG without any other change to functionality forms the basis of the comparison for this example. An implementation based on the use of preprocessor macros is compared with an implementation using metafunctions, meta-variables and derivation rules.

The beneﬁt of using FOG for the 10 non-yacc modules are presented in Table 7.1.

```
The pre-FOG version comprises sources that compact .cxx and .hxx into a
module
.xx
.fog
```

% reduction lines tokens tokens

Burg 1420 8301 7681 9.3

BurgCodeScope 496 2901 2496 14.0

BurgEntry 98 383 227 40.8

BurgNonTerm 204 849 679 20.0

BurgParserValue 78 276 248 10.1

BurgRule 361 1903 1583 16.8

BurgSharedRules 86 327 171 47.7

BurgSubExpr 428 2545 2062 19.0

BurgTerm 163 651 483 25.8

BurgTree 386 2111 1821 13.7

total 3720 20247 17451 14.0

Table 7.1 Token size reduction through use of FOG

```
single .xx ﬁle. Raw (comment and blank included) line counts are presented for
these. The post-FOG version comprises a single .fog ﬁle per module. (The
counts exclude re-usable preprocess/meta-function deﬁnitions).
```

Use of (the multi-pass implementation of) FOG reduced the token count by 14%, from 20250 to 17500. The per-module reduction varied between 9% and 48%. The larger reductions occur in small classes, where the beneﬁts of derivation rules and simpliﬁcation of interface and implementation declarations are most apparent.

```
A reduction in token count is an easily measured reduction in programming effort.
Less easily measured are the more aesthetic improvements of better modularity,
improved expression of programming intent, and automatic compliance with
programming protocols. A pair of short before/after extracts are therefore provided
for readers to make their own judgements. The code is complete save for the
removal of 4 functions whose lexical structure exactly duplicates functions that
remain. Code for this example is chosen because it is shortest, and so
demonstrates the changes more clearly. Providing the large number of unaffected
function body lines from a more typical module would not provide extra insight.
The deﬁnitions of the preprocessor macros or meta-functions is not shown. The
two are of comparable lexical size, the meta-function has a higher token count
through the use of $ operators and meta-type names, but a lower token count
through the use of more appropriate facilities. The meta-functions are modular
through having fewer interdependencies than the preprocessor macros, and more
readable through the use of more conventional structuring and the elimination of
back-slash continuation lines.
The original preprocessor macros are almost completely eliminated. The
CUSTOM_RTTI support is provided automatically by derivation. The remaining 6
macros
supporting
smart
pointers
are
all
subsumed
by
MapOfSmartPointerSpecialisations.
Other
meta-functions
such
as
Mutate just implement simple idioms.
```

**Original interface ﬁle:**

```
#ifndef ENTRY_HXX
#define ENTRY_HXX
#include <Burg.h>
#include <Id.hxx>
// A smart string class
#include <Object.hxx>
#include <ReferenceCount.hxx>
#include <SmartPointer.H>
class Entry : public Object
{
CUSTOM_RTTI_WITH_1_BASE_DECLARATION(Entry, Object)
REFERENCE_COUNT_DECLARATION(Entry)
NULL_OBJECT_DECLARATION(Entry)
private:
const Burg& _burg;
const IdHandle _id;
// Handle for a smart string
private:
Entry(const Entry&);
// No copy
Entry& operator=(const Entry&);
// No assign
protected:
Entry();
Entry(Burg& aBurg, const Id& anId);
public:
const Burg& burg() const { return _burg; }
const Id& id() const { return *_id; }
virtual Term *is_term();
const Term *is_term() const
{ return ((Entry *)this)->is_term(); }
virtual void mark_reachable();
virtual ostream& print_this(ostream& s) const;
};
#endif
```

**Original implementation ﬁle**

```
#include <Entry.hxx>
#include <Burg.hxx>
#include <MapOfSmartPointer.H>
CUSTOM_RTTI_WITH_1_BASE_IMPLEMENTATION(Entry, Object)
REFERENCE_COUNT_IMPLEMENTATION(Entry)
NULL_OBJECT_IMPLEMENTATION(Entry)
SMART_POINTER_IMPLEMENTATION(Entry)
MAP_OF_SMART_POINTER_IMPLEMENTATION(Entry)
Entry::Entry()
: _burg(Burg::null_object()) {}
Entry::Entry(Burg& aBurg, const Id& anId)
: _burg(aBurg), _id(anId) { aBurg.add_entry(*this); }
Term *Entry::is_term() { return 0; }
void Entry::mark_reachable() {}
ostream& Entry::print_this(ostream& s) const { return s << _id; }
```

Revised FOG code, with use of FOG extensions italicised

```
using "Burg.fog";
// Improved form of #include.
class Entry : public Object
{
using/interface "Burg.h";// Need a #include <Burg.h>
$NoCopy();
// Section 7.1.2
$NoAssign();
// Section 7.1.2
$Mutate();
// Section 7.1.3
private:
const Burg& _burg = Burg::null_object();
const IdHandle _id;
protected:
!inline Entry() {}
// Uses default initialiser value
public:
const Burg& burg() const { return _burg; }
const Id& id() const { return *_id; }
virtual Term *is_term() { return 0; }
const Term *is_term() const { return mutate().is_term(); }
virtual void mark_reachable() {}
virtual ostream& print_this(ostream& s) const
{ return s << _id; }
};
$MapOfSmartPointerSpecialisations(Entry);
protected Entry::Entry(Burg& aBurg, const Id& anId)
: _burg(aBurg), _id(anId) { aBurg.add_entry(*this); }
```

## 7.6 Summary

We have shown a number of examples that steadily progress from apparently trivial one-liners through usage of extended declarations and on to metaprogramming. We have concluded with an example that begins to show FOG in use for a real application. The examples show that FOG can capture repeated practice well and so avoid redundant source text and the consequent maintenance risks. The ﬁnal example highlights the modest proportion of real code that is repetitive. FOG apparently does little to reduce the programming burden of straight application code.

Unfortunately more extensive usage remains an area for further work. Many more lines of code must be adapted to exploit FOG to determine how beneﬁcial FOG is. More programmers must use FOG to determine how easy FOG is to use and learn. More usage is required to stress the enhanced syntax and identify areas in need of revision. Extensive usage is needed to build up appropriate standard coding styles and meta-library support. In the same way that C++ provides many programming opportunities that were not available in C, FOG provides opportunities not available in C++. Perhaps one of these opportunities may identify a way of creating more compact abstractions for the straight application code.
