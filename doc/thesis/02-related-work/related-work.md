# 2 Related Work

## 2.1 Language Constructs

### 2.1.1 Syntax

Stroustrup has highlighted the inadequacies of the C preprocessor in [Stroustrup97], where he calls for its eventual demise. Very little work has been published on practical alternatives. Straightforward lexical alternatives such as m4 suffer from many of the same problems by operating independently of the underlying language. Operation in collaboration with C++ involves tackling the challenge of C++ syntax, which is difﬁcult to parse and difﬁcult to extend. It is hard to add new syntax to C++ and so the limited extensions available through metafunctions and meta-variables have an inevitably inferior appearance to a solution that introduces new keywords. Werther [Werther96] provides a sensible proposal for a completely new C++ syntax using more conventional syntactical styles like Ada or Pascal. Within a clean syntactical framework, it would be much easier for researchers to examine alternative syntaxes, and it would be possible for a metaprogram to perform syntax extension.

### 2.1.2 Macros

Macros have a very long history, much of it rather old since macros were important to augment early ‘high’ level languages. Macros remain essential for assembler programming. [Solntseff74] provides a survey of 22 extensible languages, classifying them as Type A to G according to whether language extension is performed during

- lexical analysis

- syntactic analysis

- parse tree production (semantic analysis)

- intermediate analysis (tree optimisation)

- code generation

- code conversion

Consideration of code conversion was then purely hypothetical but foreshadowed Java load-time activities. It is difﬁcult to classify FOG, where substitution occurs as trees are manipulated during syntactic rather than semantic analysis. Re-use of parse trees corresponds to a Type C extension, but operation during syntactic analysis is Type B.

A different classiﬁcation of just macros rather than language extension mechanisms is made by [Cheatham66]. Extension may occur

- preceding lexical analysis - text macros

- during syntactic analysis - syntactic macros

- following syntactic analysis - computational macros

Text macros correspond to C preprocessor macros, and computational macros correspond to C++ inline functions and templates. It is syntactic macros that are missing from C++. Since there is no distinction between syntactic and semantic analysis, FOG substitution and prototypes fall tidily into the syntactic macro classiﬁcation.

An easier to understand classiﬁcation is based upon the structure of the replacement. [Weise93] identiﬁes

- character-level substitution - text macros

- token-level substitution

- syntax-level substitution - syntax macros

- semantic-level substitution

Token-level substitution as practised by the ANSI C preprocessor occurs between lexical analysis and syntactic analysis. (Or possibly as a late phase of lexical analysis as in Figure 5.4.) This does not quite ﬁt the earlier classiﬁcations.

Semantic-level substitution is used by [Maddox89] to classify semanticallysensitive macros; problems distinguishing between the use of a name from a deﬁnition or invocation context are resolved by passing environments as parameters. This difﬁculty is also addressed by [Hieb92] where problems of losing source context are addressed by hygienic macros.

```
At the usage level, the situation is rather different. in FOG. ‘Macro’ operation is
occurring at (meta-)compile-time rather than run-time in a language that supports
hierarchical naming through classes and namespaces. The problems of resolution
in the correct environment are addressed by appropriate use of @ or $’s as the
substitution operator, and if necessary by passing an appropriate metaobject as a
parameter.
At the implementation level, the problems and solutions are very similar in FOG;
an abstract FogScopeContext class is the root of a hierarchy of some 60 odd
environment deﬁning classes that are passed between compilation routines
(These classes make extensive use of the Decorator pattern to support addition
of behaviour to pre-existing behaviour. The instances form a daisy chain of stack
based objects, making for efﬁcient creation and easy traversal to resolve problems
such as multiple-dollar look-ups.) Source line tracing presents no signiﬁcant
problems for FOG.
```

The syntactic macro was introduced by [Cheatham66] and [Leavenworth66]. The macro and its arguments have syntactic types corresponding to parts of the language grammar. [Vidart74] gave these concepts a sound and efﬁcient foundation using an Abstract Syntax Tree (AST) to represent them and avoid repeated syntactical analysis of source text.

