/*!
 * \file   UniaxialTensileTest.cxx
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
#include "MFMTestGenerator/UniaxialTensileTest.hxx"

namespace mfmtg {

  UniaxialTensileTest::UniaxialTensileTest(const TestCaseParameters& p)
      : TestCaseBase(p),
        BehaviourData(p, "Tridimensional"),
        imposed_strain(getEvolution(p, "imposed_strain")) {
    message(
        "UniaxialTensileTest::UniaxialTensileTest: "
        "pre-processing of test '" +
        this->name + "' begins\n");

    message(
        "UniaxialTensileTest::UniaxialTensileTest: "
        "pre-processing of test '" +
        this->name + "' is finished\n");
  } // end of UniaxialTensileTest::UniaxialTensileTest

  UniaxialTensileTest::~UniaxialTensileTest() = default;

}  // end of namespace mfmtg
