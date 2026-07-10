# 1 Introduction

Improved languages and improved methodologies jointly contribute to improved programming practice and performance. However the two contributions are not balanced. Whereas a language extension supports a better methodology, a novel methodology may demand a language change.

The advent of practical support for Object Oriented Programming in C++ has made OO extremely popular, the most forward looking, although not yet the dominant industry programming style.

Extensive use of an OO style in C++ reveals its limitations and demands improvements. An OO style results in large numbers of classes whose structure and relationships are in some way predictable, often conforming to a number of design patterns. It can be difﬁcult to capture the predictability, so that the programming intent is expressed compactly, reliably and re-usably, indeed it is sometimes difﬁcult to express the intent in more than comments.

An OO style is well suited to a program organisation based on a data-centric ﬁle structure, but some problems are better modularised from an algorithmic or Aspect Oriented perspective. C++ lacks facilities to weave a variety of contributions together to produce the required composite program.

We address these limitations and resolve them by introducing the run-time metaprogramming capabilities of languages such as CLOS and Smalltalk as compiletime capabilities in C++, without losing the fundamental run-time efﬁciency or deviating too far from the language. At the same time we replace the historical anomaly that is the C preprocessor to create a more powerful and integrated programming language.

The solutions, which are embodied in an enhanced C++ language and compiler called FOG (Flexible Object Generator), form the topic for this thesis. Implementation of FOG required the development of a new approach to parsing C++ without the use of semantic information. This work is presented in Chapter 5 so that it may read in isolation by those only interested in C++.

**Background**

C++ [C++98] is a popular, very widely used and successful industrial strength language with support for Object Orientation. The popularity of C++ is in part attributable to a very high degree of compatibility with C [C89]. Portability and runtime efﬁciency are some of C’s and consequently C++’s attractions to programmers.

Efﬁciency is achieved in C by providing programming constructs that are relatively low level. Efﬁciency is preserved in C++ by using a restrictive form of object model that enables C++ to resolve at compile-time what many other Object-Oriented languages resolve at run-time.

Programmers as well as compiler writers seek to trade run-time for compile-time activity. A programmer may improve run-time efﬁciency by identifying better algorithms, selecting a more efﬁcient compiler, or structuring code to exploit the good, or avoid the poor, characteristics of a compiler. In order to improve compiletime, there appear to be few approaches, although different coding styles and appropriate management of include ﬁle dependencies and compilation unit sizes can show surprising beneﬁts.

The inadequacies of C support for compile-time programming were recognised from the outset by the provision of Cpp (the C preprocessor). C++ has introduced more powerful language constructs but provides no assistance for programmers who need to capture predictable programming structure that does not correspond to language constructs.

**Programming style, patterns and aspects**

Dramatic improvements in programming time (and maintainability and reliability) may be achieved when an automatic code generator such as lex or yacc is applicable, or when an application generator such as a GUI builder is suitable. For many more mundane programming applications, the structure of the code is in some way predictable, but not of sufﬁcient size or complexity to justify the development of a custom code generator. For these applications, the programmer is forced into workarounds, exploiting whatever tools are available. These workarounds often require indirect or repeated expression of the programming intent, introducing:

- maintenance problems through lack of clarity

- inefﬁciencies through the need for repeated editing

- errors through inconsistent repeated editing.

Cpp was for a long time the main tool available to C and C++ programmers. And, prior to the introduction of templates, it was standard practice to use some very large preprocessor macros to deﬁne generic classes for containers. Templates now provide a powerful solution to problems that can be characterised by the requirement to deﬁne a family of types or functions. However, for many other problems, Cpp remains the only alternative. Lexical pasting using the preprocessor is inelegant and not without its problems, but it is less error prone than manual approaches.

Compatibility with C required C++ to preserve Cpp, although its limitations as a programming tool have long been recognised (and are summarised in Section 3.4). While C++ introduces a number of new constructs that eliminate some traditional uses of Cpp, other uses remained. The power and complexity of Object Orientation and the increasing use of simple patterns [Gamma95], clichés [Gil97] or idioms [Coplien92] considerably increases the need for programmers to program at compile-time and as a result Cpp is perhaps more, rather than less, important to C++ than to C.

Cpp should be replaced rather than eliminated.

The Object Oriented Programming community has recognised that groups of objects with a shared behaviour can be found within apparently dissimilar applications. These shared behaviours are classiﬁed by patterns. There is no precise deﬁnition of what constitutes a pattern, but it is generally agreed that a pattern is a solution to a recurring problem in a context. Recurrence is an important discriminant between generic patterns and candidate patterns, which may be just special purpose tricks. A pattern is used in a speciﬁc context which imposes constraints (or forces) that inﬂuence the way in which the pattern solves the problem The description of a pattern provides a range of solutions and a discussion of how the differing forces from the application context may inﬂuence the usage. The generality of the pattern concept allows the pattern form to be very widely applied: from project management through to idiomatic coding. This generality is a little frustrating since the literature lacks focus.

Analysis, design and coding patterns are of signiﬁcance to this work. Coding patterns tend to be simple program idioms that implement standard coding practices that are not directly supported by the target language. Design patterns capture the collaborations between implemented objects. Analysis patterns similarly capture collaborations, but at a more abstract level reﬂecting the higher level analysis perspective. The extent to which patterns used during analysis, design or coding appear in the implementation is a rather contentious issue. Automated implementation of patterns would seem attractive but Misconception 4 of [Vlissides98] strongly cautions against this.

From the purist perspective, compromising between the generally conﬂicting forces unique to each application requires careful selection between a wide range

