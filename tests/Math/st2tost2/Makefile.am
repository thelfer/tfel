## Makefile.am -- Process this file with automake to produce a Makefile.in file.
AM_CPPFLAGS = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/Math/st2tost2

test_PROGRAMS =	st2tost2                                 \
		st2tost2-2                               \
		st2tost2-3                               \
		ST2toST2ChangeBasis                      \
		ST2toST2FromTinyMatrixView               \
		ST2toST2FromTinyMatrixView2              \
		ST2toST2PushForward                      \
		StensorSquareDerivative                  \
		ConvertToTangentModuliTest               \
		ConvertLogarithmicStrainTangentOperator  \
		ComputeDeterminantSecondDerivativeTest

LDADD = -lTFELMath -lTFELException -lTFELTests \
	-L$(top_builddir)/src/Tests            \
	-L$(top_builddir)/src/Math             \
	-L$(top_builddir)/src/Exception

st2tost2_SOURCES      = st2tost2.cxx
st2tost2_2_SOURCES    = st2tost2-2.cxx
st2tost2_3_SOURCES    = st2tost2-3.cxx
ST2toST2ChangeBasis_SOURCES = ST2toST2ChangeBasis.cxx
ST2toST2FromTinyMatrixView_SOURCES  = ST2toST2FromTinyMatrixView.cxx
ST2toST2FromTinyMatrixView2_SOURCES = ST2toST2FromTinyMatrixView2.cxx
ST2toST2PushForward_SOURCES = ST2toST2PushForward.cxx
StensorSquareDerivative_SOURCES = StensorSquareDerivative.cxx
ConvertToTangentModuliTest_SOURCES = ConvertToTangentModuliTest.cxx
ConvertLogarithmicStrainTangentOperator_SOURCES = ConvertLogarithmicStrainTangentOperator.cxx
ComputeDeterminantSecondDerivativeTest_SOURCES  = ComputeDeterminantSecondDerivativeTest.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml

endif

EXTRA_DIST=CMakeLists.txt
