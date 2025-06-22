/*!
 * \file   include/TFEL/Utilities/Token.hxx
 * \brief  This file declares the Token class
 * \author Helfer Thomas
 * \date   20 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_TOKEN_H_
#define LIB_TFEL_UTILITIES_TOKEN_H_

#include <string>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \brief class representing a token in a C++-like file
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT Token {
      //! a simple alias
      using size_type = size_t;
      //! possible type of the token
      enum TokenFlag {
        Standard,
        Comment,
        DoxygenComment,
        DoxygenBackwardComment,
        String,
        Char,
        Preprocessor
      };
      //! default constructor
      Token();
      /*!
       * \brief constructor
       * \param[in] v: token value
       * \param[in] l: line number
       * \param[in] o: offset
       * \param[in] f: token flag
       */
      Token(const std::string&,
            const size_type,
            const size_type,
            const TokenFlag = Standard);
      //! copy constructor
      Token(const Token&);
      //! move constructor
      Token(Token&&);
      //! assignement
      Token& operator=(const Token&);
      //! move assignement
      Token& operator=(Token&&);
      //! destructor
      ~Token() noexcept;
      //! string holded by the token
      std::string value;
      //! line number
      size_type line = 0u;
      //! offset in the line
      size_type offset = 0u;
      //! comment
      std::string comment;
      //! type of the token
      TokenFlag flag = Standard;
    };  // end of struct Token

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_TOKEN_H_ */
