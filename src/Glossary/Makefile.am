## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir) -DTFELGlossary_EXPORTS
if TFEL_WIN
AM_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELGlossary.la 
libTFELGlossary_la_SOURCES = GlossaryEntry.cxx \
			     Glossary.cxx
libTFELGlossary_la_LIBADD = -L$(top_builddir)/src/Utilities/ -lTFELUtilities

EXTRA_DIST = CMakeLists.txt
