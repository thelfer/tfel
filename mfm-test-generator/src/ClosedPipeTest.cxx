/*!
 * \file   ClosedPipeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFMTestGenerator/Log.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/ClosedPipeTest.hxx"

namespace mfmtg {

  template <typename T>
  static std::string convert(const T& v) {
    std::ostringstream s;
    s.precision(14);
    s << v;
    return s.str();
  }  // end of convert

  static ClosedPipeTest::ElementType getElementType(const std::string& e) {
    if (e == "Linear") {
      return ClosedPipeTest::LINEAR;
    } else if (e != "Quadratic") {
      tfel::raise("ClosedPipeTest::ClosedPipeTest: invalid element type '" + e +
                  "'. Expected 'Linear' or 'Quadratic'");
    }
    return ClosedPipeTest::QUADRATIC;
  }  // end of getElementType

  ClosedPipeTest::ClosedPipeTest(const TestCaseParameters& p)
      : TestCaseBase(p),
        BehaviourData(getTestCaseParameters(p, "behaviour"),
                      "AxisymmetricalGeneralisedPlaneStrain"),
        inner_radius(get<double>(p, "inner_radius")),
        outer_radius(get<double>(p, "outer_radius")),
        number_of_elements(get<int>(p, "number_of_elements")),
        element_type(getElementType(get_if(p, "element_type", "Linear"))),
        outer_pressure(getEvolution(p, "outer_pressure")),
        inner_pressure(getEvolution(p, "inner_pressure")) {
    message(
        "ClosedPipeTest::ClosedPipeTest: "
        "pre-processing of test '" +
        this->name + "' begins");
    debug("inner_radius: " + convert(this->inner_radius));
    debug("outer_radius: " + convert(this->outer_radius));
    debug("number of elements: " + convert(this->number_of_elements));
    message(
        "ClosedPipeTest::ClosedPipeTest: "
        "pre-processing of test '" +
        this->name + "' is finished");
  }  // end of ClosedPipeTest::ClosedPipeTest

  ClosedPipeTest::~ClosedPipeTest() = default;

}  // end of namespace mfmtg
