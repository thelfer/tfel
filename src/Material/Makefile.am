## Makefile.am -- Process this file with automake to produce a Makefile.in file.
if TFEL_WIN
AM_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libTFELMaterial.la

libTFELMaterial_la_SOURCES = MaterialException.cxx                     \
			     ModellingHypothesis.cxx                   \
			     DDIF2Base.cxx                             \
			     FiniteStrainBehaviourTangentOperator.cxx
libTFELMaterial_la_LIBADD = -L$(top_builddir)/src/Math      -lTFELMath      \
		            -L$(top_builddir)/src/Exception -lTFELException \
			    -L$(top_builddir)/src/Utilities -lTFELUtilities
libTFELMaterial_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELMaterial_EXPORTS

EXTRA_DIST = CMakeLists.txt
