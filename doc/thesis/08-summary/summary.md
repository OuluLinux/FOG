# 8 Summary

The many achievements of FOG will now be reviewed before highlighting what remains to be done, and the limitations upon what can sensibly be done. The relevance to other languages will then be discussed before ﬁnally concluding with a brief summary of how the problems with C++ described in the introduction have been resolved.

**Novelty**

There are few ideas in software engineering that are totally new. Most are the result of a revision or combination of prior work. The functionality of FOG combines concepts from many areas, adapting them to ﬁt the philosophical, semantic and syntactic constraints of the C++ language. The combination is certainly novel.

## 8.1 Parsing

### 8.1.1 Context-free syntactical C++ parsing

Processing C++ declarations before their semantics have been determined necessitates context-free parsing. A clear distinction between syntax and semantics is not normally made because C++ is perceived to be inherently context-dependent requiring lexical, syntactic and semantic analyses to be tightly coupled and consequently blurring the distinctions between these concepts. The ofﬁcial “(informative)” grammar provides a mixture of lexical, syntactic and a few semantic rules. The main body of the standard does not always distinguish whether described constraints are syntactic or semantic. Tradition therefore perpetuates the perception that parsing must be difﬁcult.

FOG draws a pragmatic distinction between syntax and semantics. Syntax is what can be analysed by an LALR(1) parser such as yacc. Semantics is what has to be analysed later.

Examination of the C++ grammar shows that syntactic analysis without type information causes only a minor ambiguity for expressions using casts. The ambiguity is entirely deterministic and readily deferred for resolution at the postyacc semantic level.

```
Accurate syntactic analysis without template information is impossible. However,
an iteration through all alternatives of a template < / arithmetic < ambiguity can be
performed to determine a consistent, but not necessarily correct, syntactic
analysis. Instrumentation of practical programs shows that approximately 1% of
statements contain a template ambiguity, and that for approximately 1% of those
statements, the consistent parse is incorrect. A syntactically consistent parse is
therefore possible without template information, subject to the requirement that
the semantic processing must repair the incorrect parse for approximately 0.01%
of statements.
```

### 8.1.2 Back-tracking in yacc

```
LALR parsers such as yacc have no overt support for back-tracking unlike their LL
counterparts. Ambiguity problems that cannot be resolved within the grammar
need assistance from a separate lookahead parser. Implementation of back-
tracking within yacc, using the error token to rewind, proves to be fairly
straightforward enabling lookahead parsing to be performed within yacc. The
ambiguity between arithmetic and template interpretation of an < exploits this
back-tracking technique to perform a binary search to identify a syntactically
consistent parse.
```

### 8.1.3 Superset grammar

Traditional C++ grammar implementations attempt to maximise the semantic resolution of the yacc grammar, since this minimises subsequent coding. This is also motivated by the need to incorporate a large amount of semantic intelligence to resolve recursive declaration/expression ambiguities accurately.

Context-free parsing makes resolution of declaration/expression ambiguities impossible. The ambiguity must be deferred for semantic resolution rather than attempt to resolve it syntactically.

The superset FOG grammar recognises that the declaration/expression ambiguity derives from the fundamental C language design: declarations should mimic their usage in expressions. The ambiguity is therefore not a series of inconvenient barriers to be surmounted, but rather a series of partially overlapping subsyntaxes. Generalisation of declaration and expression and a few other productions are used to perform context-free parsing in the superset grammar.

### 8.1.4 Semantic analysis restricted to semantics

Removal of semantic considerations from the syntactic analysis considerably simpliﬁes the grammar, but requires additional semantic processing. However, this processing is making the same decisions as before, but in the controlled context of an AST rather than the difﬁcult partial environment during parsing.

Appendix F.3.1.1 describes how the resolution of the declaration/expression and related conﬂicts at the semantic level involves a straightforward but not quite trivial dataﬂow algorithm propagating a bit-mask of satisﬁed semantic hypotheses from the leaves of the AST to the root, where any residual ambiguity can be resolved by applying the deﬁned ambiguity resolution rules. The propagation makes use of type information, tree structure and associated semantic constraints in determining whether a hypothesis such as is-parameter-declaration is satisﬁed.

