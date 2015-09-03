## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_builddir)/src/TFELConfig/ \
	      -I$(top_srcdir)/include

if TFEL_WIN
AM_LDFLAGS=-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

bin_PROGRAMS = tfel-config
tfel_config_SOURCES = tfel-config.cxx               
EXTRA_DIST = CMakeLists.txt
