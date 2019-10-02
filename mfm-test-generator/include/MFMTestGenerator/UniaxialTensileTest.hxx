/*!
 * \file   UniaxialTensileTest.hxx
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

#ifndef LIB_MFM_TEST_GENERATOR_UNIAXIALTENSILETEST_HXX
#define LIB_MFM_TEST_GENERATOR_UNIAXIALTENSILETEST_HXX

#include <map>
#include <vector>
#include <string>
#include "MFMTestGenerator/Config.hxx"
#include "MFMTestGenerator/TestCaseBase.hxx"
#include "MFMTestGenerator/Evolution.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"

namespace mfmtg {

  //! \brief a test on a unit cube
  struct MFMTG_VISIBILITY_EXPORT UniaxialTensileTest : TestCaseBase {
    //! \brief default constructor
    UniaxialTensileTest(const TestCaseParameters&);
    //! \brief path of the library in which the behaviour is implemented
    const std::string library;
    //! \brief function which implements the behaviour
    const std::string function;
    //! \brief modelling hypothesis
    const std::string hypothesis;
    //! \brief author (may be empty)
    const std::string author;
    //! \brief date (may be empty)
    const std::string date;
    //! \brief description (may be empty)
    const std::string description;
    //! \brief times
    const std::vector<double> times;
    //! \brief imposed strain
    const std::map<double, double> imposed_strain;
    //! \brief material properties
    const std::map<std::string, Evolution> material_properties;
    //! \brief external state variables
    const std::map<std::string, Evolution> external_state_variables;
    //! \brief destructor
    ~UniaxialTensileTest() override;
  };  // end of struct UniaxialTensileTest

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_UNIAXIALTENSILETEST_HXX */
