/*!
 * \file   StudyParameter.hxx
 * \brief
 * \author Thomas Helfer
 * \date   23 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_STUDYPARAMETER_HXX
#define LIB_MTEST_STUDYPARAMETER_HXX

#include <string>

#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  // a simple alias
  using StudyParameter = tfel::utilities::GenType<bool, real, std::string>;

}  // end of namespace mtest

#endif /* LIB_MTEST_STUDYPARAMETER_HXX */
