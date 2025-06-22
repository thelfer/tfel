/*!
 * \file   Forward.hxx
 * \brief
 * \date   07/10/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_FORWARD_HXX
#define LIB_MFM_TEST_GENERATOR_FORWARD_HXX

#include <map>
#include <string>

namespace tfel {

  namespace utilities {

    // forward declaration
    struct Data;

  }  // end of namespace utilities

}  // end of namespace tfel

namespace mfmtg {

  //! \brief a simple alias
  using TestCaseParameter = tfel::utilities::Data;
  //! \brief a structure holding all the parameters of a test case.
  using TestCaseParameters = std::map<std::string, TestCaseParameter>;

  // forward declaration
  struct Evolution;

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_FORWARD_HXX */
