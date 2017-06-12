## Makefile.am -- Process this file with automake to produce a Makefile.in file.

if TFEL_WIN
AM_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELNUMODIS.la
libTFELNUMODIS_la_SOURCES = BCC.cxx 	   \
                            Crystallo.cxx  \
                            Cubic.cxx	   \
                            FCC.cxx	   \
                            GSystem.cxx	   \
                            HCP.cxx	   \
                            Hardening.cxx  \
                            IBurgers.cxx   \
                            IDirection.cxx \
                            IPlane.cxx	   \
                            Utilities.cxx  \
                            Vect3.cxx
libTFELNUMODIS_la_LIBADD = -L$(top_builddir)/src/Math      -lTFELMath      \
		           -L$(top_builddir)/src/Exception -lTFELException \
			   -L$(top_builddir)/src/Utilities -lTFELUtilities
libTFELNUMODIS_la_CPPFLAGS  = -I$(top_srcdir)/include
libTFELNUMODIS_la_CPPFLAGS += -DTFELNUMODIS_EXPORTS

EXTRA_DIST = CMakeLists.txt
