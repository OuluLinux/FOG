# 5 Parsing

This chapter deals with the practical problems encountered during the development of the parser for FOG, and the reasons for the solutions adopted. The problems and solutions, while motivated by the needs of FOG, are almost entirely concerned with C++. This chapter therefore concentrates on the C++ perspective, so that readers only interested in the analysis and techniques for analysis of C++ syntax and the novel C++ parsing approach may read this chapter in isolation.

Readers particularly concerned about C++ language details may care to browse the on-line penultimate working draft [C++96] of [C++98] at

```
http://www.maths.warwick.ac.uk/c++/pub/wp/html/cd2
```

or print off a copy of Annex A from

```
http://www.maths.warwick.ac.uk/c++/pub/dl/cd2/CD2-{PDF,PS}.tar.Z
```

**Overview**

Traditional approaches to parsing C++ make use of semantic (type-name and template-name) knowledge during the lexical and syntactical analysis stages. Meta-programming in FOG can involve manipulation of declarations before type and template information is available. A traditional approach to parsing C++ will therefore not work for FOG, so a new superset approach is necessary.

The superset approach involves identifying a larger language than C++ that can be parsed without semantic knowledge. The traditional declaration/expression ambiguities are unambiguous in the larger language, and so their resolution can be deferred until the semantic analysis that performs the narrower C++ analysis. This approach is then able to operate without type-name information

A lack of template-name information is more disruptive, and so another new approach is required. This involves a potentially brute force search of all the template/non-template alternative parses until a syntactically consistent parse is found. This search is shown to incur only minor costs in practice, and a simple implementation of the search is provided by introducing back-tracking without modifying yacc/bison. The rare discrepancy between a syntactically consistent and the semantically correct interpretation is able to be deferred until the C++ semantic analysis, at which point minor corrections produce the required parse.

The combination of these two new approaches enables the different parsing stages to be isolated. Lexical analysis is performed using a very simple lex grammar, that makes use of no syntactical or semantic knowledge. Syntax analysis is performed with a yacc grammar that is smaller and closer to the language standard than traditional grammars. Since the grammar does not use semantic knowledge, it does not suffer from the problems of potentially inﬁnite lookahead to resolve types that normally arise. The ﬁnal semantic analysis occurs within the context of the Abstract Syntax Tree that represents the entire program, so the analysis may be coded in a natural style, rather than within the tight conﬁnes of parser action routines that have only limited context and must not provoke shift-reduce conﬂicts.

A further innovation extends the regular expression notation so that complex productions from the C++ grammar can be analysed and the ambiguities deduced.

**Chapter Summary**

The traditional technology, structure and terminology of a compiler are brieﬂy outlined in order to provide some grounding for readers not well-versed in compiler fundamentals. The dragon book [Aho86] is the recognised authority.

Typical approaches to parsing C++ are discussed, the choice of parsers available as a basis for the FOG parser is reviewed and then the evolution of FOG from a very simple to fully ﬂedged parser is described.

The potential and actual ambiguities encountered by a C++ parser are analysed, by way of demonstrating an extended form of regular expression notation that supports reasoning about grammars.

The syntax generalisations of the superset grammar are described and the new notation is then used to justify the soundness of the superset parsing approach, which solves the need for type information. The use of back-tracking in bison is then described to solve the need for template information.

A few details of the code structure are provided to demonstrate the high degree of isolation between the parsing stages.

Finally some size metrics are produced to compare the new C++ parsing approaches against other approaches and estimate the extra cost of the FOG enhancements.

## 5.1 Terminology

Figure 5.1 is based on Figure 1.9 of the dragon book [Aho86]. It shows the typical

components of an application that translates a source program into a target program.

Successive stages of analysis extract the meaning of the source program, enabling an intermediate representation to be built, optimised and then converted to the required target program. A symbol table maintains information to be shared between stages. The error handler supports generation of error messages in as helpful a fashion as possible.

The analysis is broken into three stages. The lexical analysis identiﬁes and validates individual lexemes (words), the syntax analysis identiﬁes and validates grammatical constructs (sentences), and the semantic analysis validates the meaning of each construct within a wider context.

Lexeme is the normal term for the product of lexical analysis. The same concept is more commonly referred to as token in the context of syntax analysis. Syntax

lexical analyzer (lexer)

semantic analyzer

syntax analyzer (parser)

intermediate code generator

code generator

code optimizer

symbol-table manager error handler

source program

target program Figure 5.1 Compiler Translation Stages

presents the greater technical challenge and so the term token is preferred in this thesis.

In C and C++, the preprocessor provides additional translation. It may be implemented as a separate program, or as an additional stage between lexical analysis and syntax analysis. The C++ standard treats the preprocessor as an extra stage and uses the term preprocessor token to describe a lexeme that passes from lexical analysis to preprocessor, and the term token to describe a lexeme passed from preprocessor to syntax analyser.

Lexical and syntactical analysis were once the hardest parts of a compiler to write, however with the advent of standard lexer and parser generator tools, these stages are now relatively easily automated.

The lex program converts a grammar speciﬁcation in the form of a number of regular expressions into a state machine or DFA (Deterministic Finite Automaton) that accepts characters one by one from a source ﬁle and emits a lexeme for each analysed word. lex [Lesk75] is the standard tool. ﬂex [Levine90] is a more polished version distributed as part of the GNU tool set.

The yacc program converts a grammar speciﬁcation in the form of BNF rules into an LALR(1)1 parser that demands lexemes one by one from the lexer and invokes action routines as each parsing rule is satisﬁed. yacc [Johnson75] is the standard tool. bison [Levine90] is a more polished version distributed as part of the GNU tool set.

```
The ﬂex++ [Coëtmeur93b] and bison++ [Coëtmeur93a] variants were used for
FOG. The variants encapsulate the generated lexer or parser as a C++ class, and
so readily support multiple and re-entrant lexers and parsers. FOG uses one lexer
grammar, four (tiny) parser grammars for ANSI C preprocessing and one (huge)
parser grammar for extended C++ parsing. Deferred character-level substitution
by std::parse allows lexer and parser to be re-entered during meta-compilation.
```

The operation of the state machine generated by an LALR parser is extremely simple comprising just four actions for each possible next token.

- accept the token as the termination of a sentence in the grammar

- reject the token as inconsistent with any sentence of the grammar

- shift to another state, deferring any decision

- reduce following recognition of a rule

Parser generators allow application code to be supplied for execution when a rule is recognised. This code will typically create a data structure that describes the information that has just been parsed. In compiler applications, these data structures are highly recursive and well represented by a tree structure. The structure is called an Abstract Syntax Tree.

```
In the following very simple yacc grammar, there are 7 rules leading to 4
productions. Each (production or reduction) rule has a non-terminal at its left-hand
side and may use terminals or non-terminals on its right-hand-side. Identifier
and '*' are terminals.
term:
Identifier
{ $$ = create_identifier_node($1); }
|
Number
{ $$ = create_number_node($1); }
product:
term
{ $$ = $1; }
|
product '*' term
{ $$ = create_multiply_node($1, $3); }
```

1. Parsing algorithms are categorised as LL “Left-to-right scanning of input, Lightmost derivation”, LR “Left-to-right scanning of input, Right-most derivation in reverse”, and LALR(k) “k token Look-Ahead, Left-to-right scanning of input, Rightmost derivation in reverse”. Handwritten parsers are typically LL. LR parsers are more powerful than LL, but need to be machine generated. Standard tools such as yacc and bison pursue the more compact LALR(1) approach.

```
expression:
product
{ $$ = $1; }
|
expression '+' product
{ $$ = create_sum_node($1, $3); }
grammar:
expression ';'
{ $$ = $1; }
There are two rules for the production of a term. One from an Identifier and
one from a Number. A create_xxx routine is associated with each rule using the
special $n variables to access inputs, and $$ to propagate a result.
```

The source sentence

```
a + b * 5 ;
```

is parsed to create the AST

```
Shifts occur to advance to states that reﬂect a partial parse following each of +, b
and *. Reductions occur as each rule is recognised
•
one reduction after the a
•
one reduction after the b
•
three reductions after 5, for each of the rules
-
Number to term
-
product * term to product
-
expression + product to expression.
```

Each of these reductions activates the application code that creates the AST nodes. Once the external textual representation has been converted to an internal AST form, the program can be manipulated by compilation code to perform whatever checking, correction, rearrangement or optimisation is necessary to perform the translation.

An unambiguous grammar provides for only one possible parse tree for a given input, although a potentially inﬁnite amount of lookahead may be required to distinguish between alternative partial trees for partial inputs. Conﬂicts arise from an attempt to distinguish alternatives prematurely. A reduce-reduce conﬂict arises if there are two alternate simpliﬁcations available. A shift-reduce conﬂict arises between an elaboration and a simpliﬁcation of context.

```
A parser generator has a policy by which conﬂicts are resolved to produce a
deterministic, although not necessarily useful parser. The default resolution of
conﬂicts may be controlled by the programmer using the concept of precedence.
The name reﬂects its original use to resolve precedence problems with arithmetic
operators. The concept is of more general use. A special %prec non-terminal may
be used to specify resolution of a conﬂict.
Translation programs are frequently presented with source ﬁles containing errors.
As many of these errors as possible should be detected and diagnosed in a helpful
fashion. It is rarely acceptable for a translator to just stop and report that a “parse
error” has been encountered somewhere. The analysis must therefore continue
after an error has occurred. In support of this philosophy, yacc suspends analysis
and generates a special error token when an error is encountered. A carefully
written grammar can make use of error to control resumption of the analysis.
```

*

+

number

5

identiﬁer

a

identiﬁer

b

**Figure 5.2 Abstract Syntax Tree**

## 5.2 Approaches to C++ Parsing

An LALR(1) shift-reduce parser generates a table driven parser for an unambiguous context-free grammar, subject to the requirement for detection of the right-most edge of a grammar production with 1 token of lookahead. The C++ grammar is ambiguous, context-dependent, and potentially requires inﬁnite lookahead to resolve some ambiguities:

```
int(x), y, *const z;
// int x; int y; int *const z;
```

Is a comma-separated list of declarations in which the ﬁrst is redundantly parenthesised, whereas changing the ﬁnal list element:

```
int(x), y, new int;
// ((int(x)), (y), (new int));
```

gives a list of expressions, the ﬁrst two of which are redundant, and the third causes a memory leak.

Other ambiguities are resolved by the language deﬁnition:

```
int(x), y, z = 0;
// int x; int y; int z = 0;
```

This could be an expression too, but isn’t. It may not be possible to determine the meaning until a potentially inﬁnite amount of further source text has been analysed.

An LALR parser is not an obvious match to these requirements. However the alternatives are worse. A (bottom-up) LALR parser is faster and more compact than an LR parser, and able to handle all grammars that could be handled by a simpler (top-down) LL parser, and so the most widely used parsers are based on LALR(1).

In order to satisfy the constraints of an LALR(1) parser, the ambiguities, context dependence, and lookahead problems of C++ must be resolved.

The dragon book [Aho86] recognises that the boundaries between lexical, syntactic and semantic analysis are not clear cut.

Traditional C++ approaches seek a correct high resolution parse. As a result, the boundary between syntactic and semantic analysis has to be shifted to exploit semantic information during syntactic analysis by the parser and to leak semantic information through to the lexer. Use of semantic information during syntactic analysis requires very tight coupling to ensure that scope context is honoured and that changes of name visibility in mid-statement are correct. [Roskind91] provides a particularly unpleasant example where a change of classiﬁcation midway through an apparent declaration leads to a contradiction, that is only resolvable as an expression.

The two variants of the new parsing approach described in this chapter do not move the boundary. The parser proceeds without full semantic knowledge and produces a result that is syntactically consistent, but sometimes semantically incorrect. An additional pass is therefore added to the semantic analysis to correct the inaccuracies of the syntactic analysis.

The advantages of this approach are:

- elimination of type (and template) tagging

- much simpler grammar that more closely follows the standard

- clear separation of syntactic and semantic processing

- conversion of syntactic ambiguities to semantic ambiguities

- disentangles resolution from grammar implementation

- conversion of syntactic errors to semantic errors

- avoids some losses of synchronization - provides more opportunities for helpful error diagnostics

- probably very slightly smaller code size

- more functionality to be coded - simpler context for the code

The disadvantages of this approach are:

- introduction of new ambiguities

- an additional semantic correction pass

- additional semantic validity checking

- probably very slightly slower

- more functionality to be invoked - more functionality to be executed - simpler context for the code

```
Two variants of the approach are described. The previous implementation
(available on the net in the v1 subdirectory) uses multiple passes to resolve
ambiguities. The current implementation (available on the net in the
v2
subdirectory) uses a superset grammar approach enabling operation without type
or template information, as is necessary to support meta-programming
consistently (Section 4.2.3).
```

The context-dependencies of C++ are described in detail in Appendix F.2.1.

## 5.3 Alternatives

Before developing the FOG parser, a brief review of the available alternatives was made. These alternatives are described in this section, along with two others that were not known at the time. Unfortunately, the developers of commercial C++ compilers do not make their parsers freely available in the public domain, and so the many proprietary implementations cannot be considered. However, comparison of the public domain approaches is sufﬁcient to shed useful light on the difﬁculties.

### 5.3.1 Roskind grammar

```
A yacc-able C++ 2.1 grammar was made available by Jim Roskind [Roskind91].
This grammar dates from 1991 and has not been updated to handle C++ facilities
such as templates or exceptions foreshadowed by the ARM [Ellis90], or to
incorporate concepts such as bool and namespace added during standardisation.
The paper accompanying the grammar provides a very insightful discussion into
the source of the parsing problems and some rather pathological examples, whose
correct interpretation is debatable.
```

The grammar resolves context dependence by a “lex hack”, so that the lexer classiﬁes identiﬁers as either IDENTIFIER or TYPEDEFname. The paper notes the need for another such hack to resolve template names.

The grammar is no more than a grammar. There is no action code to react to successfully analysed constructs, and only dummy hooks at the places where symbol table maintenance must be performed. The grammar code has no error recovery.

Ambiguities are resolved but are not removed from the grammar code. Some ambiguities are eliminated by rewriting parts of the grammar. Others are carefully analysed to ensure that the default ambiguity resolution policy of the parser generator chooses the required alternative.

Some potential lookahead problems are resolved by structuring the grammar code to recurse on the right-hand side, or by ﬂattening out, in each case deferring reductions until more context has been seen. Other lookahead problems are resolved using the disambiguation policies of the previous paragraph. Further

problems could have been resolved by more ﬂattening, but were perceived not to merit resolution while there was a possibility that the C++ grammar could change.

The grammar has a total of 24 shift-reduce and 18 reduce-reduce conﬂicts, originating from 11 ambiguities. (Since conﬂicts occur between states, an ambiguity results in more than one conﬂict if the ambiguity affects more than one state transition.)

### 5.3.2 gcc