```
[Weise93] applies these ideas to ANSI C and exploits a Lisp-like backquote to
support a pattern template for substitution, avoiding the need for extensive call
trees to rearrange the AST. Weise’s approach is very much an extension to ANSI
C introducing new keywords and 9 lexical operators. The approach in FOG is in
some ways very similar to Weise’s, however by giving existing concepts a compile-
time meaning, and retaining a degree of consistency for all the corollaries, FOG
achieves a notation that is more compact, supports character- or syntax-based
substitution with only two new lexical operators ($ and @) and no new reserved
words. Where Weise needs backquotes and an explicit return statement to
activate source-like declarations, FOG just treats all declarations as the return. In
FOG, all concepts are put into a C++ Object Oriented perspective.
```

### 2.1.3 Joins

Relatively well-behaved coarse-grained merging of application functionality is provided by functions. More ﬂexible but ill-disciplined merging is provided by macros. The BETA fragment system [Knudsen99] provides a ﬁner-grained composition that observes predictable semantics for code fragments that satisfy the attributes of pre-declared slots.

### 2.1.4 Meta-classes

The concepts of meta-classes were ﬁrst deﬁned for Smalltalk. Languages such as CLOS have been extended with a MOP (MetaObject Protocol) [Kiczales91]. Even Java [Gosling97] has a class type for every class. C++ has rather lagged behind, perhaps through a mismatch of the run-time characteristics of traditional MOPs and the statically compiled philosophy of C++, perhaps through the compiler writer’s desire to prevent further explosion of language complexity. FOG provides statically compiled meta-functionality, which can be used to deﬁne customised run-time meta-functionality.

Meta-classes were ﬁrst introduced to support the conﬁguration of objects at runtime in Smalltalk, and have subsequently become an important part of most Object Oriented languages. Limitations of the Smalltalk implementation led to the development of the simpler Deltatalk [Borning87]. The problems are resolved in the pure object model of ObjVlisp [Cointe87]. However, [Maes87] argues that the pure object model fails to distinguish the meta-level adequately. The restricted object model in FOG does precisely this; only metaobjects exist at compile-time, and only real objects at run-time.

A more versatile object model allowing the inheritance of meta-classes to differ from their classes is supported by CLOS and SOM. [Graube89] identiﬁed the resulting compatibility problems that arise during traversal of parts of the cycle: class, derived-class, derived-meta-class, meta-class, class. The history of these problems and their solutions in SOM 1, 2, and 2.1 are described in [Danforth94]. [Bouraqadi-Saâdani98] synthesises additional meta-classes with multiple inheritance to resolve the problems. These problems do not arise in FOG since the two inheritance hierarchies are the same. They also do not arise since there is no object creation at meta-compile time and so no level traversal.

[Briot89] explains the motivation for distinct meta-class inheritance as an ad hoc solution to the propagation problem whereby a concrete class inherits the inappropriate property of abstractness from its abstract base class. This problem does not arise in FOG, since C++ offers at least two distinct solutions to the original problem using pure virtual functions or protected constructors.

The lack of meta-classes has always been a deﬁciency of C++, for which various proposals were suggested during standardisation. Eventually the standardisation committee compromised on the relatively limited functionality known as Run-Time Type Information (RTTI). A more substantive proposal [Buschmann92], is largely proprietary and so it is difﬁcult to assess accurately. It deﬁnes a run-time Meta- Information Protocol providing more extensive data structures with global functions to support iteration. FOG provides a compile-time meta-level, in which application meta-programs may be used to create whatever run-time data structures are appropriate. These may vary from just class names to large descriptive tables for use by a run-time environment that supports run-time metaprogramming.

## 2.2 Meta-level and Reﬂection

A procedure that manipulates declarations at compile-time might seem to be a simple generalisation of a macro, however harnessing the increased power, provided by this form of self-modifying code, offers ample scope for some fairly difﬁcult papers.

### 2.2.1 The tower

