## Makefile.am -- Process this file with automake to produce a Makefile.in file.
if TFEL_WIN
AM_LDFLAGS=-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

INCLUDES = -I$(top_srcdir)/src/TFELConfig/

bin_PROGRAMS = tfel-config
tfel_config_SOURCES = tfel-config.cxx               
EXTRA_DIST = CMakeLists.txt