## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests cmake tfel-doc

doc:
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

EXTRA_DIST = bootstrap.sh        \
	     CMakeLists.txt      \
	     LGPL.txt            \
	     CeCILL-B.txt        \
	     INSTALL
