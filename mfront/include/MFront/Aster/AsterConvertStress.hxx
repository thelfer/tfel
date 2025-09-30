/*!
 * \file   AsterConvertStress.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06 févr. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_ASTER_ASTERCONVERTSTRESS_HXX
#define LIB_ASTER_ASTERCONVERTSTRESS_HXX

namespace aster {

  /*!
   * \brief structure in charge of converting stress
   * between finite strain formulation
   * \tparam btype: behaviour type
   * \tparam afsf:  finite strain formulation
   * \tparam N:     space dimension
   */
  template <AsterBehaviourType btype,
            AsterFiniteStrainFormulation afsf,
            unsigned short N>
  struct AsterConvertStress {
    /*!
     * \brief : do nothing
     */
    static TFEL_ASTER_INLINE void exe(const AsterReal* const,
                                      const AsterReal* const) {}  // end of exe
  };  // end of struct AsterConvertStress

  /*!
   * \brief structure in charge of converting stress
   * between finite strain formulation
   * \tparam btype: behaviour type
   * \tparam afsf:  finite strain formulation
   * \tparam N:     space dimension
   */
  template <unsigned short N>
  struct AsterConvertStress<aster::STANDARDFINITESTRAINBEHAVIOUR,
                            aster::GROT_GDEP,
                            N> {
    /*!
     * \brief : convert the Cauchy stress to the second
     * Piola-Kirchhoff stress.
     * \param[in,out] STRESS: stresss
     * \param[in]     F1:     deformation gradient at the end of the time step
     */
    static TFEL_ASTER_INLINE void exe(AsterReal* const STRESS,
                                      const AsterReal* const F1) {
      using namespace tfel::math;
      stensor<N, AsterReal> s;
      s.importTab(STRESS);
      const auto F = tensor<N, AsterReal>::buildFromFortranMatrix(F1);
      const auto pk2 = convertCauchyStressToSecondPiolaKirchhoffStress(s, F);
      pk2.exportTab(STRESS);
    }  // end of exe
  };   // end of struct AsterConvertStress

}  // end of namespace aster

#endif /* LIB_ASTER_ASTERCONVERTSTRESS_HXX */
