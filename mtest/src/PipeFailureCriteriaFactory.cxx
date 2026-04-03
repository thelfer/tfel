/*!
 * \file   mtest/src/PipeFailureCriteriaFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12/09/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/PipeFailureCriterion.hxx"
#include "MTest/PipeFailureCriteriaFactory.hxx"

namespace mtest {

  PipeFailureCriteriaFactory&
  PipeFailureCriteriaFactory::getFactory() noexcept {
    static PipeFailureCriteriaFactory f;
    return f;
  }  // end of getFactory

  PipeFailureCriteriaFactory::PipeFailureCriteriaFactory() noexcept {
  }  // end of PipeFailureCriteriaFactory

  void PipeFailureCriteriaFactory::addGenerator(const std::string& n,
                                                const Generator& g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "PipeFailureCriteriaFactory::addGenerator: "
          "a generator named '" +
          n + "' has already been declared");
    }
  }  // end of addGenerator

  std::unique_ptr<PipeFailureCriterion> PipeFailureCriteriaFactory::generate(
      const std::string& n, const ParametersMap& d) const {
    const auto pg = this->generators.find(n);
    if (pg == this->generators.end()) {
      tfel::raise(
          "PipeFailureCriteriaFactory::generate: "
          "no generator named '" +
          std::string{n} + "' declared");
    }
    return (pg->second)(n, d);
  }  // end of addGenerator

  PipeFailureCriteriaFactory::~PipeFailureCriteriaFactory() = default;

}  // end of namespace mtest