Deferring the ambiguity aids error diagnosis as well. An error during syntactic processing indicates that the syntactic analysis has failed to understand the token sequence and as a result an error recovery mechanism must be invoked to resynchronize. Since analysis failed, it is often difﬁcult to make a better error diagnosis than “syntax error near line x”. The more general syntax to defer the ambiguity accepts many sentences corresponding to simple typographical programming errors with the result that the syntactical analysis does not lose synchronisation, and so a more appropriate error diagnostic can be produced.

### 8.1.5 Extended regular expressions

Demonstrating that the superset grammar covered the existing syntax required analysis of the C++ grammar. An extended form of regular expression was introduced to describe sentences of C++ enabling the traditional ambiguities to be deduced and the superset justiﬁed.

## 8.2 C++ Extensions

### 8.2.1 Meta-programming

Meta-programming has been introduced, and as a result the C preprocessor rendered redundant, through the use of features that integrate with, rather than conﬂict with, the language.

Meta-variables and meta-functions supplant object-like and function-like macros, and beneﬁt from the consistent availability of argument and return types and deﬁnition within a class hierarchy.

Meta-statements replace conditionalisation, supporting loops as well.

An invited substitution mechanism avoids the hazards of imposed substitution with the preprocessor and provides a simple solution to the problem of lexical concatenation.

### 8.2.2 Composition rather than One Deﬁnition Rule

The C++ One Deﬁnition Rule requires complete declarations to occur in a single place. This prevents code being organised by algorithm rather than by data. FOG eliminates this restriction so that multiple declarations are combined to give a composite meaning. This supports weaving of declarations together for Aspect Oriented Programming, or elaboration of declarations by meta-programming.

### 8.2.3 Minor extensions

Some minor enhancements to C++ are introduced to provide greater consistency when declarations are composed.

### 8.2.4 Derivation rules

Derivation rules are perhaps just a little bit of syntactic sugar to simplify metaprogramming. However many realistic problems involve a policy that has to be observed by classes within an inheritance hierarchy. This requirement is captured directly by derivation rules. Related work on automatic generation of code appears to concentrate more on resolution of composition conﬂicts.

### 8.2.5 Syntax macros

```
The illusion of a language extension can be created by a syntax macro, so that
users may introduce new keywords such as synchronised or persistent.
```

## 8.3 Detailed Language Issues

### 8.3.1 Scoped preprocessing

Macros and preprocessing are a neglected, perhaps scorned, ﬁeld in software engineering. Little work has been done and no work that considers macros within the hierarchical context of C++. Resolution of macro-names within a prevailing scope, with the consequent beneﬁts that can accrue from isomorphism and inheritance is new in FOG.

### 8.3.2 Deferred substitution

```
Resolution of names within the correct name-space at the correct-time is a
traditional concern of language designers and consequently programmers. The
functional argument (FUNARG) problem in Lisp demonstrates the problems of
avoiding name capture. The distinction between ‘ and & substitution operators in
VAX/VMS DCL show the need to control resolution time. FOG applies related
concepts to substitution within meta-programs through the $ and @ operators.
```

### 8.3.3 Polymorphic syntax

Syntax macros are traditionally syntax-driven: the known syntactical requirements of the macro (a meta-function or meta-variable in FOG) are used to guide the syntactic analysis. This introduces two semantic context-dependencies to the syntactic analysis.

```
Exploiting the known syntactical requirements may require semantic analysis to
determine what is required by the particular usage. The superset grammar uniﬁes
many C++ constructs and the approach is extended to deﬁne the tree-statement
production that encompasses almost the entire C++ grammar, enabling a syntax-
independent and consequently context-free parse of meta-function arguments and
```

meta-variable initializers. Syntactic analysis of each meta-function and metavariable usage is therefore context-free in FOG.

```
Deﬁning the macro in the ﬁrst place requires semantic analysis of the deﬁnition,
albeit a degenerate semantic analysis such as the extra-lingual #define for the C
preprocessor. FOG also uses a syntax for syntax macro deﬁnitions for which a
premature semantic analysis can be activated during syntactical analysis.
```

