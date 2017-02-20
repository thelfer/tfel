AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/stensor
test_PROGRAMS = stensor                                  \
		stensor2                                 \
		stensor4                                 \
		stensor5                                 \
		stensor6                                 \
		stensor7                                 \
		stensor8                                 \
		stensor9                                 \
		stensor10                                \
		stensor_changeBasis                      \
		stensor_changeBasis2                     \
		stensor_changeBasis3                     \
		stensor_changeBasis4                     \
		stensor_eigenvalues                      \
		stensor_eigenvectors                     \
		stensor_eigenvectors2                    \
		stensor_isotropic_function               \
		StensorFromTinyMatrixColumnView          \
		InvariantsDerivatives                    \
		StensorComputeJacobianDerivativeTest

LDADD = -L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Exception \
        -L$(top_builddir)/src/Tests     \
	-lTFELMath -lTFELUtilities      \
	-lTFELException -lTFELTests

stensor_SOURCES               = stensor.cxx              
stensor2_SOURCES              = stensor2.cxx
stensor4_SOURCES              = stensor4.cxx
stensor5_SOURCES              = stensor5.cxx
stensor6_SOURCES              = stensor6.cxx
stensor7_SOURCES              = stensor7.cxx
stensor8_SOURCES              = stensor8.cxx
stensor9_SOURCES              = stensor9.cxx
stensor10_SOURCES             = stensor10.cxx
stensor_changeBasis_SOURCES   = stensor_changeBasis.cxx
stensor_changeBasis2_SOURCES  = stensor_changeBasis2.cxx
stensor_changeBasis3_SOURCES  = stensor_changeBasis3.cxx
stensor_changeBasis4_SOURCES  = stensor_changeBasis4.cxx
stensor_eigenvalues_SOURCES   = stensor_eigenvalues.cxx  
stensor_eigenvectors_SOURCES  = stensor_eigenvectors.cxx 
stensor_eigenvectors2_SOURCES = stensor_eigenvectors2.cxx
stensor_isotropic_function_SOURCES = stensor_isotropic_function.cxx
StensorFromTinyMatrixColumnView_SOURCES = StensorFromTinyMatrixColumnView.cxx
InvariantsDerivatives_SOURCES = InvariantsDerivatives.cxx
StensorComputeJacobianDerivativeTest_SOURCES = StensorComputeJacobianDerivativeTest.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml

endif

EXTRA_DIST=CMakeLists.txt
