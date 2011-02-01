INCLUDES=-I@top_srcdir@/tfel-check/include \
	 -I@top_srcdir@/include

bin_PROGRAMS =  tfel-check

tfel_check_SOURCES  = tfel-check.cxx   \
		      TestLauncher.cxx

tfel_check_LDADD = -L@top_builddir@/src/System    \
		   -L@top_builddir@/src/Utilities \
		   -L@top_builddir@/src/Exception \
		   -lTFELSystem                   \
		   -lTFELUtilities                \
		   -lTFELException
