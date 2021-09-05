## Makefile.am -- Process this file with automake to produce a Makefile.in file.

AM_CPPFLAGS = -I$(top_srcdir)/include            \
	      -I$(top_srcdir)/mfront/include     \
	      -I$(top_srcdir)/docs/mfront/abaqus

if WITH_TESTS

testdir = $(pkgdatadir)/mfront/tests/unit-tests
test_PROGRAMS = DSLTest                                    \
		DSLTest2                                   \
		SwellingTest                               \
		SupportedTypesTest                         \
		BehaviourDataTest                          \
		BehaviourDescriptionTest                   \
		BehaviourDescriptionBoundsHandlingTest     \
		TargetsDescriptionTest                     \
		StandardElasticityBrickTest                \
         StandardElastoViscoPlasticityBrickTest     \
		LocalDataStructureTest                     \
		ComputeStiffnessTensorTest                 \
		SlipSystemsTest                            \
		UMATTest				   \
		VUMATTest_dp				   \
		VUMATTest_sp                               \
		OrthotropicAxesConventionTest

DSLTest_SOURCES                     = DSLTest.cxx
DSLTest2_SOURCES                    = DSLTest2.cxx
SwellingTest_SOURCES                = SwellingTest.cxx
SupportedTypesTest_SOURCES          = SupportedTypesTest.cxx
BehaviourDataTest_SOURCES           = BehaviourDataTest.cxx
BehaviourDescriptionTest_SOURCES    = BehaviourDescriptionTest.cxx
TargetsDescriptionTest_SOURCES      = TargetsDescriptionTest.cxx
StandardElasticityBrickTest_SOURCES = StandardElasticityBrickTest.cxx
StandardElastoViscoPlasticityBrickTest_SOURCES = StandardElastoViscoPlasticityBrickTest.cxx
LocalDataStructureTest_SOURCES      = LocalDataStructureTest.cxx
ComputeStiffnessTensorTest_SOURCES  = ComputeStiffnessTensorTest.cxx
BehaviourDescriptionBoundsHandlingTest_SOURCES = BehaviourDescriptionBoundsHandlingTest.cxx
SlipSystemsTest_SOURCES    = SlipSystemsTest.cxx
UMATTest_SOURCES           = UMATTest.cxx
VUMATTest_sp_SOURCES       = VUMATTest_sp.cxx
VUMATTest_dp_SOURCES       = VUMATTest_dp.cxx
OrthotropicAxesConventionTest_SOURCES = OrthotropicAxesConventionTest.cxx

LDADD = -L$(top_builddir)/mfront/src    \
	-L$(top_builddir)/src/Material  \
        -L$(top_builddir)/src/Math      \
	-L$(top_builddir)/src/System    \
	-L$(top_builddir)/src/Glossary  \
	-L$(top_builddir)/src/Utilities \
	-L$(top_builddir)/src/Exception \
        -L$(top_builddir)/src/Tests     \
	-lTFELMFront -lTFELMaterial     \
	-lTFELMath -lTFELSystem         \
	-lTFELUtilities -lTFELGlossary  \
	-lTFELException -lTFELTests

UMATTest_LDADD     = $(LDADD)
VUMATTest_dp_LDADD = $(LDADD)
VUMATTest_sp_LDADD = $(LDADD)
if !TFEL_WIN
UMATTest_LDADD     += -lpthread
VUMATTest_sp_LDADD += -lpthread
VUMATTest_dp_LDADD += -lpthread
if TFEL_LINUX
UMATTest_LDADD     += -ldl
VUMATTest_sp_LDADD += -ldl
VUMATTest_dp_LDADD += -ldl
endif
endif # !TFEL_WIN



endif

EXTRA_DIST=CMakeLists.txt      \
	   mfront/test1.mfront \
	   mfront/test2.mfront 

