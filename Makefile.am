## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests cmake docs

if !TFEL_WIN
SUBDIRS += tfel-doc
endif

doc: mfront-doc
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

mfront-doc:
	mkdir -p docs/mfront
	cd docs/mfront ;                                                      \
	ln -s @abs_top_srcdir@/docs/tex/texmf                             . ; \
	if test "@top_srcdir@" != "@top_builddir@" ;                          \
	then                                                                  \
		ln -s @abs_top_srcdir@/docs/mfront/Images                 . ; \
		ln -s @abs_top_srcdir@/docs/mfront/mfront                 . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Bibliographie.bib      . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Makefile.doc           . ; \
		ln -s @abs_top_srcdir@/docs/mfront/mfront.tex             . ; \
		ln -s @abs_top_srcdir@/docs/mfront/style.ist              . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Champs.tex             . ; \
		ln -s @abs_top_srcdir@/docs/mfront/annexe-orthotropie.tex . ; \
	fi ;                                                                  \
	make -f Makefile.doc

clean-local:
	cd docs/mfront ;                             \
	rm -f texmf                                ; \
	if test "@top_srcdir@" != "@top_builddir@" ; \
	then                                         \
		 rm -f Images                      ; \
		 rm -f mfront                      ; \
		 rm -f Bibliographie.bib           ; \
		 rm -f Makefile.doc                ; \
		 rm -f mfront.tex                  ; \
		 rm -f style.ist                   ; \
		 rm -f Champs.tex                  ; \
		 rm -f annexe-orthotropie.tex      ; \
	fi ;

EXTRA_DIST = bootstrap.sh        				     \
	     CMakeLists.txt      				     \
	     LGPL.txt            				     \
	     CeCILL-B.txt        				     \
	     INSTALL