Syntactic analysis of the use of a syntax macro requires dynamic changes to the table of reserved words and a data-dependency upon the number but not type of parameters.

This is only novel within the context of C++. In a language with a clean syntax a generic parse should be trivial, but still worth implementing to remove contextdependency.

### 8.3.4 Literal source

Meta-code surrounds ordinary statements and declarations so that there is no need for special syntax or procedures to deﬁne source syntax literals.

The source syntax is its own literal (overlined).

```
auto statement switchBody = { default: return 0; };
auto switchBody += case 1: { flags++; return 1; };
```

There is no need for any insight into the structure of the internal ASTs or their support functions.

The entire function body is returned:

```
auto declaration declare_pointer_classes(identifier aClass)
{
class $aClass;
typedef PointerTo<$aClass> ${aClass}Pointer;
typedef PointerTo<const $aClass> ${aClass}ConstPointer;
}
```

### 8.3.5 Potential and Actual

Meta-programming is traditionally practised in Smalltalk and Lisp-like languages, where meta-programming occurs at run-time re-using functionality necessary to establish an Object Oriented execution environment. More recently metaprogramming has been possible in Java at run-time and rather more interestingly and uniquely at load-time.

Compile-time (or static) meta-programming is not widely used since it is only available in research languages such as OpenC++ or MPC++. These languages support programmed manipulation of the declaration pool.

Existing approaches therefore deal with actual declarations. FOG with its syntactic support for source literals introduces the distinction between potential declarations and actual declarations, allowing meta-programs to operate consistently on declarations with determined or undetermined scopes.

## 8.4 Further Work

The ﬁrst version of FOG currently available on the net used the multi-pass grammar approach. The more stream-lined and efﬁcient approach supported by the superset grammar is also available, but requires considerable further development.

FOG is currently written in C++ using a trivial custom preprocessor that just splits interface and implementation from a single ﬁle and performs code synthesis only for include ﬁles and their guards. The source code for FOG should be revised to exploit FOG functionality, and thereby demonstrate and test the use of FOG more

convincingly. A measurement of the lexical source size reduction should show how beneﬁcial FOG is for large programs with deep inheritance hierarchies.

C++ is a large language, which FOG should as a minimum parse and emit unchanged. FOG has a useful degree of functionality in many areas, however practical experience with interesting small examples tends to encounter unimplemented or misimplemented functionality with respect to the current state of the implementation.

**Meta-library**

Use in a diverse range of applications needs to be assessed and a meta-library of common utilities developed.

**Join discipline**

The original multi-pass implementation did not analyze function bodies, and so function body composition was performed by lexical concatenation without regard to even syntactical validity. The full super-set parse, combined with the use of token lists to maintain ASTs, can ensure semantic validity at the language level. However the more challenging issue of establishing or enforcing programming practices that ensure integrity of programming intent remains to be addressed.

**Use of self-evident semantics at syntactical level**

```
Section 4.2.3 identiﬁed the need for a possible variant of the $ trigger to allow the
known semantic type of the argument to be exploited.
```

**Composition of exceptions**

```
Section 4.4.8 identiﬁed a possible policy for composition of exception speciﬁcations
and function-try-blocks.
```

**Meta-programming phases**

Section 4.6 identiﬁed the inadequacy of the meta-construction, meta-main and meta-destruction compilation stages.

**Syntax macros**

Section 4.7 described a partially implemented proposal for syntax macros, and identiﬁed severe limitations for the case of multi-argument syntax macros.

**Expression AST traversal**

The built-in meta-functions described in this thesis support meta-programming of declarations. Further meta-functions could be added to support metaprogramming of expressions thereby providing the ability to peek and poke in arbitrary fashion just like OpenC++. Further research is needed to determine whether it is merely necessary to support arbitrary user access, or whether a more disciplined form of support can be identiﬁed.

## 8.5 Limitations

FOG operates as a translator to C++ and so necessarily precedes C++ compilation. FOG cannot operate on actual compilation results, only upon predictions of those results. This has two consequences.

