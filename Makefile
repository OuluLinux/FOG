.PHONY: check

UNAME_S := $(shell uname -s)
UNAME_O := $(shell uname -o)
MFILE := Makefile
OUTFILE := bin/fog

ifeq ($(UNAME_S),Linux)
	MFILE:="script/Makefile.Linux"
else ifeq ($(UNAME_S),FreeBSD)
	MFILE:="script/Makefile.FreeBSD"
else ifeq ($(UNAME_O),Msys)
	MFILE:="script/Makefile.msys2clang64"
	OUTFILE := bin/fog.exe
endif

all:
	$(MAKE) -f $(MFILE)
	mkdir -p bin
	cp Fog.out $(OUTFILE)
	rm Fog.out

check:
	perl script/test.pl executable="bin/fog" tests/*

clean:
	rm -r _out

