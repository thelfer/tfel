/*!
 * \file   include/TFEL/Utilities/CxxKeywords.hxx
 * \brief
 * \author Thomas Helfer
 * \date   16 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_CXXKEYWORDS_HXX
#define LIB_TFEL_UTILITIES_CXXKEYWORDS_HXX

#include <cstddef>
#include <string_view>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::utilities {

  //! \return the number of c++ keywords
  TFELUTILITIES_VISIBILITY_EXPORT
  std::size_t getNumberOfCxxKeywords();

  /*!
   * \return true if the specified string is a of c++ keyword
   * \param[in] s : string to be tested
   */
  TFELUTILITIES_VISIBILITY_EXPORT
  bool isReservedCxxKeywords(std::string_view);

}  // namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_CXXKEYWORDS_HXX */
