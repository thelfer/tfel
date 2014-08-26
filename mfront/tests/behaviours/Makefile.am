SUBDIRS = aster castem

MFRONTS= DDIF2.mfront                                            \
         Elasticity.mfront                                       \
         FiniteRotationSmallStrainElasticity.mfront              \
	 Plasticity.mfront                                       \
	 DruckerPrager.mfront                                    \
         Norton.mfront                                           \
         ImplicitNorton.mfront                                   \
	 ImplicitNortonPlaneStress.mfront                        \
	 ImplicitNortonGeneralisedPlaneStress.mfront             \
         ImplicitFiniteStrainNorton.mfront                       \
         ImplicitNorton2.mfront                                  \
         ImplicitNorton_Broyden.mfront                           \
         ImplicitNorton_Broyden2.mfront                          \
	 EllipticCreep.mfront                                    \
         NortonRK.mfront                                         \
         NortonRK2.mfront                                        \
         NortonRK3.mfront                                        \
         NortonRK4.mfront                                        \
         Norton_Euler.mfront                                     \
         Norton_rk2.mfront                                       \
         Norton_rk4.mfront                                       \
         Norton_rk42.mfront                                      \
         Norton_rkCastem.mfront                                  \
         Burger.mfront                                           \
         OrthotropicCreep.mfront                                 \
         OrthotropicCreep_rk42.mfront                            \
         OrthotropicCreep_rkCastem.mfront                        \
         ImplicitOrthotropicCreep.mfront                         \
         OrthotropicElastic.mfront                               \
         StrainHardeningCreep.mfront                             \
         Mazars.mfront                                           \
         Lorentz.mfront                                          \
	 PlasIsoCin.mfront                                       \
	 ViscoPlasIsoCin.mfront                                  \
         Chaboche.mfront                                         \
         Chaboche2.mfront                                        \
	 ImplicitMonoCristal.mfront                              \
	 MonoCristal.mfront                                      \
	 MonoCristalNewtonRaphson.mfront                         \
	 MonoCristal_DD_CFC.mfront                               \
	 MonoCristal_DD_CFC_NumericalJacobian.mfront.mfront      \
	 MonoCristal_DD_CFC_InteractionMatrix.mfront             \
	 PolyCristals.mfront                                     \
	 PolyCristals_DD_CFC.mfront                              \
	 Hayhurst.mfront                                         \
	 ImplicitHayhurst.mfront                                 \
	 Tvergaard.mfront                                        \
	 SaintVenantKirchhoffElasticity.mfront                   \
	 CompressibleNeoHookeanBehaviour.mfront                  \
	 IncompressibleMooneyRivlinBehaviour.mfront              \
	 IncompressiblePlaneStressMooneyRivlinBehaviour.mfront   \
	 IncompressiblePlaneStressGornetDesmoratBehaviour.mfront \
	 ImplicitSimoMieheElastoPlasticity.mfront

testsdir = $(pkgdatadir)/mfront/tests/behaviours
tests_DATA = $(MFRONTS)

EXTRA_DIST=CMakeLists.txt                                   \
	   $(MFRONTS)                                       \
           references/plasticity.ref                        \
	   references/elasticity15-aster.ref                \
	   references/mazars-aster.ref                      \
	   references/plasisocin.ref                        \
	   references/viscoplasisocin.ref                   \
	   references/chaboche2-aster.ref                   \
	   references/chaboche-aster.ref                    \
	   references/chaboche2-aster.ref                   \
	   references/monocristal.ref                       \
	   references/monocristal_dd_cfc.ref                \
	   references/PolyCristalsAngles.txt                \
	   references/polycristals.ref                      \
	   references/polycristals_dd_cfc.ref               \
	   references/hayhurst.ref                          \
	   references/burger.ref                            \
	   references/tvergaard.ref                         \
	   references/druckerprager.ref                     \
	   references/druckerprager2.ref                    \
	   references/implicitsimomieheelastoplasticity.ref
