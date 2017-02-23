#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# This file is part of the build system of a GAP kernel extension.
# Requires automake.
#
ACLOCAL_AMFLAGS = -I m4

BINARCHDIR = bin/$(GAPARCH)
GAPINSTALLLIB = $(BINARCHDIR)/Gurobify.so

lib_LTLIBRARIES = Gurobify.la

Gurobify_la_SOURCES = src/Gurobify.c
Gurobify_la_CPPFLAGS = $(GAP_CPPFLAGS) -DCONFIG_H
Gurobify_la_LDFLAGS = -module -avoid-version
if SYS_IS_CYGWIN
Gurobify_la_LDFLAGS += -no-undefined -version-info 0:0:0 
Gurobify_la_LDFLAGS += -Wl,$(GAPROOT)/bin/$(GAPARCH)/gap.dll
endif

all-local: $(GAPINSTALLLIB)

$(GAPINSTALLLIB): Gurobify.la
	$(mkdir_p) $(BINARCHDIR)
if SYS_IS_CYGWIN
	cp .libs/Gurobify.dll $(GAPINSTALLLIB)
else
	cp .libs/Gurobify.so $(GAPINSTALLLIB)
endif

clean-local:
	rm -f $(GAPINSTALLLIB)

distclean-local:
	rm -rf bin/*

doc:
	($(GAPROOT)/bin/gap.sh -A makedoc.g)
