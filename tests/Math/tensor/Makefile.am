## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/tensor
test_PROGRAMS = tensor                              \
		tensor2                             \
		tensor3                             \
		tensor4                             \
		TensorComputeJacobianDerivativeTest

LDADD = -L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Exception \
        -L$(top_builddir)/src/Tests     \
	-lTFELMath -lTFELUtilities      \
	-lTFELException -lTFELTests
tensor_SOURCES                = tensor.cxx
tensor2_SOURCES               = tensor2.cxx
tensor3_SOURCES               = tensor3.cxx
tensor4_SOURCES               = tensor4.cxx
TensorComputeJacobianDerivativeTest_SOURCES = TensorComputeJacobianDerivativeTest.cxx

TESTS=$(test_PROGRAMS)

endif

EXTRA_DIST=CMakeLists.txt
