## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront mtest tests bindings cmake docs

if !TFEL_WIN
SUBDIRS += tfel-doc tfel-check
endif

DISTCHECK_CONFIGURE_FLAGS = --enable-tests

doc-install: doc
	mkdir -p @docdir@
	mkdir -p @docdir@/reference-guide
	mkdir -p @docdir@/mfront
	mkdir -p @docdir@/mtest
	mkdir -p @docdir@/ptest
	mkdir -p @docdir@/release-notes-2.0
	mkdir -p @docdir@/release-notes-3.0
	mkdir -p @docdir@/release-notes-2.0.4
	mkdir -p @docdir@/presentations
	cp -R docs/html @docdir@
	cp    docs/tfel/reference-guide.pdf              @docdir@/reference-guide
	cp    docs/mfront/general/mfront.pdf             @docdir@/mfront
	cp    docs/mfront/behaviours/behaviours.pdf      @docdir@/mfront
	cp    docs/mfront/finitestrain/finitestrain.pdf  @docdir@/mfront
	cp    docs/mfront/planestress/planestress.pdf    @docdir@/mfront
	cp    docs/mfront/castem/castem.pdf              @docdir@/mfront
	cp    docs/mfront/aster/aster.pdf                @docdir@/mfront
	cp    docs/mfront/zmat/zmat.pdf                  @docdir@/mfront
	cp    docs/mfront/abaqus/abaqus.pdf              @docdir@/mfront
	cp    docs/mtest/mtest.pdf                       @docdir@/mtest
	cp    docs/mtest/ptest.pdf                       @docdir@/ptest
	cp    docs/release-notes-2.0/tfel-2.0.pdf        @docdir@/release-notes-2.0
	cp    docs/release-notes-2.0.3/tfel-2.0.3.pdf    @docdir@/release-notes-2.0.3
	cp    docs/release-notes-2.0.4/tfel-2.0.4.pdf    @docdir@/release-notes-2.0.4
	cp    docs/release-notes-3.0/tfel-3.0.pdf        @docdir@/release-notes-3.0

#	cp    docs/presentations/mfront-presentation.pdf @docdir@/presentations

doc: doc-pdf doc-html

doc-html: tfel-doc-html

doc-pdf: tfel-doc-pdf mfront-doc-pdf mtest-doc-pdf ptest-doc-pdf \
	 presentations-doc-pdf                                   \
	 release-notes-2.0-doc-pdf release-notes-2.0.3-doc-pdf   \
	 release-notes-2.0.4-doc-pdf release-notes-3.0-doc-pdf

tfel-doc-html:
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

tfel-doc-pdf:
	cd docs/tfel ;                                            \
	make -f Makefile.doc

mtest-doc-pdf:
	cd docs/mtest ;                                            \
	make -f Makefile.doc

ptest-doc-pdf:
	cd docs/ptest ;                                            \
	make -f Makefile.doc

release-notes-2.0-doc-pdf:
	cd docs/release-notes-2.0 ;                                  \
	make -f Makefile.doc

release-notes-2.0.3-doc-pdf:
	cd docs/release-notes-2.0.3 ;                              \
	make -f Makefile.doc

release-notes-2.0.4-doc-pdf:
	cd docs/release-notes-2.0.4 ;                              \
	make -f Makefile.doc

release-notes-3.0-doc-pdf:
	cd docs/release-notes-3.0 ;                                \
	make -f Makefile.doc

mfront-doc-pdf:
	cd docs/mfront ;                                            \
	make -f Makefile.doc all

presentations-doc-pdf:
# cd docs/presentations ;                                     \
# make -f Makefile.doc

EXTRA_DIST = bootstrap.sh    \
	     CMakeLists.txt  \
	     tfel.doxygen    \
	     INSTALL         \
             INSTALL-cmake   \
	     env-win32.sh.in \
	     README.md

licencesdir = $(pkgdatadir)
dist_licences_DATA = LICENCE-GNU-GPL \
                     LICENCE-CECILL-A-EN \
                     LICENCE-CECILL-A-FR
