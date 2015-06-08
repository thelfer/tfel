AM_CPPFLAGS = -I$(top_srcdir)/include                     \
	      -I$(top_srcdir)/mfront/include              \
	      -I$(top_srcdir)/mfront/tests/models/include

check_LTLIBRARIES = libLicosMFrontModel.la    \
		    libGerminalMFrontModel.la

libLicosMFrontModel_la_SOURCES = LicosModelInterface.cxx
libLicosMFrontModel_la_LIBADD  = -L$(top_builddir)/mfront/src \
			 	 -lMFrontLogStream  

libGerminalMFrontModel_la_SOURCES = MFrontGerminalModelInterface.cxx     \
				    MFrontPleiadesModelInterfaceBase.cxx
libGerminalMFrontModel_la_LIBADD  = -L$(top_builddir)/mfront/src \
			 	    -lMFrontLogStream  

EXTRA_DIST=CMakeLists.txt
