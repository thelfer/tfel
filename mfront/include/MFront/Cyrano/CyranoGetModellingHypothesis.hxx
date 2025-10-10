/*!
 * \file  mfront/include/MFront/Cyrano/CyranoGetModellingHypothesis.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANO_CYRANOGETMODELLINGHYPOTHESIS_HXX
#define LIB_MFRONT_CYRANO_CYRANOGETMODELLINGHYPOTHESIS_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Cyrano/CyranoConfig.hxx"
#include "MFront/Cyrano/Cyrano.hxx"

namespace cyrano {

  /*!
   * \return the modelling hypothesis contained in the NDI parameter
   * used by the Cyrano Interface of the Cast3M finite element solver
   *
   * \param[in] ndi : integer used by the Cast3M finite element solver
   * to represent the modelling hypothesis
   */
  MFRONT_CYRANO_VISIBILITY_EXPORT
  tfel::material::ModellingHypothesis::Hypothesis getModellingHypothesis(
      const CyranoInt);

  /*!
   * \return the identifier associted with the given modelling hypothesis
   *
   * \param[in] h : modelling hypothesis
   */
  MFRONT_CYRANO_VISIBILITY_EXPORT CyranoInt getModellingHypothesisIdentifier(
      const tfel::material::ModellingHypothesis::Hypothesis&);

}  // end of namespace cyrano

#endif /* LIB_MFRONT_CYRANO_CYRANOGETMODELLINGHYPOTHESIS_HXX */
