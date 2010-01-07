## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests

if WITH_GRAPHICS
SUBDIRS +=  po tplot
endif

doc:
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

EXTRA_DIST = config/config.rpath  bootstrap.sh        \
	     config/config.rpath \
	     GPL.txt             \
	     LGPL.txt            \
	     CeCILL-B.txt        \
	     INSTALL
