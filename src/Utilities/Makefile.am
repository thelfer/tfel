## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -DTFELUtilities_EXPORTS
if TFEL_WIN
AM_LDFLAGS =-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELUtilities.la 
libTFELUtilities_la_SOURCES = TextData.cxx         \
			      TerminalColors.cxx   \
			      GenTypeCastError.cxx \
			      Token.cxx            \
			      CxxKeywords.cxx      \
			      Name.cxx             \
			      CxxTokenizer.cxx     \
			      StringAlgorithms.cxx

EXTRA_DIST = CMakeLists.txt
