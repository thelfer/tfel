## Makefile.am -- Process this file with automake to produce a Makefile.in file.

INCLUDES = -I$(top_srcdir)/include

lib_LTLIBRARIES = libTFELMaterialLaw.la 
libTFELMaterialLaw_la_SOURCES = MaterialLawException.cxx
libTFELMaterialLaw_la_LIBADD = -L$(top_builddir)/src/Math      -lTFELMath      \
			       -L$(top_builddir)/src/Exception -lTFELException \
			       -L$(top_builddir)/src/Utilities -lTFELUtilities
