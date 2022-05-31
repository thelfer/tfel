## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_builddir)/include \
	          -I$(top_srcdir)/include

if TFEL_WIN
AM_LDFLAGS   = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELConfig.la 
libTFELConfig_la_SOURCES  = GetInstallPath.cxx \
			               GetTFELVersion.c

libTFELConfig_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELConfig_EXPORTS

EXTRA_DIST = CMakeLists.txt
