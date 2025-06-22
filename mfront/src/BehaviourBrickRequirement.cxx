/*!
 * \file   BehaviourBrickRequirement.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   26 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include <stdexcept>
#include "MFront/BehaviourBrick/Requirement.hxx"

namespace mfront {

  namespace bbrick {

    Requirement::Requirement(Requirement&&) = default;
    Requirement::Requirement(const Requirement&) = default;

    Requirement::Requirement(const std::string& t,
                             const std::string& n,
                             const unsigned short s,
                             const std::vector<ProviderIdentifier>& a)
        : type(t), name(n), asize(s), aproviders(a) {
      if (this->aproviders.empty()) {
        throw(
            std::runtime_error("Requirement::Requirement : "
                               "empty list of providers specified for "
                               "requirement '" +
                               this->name + "'"));
      }
    }  // end of Requirement::Requirement

    Requirement::~Requirement() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