[Smith84] coined the term reﬂective for a program that is self-aware. He introduces a minor Lisp variant 2-Lisp that subsumes Scheme, but provides a semantically rationalised notion of evaluation. The 2-Lisp dialect forms a sound

foundation upon which the reﬂective 3-Lisp dialect is realised. Further application dialects can then be deﬁned recursively to form the ‘reﬂective tower’.

The use of a different language dialect at each level of the tower presents challenges to analysis. [Wand88] describes the behaviour without using reﬂection. Multiple dialects are well-understood in theory, but a major inconvenience in practice. The difﬁculties are overlooked by many implementations, leading to the problem of meta-circularity when more than one meta-program operates upon the same program. Which meta-program operates ﬁrst? Does the second metaprogram operate on the source or the results of the ﬁrst meta-program? How does a meta-program behave when it changes its own declarations? [Chiba96] notes that each level of the reﬂective tower must exist to deﬁne an ordering, so that one meta-program operates consistently on the results of a more nested metaprogram and is isolated from its own and less nested meta-programs.

### 2.2.2 Metaobject Protocols

A reﬂective program manipulates the metaobjects that describe the program. The programming interface to these objects is deﬁned by a MetaObject Protocol such as that provided for CLOS by [Kiczales91].

Each metaobject represents the reiﬁcation of some programming concept, and so different reﬂective languages support very different MOPs. This may be as minimal as the RTTI facilities of C++, or as substantial as the meta-computation protocol of [Sobel96].

### 2.2.3 Languages

OpenC++ version 1 [Chiba93] provided a run-time MOP for C++, through the recognition of comment mark-ups requesting indirection of method calls through metaobjects. This approach was abandoned in favour of a largely compile-time MOP in OpenC++ version 2 [Chiba95], using a two-stage compilation process, ﬁrst to build an enhanced compiler for the extended language and then to use that compiler. The two stage process avoids the costs of interpreted meta-execution, but unfortunately prevents the realisation of the reﬂective tower. Although OpenC++ extends most C++ concepts to the meta-level, somehow the language doesn’t feel like C++; the extensions are rather haphazard, and much of the metaprogramming involves considerable insight into the operation of a compiler. The working representation is a rather strange hybrid between a Lisp-like list and a C++ syntax tree in which punctuation remains signiﬁcant. This is perhaps attributable to the development path through S++, a form of Scheme supporting C++ concepts.

OpenC++ claims to be based on the principles of the CLOS MOP [Kiczales91], but C++ concepts are so different that it is difﬁcult to see any resemblance. The CLOS MOP was developed for run-time support, and is a natural formalisation of an API that is present anyway. Lists are well-supported by CLOS and so while the list manipulations involved in meta-programming may be difﬁcult for a C++ programmer to understand, they are consistent and compact. When these concepts are transferred to C++, the alien nature of list processing, the consequent lack of language support and the very different C++ perspectives make for an uncomfortable programming environment.

```
MPC++ [Ishikawa96a] provides a compile-time meta-level that like OpenC++
supports fairly extensive interception of compiler activities and subsequent peek
and poke meta-programming, using conventional C++ syntax shifted to the meta-
level by $meta. MPC++ avoids the problems of the tower by supporting a stack of
output streams, but since these are text rather than syntax-based, they lack the
integrity of composition of declarations, statement nesting or character
concatenation in FOG.
```

OpenC++ and MPC++ share the same principle of operation. A meta-program intercepts stages in the compilation process, recognises patterns1 in the code, and modiﬁes the syntax tree by construction of alternate partial trees. The FOG approach is completely different. The pattern is explicitly identiﬁed by the invocation of some declaration (often a meta-function) that implements the pattern. Additional declarations may be composed to elaborate the implementation for more sophisticated requirements.

The peek and poke approach is able to achieve most programming goals, but requires the programming skills to peek and poke syntax trees and poses the challenge of recognising all variants of the target pattern. The declarative approach in FOG is less ﬂexible, but only requires relatively conventional skills and may need the interception points to be explicitly identiﬁed, albeit by inline function calls. In FOG two declarations are woven by providing the two declarations. The peek and poke approach requires a program to be invoked, the join point to be identiﬁed in one tree, and the tree for the other declaration to be hand assembled and merged.

