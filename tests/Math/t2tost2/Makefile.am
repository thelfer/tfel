## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/T2tost2

test_PROGRAMS =	t2tost2 leftCauchyGreenTensorDerivative \
		rightCauchyGreenTensorDerivative        \
		RateOfDeformationDerivative             \
		SaintVenantKirchhoffTangentOperator

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

t2tost2_SOURCES      = t2tost2.cxx
leftCauchyGreenTensorDerivative_SOURCES     = leftCauchyGreenTensorDerivative.cxx
rightCauchyGreenTensorDerivative_SOURCES    = rightCauchyGreenTensorDerivative.cxx
SaintVenantKirchhoffTangentOperator_SOURCES = SaintVenantKirchhoffTangentOperator.cxx
RateOfDeformationDerivative_SOURCES         = RateOfDeformationDerivative.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
