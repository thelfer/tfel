/*!
 * \file  mfront/src/CyranoGetModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>

#include "MFront/Cyrano/CyranoException.hxx"
#include "MFront/Cyrano/CyranoGetModellingHypothesis.hxx"

namespace cyrano {

  tfel::material::ModellingHypothesis::Hypothesis getModellingHypothesis(
      const CyranoInt ndi) {
    using namespace std;
    using namespace tfel::material;
    switch (ndi) {
      case 1:
        return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
      case 2:
        return ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
      default:;
    }
    ostringstream msg;
    msg << "cyrano::getModellingHypothesis : "
        << "invalid argument ('" << ndi << "')";
    throw(CyranoException(msg.str()));
  }

}  // end of namespace cyrano