of possible solutions. It is inappropriate and impossible to offer a cookbook solution to a pattern, partly because such a solution cannot offer sufﬁcient ﬂexibility. In many applications, more than one pattern is employed and where the patterns overlap, the solution must be adapted to share rather than duplicate the overlapping functionality.

From the practical perspective, an inferior set of proven cookbook solutions is often preferable to optimal handcrafted solutions. Programmers tend to implement solutions they are familiar with, rather than those that could be more optimal. If patterns have been used as part of the analysis and design phases, it seems appropriate for these patterns to ﬁnd some form of expression in the code. Otherwise, if the patterns are not expressed at all, the patterns are lost [Soukup94] and subsequent code maintenance is hampered by greater barriers to comprehension. When patterns are expressed only in the form of comments, compliance with design principles and constraints is informal; no enforcement occurs during implementation or subsequent maintenance.

The challenge is to provide cookbook solutions with sufﬁcient parameterisation to satisfy the purists, while offering adequate efﬁciency and utility for practical requirements. Enthusiastic use of templates, as practised in Generative Programming [Czarnecki97] can result in highly conﬁgurable functions and to some extent types and components. However much of the required parameterisation involves appropriate selection and conﬁguration of declarations in ways not amenable to template programming. Program conﬁguration in C++ should occur at compile-time, but the facilities of the C preprocessor are inadequate for the task and a generally inappropriate foundation for this new programming paradigm.

Compile-time programming is necessary to conﬁgure declarations.

A problem with implementing patterns is that a pattern tends to involve more than one class, and so use of a pattern requires code to be added to more than one class. A similar but larger scale problem arises in Aspect Oriented Programming [Mens97], in which an aspect is a programming concern (such as error recovery) that cuts across more than one class. AO programs organise source code according to the programming concerns and then use a weaver to combine the disparate contributions into a complete program.

Patterns and AOP require weaving.

The C++ One Deﬁnition Rule ([C++98] §3.2) mandates a single deﬁnition of each declaration, with the result that with a few exceptions (forward references, externs, typedefs, and namespaces) C++ source code must be organised to satisfy the constraint of complete declarations. It is not possible to interleave class declarations. This might seems like a desirable restriction, until it is appreciated that this prevents weaving in support of AOP or even support for a multi-class pattern solution.

The One Deﬁnition Rule must be circumvented.

**Meta-programming**

Prior to C++, Object Orientation, as then exempliﬁed by Smalltalk, was perceived to be inherently inefﬁcient because of the run-time costs associated with message dispatch. C++ introduced a more restrictive object model that enabled most of the run-time costs to be replaced by compile-time computation. As a result Object Orientation in C++ is efﬁcient and widely used. (More efﬁcient implementation approaches developed to make Smalltalk costs more acceptable are now being exploited by Java).

C++ requires that the layout of objects be frozen at compile-time, and that the (base) type of the recipient of any message is known. The layout constraint implies a single contiguous memory allocation for each object, simplifying memory

management and providing member variable access by a simple indexing operation. The messaging constraint enables static and some dynamic methods to be implemented as simple function calls. The remaining dynamic methods require a virtual function that is implemented by a single indirection from a known index into a relatively small dispatch table. These are pragmatic constraints on the object model. Elimination of run-time object ﬂexibility removed the need for runtime code to manipulate object structure, and for run-time objects to describe it. The meta-classes that are essential for languages such as Smalltalk were therefore not necessary for C++, and so they are not part of the C++ language.

It has been found that some degree of self-awareness is useful to an Object Oriented program. This may involve

- a knowledge of class names for diagnostic purposes

- availability of inheritance information as Run-Time Type Information to validate dynamic casts

- object layout information to support marshalling for communication

- object layout information for persistent storage in data bases

- full class descriptions for browsers or debuggers

The ﬁrst two of these needs have been addressed as C++ has evolved from ARM [Ellis90] to ISO standard [C++98]. Applications requiring more substantial information must resort to special purpose pre- or post-processing. Reﬂection supports this extra processing directly as part of the compilation process.

Introspection is useful for simple applications. Reﬂection is almost essential for sophisticated applications.

When a Smalltalk or CLOS program reﬂects upon itself, this necessarily happens at run-time, since this is when object structure is deﬁned. Support for reﬂection is relatively easily provided by formalising the interface to the underlying run-time language support.

In C++, objects are deﬁned at compile-time, and so an opportunity exists for a program to reﬂect upon itself at compile-time as well as, or instead of, at run-time. If the purpose of that reﬂection is just to extract some information or perform some checking in a one-off fashion, it is clearly preferable for such code to execute at compile-time. This is very much in the C++ spirit of maximising run-time performance by resolving as much program structure as possible at compile-time. C++ only optimises those constructs that form part of C++. Reﬂection supports optimisation of user deﬁned concepts.

If reﬂection is to happen continuously, then it must occur at run-time. The C++ philosophy dictates that unwanted language functionality should not impose runtime costs, so provision of run-time reﬂection must be cost free, when unused. The amount of run-time reﬂection may vary between applications, with the majority not using it at all. Some may wish to just browse data structures describing declarations. Very sophisticated applications may seek to reify1 the different stages of message dispatch to validate argument lists or call-from access rights. The required support for run-time reﬂection can be achieved by using compiletime reﬂection to create data structures and modify code to collaborate with a runtime support environment. The degree of support can be tailored to match the requirements.

