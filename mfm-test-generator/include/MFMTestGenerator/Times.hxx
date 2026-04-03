/*!
 * \file   Times.hxx
 * \brief
 * \author Thomas Helfer
 * \date   11/10/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_TIMES_HXX
#define LIB_MFM_TEST_GENERATOR_TIMES_HXX

#include <map>
#include <vector>
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mfmtg {

  /*!
   * \brief a simple structure used to describe an evolution defined in an
   * external file.
   */
  struct TimesFromFile {
    //! \brief a simple alias
    using Values = tfel::utilities::GenType<unsigned int,  // column number
                                            std::string    // formula
                                            >;
    //! \brief name of the file
    std::string file;
    //! \brief description of times in file
    Values times;
  };  // end of struct TimesFromFile

  //! \brief types used to describe a set of times
  using TimesTypes =
      tfel::meta::GenerateTypeList<std::vector<double>, TimesFromFile>::type;

  //! \brief various ways of storing an evolution
  struct Times : tfel::utilities::GenTypeBase<TimesTypes> {
    //! a simple alias
    using GenTypBase = tfel::utilities::GenTypeBase<TimesTypes>;
    //! inheriting constructors
    using GenTypBase::GenTypBase;
    //! inheriting assignement operator
    using GenTypBase::operator=;
  };  // end of struct Times

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_TIMES_HXX */
