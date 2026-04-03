/*!
 * \file   AbstractTestCase.hxx
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

#ifndef LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASE_HXX
#define LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASE_HXX

#include <vector>
#include <string>
#include <functional>
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  //! \brief an abstract interface to describe a test case.
  struct MFMTG_VISIBILITY_EXPORT AbstractTestCase {
    //! a simple alias
    using generator = std::function<void(const AbstractTestCase&)>;
    //! \brief add a generator
    virtual void addInputFileGenerator(const generator&) = 0;
    /*!
     * \brief generate the test case for the given interfaces
     * \param[in] p: parameters
     */
    virtual void generate() const = 0;
    //! destructor
    virtual ~AbstractTestCase();
  };  // end of struct AbstractTestCase

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASE_HXX */
