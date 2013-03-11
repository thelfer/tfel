## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests bindings cmake docs

if !TFEL_WIN
SUBDIRS += tfel-doc
endif

doc: mfront-doc
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

mfront-doc:
	mkdir -p docs/mfront
	cd docs/mfront ;                                            \
	make -f Makefile.doc

EXTRA_DIST = bootstrap.sh        				    \
	     CMakeLists.txt      				    \
	     LGPL.txt            				    \
	     CeCILL-B.txt        				    \
	     INSTALL