It is not entirely clear that compile-time modiﬁcation of executable code is necessary for many practical applications, and so we consider only perusal and modiﬁcation of declarations. Modiﬁcation of code is nevertheless possible, provided the intended target for modiﬁcation is encapsulated within a template or inline function, which can then be modiﬁed as a declaration.

1. To make a thing, typically by creating an object to represent an abstract concept.

## 1.1 Language Limitations

The work described in this thesis grew out of a recognition that the programming effort for C++ programs increased disproportionately with the size of the program. This is not particularly unexpected. Large programs have been a consistent problem for software developers. Solutions have been found through improved languages and programming methodologies, with C++ and Object Orientation making their contributions. The concepts of patterns and Aspect Orientation offer further improvements to programming methodologies but highlight constraints imposed by C++. We therefore assess and resolve these limitations.

**Declarative Redundancy**

C++ exhibits signiﬁcant lexical redundancy.

There is duplication between many interface and implementation declarations. This requires a duplication of editing effort and provides a limited opportunity for inconsistencies to introduce errors.

In a deep inheritance hierarchy, the same virtual function may have many implementations, again requiring repeated editing effort. This effort is most noticeable when it is necessary to change the function signature: two additional edits may be needed for each derived class to realise what is a single conceptual change. More seriously, an inconsistency between declarations within the hierarchy is not necessarily an error, although a helpful compiler may choose to ﬂag the hazard.

Lexical redundancy should be eliminated.

**Algorithmic Redundancy**

Implementation of many idioms imposes a well-deﬁned protocol that must be observed by derived classes. C++ provides no mechanism for implementation of this protocol, although in some cases use of a pure virtual function may diagnose a non-implementation. Correct observance of the protocol requires implementors of derived classes to add the code manually, sometimes making use of preprocessor macros. This practice is at best tedious. It makes derivation from third party libraries unnecessarily difﬁcult because application writers have to supply extra library support code.

Predictable code should be provided automatically.

**Organisational Restrictions**

Implementation of more interesting idioms and patterns requires code injection into multiple classes. This is not supported in C++. A particular solution to a pattern requires an interface and an implementation preprocessor macro for each collaborator. These macros are typically invoked from the interface and implementation ﬁles of each class. As a result a pattern involving 3 classes may require 6 macros. These 6 macro invocations that instantiate a single concept are dispersed throughout the source code.

A concept should be instantiated by a single invocation.

For some applications, it is appropriate to partition the source code according to the data structures: algorithmic code is then naturally assigned to classes. For other applications, in particular Aspect Oriented applications, the algorithmic perspective may be more important, and so all the code for one algorithm or aspect should be kept in one source module, while that for another should be in another module. In this situation the aspect cuts across the class structure. C++

requires sequential declaration of complete classes. It is not possible for partial declarations to be interleaved.

Interleaved declarations should be allowed.

**Summary of Problems**

Cpp should be replaced rather than eliminated. Compile-time programming is necessary to conﬁgure declarations. Patterns and AOP require weaving. The One Deﬁnition Rule must be circumvented. Introspection is useful for simple applications. Reﬂection is almost essential for sophisticated applications. Lexical redundancy should be eliminated. Predictable code should be provided automatically. A concept should be instantiated by a single invocation. Interleaved declarations should be allowed.

**The FOG Solution**

These problems require revision of C++, and so in order to avoid development of a new language, or a new compiler for a modiﬁed language, revisions were implemented within a translator from extended C++ to standard C++.

The experimental translator is called the Flexible Object Generator (FOG) and the extended language is FOG. The translator

- revises C++ (upward compatibly, with no new reserved words)

- rearranges source code

- synthesizes declarations

- interprets meta-programs

The most signiﬁcant revision is the relaxation of the One Deﬁnition Rule to support composition of multiple declarations. This is a major semantic enhancement, but it is almost invisible syntactically. This revision opens the door to weaving and pattern implementation.

The facilities of the C preprocessor are essential for practical programming, but integrate very poorly with C or C++. Many of the FOG extensions provide replacements for Cpp functionality, thus meta-functions replace function-like macros, tree-literals invite a replacement avoiding the accidental substitutions characteristic of Cpp, and meta-statements support conditional compilation. Consistent generalisation of each of these concepts results in a compile-time environment in which meta-programs can be interpreted.

## 1.2 Organisation

An overview of FOG is provided in this chapter, so that the later full exposition of the FOG grammar can provide examples less constrained by forward referencing.

Some related work has already been mentioned as part of the motivation for this work. Chapter 2 contains a more extensive review and comparison.

The main description of FOG is provided with Chapter 3 covering the lexical and syntactical enhancements as the foundation for the more substantial discussions of the semantics of substitution, composition and meta-classes in Chapter 4. The changes are described one at a time in Chapter 3 interspersed with discussion. The modiﬁed grammar is therefore repeated in Appendix A for ease of comparison with Annex A of [C++98].

A context-free grammar is important for ﬂexible meta-programming, and it is wellknown that the C++ grammar is not context-free. This would appear to preclude context-free meta-programming. Chapter 5 examines existing parsing approaches, and introduces a new approach that supports context-free syntactical analysis of C++. The validity of the new approach is shown by using an extended form of regular expression to analyze the C++ grammar and deduce the ambiguities. A working yacc-able implementation of the C++ grammar alone is presented in Appendix B, with the full FOG version in Appendix C.

Operation of FOG as a translator to C++ involves a number of practical concerns regarding ﬁle locations, partitioning of declarations into ﬁles and generation of appropriate include ﬁle dependencies. These issues and a description of the algorithms required for code emission are described in Chapter 6.

