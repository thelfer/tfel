## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS  = -I$(top_srcdir)/include
if TFEL_WIN
AM_CPPFLAGS += -DTFELUnicodeSupport_EXPORTS
AM_LDFLAGS   = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELUnicodeSupport.la 
libTFELUnicodeSupport_la_SOURCES = UnicodeSupport.cxx

EXTRA_DIST=CMakeLists.txt