/*!
 * \file   mfm-test-generator/src/TestCaseBase.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   25/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFMTestGenerator/TestCaseBase.hxx"

namespace mfmtg {

  TestCaseBase::TestCaseBase(const TestCaseParameters& p)
      : name(get<std::string>(p, "name")) {
  }  // end of TestCaseBase::TestCaseBase

  void TestCaseBase::addInputFileGenerator(const generator& g) {
    this->generators.push_back(g);
  }  // end of TestCaseBase::addGenerator

  void TestCaseBase::generate() const {
    for (const auto& g : this->generators) {
      g(*this);
    }
  }  // end of TestCaseBase::addGenerator

  TestCaseBase::~TestCaseBase() = default;

}  // end of namespace mfmtg
