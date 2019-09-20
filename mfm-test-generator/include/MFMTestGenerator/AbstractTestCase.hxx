/*!
 * \file   AbstractTestCase.hxx
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

#ifndef LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASE_HXX
#define LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASE_HXX

#include <vector>
#include <string>
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  // forward declaration
  struct TestCaseParameters;

  /*!
   * \brief a base class to describe a test case.
   */
  struct MFMTG_VISIBILITY_EXPORT AbstractTestCase {
    /*!
     * \brief generate the test case for the given interfaces
     * \param[in] p: parameters
     * \param[in] i: list of interfaces
     */
    virtual void generate(const TestCaseParameters&,
                          const std::vector<std::string>&) const = 0;
    //! destructor
    virtual ~AbstractTestCase();
  }; // end of struct AbstractTestCase 

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASE_HXX */
