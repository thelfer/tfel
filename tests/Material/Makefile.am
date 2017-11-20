if WITH_TESTS

AM_CPPFLAGS = -I$(top_srcdir)/include

testdir = $(pkgdatadir)/tests/Material
test_PROGRAMS = boundsCheck \
		OrthotropicAxesConventionTest          \
		AbaqusTangentOperator                  \
		AbaqusTangentOperator2                 \
		FiniteStrainBehaviourTangentOperator   \
		FiniteStrainBehaviourTangentOperator2  \
		FiniteStrainBehaviourTangentOperator3  \
		FiniteStrainBehaviourTangentOperator4  \
		FiniteStrainBehaviourTangentOperator5  \
		FiniteStrainBehaviourTangentOperator6  \
		FiniteStrainBehaviourTangentOperator7  \
		FiniteStrainBehaviourTangentOperator8  \
		FiniteStrainBehaviourTangentOperator9  \
		FiniteStrainBehaviourTangentOperator10 \
		FiniteStrainBehaviourTangentOperator11 \
		FiniteStrainBehaviourTangentOperator12 \
		FiniteStrainBehaviourTangentOperator13 \
		LogarithmicStrainHandlerTest           \
		Hyperelasticity                        \
		Ogden                                  \
		SlipSystemsDescriptionTest             \
		HosfordSecondDerivativeTest            \
		BarlatSecondDerivativeTest

LDADD = -lTFELMaterial -lTFELMath       \
	-lTFELUtilities -lTFELException \
	-lTFELTests                     \
	-L$(top_builddir)/src/Material  \
	-L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Tests     \
	-L$(top_builddir)/src/Exception

boundsCheck_SOURCES = boundsCheck.cxx               
OrthotropicAxesConventionTest_SOURCES          = OrthotropicAxesConventionTest.cxx
AbaqusTangentOperator_SOURCES                  = AbaqusTangentOperator.cxx
AbaqusTangentOperator2_SOURCES                 = AbaqusTangentOperator2.cxx
FiniteStrainBehaviourTangentOperator_SOURCES   = FiniteStrainBehaviourTangentOperator.cxx
FiniteStrainBehaviourTangentOperator2_SOURCES  = FiniteStrainBehaviourTangentOperator2.cxx
FiniteStrainBehaviourTangentOperator3_SOURCES  = FiniteStrainBehaviourTangentOperator3.cxx
FiniteStrainBehaviourTangentOperator4_SOURCES  = FiniteStrainBehaviourTangentOperator4.cxx
FiniteStrainBehaviourTangentOperator5_SOURCES  = FiniteStrainBehaviourTangentOperator5.cxx
FiniteStrainBehaviourTangentOperator6_SOURCES  = FiniteStrainBehaviourTangentOperator6.cxx
FiniteStrainBehaviourTangentOperator7_SOURCES  = FiniteStrainBehaviourTangentOperator7.cxx
FiniteStrainBehaviourTangentOperator8_SOURCES  = FiniteStrainBehaviourTangentOperator8.cxx
FiniteStrainBehaviourTangentOperator9_SOURCES  = FiniteStrainBehaviourTangentOperator9.cxx
FiniteStrainBehaviourTangentOperator10_SOURCES = FiniteStrainBehaviourTangentOperator10.cxx
FiniteStrainBehaviourTangentOperator11_SOURCES = FiniteStrainBehaviourTangentOperator11.cxx
FiniteStrainBehaviourTangentOperator12_SOURCES = FiniteStrainBehaviourTangentOperator12.cxx
FiniteStrainBehaviourTangentOperator13_SOURCES = FiniteStrainBehaviourTangentOperator13.cxx
LogarithmicStrainHandlerTest_SOURCES           = LogarithmicStrainHandlerTest.cxx
Hyperelasticity_SOURCES = Hyperelasticity.cxx
Ogden_SOURCES = Ogden.cxx
SlipSystemsDescriptionTest_SOURCES  = SlipSystemsDescriptionTest.cxx
HosfordSecondDerivativeTest_SOURCES = HosfordSecondDerivativeTest.cxx
BarlatSecondDerivativeTest_SOURCES  = BarlatSecondDerivativeTest.cxx

TESTS=$(test_PROGRAMS)

clean-local:
	-$(RM) *.xml

endif

EXTRA_DIST=CMakeLists.txt