The ease with which OpenC++, MPC++, Sina/St (a precursor to C++/CF) and D (a precursor to AspectJ) could be used is discussed in [Lin99]. All are found lacking for a three-level architecture and so Adapter++ is presented. The problem would appear to be directly soluble with FOG.

Iguana [Gowing96] provides a run-time MOP in which many different activities can be reiﬁed on a per-object, per-method or per-class basis. The implementation cost is therefore determined by the required degree of reiﬁcation. This provides considerable ﬂexibility at run-time but offers little at compile-time, since Iguana is not a reﬂective complier. [McAffer95] takes reiﬁcation even further with seven components to a message invocation.

Meta-classes form part of the Java language deﬁnition, and so there is more language support for reﬂection and the interesting opportunity for a user-deﬁned class loader to perform meta-programming at load-time. Dalang [Welch98] and Kava [Welch99] provide an ability to intercept method calls. Guaraná [Oliva98] introduces composer metaobjects to enforce composition policies.

OpenJava [Chiba98a] migrates the peek and poke concepts of OpenC++ to Java, providing a way of implementing parameterised classes [Chiba98b] that is rather at odds with more serious language proposals such as GJ [Bracha98], NextGen [Cartwright98] or [Agesen97]. Java Beans are used as the basis for a two-layer meta-level model by [Wu98], and by [Lorenz98] to provide a reﬂective implementation of the Visitor pattern.

### 2.2.4 Applications

Reﬂection has been used to solve problems in a wide variety of applications.

[Cartwright98] reports on the successful use of OpenC++ to simplify and enforce the interface to an AI library, but only after ﬂattening their meta-program to avoid OpenC++ restrictions.

[Kasbekar98] again using OpenC++, identiﬁes run-time data-dependencies so that a roll-back to a checkpoint can be done efﬁciently.

[Yokote92] describes the reﬂective Operating System Apertos.

Further applications involving aspects, communication, constraints, distribution, patterns, persistence and synchronisation are discussed elsewhere.

1. Not a design pattern, although the FOG invocation is design pattern-like.

## 2.3 Programming Styles

### 2.3.1 Patterns

The original GoF patterns book [Gamma95] has provoked considerable interest and a growing number of specialised conferences and workshops. Example implementations of the patterns were mostly in C++ but just occasionally in Smalltalk, so translations exist for Java [Grand98] and Smalltalk [Alpert98]. The POSA book [Buschmann96] provides further architectural and design patterns.

Unfortunately, from a programmer’s perspective, the ability to present any problem as a pattern has broadened the ﬁeld so that the additional coding patterns contributions provided by the PLOPD conferences [Coplien95b], [Vlissides96], [Martin97] and [Harrison99] are diluted by management and organisational patterns. However, remaining within the programming domain, [Fowler97] covers the rather different perspective required during analysis.

Patterns are very abstract and so pose considerable classiﬁcation challenges. A pattern is often named rather arbitrarily by its author, and so looking for a pattern that concerns a particular kind of problem is hampered by the lack of clear vocabulary for problem or solution. Two attempts at classifying the basic patterns have been made. [Gil97] distinguishes between clichés (straightforward use of prevalent mechanisms), idioms (a built-in facility in some languages) and cadets (not built-in to any language). [Agerbo98] takes a harsher view of just the GoF patterns, discarding 2 as not patterns at all, 7 as Language-Dependent Design Patterns, and 2 as closely Related Design Patterns: only 12 survive to be classiﬁed as Fundamental Design Patterns.

A much more fundamental perspective on patterns is taken by [Pree94], where all collaborations are reduced to seven meta-patterns corresponding to different forms of 1:1 and 1:N relationship. Pree claims a high-level perspective, which is surely wrong: a complete set of object to object relationships is a suitable lowlevel abstraction that can form one of the layers for a parameterised pattern. The GenVoca approach [Batory97] with its cascade of small orthogonal behaviours would appear to combine well with Pree’s meta-patterns, once a suitable tool, such as FOG, is available to glue the behaviours together.

