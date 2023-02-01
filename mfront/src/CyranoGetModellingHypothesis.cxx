/*!
 * \file  mfront/src/CyranoGetModellingHypothesis.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/Cyrano/CyranoException.hxx"
#include "MFront/Cyrano/CyranoGetModellingHypothesis.hxx"

namespace cyrano {

  tfel::material::ModellingHypothesis::Hypothesis getModellingHypothesis(
      const CyranoInt ndi) {
    using namespace std;
    using namespace tfel::material;
    switch (ndi) {
      case 14:
        return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
        // C                       =  3 a 11 : unidimensionnel plan (massif 1D)
        // C                       = 12 a 14 : unidimensionnel axisymetrique
        // (massif 1D) C                       = 15 : unidimensionnel spherique
        // (massif 1D)
      default:;
    }
    ostringstream msg;
    msg << "cyrano::getModellingHypothesis : "
        << "invalid argument ('" << ndi << "')";
    tfel::raise<CyranoException>(msg.str());
  }

}  // end of namespace cyrano
