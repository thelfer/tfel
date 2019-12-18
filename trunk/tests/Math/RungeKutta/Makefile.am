## Makefile.am -- Process this file with automake to produce a Makefile.in file.
SUBDIRS = include

AM_CPPFLAGS = -I$(top_srcdir)/include \
	      -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/RungeKutta
test_PROGRAMS = runge_kutta2 \
		runge_kutta4 \
		rk42         \
		rk54

LDADD = -L$(top_builddir)/src/Math                  \
	-L$(top_builddir)/src/Utilities             \
	-L$(top_builddir)/src/Exception             \
	-lTFELMath -lTFELUtilities -lTFELException

runge_kutta2_SOURCES = runge_kutta2.cxx
runge_kutta4_SOURCES = runge_kutta4.cxx
rk42_SOURCES         = rk42.cxx
rk54_SOURCES         = rk54.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml
	-$(RM) 'rk2_long double.txt'
	-$(RM) 'rk4_long double.txt'
	-$(RM) rk2_float.txt
	-$(RM) rk4_double.txt
	-$(RM) rk54.txt
	-$(RM) rk42.txt
	-$(RM) rk4_float.txt
	-$(RM) rk2_double.txt

endif

EXTRA_DIST=CMakeLists.txt