Most attempts to represent patterns in code are informal. Soukup addresses the problems of implementing patterns, with [Soukup95] summarising the much more extensive treatment in [Soukup94]. Soukup’s solution, supported by the CodeFarms library, realises each pattern as a data-less class that is declared as a friend of each collaborator. Pattern operations are realised by static member functions of the pattern class, so the programming interface is unnatural: operations are invoked upon the pattern, not the primary collaborator. However, the behaviour is very regular and has extremely good characteristics with respect to include ﬁle dependencies. The implementation is hampered by the same limitations that motivated the removal of the One Deﬁnition Rule from C++: a special preprocessor has to be used, which could beneﬁcially be replaced by FOG.

An alternative text-based mechanism is provided by SNIP [Wild96], although the enhancements effectively introduce two new languages to implement a kind of marked up source text with a rather accidental semantics for lexical composition.

A reﬂective implementation of two patterns using OpenJava is described in [Tatsubori98]. Instantiation of a pattern is quite tidy. Deﬁnition of a pattern involves a signiﬁcant amount of peek and poke code and the introduction of reserved words.

The difﬁculty of actually ﬁelding a reusable implementation of a particular solution is rather neglected. Vlissides participated in the development of a GUI tool to generate code for patterns automatically [Budinsky96], but then expresses

considerable reservations in his book [Vlissides98]. It is indeed difﬁcult to conceive an automatic generator that will have sufﬁcient ﬂexibility to balance all the conﬂicting forces and select the appropriate cookbook implementation. There is rightly much generality and ambiguity in pattern descriptions. However programmers regularly re-use particular pattern implementations with which they are familiar, and providing an improvement over cut and paste for such re-use would be beneﬁcial.

Some experimental work on a GUI-based interface for pattern instantiation in Smalltalk has been reported in [Florijn97] and [Meijers96]. It is difﬁcult to assess quite what has been achieved. Their system seems to exploit the ability of a Smalltalk program to reconﬁgure dynamically at run-time allowing pattern objects to be cloned interactively to extend the system under development.

Early descriptions of patterns identiﬁed the participants as the collaborators, which tended to be classes in an implementation. More recent work [Riehle97] and [Riehle98] has concentrated on the different roles that the participants play, and thereby begun to establish a hierarchy in which some more sophisticated patterns are composites of simpler patterns.

Application of a pattern requires roles to be associated with classes. The early descriptions tended to assume that a particular set of roles was performed by each collaborator, blurring the distinction between roles and classes. The rolebased perspective provides a more generic insight and offers more opportunity for providing ﬂexible tool support.

Composition of patterns is also addressed by [Lauder98]. A very generalised abstract pattern is identiﬁed for decoupled collaborations that can be trimmed to satisfy the more speciﬁc behaviour of the Adapter, Facade, Mediator, Observer or Reactor [Schmidt95] patterns.

Although patterns and roles (and aspects) appeared rather late in the evolution of UML, the generalisation of a collaboration diagram described in the User Guide [Booch99] permits parameterised instantiation. This appears to provide the required notational support. Unfortunately the change is minor and recent, and so attracts little attention in the Reference Manual [Rumbaugh99] or the Uniﬁed Process [Jacobson99] and temporarily lacks graphical tool support.

An approach to the enforcement of compliance with pattern constraints is provided by [Hedin97a] using attribute extension [Hedin97b] to extend a language grammar to incorporate patterns directly.

Reverse engineering patterns from code is provided by DP++ [Bansiya98]. Heuristics are required to recognise the relatively ill-deﬁned implementations that need to be found, and it is not possible to distinguish structurally similar but operationally different patterns.

Direct generation of code from CAD tools has been an unrealised goal for many years. The ﬂexibility for the invocation of a single meta-function to generate declarations in many classes, and for meta-programming to enforce or create nontrivial program structure may provide the necessary support for a CAD tool that just emits parameterised calls to a suite of meta-functions implemented by a metalibrary developer.

