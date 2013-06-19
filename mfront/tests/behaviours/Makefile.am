SUBDIRS = aster castem

MFRONTS= DDIF2.mfront                                \
         Elasticity.mfront                           \
         Plasticity.mfront                           \
         Norton.mfront                               \
         ImplicitNorton.mfront                       \
         ImplicitNorton2.mfront                      \
         ImplicitNorton_Broyden.mfront               \
         ImplicitNorton_Broyden2.mfront              \
         NortonRK.mfront                             \
         NortonRK2.mfront                            \
         NortonRK3.mfront                            \
         NortonRK4.mfront                            \
         Norton_Euler.mfront                         \
         Norton_rk2.mfront                           \
         Norton_rk4.mfront                           \
         Norton_rk42.mfront                          \
         Norton_rkCastem.mfront                      \
         Burger.mfront                               \
         OrthotropicCreep.mfront                     \
         OrthotropicCreep_rk42.mfront                \
         OrthotropicCreep_rkCastem.mfront            \
         ImplicitOrthotropicCreep.mfront             \
         OrthotropicElastic.mfront                   \
         StrainHardeningCreep.mfront                 \
         Mazars.mfront                               \
         Lorentz.mfront                              \
         Chaboche.mfront                             \
         Chaboche2.mfront                            \
	 MonoCristal.mfront                          \
	 MonoCristalNewtonRaphson.mfront             \
	 MonoCristal_DD_CFC.mfront                   \
	 MonoCristal_DD_CFC_InteractionMatrix.mfront \
	 PolyCristals.mfront                         \
	 Hayhurst.mfront

testsdir = $(pkgdatadir)/mfront/tests/behaviours
tests_DATA = $(MFRONTS)

EXTRA_DIST=CMakeLists.txt                    \
	   $(MFRONTS)                        \
           references/plasticity.ref         \
	   references/elasticity15-aster.ref \
	   references/mazars-aster.ref       \
	   references/chaboche-aster.ref     \
	   references/chaboche2-aster.ref    \
	   references/monocristal.ref        \
	   references/monocristal_dd_cfc.ref \
	   references/PolyCristalsAngles.txt \
	   references/polycristals.ref       \
	   references/hayhurst.ref           \
	   references/burger.ref