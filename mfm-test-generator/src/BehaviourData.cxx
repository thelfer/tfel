/*!
 * \file   BehaviourData.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   07/10/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/BehaviourData.hxx"

namespace mfmtg {

  BehaviourData::BehaviourData(const TestCaseParameters& p,
                               const std::string& h)
      : library(get(p, "behaviour", "library")),
        function(get(p, "behaviour", "function")),
        hypothesis(h),
        material_properties(getEvolutions(p, "material_properties")),
        external_state_variables(getEvolutions(p, "external_state_variables")) {
  }  // end of

  BehaviourData::BehaviourData(BehaviourData&&) = default;

  BehaviourData::BehaviourData(const BehaviourData&) = default;

  BehaviourData::~BehaviourData() = default;

}  // end of namespace mfmtg
