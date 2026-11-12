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
     * The value of a token is the spelling of the token in the file, that is
     * the substring of the line taken at the offset of the token, excepted for
     * comments and for numbers:
     *
     * - the value of a comment, that is of a token whose flag is Comment,
     *   DoxygenComment or DoxygenBackwardComment, is the contents of the
     *   comment: the delimiters of the comment and the spaces which surround
     *   its contents are not a part of the value. An empty comment, as `//` or
     *   a `C` style comment without any contents, thus gives a token with an
     *   empty value,
     *
     * - the value of a string token holds the whole literal: the double quotes
     *   of a quoted string as well as the `R"delim(` prefix and the `)delim"`
     *   suffix of a raw string literal are a part of the value, so that the
     *   contents of a string token can be read back without relying on the
     *   flag of the token only, see issue #1390,
     *
     * - the value of a number does not hold the digit separators of the
     *   `C++14` integer literals, see the description of the CxxTokenizer
     *   class.
     */
    std::string value;
    //! \brief line number
    size_type line = 0u;
    /*!
     * \brief offset in the line
     *
     * The offset of a token is the position, in the line the token starts on,
     * of the first character the token stands for, so that it is always
     * strictly smaller than the size of that line. This is the case of a raw
     * string token, whose offset is that of its `R` delimiter as the offset of
     * a quoted string token is that of its opening quote, see issue #1390. The
     * offset of a token whose value is empty, which is only possible for a
     * comment, is that of the delimiter which opens it, see issue #1391.
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

  /*!
   * \return true if the token holds a raw string literal, that is a literal
   * of the `R"delim(contents)delim"` form, whatever the contents of the token.
   * \param[in] t: token
   */
  TFELUTILITIES_VISIBILITY_EXPORT bool isRawString(const Token&);

  /*!
   * \return the delimiter of the raw string literal held by the given token,
   * that is the sequence of characters between the `R"` and the `(` of its
   * value, which is empty for a literal of the `R"(contents)"` form. An empty
   * string is also returned if the token does not hold a raw string literal.
   * \param[in] t: token
   */
  TFELUTILITIES_VISIBILITY_EXPORT std::string getRawStringDelimiter(
      const Token&);

  /*!
   * \return the contents of the literal held by the given token: its value
   * without the double quotes of a quoted string, without the single quotes of
   * a character literal and without the `R"delim(` prefix and the `)delim"`
   * suffix of a raw string literal.
   *
   * The consumers which strip the quotes of a string token shall use this
   * method instead of removing the first and last characters of the value,
   * which truncates a raw string literal, see issue #1390.
   * \param[in] t: token
   */
  TFELUTILITIES_VISIBILITY_EXPORT std::string getContents(const Token&);

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_TOKEN_HXX */
