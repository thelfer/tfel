AM_CPPFLAGS = -I$(top_srcdir)/include            \
	          -I$(top_srcdir)/tfel-check/include \
	          -I$(top_srcdir)/mfront/include

lib_LTLIBRARIES=libTFELCheck.la

libTFELCheck_la_SOURCES = Configuration.cxx     \
			  ConfigurationManager.cxx          \
			  PCILogDriver.cxx	            \
			  PCJUnitDriver.cxx	            \
		          PCLogger.cxx		            \
			  PCTextDriver.cxx	            \
			  TestLauncher.cxx	            \
			  TestLauncherV1.cxx	            \
			  SplineInterpolation.cxx	    \
			  SplineLocalInterpolation.cxx	    \
			  Interpolation.cxx	            \
			  LinearInterpolation.cxx	    \
			  Linearization.cxx		    \
			  NoInterpolation.cxx	            \
			  Column.cxx		            \
			  Comparison.cxx		    \
			  AbsoluteComparison.cxx	    \
			  RelativeComparison.cxx	    \
			  RelativeAndAbsoluteComparison.cxx \
			  MixedComparison.cxx		    \
			  AreaComparison.cxx		    \
			  Test.cxx

libTFELCheck_la_LDFLAGS  = -L$(top_builddir)/src/System  -lTFELSystem       \
			               -L$(top_builddir)/src/Utilities  -lTFELUtilities \
			               -L$(top_builddir)/src/Math  -lTFELMathParser     \
			               -lTFELMathCubicSpline
libTFELCheck_la_CPPFLAGS = $(AM_CPPFLAGS) -DTFELCheck_EXPORTS

bin_PROGRAMS = tfel-check

tfel_check_SOURCES = tfel-check.cxx
tfel_check_LDADD   = libTFELCheck.la
tfel_check_LDFLAGS = -L$(top_builddir)/mfront/src    -lTFELMFront    \
                     -L$(top_builddir)/src/System    -lTFELSystem    \
                     -L$(top_builddir)/src/Utilities -lTFELUtilities
EXTRA_DIST=CMakeLists.txt
