/*!
 * \file   ClosedPipeTest.cxx
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

#include <ostream>
#include "MFMTestGenerator/Log.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/ClosedPipeTest.hxx"

namespace mfmtg {

  ClosedPipeTest::ClosedPipeTest(const TestCaseParameters& p)
      : TestCaseBase(p),
        BehaviourData(p, "AxisymmetricalGeneralisedPlaneStrain"),
        inner_radius(get<double>(p, "inner_radius")),
        outer_radius(get<double>(p, "outer_radius")),
        number_of_elements(get<int>(p, "number_of_elements")),
        imposed_pressure(getEvolution(p, "imposed_pressure")) {
    message(
        "ClosedPipeTest::ClosedPipeTest: "
        "pre-processing of test '" +
        this->name + "' begins\n");

    message(
        "ClosedPipeTest::ClosedPipeTest: "
        "pre-processing of test '" +
        this->name + "' is finished\n");
  } // end of ClosedPipeTest::ClosedPipeTest

  ClosedPipeTest::~ClosedPipeTest() = default;

}  // end of namespace mfmtg
