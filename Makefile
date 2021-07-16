UNAME_S := $(shell uname -s)
MFILE := Makefile

ifeq ($(UNAME_S),Linux)
	MFILE:="Makefile.Linux"
endif

all:
	$(MAKE) -C src -f $(MFILE)
