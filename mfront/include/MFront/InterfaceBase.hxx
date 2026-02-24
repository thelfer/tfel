/*!
 * \file   mfront/include/MFront/InterfaceBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   07 nov 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_INTERFACEBASE_HXX
#define LIB_MFRONT_INTERFACEBASE_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

namespace mfront {

  /*!
   * \brief a helper class which provides useful methods to build interfaces.
   */
  struct MFRONT_VISIBILITY_EXPORT InterfaceBase {
   protected:
    //! \brief a simple alias
    using TokensContainer = tfel::utilities::CxxTokenizer::TokensContainer;
    /*!
     * \brief throw a std::runtime_error
     * \param[in] c: iterator to the current token
     * \param[in] e: iterator past the last token
     * \param[in] m: method
     * \param[in] msg: error message
     */
    [[noreturn]] static void throwRuntimeError(
        TokensContainer::const_iterator&,
        const TokensContainer::const_iterator,
        const std::string&,
        const std::string&);
    //
    static void checkNotEndOfFile(TokensContainer::const_iterator&,
                                  const TokensContainer::const_iterator,
                                  const std::string&,
                                  const std::string& = "");

    static void readSpecifiedToken(TokensContainer::const_iterator&,
                                   const TokensContainer::const_iterator,
                                   const std::string&,
                                   const std::string&);

    static std::vector<std::string> readArrayOfString(
        TokensContainer::const_iterator&,
        const TokensContainer::const_iterator,
        const std::string&);

    static std::vector<std::string> readStringOrArrayOfString(
        TokensContainer::const_iterator&,
        const TokensContainer::const_iterator,
        const std::string&);

  };  // end of struct InterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_INTERFACEBASE_HXX */
