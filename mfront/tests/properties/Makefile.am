
if WITH_TESTS

mfronts = VanadiumAlloy_YoungModulus_SRMA.mfront        \
	  VanadiumAlloy_PoissonRatio_SRMA.mfront        \
	  VanadiumAlloy_SpecificHeat_SRMA.mfront        \
	  VanadiumAlloy_ThermalExpansion_SRMA.mfront    \
	  VanadiumAlloy_ThermalConductivity_SRMA.mfront

INCLUDES = -I@builddir@/include

check_LTLIBRARIES = libMFrontMaterialProperties.la
nodist_libMFrontMaterialProperties_la_SOURCES = src/VanadiumAlloy_YoungModulus_SRMA.cxx        \
						src/VanadiumAlloy_PoissonRatio_SRMA.cxx        \
						src/VanadiumAlloy_SpecificHeat_SRMA.cxx        \
						src/VanadiumAlloy_ThermalExpansion_SRMA.cxx    \
						src/VanadiumAlloy_ThermalConductivity_SRMA.cxx

if HAVE_FORTRAN
check_LTLIBRARIES += libMFrontMaterialProperties-fortran.la
nodist_libMFrontMaterialProperties_fortran_la_SOURCES = src/VanadiumAlloy_YoungModulus_SRMA-fortran.cxx        \
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
						       src/VanadiumAlloy_ThermalConductivity_SRMA-castem.cxx
endif

testdir = $(pkgdatadir)/mfront/tests/

check_PROGRAMS = test-c

test_c_SOURCES = test-c.c
test_c_LDFLAGS = -L@builddir@ -lMFrontMaterialProperties -lstdc++

if HAVE_FORTRAN
check_PROGRAMS += test-f
test_f_SOURCES = test-f.c compute.f
test_f_LDFLAGS = -L@builddir@ -lMFrontMaterialProperties-fortran -lstdc++
endif

TESTS=$(check_PROGRAMS)

src/%.cxx : %.mfront
	@top_builddir@/mfront/src/mfront --interface=c $<

if HAVE_FORTRAN
src/%-fortran.cxx : %.mfront
	@top_builddir@/mfront/src/mfront --interface=fortran $<
endif

if HAVE_CASTEM
src/%-castem.cxx : %.mfront
	@top_builddir@/mfront/src/mfront --interface=castem  $<
endif

clean-local:
	-rm -fr src include

endif

EXTRA_DIST = $(mfronts)