A number of small examples are provided as part of the overview and enlarged upon together with a few slightly larger ones in Chapter 7.

The achievements of FOG are summarised in Chapter 8, followed by a glossary of signiﬁcant terms and acronyms and a list of all references.

The FOG command line is described in Appendix D.

The preliminary catalogue of built-in meta-functions may be found in Appendix E.

Detailed descriptions of

- discarded syntax

- parsing ambiguity resolution

- C++ ambiguities introduced by the superset grammar approach

- FOG ambiguities introduced by the superset grammar approach

- ﬁle placement and dependency syntax

are presented in Appendix F.

## 1.3 Conventions

Before we provide an overview of the FOG functionality, we must deﬁne the typographical conventions used throughout the rest of the text.

The FOG grammar is a superset of C++ and so it is necessary to make occasional reference to the C++ grammar as deﬁned by [C++98] and then summarised in its Annex A, which is very similar to Appendix A of [Stroustrup97]. In order to save the reader having to keep a copy to hand, relevant sections are included in the text and in Appendix A. Speciﬁc paragraphs are referenced as §11.4, or more commonly as §11.4-5 where the 5 denotes the numbered paragraph within the section numbered 11.4 in [C++98].

```
The BNF-like (Backus Naur Form) language of the C++ standard is used in this
document. Terminals (such as static) are distinguished by the use of a typewriter
font. Non-terminals (such as
parameter-declaration-clause) are in an italicised
seriphed font. A production (rule) comprises a non-terminal on its left-hand side
followed by a colon followed by the right-hand side. Productions that share a
common left-hand side are grouped together with one right-hand side per line.
Optional elements are denoted by an
opt sufﬁx. Comments may be supplied
following a //. Thus
base-speciﬁer:
::opt nested-name-speciﬁeropt class-name
// defaults to private
virtual access-speciﬁeropt ::opt nested-name-speciﬁeropt class-name
access-speciﬁer virtualopt ::opt nested-name-speciﬁeropt class-name
comprises the 3 rules that deﬁne the syntax of a base-speciﬁer. (The standard is a
little lax in its formatting of these productions, neglecting to use typewriter font for
the virtual keyword or the :: punctuation.)
```

In order to ease comparison of similar FOG and C++ grammar, the two are combined with a strike-through to denote C++ constructs removed in FOG, and an underline to denote FOG constructs added to C++. Thus the C++ rules

```
primary-expression:
literal
this
:: identiﬁer
:: operator-function-id
:: qualiﬁed-id
( expression )
id-expression
and the replacement FOG rules
primary-expression:
literal
this
( expression )
declarator-id
may be shown in combination as
primary-expression:
literal
this
:: identiﬁer
:: operator-function-id
:: qualiﬁed-id
( expression )
id-expression
declarator-id
When a strike-through or underline is applied to a shared left-hand side rather
than a rule, the strike-through or underline applies to all rules, but is omitted in the
interests of readability.
using-declaration:
using typenameopt ::opt nested-name-speciﬁer unqualiﬁed-id ;
using :: unqualiﬁed-id ;
Application of a strike-through to the left-hand side (deﬁnition) non-terminal
implies application of a strike-through to all references of the non-terminal as well.
C++ grammar productions are generalised in FOG, but are not given different
meanings. There is therefore no ambiguity in referring to the grammar production
for a declaration as declaration. Italics in normal text denote a non-terminal.
When it is necessary to show examples of grammar implementation rather than
speciﬁcation, typewriter font is used throughout. Non-parametric terminals are
spelled out in single or double quotes ('*' or "class"). Parametric terminals are
shown in mixed-case (StringLiteral). Non-terminals are shown in lower case
(base_specifier).
base_specifier:
"::".opt nested_name_specifier.opt class_name
|
"virtual" access_specifier.opt "::".opt nested_name_specifier.opt class_name
|
access_specifier "virtual".opt "::".opt nested_name_specifier.opt class_name
Multi-character terminals such as
"::" or
"virtual" are a non-standard
extension and are not supported by yacc or bison. They are used in the main text
for clarity. They are not used in the grammars in Appendix B or Appendix C where
an upper-case lexical token such as SCOPE or VIRTUAL is used.
```

## 1.4 Traditional Preprocessing

This overview of FOG starts by showing how the basic facilities of Cpp are replaced, using very simple examples, that are gradually reworked as more powerful facilities are described and exploited.

### 1.4.1 Lexical substitution

Lexical substitution enables common deﬁnitions to be shared, given sensible names, and factored out if alternative deﬁnitions are needed in different contexts. When used responsibly, this leads to a considerable improvement in code quality, and is one of the main reasons for the widespread use of the preprocessor. However it is very easy for unfortunate substitutions to occur, and the presence of all names from all header ﬁles in a single name space is a source of many problems.

```
C++ has removed the need for many substitutions by the introduction of initialised
consts and scoped enumerations. However, even where these are appropriate,
the need for a non-integral type may defeat C++ enhancements.
Problems with Cpp substitution stem from the single namespace and from forceful
substitution irrespective of context. Resolution of the namespace problem in FOG
will be dealt with later. The problem of over-enthusiastic substitution is resolved
by changing to a policy of substitution by invitation, rather than substitution by
imposition. In FOG, instantiation of the deﬁnition of NAME is invited by $NAME, with
the fallback of ${NAME} when subsequent characters could cause an unwanted
meaning. The increased safety incurs the cost of the trigger character(s) to invite
the substitution. These characters are not too out of place in a cryptic language
such as C. The syntax should be familiar to Unix shell or make programmers.
```

