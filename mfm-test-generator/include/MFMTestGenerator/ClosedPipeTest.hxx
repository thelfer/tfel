/*!
 * \file   ClosedPipeTest.hxx
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

#ifndef LIB_MFM_TEST_GENERATOR_CLOSEDPIPETEST_HXX
#define LIB_MFM_TEST_GENERATOR_CLOSEDPIPETEST_HXX

#include <map>
#include <vector>
#include <string>
#include "MFMTestGenerator/Config.hxx"
#include "MFMTestGenerator/Forward.hxx"
#include "MFMTestGenerator/TestCaseBase.hxx"
#include "MFMTestGenerator/BehaviourData.hxx"

namespace mfmtg {

  //! \brief a test on a unit cube
  struct MFMTG_VISIBILITY_EXPORT ClosedPipeTest : TestCaseBase,
                                                       BehaviourData {
    //! \brief default constructor
    ClosedPipeTest(const TestCaseParameters&);
    //! \brief inner radius
    const double inner_radius;
    //! \brief outer radius
    const double outer_radius;
    //! \brief number of elements
    const int number_of_elements;
    //! \brief imposed pressure
    const std::map<double, double> imposed_pressure;
    //! \brief destructor
    ~ClosedPipeTest() override;
  };  // end of struct ClosedPipeTest

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_CLOSEDPIPETEST_HXX */
