/*!
 * \file   mfront/include/MFront/IntegrationScheme.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   29/08/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_INTEGRATIONSCHEME_HXX
#define LIB_MFRONT_INTEGRATIONSCHEME_HXX

namespace mfront {

  /*!
   * \brief Available integration schemes.
   * One of the first thing a dsl shall do is to set the
   * integration scheme it uses.
   */
  enum struct IntegrationScheme {
    IMPLICITSCHEME,             //!< value set by the `Implicit` Dsl family
    EXPLICITSCHEME,             //!< value set by the `Runge-Kutta` Dsl
    SPECIFICSCHEME,             /*!< value set by the `	IsotropicMisesCreepDSL`,
                                 * `IsotropicMisesPlasticFlowDSL`,
                                 * `IsotropicStrainHardeningMisesCreepDSL`
                                 * `MultipleIsotropicMisesFlowsDSL` dsls. */
    USERDEFINEDSCHEME,          //!< default value.
    UNDEFINEDINTEGRATIONSCHEME  //!< undefined value.
  };                            // end of enum struct IntegrationScheme

}  // end of namespace mfront

#endif /* LIB_MFRONT_INTEGRATIONSCHEME_HXX */
