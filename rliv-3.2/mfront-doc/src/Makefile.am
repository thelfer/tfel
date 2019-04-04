# Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_srcdir)/include         \
	      -I$(top_srcdir)/mfront/include  \
	      -I$(top_srcdir)/mfront-doc/include

if HAVE_CASTEM
if !LOCAL_CASTEM_HEADER
AM_CPPFLAGS += -I$(CASTEM_ROOT)/include
AM_CPPFLAGS += -I$(CASTEM_ROOT)/include/c
endif
AM_CPPFLAGS += $(CASTEM_CPPFLAGS)
endif

bin_PROGRAMS = mfront-doc
mfront_doc_SOURCES = mfront-doc.cxx BehaviourDocumentationGenerator.cxx
mfront_doc_LDADD = -L$(top_builddir)/mfront/src -lTFELMFront -lMFrontLogStream \
                   -L$(top_builddir)/src/Glossary -lTFELGlossary               \
                   -L$(top_builddir)/src/Material -lTFELMaterial               \
                   -L$(top_builddir)/src/Math -lTFELMath                       \
                   -L$(top_builddir)/src/Utilities -lTFELUtilities             \
                   -L$(top_builddir)/src/System -lTFELSystem

if !TFEL_WIN
mfront_doc_LDFLAGS  = -export-dynamic
endif # TFEL_WIN

EXTRA_DIST=CMakeLists.txt