### 2.3.2 Aspects

Traditional programming approaches decompose a problem into functions or objects that often have a direct realisation in an implementation. An alternative decomposition can be made in terms of the concerns, properties or aspects of the system that the programmer must consider in order to satisfy system requirements. Effective decomposition identiﬁes loosely coupled modules that can be implemented and tested independently, but decomposition from one perspective generally destroys modularity from other perspectives. For instance,

a concern for error logging may pervade many functions and objects. Since a practical implementation is likely to be a function or object, the problem arises as to how to modularise the error logging aspect. The design of an aspect should be isolated from the application functionality but the implementation must be interwoven with the application code.

An informal2 estimate indicates that about 80% of an application is well suited to a structural decomposition, whereas the remaining 20% cuts across this structure and is more appropriately decomposed into aspects or programming concerns.

[Aksit96] discusses the need to maintain separation between the different concerns, and advocates the use of Composition Filters [Glandrup95]. Therefore, in C++/CF the different stages of message passing are reiﬁed, so that meta-code can intercept messages according to a variety of source and destination criteria. The practical implementation of C++/CF involves a custom preprocessor of limited capability that imposes considerable restrictions and obligations on the programmer. The greater capabilities of FOG could remove much of the inelegance.

The concepts of aspects have been taken up more generally in the new ﬁeld of Aspect Oriented Programming. The review of the ﬁrst ECOOP workshop [Mens97] considered whether AOP was really new or just a more palatable name for metaprogramming, given that AOP problems could be solved by reﬂection. [Kiczales97] provides an extensive discussion of the need for AOP and some interesting examples involving loop fusion, arguing that a meta-programming approach is just a stepping stone. Eventually each aspect could be supported by a customised programming language or environment [Fradet99], [Seinturier99].

Combining the functionality of the aspect code with the non-aspect code requires a mechanism to deﬁne how the code should be combined and requires a weaver to perform the composition. [Ossher98] discusses the join points where composition occurs and highlights the dangers imposed by the invasive characteristics of extra Aspect-Oriented code in comparison to the additive characteristic of Object-Oriented code. In a rather different ﬁeld, [Mulet95] describes constraints upon the composition of functions so that composition occurs predictably via nested invocation rather than ad hoc cut and paste. FOG can provide rather ad hoc code merging, but the use of nested meta-variables as in Section 7.4.1 provides an opportunity for greater discipline.

AspectJ [Lopes98] provides the ﬂexibility for Java programmers to make controlled additions to classes via introduce and advise weaves. An aspect is introduced into Java as an instantiable entity, providing the ﬂexibility to dynamically associate aspects and objects at run-time; the object behaviour can mutate. This provides direct language support for a particular and certainly useful pattern, and by introducing the support at load-time3 rather than compile-time is able to do so without specifying detailed implementation semantics. The aspect syntax forces tight coupling to the application, and so [Beaugnard99] suggests a relaxation to allow aspect, join and application to be independent.

FOG provides more extensive and more varied facilities than AspectJ, but needs to resort to meta-programs to implement the limited but useful capabilities directly available in AspectJ.

The ﬂexibility of load-time weaving is also exploited by [Welch99]. Run-time weaving with support for aspects at the meta-level is advocated in [Böllert99], [Lunau98] and [Pryor99]. A more pragmatic compromise in which some aspects are statically woven at compile-time while others are retained at run-time is suggested by [Matthijs97].

2. Gregor Kikzales at an AspectJ tutorial

3. The current implementation weaves at compile-time.

A slightly different take on programming concerns arises in Subject Oriented Programming [Harrison93], where the differing perspectives of the same objects appropriate to different applications is considered. The concepts of different views of an object provided by CV++ [Shilling89] are taken much further so that each application may be written with its own subjective view of each object. When such objects are shared between subsystems or applications, an update to one subject must make consistent changes to the underlying object and alternate subjective views. Composition policies and language extensions to achieve this are described by [Ossher95]. FOG could be used to implement the associated disciplines.

