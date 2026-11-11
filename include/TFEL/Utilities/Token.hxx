/*!
 * \file   include/TFEL/Utilities/Token.hxx
 * \brief  This file declares the Token class
 * \author Thomas Helfer
 * \date 20/11/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_TOKEN_HXX
#define LIB_TFEL_UTILITIES_TOKEN_HXX

#include <string>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::utilities {

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
      Number,
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
    //! \brief copy constructor
    Token(const Token&);
    //! \brief move constructor
    Token(Token&&) noexcept;
    //! \brief assignement
    Token& operator=(const Token&);
    //! \brief move assignement
    Token& operator=(Token&&) noexcept;
    //! \brief destructor
    ~Token() noexcept;
    /*!
     * \brief string holded by the token
     *
     * The value of a comment, that is of a token whose flag is Comment,
     * DoxygenComment or DoxygenBackwardComment, is the contents of the comment:
     * the delimiters of the comment and the spaces which surround its contents
     * are not a part of the value. An empty comment, as `//` or a `C` style
     * comment without any contents, thus gives a token with an empty value.
     */
    std::string value;
    //! \brief line number
    size_type line = 0u;
    /*!
     * \brief offset in the line
     *
     * The offset of a token is the position, in the line the token starts on,
     * of the first character the token stands for. The offset of a token whose
     * value is empty, which is only possible for a comment and for a raw string
     * literal, is that of the delimiter which opens it. The offset of a comment
     * is therefore always strictly smaller than the size of the line the
     * comment starts on, see issue #1391. The raw string literals are the
     * exception: the value and the offset of such a token are those of its
     * contents, not those of the literal itself, which is the defect reported
     * in issue #1390.
     *
     * A token may span several lines, which is the case of a comment and of a
     * raw string literal: the line and the offset of such a token are those of
     * its first line, whereas its value gathers the lines it spans. The offset
     * of a comment without contents on the line it starts on, as the offset of
     * an empty comment, is that of the delimiter which opens it.
     */
    size_type offset = 0u;
    //! \brief comment
    std::string comment;
    //! \brief type of the token
    TokenFlag flag = Standard;
  };  // end of struct Token

  /*!
   * \return true if the argument is a comment
   * \param[in] t: token
   */
  TFELUTILITIES_VISIBILITY_EXPORT bool isComment(const Token&);

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_TOKEN_HXX */