The GNU C compiler [Stallman98] has evolved to handle Objective C and C++. The compiler continues to improve, and is close to the C++ standard, but currently (version 2.8.0) experiences signiﬁcant problems with template instantiation, because of the lack of a compilation database. These problems would not affect the use of gcc as a foundation for FOG.

The gcc compiler is portable to a very large number of operating systems, on which a build process normally involves compiling gcc through the local compiler, then recompiling gcc using the potentially better optimisations of gcc. The need to bootstrap through the local compiler requires extensive conditionalisation so that the gcc sources avoid the defects of all known compilers. gcc source code is therefore harder to read than it might be.

gcc source code is necessarily written in C, and so lacks the modularization and polymorphism that can be achieved using classes and Object Orientation in a language such as C++. The internal data structures of gcc comprise a tree node that is a union of all possible expressions, operators, names, declarations, statements, ﬁles etc.

The gcc compiler is a complete compiler including good error recovery and diagnosis, and full symbol table maintenance. gcc is recognised to be of production quality.

```
The lexer is hand coded and makes a seven way categorisation of identiﬁers to
disambiguate the subsequent parser. However the subsequent parser requires
109 %prec directives to resolve 704 conﬂicts explicitly, leaving 5 shift-reduce and
38 reduce-reduce conﬂicts to be resolved automatically.
```

### 5.3.3 CPPP

```
A C++ parser was developed at Brown University, as a general purpose tool for
which a variety of applications were foreseen [Reiss95]. This parser has steadily
evolved, however the most recent version available on the net is version 1.82 from
1996. It would appear that development stopped before facilities such as
namespace or bool were implemented.
```

CPPP achieves a higher degree of decoupling between lexer, parser and database than gcc, and has a grammar that closely resembles the published C++ grammar. CPPP comprises three stages: a lexer, a lookahead parser, and a main parser. The lookahead parser recognises potentially ambiguous constructs and invokes custom parsing routines to look sufﬁciently far ahead to resolve the ambiguity. Additional tokens are inserted into the token stream so that the subsequent parser proceeds without ambiguity.

The P++ developers [Singhal96] report extending CPPP successfully, but only after resolving a fair number of bugs. CPPP was also used for Iguana [Gowing96].

```
The grammar has only one unresolved shift-reduce conﬂict, but uses precedence
extensively (22 %prec) to suppress a further 410 conﬂicts. The high number of
conﬂicts is misleading. Most are the result of ﬂattening the expression syntax and
using grammar precedence to implement arithmetic precedence. This results in a
faster parser since an expression term is reduced just once, rather than once at
each of the ten binary operator precedence levels of the C++ grammar.
```

### 5.3.4 PCCTS

The Purdue Compiler Construction Tool Set provides alternative versions of lex and yacc called DLG and ANTLR. ANTLR is an LL(k) parser generator. Use of LL principles provides the freedom and (for practical grammars) the necessity of incorporating semantic resolution within the parse. A C++ grammar is available for use with PCCTS [Lilley97], but is heavily disclaimed as initial and experimental. A signiﬁcantly customised version of ANTLR is required by the grammar.

### 5.3.5 C++ to F-code translator

The work described in this thesis concerns meta-compilation and the FOG implementation. The inspiration for FOG arose from research aimed at the development of a compiler implementing optimisations appropriate to DSP processors. This compiler was to use F-code [Muchnick93] as its intermediate representation. Some work was performed on a C++ to F-code translator as part of this earlier research. At that time, there was no knowledge of the CPPP or PCCTS grammar and so there was an implementation choice between

- the out-of-date Roskind grammar (42 conﬂicts)

- the tightly coupled gcc grammar (747 conﬂicts)

- a custom solution

The author had previously extended gcc to perform automated documentation generation for C++ code. This had merely required a late read-only traversal of the internal data structures. The difﬁculties of debugging with union nodes, the lack of clear documentation on the semantics of each node, and the enormous 250,000 line size of the code indicated that wholesale extension of gcc could lead to considerable problems.

A simpler alternative of just reusing the gcc parser grammar was examined. Examination of the parser showed that the parser, lexer and program data base were too closely coupled, making separate re-use of the grammar alone impractical.

The seemingly large number of unresolved conﬂicts, out-of-date character and obscure coding of declarators in the Roskind grammar discouraged its use. Recognition that the introduction of tree-literals would impact the heart of the grammar indicated that a clean grammar should be the starting point. A custom solution seemed the only alternative.

## 5.4 FOG parsing

The original aims of FOG concerned facilities for

- automatic insertion of repetitious code into class declarations

- elimination of redundant source text

- support for algorithm-centric modularization

The ﬁrst aim is satisﬁed by derivation rules.

The others correspond to what is now known as weaving in the Aspect Oriented Programming world.

**FOG/1 - superﬁcial guided parse**

```
It was perceived that these goals could be satisﬁed by a very simple parser
assisted by extra keywords (guides). The ﬁrst implementation therefore explicitly
annotated C++ source adding new reserved words such as constructor,
function, variable and type for the guides. Lines that needed special treatment
could be easily identiﬁed and other lines copied from input to output without
interpretation.
constructor Class::Class(int aSize) : _size(aSize) {};
```

This approach imposed a language incompatibility. For a legal C++ source ﬁle to be acceptable to FOG/1, it was necessary to manually add the guides to every declaration.

A second problem arose as to what syntax to use following the guide. The C++ declaration syntax is complicated and highly recursive. There are better syntaxes than the C++ syntax [Werther96], but they look very out of place in a C++ program. The C++ declaration syntax was preserved, and the problem of recursion of function signatures was partially solved by treating the function signature as unparsed text to be copied though to the output. This hid the problem for most functions but could not cope with pointers to functions, where the name is buried in, rather than preceding, punctuation.

```
variable int (*v)();
function int (*f())();
```

**FOG/2 - pragmatic guided parse**

Resolution of the function signature problem mandated an accurate parse of the signature. This problem had already been solved as part of the C++ to F-code translator and so the relevant part of the grammar was re-used. C++ function signatures comprise parameter types, names and optional default values. The default value was initially left unparsed, with the text copied unchanged to the output.

```
function void f(int a = unparsed_text);
This usually works, but requires recognition of the ) or , that terminates the
initializer. The initializer is an expression and so is subject to the use of templates.
A simple parse of the initializer in
template <bool T1, int T2> class B;
function void f(int a = B < c, 5>);
may identify the comma-terminated
B < c as the ﬁrst initializer, before
misinterpreting the residue. It requires the knowledge that B in the unparsed text
is a template to correctly resolve the instantiation. Reliable parsing of declarations
requires reliable parsing of expressions too.
```

**FOG/3 - pragmatic full parse**

At this point it was becoming clear that many of the complexities of C++ parsing could not be avoided in FOG, and it seemed likely that every attempt to avoid a complexity would introduce a deﬁciency. This is a far from unique discovery. There are a number of commercial development tools that have taken short-cuts to parsing C++, with the result that interesting C++ programs are misinterpreted. The class browser of Microsoft Visual C++ is just one example.

Another large segment of the C++ to F-code grammar was therefore added to FOG, so that FOG contained most of the C++ grammar. The traditional C++ declaration / expression ambiguities did not (yet) arise, because expressions occurred only in the limited context of initializers.

```
The need for guides was a barrier to porting existing C++ code to exploit FOG.
Some of the ﬁrst code to be ported from C++ to run through FOG was some
compiler code. This code made use of the guides as function names. Requiring
that the member function type() be renamed did not seem sensible.
```

With the increasingly accurate C++ parsing in FOG, it was no longer essential to have the extra guides. Parsing was no longer made easy by their use.

Once the extra guide keywords had been eliminated, there were few fundamental differences between FOG and C++ syntax. Each of these was challenged and

eliminated, with the result that the FOG grammar is an almost pure2 superset of C++.

**$-parsing**

Tree-literals were originally envisaged as being resolved by a preprocessor before the real code emerged. A tree-literal was therefore recognised very early and replaced. Replacement could occur anywhere, including within strings. The semantics of the replacement were purely lexical, and so the replaced text could contribute partial strings and partial reserved words. This was ﬂexible, powerful, undisciplined and awkward to implement. The ability to handle partial tokens required an unpleasant ability to recurse earlier lexing stages. The implementation resulted in a very complicated lexer, that had to maintain a stack of states according to how far through a string / character / number it was when a replacement started. It was far from clear that the implementation would behave correctly under perverse usage.

Recognition that the ANSI C string concatenation could be generalised to concatenation of adjacent textual tokens revealed that there was no need for substitution within strings:

```
"built at $time on $date"
// Not FOG
```

It could happen almost as easily between strings:

```
"built at "$time" on "$date
// FOG
```

This then enabled the lexer to be more disciplined; preprocessor tokens could be identiﬁed ﬁrst, then substitution could occur token by token, with only a minor complexity in retokenizing the result of a concatenation. Retokenisation was eventually discarded as unnecessary (Section 3.1.1.4).

**Ambiguity resolution**

Removal of the guides requires the syntaxes for typedefs, variables and functions to coexist. Ambiguities arise:

```
T ( A );
could be a constructor for T with an unnamed parameter of type A. Or it could be
a variable of type T with the redundantly parenthesised name A. This problem is
traditionally resolved by ensuring that the lexer has the semantic information
available to classify T and A as class-name or type-name or identiﬁer. In C++, this
classiﬁcation requires accurate scope context and symbol table maintenance to
ensure that an identiﬁer is classiﬁed as a type-name at the declaration point.
```

Type information was not being used in the FOG parse and it seemed desirable to continue not using type information. The few ambiguities that appeared as a result were resolved by back-tracking, which is described in Section 5.8.

Development of the C++ to F-code translator had revealed how difﬁcult resolving the expression / declaration ambiguity was. The grammar grew unpleasantly large as productions were elaborated to create sub-productions without ambiguities. Back-tracking was introduced to support sequential rather than concurrent consideration of alternatives.

Since FOG was then only analysing declarations and meta-statements, the major C++ ambiguities did not arise, only minor problems and implementation inconveniences. It was not necessary to perform semantic correction of syntactic errors, the parse-declarations-ﬁrst policy disambiguated adequately.

2. Some very minor exceptions are listed in Section 3.3.

This summarises the evolution leading up to the previous implementation of FOG which uses back-tracking to resolve all ambiguities. Semantic leakage is limited to template names; type information is not used.

**Superset**

Consideration of the performance overheads associated with marking and backtracking for every meta-statement, and of the validity of not using type information led to the more efﬁcient superset parsing approach described in this chapter. The grammar for this approach has been implemented, cross-checked for completeness against the C++ standard, and processed by both bison and yacc to show lack of fundamental ambiguity, and successfully used to parse C++ programs. The bison report ﬁle has been used to verify correct resolution of the 24 conﬂicts that result from the 4 residual C++ ambiguities, 1 introduced ambiguity, and 5 implementation artefacts. These ambiguities are summarised in the comment header of Appendix B and described in Appendix F.2.

The ambiguities and semantic corrections resulting from the lack of type information apply to this superset parsing approach, and to a lesser extent to the multi-pass parsing approach.

The back-tracking to perform a binary tree search to resolve the template name ambiguities applies to both approaches.

## 5.5 Analysis of the C++ Grammar

The parsing approach described in this thesis deviates from accepted practice. It is therefore necessary to justify that the approach is sound. In order to do this we must ﬁrst understand the standard problems and then identify any new problems before showing how the new approach resolves them. This requires a fairly detailed examination of some aspects of the C++ grammar and the introduction of a notation that supports reasoning about that grammar.

### 5.5.1 Notation

The dragon book [Aho86] describes two different notations for deﬁning languages.

**Regular Expressions**

The simple regular expression notation supports description of classes of character sequences and is the basis for the lex lexer generator. An identiﬁer can be deﬁned by

```
[A-Z_a-z][0-9A-Z_a-z]*
The expression starts with a character from the class containing the alphabetic
characters or underscore and continues with an arbitrary number of repetitions of
the second class that adds the numeric characters. This is a compact notation but
is unable to express recursion and so cannot describe the language of matched
nested braces ( {}, {{}}, {{{}}} etc. ). The inability to specify matching
delimiters prevents the use of regular expressions to deﬁne most (if not all)
programming languages; the use of matched parentheses to enforce arithmetic
precedence is almost universal.
```

**Context-Free Grammars**

```
A (Context-Free) Grammar (CFG) is used to describe a more complicated
language and is the basis of the yacc parser generator. A grammar is deﬁned by
rules that operate on the terminals and non-terminals of the language. Terminals
correspond to the input tokens, non-terminals appear as the left-hand side of
production rules involving terminals and/or non-terminals on their right-hand side.
The brace language may be deﬁned using { and } as terminals and braces and
grammar as non-terminals. grammar is the distinguished non-terminal that deﬁnes
the language.
grammar:
braces
|
{ grammar }
braces:
{ }
The grammar is speciﬁed using a Backus-Naur Form. Alternative rules producing
the same non-terminals are separated by |. When a clear multi-line formatting
policy is used as in the C++ standard, the | may be omitted. Rules are variously
referred to as production rules, or reduction rules.
```

The availability of intermediate non-terminals gives Context-Free Grammars much greater power than regular expressions. However the requirement to use multiple rules and the transformation of repetition into recursion makes it difﬁcult to reason about the grammar.

**Extended Regular Expression notation**

C++ comprises two relatively independent subgrammars, one to deﬁne expressions and another to deﬁne declarations. It is well known that there are sentences such as.

```
int (var);
that are ambiguous. It could be a functional cast of the variable var to an integer
value or a declaration of the redundantly parenthesised var as an integer.
Identifying these ambiguities in the grammar is difﬁcult because a few hundred
inter-related rules are not a convenient representation for logical reasoning.
```

We need to be able to substitute one rule in another in order to derive the rules that identify each non-terminal with respect to terminals, or relatively fundamental non-terminals. We will therefore extend regular expressions so that C++ syntax can be represented. We can then represent an expression as one extended regular expression, a declaration as another, and identify the ambiguities by comparing their terms.

Both regular expressions and context-free grammars can describe the concatenation of lexically adjacent elements, but only context-free grammars support a complex ordering through nesting of non-terminals. We therefore introduce •, a functional operator to support more arbitrary ordering in regular expressions.

The • operator is always applied to a speciﬁc argument and so associates from right to left. This of course differs from the composition operation in functional languages, since we are interested in successive application, not in function composition.

The • operator has higher precedence than lexical concatenation. Thus:

α Z • γ ;

```
represents α concatenated with the application of Z to γ. If Z denotes application
of braces, and α and γ are the identiﬁers a and g, the above expression denotes
the sentence
a { g } ;
```

The nested braces language, using ε as the empty set of sentences, is

Z•ε { } Z•Z•ε { { } } Z•Z•Z•ε { { { } } } Z•Z•Z•Z•ε { { { { } } } } etc.

which we abbreviate to

Z+•ε

