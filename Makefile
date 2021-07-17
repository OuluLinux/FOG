.PHONY: check

UNAME_S := $(shell uname -s)
MFILE := Makefile

ifeq ($(UNAME_S),Linux)
	MFILE:="script/Makefile.Linux"
else ifeq ($(UNAME_S),FreeBSD)
	MFILE:="script/Makefile.FreeBSD"
endif

all:
	$(MAKE) -f $(MFILE)
	mkdir -p bin
	cp Fog.out bin/fog
	rm Fog.out

check:
	perl script/test.pl executable="bin/fog" tests/*

clean:
	rm -r _out

