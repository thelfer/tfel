if TFEL_WIN
AM_LDFLAGS  =-no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES  = libTFELPhysicalConstants.la 
libTFELPhysicalConstants_la_SOURCES   = PhysicalConstants.cxx
libTFELPhysicalConstants_la_CPPFLAGS  = -I$(top_srcdir)/include
if TFEL_WIN
libTFELPhysicalConstants_la_CPPFLAGS += $(AM_CPPFLAGS) -DTFELPhysicalConstants_EXPORTS
endif

EXTRA_DIST = CMakeLists.txt