More practically, we represent the pointer preﬁx of a declarator by Pd(ζ) and the

```
array sufﬁx of a declarator by Ad(η?), where ζ is a free symbol denoting a ptr-
operator, η is a free symbol denoting an assignment-expression, and ? is the standard
regular expression operator denoting 0 or 1 of.
Pd(X::y::*const)•Ad(5)•v
```

therefore denotes

```
X::y::*const v [5]
```

The subscripts in these names form part of a compact naming policy: P for pointer, A for array, Pd for a pointer valid for use in a declarator, Ae for an array valid for use in an expression.

Since the parenthesised parameterization is not usually signiﬁcant in ambiguity reasoning, we can refer more simply to

Pd•Ad•θd

```
where θd is any name valid in a declarator.
```

The full notation is summarised in the following tables

There are no operators that apply to more than one operand, although operators such as Pd may take additional parenthesised parameters.

The inﬁx operator Oi applies to one operand that denotes two (or three) terms, each independently of the same form as the one operand. Thus, a particular

a. See Section 5.5.2.1.

(Non-)Terminal Notation Description or example

Free (untyped) symbols α

```
Built-in type (one word)
β
int
Declaration
δ
simple-declaration
```

The empty set ε

```
Pointer Type
ζ
ptr-operator
Assignment-expression
η
assignment-expression
Name in declaration
θd
declarator-id
Name in expression
θe
id-expression plus a bita
```

Name other than local destructor θ

```
Constant-expression
κ
constant-expression
Character, Number or String
λ
literal
Parameter-Declaration
π
parameter-declaration
```

Parameter-Declaration and Assignment-Expression

ρ ρ = η ∩π

```
Type-name in declaration
σd
size_t
Simple-type-speciﬁer
σe
simple-type-speciﬁer
Type
τ
type-id
```

Generalised Assignment-Expression χ χ ⊇η

Generalised Parameter-Declaration or Assignment-Expression

ω ω ⊇η ∪π

**Table 5.1 Terminals and Non-Terminals**

```
lexical presentation of the tertiary Oi•η is "test ? 5 / 8 : 6", since each of
"test", "5 / 8" and "6" are assignment-expressions.
```

The list operator L uses a pre-superscript rather than a post-superscript to highlight the distinction between the repetition of the operand many times and multiple application of the operator. The operand is repeated with no constraint between the operands. Thus 2L•Z?•α covers the 4 possibilities of Z present or absent independently for each operand whereas Z?•2L•α covers only the 2 possibilities of Z jointly present or absent.

Overloading of lexical tokens and other simple lexical properties lead to the properties shown in Table 5.4.

### 5.5.2 C++ Grammar Properties

```
This notation will now be applied to analyze the declaration and expression
syntaxes. The analysis is a little lengthy and ignores a number of peripheral
syntaxes that do not contribute to ambiguities. Thus ..., which is unique to a
parameter-declaration-clause, and sizeof(), which is unique to an expression are
excluded from the analysis.
```

#### 5.5.2.1 Names

Before we can analyse the grammar, and show that the new approach is compatible, we must ﬁrst understand how names are used in the standard grammar.

When a variable is declared, it complies to a syntax that simpliﬁes a little to:

```
decl-speciﬁer-seq declarator = initializer ;
The decl-speciﬁer-seq comprises the miscellaneous qualiﬁers such as static or
extern and a type-speciﬁer-seq that provides the type such as const unsigned int
(but no pointers). The declarator comprises a name in the form of a declarator-id
potentially wrapped up with pointer preﬁxes, function and array sufﬁxes and
clarifying parentheses.
When a variable is used in an expression, it complies to the id-expression syntax.
```

Operator Notation Description

Independent instances α1, α2, α3, α4

Zero or one Z?•α α or Z•α

Zero or more Z*•α α or Z•α or Z•Z•α or Z•Z•Z•α or etc..

One or more Z+•α Z•α or Z•Z•α or Z•Z•Z•α or Z•Z•Z•Z•α or etc..

Exactly one of α1 or α2

Comma separated list of L•α 0L•α is ε 1L•α is α 2L•α is α1 , α2 3L•α is α1 , α2 , α3 etc.

*L•α / +L•α have at least 0 / 1 elements

Covers/Contains α1 ⊆α2 Every sentence of α1 is contained in α2

Strictly Covers / Contains

α1 ⊂α2 Every sentence of α1 is contained in α2, and some sentence of α2 is not contained in α1

Intersection α1 ∩α2 Sentences common to α1 and α2

Union α1 ∪α2 Sentences of α1 or α2

**Table 5.2 Mathematical Operators**

α1 α2      

```
The id-expression and declarator-id are therefore the main name concepts, that have
subtle but signiﬁcant differences. Understanding the distinctions from the C++
grammar is quite hard, so the same information is presented in graphical form in
Figure 5.3. The diagram is a kind of Venn diagram in which different forms of name
are arranged in four columns and thirteen rows. Shaded areas indicate the
coverage of each production.
```

Four columns represent the possible scope nestings of a name:

```
•
unscoped name (e.g. name)
•
nested name (e.g. Nested::name)
•
global scope nested name (e.g. ::Nested::name)
•
global name (e.g. ::name)
a. The ={} form of initializer is incorporated directly into a generalised Oi to
```

slightly reduce the number of distinct declarator / expression operators.

Operator Notation Lexical Example

Array declarator sufﬁx Ad(κ?)•α α [ ]

Array expression sufﬁx Ae(+L•η)•α α [ η ]

Array for declarator and expression Ade(κ)•α α [ κ ]

Array for declarator or expression A(*L•η)•α α [ η ]

Parenthesis Brackets B•α ( α )

Cast expression C(τ)•α ( τ ) α

```
decl-speciﬁer preﬁx
D•α
extern α
exception-speciﬁcation sufﬁx
E•α
α throw ( )
```

Function declarator sufﬁx E?•V?•Fd(*L•π)•α α ( π1 , π2 ) const throw ( )

Function call sufﬁx Fe(*L•η)•α α ( η1 , η2 )

Function call and declarator Fde(*L•ρ)•α α ( ρ1 , ρ2 )

Function call or declarator E?•V?•F(*L•ω)•α α ( ω1 , ω2 ) const throw ( )

```
Assigned initializer
I(η)•α
α = η
α = { 5 }
```

Constructed initializer J(+L•η)•α α ( η1 , η2 )

```
(Non-pointer) Preﬁx operator
Op•α
++ α
Inﬁx operatora
Oi•α
α1 / α2
α = { 5 }
α1 ? α2 : α3
Sufﬁx operator
Os•α
α ++
ptr-operator
Pd(ζ)•α
* volatile α
& α
Class::* const α
Pointer for expression
Pe(ζ)•α
* α
& α
Uniﬁed Pointer operator
P(ζ)•α
*cv-qualiﬁer-seqopt α
& α
D+•ε ::*cv-qualiﬁer-seqopt α
type-speciﬁer preﬁx
T•α
int α
cv-qualiﬁer-seq sufﬁx
V•α
α const
```

**Table 5.3 Grammatical Operators**

Thirteen rows represent each of the different categories of name. The top 8 rows correspond to type-names:

```
•
...decl-speciﬁer, keywords such as virtual, static and friend
•
elaborated-type-speciﬁer, an enum or class reference
•
class-speciﬁer, a class deﬁnition
•
enum-speciﬁer, an enum deﬁnition
•
cv-qualiﬁer, const or volatile
•
β, a single word built-in type such as int
•
template-id (e.g. FixedSizeArray < 4 >)
•
identiﬁer (e.g. MyType)
```

The bottom ﬁve rows denote non-type names:

```
•
template-id (e.g. sort < int >)
•
identiﬁer (e.g. my_variable)
•
operator-function-id (e.g. operator+=)
•
conversion-function-id (e.g. operator MyClass **)
•
destructor name (e.g. ~MyClass)
identiﬁer
operator-function-id
conversion-function-id
~ class-name
template-id
name
Nested::name
unqualiﬁed-id
qualiﬁed-id
id-expression
θe = name part of a primary-expression
::name
::Nested::name
```

**Figure 5.3 C++ Names**

```
θd = declarator-id
enum-speciﬁer
class-speciﬁer
cv-qualiﬁer
type-name
```

β

```
elaborated-type-speciﬁer
...decl-speciﬁer
template-id
identiﬁer
```

θ = generalised name

```
D•ε = decl-speciﬁer
T•ε = type-speciﬁer
σe = simple-type-speciﬁer
σd = type part of declarator-id
enum X {}
class X {}
class X
virtual
const
int
Figure 5.3 shows that qualiﬁed-id grammar production covers all categories of
nested non-type-name, and that a declarator-id covers all possible non-type-names
and user-deﬁnable type-names.
The irregular shape of the θe contributions to a primary-expression is the source of
many difﬁculties in implementing the parser grammar. Reduce-reduce conﬂicts
arise from a need to commit to a declarator-id or primary-expression before sufﬁcient
lookahead context has been examined. Part of this is just a trap for the unwary
implementor. Since θe ⊂θd, no conﬂict need arise. It is just the unhelpful way the
grammar is written that is a problem.
```

**Name differences between declarator and expression**

```
A type-name is not generally valid in an expression. However, a speciﬁc variant of
function call in the syntax for postﬁx-expression, supports use of a type-name as the
function name, and serves to invoke a constructor or functional cast.
Omission of an unscoped destructor name from id-expression resolves the ambiguity
between the one’s complement operator and a destructor for
~non_class_name & 7;
```

and gives the correct interpretation (§5.3.1-9) of

Notation Description

σd ⊂σe ⊂ T•ε ⊂ D•ε Application type-names

β ⊂σe ⊂ T•ε ⊂ D•ε Built-in type-names

σd ⊂θd Declarator type-names

θe ⊂θd Non-type-names

V•ε ⊂ T•ε

Ade•α ⊂ Ad•α , Ade•α ⊂ Ae•α

A•α ⊃ Ad•α , A•α ⊃ Ae•α

Fde•α ⊂ Fd•α , Fde•α ⊂ Fe•α

E?•V?•F•α ⊃ Fd•α , E?•V?•F•α ⊃ Fe•α

I•α ⊂ Oi•α

Pde•α ⊂ Pd•α , Pde•α ⊂ Pe•α

P•α ⊃ Pd•α , P•α ⊃ Pe•α

B•τ = C(τ)•ε = F(τ)•ε Parenthesis / cast null / abstract function

B•*L•π = Fd(*L•π)•ε Parenthesis / abstract function

B•*L•η = Fe(*L•η)•ε Parenthesis / null-call

B•*L•ω = F(*L•ω)•ε Parenthesis / null-call or function

Preﬁx and sufﬁx operators commute

**Table 5.4 Properties**

Op

P D T               Os

A F E V                  

α

Os

A F E V                  

Op

P D T              

α

- =

```
~ClassName();
Omission of a global conversion-function-id from id-expression is semantically correct,
but represents a needless syntactic complexity, since an ambiguity resolution of
something-ending-in-:: conversion-function-id
exploiting the syntactic exclusion requires something-ending-in-:: to be
meaningful. The only construct ending in :: is a nested-name-speciﬁer, whose
presence contradicts the presence of a global name.
Omission of a global destructor name from id-expression is similarly semantically
correct, but syntactically redundant.
The same argument applies to a global template-id, however in this case, it would
appear that the omission is an error. Given:
template <class T> void sort(T *anArray, size_t arraySize);
Refusal to permit the hopefully redundant :: in
p = &::sort<int>;
```

seems unreasonable.

**Conclusion**

```
The global name exclusions from a primary-expression can be ignored syntactically.
A subsequent semantic check may yield a helpful diagnostic. Only the omission of
the local destructor need be honoured.
Once the distinction between
type-name and
identiﬁer is removed, the local
destructor exclusion is the sole difference between a declarator-id and a name in a
primary-expression. The generalised name coverage shown as θ is used for both
purposes in Section 5.7. This covers some syntactically meaningless names in an
expression, but misses out the local destructor from a declarator-id. The omission
will be covered by a complement expression and must be repaired semantically.
```

#### 5.5.2.2 Declarators, Declarations and Type Identiﬁers

The C++ grammar deﬁnes

```
declarator:
direct-declarator
ptr-operator declarator
direct-declarator:
declarator-id
direct-declarator ( parameter-declaration-clause )
cv-qualiﬁer-seqopt exception-speciﬁcationopt
direct-declarator [ constant-expressionopt ]
( declarator )
It is the two level recursion between these productions that makes them difﬁcult
to understand. Two levels are required because a CFG cannot express both preﬁx
and sufﬁx elaboration in the same production unambiguously. Considering:
afﬁxed-production:
terminal
prefix afﬁxed-production
afﬁxed-production suffix
It is unclear whether the preﬁx or sufﬁx production rule is reduced ﬁrst in:
prefix terminal suffix
In the extended regular expression notation outlined above, functional operators
are used for lexical preﬁxes, sufﬁxes and their combinations. The preﬁx-sufﬁx
ordering ambiguity is therefore removed and all forms of lexical decoration can be
expressed uniformly. We may express the declarator syntax as
```

The recursion in the ﬁrst choice can be simpliﬁed:

and then substituted in the second to give

Applying the same analysis to other parts of the C++ grammar we ﬁnd that

```
declarator
direct-declarator
Pd
declarator
⋅






=
direct-declarator
```

θd

```
E ?
V ?
•
F
•
d
direct-declarator
⋅
Ad
direct-declarator
⋅
B declarator
⋅














```

=

```
declarator
Pd
*
direct-declarator
•
=
direct-declarator
```

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

```
declarator
Pd
```

* E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

```
abstract-declarator
Pd
```

*

E ? V ?

- F

- d Ad

B Pd

*

-          

* E ? V ?

- F

- d Ad

B Pd

+

-          

Pd              

ε

- =

```
type-id
τ
T
=
+
Pd
•
*
E ?
V ?
•
F
•
d
Ad
```

B Pd

*

-          

* E ? V ?

- F

- d Ad

B Pd

+

-          

         

?

ε

- =

```
new-type-id
T +
Pd
•
*
Ad
```

* ε

- =

```
init-declarator
I
J



```

?

Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

where

```
(The usage of θd in a parameter-declaration is syntactically correct although semantic
constraints allow only an identiﬁer.)
```

Ignoring the optional ellipsis which is not a source ambiguity:

```
Generalising slightly by ignoring the constraint that a decl-speciﬁer-seqopt applies
only to the ﬁrst element of an init-declarator-list:
```

#### 5.5.2.3 Expressions

```
Analysis of an expression omitting uniquely preﬁxed terms such as new or
const_cast that may be conveniently considered to be part of λ leads to
While analysing preﬁx operations, we lump the more mundane operators such as
++ under Op, but keep the potentially ambiguous pointer operators * and & as Pe.
init-declarator-list
L
+
I
J



```

?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

```
parameter-declaration
π
=
```

π D = + I?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

ε                  

