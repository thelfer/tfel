/*!
 * \file  mfront/src/CastemGetModellingHypothesis.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 17 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>

#include "TFEL/Raise.hxx"
#include "MFront/Castem/CastemException.hxx"
#include "MFront/Castem/CastemGetModellingHypothesis.hxx"

namespace castem {

  tfel::material::ModellingHypothesis::Hypothesis getModellingHypothesis(
      const CastemInt ndi) {
    using namespace tfel::material;
    switch (ndi) {
      case 2:
        return ModellingHypothesis::TRIDIMENSIONAL;
      case 0:
        return ModellingHypothesis::AXISYMMETRICAL;
      case -1:
        return ModellingHypothesis::PLANESTRAIN;
      case -2:
        return ModellingHypothesis::PLANESTRESS;
      case -3:
        return ModellingHypothesis::GENERALISEDPLANESTRAIN;
      case 14:
        return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
        // C                       =  3 a 11 : unidimensionnel plan (massif 1D)
        // C                       = 12 a 14 : unidimensionnel axisymetrique
        // (massif 1D) C                       = 15 : unidimensionnel spherique
        // (massif 1D)
    }
    std::ostringstream msg;
    msg << "castem::getModellingHypothesis : "
        << "invalid argument ('" << ndi << "')";
    tfel::raise<CastemException>(msg.str());
  }

}  // end of namespace castem