### 1.4.2 Name concatenation

Name concatenation is useful for generating a new name derived from some stem. Thus an implementation of the NullObject pattern [Martin97] may automatically deﬁne a Null class derived from its AbstractObject by sufﬁxing Null to the class name of the AbstractObject.

```
class AbstractObjectNull : public AbstractObject
{ /*...*/ };
```

This can be realised directly in FOG, where unseparated identiﬁers and literals (numbers, strings and characters) are concatenated

```
class ${ABSTRACTOBJECT}Null : public $ABSTRACTOBJECT
{ /*...*/ };
Cpp provides the ## concatenation operator that can only be used within a macro:
#define NULLOBJECT_INTERFACE(ABSTRACTOBJECT) \
class ABSTRACTOBJECT ## Null : public ABSTRACTOBJECT { /*...*/ };
```

### 1.4.3 String conversion

It is sometimes necessary, particularly for diagnostic purposes, to use a name as both an identiﬁer and a string.

```
const char *Class::class_name() const { return "Class"; }
```

This may be expressed directly in FOG, exploiting concatenation of an empty string to perform a lexical cast, since the result of a concatenation is of the same kind as the ﬁrst contribution.

```
const char *${CLASS}::class_name() const { return ""$CLASS; }
Cpp provides the # operator for use within macros.
#define CLASS_NAME_IMPLEMENTATION(CLASS) \
const char *CLASS::class_name() const { return # CLASS; }
```

### 1.4.4 Text replacement

```
The preprocessor #define directive is used to deﬁne object-like macros
#define PI 3.14159
```

and function-like macros

```
#define max(a,b) ((a) > (b) ? (a) : (b))
```

supporting usage as

```
a = max(sin(2*PI*f),0.5)
```

The replacement text is an arbitrary sequence of preprocessor tokens that are substituted without regard to context. Errors, particularly in nested deﬁnitions, are difﬁcult to diagnose, because substitution occurs before any language interpretation is applied; few compilers or debuggers support tracing back to the source once substitution has occurred. Long deﬁnitions require the use of backslashed continuation lines, which are inconvenient and unreliable to edit or read. Readability is further impaired by the need to use parentheses to guard against the possibility of accidental association problems.

```
FOG provides a meta-level where conventional run-time concepts can be used at
(meta-)compile time. Meta-variables replace object-like macros and meta-
functions replace function-like macros. Meta-variables and meta-functions are
declared and typed in a very similar way to normal C++ variables and functions,
save for the new use of the auto keyword and the introduction of meta-types:
auto double PI = 3.14159;
// Meta-variable
auto expression max(expression a, expression b)
// Meta-function
{
$a > $b ? $a : $b;
}
```

for use as

```
a = $max(sin(2*$PI*f),0.5)
The auto keyword is almost totally obsolete in C++, where auto is only permitted
within functions. auto is reused outside of functions in FOG to declare meta-
functionality. Readers may choose to pronounce auto as meta, throughout this
thesis.
The meta-types correspond to the basic kinds of token (identifier, string and
character), the numeric types (bool, double, int and etc.) and also to
productions such as declaration and expression from the C++ grammar.
```

Use of meta-types enables the parser to ensure that arguments are passed and returned compatibly, and to diagnose errors more helpfully. When appropriate, conversions between the basic kinds are performed automatically.

Substitution within the meta-function replaces each invocation by its corresponding argument expression.

```
The simple meta-function implementation of max solves the parenthesis problem,
works for arbitrary types but remains prone to side effects. The invocation
$max(a++, b++)
```

will result in one argument receiving a double increment just as in Cpp.

### 1.4.5 Conditional compilation

Conditional compilation is essential to support a variety of conﬁguration options, often to resolve distinctions between different operating systems. It may be appropriate to deﬁne

```
static const char *temp_path = "/tmp/";
```

for use under Unix whereas NT might require

```
static const char *temp_path = "C:\\Temp\\";
FOG elevates C++ run-time statements such as if ... else ... for use at the
meta-level, so that the selection may be made using an apparently conventional
test:
auto if ($UNIX)
static const char *temp_path = "/tmp/";
else
static const char *temp_path = "C:\\Temp\\";
```

Cpp provides line-oriented conditional directives that mark-up rather than form part of the source text:

```
#if defined(UNIX)
static const char *temp_path = "/tmp/";
#else
static const char *temp_path = "C:\\Temp\\";
#endif
C++ statements occur only within functions. The use of the auto preﬁx in FOG is
therefore redundant in the above example. However since the preﬁx makes meta-
code easier to distinguish, the preﬁx will be used throughout this thesis.
```

## 1.5 Object-Oriented preprocessing

The facilities described above provide consistent replacement for Cpp behaviour. Most of the extensions could be regarded as extensions to C rather than C++. Reviewing and generalising the facilities within the context of C++ leads to a much more powerful programming environment in which predictable program structures can be coded effectively.

### 1.5.1 Scopes

Meta-variables and meta-functions may be scoped and inherited, and metastatements may occur within declaration scopes.

Revisiting the conditional compilation example of Section 1.4.5 from an Object- Oriented perspective, we ﬁnd no need for conditional compilation. The characteristics of each conﬁguration option may be packaged as meta-variables (and meta-functions) of a (meta-)class.

```
class OsTraits_Abstract
{
auto bool NT = false;
// default value
auto bool UNIX = false;
//...
};
class OsTraits_NT : public OsTraits_Abstract
// derived class
{
auto bool NT = true;
// overriding value
auto string temp_path = "C:\\Temp\\";
//...
};
class OsTraits_Unix : public OsTraits_Abstract
{
auto bool UNIX = true;
auto string temp_path = "/tmp/";
//...
};
OsTraits_NT may be conﬁgured as the implementation of
OsTraits, by
specifying the value of OS on the FOG command line
fog ... -D OS=NT ...
```