### 2.3.3 Generative Programming

Generative Programming [Czarnecki97] seeks to provide highly conﬁgurable components that can be combined and optimised at compile-time so that minimal overheads are incurred by an application.

```
An early perspective into reusable components is provided by [Batory92]. Two
independently developed domain-speciﬁc module generators, Genesis and Avoca,
were found to have very similar design and implementation. The common
concepts were combined to give the GenVoca principles of composing very thin
fairly orthogonal layers to create a desired component. In [Batory93] these
concepts are applied to C++ libraries, resulting in fewer source concepts to
generate more, smaller and faster library components than standard C++ libraries.
Further improvements in speed and ﬂexibility are reported in [Batory94], using a
succession of customised preprocessors and compilers: P1, P2 leading to P++
that adds support for realm and component to C++ [Singhal96]. Composition of
components is based on a realm of interchangeable components with a common
interface. The realm may therefore be used as a type signature. However, in
practice, not all components are completely interchangeable, there may be
constraints on, or prerequisites for, the ordering of compositions. [Batory97]
identiﬁes the need for upward and downward checking of constraints, using
pre/post-conditions/restrictions. A related implementation of container libraries
using template meta-programming is described in [Czarnecki99].
```

A direct form of generative programming is possible with imaginative use of C++ templates. [Myers95] describes the concepts of traits classes (template classes of parameters), which are used to pass a set of parameters and interrelationships as a single template parameter, thereby considerably simplifying the instantiation interface. These concepts underlie much of the ﬂexibility of the Standard Template Library [C++98]. Inference of expression types at compile-time is exploited in [Veldhuizen94] to generate customised inline functions that outperform conventional library implementations. Control structures are realised by recursive template instantiation in [Veldhuizen95] supporting generation of more sophisticated customised inline functions for sin, FFT or bubble sort.

The practise of composition of behaviour using mix-ins is reported to incur a potentially exponential growth in the length of template class names in [Smaragdakis98], where an extra outer mix-in layer is introduced to resolve the problem.

The GenVoca approach seems well suited to the implementation of efﬁcient reusable components from very simple building blocks. Template meta-programs provide effective techniques for composing the building blocks at compile-time, provided the composition results in a function or type. Unfortunately interesting components are more complicated. [Eisenecker97] suggests that GP subsumes AOP, but the template approach requires aspect functionality to use pre-existing parameterisation, rather than an independently developed weave. The facilities of FOG are required to compose more general declarations, in particular for extensions not supported by the base functionality.

## 2.4 Applications

C++ has been extended in minor ways by practical compilers [Stallman98] and [Microsoft97], and a few isolated language extensions such as [Baumgartner97] have been published. Researchers in many ﬁelds have chosen to use C++, but found it inadequate for their purposes. There are therefore many domain speciﬁc extensions to C++, just some of which are mentioned here.

Domain speciﬁc extensions, when fully integrated with C++, can provide a clean solution to the domain problem. However, many extensions are poorly integrated because of the size and complexity of C++ and so provide little more than a research tool. Many of the problems dealt with in a domain-speciﬁc fashion can be resolved in a domain-independent way by using the meta-level programming facilities of FOG. However FOG meta-programming is restricted to declarations and so the more radical changes of C** [Larus96] in which data parallel semantics are introduced to expressions could probably not be addressed.

### 2.4.1 Design by contract

Design by contract advocates the use of pre- and post-conditions and invariants to deﬁne the behaviour of components, and it is beneﬁcial for these properties to be expressed in implementation code.

Support for contracts is an integral part of Eiffel [Meyer92]. It has to be added for C++.

A++ [Cline90] extends C++ in a fairly natural way to support class assertions and invariants, which can in principle be optimised at compile-time.

CCEL [Duby92] adds a form of meta-programming using assertions in a predicate calculus so that constraints can be validated.

[Porat95] proposes some language extensions to support pre-conditions, postconditions and invariants.

### 2.4.2 Persistence and Marshalling

