## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include \
	    $(GTKMM_CFLAGS)

lib_LTLIBRARIES = libTFELGraphics.la 
libTFELGraphics_la_SOURCES = ArrowStyle.cxx         \
			     Curve.cxx              \
			     DataCurve.cxx	    \
			     EvaluatedCurve.cxx	    \
			     GraphCoordinates.cxx   \
			     Graph.cxx		    \
			     GraphTheme.cxx	    \
			     StandardGraphTheme.cxx \
			     ThemeManager.cxx

libTFELGraphics_la_LIBADD = -L@top_builddir@/src/Exception/ \
			    -L@top_builddir@/src/Math/      \
			    -lTFELMathParser                \
			    -lTFELMath -lTFELException      \
			    $(GTKMM_LIBS)                   \
		 	    @LTLIBINTL@

if HAVE_GSL
libTFELGraphics_la_LIBADD += @GSLLIBS@
endif

if HAVE_OCTAVE
libTFELGraphics_la_LIBADD += @OCTAVELIBS@
endif