and using the built-in meta-function

```
auto string std::get_cpp(string macroName)
```

to access it from

```
class OsTraits : public OsTraits_$std::get_cpp("OS") {};
```

thereby creating the equivalent declaration

```
class OsTraits : public OsTraits_NT {};
This maps the required conﬁguration to OsTraits, so that an operating system
speciﬁc ﬁle may be deﬁned using the temporary path by
const char *fileName = $OsTraits::temp_path "results.dat";
```

This is then resolved at compile-time to

```
const char *fileName = "C:\\Temp\\results.dat";
```

Having isolated the conﬁguration in separate classes and an associated header ﬁle, a new operating system can be supported by providing a preﬁx ﬁle characterising the new system and invoking it with an appropriate command line. Existing source ﬁles need no change. This could be achieved directly using multiple layers of name substitutions with C preprocessor, but it never is. Modularization is much easier when supported by the programming environment. This cannot be achieved using C++ templates, which lack the ability to perform string manipulations.

### 1.5.2 Joint interface and implementation

```
Introduction of a meta-compiler that synthesises interface and implementation
ﬁles eliminates the need for independent interface and implementation
declarations. It is appropriate to generalise C++ declarations to remove the
distinction between interface-speciﬁc and implementation-speciﬁc declarations.
This generalisation turns out to be almost entirely semantic, since the C++
grammar already permits an interface-speciﬁc keyword such as virtual to
accompany a function-deﬁnition. It is only necessary to allow an access-speciﬁer (e.g.
protected) as part of a decl-speciﬁer (the type part of a declaration), and to permit
a full id-expression (e.g. Scope::name) where previously only an identiﬁer was
allowed.
```

Programmers may then use an implementation style of declaration for parts of interfaces

```
public typedef size_t Class::SizeType;
```

or provide complete implementations in interfaces:

```
class Class
{
protected virtual void f(int x = 0) = 0 { std::cout << x; }
public:
static double y = 0;
};
A complete solution to the class_name() example from Section 1.4.3 may now be
captured by the single meta-function
auto declaration ClassName()
{
public virtual !inline const char *class_name() const
{ return ""$Scope; }
};
```

which can be invoked as

```
class NamedClass
{
$ClassName();
};
The reserved meta-variable Scope refers to the prevailing scope, avoiding the
need to pass it as a parameter.
The negated keyword !inline ensures that the function body is not inlined.
Similarly !static would provide for explicit rather than default programming
intent.
```

The single meta-function invocation generates the equivalent C++ interface

```
class NamedClass
{
/* ... */
public:
virtual const char *class_name() const;
};
```

and implementation

```
const char *NamedClass::class_name() const { return "NamedClass"; }
```

This requires a pair of macros when implemented using Cpp.

```
#define CLASS_NAME_INTERFACE() \
virtual const char *class_name() const;
#define CLASS_NAME_IMPLEMENTATION(CLASS) \
const char *CLASS::class_name() const { return # CLASS; }
```

and a corresponding pair of invocations one from the interface

```
CLASS_NAME_INTERFACE()
```

and one from the implementation

```
CLASS_NAME_IMPLEMENTATION(NamedClass)
```

### 1.5.3 Composition

In C++, multiple declarations are an error. In FOG, multiple compatible declarations are composed; only incompatible declarations are an error. A brief summary of the composition rule is given here. A full exposition is provided in Section 4.4.

```
Composed declarations merge their components, and so a variable qualiﬁed with
static carries the static with it when merged with another variable that has no
static speciﬁcation, but provokes an error message if merged with a !static.
```

Overloaded function declarations compose independently. Default values may be repeated but may not conﬂict.

Arrays and enumerations extend to accommodate all contributions. Duplicate initialisations must match. Holes in arrays are zero ﬁlled. The GNU C [Stallman98] extension is supported so that sparse arrays can be deﬁned and composed.

```
bool is_prime[] = { [2] true, true, [5] true, [7] true, [11] true };
```

The constructor initialisers for a particular constructor are composed and must not conﬂict. Unspeciﬁed initialisers for non-copy constructors are obtained from member variable initialisers. For example, code to support an error handling aspect may add a member variable with a default initializer:

```
public bool Class::_error_generated = false;
```

A constructor independently added in support of some other aspect

```
Class::Class(PersistenceManager&) /*...*/;
```

then provides the requisite initialisation.

Classes expand to encompass all distinct member declarations, with repeated declarations composed recursively.

```
Function (and constructor) bodies are composed by concatenating code
contributions within named regions, which are in turn concatenated to form the
overall function body. The regions named entry and exit typically provide for
variable declaration and initialisation and a return statement, ensuring a
predictable structure. Regions named pre and post provide code to operate
before or after the default body region of the function. Function deﬁnitions are
extended to support a declarative scope within which regions are preﬁxed by their
name.
public bool Manager::do_it()
:{
// Start of declarative scope
entry { bool exitStatus = true; };
exit { return exitStatus; };
};
```

deﬁnes a framework for a composed function. A return variable is initialised in the entry region, and returned by the exit region. With the framework in place, code concerned with a particular aspect may contribute code to the function:

```
private Aspect Manager::_aspect;
public bool Manager::do_it()
{
if (!_aspect.do_it())
exitStatus = false;
}
```

