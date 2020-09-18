# AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = interfaces

mfronts = ErrnoHandlingCheck.mfront                            \
	  VanadiumAlloy_YoungModulus_SRMA.mfront               \
	  VanadiumAlloy_PoissonRatio_SRMA.mfront               \
	  VanadiumAlloy_ShearModulus_SRMA.mfront               \
	  VanadiumAlloy_SpecificHeat_SRMA.mfront               \
	  VanadiumAlloy_ThermalExpansion_SRMA.mfront           \
	  VanadiumAlloy_ThermalConductivity_SRMA.mfront        \
	  YoungModulusTest.mfront                              \
	  PoissonRatioTest.mfront                              \
	  YoungModulusTest_1.mfront                            \
	  YoungModulusTest_2.mfront                            \
	  YoungModulusTest_3.mfront                            \
	  PoissonRatioTest_12.mfront                           \
	  PoissonRatioTest_23.mfront                           \
	  PoissonRatioTest_13.mfront                           \
	  ShearModulusTest_12.mfront                           \
	  ShearModulusTest_23.mfront                           \
	  ShearModulusTest_13.mfront                           \
	  ThermalExpansionCoefficientTest.mfront               \
	  ThermalExpansionCoefficientTest2.mfront              \
	  ThermalExpansionCoefficientTest_1.mfront             \
	  ThermalExpansionCoefficientTest_2.mfront             \
	  ThermalExpansionCoefficientTest_3.mfront             \
	  Inconel600_YoungModulus.mfront                       \
	  T91AusteniticSteel_ThermalExpansion_ROUX2007.mfront  \
	  T91AusteniticSteel_b_ROUX2007.mfront		       \
	  T91AusteniticSteel_F_ROUX2007.mfront		       \
	  T91AusteniticSteel_Kvisc_ROUX2007.mfront	       \
	  T91AusteniticSteel_Nexp_ROUX2007.mfront	       \
	  T91AusteniticSteel_Rinf_ROUX2007.mfront	       \
	  T91AusteniticSteel_YieldStress_ROUX2007.mfront       \
	  T91AusteniticSteel_YoungModulus_ROUX2007.mfront      \
	  T91MartensiticSteel_b_ROUX2007.mfront		       \
	  T91MartensiticSteel_C1_ROUX2007.mfront	       \
	  T91MartensiticSteel_C2_ROUX2007.mfront	       \
	  T91MartensiticSteel_F_ROUX2007.mfront		       \
	  T91MartensiticSteel_gamma1_ROUX2007.mfront	       \
	  T91MartensiticSteel_growth_ROUX2007.mfront	       \
	  T91MartensiticSteel_Kvisc_ROUX2007.mfront	       \
	  T91MartensiticSteel_Nexp_ROUX2007.mfront	       \
	  T91MartensiticSteel_Rinf_ROUX2007.mfront	       \
	  T91MartensiticSteel_ThermalExpansion_ROUX2007.mfront \
	  T91MartensiticSteel_YieldStress_ROUX2007.mfront      \
	  T91MartensiticSteel_YoungModulus_ROUX2007.mfront     \
	  YoungModulusBoundsCheckTest.mfront

EXTRA_DIST = $(mfronts)      \
	     CMakeLists.txt

if WITH_TESTS

AM_CPPFLAGS = -I@builddir@/include -I$(top_srcdir)/include

check_LTLIBRARIES = libMFrontMaterialProperties.la
nodist_libMFrontMaterialProperties_la_SOURCES = src/ErrnoHandlingCheck.cxx                     \
	 					src/VanadiumAlloy_YoungModulus_SRMA.cxx        \
						src/VanadiumAlloy_PoissonRatio_SRMA.cxx        \
						src/VanadiumAlloy_SpecificHeat_SRMA.cxx        \
						src/VanadiumAlloy_ThermalExpansion_SRMA.cxx    \
						src/VanadiumAlloy_ThermalConductivity_SRMA.cxx
libMFrontMaterialProperties_la_LIBADD = -lm

check_LTLIBRARIES += libMFrontMaterialProperties-cxx.la
nodist_libMFrontMaterialProperties_cxx_la_SOURCES = src/ErrnoHandlingCheck-cxx.cxx                     \
	 					    src/VanadiumAlloy_YoungModulus_SRMA-cxx.cxx        \
	 					    src/VanadiumAlloy_PoissonRatio_SRMA-cxx.cxx        \
						    src/VanadiumAlloy_SpecificHeat_SRMA-cxx.cxx        \
						    src/VanadiumAlloy_ThermalExpansion_SRMA-cxx.cxx    \
						    src/VanadiumAlloy_ThermalConductivity_SRMA-cxx.cxx


