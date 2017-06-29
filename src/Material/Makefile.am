## Makefile.am -- Process this file with automake to produce a Makefile.in file.

if TFEL_WIN
AM_LDFLAGS  = -no-undefined -avoid-version -Wl,--add-stdcall-alias -Wl,--kill-at
endif

lib_LTLIBRARIES = libTFELMaterial.la
libTFELMaterial_la_SOURCES = DDIF2Base.cxx                             \
			     BoundsCheck.cxx                           \
			     MaterialException.cxx                     \
			     ModellingHypothesis.cxx                   \
			     FiniteStrainBehaviourTangentOperator.cxx  \
			     SlipSystemsDescription.cxx                \
			     LogarithmicStrainHandler.cxx
libTFELMaterial_la_LIBADD = -L$(top_builddir)/src/Math      -lTFELMath      \
		            -L$(top_builddir)/src/NUMODIS   -lTFELNUMODIS   \
			    -L$(top_builddir)/src/Utilities -lTFELUtilities \
		            -L$(top_builddir)/src/Exception -lTFELException
libTFELMaterial_la_CPPFLAGS  = -I$(top_srcdir)/include
libTFELMaterial_la_CPPFLAGS += -DTFELMaterial_EXPORTS

EXTRA_DIST = CMakeLists.txt