FOG weaves the contributions together to produce the equivalent C++ declarations:

```
class Manager
{
private:
Aspect _aspect;
public:
bool do_it();
};
bool Manager::do_it()
{
bool exitStatus = true;
if (!_aspect.do_it())
exitStatus = false;
return exitStatus;
}
```

Readers who have programmed extensively with a macro assembler may recognise that the ability to extend classes, function code regions, enumerations and arrays at will gives each declaration space the attributes of a program section.

It is possible to deﬁne a meta-function that performs extension of an enumeration and a text array so that numeric and text declarations are automatically synchronised.

```
auto declaration NamedEnum(identifier aName)
{
public enum Enum { $aName };
public static const char *names[] = { ""$aName };
}
```

Invocation as

```
class Colours
{
$NamedEnum(RED);
$NamedEnum(GREEN);
$NamedEnum(BLUE);
};
provides successive entries for Colours::Enum and corresponding entries for
Colours::names[], as if the user had typed:
class Colours
{
public:
enum Enum { RED, GREEN, BLUE };
static const char *names[];
};
```

and

```
const char *Colours::names[] = { "RED", "GREEN", "BLUE" };
The conversion of a single name such as RED into multiple interleaved declarations
cannot generally be achieved using the preprocessor or C++ templates.
```

### 1.5.4 Derivation rules

```
There are many idioms that require entirely predictable code to be provided by
derived classes in order to comply with a protocol deﬁned by a base class. The
class_name() method of Section 1.5.2 provides one example. In C++, a
declaration applies to the scope for which it is speciﬁed. In FOG, this scope is
referred to as the root scope for that potential declaration. An optional derivation
rule speciﬁes how that potential declaration may be automatically redeﬁned in the
inheritance tree of scopes that derive from the root scope to contribute to a
number of actual declarations. Reﬁning the example from Section 1.5.2:
auto declaration ClassName()
{
public virtual !inline const char *class_name() const
:{
derived(true) { return ""@Scope; };
};
};
A declarative scope has been introduced by the :{ ... } to preﬁx a derivation
rule to the function body. The predicate of derived(true) is always true and so
the potential declaration is applied throughout the entire inheritance tree, that is
at the root scope and all derived scopes.
The change of substitution operator from $ to @, changes the evaluation time. $ is
an early substitution operator, evaluated when source tokens are ﬁrst parsed to
create a potential declaration in its associated root scope, at which point Scope
resolves to the root scope. @ is a late substitution operator, evaluated when a
potential declaration becomes an actual declaration in its eventual scope, at which
point Scope resolves to the actual scope. (If the $ operator were used in the
example, all derived scopes would return the name of the root scope.)
```

Derivation rules can apply to the declaration of any entity. Michael Tiemann provided a solution [Stroustrup94] to the problem of providing a mnemonic name for the primary base class

```
class foreman : public employee {
typedef employee inherited;
//...
void print();
};
class manager : public foreman {
typedef foreman inherited;
//...
void print();
};
enabling a derived class to refer to its base class mnemonically as inherited
rather than explicitly.
void manager::print()
{
inherited::print();
//...
}
```

In FOG, the entire hierarchy of typedefs can be expressed by a single declaration.

```
private typedef @Super employee::inherited
:{ derived(!is_root()); };
This provides a typedef for all derived classes. The derivation predicate inhibits
the declaration at the root, where the built-in meta-variable Super may have no
valid resolution for the primary base class.
The Prototype pattern [Gamma95], virtual constructor, or cloning idiom
[Stroustrup97] is also provided very easily using a derivation rule. The
conventional approach requires that a clone method be deﬁned for every non-
abstract class in an inheritance hierarchy
class ConcreteClass /* ... */
{
/* ... */
virtual RootClass *clone() const;
};
RootClass *ConcreteClass::clone() const
{ return new ConcreteClass(*this); }
```

This requires the programmer to manually weave the code in to every class. This is potentially error prone and costs at least one line per interface and one line per implementation ﬁle of every class. Using FOG, the idiom can be fully deﬁned by a meta-function:

```
auto declaration Prototype()
{
public virtual $Scope2 *clone() const = 0
:{
derived(!Scope.is_pure()) { return new @{Scope}(*this); };
};
}
The
!Scope.is_pure() derivation predicate speciﬁes that the declaration
contributes code to all derived classes that have no pure virtual functions.
```

Instantiation requires a single line in the base class that deﬁnes the protocol. No code is required in derived classes.

```
class Base
{
$Prototype();
};
```

These two examples demonstrate FOG at its most advantageous: one line in the base class guarantees protocol observance and replaces one or more lines in each derived class.

```
2. $Scope may be changed to @Scope in the above example to use the derived type
as the return type.
```

### 1.5.5 Compilation model

C++ supports a two stage translation process involving multiple independent compilations followed by a link editing stage to produce a ﬁnal executable. The independent compilations are consistent provided the One Deﬁnition Rule (§3.2) is observed. Simply stated, this rule requires that a declaration in one compilation must not have a different meaning in any other. In practice, placing declarations in header (interface) ﬁles, which are included by each compilation session that requires them, usually satisﬁes the One Deﬁnition Rule.

From the perspective of a compiler writer, the One Deﬁnition Rule is very useful, if not essential. From the perspective of the programmer, the One Deﬁnition Rule is very inconvenient. Declarations must be provided twice, once in the interface ﬁle and again in the implementation ﬁle. Declarations cannot be freely interleaved.