It is not guaranteed that FOG sees the ﬁnal state of declarations. In a multisession compilation, a class may appear to be a leaf class in one session, but further derived classes may exist in other sessions. Decisions predicated on leaf-

ness will therefore be in error. A complete ﬁx of this problem requires global knowledge. Detection of the anomaly can be resolved as described in Section 6.5.8 through the use of a checksum to express the non-global knowledge of a meta-compilation session.

Template instantiation occurs during or after compilation. It is therefore difﬁcult for FOG to know which parameter combinations will be used, or to detect which member functions will actually be required. In the general case where FOG is used to prepare library code, FOG cannot know what the instantiations will be. It is therefore impossible for meta-programs to manipulate template instantiations usefully. Meta-programming in FOG is limited to manipulation of template declarations.

Since templates create contexts in which FOG cannot know what type is in use, and so restricts the amount of meta-programming that can be reliably performed. This problem can only be resolved by integration of meta-compilation with normal compilation, so that meta-compilation is performed on instantiated as well as declared templates.

Syntax macros provide a limited mechanism for introducing language extensions, however it is difﬁcult to do better within the conﬁnes of the poorly structured C++ syntax.

Most programmers have an, at least initial, dislike of the compact and idiomatic style of the C and C++ syntax. Experienced C and C++ programmers come to like it. FOG adds further extensions in the style of C++ and so provides more to confuse or dislike. It remains to be seen whether real programmers learn to ﬁnd the extensions acceptable and natural.

## 8.6 Other Languages

Although the work described in this thesis is primarily concerned with resolving deﬁciencies in the use of C++, the work is of greater applicability, mainly to languages that involve signiﬁcant compilation activity such as Eiffel, Ada or Java. Introduction of extra compilation stages is inappropriate for languages such as Smalltalk or CLOS where object structure is deﬁned at run-time.

The distinction between potential and actual declarations, the concept of derivation rules and a ﬂexible substitution based upon tree-literals combined with a lexical concatenation are not speciﬁc to C++, although some of the detailed syntactical issues are. Implementation of these concepts in other languages is likely to be a little simpler, since few other modern languages have quite such a challenging syntax as C++.

The observation that the One Deﬁnition Rule is a major hurdle to implementation of patterns and Aspect Oriented Programming is again applicable to all languages. Language designers should endeavour to support interleaved declarations.

Programming involves repetition at many levels, and programmers naturally seek to factor the repetition into some parameterisable reusable construct, which may be a loop, subroutine, class, template, macro, ﬁle or library. Omission of any of these capabilities simpliﬁes a language, but limits the programmer’s or the program’s efﬁciency. Some form of macro to perform lexical processing and metaprogramming is therefore beneﬁcial to all languages, although the precise syntax must be carefully chosen to ﬁt within the traditional style of each language.

## 8.7 Resolution of Goals

The introductory discussion highlighted problems that arise with C++. The way in which these are resolved in FOG will be summarised.

Cpp should be replaced rather than eliminated.

Object-like and function-like macros are replaced by meta-variables and metafunctions.

```
Substitution by imposition is replaced by a substitution invited by $ or @.
```

## and # are replaced by adjacent lexical element concatenation.

Conditional processing is replaced by meta-programming.

Compile-time programming is necessary to conﬁgure declarations. Introspection is useful for simple applications. Reﬂection is almost essential for sophisticated applications.

Meta-programs can manipulate declarations.

Patterns and AOP require weaving. The One Deﬁnition Rule must be circumvented. Interleaved declarations should be allowed.

The One Deﬁnition Rule has been relaxed to allow declarations to be introduced outside the conﬁnes of class braces. This supports interleaved declarations and weaving within classes. Multiple contributions to the same declaration are composed, supporting weaving of individual declarations.

Lexical redundancy should be eliminated.

The need for distinct interface and implementations has been removed.

Derived code can reuse inherited declarations.

Predictable code should be provided automatically.

Derivation rules support automatic generation of derived code.

Meta-programs can generate code for more specialised applications.

A concept should be instantiated by a single invocation.

Invocation of a meta-function can provide complete instantiation, exploiting composition to inject code as appropriate.
