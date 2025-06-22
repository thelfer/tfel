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

#ifndef LIB_TFEL_CXXKEYWORDS_HXX
#define LIB_TFEL_CXXKEYWORDS_HXX

#include <string>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \return the number of c++ keywords
     */
    TFELUTILITIES_VISIBILITY_EXPORT
    unsigned short getNumberOfCxxKeywords();

    /*!
     * \return true if the specified string is a of c++ keyword
     * \param[in] s : string to be tested
     */
    TFELUTILITIES_VISIBILITY_EXPORT
    bool isReservedCxxKeywords(const std::string&);

  }  // namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_CXXKEYWORDS_HXX */