```
parameter-declaration-clause
B
L
*
π
•
•
=
simple-declaration
δ
D*
L
*
•
I
J



```

?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- = =

```
primary-expression
```

θe

```
λ
B
expression
•










```

=

```
postfix-expression
primary-expression
Ae
postfix-expression
•
F e
postfix-expression
•
```

F e σe

```
Os
postfix-expression
•


















```

Ae F e Os          

* θe

λ F e σe

```
B
expression
•














```

- = =

```
Simplifying the nested arbitrary choice, and substituting the postﬁx-expression
```

The ten levels of precedence for binary operators, tertiary operator and assignment are not signiﬁcant to this analysis. All such operators are represented by Oi and we may write

The doubly nested one or more choices permit arbitrary ordering

```
unary-expression
postfix-expression
```

Op

```
Pe






cast-expression
•










```

=

```
cast-expression
unary-expression
C
cast-expression
•






C*
unary-expression
•
=
=
```

C* Op

Pe       C*

-      

```
*
•
postfix-expression
•
=
cast-expression
```

Op

Pe

C          

* Ae F e Os          

* θe

λ F e σe

```
B
expression
•














```

- =

```
assignment-expression
η
Oi
```

* Op

Pe

C          

* Ae F e Os          

* θe

λ F e σe

```
B
expression
•














```

- = =

```
expression
expression-list






L
+
Oi
•
*
Op
```

Pe

C          

* Ae F e Os          

* θe

λ F e σe

```
B
expression
•














```

- =

L + Oi

- * Op

Pe

C          

* Ae F e Os          

*

- B L +

- Oi

- * Op

Pe

C          

* Ae F e Os          

*

         

*

θe

λ F e σe

-          

- =

and the arbitrary ordering subsumes the multiplicity of preﬁxes

### 5.5.3 C++ Ambiguities (using type information)

We have derived relatively simple formulae for the major declaration and expression constructs. The utility of this representation will be shown by deriving formulae that describe the conventional C++ ambiguities, when type information is available. This derivation can then be revisited to assess the consequences of parsing without type information.

```
The major ambiguities occur between and within declarations and expressions,
since these syntaxes lack unique keywords. Ambiguities in statement syntax are
isolated, since a leading if isolates the syntax from all other syntax (but not from
the dangling else problem).
```

We concentrate on declarations and expressions since these are the sources of problems

- as many other implementors have already discovered

- as any attempt to implement a C++ grammar with yacc detects

#### 5.5.3.1 Declaration / Declaration Ambiguity

A declaration / declaration ambiguity arises when

```
expression
expression-list






L
+
Oi
•
*
Op
```

Pe

C          

* Ae F e Os          

*

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

θe

λ F e σe

-          

- =

```
expression
expression-list






L
+
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

θe

λ F e σe

-          

- =

```
simple-declaration
δ
D*
L
*
•
I
J



```

?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- = =

is open to more than one interpretation. Multiple interpretations are possible because there is lexical overlap between the different operators.

D*•θd and D*•Pd•θd

```
A declarator-id (θd) or ptr-operator (Pd) may start with a :: which may be ambiguous
with respect to the last name in a decl-speciﬁer (D).
Class ::Scope::p
Class ::Scope::* p
// A pointer to member
This ambiguity is not explicitly resolved in the standard, but a resolution may be
inferred from the requirement to maximise the length of a decl-speciﬁer-seq (§7.1-2).
The resolution guarantees a semantic error.
Class::Scope::p
// Error name but no type
Class::Scope ::* p
// Error illegal ptr-operator
```

Ad•θd or θd

```
An array declarator may add a [] sufﬁx to a name.
The names operator new[] and operator delete[] end in [], and the names
operator new and operator delete exist. It is therefore unclear whether
int operator new[];
```

declares an array or a scalar. Since neither alternative is semantically valid, the syntactic problem is academic.

Fd•θd or θd

```
A function declarator may add a () sufﬁx to a name.
The name operator() ends in (), but there is no name operator so there is no
ambiguity.
```

Fd(1L•π)•θd or D•B•π

The overloaded usage of parentheses leads to an ambiguity between

- Fd(1L•π)•θd - a single argument constructor declaration

- D•B•π - a redundantly parenthesised variable declaration

for:

```
T(a)
```

Trimming δ to match these two forms

The initializers, array and function sufﬁxes can be dismissed using the semantic constraints on a constructor, leaving

δc

I J   

? E ? V ?

- F

- d Ad      

* F d πc ( ) θd

- =

δv D I J   

?

- E ? V ?

- F

- d Ad      

* B

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

δc F d πc ( ) θd

- =

The ambiguity arises for , which is non-trivial when the name preceding the

parentheses is (see Figure 5.3). The inside of the parenthesis is

ambiguous when the parameter

is ambiguous with respect to

Trimming impossible terms gives the two constraints on the parenthesised ambiguity in

```
TypeName(π)
```

is a simple solution to .

A more complicated solution arises through the recursive ambiguity between precedence and function argument parentheses.

leading to the complete form of the constructor / parenthesised variable ambiguity

δv D B Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

δc δv ∩

σd θd D ε

- ( ) ∩ =

π D = + I?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

ε                  

Pd

* E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

πc

D+

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

D+ ε

-                      

=

πv

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

σd θd D+ ε

- ( ) ∩ = πcv πc πv ∩ =

E ? V ?

- F

- d Ad      

* F d

E ? V ?

- F

- d Ad      

* σd

-       σd

J(*L•η)•θd or D•B•*L•π

Parenthesis overloading would also appear to lead to an ambiguity between construction of an object and a redundantly parenthesised variable. However, construction of an object requires an explicit type and so the object construction must at least be of the form D•J(*L•η)•θd. The ambiguity of this with respect to a function declaration is considered next.

D*•J(*L•η)•θd or D*•Fd(*L•π)•θd

There is a lexical ambiguity between

- D*•J(*L•η)•θd - a constructed object declaration

- D*•Fd(*L•π)•θd - a function declaration

for:

```
T a(b), c(d), e(f);
Semantic constraints permit and require ambiguous decl-speciﬁer-seq preﬁxes for
the ﬁrst element of an
init-declarator-list, but require no
decl-speciﬁer-seq for
subsequent elements. The D and L terms are therefore eliminated to give the
following forms for each alternative simple-declaration.
```

These two terms are a direct match syntactically, subject to the recursive ambiguity to satisfy the parameter lists. This is Fde(*L•ρ) and is analysed in the next section. The ambiguity is therefore:

#### 5.5.3.2 Declaration / Expression ambiguity

The declaration / expression ambiguity arises when a

```
followed by a semi-colon, forming part of a statement, is ambiguous with respect to
```

δcv F d

E ? V ?

- F

- d Ad      

* F d

E ? V ?

- F

- d Ad      

* σd

-      

-      

?

σd

-       σd

- =

δo J L * η

- ( ) Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

δf Pd

* F d L * π

- ( )

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

δof Pd

* F de L * ρ

- ( )

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- =

```
simple-declaration
δ
D*
L
*
•
I
J



```

?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

*

θd

- = =

```
which when followed by a semi-colon is an expression-statement.
```

The expression / declaration ambiguity may be identiﬁed by comparing terms to give

D0 matches the absence of D in an expression.

+L enforces the expression requirement for at least one element.

Commutativity of preﬁx and sufﬁx operators permits the intervening pointer to be traversed by the initializers so that: I resolves the ambiguity between I and Oi since I ⊂Oi. J is covered by an Fe in an expression.

Pe uniﬁes Pe and Pd acquiring the Pe from the expression multiplier term.

Fde is the recursive ambiguity between Fd and Fe analysed below.

```
Ade resolves the ambiguity between Ad and Ae restricting the array argument to
exactly one constant-expression.
```

1L is the identity operator necessary to cover B•Pd * by successive B•+L and Pe terms from the expression multiplier term.

θe resolves the ambiguity between θd and θe since θe ⊂ θd.

The Fde•σe terms arises because σe ⊂ θd.

A quick test of this formula makes the prediction that

```
i = 0;
```

should be ambiguous, although such a simple ambiguity is not mentioned elsewhere, and resolution as a declaration would reject most C++ programs. The reason is that the analysis above is purely syntactic. In the production

```
simple-declaration:
decl-speciﬁer-seqopt init-declarator-listopt ;
the strict syntactic interpretation permits the decl-speciﬁer-seq (the type) to be
omitted. i = 0 is a valid form of init-declarator-list. An untyped name alone is
therefore syntactically ambiguous. Since §6.8-3 prohibits the use of more than
semantic type information to disambiguate, there appears to be a problem. The
problem disappears if the constraint in §7-7 is interpreted as a syntactic rather
than semantic constraint. The constraint speciﬁes that a decl-speciﬁer-seq may only
be omitted for function-like declarations.
expression
L
+
η
•
L
+
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

θe

λ F e σe

-          

- = =

δ L + η

- ∩ D0 L +

- I J   

?

- Pe

*

F de Ade

B L 1 Pe

- *

-          

* θe F de σe

-      

- =

```
Incorporating this constraint, together with the constraint on no implicit int
functions, eliminates the possibility of the Pe preﬁxes.
```

The constraint that functions are not constructed eliminates the J.

```
The I? covers only the = 0 of a pure-speciﬁer and can be eliminated since there is
no D preﬁx to supply a virtual keyword.
θe covers only destructor names and conversion-function-ids, which cannot be
declared where expression statements are valid.
```

Functions returning arrays or functions are invalid.

Application of semantic constraints therefore reduces the ambiguity to the more familiar:

```
Continuing the analysis to determine the recursive ambiguity. The Fde ambiguity
arises when a
parameter-declaration-clause is ambiguous with respect to a
parenthesised expression-list. This occurs when each parameter-declaration
```

is ambiguous with respect to the corresponding

```
The presence of D+ in the parameter-declaration would appear to preclude an
ambiguity, however σe ⊂ D•ε. Trimming completely unsuitable terms and arranging
```

to exploit the lexical commutation gives the solution as

δ L + η

- ∩ L + I?

- F de Ade      

* F de

θe σe      

- =

δ L + η

- ∩ L + F de

θe σe      

- =

π D = + I?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

ε                  

```
assignment-expression
η
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

=

*

θe

λ F e σe

-          

- =

D F

- ε

- F D ε

- =

This simpliﬁes to give

```
as the form of an argument of Fde that is ambiguously either a parameter-declaration
or an expression. This ambiguity is not only recursive, but also exhibits multiplicity
in its recursion.
```

Performing a sanity check: the simplest form is

```
TypeName()
which is recognisable as an expression involving construction of a TypeName. That
it is also a parameter-declaration requires understanding of a very dark corner of C++
(§8.3.5-3). The declaration interpretation is of an abstract (unnamed) function
taking no arguments and returning TypeName. This has no meaning in C++, since
functions are not ﬁrst class entities. Function names are interpreted as pointers
to functions and so the example is equivalent to:
TypeName (*)()
```

#### 5.5.3.3 Expression / Expression ambiguities

The foregoing analysis has taken little account of ambiguity between operators. Operators are excessively overloaded: parentheses variously denote a cast, a function-call or arithmetic grouping. We therefore analyse an assignment expression in terms of its lexical layout eliminating the application operator and

using only lexical adjacency. For this purpose we introduce ~ so that denotes a comma separated list of zero or more elements of α. Punctuation such as (, ), [, and ] represent the lexical character.

The lexical production rule for an assignment expression is:

```
The use of [η] for an array is almost unique to indexing an array and so creates no
expression ambiguities. The sole other use occurs in operator new [] and
operator delete [] where the absence of η disambiguates.
```

ρ π η ∩ D I?

- F d Ad      

* F d ε

Ae F e

Oi          

*

F e σe

- ∩ = =

ρ π η ∩ I? F de Ade      

* F de σe

- = =

~α

η

η η [ ] array

η η˜ ( ) call ηOs suffix

Opη prefix

τ ( )η explicit-cast ηOiη infix

Pη pointer

η˜ ( ) parenthesis θe name

λ value

σe η˜ ( ) functional-cast                                          

→

```
The () of calls, casts, parentheses and function-casts may create
```

- parenthesised-call / cast-parenthesis

- parenthesised-binary / cast-unary

- call / functional-cast

ambiguities.

The parenthesised-call / cast-parenthesis ambiguity is

and the parenthesised-binary / cast-unary is

There are ambiguities when η2 = η3 and η1

```
~ = τ and Oi is +, −, ∗ or &. Further
ambiguities exist when Oi is ++ or -- and η2 and η3 are parenthesised.
The ambiguities arising when η = τ are analysed in Section 5.5.3.4 as the type-id /
assignment-expression ambiguity.
```

The call / functional-cast ambiguity analysed in Section 5.5.3.5 does not arise when type-name information is available.

```
Some sufﬁx operators such as ++ are also preﬁx operators. No ambiguity can
arise because sufﬁx operators cannot precede a preﬁx operator.
Some inﬁx operators such as + are also preﬁx operators, while others such as *
are also pointer operators. No ambiguity arises since, in an expression such as
a * * * b
the absence of a further sufﬁx ambiguity ensures that the ﬁrst * must be inﬁx and
subsequent *’s preﬁx. However, a conversion-function-id is covered by θe and may
end in a * creating this further sufﬁx ambiguity.
&Class::operator int* * *pointer
This is resolved by language deﬁnition (§12.3.2-4) to maximise the length of the
conversion-function-id: a resolution that can never avoid a subsequent semantic error.
An equivalent ambiguity within a new-expression is similarly resolved (§5.3.4-2).
```

θe and λ are independent and cause no ambiguity, beyond those already discussed.

#### 5.5.3.4 type-id / expression-list ambiguity

A parenthesised-call / cast-parenthesis or parenthesised-binary / cast-unary ambiguity exists when the

of the cast may be confused with the

~η

1     ~η

2     τ ( ) η3 ( ) ∩

~η

1    Oi η2 τ ( )

Op

Pe      η3 ∩

```
type-id
τ
T
=
+
Pd
•
*
E ?
V ?
•
F
•
d
Ad
```

B Pd

*

-          

* E ? V ?

- F

- d Ad

B Pd

+

-          

         

?

ε

- =

```
of the parenthesised call as (a) in
(a)(b)
(a)-b
(a)++(b)
```

The T+ term can only match the σe, since σe ⊂ T•ε, leading to the solution

which again makes use of the strange equivalence of a function and pointer to function to determine that type information alone is insufﬁcient to disambiguate

```
(Class())(x)
// Cast to function
```

which could be

```
Class().operator()(x)
// Call of operator()
```

or

```
(Class (*)())(x)
// Cast to pointer to function
```

A cast to function is not one of the recognised forms of cast enumerated in §5.4, and so the ambiguity has a well-deﬁned semantic resolution, which may be used to avoid the syntactic ambiguity.

#### 5.5.3.5 call / functional-cast ambiguity

```
The call / functional-cast ambiguity arises when the type-name σe in a functional-
call can be mistaken for an assignment-expression. A type-name is not valid as an
assignment-expression so there is no call / functional-cast ambiguity when type-name
information is available.
```

## 5.6 Parsing the ambiguities

We have used an extended regular expression notation to derive the declaration and expression ambiguities in C++ syntax, in a way that is not possible with the standard regular expression or C++ grammar notations. We now examine how ambiguities may be resolved in a practical parser.

If the BNF provided as part of the C++ standard is converted directly into a yacc grammar, yacc reports many ambiguities, mostly relating to declarations and expressions. The expression / declaration ambiguities are by far the most serious and much the hardest to resolve.

```
expression
expression-list






