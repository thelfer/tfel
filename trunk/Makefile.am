## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests bindings cmake docs

if !TFEL_WIN
SUBDIRS += tfel-doc
endif

doc-install: doc
	mkdir -p @docdir@
	mkdir -p @docdir@/reference-guide
	mkdir -p @docdir@/mfront
	mkdir -p @docdir@/mtest
	mkdir -p @docdir@/presentations
	cp -R docs/html @docdir@
	cp    docs/tfel/reference-guide.pdf              @docdir@/reference-guide
	cp    docs/mfront/mfront.pdf                     @docdir@/mfront
	cp    docs/mfront/behaviours.pdf                 @docdir@/mfront
	cp    docs/mfront/umat.pdf                       @docdir@/mfront
	cp    docs/mfront/aster.pdf                      @docdir@/mfront
	cp    docs/mtest/mtest.pdf                       @docdir@/mtest
	cp    docs/presentations/mfront-presentation.pdf @docdir@/presentations

doc: doc-pdf doc-html

doc-html: tfel-doc-html

doc-pdf: tfel-doc-pdf mfront-doc-pdf mtest-doc-pdf presentations-doc-pdf

tfel-doc-html:
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

tfel-doc-pdf:
	cd docs/tfel ;                                            \
	make -f Makefile.doc

mtest-doc-pdf:
	cd docs/mtest ;                                            \
	make -f Makefile.doc

mfront-doc-pdf:
	cd docs/mfront ;                                            \
	make -f Makefile.doc mfront ;				    \
	make -f Makefile.doc behaviours ;			    \
	make -f Makefile.doc aster ;				    \
	make -f Makefile.doc umat

presentations-doc-pdf:
	cd docs/presentations ;                                     \
	make -f Makefile.doc

EXTRA_DIST = bootstrap.sh   \
	     CMakeLists.txt \
	     LGPL.txt       \
	     CeCILL-B.txt   \
	     INSTALL        \
             INSTALL-cmake
