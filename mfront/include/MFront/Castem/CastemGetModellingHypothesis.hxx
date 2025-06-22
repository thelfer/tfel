/*!
 * \file  mfront/include/MFront/Castem/CastemGetModellingHypothesis.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 16 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMGETMODELLINGHYPOTHESIS_HXX
#define LIB_MFRONT_CASTEM_CASTEMGETMODELLINGHYPOTHESIS_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/Castem.hxx"

namespace castem {

  /*!
   * \return the modelling hypothesis contained in the NDI parameter
   * used by the Castem Interface of the Cast3M finite element solver
   *
   * \param[in] ndi : integer used by the Cast3M finite element solver
   * to represent the modelling hypothesis
   */
  MFRONT_CASTEM_VISIBILITY_EXPORT
  tfel::material::ModellingHypothesis::Hypothesis getModellingHypothesis(
      const CastemInt);

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEM_CASTEMGETMODELLINGHYPOTHESIS_HXX */
