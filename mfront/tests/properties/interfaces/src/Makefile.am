INCLUDES = -I$(top_srcdir)/include                                    \
	   -I$(top_srcdir)/mfront/include                             \
	   -I$(top_srcdir)/mfront/tests/properties/interfaces/include

check_LTLIBRARIES = libPleiadesMaterialProperties-2.0.la
libPleiadesMaterialProperties_2_0_la_SOURCES=MFrontPleiadesMaterialPropertyInterface-2.0.cxx
libPleiadesMaterialProperties_2_0_la_LIBADD  = -L$(top_builddir)/mfront/src \
					       -lMFrontLogStream  

EXTRA_DIST=CMakeLists.txt