L
+
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

θe

λ F e σe

-          

- =

τ L + η

- ( ) ∩

Ade F de      

* F de σe

- =

We now consider the traditional approach to resolving an ambiguity, a multi-pass approach formerly adopted by FOG, and a more efﬁcient superset grammar approach.

### 5.6.1 Parsing against an ambiguity (the traditional approach)

An ambiguity may be resolved within the grammar, by rewriting the grammar to remove the ambiguity, or by providing assistance in the form of disambiguation tokens from a lookahead parser. The lookahead approach is straightforward, but just redeﬁnes the problem as one to be solved elsewhere, potentially using ad hoc code that may be prone to incorrect programming assumptions resulting from the complexity of a recursive ambiguity.

Removing the ambiguities within the grammar is very hard. Given two mutually ambiguous subgrammars A and B, it is easy to see that the ambiguity is removed by identifying the ambiguity AandB comprising all sentences that could form part of A and could form part of B. The ambiguous case may then be removed from each of the original subgrammars to leave OnlyA and OnlyB. The revised grammars comprising OnlyA, AandB and OnlyB are free from the A B ambiguity.

The analyses in the previous section show how complex grammars could be analysed, enabling AandB to be identiﬁed from A and B, and show that in the case of the expression / declaration ambiguity, the ambiguity has to be expressed recursively. Converting AandB back into BNF rules is relatively straightforward. However OnlyA and OnlyB require a subtraction of formulae and consequently result in very complicated expressions for OnlyA and OnlyB. Determining these in the non-recursive context is somewhat daunting. An accurate recursive resolution of expression-that-is-not-a-declaration is a major undertaking.

For the C++ declaration / expression case, the other subtraction leading to declaration-that-is-not-an-expression does not need to be evaluated, because the disambiguation rule (§6.8) mandates that the ambiguity be resolved as a declaration. It is sufﬁcient to parse the declaration unchanged as A and the expression-that-is-not-a-declaration as OnlyB.

Once unambiguous formulae have been identiﬁed, they then need to be converted to BNF in a way that does not require more than one token of lookahead. It is not sufﬁcient to convert the formulae independently. They must be converted together so that no shift-reduce or reduce-reduce conﬂicts are introduced when the parser has seen a partial input that could preﬁx more than one alternative.

In conventional C++ parsers, the use of type information resolves nearly all ambiguities, so that the shared preﬁx constraint is the hard problem.

The relatively arbitrary nature of the gcc, CPPP and Roskind grammars suggest that an approximate solution to the above problems was discovered empirically. The problem is too complex to be amenable to an empirical approach. Both the gcc and CPPP grammars are reported to fail to correctly resolve more complicated declaration / expression ambiguities.

In summary, parsing against an ambiguity requires an accurate implementation of the grammar. This is hard to achieve for a deep and recursive ambiguity. [Roskind91] describes his solution as “A LOT of work” and notes that some

A B      

OnlyA AandB      

AandB

OnlyB      

                 

OnlyA AandB

OnlyB          

A OnlyB       = = =

ambiguities are resolved prematurely. The gcc implementation was also not easily reached.

### 5.6.2 Parsing without an ambiguity (the multi-pass approach)

Expressions and declarations are disambiguated by preferring a declaration to an expression whenever there is an ambiguity (§6.8). A two-pass parser can therefore be designed that ﬁrst parses for a declaration, and if that parse fails, then parses for an expression.

This makes for a much simpler grammar implementation since no grammar revision is required to remove the ambiguity. We just need to support the ability to perform multiple passes.

Back-tracking in the context of a parser involves examining the input token stream to see whether the stream satisﬁes a candidate syntax, and if not backing up again to try another candidate.

This practice is common in hand-written parsers, which normally use a top-down left-most reduction at the left policy (LL). Examining the left of a production tends to make premature decisions that then need to be undone.

Use of derivation at the right in an LR parser avoids premature decisions and can make back-tracking unnecessary. Generally, back-tracking is undesirable, since work performed upon each backed-up path is wasted. Well-structured grammar code does not need to back-track.

The standard parser tools do not support back-tracking, and so the implementation of back-tracking presented in Section 5.8 may be novel.

**Cost**

Accurate determination of the cost of back-tracking requires instrumentation of a parser that adopts both approaches. Such a parser has not been developed, so we can only estimate the likely costs.

Back-tracking incurs three costs:

- marking and unmarking a restart position (always)

- restarting at the mark (only when a back-track necessary)

- wasted analysis effort (only when a back-track necessary)

Maintenance of the marked position need not be particularly costly, if each token is already represented by a polymorphic object, but may be more noticeable if the tokens would otherwise have been acquired directly as a binary stream from a preprocessor.

FOG maintains a garbage collector context at each mark, so re-establishing the mark not only back-tracks on the input context, but also destroys any unwanted objects created to support the failed analysis. This cost will be low, since a failed analysis will normally fail before creating many objects.

The wasted analysis effort will also be small, since an illegal syntax will usually fail after only a few tokens.

```
Back-tracking incurs no costs for syntax such as selection-statements that can be
identiﬁed from their ﬁrst token. Costs remain small for syntax that corresponds to
the ﬁrst analysis alternative. The costs only become signiﬁcant for expression-
statements that closely resemble declaration-statements and so cause signiﬁcant
wasted effort.
```

A B       A or else B =

The parsing cost is estimated to increase by 20%, which is undesirable for a production compiler, but justiﬁable in terms of the improved modularity for a research tool.

The approach recommended in the next section reduces this 20% estimate to a negligible level, and so no attempt at accurate measurement has been made.

### 5.6.3 Parsing with the ambiguity (the superset approach)

A design philosophy of C is the principle that declarations imitate the style of their usage. The declaration syntax is therefore deliberately rather than accidentally and inconveniently similar to the expression syntax. This property can be exploited to develop a superset syntax that encompasses both declarations and expressions.

AorB covers all sentences that satisfy A and all sentences that satisfy B. Deriving AorB involves adding the relevant formulae and pruning any duplication. In order to simplify the grammar we may choose to add further terms to AorB provided we do not introduce any new ambiguities, or at least provided we can resolve any new ambiguities that we do introduce. We require the grammar for AorB to provide cover for at least A and at least B. We do not require precise equivalence.

The consequence of choosing to make AorB larger is that some sentences that were formerly syntax errors, are now accepted by the superset grammar. These sentences should be diagnosed in a later semantic analysis. This is actually beneﬁcial, since the extra sentences that are accepted have a close similarity to legal sentences and so cover likely programming errors. Accepting such errors syntactically improves the likelihood that a diagnostic will report an error that is relevant to the programmers intent. For instance given

```
typedef type a virtual;
many compilers may succeed in diagnosing an illegal typedef. The greater
syntactic coverage may allow the compiler to accept the typedef initially but report
that virtual is not a legal qualiﬁer for the name of a typedef.
```

With such a superset grammar there is then no ambiguity, merely a loss of resolution. The loss of resolution can be recovered by semantic processing following the superset syntactic parse.

## 5.7 The Superset Grammar Approach

The superset grammar approach described in this section comprises two innovations, each of which could in principle be used independently.

Uniﬁcation of declaration and expression syntaxes provides a solution to the major C++ parsing problem: the declaration / expression ambiguity. The problem ceases to be syntactic. It is deferred to the semantic level where it belongs and is relatively easily resolved.

Elimination of the use of type information avoids the need for potentially inﬁnite lookahead to perform type disambiguation (of ρ in Section 5.5.3.2), since incorporation of type requires substantial grammar elaboration to handle the undecided lookahead. Removal of type simpliﬁes the grammar, allowing type related ambiguities to be removed from the grammar and deferred for semantic resolution, where they too belong.

We ﬁrst revisit the ambiguity analysis of Section 5.5.3 to see what problems a lack of type information causes. We then present relevant parts of the superset grammar to show how the superset is implemented. The full superset C++

A B       AorB ⊆

grammar may be found in Appendix B, and the extended FOG superset in Appendix C.

### 5.7.1 C++ Ambiguities (without type information)

With type information, σe is a type-name and is distinct from θe which is a nontype-name. Their C++ usage is distinct:

Without type information there is no distinction, and we just use the superset name θ. This was shown graphically in Figure 5.3 on page 158.

The Fe•σe term may be subsumed by the replicator to give the superset expression.

#### 5.7.1.1 Declaration / Expression ambiguity

The strict ambiguity for a declaration / expression ambiguity (from Section 5.5.3.2) simpliﬁes initially to

and after application of semantic constraints to:

```
expression
expression-list






L
+
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

θe

λ F e σe

-          

- =

θ D ε

- ⊃ θe ∪

```
expression
expression-list






L
+
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

- θ λ   

- =

δ L + η

- ∩ D0 L +

- I J   

?

- Pe

*

F de Ade

B L 1 Pe

- *

-          

*

θ

- =

δ L + η

- ∩ L + F de θ

- =

with the corresponding recursive parameter ambiguity ultimately simplifying to

These are greater ambiguities but they have the same structure as before. The former ambiguities involving just σe now involve θ. This loss of precision can be recovered as soon as type information is available. Type information is not necessary to identify the syntactical structure in which θ is used. The ambiguity may therefore be safely deferred for semantic rather than syntactic resolution.

#### 5.7.1.2 Expression / Expression ambiguity

Removal of the distinction between σe and θe requires reassessment of ambiguities related to σe and τ.

```
The former declaration / declaration ambiguity involving operator new[] now
becomes an expression / expression ambiguity as well.
```

#### 5.7.1.3 type-id / expression-list ambiguity

A parenthesised-call / cast-parenthesis or parenthesised-binary / cast-unary ambiguity exists when the

of the cast may be confused with

The T+ term can now match part of θ, since without type information , leading to the solution (for the potential type names)

indicating that

```
(T)+5
is now ambiguous: is it a cast of +5 to type T, or the sum of T and 5?
```

This decision cannot be made without type information. However it can be deferred until type information is available, since an AST node that misleadingly

π η ∩ I? F de Ade      

* F de θ

- =

```
type-id
τ
T
=
+
Pd
•
*
E ?
V ?
•
F
•
d
Ad
```

B Pd

*

-          

* E ? V ?

- F

- d Ad

B Pd

+

-          

         

?

ε

- =

```
assignment-expression
η
```

Ae F e Os Op

Oi Pe

C

B L +

-                              

=

*

θ λ   

- =

T ε

- θ ∩ ε ≠

τ L + η

- ( ) ∩

Ade F de      

* θ T ε

- ∩ ( )

- =

describes the addition of a type to a value can be detected and corrected to describe the corresponding cast. This error is highly localised unlike the template corrections of Section 5.8.1.

#### 5.7.1.4 Call / functional-cast ambiguity

```
The call / functional-cast ambiguity arises when the type-name σe in a functional-
call can be mistaken for an assignment-expression. A type-name is not distinct from θ
which is a valid
assignment-expression. All forms of functional-cast become
ambiguous. The functional-cast is excised from the grammar, with detection of
functional-casts deferred until type information is available to determine whether
the function-name associated with a call is a type-name or not.
```

### 5.7.2 A naive Assignment-Expression / Parameter-Declaration superset

```
assignment-expression and
parameter-declaration occur as part of the recursive
ambiguity between an expression-list as a function call argument list and the
parameter-declaration-clause of a function-deﬁnition. We must ﬁnd a superset that covers
both
```

and

.

A very naive common superset for is

π D = + I?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

ε                  

η

Ae F e Os Op

Oi Pe

C

B L +

-                              

*

θ λ   

- =

ω π η    ⊇

where

- A is the superset of Ad and Ae

- F(*L•ω) is the superset of Fd(*L•π) and Fe(*L•η)

- P is the superset of Pd and Pe

This superset covers ε and so introduces numerous ambiguities. For instance a name could be either θ or D•ε, and an inﬁx operator with ε as its left operand is indistinguishable from the equivalent preﬁx operator. A much tighter superset is required.

### 5.7.3 The Superset

```
Deﬁnition of a superset that covers a parameter-declaration and assignment-expression
requires that their component terms also represent corresponding supersets. We
therefore present the component supersets in increasing order of complexity
before ﬁnally reaching the generalised parameter-declaration and showing that it
provides superset grammar coverage.
```

#### 5.7.3.1 Generalised Name

The ambiguity in

```
prefix ( b )
causes extreme parsing difﬁculties because the C++ grammar prepends the
prefix to the parenthesis for two different reasons, only one of which is
guaranteed to have parentheses present:
```

- a preﬁx name is associated with parentheses for a function call

```
•
a preﬁx name is associated with an optionally parenthesised declarator for
a simple-declaration
One of these alternatives must be eliminated to resolve the parsing difﬁculty.
Eliminating a function call is undesirable since parsing of function arguments
occurs in the midst of productions that enforce arithmetic precedences.
Eliminating the name from the decl-speciﬁer-seq preﬁx of declarations is possible.
```

The solution is to maximise the parsed length of any word-like sequence before associating punctuation. Therefore

```
extern int f()
is parsed so that extern int f is parsed in its entirety before the parentheses
are applied. This avoids the problem of whether a preﬁxed name may be followed
by a parenthesis or not, since a name is only prepended in one place. The cascade
of names is parsed ﬁrst. As a result of this, the parse trees for
```

D*

A

E ? V ?

- F

- Os Op

Oi

P

E ? V ?

- C

E ? V ?

- B L +

-                              

*

θ λ

ε     

```
int (var)
Class(arg)
Class(int)
```

are all the same. Semantic processing must use type information to separate the possibilities and recognise the equivalence of

```
int (var);
int var;
The template keyword affects the way in which a name is used. It must therefore
bind to that name. template is therefore always parsed close to the name. This
potentially causes a conﬂict with an explicit-instantiation which provides an external
binding of the template keyword.
explicit-instantiation:
template declaration
The C++ grammar therefore suggests that
template int X<int>::f();
```

be parsed as

```
f
X<int>::f
X<int>::f()
int X<int>::f();
template int X<int>::f();
```

in which each subsequent line represents the increased knowledge resulting from reducing a parsing rule.

