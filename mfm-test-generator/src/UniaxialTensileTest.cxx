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
        library(get(p, "behaviour", "library")),
        function(get(p, "behaviour", "function")),
        hypothesis("Tridimensional"),
        author(get_if(p, "author")),
        date(get_if(p, "date")),
        description(get_if(p, "description")),
        times(tfel::utilities::convert<std::vector<double>>(
            getParameter(p, "times"))),
        imposed_strain(getEvolution(p, "imposed_strain")),
        material_properties(getEvolutions(p, "material_properties")),
        external_state_variables(getEvolutions(p, "external_state_variables")) {
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
