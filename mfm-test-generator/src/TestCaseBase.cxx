/*!
 * \file   mfm-test-generator/src/TestCaseBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/TestCaseBase.hxx"

namespace mfmtg {

  static Times readTimes(const TestCaseParameter& p) {
    if (p.is<TestCaseParameters>()) {
      auto times = TimesFromFile{};
      const auto& d = p.get<TestCaseParameters>();
      times.file = get(d, "file");
      const auto& c = getParameter(d, "values");
      if (c.is<int>()) {
        const auto cn = c.get<int>();
        if (cn <= 0) {
          tfel::raise("readTimes: invalid column number '" +
                      std::to_string(cn) + "'");
        }
        times.times = static_cast<unsigned int>(cn);
      } else {
        times.times = c.get<std::string>();
      }
      return times;
    }
    return tfel::utilities::convert<std::vector<double>>(p);
  }  // end of readTimes

  TestCaseBase::TestCaseBase(const TestCaseParameters& p)
      : name(get<std::string>(p, "name")),
        author(get_if(p, "author", "")),
        date(get_if(p, "date", "")),
        description(get_if(p, "description", "")),
        times(readTimes(getParameter(p, "times"))) {
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
