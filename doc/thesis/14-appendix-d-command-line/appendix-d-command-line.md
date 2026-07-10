# D Command Line

The FOG command line is

```
fog <tokens> <files>
<files> is one or more input ﬁles, conventionally using the extension .fog.
- may be used to indicate that the standard input be used as an input ﬁle.
<tokens> is any combination of the following, with or without spacing between a
token such as -I and a subsequent text argument denoted as *.
```

## D.1 Miscellaneous options

```
-help
Display usage help.
-q
Suppress the program identiﬁcation message.
```

## D.2 Preprocessor options

```
-D*
Deﬁne a preprocessor macro value.
-I*
-i*
Source include ﬁle path(s) (defaults to current directory).
```

## D.3 Variant C++ options

```
-long_long_type
Treat long long as built-in (Sun C++ language extension).
-mbc#
Bytes in a multi-byte character(4).
-no_access
Diagnose access declarations (ARM C++ compatibility).
-no_bool_type
Do not treat bool as built-in (ARM C++ compatibility).
-no_namespace
Treat namespace as a synonym for class.
-no_specialisation_prefix Do not require template<> for specialisation (ARM C++
compatibility).
-no_using
Emit
using-declarations
as
access-declarations
(ARM
C++
compatibility).
-no_wchar_t_type
Do not treat wchar_t as built-in (ARM C++ compatibility).
```

## D.4 Generated C++ options

```
-anon_prefix*
Preﬁx for “anonymous” names (default is _anon_).
-c++
Behave more like a C++ compiler.
-comment_line_numbers
Enclose #line numbers as comments in emitted ﬁles.
-extern_prefix*
Preﬁx for extern linkage names (default is _extern_).
-no_line_numbers
Omit #line numbers from emitted ﬁles.
-nobanner
Suppress emitted comment banners (to ease regression
testing).
-t#
Columns per tab in source ﬁles (default 8).
-template_parameter_prefix Preﬁx for normalised template parameter names (default is
"_").
-unnest
Emit nested classes after rather than within enclosing class.
```

## D.5 Output ﬁle options

```
-cd*
Emitted implementation ﬁle directory path.
-cp*
Emitted implementation ﬁle preﬁx.
-cs*
Emitted implementation ﬁle sufﬁx.
-ctd*
Emitted template implementation ﬁle directory path.
-ctp*
Emitted template implementation ﬁle preﬁx.
-cts*
Emitted template implementation ﬁle sufﬁx.
-f
Force ﬁle emission (bypass redundancy comparisons).
-global*
(File)name of the global namespace.
-hd*
Emitted interface ﬁle directory path.
-hp*
Emitted interface ﬁle preﬁx.
-hs*
Emitted interface ﬁle sufﬁx.
-htd*
Emitted template interface ﬁle directory path.
-htp*
Emitted template interface ﬁle preﬁx.
-hts*
Emitted template interface ﬁle sufﬁx.
-log*
Log ﬁle name (duplicates standard error).
-max_errors#
maximum number of errors before program termination
(100).
-max_warnings#
maximum number of warnings before program termination
(0).
-nc
Notify emitted ﬁle names that are created.
-ne
Notify emitted ﬁle names that are suppressed through
equivalence.
-o*
File name for make dependencies between source and
emitted ﬁles.
```

## D.6 Diagnostic options

```
-readonly
Just read source ﬁles to gather token count statistics.
-statistics
Emit program performance statistics.
-z2h
Display each token passed between lexer and hash parser.
-z2l
Display each token passed between lexer and locate parser.
-z2m
Display each token passed between lexer and main parser.
-z2r
Display each token passed between lexer and replace
parser.
-z2s
Display each token passed between lexer and substitute
parser.
-za
Display changes to the activity status of declarations.
-zd
Delete all objects rigorously on exit (for testing with purify).
-zf
Display ﬁle name as each entity is (re)positioned.
-zi
Display each input and macro line.
-zl
Display the behaviour of the lexer.
-zp
Display changes to the purity status.
-zs
Display changes to the default parser scope.
-zt
Display each token passed between lexer and main parser.
-zu
Display changes to the composed entity and parser default
utility.
-zx
Display full hex address of each object in diagnostics.
-zy
Display yacc parser progress.
-zz
Repeat certain failed invocations after generating an error to
aid debug.
```

## D.7 Predeﬁned macros

```
#define __STDC__ 0
#define __cplusplus 0
```
