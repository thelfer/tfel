## Makefile.am -- Process this file with automake to produce a Makefile.in file.
if TFEL_WIN
AM_CPPFLAGS=-DTFELException_EXPORTS
AM_LDFLAGS =-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

INCLUDES = -I$(top_srcdir)/include

lib_LTLIBRARIES = libTFELException.la 
libTFELException_la_SOURCES = TFELException.cxx
EXTRA_DIST = CMakeLists.txt