In more serious applications, a conﬂict arises between language constraints and the programmer's need to organise code to suit algorithmic or functional perspectives. Code has to be organised to suit the compiler. Patterns cannot be preserved in the code [Soukup94] and Aspect-Oriented Programming [Kiczales97] is not readily supported.

A preprocessor for C++, that performs its processing prior to compilation, can bridge the gap between the organisational requirements of the programmer and the integrity requirements of the compiler. FOG operates in this way using an augmented compilation model as shown in Figure 1.1.

The centre and right hand sides show the conventional C++ compilation model. Interface ﬁles provide the declarations to be shared by independent compilations, which produce object ﬁles to be linked together with libraries to produce an executable. (The complexities of static construction and template instantiation are conveniently hidden by the ‘Linker2’ activity.) Meta-compilation adds the extra stages on the left hand side. The conventional C++ interface and implementation ﬁles are generated by one or more meta-compilations from source ﬁles (the forward arrows) and from frozen interfaces (the reverse arrows). Sources may be shared between meta-compilations, and a single meta-compilation may generate any number of interfaces and/or implementations.

Clearly the One Deﬁnition Rule must still be respected by the interface and implementation ﬁles fed to the compiler. However a more relaxed Composite Deﬁnition Rule can now be imposed on the source ﬁles. Simply stated, the composite meaning of all like declarations must be the same in each metacompilation. The composite meaning is explained in Section 4.4.

Meta-compilation Linking

Implementation

Executable Linker

Object

Object

Library

Compiler

Compiler

Interface

Implementation

Interface

Linker2

Meta- Compiler

Meta- Compiler

Source

Source

Source

**Figure 1.1 Meta-compilation model**

Library Interface

Object

Compilation

### 1.5.6 Meta-concepts

Compile-time programming is often referred to as meta-programming, and data structures describing data are often referred to as meta-data. In particular, classes are described by meta-classes. We therefore refer to FOG as a metacompiler, and generally use the meta- preﬁx to refer to a conventional run-time C++ concept elevated to compile-time, or more precisely the new meta-compiletime that precedes conventional compile-time. It is however often convenient to loosely refer to meta-compile-time and compile-time together as just compiletime.

Thus meta-classes describe classes, and meta-inherit from base meta-classes: classes which are in turn the meta-classes of the corresponding base classes. A meta-class has meta-members which are meta-functions and meta-variables with static and non-static variants. Statements that are interpreted at meta-compiletime are meta-statements. Declarations for use at meta-compile-time are metadeclarations. The type system available for use at meta-compile-time comprises meta-types.

Meta-compilation involves a two stage translation from FOG source text declarations to C++ declarations suitable for emission. Source declarations are ﬁrst converted into potential declarations (or speciﬁers) at which point the eventual scope into which the declaration contributes may be undetermined. Once the scope is determined, the potential declaration is installed as an actual declaration of the determined scope.

Meta-programs run within the compiler, and so it is more accurate to say that the meta-program is interpreted rather than executed. Of course a sophisticated meta-compiler could compile the meta-program and then (meta-)execute it. Metaprogramming may operate on potential or actual declarations.

### 1.5.7 Meta-Programming

FOG supports static meta-programming, that is the execution of user supplied code at meta-compile-time. This code may analyze and modify declarations in order to ensure compliance with some constraint.

This may just involve monitoring declarations to ensure that some policy is observed. For instance a meta-program could verify the presence of a virtual destructor under all or selected conditions.

More powerfully, a meta-program may be used to generate code automatically to support run-time access to compile-time information. The generated code can be optimised to suit user requirements:

- a very modest form comparable to RTTI for simple debugging

- a more extensive set of tables or functions to support persistence or marshalling

- a very substantial set of tables to support full run-time meta-programming supported by a corresponding run-time executive

The facilities provided by FOG are very much focused on meta-programming as a manipulation of program declarations. This contrasts with but complements the manipulation of types, functions and expressions using template metaprogramming.

## 1.6 FOG versions

The FOG grammar is a superset of the C++ grammar, with extensions to support compile-time programming. FOG is a translator from extended C++ to standard C++. The extensions are sufﬁcient to render the C preprocessor redundant, although Cpp continues to be supported for compatibility.

It was originally thought that the goals of FOG could be achieved by relatively simple processing at a lexical level. This proved to be naive. Useful manipulation of C++ declarations requires accurate parsing of those declarations and therefore a fully ﬂedged C++ parser is needed. C++ is context dependent, but ﬂexible metaprogramming requires source declarations to be interpreted before their context is known. A new approach is therefore introduced so that FOG can perform contextfree syntactical analysis of C++. The evolution to the current implementation is described in Chapter 5.

**Superset Implementation (version 2)**

The description of the FOG meta-compiler in this thesis applies mainly to the current implementation that uses a superset grammar to support context-free parsing of FOG (and C++) source. The lexical and syntactical analyses of this meta-compiler are substantially complete. The semantic analysis and subsequent stages are fairly complete, and more than sufﬁcient to demonstrate the soundness of the approach using simple examples. Missing functionality is mostly at the edge of the language and so concerns exceptions, partial template specialisations, namespace-aliases and using-directives.

In some areas, the examination and resolution of design decisions goes some way beyond the implementation.

**Multi-pass Implementation (version 1)**

The previous implementation resolved syntactical ambiguities using multiple passes. Ambiguities in meta-constructs were avoided by using a syntax-driven and consequently context-dependent method. This implementation suffered from deﬁciencies inherent in the syntax-driven method.

FOG and associated publications are available from the FOG home page:

http://www.computing.surrey.ac.uk/research/dsrg/fog
