## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests cmake tfel-doc

doc: mfront-doc
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

mfront-doc:
	mkdir -p docs/mfront
	cd docs/mfront ;                                                      \
	if test "@top_srcdir@" != "@top_builddir@" ;                          \
	then                                                                  \
		ln -s @abs_top_srcdir@/docs/tex/texmf                     . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Images                 . ; \
		ln -s @abs_top_srcdir@/docs/mfront/mfront                 . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Bibliographie.bib      . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Makefile.doc           . ; \
		ln -s @abs_top_srcdir@/docs/mfront/mfront.tex             . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Champs.tex             . ; \
		ln -s @abs_top_srcdir@/docs/mfront/annexe-orthotropie.tex . ; \
	fi ;                                                                  \
	make -f Makefile.doc


EXTRA_DIST = bootstrap.sh        \
	     CMakeLists.txt      \
	     LGPL.txt            \
	     CeCILL-B.txt        \
	     INSTALL
