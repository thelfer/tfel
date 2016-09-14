## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS  = -I$(top_srcdir)/include
if TFEL_WIN
AM_CPPFLAGS += -DTFELUtilities_EXPORTS
AM_LDFLAGS   = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELUtilities.la 
libTFELUtilities_la_SOURCES = TextData.cxx           \
			      GenTypeCastError.cxx   \
			      Token.cxx              \
			      Data.cxx               \
			      CxxKeywords.cxx        \
			      CxxTokenizer.cxx       \
			      TerminalColors.cxx     \
			      StringAlgorithms.cxx   \
			      Argument.cxx           \
			      ArgumentParser.cxx 

EXTRA_DIST = CMakeLists.txt