```
In FOG, names and the template keyword are resolved with high priority to avoid
ambiguities, the reduction order is
f
X<int>::f
int X<int>::f
template int X<int>::f
template int X<int>::f()
template int X<int>::f();
The conﬂict is resolved by ensuring that every valid declaration that can
participate in an explicit-instantiation incorporates the template preﬁx. In practice
this means that every preﬁxing rule that could form part of a declaration in the
generalised expression must apply a template preﬁx if it applied any other preﬁx.
Parsing of an explicit-instantiation is therefore subsumed by parsing a declaration.
```

In the grammar analysis, a maximised name is denoted by D+, which is convenient for analysis but does not perform very much of a syntactic breakdown. Classifying the components of D as

```
u
a user-deﬁned name (e.g. ::name or operator const char *
or class X {...}) already including an optional template preﬁx
b
a built-in type name (e.g. short)
t
the template keyword
q
anything else (e.g. const or extern or virtual)
```

D+ is parsed (using regular expression syntax) as

```
(t*q)* ( (t*b(b|q)*)? (uq*(b(b|q)*)?)* | t*b(b|q)*)
```

rather than

```
(u|b|q|t)+
All possibilities are parsed and the requirement to gather template preﬁxes is
observed. The b(b|q)* grouping captures a complete (multi-word) built-in type
name together with all interspersed and trailing qualiﬁers. The uq* similarly
captures a single word user-deﬁned name together with all trailing qualiﬁers. An
arbitrary mix of names is permitted recognising that two built-in type-names
cannot be adjacent. This grouping ensures that only one AST node need be
created for each name and that trailing qualiﬁers attach to their preceding name.
The (t*q)* preﬁx associates any preﬁx qualiﬁers with the ﬁrst name.
The leading (t*q)* is separated and not implemented as part of a D+ parse. This
exploits the semantic constraint that a name must contain at least a user-deﬁned
or built-in type name, and avoids ambiguities when a generalised name:
•
follows a cast generalised with a trailing cv-qualiﬁer-seqopt to cover an
abstract function declarator
(cast) const p
```

- follows a cv-qualiﬁed pointer

```
int * const p
```

- is used with the FOG negated qualiﬁer extension

```
!static int p;
The omitted preﬁx (t*q)* term is only valid as part of a decl-speciﬁer-seq which
occurs at the start of certain declarations. The missing speciﬁers are therefore
parsed as a preﬁx once all other ambiguities have been removed:
simple_declaration:
';'
|
init_declaration_list ';'
|
decl_specifier_prefix simple_declaration
A related complexity arises with pointers to members
Class :: *
```

since the following is syntactically valid

```
Type Class::* p = 0
```

In order to pursue the same policy of maximising the name to avoid shift-reduce conﬂicts, the parse for the pointer scope absorbs all the preceding name components. This must of course eventually be sorted out by the semantic processing, but differs little from the problem of resolving

```
int * * p = 0
which the superset parse identiﬁes as an assignment of zero to the product of the
name int and the dereference of p. This might appear to be a severe misparse,
but actually corresponds to an economy of AST nodes. The pointer-to declarator
node does not need to exist. Its functionality is folded into the superset multiplier
node, which now has one behaviour for types and another behaviour for values.
```

#### 5.7.3.2 Generalised Array

The two forms of array sufﬁx: Ad(κ?) and Ae(+L•η) are generalised to A(*L•η).

#### 5.7.3.3 Generalised Parentheses

The two forms of function sufﬁx: E?•V?•Fd(*L•π) and Fe(+L•η) are generalised to

E?•V?•F(*L•ω) where ω ⊇ π ∪ η

The precedence enforcing parentheses are generalised from B•*L to E?•V?•B•*L.

Although not necessary to unify declarations and expressions, generalisation of a cast is necessary to avoid shift-reduce conﬂicts. C is therefore replaced by E?•V?•C.

```
In order to parse delete[] followed by an unparenthesised expression. C(τ)•α is
further generalised to cover [ η? ] α as well as ( τ ) α.
```

#### 5.7.3.4 Generalised pointers

```
ptr-operator is not generalised, however the usage of * as a binary operator is
replaced by star-ptr-operator in order to accept a cv-qualiﬁer-seqopt following a * in a
multiplicative-expression. Additionally any decl-speciﬁer preceding the scope of a
pointer to member is associated with the scope, thereby avoiding conﬂicts and the
need to introduce another inﬁx expression operator.
unary_expression:
postfix_expression
|
"++" cast_expression
|
"--" cast_expression
|
ptr_operator cast_expression
|
suffix_decl_specified_scope star_ptr_operator cast_expression
...
multiplicative_expression:
pm_expression
|
multiplicative_expression star_ptr_operator pm_expression
|
multiplicative_expression '/' pm_expression
|
multiplicative_expression '%' pm_expression
star_ptr_operator:
'*'
|
star_ptr_operator cv_qualifier
```

#### 5.7.3.5 Generalised primary-expression

```
In order to cover some forms of abstract-declarator as expressions and thereby avoid
shift-reduce conﬂicts, a primary-expression is extended to cover an abstract array A•ε
in addition to an abstract function covered by the parenthesis generalisation to
E?•V?•B•*L•ω. The same
term is therefore used as an expression (to support abstract-declarator), as an
expression preﬁx (to support a cast) and as an expression sufﬁx (to support
function parameters). Use of precisely the same syntax avoids shift-reduce
conﬂicts.
primary_expression:
literal
|
"this"
...
|
abstract_expression
abstract_expression:
parenthesis_clause
|
'[' expression.opt ']'
|
"template" abstract_expression
parenthesis_clause:
parameters_clause cv_qualifier_seq.opt exception_specification.opt
parameters_clause:
'(' parameter_declaration_clause ')'
```

#### 5.7.3.6 Generalised assignment-expression

```
In order to provide complete coverage of a declarator initializer, the right-hand
expression of an assignment-expression using = is extended to accept {} forms.
assignment_expression:
conditional_expression
|
logical_or_expression assignment_operator assignment_expression
|
logical_or_expression '=' braced_initializer
|
throw_expression
braced_initializer:
'{' initializer_list '}'
|
'{' initializer_list ',' '}'
|
'{' '}'
```

A

E ? V ?

- B L *

-      

```
The overall form of the generalised assignment-expression is
```

χ ⊃η since A ⊃ Ae, F ⊃ Fe, P ⊃ Pe, D+•ε ⊃θ.

#### 5.7.3.7 Generalised parameter-declaration

Adding one extra term to χ provides coverage for π, and ensures that ω ⊃χ ⊃η.

```
The additional term deﬁnes ω as a generalised parameter-declaration
```

The extra D+•Pd

*•I?•ε exhibits signiﬁcant preﬁx ambiguity with a binary expression. It is only the ε at the right-hand side that disambiguates

```
a **** ,
```

from

```
a **** b,
```

Implementation of this term therefore re-uses intermediate expression productions to avoid shift-reduce conﬂicts, and consequently covers many meaningless sentences.

```
abstract_pointer_declaration:
ptr_operator_seq
|
multiplicative_expression star_ptr_operator ptr_operator_seq.opt
abstract_parameter_declaration:
abstract_pointer_declaration
|
and_expression '&'
|
and_expression '&' abstract_pointer_declaration
special_parameter_declaration:
|
abstract_parameter_declaration
|
abstract_parameter_declaration '=' assignment_expression
|
“...”
parameter_declaration:
assignment_expression
|
special_parameter_declaration
|
decl_specifier_prefix parameter_declaration
```

#### 5.7.3.8 Coverage of the generalised parameter-declaration

It must be shown that where

χ

A

E ? V ?

- F

- Os Op

Oi

P

E ? V ?

- C

E ? V ?

- B L *

-                              

=

*

D+ ε

- λ

A

E ? V ?

- B L * D* I?

- Pd

+

-       ε

-              

ω

χ

D* I? Pd

+

- ε

-       =

π ω ⊆

To keep the equations more manageable:

π may be split to create 4 sub-problems:

each of which will be shown after ﬁrst showing that pointers to ξ are covered.

**Pd**

*•I?•ξ ⊂ ω

Of the four alternatives offered by the right hand term of ξ, the top one and bottom two are clearly covered, since I ⊂Oi, Pd ⊂ P, θd ⊂ D+•ε. The second may be written more fully as

ω

A

E ? V ?

- F

- Os Op

Oi

P

E ? V ?

- C

E ? V ?

- B L *

-                              

*

D+ ε

- λ Ad ε

E ? V ?

- B L * D* I?

- P

- d

+

- ε

-              

D* I? Pd

+

- ε

-                                      

=

π D = + I?

- Pd

*

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

ε                  

ξ

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

- =

π

D+ Pd

+ I?

- ξ

D+ I? ξ

D+ I? ε

D+ Pd

+ I?

- ε

-                  

=

and then rewritten

and then expanding ω gives

Examining top and bottom right-hand terms shows them to be covered by ω.

1) D+•I?•ε ⊂ ω

This term is covered by Oi

```
?•D+•ε since preﬁx and sufﬁx operators commute and
we generalised the right-hand side of an assignment to support the {} form of
declaration initializer.
```

2) D+•Pd +•I?•ε ⊂ ω

```
This is covered by the extra term added for use of a generalised parameter-
declaration rather than a generalised assignment-expression.
```

3) D+•Pd +•I?•ξ ⊂ ω

Taking the lexical perspective for ordinary pointers and references: the ﬁrst Pd in a term which could be a multiplier in a declaration of the form

```
static long int * *a = 0 ;
```

The problem

D+•Pd•Pd

*•I?•ξ

can be written, after introducing lexical separation around the ﬁrst Pd, as

D+•ε Pd•ε Pd

*•I?•ξ

which is covered by

D+•ε Oi•ε Pd

*•I?•ξ

```
since Pd is covered by Oi, and the possibility of a trailing cv-qualiﬁer-seq on Pd is
covered by the generalisations to
unary-expression and
multiplicative-expression.
Introduction of the dyadic Oi partitions the problem into the two smaller problems:
```

D+•ε ⊂ ω and Pd

*•I?•ξ ⊂ ω.

It is clear that D+•ε ⊂ ω, and we have already shown that Pd

*•I?•ξ ⊂ ω.

For pointers to members such as

```
extern int Class::* p = 5
```

Pd

* I?

E ? V ?

- F

- d Ad

B Pd

*

-          

*

E ? V ?

- F

- d L * ω

- ( ) ε

Pd

* I?

E ? V ?

- F

- d Ad

B Pd

*

-          

*

E ? V ?

- B L *

- ω

Pd

* I?

E ? V ?

- F

- d Ad

B Pd

*

-          

* E ? V ?

- B L *

- χ

E ? V ?

- B L *

- D* I? Pd

+

- ε

-      

the generalised form of pointer P subsumes the D+ preﬁx leaving the problem Pd

*•I?•ξ ⊂ ω which has already been shown.

4) D+•I?•ξ ⊂ ω

This may be shown by considering the three alternative locations for the left-most occurrence of a B in:

There may be no occurrence of a B:

This is covered since the absence of preﬁx operators allows the D preﬁx to be combined with the θd or ε.

There may only be a B in the ﬁnal replicator.

which may be rewritten as

which is satisﬁed since the function argument Pd

+•ε ⊂ω ⊂*L•ω.

There may be a B in the ﬁrst multiplier

which may be rewritten as

which is covered since the recursive problem Pd

*•ξ ⊂ω ⊂*L•ω has already been shown.

### 5.7.4 Ambiguities in the superset grammar

The complete C++ grammar implemented using the superset approach is provided in Appendix B. Processing that grammar through bison and yacc provides a rigorous check of the ambiguities, which are described in Appendix F.2.

D+ I?

E ? V ?

- F

- d Ad

B Pd

*

-          

* θd

E ? V ?

- F

- d Ad

B Pd

+

-          

ε

             

- ω ⊂

D+ I?

- E ? V ?

- F

- d Ad      

* θd

E ? V ?

- F

- d Ad       ε

-          

- ω ⊂

D+ I?

- E ? V ?

- F

- d Ad      

* B Pd

+

- ε

- ω ⊂

I? E ? V ?

- F

- d Ad      

* F Pd

+ ε

- ( ) D+ ε

- ω ⊂

D+ I?

- E ? V ?

- F

- d Ad      

* B Pd

*

- ξ

- ω ⊂

I? E ? V ?

- F

- d Ad      

* F Pd

* ξ

- ( ) D+ ε

- ω ⊂

The overall accuracy of the grammar depends on transcription errors in the conversion of the textual exposition of Annex A of the standard [C++98] to an executable yacc grammar, and the number of changes required to resolve ambiguities. The grammar is similar to the standard and so relatively easily checked in conjunction with the justiﬁcation of the superset.

The more substantial changes to resolve ambiguities are based on establishing a superset of the declaration and expression syntax. The ability of the superset to parse the composite declaration and expression syntax was shown in the previous section. Much simpler proofs are used to show that the same or a slightly tailored superset solves other ambiguity problems. Restructuring of a few productions is necessary to remove or at least resolve shift-reduce conﬂicts.

Provided that the syntactic parse discards no information, ambiguities that are not resolved syntactically can be resolved semantically. This is easily achieved by ensuring that each reduction fully parameterises each AST node that is created.

A discussion of the resolution of the practical difﬁculties associated with the detailed C++ syntax may be found in Appendix F.2 including details of the new ambiguities resulting from the superset approach in Appendix F.2.6. The semantic processing required to recover from the reduced syntactic resolution is described in Appendix F.3.

## 5.8 Back-tracking

Introduction of back-tracking into an LALR parser grammar in order to support a reparse against an alternate syntax proves to be remarkably easy. The author is not aware of any other implementation that back-tracks, however tools such as yacc are in such widespread use by ‘imaginative’ programmers that it is unlikely that the approach is totally new.

We ﬁrst show how a linear search of alternative syntaxes may be made, as was required to resolve the declaration/expression ambiguity in the earlier multi-pass grammar.

We then show how a binary tree search of alternative syntaxes can be orchestrated in order to resolve the template/arithmetic ambiguity for the superset grammar.

### 5.8.1 Linear search in yacc

```
The multi-pass FOG productions to perform a linear search through the
declaration and expression ambiguities of a statement resemble
statement: mark declaration_statement unmark
|
mark remark expression_statement unmark
|
mark remark error ';' unmark
mark:
/* empty */
{ push_input_context_marker(); }
remark:
error
{ rewind_input_context_to_marker(); }
unmark:
/* empty */
{ pop_input_context_marker(); }
```

in which

