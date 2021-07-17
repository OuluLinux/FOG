.PHONY: check

all:
	$(MAKE) -f script/Makefile.FreeBSD
	mkdir -p bin
	cp Fog.out bin/fog
	rm Fog.out

check:
	perl script/test.pl executable="bin/fog" tests/*

clean:
	rm -r _out