A low level understanding of object layout is necessary for persistent storage of objects in databases or for marshalling objects whether for signalling between nodes in a communication network or distribution between nodes in a parallel processor.

```
Persistence is commonly supported by an extended language adding a
persistent keyword to C++ as in OQL or E [Vemulapati95], or a replacement
allocator such as pnew in O++. An alternative approach is taken by [Park96] using
an object pre-header at negative address offset, so that persistent objects are
interchangeable with non-persistent objects. A MOP approach is recommended by
[Stroud94] to avoid the inﬂexibility imposed by the extended language PC++. A
simple reﬂective system based upon the Java API is described by [Lee98].
```

Wilson and Lu [Wilson96] provides extended articles by 16 of the leading research teams using C++ for parallel processing. Some researchers used only library classes and run-time support code, and so remain entirely within the normal conﬁnes of the C++ language. Others introduce language extensions, which are variously implemented as translators to C++, or modiﬁed C++ compilers. MPC++ [Ishikawa96b] exploits meta-level facilities to support an extended syntax within a “standard” C++ compiler. Many of the C++ extensions appear unnecessary and some authors recognise that more imaginative use of C++ facilities, particularly those not readily available at the start of their research could have reduced the need for divergence.

### 2.4.3 Synchronisation

Synchronisation is critical for reliable multi-process or multi-processor applications. Concurrent access to data has to be restricted, a problem resolved in principle by monitors [Hoare74]. In practice there are typographical difﬁculties in ensuring that monitor protocols are not accidentally bypassed and genuine difﬁculties in ensuring that a synchronisation policy is sensibly applied by derived classes. Alternative strategies are considered in [Matsuoka93], where reﬂection is considered necessary to solve the inheritance anomaly. Reﬂection is exploited by [Stroud95] to implement atomic access to data types.

## 2.5 Summary

The different approaches demonstrate that language extension can occur at three different levels:

- Library classes and run-time environments can be developed without any language or compiler changes. FOG’s increased capabilities at compiletime provide library developers with more options, perhaps supporting simpler interfaces, reduced requirements for user support code, or stronger compile-time detection of protocol violations.

- Translators that recognise one or two extra reserved words require development of the translator but do not affect the underlying compiler. FOG syntax macros provide an ability to introduce custom extensions to C++, enabling many of the characteristics of custom translators to be achieved by a general-purpose translator.

- New forms of code generation necessitate signiﬁcant revision to both language and compiler. FOG offers very little to applications that need to rewrite the basic compiler.

The need for many different research teams to develop customised variants of C++ demonstrates the need to be able to extend C++ to support new domains.

Research in the ﬁelds of patterns and Generic Programming shows an increasing need to structure large software components from smaller ones.

Aspect Oriented Programming demonstrates the need to combine relatively independent software modules into a composite whole.

FOG provides facilities to assist in all these areas. Unfortunately, it is difﬁcult to answer the critique that C++ is too large, and that adding meta-functionality is an enhancement too far. However, it is also difﬁcult to avoid recognising that the absence of meta-functionality is restrictive for some domains and an inhibition to re-use for all.

Programmers need to be able to express their ideas in compact modules. so that any form of repetitive and consequently predictable practice can be captured by a module that can be re-used. Functions support this concept for algorithmic behaviour. Records and objects support this for data structures. Templates extend the concepts across a variety of data types. FOG derivation rules provide further extension to program declarations. More arbitrary ﬂexibility requires compile-time or meta-programming with meta-functions capturing the repetitive program structure.

FOG, OpenC++ and MPC++ each provide meta-programming, but FOG’s capabilities are distinctly inferior because FOG does not currently provide direct access to the underlying ASTs. However, because FOG operates by composing declarations, FOG meta-programming integrates with C++, and so replaces the C preprocessor, and solves many practical programming problems decoratively. Equivalent solutions using OpenC++ or MPC++ must use imperative peeking and poking. FOG is upward compatible with C++ and so can make a much stronger claim to be an extended C++ than OpenC++ of MPC++.

Expanding FOG’s support for analysis of expression ASTs is a matter for further research.
