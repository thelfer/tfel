/*!
 * \file   mfm-test-generator/src/AbstractTestCaseFactory.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFMTestGenerator/AbstractTestCaseFactory.hxx"

namespace mfmtg {

  AbstractTestCaseFactory& AbstractTestCaseFactory::get() {
    static AbstractTestCaseFactory f;
    return f;
  }  // end of AbstractTestCaseFactory::get

  AbstractTestCaseFactory::AbstractTestCaseFactory() = default;

  void AbstractTestCaseFactory::add(const std::string& n, const generator g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "AbstractTestCaseFactory::register: "
          "a test case named '" +
          n + "' has already been registred");
    }
  }  // end of AbstractTestCaseFactory::register

  std::shared_ptr<AbstractTestCase> AbstractTestCaseFactory::generate(
      const std::string& n) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "AbstractTestCaseFactory::generate: "
          "no test case named '" +
          n + "' registred");
    }
    const auto& g = p->second;
    return g();
  }  // end of AbstractTestCaseFactory::generate

  AbstractTestCaseFactory::~AbstractTestCaseFactory() = default;

}  // end of namespace mfmtg