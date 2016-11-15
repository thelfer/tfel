## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/T2toT2

test_PROGRAMS =	t2tot2 t2tot2-specialvalues                    \
		TensorProductDerivative                        \
		TensorSquareDerivative                         \
		SpinRateDerivative                             \
		VelocityGradientDerivative                     \
		TransposeDerivative                            \
		ComputeTensorDeterminantSecondDerivativeTest

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

t2tot2_SOURCES                                       = t2tot2.cxx
t2tot2_specialvalues_SOURCES                         = t2tot2-specialvalues.cxx
TensorProductDerivative_SOURCES                      = TensorProductDerivative.cxx
TensorSquareDerivative_SOURCES                       = TensorSquareDerivative.cxx
TransposeDerivative_SOURCES                          = TransposeDerivative.cxx
VelocityGradientDerivative_SOURCES                   = VelocityGradientDerivative.cxx
SpinRateDerivative_SOURCES                           = SpinRateDerivative.cxx
ComputeTensorDeterminantSecondDerivativeTest_SOURCES = ComputeTensorDeterminantSecondDerivativeTest.cxx
TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