if HAVE_FORTRAN
check_LTLIBRARIES += libMFrontMaterialProperties-fortran.la
nodist_libMFrontMaterialProperties_fortran_la_SOURCES = src/ErrnoHandlingCheck-fortran.cxx                     \
							src/VanadiumAlloy_YoungModulus_SRMA-fortran.cxx        \
							src/VanadiumAlloy_PoissonRatio_SRMA-fortran.cxx        \
							src/VanadiumAlloy_SpecificHeat_SRMA-fortran.cxx        \
							src/VanadiumAlloy_ThermalExpansion_SRMA-fortran.cxx    \
							src/VanadiumAlloy_ThermalConductivity_SRMA-fortran.cxx
endif

if HAVE_CASTEM
check_LTLIBRARIES += libMFrontMaterialProperties-castem.la
nodist_libMFrontMaterialProperties_castem_la_SOURCES = src/VanadiumAlloy_YoungModulus_SRMA-castem.cxx        \
						       src/VanadiumAlloy_PoissonRatio_SRMA-castem.cxx        \
						       src/VanadiumAlloy_SpecificHeat_SRMA-castem.cxx        \
						       src/VanadiumAlloy_ThermalExpansion_SRMA-castem.cxx    \
						       src/VanadiumAlloy_ThermalConductivity_SRMA-castem.cxx \
						       src/Inconel600_YoungModulus-castem.cxx
endif

testdir = $(pkgdatadir)/mfront/tests/

test_PROGRAMS = test-c

test_c_SOURCES = test-c.c
test_c_LDFLAGS = -L@builddir@ -lMFrontMaterialProperties -lm
test_c_DEPENDENCIES = libMFrontMaterialProperties.la

test-c.c : include/ErrnoHandlingCheck.hxx                     \
	   include/VanadiumAlloy_YoungModulus_SRMA.hxx        \
	   include/VanadiumAlloy_PoissonRatio_SRMA.hxx        \
	   include/VanadiumAlloy_SpecificHeat_SRMA.hxx        \
	   include/VanadiumAlloy_ThermalExpansion_SRMA.hxx    \
	   include/VanadiumAlloy_ThermalConductivity_SRMA.hxx

test_PROGRAMS += test-cxx
test_cxx_SOURCES = test-cxx.cxx
test_cxx_LDFLAGS = -L@builddir@ -lMFrontMaterialProperties-cxx \
        -L$(top_builddir)/src/Tests -lTFELTests

test_cxx_DEPENDENCIES = libMFrontMaterialProperties-cxx.la


test-cxx.cxx : include/ErrnoHandlingCheck-cxx.hxx                     \
	       include/VanadiumAlloy_YoungModulus_SRMA-cxx.hxx        \
	       include/VanadiumAlloy_PoissonRatio_SRMA-cxx.hxx        \
	       include/VanadiumAlloy_SpecificHeat_SRMA-cxx.hxx        \
	       include/VanadiumAlloy_ThermalExpansion_SRMA-cxx.hxx    \
	       include/VanadiumAlloy_ThermalConductivity_SRMA-cxx.hxx


if ! TFEL_WIN
test_c_LDFLAGS += -lstdc++
endif

if HAVE_FORTRAN
test_PROGRAMS += test-f
test_f_SOURCES = test-f.c compute.f
test_f_LDFLAGS = -L@builddir@ -lMFrontMaterialProperties-fortran -lstdc++
test_f_DEPENDENCIES = libMFrontMaterialProperties-fortran.la
endif

TESTS=$(test_PROGRAMS)

include/%.hxx src/%.cxx : %.mfront
if !TFEL_WIN
	@top_builddir@/mfront/src/mfront --interface=c $<
else
	@top_builddir@/mfront/src/mfront.exe --interface=c $<
endif

include/%-cxx.hxx src/%-cxx.cxx : %.mfront
if !TFEL_WIN
	@top_builddir@/mfront/src/mfront --interface=cxx $<
else
	@top_builddir@/mfront/src/mfront.exe --interface=cxx $<
endif

if HAVE_FORTRAN
src/%-fortran.cxx : %.mfront
if !TFEL_WIN
	@top_builddir@/mfront/src/mfront --interface=fortran $<
else
	@top_builddir@/mfront/src/mfront.exe --interface=fortran $<
endif
endif

if HAVE_CASTEM
src/%-castem.cxx : %.mfront
if !TFEL_WIN
	@top_builddir@/mfront/src/mfront --interface=castem $<
else
	@top_builddir@/mfront/src/mfront.exe --interface=castem $<
endif
endif

clean-local:
	-$(RM) -fr src include *.xml

endif
