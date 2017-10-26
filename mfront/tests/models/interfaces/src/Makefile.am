AM_CPPFLAGS = -I$(top_srcdir)/include                                \
	      -I$(top_srcdir)/mfront/include                         \
	      -I$(top_srcdir)/mfront/tests/models/interfaces/include

check_LTLIBRARIES = libLicosMFrontModel.la    \
		    libGerminalMFrontModel.la \
		    libPleiadesModelInterface-2.0.la

libLicosMFrontModel_la_SOURCES = LicosModelInterface.cxx
libLicosMFrontModel_la_LIBADD  = -L$(top_builddir)/mfront/src \
			 	 -lMFrontLogStream  

libGerminalMFrontModel_la_SOURCES = MFrontGerminalModelInterface.cxx     \
				    MFrontPleiadesModelInterfaceBase.cxx
libGerminalMFrontModel_la_LIBADD  = -L$(top_builddir)/mfront/src \
			 	    -lMFrontLogStream  

libPleiadesModelInterface_2_0_la_SOURCES = PleiadesModelInterface-2.0.cxx
libPleiadesModelInterface_2_0_la_LIBADD  = -L$(top_builddir)/mfront/src \
			 	           -lMFrontLogStream  

EXTRA_DIST=CMakeLists.txt