```
•
/* empty */ is a comment to highlight use of the ε terminal
•
{} surrounds the code invoked when a rule is reduced.
•
error is a special error recovery token generated by the parser when an
inconsistent token is encountered.
The mark rule places a marker on the input token sequence. remark back-tracks
and starts another attempt to parse the same sequence. unmark removes the
marker but does not affect the token stream. In practice, statements nest, and this
approach is used to resolve other parsing problems too, so mark pushes onto and
unmark pops from a stack of marked positions.
The operation of the parser requires that all three alternatives start by shifting and
then reducing the mark to ensure that the parser generator has a common preﬁx,
and that the context stack is consistently maintained. The ﬁrst alternative then
attempts to parse the input token sequence as a declaration-statement and unmarks
if that parse succeeds. The other two alternatives are inactivate until an error
occurs, as will be the case if the declaration-statement parse fails. Reduction of the
remark rule that handles the error rewinds the input context back to the mark so
that an attempt at an expression-statement parse occurs. If this also fails, the further
error of the third alternative is satisﬁed and error recovery proceeds by
discarding tokens until a semicolon is encountered.
Successful parsing of the ﬁrst alternative therefore occurs without the use of any
error tokens. Successful parsing of the second alternative occurs after a single
error. Only after two errors does a proper error recovery get activated as the
third alternative.
```

This approach directly implements the disambiguation rule (§6.8-1): if it could be a declaration, it is (ﬁrst alternative), otherwise it’s an expression (second alternative). However the parse is a syntactic one. If it looks like a declaration, it is parsed as a declaration, otherwise it is parsed as an expression. This is not a problem when accurate type information is available, however we are advocating parsing without type information and so those ambiguities that require type information are resolved in favour of declarations (because declarations are parsed in the ﬁrst pass). Expressions that are misparsed because they look like, but cannot be, declarations must be corrected once type information is introduced in subsequent semantic processing. The nature of these expressions is discussed in Appendix F.2.5 and their resolution in Appendix F.3.1.

Code to detect the misparse has the full construct available as AST nodes, avoiding the difﬁcult partial context and lookahead problems that occur while trying to disambiguate in the grammar. Code to correct the misparse performs a localised tree rearrangement with many of the misparsed declaration AST nodes re-usable as nodes in the corrected expression tree.

```
The approach is relatively easy to implement in the parser, but requires the input
token source to support marking, and back-tracking to speciﬁc positions. In
practice there is a potential for errors in the grammar. Failure to pair mark and
unmark leads to stack drift that materialises as an unhelpful parsing failure with a
generally confusing diagnostic. Parsing control actions are transparent to the
parse table generator, and so unnecessary marks may be omitted:
statement:
selection_statement
|
mark declaration_statement unmark
|
mark remark expression_statement unmark
|
mark remark error ';' unmark
selection-statement:
if ( condition ) statement
if ( condition ) statement else statement
switch ( condition ) statement
A statement starting with an if keyword can only be a selection-statement, since if
occurs nowhere else in the grammar. A selection-statement can therefore be parsed
without marking the context for backtracking. This works because the parser
generator uses one token of lookahead. It may therefore examine the token
logically following the mark before deciding which parsing alternatives to retain.
This avoids incurring the cost of back-tracking. The corollary is that the marked
position in the input token sequence must be adjusted to ensure that the
lookahead token is made available following a back-track. A more unpleasant
corollary occurs if the omission of a mark is a programming error. In the above
example, if a declaration-statement could start with an if, the ambiguity would be
diagnosed by the parse generator as a shift-reduce conﬂict. However if the
expression-statement could start with an if, no diagnostic results, the generated
parser parses the selection-statement, but does not perform a back-track for the
expression-statement. This is one of the harder forms of parsing error to cure: the
grammar code looks correct.
```

The back-tracking approach is able to resolve the type-name or identiﬁer ambiguity adequately because there is very little syntactic difference: both are names. Most ambiguities are correctly resolved in favour of the declaration.

**Back-tracking for templates**

Back-tracking is less well-suited to resolving the template-name ambiguity, since in one case the syntax involves what should be paired brackets

```
template_name < a , b > - 5
// template_name<a,b> - 5
```

and in the other case an inﬁx operator

```
non_template_name < a , b > - 5
// non_template_name<a , b>-5
```

The ASTs for the two alternatives are rather different, the error in the tree structure is poorly localised, and grows as the number of ambiguous <’s increase. However, it is not resolvable by back-tracking to try an alternative restarting at the <, because the template name may be parsed successfully

```
template_name < a > b [ 5 ]
// Ok
template_name < a > b [ 5 ] + 7
// Syntax error
non_template_name < a > b [ 5 ] + 7
// Ok
```

and consequently reduced and removed from the parser stack before a syntactic contradiction is detected.

For back-tracking to succeed, the entire expression and more generally the entire declaration or statement, must be retried to test each template / non-template hypothesis. This requires a search of the binary tree of alternate hypotheses.

Back-tracking for template-names is perhaps not appropriate for a C++ parser that performs semantic interpretation. Relatively loose coupling between syntactic and semantic processing will sufﬁce to provide the information needed for syntactic analysis, since template-names cannot be introduced in mid-statement.

Back-tracking for template-names may be appropriate for C++ parsers that do not need full program comprehension. For instance, the relatively rare errors from a template misparse need not render a pretty printing program unusable.

However back-tracking for templates is unavoidable in FOG, since semantic information cannot be provided.

### 5.8.2 Binary tree search in yacc

The lack of context during meta-programming described in Section 4.2.3 mandates parsing without template information, and so a search of the binary tree of possible parses is required.

```
The problem to be solved is this: given a declaration or statement containing an
arbitrary number of occurrences of an identiﬁer followed by a <, ﬁnd a permutation
of the template/arithmetic interpretations for each occurrence such that a
syntactic parse can succeed.
```

The solution may be expressed as a boolean vector whose length is the number of identiﬁer < occurrences, with a false value for each template verdict and a true value for each arithmetic verdict.

The parser interacts with the solution in two contexts. An outer context sequences the search, advancing in depth ﬁrst fashion through the solutions. Interaction occurs through

```
start_search to mark the current context and create the ﬁrst solution
advance_search to restore the marked context and select the next solution
end_search to accept the current (or no) solution and clean up
```

(Solutions are maintained on a stack, so that nested problems can be solved.)

```
The outer context of course has no knowledge of the problem depth, which is
maintained by the inner context: where the identiﬁer < occurs.
```

For the ﬁrst solution, and for deeper exploration of later solutions, each encounter of identiﬁer < establishes a deeper problem and so the encounter lengthens the solution vector and enters a false verdict making a presumption of template usage.

```
If a solution attempt fails, advance_search is invoked and the solution vector is
advanced to the next candidate solution by binary addition at the deep end of the
vector with ripple carry to the shallow end. The current depth is then reset to zero
ready for the next pass, during which the depth is incremented as each identiﬁer
< is re-encountered. Each re-examined identiﬁer < therefore responds according
to the template/arithmetic behaviour appropriate to the candidate solution.
```

**Outer context**

The search is orchestrated by marking the input token context at the start of each declaration or statement, and associating a binary tree search context with that mark. The grammar

```
compound-statement:
{ statement-seqopt }
statement:
control-statement
...
is implemented as
compound_statement:
'{' statement_seq.opt '}'
|
'{' statement_seq.opt looping_statement '#' bang error '}'
{ UNBANG("Bad statement-seq."); }
statement_seq.opt:
/* empty */
|
statement_seq.opt looping_statement
|
statement_seq.opt looping_statement '#' bang error ';'
{ UNBANG("Bad statement."); }
looping_statement:
start_search looped_statement
{ end_search(); }
looped_statement:
statement
|
advance_search '+' looped_statement
|
advance_search '-'
statement:
control_statement
...
advance_search:
error
{ yyerrok; yyclearin; advance_search(); }
bang:
/* empty */
{ BANG(); }
start_search:
/* empty */
{ start_search(); }
Usages of statement are served by looping_statement which organises the binary
tree search around statement.
The search iteration is managed by the three functions start_search(),
advance_search() and
end_search() invoked from the action routines.
start_search() and end_search() occur as the pre and post actions that
looping_statement imposes upon looped_statement.
Retries are triggered by the error token, which invokes advance_search to
rewind the context for the next attempt.
In order to avoid an inﬁnite loop, advance_search needs a way to return a value
to the parser to signal whether to continue looping, or to ﬁnish the loop. It might
seem that an error token could terminate the loop, but the error token is already
being used to sequence the loop, so another approach is required.
advance_search() injects either a '+' token or a '-' '#' token sequence into
the source from which yylex() reads tokens, and ensures that one is fetched by
clearing out any lookahead with yyclearin.3
```

The third solution is therefore accepted after the parser has apparently parsed the rule:

```
start_search advance_search '+' advance_search '+' statement { end_search(); }
An unsuccessful attempt may have the apparent rule:
start_search advance_search '+' advance_search '-' { end_search(); }
Re-use of the error token enables the loop to be realised, however the
unsuccessful parse has to be successful in order to avoid making double use of
the error token. The extra '#' token is therefore injected to cause a guaranteed
regeneration of the error, since '#' is reserved for use by the C preprocessor
and so never occurs after preprocessing. In order to give a useful diagnostic, the
injected '#' is caught in the sequence
... looping_statement '#' bang error '}' { UNBANG("Bad statement-seq."); }
The bang production is empty but has an action routine that interacts with an error
message handler to ensure that users don’t see a "parser error" diagnostic
from each failed parsing attempt. There are no productions able to successfully
handle any tokens following the injected '#', so the parser is forced to start error
recovery, for which it ﬁnds continuation of the above possible. Error recovery
continues by discarding tokens until resynchronisation occurs at the next '}', at
which point the UNBANG macro interacts once again with the error message handler
to restore the previous behaviour and emit an appropriate message, together with
helpful context information.
```

Generation of a good quality diagnostic requires the diagnostics associated with the most-nearly-successful loop iteration to be cached during the loop for emission at the end of the loop. The most-nearly-successful metric is conveniently determined as the largest number of tokens parsed prior to the error.

**Inner context**

The domain of the search cannot be directly known by the loop management productions. It is maintained as a side effect of identiﬁer parsing.

```
id:
identifier                      %prec SHIFT_THERE
|
identifier template_test '+' template_argument_list '>'
|
identifier template_test '-'
|
template_id
template_test:
'<' { template_test(); }
template_id:
"template" identifier '<' template_argument_list '>'
A template ambiguity arises, whenever an identiﬁer is followed by a < without a
template preﬁx. The ambiguity is resolved, in the absence of template
information, by the prevailing state of the binary tree search. This state is
determined by the template_test() routine which injects '+' token to select a
template interpretation or a
'-'
'<' token sequence for an arithmetic
interpretation.
The template_test rule introduces a shift-reduce conﬂict between the usage in
the expression syntax and the usage presented above. This conﬂict is resolved by
the %prec to force all occurrences of identiﬁer < to take the test. When the test
injects a '+', parsing continues by analysing template arguments. When the test
injects a '-' and '<', the '-' satisﬁes the parse for an identiﬁer and the extra
'<' restores the token required to proceed with the arithmetic interpretation.
On the ﬁrst traversal of source tokens, template_test() increases the binary
tree search depth and selects the template hypothesis. On subsequent traversals,
template_test() signals the prevailing binary tree search hypothesis.
3. Any pair of tokens other than error could be used since the injected token is used
solely to distinguish two forms of advance_search in the grammar.
```

It is rarely necessary to perform the full binary tree search, since a branch and bound can exploit early failure and avoid searching other hypotheses that share the same failing preﬁx.

There is unfortunately no guarantee that the ﬁrst accepted hypothesis is correct, and so the subsequent semantic analysis must be prepared to reorganise the AST to accommodate errors in either direction.

It should be noted that there is no error handling for template arguments, so that a template argument parse failure propagates onwards enabling the statement level binary tree search to poll syntax alternatives.

**Cost**

```
This algorithm clearly has exponential complexity with respect to the number of
ambiguous <’s in a statement, although this complexity does not arise in practice
as is demonstrated by applying the grammar of Appendix C to three large bodies
of code to determine its cost. The results are summarised in Table 5.5.
```

The fog code is the entire C++ source code for FOG. Fewer than 1% (290 out of 38470) of statements contained an identiﬁer followed by a <, and a consistent syntax for these was usually (166 out of 290) found without back-tracking by assuming the template form. A consistent syntax was always found after one backtrack. The comparatively low proportion of ambiguous statements is due to a coding habit that hides templated lists and pointers behind typedefs.

The product code is the entire C++ source code for a proprietary product. A higher proportion but still fewer than 4% of statements contained an identiﬁer followed by an <, and a consistent syntax for these was almost always found after one backtrack. A consistent syntax was always found after three back-tracks.

The gcc code comprises almost the entire source code of the gcc compiler. This is C code and so gives worst case performance under the back-tracking presumption to try template syntax ﬁrst. Nearly 2% of statements were ambiguous. 29 statements were incorrectly resolved as templates without backtracking. 2047 statements were correctly resolved after the back-track necessary to change the template presumption. The higher number of back-tracks do not indicate the correctness of their parsing conclusion. However since the maximum depth of the binary tree search was 6, it is clear that the use of branch and bound in the search ensured that the 64th solution was found in at most 8 tries.

The C++ examples use templates but predate the Standard Template Library [C++98]. Although STL code may well use more templates, the parsing is likely to be more accurate since the current implementation assumes a template resolution and only retries when that is inconsistent. The gcc code without any templates is therefore a representative worst case.

These results conﬁrm the intuitively expected behaviour. Arithmetic expressions tend to have at most one relational operator, and so favouring template usage

Code

body

Preprocessed

```
Lines
Statements
identiﬁer <
statements
```

% ambigu

ous

Back-tracks

0 1 2 3 4 5 6 7

fog 88133 38470 290 0.75 166 124 0 0 0 0 0 0

product 119383 44353 1652 3.7 784 851 9 8 0 0 0 0

gcc 426797 127366 2099 1.7 29 2047 7 21 1 2 2 1

**Table 5.5 Back-tracking costs**

when syntactically consistent results in few expressions that need semantic correction.

The exponential search complexity does not arise in practice. From the C++ results, we may estimate that back-tracking may occur for 50% of ambiguous statements, which appear to be about 2% of all statements. From the gcc results we may estimate that a wrong parse is found 1% of the time. The syntactic cost is therefore an extra 1% of syntactic statement analysis. Estimating the cost of an aborted partial analysis at half the cost of a successful complete analysis, we may estimate that the net semantic cost involves the correction of a template misparse for 0.01% of statements.

## 5.9 FOG grammar

The full FOG grammar in Appendix C uses similar techniques to those for the simpler C++ grammar in Appendix B.

In addition to the use of back-tracking in the superset grammar to solve the C++ parsing problems

- template name (Appendix F.2.1.3 and Appendix F.2.1.4)

- bit ﬁeld or inheritance (Appendix F.2.4.1)

the back-tracking approach is also used to solve the following parser problems

- “old-style” C (type I) function declarations(Appendix F.2.4.2)

**Multi-pass implementation**

The earlier multi-pass implementation of FOG parsed ﬁrst for declarations and then back-tracked for expressions. Further nested back-tracking was used to resolve

- cast ambiguities (Appendix F.2.5.2)

```
•
new placement / initializer ambiguity (Appendix F.2.5.5)
•
sizeof ambiguity (Appendix F.2.5.6)
•
typeid ambiguity (Appendix F.2.5.7)
```

