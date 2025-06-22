/*!
 * \file   PipeProfile.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   14 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MTest/CurrentState.hxx"
#include "MTest/PipeProfile.hxx"

namespace mtest {

  PipeProfile::~PipeProfile() = default;

  PipeStressProfile::PipeStressProfile() : all(true), c(0u) {}

  PipeStressProfile::PipeStressProfile(const unsigned short cn)
      : all(false), c(cn) {}  // end of PipeStressProfile::PipeStressProfile

  void PipeStressProfile::report(std::ostream& os,
                                 const CurrentState& s) const {
    if (all) {
      os << " " << s.s1[0] << " " << s.s1[1] << " " << s.s1[2];
    } else {
      os << " " << s.s1[this->c];
    }
  }

  PipeStressProfile::~PipeStressProfile() = default;

  PipeStrainProfile::PipeStrainProfile() : all(true), c(0u) {}

  PipeStrainProfile::PipeStrainProfile(const unsigned short cn)
      : all(false), c(cn) {}  // end of PipeStrainProfile::PipeStrainProfile

  void PipeStrainProfile::report(std::ostream& os,
                                 const CurrentState& s) const {
    if (all) {
      os << " " << s.e1[0] << " " << s.e1[1] << " " << s.e1[2];
    } else {
      os << " " << s.e1[this->c];
    }
  }

  PipeStrainProfile::~PipeStrainProfile() = default;

  PipeInternalStateVariableProfile::PipeInternalStateVariableProfile()
      : all(true), c(0u) {}

  PipeInternalStateVariableProfile::PipeInternalStateVariableProfile(
      const unsigned short cn)
      : all(false),
        c(cn) {
  }  // end of
     // PipeInternalStateVariableProfile::PipeInternalStateVariableProfile

  void PipeInternalStateVariableProfile::report(std::ostream& os,
                                                const CurrentState& s) const {
    if (all) {
      for (const auto v : s.iv1) {
        os << " " << v;
      }
    } else {
      os << " " << s.iv1[this->c];
    }
  }

  PipeInternalStateVariableProfile::~PipeInternalStateVariableProfile() =
      default;

}  // namespace mtest
