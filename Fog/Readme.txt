
== Making changes to parser files (.y) ==

1. Make changes
2.1 Make lexer changes to FogTokenEnum project and run it
2.2 (use original sed script or just) append generated FogParserEnum.h content to
"*.y" files BEFORE "%start " line. Remove previous almost duplicate code before doing that.
2.3 Copy "FogTokenTypeEnum.h" to this project folder overwriting the previous
3.1 Run flex++ in this project folder.
Unfortunately only the ancient flex++ works properly.
The ancient flex++ is in the file http://www.computing.surrey.ac.uk/research/dsrg/fog/FogSrc.tgz
/ home / sblo / Ohjelmat / FOG / tools / flex_pp / flex++ -L - H / home / sblo / Ohjelmat / FOG / tools / flex_pp / flexskel.h - S / home / sblo / Ohjelmat / FOG / tools / flex_pp / flexskel.cc - hFogLexer.h - oFogLexer.cpp FogLexer.l
3.2 Run bison in this project folder:
bison++ --no - lines - t - v - d - o FogParser.cpp - h FogParser.h FogParser.y
bison++ --no - lines - t - v - d - o FogLocateParser.cpp - h FogLocateParser.h FogLocateParser.y
bison++ --no - lines - t - v - d - o FogHashParser.cpp - h FogHashParser.h FogHashParser.y
bison++ --no - lines - t - v - d - o FogReplaceParser.cpp - h FogReplaceParser.h FogReplaceParser.y
bison++ --no - lines - t - v - d - o FogSubstituteParser.cpp - h FogSubstituteParser.h FogSubstituteParser.y