- type / value template arguments (Appendix F.2.5.8)

The preliminary scan of the body of a meta-function marked the position of the start of any tree-literal and attempted to parse a formal parameter. If a formal parameter was referenced, its value was used. Otherwise the parser back-tracked and copied the tree-literal unchanged into the meta-function body.

```
The multi-pass implementation did not parse statements within function bodies
further than the replacement of $-expressions. It therefore incurred very few of the
severe ambiguities that arise when expressions and declarations coexist.
```

## 5.10 Code Structure

The ﬁle input processing acquires tokens from source ﬁles and performs ANSI C preprocessing. The implementation closely follows the translation phases described in §2.1. It is shown in pictorial form in Figure 5.4.

Phase 1 (character mapping, trigraph replacement and universal character replacement) and phase 2 (backslash line continuation) are performed by nested subroutines in the input routine to the lexer.

Phase 2 operates in a demand-driven fashion, returning one line at a time to the lexer.

The lexer, automatically generated by ﬂex++, performs phase 3. The lexer is very simple (150 lines of rules), just identifying each of the different lexemes, and creating an appropriate derived instance of the polymorphic FogToken.

```
Construction costs are reduced by making substantial use of the Flyweight pattern
[Gamma95], with all tokens stored in a tokenization symbol table which is initially
populated by special objects for all the reserved words and punctuation
sequences. Whenever a text string is tokenized, the appropriate Flyweight is
chosen, so that a string such as and_eq is encoded as an object that retains its
spelling for the purposes of stringizing, but which otherwise behaves the same as
the object created for the text string &=. Reserved words such as bool or else are
similarly associated with objects that have relevant behaviour.
```

Minor complexities are resolved by using four alternate lexer states to handle

- normal tokenization

```
•
waiting for */ in a multi-line C-style comment
•
recognising preprocessor keywords between the
# and subsequent
directive
•
non-standard tokenization after a #include.
```

Phase 3 operates in a demand driven fashion, returning one lexeme at a time. Each lexeme has a dual identity, as a preprocessor token and as a token.

The phase 4 preprocessing directives and macro substitution are not performed by the lexer. Phase 4 processing is performed by two further layers of processing, each operating in a demand-driven fashion to return one lexeme at a time to its caller. The inner layer performs macro substitution. The outer layer detects and implements preprocessor directives. Macro substitution is performed by three very simple parser grammars that

- locate any macro arguments following a function-like macro

- substitute any identiﬁers in the macro replacement

```
•
retokenize around ## and stringize # identiﬁer
```

Substitution (scan 1) and replacement (scan 2) of identiﬁers may involve a recursive substitution, but not where scan 1 detects an identiﬁer matching the current macro (§16.3.4-2). The entire replacement for the detected macro and

Phase 1 : character decoding - hand-coded

Phase 2 : continuation lines - hand-coded

Phase 3 : tokenization - ﬂex++ grammar

Phase 5 : character set mapping - empty

Phase 4a : macro substitution - 3 bison++ grammars Phase 4b : preprocessor directives - bison++ grammar

Phase 6 : concatenation - hand-coded Phase 7a : syntactic analysis- bison++ grammar

Phase 7b : semantic analysis - hand-coded

C preprocessor

Lexical Analysis

Semantic Analysis

Syntactical Analysis

AST representation

Source Program

**Figure 5.4 FOG analysis phases**

arguments is determined as soon as the macro is detected, with the resultant token sequence cached and then emptied by successive demands for more lexemes.

```
Preprocessor # directive lines are parsed using a further grammar, with identiﬁer
substitution bypassed except where permitted by the syntax. Conditional
directives maintain a state stack, the top of which determines whether lexemes
from non-preprocessor lines should be discarded. #define and #undef maintain
the dictionary of macro deﬁnitions referenced by the inner preprocessor layer.
#include pushes a new source context on to the lexer.
```

The lexemes returned from the outer preprocessor layer comprise the token stream after all ANSI C preprocessing has been completed, and before any FOG extensions have been realised. The token stream includes whitespace and new lines. To improve error diagnostics, each token is returned with its source ﬁle, source line and source column context.

There is no phase 5 in FOG, the execution and source character sets of a source to source translator are necessarily the same. There is however a distinct internal character set in which escape sequences, universal characters and other characters are uniformly represented by binary values within characters and strings. The uniform internal representation forms the basis for concatenation, and is created (lazily) when the string or character lexeme is created during phase 3. Each number, string or character has three spellings: the original ASCII source spelling to be used by preprocessor stringizing, the uniform internal binary representation, and a regenerated ASCII spelling for output purposes. The output may need to differ from the input to preserve the meaning of concatenated escape sequences (§2.13.4-3):

```
"\xA" "B"
concatenates to
"\012B"
not
"\xAB"
```

The concatenation phase 6 and the analysis phase 7 are described in Section 3.1.1.

```
Early FOG implementations performed
$-substitution, Cpp substitution and
#directive resolution within the lexer, permitted $-substitution within characters
and strings and identiﬁed reserved words explicitly. The implementation was
large, complicated, vulnerable to obscure programs and lacked clear semantics.
The current implementation is simple, with a clear responsibility for each
processing layer. The leakage between layers is very constrained:
•
macro deﬁnitions are accessible via the std::get_cpp() meta-function
•
the
using form of include currently shares the same low-level
implementation mechanism as #include
```

- the source ﬁle identiﬁcation describing the name and origin of each source ﬁle contains a mixture of pre-processor and non-preprocessor attributes

The very mundane organisation of analysis phases demonstrates how the superset approach successfully avoids the phase dependencies prevalent in other C++ compilers.

## 5.11 Grammar Metrics

The complexity of the FOG grammar and the success of the multi-pass and superset approaches can be judged by comparing the parser generator statistics for three alternative C++ grammars. Table 5.6 provides a summary of the statistics

variously extracted from the source grammar, from the generated output or from the report ﬁle of the modiﬁed version of bison++ used to build FOG.

```
Imperfections in the grammar result in conﬂicts, which may be left unresolved, in
which case they are tabulated in the top two lines. Alternatively %prec may be
exploited to resolve them, in which case the conﬂicts appear on the third line.
Whether conﬂicts should be resolved is something of a matter of taste: any
unresolved conﬂicts give cause to concern that grammar problems are present,
and that the default resolution is erroneous, but equally a large number of resolved
conﬂicts may hide some erroneous resolutions.
The number of %prec’s used to resolve conﬂicts are shown on the fourth line.
%prec’s operate by resolving the competition between a shift and a reduction. A
large number %prec’s is bad since a %prec introduced to solve one problem may
accidentally resolve another wrongly.
%prec’s operate by establishing a precedence between rules and input tokens.
The number of input tokens that can be compared is shown on the ﬁfth line. Again
large numbers are bad because there is greater opportunity for accidental wrong
resolution of problems.
```

The remaining lines are explained in subsequent commentary.

The ﬁgures should be treated with some caution, because the goals and completeness of each grammar are not comparable. (A smaller number is better for all entries.)

The Roskind grammar is accurate, but lacks templates and other modern facilities.

The gcc grammar is an almost complete C++ grammar, and contains a few extensions. The grammar code also contains error recovery and performance optimisations.

The CPPP grammar is preliminary, and lacks some modern facilities.

The multi-pass and superset grammars aim for syntactic consistency, whereas the other grammars aim for semantic accuracy.

The C++ superset grammar (Appendix B) is a complete working C++ grammar with error recovery. The grammar uniﬁes the declaration and expression syntax to resolve ambiguities.

The FOG multi-pass grammar is that of the earlier FOG implementation. It is fairly complete, adds syntax for meta-level processing, has some error recovery, but does not provide parsing within function bodies.

Roskind gcc CPPP

C++ super-

set

C++ multi-

pass

FOG multi-

pass

FOG super-

set

Unresolved Shift-Reduce conﬂicts 24 5 1 0 0 0 0

Unresolved Reduce-Reduce conﬂicts 18 38 0 0 0 0 0

Resolved conﬂicts 0 704 410 23 14 16(+1) 31

```
%prec usage
0
109
22
8
6
6(+1)
15
```

Tokens with precedence 0 61 40 14 4 4 17

Rules 664 779 431 561 661 1456 958

States 1257 1399 702 897 1119 2217 1585

Non-ASCII terminals 77 82 104 95 82 241 110

YYLAST 13954 9534 3724 5060 6501 30119 14545

**Table 5.6 Grammar Statistics**

The C++ multi-pass grammar is a version of the FOG multi-pass grammar trimmed to handle just C++ but adapted to parse all of C++. The grammar contains some error recovery, and a structure that retains some unnecessary generality from the full grammar. The grammar has never been used. The results serve to compare the parsing approach adopted in FOG with other C++ approaches, and to determine the extra complexity introduced by FOG extensions.

The FOG superset grammar (Appendix C) is the working FOG grammar, using uniﬁed parsing and incorporating error recovery.

**Conﬂicts**

Each conﬂict provides an opportunity for a programming error to yield an incorrect grammar implementation. The low number of conﬂicts in the Roskind and even smaller number in the new grammars ensure that the residual conﬂicts can be analysed by hand.

```
Roskind makes a point of avoiding %prec, relying on line ordering in the grammar
instead. This is an understandable policy with yacc, since yacc does not report
conﬂicts resolved by using %prec. However, the use of %prec in FOG, and
validation by bison, allows lines to be reordered and explicitly identiﬁes each
conﬂict where that conﬂict is intended to be resolved. The extensive use of
precedence by gcc and CPPP and the very large number of tokens which can
participate in precedence resolution offer ample scope for a conﬂict to be silently
and erroneously resolved.
The FOG grammars use only two active precedence levels, reducing hazards of
fortuitous resolution between levels. 16 tokens share the same level, and so only
one token has a distinct precedence level. Two bordering precedence levels have
the dummy tokens REDUCE_HERE and SHIFT_THERE so that precedence rules are
coded as in Appendix F.2.3.1.
```

The superset and multi-pass approaches clearly have fewer conﬂicts. The multipass approach has fewest, since grammar conﬂicts are resolved in independent passes. The (+1) on the FOG grammar accounts for the dangling else resolved by the token acquisition routine. Substantially larger numbers should be added to gcc and CPPP metrics to account for their lookahead code.

**Size**

The number of rules in a grammar corresponds to the line count in a more conventional program, and so measures the programming effort. The number of states in the generated state machine is a measure of the complexity of the problem that the program solves. YYLAST is the dimension of the two largest tables generated for the table-driven parser. YYLAST therefore provides a good indication of the executable size after all compactions have been performed. The size of the FOG multi-pass grammar is close to the 32767 implementation limit for the standard bison parser. During development FOG exceeded that limit, which is why a modiﬁed version of bison++ was used to support 99999 table entries.

The number of rules and states in the Roskind and gcc grammars are surprisingly similar, more so after allowing for the greater completeness and error recovery in the gcc grammar. The CPPP grammar is noticeably smaller, which might be an indication of a better grammar, or conﬁrmation of its inaccurate status.

The C++ superset grammar is noticeably smaller, the beneﬁt of merging declarator rules into expressions rather than ﬂattening out to try to create more lookahead.

The trimmed C++ multi-pass grammar is similar in size to the gcc and Roskind grammars. Each of these grammars is over-size through the use of grammar ﬂattening to extract or resolve extra semantic context at the syntactic level.

The full multi-pass FOG grammar is at least twice as large, the result of some extra grammar for the meta-level syntax and the extra rules to support syntaxdriven interaction with meta-functions and meta-variables. The extra rules are mostly simple. It would be wrong to infer that FOG doubles the grammar complexity.

A fairer comparison of the parsing cost of FOG can be drawn from the more effective superset grammar which is rather less than twice the complexity of its C++ counterpart and pretty similar to the gcc and Roskind grammars.

**Parsing Assistance**

```
The number of non-ASCII terminals used by the grammar provides a little insight
into the assistance provided to the parser. Some non-ASCII terminals are
unavoidable to represent the 24 special character sequences (+=), 63 primary
reserved words (class), 11 alternate reserved words (xor_eq) and 4 parametric
tokens (identiﬁer). The exact number of such terminals depends upon whether
alternate reserved words are resolved by the lexer, and whether groups of similar
reserved words such as private, protected and public are replaced by a
parametric token such as access-speciﬁer. A practical implementation uses more
than the minimum number of terminals in order to communicate extra
disambiguation information to the parser.
```

The Roskind ﬁgure does not account for some more recent reserved words and has one extra disambiguating token.

The gcc ﬁgure is more representative, adding six disambiguating tokens.

The larger value for CPPP is the result of the extra information provided by its lookahead parser.

The values for the superset C++ grammar correspond to exactly one token per reserved word, punctuation sequence, and 5 parametric tokens.

The much larger ﬁgure for the FOG multi-pass grammar is mostly caused by the need for two tokens per meta-type, one to prime the parser to perform the correct syntax-driven parse, and one to identify the meta-type of a meta-variable instantiation.

The additional tokens for the FOG superset grammar are required for the nonreserved words.

**Performance**

The superset approach incurs minimal costs from back-tracking, since a backtrack occurs only for very unusual code and the very occasional4 template retry. The parser is probably slightly slower because more reductions are performed for the many levels of expression precedence than for the normal nesting of declarators. This cost could be alleviated by ﬂattening the expression syntax and using precedence as in the CPPP grammar. The extra semantic disambiguation will incur a small cost, but since it occurs within the disciplined context of a tree rather than the less appropriate parser stack, there is probably a small net saving. The need for semantic correction to the AST is rare and so should incur very little cost.

The overall relative speed cost of the superset approach may be slightly adverse. Additional semantic analysis work is introduced by the need to correct the syntax analysis, and some time is wasted on pursuing inconsistent candidate template/arithmetic syntaxes. It is difﬁcult to assess to what extent these moderate costs are offset by the signiﬁcant simpliﬁcations of each of the lexical, syntactic and semantic analyses. However this loss is well justiﬁed given the

4. Estimated at 0.01% in Section 5.8.

elimination of intra-statement coupling between syntactic and semantic analysis stages and the possibility of demonstrating that the grammar implementation is accurate.

The parsing tables for the superset grammar approach are clearly smaller. The AST provides an easier environment in which to resolve ambiguities. It is therefore likely that deferring ambiguity resolution to the semantic level may contribute a further small code saving.

## 5.12 Summary

We have shown how the need for type information to parse C++ can be eliminated by identifying a larger superset language of which C++ is a part.

We have introduced an extended form of regular expressions that enables ambiguities in the C++ grammar to be analysed.

We have described a potentially novel approach to back-tracking in yacc that enables C++ to be parsed without template information.

We have therefore shown how a C++ parser can be constructed without leakage between its lexical, syntactic and semantic stages leading to considerable simpliﬁcations in each stage.

A minor disadvantage arises from the need to correct a syntax analysis error during semantic analysis. This is estimated to occur for approximately 0.01% of statements.
