/*!
 * \file   src/Utilities/CxxTokenizer.cxx
 * \brief
 * \author Thomas Helfer
 * \date 12/11/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxKeywords.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace tfel::utilities {

  // the functions of <cctype> require values representable as unsigned char
  static bool is_space(const char c) {
    return std::isspace(static_cast<unsigned char>(c)) != 0;
  }  // end of is_space

  static bool is_digit(const char c) {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
  }  // end of is_digit

  static bool is_alpha(const char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
  }  // end of is_alpha

  /*!
   * \brief search the additional separator which appears first in the line
   * \param[in] p: first character to search
   * \param[in] pw: first character which cannot be a part of the token which
   * starts at p, see `parseStandardLine`
   * \param[in] pe: end of the line
   * \param[in] separators: additional separators
   * \return the position and the value of the first separator, `pe` and an
   * empty string if the line holds none which begins before `pw`
   *
   * Only the separators which begin before `pw` are searched: the caller builds
   * the token which starts at `p`, which stops at `pw` if no separator begins
   * before it, so that a separator which begins at `pw` or after it cannot
   * change the token nor the position the tokenizer reads next. The search is
   * thus bounded by the length of the token and of the longest separator, and
   * not by the length of the line, which keeps the cost of the tokenizer linear
   * in the length of a line, see issue #1410. The characters of the line are
   * read through a view of them, as copying the rest of the line at each call,
   * as the previous implementation did, is as expensive as the search itself.
   */
  static std::pair<std::string::const_iterator, std::string> findSeparator(
      const std::string::const_iterator p,
      const std::string::const_iterator pw,
      const std::string::const_iterator pe,
      const std::vector<std::string>& separators) {
    std::pair<std::string::const_iterator, std::string> r = {pe, ""};
    if (separators.empty()) {
      // the search can only fail: it is the case of a tokenizer which uses the
      // default options, which is that of most of the consumers of the library
      return r;
    }
    if (p == pe) {
      // the rest of the line is empty, the search can only fail. This test
      // keeps the following view of the line well defined
      return r;
    }
    auto n = std::size_t{0u};
    for (const auto& s : separators) {
      n = std::max(n, s.size());
    }
    // a separator which begins before `pw` is entirely held by the range which
    // ends at `pw` plus its own length, whatever the separator is
    const auto e = std::min(pe, pw + n);
    const auto w = std::string_view{&*p, static_cast<std::size_t>(e - p)};
    for (const auto& s : separators) {
      const auto pos = w.find(s);
      if (pos != std::string_view::npos) {
        const auto q = p + pos;
        if ((q < pw) && (q < r.first)) {
          r = {q, s};
        }
      }
    }
    return r;
  }  // end of findSeparator

  CxxTokenizer::CxxTokenizer() = default;

  CxxTokenizer::CxxTokenizer(const CxxTokenizerOptions& o)
      : CxxTokenizerOptions(o) {}  // end of CxxTokenizer::CxxTokenizer

  CxxTokenizer::CxxTokenizer(const std::string& f) : CxxTokenizer() {
    this->openFile(f);
  }  // end of CxxTokenizer::CxxTokenizer

  CxxTokenizer::CxxTokenizer(const std::string& f, const CxxTokenizerOptions& o)
      : CxxTokenizerOptions(o) {
    this->openFile(f);
  }  // end of CxxTokenizer::CxxTokenizer

  void CxxTokenizer::treatCharAsString(const bool b) {
    this->charAsString = b;
  }  // end of CxxTokenizer::treatCharAsString

  void CxxTokenizer::keepCommentBoundaries(const bool b) {
    this->bKeepCommentBoundaries = b;
  }  // end of CxxTokenizer::keepCommentBoundaries

  void CxxTokenizer::treatDotAsSeparator(const bool b) {
    this->dotAsSeparator = b;
  }  // end of CxxTokenizer::treatDotAsSeparator

  void CxxTokenizer::treatPlusAsSeparator(const bool b) {
    this->plusAsSeparator = b;
  }  // end of CxxTokenizer::treatPlusAsSeparator

  void CxxTokenizer::treatMinusAsSeparator(const bool b) {
    this->minusAsSeparator = b;
  }  // end of CxxTokenizer::treatMinusAsSeparator

  void CxxTokenizer::mergeStrings(const bool b) {
    this->shallMergeStrings = b;
  }  // end of CxxTokenizer::mergeStrings

  void CxxTokenizer::extractNumbers(const bool b) {
    this->treatNumbers = b;
  }  // end of CxxTokenizer::extractNumbers

  void CxxTokenizer::openFile(const std::string& f) {
    std::ifstream file{f};
    raise_if(!file,
             "CxxTokenizer::openFile: "
             "unable to open file '" +
                 f + "'");
    auto n = Token::size_type{};
    this->parseStream(file, n, " of file '" + f + "'");
  }

  void CxxTokenizer::addSeparator(const std::string& s) {
    if (std::find(this->additional_separators.begin(),
                  this->additional_separators.end(),
                  s) == this->additional_separators.end()) {
      this->additional_separators.push_back(s);
    }
  }  // end of CxxTokenizer::addSeparator

  void CxxTokenizer::parseString(const std::string& s) {
    std::istringstream iss(s);
    auto n = Token::size_type{};
    this->parseStream(iss, n, " of string '" + s + "'");
  }  // end of CxxTokenizer::parseOneString

  void CxxTokenizer::parseStream(std::istream& in,
                                 Token::size_type& n,
                                 const std::string& from) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::parseStream: " + m);
    };
    auto tokenize_line = [this, &n, &from](const std::string& line) {
      ++n;
      try {
        this->splitLine(line, n);
      } catch (std::runtime_error& e) {
        raise(std::string(e.what()) + ".\n" +
              "Error at line: " + std::to_string(n) + from);
      }
    };
    try {
      auto line = std::string{};
      while (std::getline(in, line)) {
        tokenize_line(line);
        // std::getline does not clear the line when the extraction fails, so
        // that the content of the last line is not kept for the test below
        line.clear();
      }
      throw_if(in.bad(), "error while parsing stream");
      // The extraction may fail although some characters have been extracted,
      // which is the case of the last line of a stream which is not terminated
      // by an end of line character: such a line still has to be tokenized. On
      // the contrary, an extraction which extracts nothing means that the end
      // of the stream is reached: such an empty line shall not be tokenized as
      // it would add a spurious end of line character at the end of the value
      // of a token which spans several lines, as an unterminated comment for
      // instance. Tokenizing a stream line by line gives then the same result
      // as tokenizing its whole contents
      if (!line.empty()) {
        tokenize_line(line);
      }
    } catch (...) {
      this->clear();
      throw;
    }
    if (this->addCurlyBraces) {
      this->tokens.insert(this->tokens.begin(), Token("{", 0, 0));
      if (!this->tokens.empty()) {
        const auto& b = this->tokens.back();
        this->tokens.insert(this->tokens.end(), Token("}", b.line + 1u, 0u));
      } else {
        this->tokens.insert(this->tokens.end(), Token("}", 0u, 0u));
      }
    }
  }  // end of parseString

  struct is_separator {
    //! constructor
    explicit is_separator(const bool bd,
                          const bool bp,
                          const bool bm,
                          const bool ba)
        : dot_as_separator(bd),
          plus_as_separator(bp),
          minus_as_separator(bm),
          grave_accent_as_separator(ba) {}  // end of is_separator
    bool operator()(const std::string::value_type& c) const {
      using ctype = std::string::value_type;
      constexpr std::array<ctype, 24> s = {
          {'?', ';', '/', '!', '&', '*', '|', '{', '}',  '[',  ']', '(',
           ')', '%', '=', '^', ',', ':', '<', '>', '\'', '\"', '\\'}};
      if (c == '.') {
        return this->dot_as_separator;
      }
      if (c == '+') {
        return this->plus_as_separator;
      }
      if (c == '-') {
        return this->minus_as_separator;
      }
      if (c == '`') {
        return this->grave_accent_as_separator;
      }
      return std::find(std::begin(s), std::end(s), c) != std::end(s);
    }

   protected:
    bool dot_as_separator = true;
    bool plus_as_separator = true;
    bool minus_as_separator = true;
    bool grave_accent_as_separator = true;
  };

  struct is_separator_or_space : public is_separator {
    explicit is_separator_or_space(const bool bd,
                                   const bool bp,
                                   const bool bm,
                                   const bool ba)
        : is_separator(bd, bp, bm, ba) {}
    bool operator()(const std::string::value_type& c) const {
      return ((is_space(c)) || (is_separator::operator()(c)));
    }
  };  // end of struct is_separator_or_space

  static void advance(Token::size_type& o,
                      std::string::const_iterator& p,
                      Token::size_type n) {
    o += n;
    std::advance(p, n);
  }

  static void advance(Token::size_type& o,
                      std::string::const_iterator& p,
                      const std::string::const_iterator np) {
    o += static_cast<Token::size_type>(np - p);
    p = np;
  }

  static void ignore_space(Token::size_type& o,
                           std::string::const_iterator& p,
                           const std::string::const_iterator pe) {
    auto np = std::find_if_not(
        p, pe, [](const std::string::value_type c) { return is_space(c); });
    advance(o, p, np);
  }

  void CxxTokenizer::parseChar(Token::size_type& o,
                               std::string::const_iterator& p,
                               const std::string::const_iterator pe,
                               const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::parseChar: " + m);
    };
    if (this->charAsString) {
      this->parseString(o, p, pe, n, '\'');
    } else {
      const auto pn = std::next(p, 1);
      throw_if(pn == pe, "unterminated char");
      const auto pn2 = std::next(p, 2);
      throw_if(pn2 == pe, "unterminated char");
      const auto pn3 = std::next(p, 3);
      if (*pn == '\\') {
        throw_if(pn3 == pe, "unterminated char");
        const auto pn4 = std::next(p, 4);
        throw_if(*pn3 != '\'',
                 "unexpected token '" + std::string(1u, *pn3) + "'");
        this->tokens.emplace_back(std::string(p, pn4), n, o, Token::Char);
        advance(o, p, pn4);
      } else {
        throw_if(*pn2 != '\'',
                 "unexpected token '" + std::string(1u, *pn2) + "'");
        this->tokens.emplace_back(std::string(p, pn3), n, o, Token::Char);
        advance(o, p, pn3);
      }
    }
  }

  void CxxTokenizer::parseString(Token::size_type& o,
                                 std::string::const_iterator& p,
                                 const std::string::const_iterator pe,
                                 const Token::size_type n,
                                 const char e) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::parseString: " + m);
    };
    auto ps = std::next(p, 1);
    auto found = false;
    for (; (ps != pe) && (!found); ++ps) {
      if (*ps == e) {
        std::string::const_reverse_iterator ps2(ps);
        const std::string::const_reverse_iterator pse(p);
        found = true;
        while ((ps2 != pse) && (*ps2 == '\\')) {
          found = !found;
          ++ps2;
        }
      }
    }
    throw_if(!found, "found no matching '" + std::string(1u, e) +
                         "' to close string\n");
    if (((!this->tokens.empty()) &&
         (this->tokens.back().flag == Token::String)) &&
        (*p == '\"') && (this->shallMergeStrings)) {
      auto& value = this->tokens.back().value;
      this->tokens.back().value =
          value.substr(0, value.size() - 1) + std::string(p + 1, ps);
    } else {
      this->tokens.emplace_back(std::string(p, ps), n, o, Token::String);
    }
    advance(o, p, ps);
  }

  void CxxTokenizer::try_join(Token::size_type& o,
                              std::string::const_iterator& p,
                              const std::string::const_iterator pe,
                              const Token::size_type n,
                              const char c) {
    if (this->joinCxxTwoCharactersSeparators) {
      const auto pn = std::next(p, 1);
      if ((pn != pe) && (*pn == c)) {
        this->tokens.emplace_back(std::string{*p, *pn}, n, o, Token::Standard);
        advance(o, p, 2u);
      } else {
        this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
        advance(o, p, 1u);
      }
    } else {
      this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
      advance(o, p, 1u);
    }
  }  // end of CxxTokenizer::try_join

  void CxxTokenizer::try_join(Token::size_type& o,
                              std::string::const_iterator& p,
                              const std::string::const_iterator pe,
                              const Token::size_type n,
                              const char c1,
                              const char c2) {
    if (this->joinCxxTwoCharactersSeparators) {
      const auto pn = std::next(p, 1);
      if ((pn != pe) && ((*pn == c1) || (*pn == c2))) {
        this->tokens.emplace_back(std::string{*p, *pn}, n, o, Token::Standard);
        advance(o, p, 2u);
      } else {
        this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
        advance(o, p, 1u);
      }
    } else {
      this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
      advance(o, p, 1u);
    }
  }  // end of CxxTokenizer::try_join

  static bool is_binary_digit(const char c) {
    return (c == '0') || (c == '1');
  }  // end of is_binary_digit

  static bool is_hexadecimal_digit(const char c) {
    return (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) ||
            ((c >= 'A') && (c <= 'F')));
  }  // end of is_hexadecimal_digit

  /*!
   * \return true if the given string is a valid suffix of an integer literal
   * \param[in] s: suffix of an integer literal
   *
   * A suffix of an integer literal is one of `u`, `U`, `l`, `L`, `ll`, `LL`, or
   * one of `u`, `U`, `l`, `L`, `ll`, `LL` followed by one of `u`, `U`, `l`,
   * `L`. The two characters of a `long long` suffix have the same case, so that
   * `1ll` and `1LL` are valid whereas `1lL` is not. The other combinations, as
   * `1ulu`, are ill formed: they are rejected and not split in several tokens
   * since a preprocessing token is the longest sequence of characters which can
   * build one, see issue #1460.
   */
  static bool is_integer_suffix(const std::string_view s) {
    auto is_unsigned_suffix_character = [](const char c) {
      return (c == 'u') || (c == 'U');
    };
    auto is_long_suffix_character = [](const char c) {
      return (c == 'l') || (c == 'L');
    };
    auto is_long_long_suffix = [&is_long_suffix_character](const char c1,
                                                           const char c2) {
      return ((c1 == c2) && (is_long_suffix_character(c1)));
    };
    switch (s.size()) {
      case 1u:
        return (is_unsigned_suffix_character(s[0])) ||
               (is_long_suffix_character(s[0]));
      case 2u:
        return ((is_unsigned_suffix_character(s[0])) &&
                (is_long_suffix_character(s[1]))) ||
               ((is_long_suffix_character(s[0])) &&
                (is_unsigned_suffix_character(s[1]))) ||
               (is_long_long_suffix(s[0], s[1]));
      case 3u:
        return ((is_unsigned_suffix_character(s[0])) &&
                (is_long_long_suffix(s[1], s[2]))) ||
               ((is_long_long_suffix(s[0], s[1])) &&
                (is_unsigned_suffix_character(s[2])));
      default:
        return false;
    }
  }  // end of is_integer_suffix

  /*!
   * \return true if the given string is the suffix of a user defined literal
   * \param[in] s: suffix of a number
   *
   * The user defined literals of the `C++11` standard are literals followed by
   * a suffix which starts with an underscore, as `1_km` or `0x1FFFu_k`. Such a
   * suffix, which can neither be that of an integer literal nor that of a
   * floating point literal, is a part of the value of the number token, see
   * issue #1460.
   */
  static bool is_user_defined_literal_suffix(const std::string_view s) {
    return (!s.empty()) && (s.front() == '_');
  }  // end of is_user_defined_literal_suffix

  /*!
   * \return the prefix which introduces the radix of the given literal
   * \param[in] v: spelling of a literal, whose sign has been removed
   *
   * The prefix of a binary literal is `0b` or `0B` and that of a hexadecimal
   * literal is `0x` or `0X`. The octal literals are not supported: a number
   * with a leading zero has no prefix and is read as a decimal literal, see
   * issue #1460.
   */
  static std::string_view get_literal_prefix(const std::string_view v) {
    if ((v.size() < 3u) || (v[0] != '0')) {
      return {};
    }
    const auto c = v[1];
    if ((c == 'x') || (c == 'X') || (c == 'b') || (c == 'B')) {
      return v.substr(0u, 2u);
    }
    return {};
  }  // end of get_literal_prefix

  /*!
   * \brief split the value of a number token into the digit sequence of the
   * literal it holds and its suffixes
   * \param[in]  v: value of a number token, whose digit separators have been
   *                removed
   * \param[out] o: offset of the first character of the suffixes of the literal
   * \return true if the literal is a floating point literal
   *
   * The value of a number token is the spelling of a valid literal, so that its
   * suffixes are made of the characters which follow its digit sequence, the
   * tokenizer reading them at once, see issue #1460. The digit sequence of a
   * literal is its base prefix, which introduces the radix of its digits, its
   * integral and fractional parts and its exponent part, which is introduced by
   * `p` for a hexadecimal literal.
   */
  static bool get_suffix_offset(const std::string& v,
                                std::string::size_type& o) {
    auto is_float = false;
    auto p = v.cbegin();
    const auto pe = v.cend();
    if ((p != pe) && ((*p == '-') || (*p == '+'))) {
      ++p;
    }
    const auto prefix = get_literal_prefix({p, pe});
    const auto radix =
        (prefix.size() != 2u)
            ? 10u
            : (((prefix[1] == 'x') || (prefix[1] == 'X')) ? 16u : 2u);
    p += prefix.size();
    auto is_digit_of_the_radix = [radix](const char c) {
      if (radix == 2u) {
        return is_binary_digit(c);
      }
      if (radix == 16u) {
        return is_hexadecimal_digit(c);
      }
      return is_digit(c);
    };
    while ((p != pe) && (is_digit_of_the_radix(*p))) {
      ++p;
    }
    if ((p != pe) && (*p == '.')) {
      is_float = true;
      ++p;
      while ((p != pe) && (is_digit_of_the_radix(*p))) {
        ++p;
      }
    }
    // the exponent part is that of a floating point literal: an integer literal
    // has none and the digits of the exponent are always decimal digits
    if ((p != pe) && ((radix == 16u) ? ((*p == 'p') || (*p == 'P'))
                                     : ((*p == 'e') || (*p == 'E')))) {
      is_float = true;
      ++p;
      if ((p != pe) && ((*p == '+') || (*p == '-'))) {
        ++p;
      }
      while ((p != pe) && (is_digit(*p))) {
        ++p;
      }
    }
    o = p - v.cbegin();
    return is_float;
  }  // end of get_suffix_offset

  CxxTokenizer::ParseNumberArgument::ParseNumberArgument() = default;

  /*!
   * \return the spelling of the literal which the given number token holds,
   * without its digit separators and without the suffixes which the given
   * options allow to discard, so that its value can be read
   * \param[in] t:      token
   * \param[in] args:   how the spelling of the literal is interpreted
   * \param[in] method: name of the calling method, used in the error messages
   * \throw std::exception if the token is not a number or if it holds a suffix
   * which the options do not allow to discard
   *
   * The value of a number token is the spelling of the literal with its two
   * suffixes, as `1'000'000u_km`, see the description of the `CxxTokenizer`
   * class: that of the literal itself, which tells the type of the value of the
   * literal, as the `u` of `1'000'000u_km`, and that of the user defined
   * literal it belongs to, as its `_km`. The latter is always removed, the
   * former only if `discard_type_suffix` is true, see issue #1460.
   */
  static std::string get_number_literal_value(
      const Token& t,
      const CxxTokenizer::ParseNumberArgument& args,
      const std::string& method) {
    raise_if(t.flag != Token::Number,
             method + ": given value is not a number (read '" + t.value + "')");
    auto v = t.value;
    replace_all(v, '\'', "");
    auto o = std::string::size_type{};
    get_suffix_offset(v, o);
    const auto suffix = std::string_view{v}.substr(o);
    const auto ud = suffix.find('_');
    const auto type_suffix = suffix.substr(0u, ud);
    if ((!args.discard_type_suffix) && (!type_suffix.empty())) {
      raise(method + ": token '" + t.value +
            "' holds a literal with the suffix '" + std::string{type_suffix} +
            "'.");
    }
    if ((!args.discard_user_defined_literal) &&
        (ud != std::string_view::npos)) {
      raise(method + ": token '" + t.value +
            "' holds a user defined literal '" +
            std::string{suffix.substr(ud)} + "'.");
    }
    v.erase((args.discard_type_suffix) ? o : (o + type_suffix.size()));
    return v;
  }  // end of get_number_literal_value

  /*!
   * \brief convert the value of a number token to an integer
   * \return true on success
   * \param[out] r: integer
   * \param[in] v: spelling of an integer literal, whose digit separators and
   *               suffixes have been removed
   *
   * The spelling must be that of a decimal, binary or hexadecimal integer
   * literal, as `3`, `-0x10` or `+0b1000`, in the range of the integer type: a
   * fractional part or an exponent is rejected, even if it does not change the
   * value of the literal (`3.0`). The octal literals are not supported: a
   * number with a leading zero is read as a decimal integer, see issue #1460.
   *
   * \see `CxxTokenizer::readInt` and `get_number_literal_value`
   */
  template <typename IntegerType>
  static bool convertToInteger(IntegerType& r, const std::string& v) {
    // std::from_chars does not accept an explicit plus sign
    auto s = std::string_view{v};
    if ((!s.empty()) && (s.front() == '+')) {
      s.remove_prefix(1u);
    }
    const auto negated = ((!s.empty()) && (s.front() == '-'));
    if ((negated) && (!std::is_signed_v<IntegerType>)) {
      return false;
    }
    // std::from_chars does not accept the base prefix of the binary and
    // hexadecimal literals: the sign which may precede it is applied to the
    // value which is read, so that a literal whose absolute value is out of the
    // range of the type is rejected, as `-0x80000000` for an `int`. On the
    // contrary, it reads the sign of a decimal literal, which is therefore kept
    // so that the minimum value of the type, which has no positive counterpart,
    // can be read
    auto digits = (negated) ? s.substr(1u) : s;
    auto radix = 10;
    if ((digits.size() > 2u) && (digits[0] == '0')) {
      if (((digits[1]) == 'x') || ((digits[1]) == 'X')) {
        radix = 16;
      } else if (((digits[1]) == 'b') || ((digits[1]) == 'B')) {
        radix = 2;
      }
    }
    if (radix != 10) {
      digits.remove_prefix(2u);
    } else {
      digits = s;
    }
    const auto de = digits.data() + digits.size();
    // std::from_chars reports the values out of the range of the type
    auto i = IntegerType{};
    const auto [pi, ec] = std::from_chars(digits.data(), de, i, radix);
    if (ec != std::errc{}) {
      return false;
    }
    const auto suffix = std::string_view(pi, de - pi);
    if (suffix.find_first_not_of("uUlL") != std::string_view::npos) {
      return false;
    }
    r = ((negated) && (radix != 10)) ? static_cast<IntegerType>(-i) : i;
    return true;
  }  // end of convertToInteger

  /*!
   * \return true if the given character may introduce the suffix of a numeric
   * literal
   * \param[in] c: character
   */
  static bool is_suffix_character(const char c) {
    return ((c == 'u') || (c == 'U') || (c == 'l') || (c == 'L') ||
            (c == 'f') || (c == 'F'));
  }  // end of is_suffix_character

  void CxxTokenizer::parseNumber(Token::size_type& o,
                                 std::string::const_iterator& p,
                                 const std::string::const_iterator pe,
                                 const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::parseNumber: " + m);
    };
    // reads the digit sequence which begins at `p` and whose digits are
    // accepted by the given predicate. The digit separators of the `C++14`
    // literals are only allowed between two digits, so that `1'`, `1''2` and
    // `0x'1` are rejected. \return the number of digits which have been read
    auto read_digit_sequence = [&](const auto& is_digit_of_the_radix) {
      auto nb_digits = Token::size_type{};
      while (p != pe) {
        if (is_digit_of_the_radix(*p)) {
          ++nb_digits;
          ++p;
        } else if ((nb_digits != 0u) && (*p == '\'')) {
          // a digit separator, which is not a part of the value of the token
          throw_if(++p == pe, "a digit separator shall be followed by a digit");
          throw_if(!is_digit_of_the_radix(*p),
                   "a digit separator shall be followed by a digit");
          ++nb_digits;
          ++p;
        } else {
          break;
        }
      }
      return nb_digits;
    };
    const auto b = p;
    // In `C++`, the sign of a number is given by an operator and is not a part
    // of the literal: holding the sign in the value of the token is a deviation
    // of `TFEL` which is used by the parsers built on top of `CxxTokenizer`,
    // see issue #1460
    auto is_signed = false;
    if ((*p == '-') || (*p == '+')) {
      if (*p == '-') {
        is_signed = true;
      }
      throw_if(++p == pe, "invalid number");
    }
    throw_if((!(is_digit(*p))) && (*p != '.'),
             std::string("expected digit, read '") + *p + '\'');
    // a floating point literal has a fractional part or an exponent part: the
    // sign of the exponent does not matter
    auto is_float = false;
    // radix of the digits which follow the base prefix of the literal
    auto radix = 10u;
    auto nb_digits = Token::size_type{};
    if (*p == '.') {
      // a fractional-constant may begin with the decimal point, as `.5`. Only a
      // decimal literal can start with the decimal point since the base prefix
      // of a binary or hexadecimal literal comes first
      is_float = true;
      throw_if(++p == pe, "invalid number");
      nb_digits = read_digit_sequence(is_digit);
      throw_if(nb_digits == 0u,
               "a fractional constant shall have at least one digit");
    } else {
      if ((*p == '0') && (std::next(p) != pe)) {
        if ((*std::next(p) == 'b') || (*std::next(p) == 'B')) {
          radix = 2u;
          std::advance(p, 2);
        } else if ((*std::next(p) == 'x') || (*std::next(p) == 'X')) {
          radix = 16u;
          std::advance(p, 2);
        }
      }
      auto is_digit_of_the_radix = [&](const char c) {
        if (radix == 2u) {
          return is_binary_digit(c);
        }
        if (radix == 16u) {
          return is_hexadecimal_digit(c);
        }
        return is_digit(c);
      };
      nb_digits = read_digit_sequence(is_digit_of_the_radix);
      if ((radix != 10u) && (nb_digits == 0u)) {
        // the digits of the integral part may be given by the fractional part,
        // as in `0x.8p+2`
        throw_if((p == pe) || (*p != '.'),
                 (radix == 2u)
                     ? "a binary literal shall have at least one "
                       "binary digit"
                     : "a hexadecimal literal shall have at least one "
                       "hexadecimal digit");
      }
    }
    // the fractional part
    if ((p != pe) && (*p == '.')) {
      throw_if(radix == 2u, "a binary literal has no fractional part");
      throw_if(is_float, "decimal sign multiply defined");
      is_float = true;
      ++p;
      if (radix == 16u) {
        read_digit_sequence(is_hexadecimal_digit);
      } else {
        read_digit_sequence(is_digit);
      }
    }
    // the exponent part, which is introduced by `e` for a decimal literal and
    // by `p` for a hexadecimal literal: an integer literal has no exponent
    // part, so that a binary literal, which is always an integer literal, has
    // none
    auto has_exponent = false;
    if ((p != pe) &&
        ((radix == 16u)
             ? ((*p == 'p') || (*p == 'P'))
             : ((radix == 2u) ? false : ((*p == 'e') || (*p == 'E'))))) {
      is_float = true;
      has_exponent = true;
      ++p;
      if ((p != pe) && ((*p == '+') || (*p == '-'))) {
        ++p;
      }
      // the digits of the exponent part are always decimal digits
      throw_if(read_digit_sequence(is_digit) == 0u,
               "the exponent part of a number shall have at least one digit");
    }
    // a hexadecimal floating point literal has no decimal exponent: the
    // exponent is required since it is the only way to tell it from an integer
    // literal
    throw_if((radix == 16u) && (is_float) && (!has_exponent),
             "a hexadecimal floating point literal requires an exponent");
    // a binary literal is always an integer literal: it has neither fractional
    // part nor exponent part
    throw_if((radix == 2u) && (p != pe) &&
                 (((*p == 'e') || (*p == 'E') || (*p == 'p') || (*p == 'P'))),
             "a binary literal is an integer literal, which has no exponent "
             "part");
    if (is_float) {
      // the suffix of a floating point literal is `f`, `F`, `l` or `L`: the
      // suffixes of the floating point types of `C++23`, as `f32`, are not
      // supported, see issue #1460
      throw_if((p != pe) && ((*p == 'u') || (*p == 'U')),
               "invalid number (floating points can't be unsigned)");
      if ((p != pe) &&
          ((*p == 'f') || (*p == 'F') || (*p == 'l') || (*p == 'L'))) {
        ++p;
      }
    } else {
      // the suffix of an integer literal is read at once so that an ill formed
      // one is rejected and not split in several tokens
      auto suffix = std::string{};
      while ((p != pe) &&
             ((*p == 'u') || (*p == 'U') || (*p == 'l') || (*p == 'L'))) {
        suffix += *p;
        ++p;
      }
      if (!suffix.empty()) {
        throw_if(!is_integer_suffix(suffix),
                 "invalid suffix '" + suffix + "' on an integer literal");
        throw_if(
            (is_signed) && (suffix.find_first_of("uU") != std::string::npos),
            "invalid number (unsigned can't be signed)");
      }
    }
    // the maximal munch rule of the preprocessing tokens, which states that a
    // token is the longest sequence of characters which can build one, can not
    // be applied as strictly as `C++` does it: the tokenizer is also used to
    // read free form texts, as the `@Description` blocks of `MFront`, in which
    // a number is often followed by an identifier, as `5Cr` or `21st`. So that
    // only the characters which can not be explained by the literal which has
    // just been read are rejected: a second decimal point, a digit which is not
    // a digit of the radix of the literal, as the `2` of `0b10102`, and the
    // characters of a suffix which is not a valid one. The identifier `123abc`
    // is still split in `123` and `abc`, which a `C++` compiler will reject
    // later on, see issue #1460.
    if ((p != pe) && (*p == '_')) {
      // suffix of a user defined literal, which is a part of the value of the
      // token
      throw_if(++p == pe, "invalid user litteral");
      while ((p != pe) && ((is_alpha(*p)) || (is_digit(*p)) || (*p == '_'))) {
        ++p;
      }
    } else if ((p != pe) &&
               ((*p == '.') || (is_digit(*p)) || (is_suffix_character(*p)))) {
      throw_if(true,
               std::string{"invalid numeric literal: unexpected character '"} +
                   *p + "' after the literal");
    }
    this->tokens.emplace_back(replace_all(std::string{b, p}, "\'", ""), n, o,
                              Token::Number);
    const auto d = p - b;
    p = b;
    advance(o, p, d);
  }  // end of CxxTokenizer::parseNumber

  void CxxTokenizer::parseCComment(Token::size_type& o,
                                   std::string::const_iterator& p,
                                   const std::string::const_iterator pe,
                                   const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::parseCComment: " + m);
    };
    auto get_end = [&pe](const std::string::const_iterator cp)
        -> std::string::const_iterator {
      auto c = cp;
      while (c != pe) {
        if (*c == '*') {
          const auto cn = std::next(c);
          if (cn == pe) {
            return pe;
          } else if (*cn == '/') {
            return c;
          }
        }
        ++c;
      }
      return pe;
    };
    const auto b = p;
    // The offset of the delimiter which opens the comment. It is kept before
    // the delimiters are read, as o is increased while reading the line. It is
    // that of the token in two cases:
    //
    // - when the comment is empty, since such a token has no contents to point
    //   to, see issue #1391,
    //
    // - when the boundaries of the comment are a part of its value, since such
    //   a value starts with that delimiter, see issue #1407
    const auto bo = o;
    throw_if((p == pe) || (*p != '/'), "invalid call");
    advance(o, p, 1u);
    throw_if((p == pe) || (*p != '*'), "invalid call");
    advance(o, p, 1u);
    auto flag = Token::Comment;
    if ((p != pe) && (*p == '!')) {
      advance(o, p, 1u);
      if ((p != pe) && (*p == '<')) {
        flag = this->tokens.empty() ? Token::Comment
                                    : Token::DoxygenBackwardComment;
        advance(o, p, 1u);
      } else {
        flag = this->tokens.empty() ? Token::Comment : Token::DoxygenComment;
      }
    }
    if (!this->bKeepCommentBoundaries) {
      ignore_space(o, p, pe);
    }
    auto e = get_end(p);
    if (this->bKeepCommentBoundaries) {
      // the value of the token is the whole comment, whose delimiters are a
      // part of it, so that the offset of the token is that of the delimiter
      // which opens it. Reading the delimiters advances the offset along with
      // the iterator, whatever the options are, so that the tokens which follow
      // the comment on the same line keep the offsets of the line, see issue
      // #1407
      if (e == pe) {
        this->tokens.emplace_back(std::string{b, e}, n, bo, flag);
      } else {
        this->tokens.emplace_back(std::string{b, e + 2}, n, bo, flag);
      }
    } else {
      auto value = std::string{};
      if (p != e) {
        auto ps = std::prev(e);
        while ((is_space(*ps)) && (ps != p)) {
          --ps;
        }
        ++ps;
        value = std::string{p, ps};
      } else {
        value = std::string{p, e};
      }
      // A comment without contents on this line, as /**/ or /*   */, or as a
      // `C` style comment opened at the end of a line and whose contents starts
      // on a later line, gives a token with an empty value: the offset of such
      // a token is that of the delimiter which opens it, since it has no
      // contents to point to, see issue #1391
      this->tokens.emplace_back(value, n, (value.empty() ? bo : o), flag);
    }
    advance(o, p, e - p);
    if (e == pe) {
      this->cStyleCommentOpened = true;
    } else {
      advance(o, p, 2u);
    }
  }

  void CxxTokenizer::parseCxxComment(Token::size_type& o,
                                     std::string::const_iterator& p,
                                     const std::string::const_iterator pe,
                                     const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::parseCxxComment: " + m);
    };
    const auto b = p;
    // The offset of the delimiter which opens the comment. It is kept before
    // the delimiters are read, as o is increased while reading the line. It is
    // that of the token in two cases:
    //
    // - when the comment is empty, since such a token has no contents to point
    //   to, see issue #1391,
    //
    // - when the boundaries of the comment are a part of its value, since such
    //   a value starts with that delimiter, see issue #1407
    const auto bo = o;
    throw_if((p == pe) || (*p != '/'), "invalid call");
    advance(o, p, 1u);
    throw_if((p == pe) || (*p != '/'), "invalid call");
    advance(o, p, 1u);
    auto flag = Token::Comment;
    if ((p != pe) && (*p == '!')) {
      advance(o, p, 1u);
      if ((p != pe) && (*p == '<')) {
        flag = this->tokens.empty() ? Token::Comment
                                    : Token::DoxygenBackwardComment;
        advance(o, p, 1u);
      } else {
        flag = this->tokens.empty() ? Token::Comment : Token::DoxygenComment;
      }
    }
    if (this->bKeepCommentBoundaries) {
      // the value of the token is the whole comment, whose delimiters are a
      // part of it, so that the offset of the token is that of the delimiter
      // which opens it. Reading the delimiters advances the offset along with
      // the iterator, whatever the options are, so that the offsets of the
      // lines which follow are those of the lines, see issue #1407
      this->tokens.emplace_back(std::string{b, pe}, n, bo, flag);
    } else {
      ignore_space(o, p, pe);
      // An empty comment, as `//`, gives a token with an empty value: the
      // offset of such a token is that of the delimiter which opens it, since
      // it has no contents to point to, see issue #1391
      this->tokens.emplace_back(std::string{p, pe}, n, (p == pe) ? bo : o,
                                flag);
    }
    advance(o, p, pe - p);
  }

  void CxxTokenizer::parseRawString(Token::size_type& o,
                                    std::string::const_iterator& p,
                                    const std::string::const_iterator pe,
                                    const Token::size_type n,
                                    const Token::size_type ro) {
    auto findDelimiter = [this, pe](std::string::const_iterator c) {
      if (*c != ')') {
        return false;
      }
      ++c;
      for (Token::size_type i = 0; i != this->currentRawStringDelimiter.size();
           ++i) {
        if (c == pe) {
          return false;
        }
        if (*c != this->currentRawStringDelimiter[i]) {
          return false;
        }
        ++c;
      }
      if (c == pe) {
        return false;
      }
      return *c == '\"';
    };
    // the value of a raw string token holds the whole literal, from the `R`
    // delimiter to the closing `"` included, as for a quoted string, and its
    // offset is that of the `R`, see issue #1390. The opening `R"delim(`
    // prefix is restored from the delimiter of the literal, which the caller
    // has read and kept in `currentRawStringDelimiter`
    this->tokens.emplace_back("R\"" + this->currentRawStringDelimiter + "(", n,
                              ro, Token::String);
    auto& value = this->tokens.back().value;
    while (p != pe) {
      if (findDelimiter(p)) {
        // the closing `)delim"` suffix is a part of the value
        value += std::string{p, p + this->currentRawStringDelimiter.size() + 2};
        advance(o, p, this->currentRawStringDelimiter.size() + 2);
        this->rawStringOpened = false;
        this->currentRawStringDelimiter.clear();
        return;
      } else {
        value += *p;
        advance(o, p, 1u);
      }
    }
  }  // end of parseRawString

  void CxxTokenizer::parsePreprocessorDirective(
      Token::size_type& o,
      std::string::const_iterator& p,
      const std::string::const_iterator,
      const std::string::const_iterator pe,
      const Token::size_type n) {
    auto is_preprocessor_keyword = [](const std::string& k) {
      const std::array<const char*, 13> keys = {
          {"#define", "#undef", "#include", "#line", "#error", "#if", "#ifdef",
           "#ifndef", "#elif", "#else", "#endif", "#pragma", "#warning"}};
      return std::find(keys.begin(), keys.end(), k) != keys.end();
    };
    auto throw_if = [](const bool c, const std::string& m) {
      raise_if(c, "CxxTokenizer::parsePreprocessorDirective: " + m);
    };
    // preprocessor
    throw_if((p == pe) || (*p != '#'), "invalid call");
    this->tokens.emplace_back(std::string(1u, '#'), n, o, Token::Preprocessor);
    advance(o, p, 1);
    ignore_space(o, p, pe);
    throw_if(p == pe, "lonely ‘#’");
    auto pn =
        std::find_if(p, pe,
                     is_separator_or_space(
                         this->dotAsSeparator, this->plusAsSeparator,
                         this->minusAsSeparator, this->graveAccentAsSeparator));
    throw_if(p == pn, "unexpected token '" + std::string(1u, *p) + "'");
    const auto key = std::string{p, pn};
    throw_if(!is_preprocessor_keyword('#' + key),
             "invalid preprocessor keyword '" + key + "'");
    this->tokens.emplace_back(std::string{p, pn}, n, o, Token::Preprocessor);
    advance(o, p, pn);
    this->parseStandardLine(o, p, p, pe, n);
  }  // end of CxxTokenizer::parsePreprocessorDirective

  void CxxTokenizer::parseStandardLine(Token::size_type& o,
                                       std::string::const_iterator& p,
                                       const std::string::const_iterator b,
                                       const std::string::const_iterator pe,
                                       const Token::size_type n) {
    auto throw_if = [](const bool c, const std::string& m) {
      raise_if(c, "CxxTokenizer::parseStandardLine: " + m);
    };
    ignore_space(o, p, pe);
    while (p != pe) {
      if (*p == '#') {
        if ((this->treatHashCharacterAsCommentDelimiter) &&
            (this->allowStrayHashCharacter)) {
          if (this->bKeepCommentBoundaries) {
            this->tokens.emplace_back(std::string{p, pe}, n, o, Token::Comment);
            p = pe;
          } else {
            ++p;
            ignore_space(o, p, pe);
            if (p != pe) {
              this->tokens.emplace_back(std::string{p, pe}, n, o,
                                        Token::Comment);
              p = pe;
            }
          }
          advance(o, p, pe - p);
        } else {
          auto pn = std::next(p);
          if (pn == pe) {
            throw_if(!this->allowStrayHashCharacter, "stray ‘#’");
          } else {
            throw_if((!this->allowStrayHashCharacter) && (!is_alpha(*pn)),
                     "stray ‘#’");
          }
          this->tokens.emplace_back("#", n, o, Token::Standard);
          advance(o, p, 1u);
        }
      } else if (*p == '\\') {
        throw_if((!this->allowStrayBackSlash) && (p != std::prev(pe)),
                 "stray ‘\\’");
        this->tokens.emplace_back("\\", n, o, Token::Standard);
        advance(o, p, 1u);
      } else if (is_digit(*p)) {
        if (this->treatNumbers) {
          this->parseNumber(o, p, pe, n);
        } else {
          this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
          advance(o, p, 1u);
        }
      } else if ((*p == 'R') &&
                 ((std::next(p) != pe) && ((*(std::next(p)) == '\"')))) {
        // raw strings litteral
        // the offset of the `R` delimiter: it is that of the token, whose
        // value holds the whole literal, see issue #1390
        const auto ro = o;
        advance(o, p, 2u);
        this->currentRawStringDelimiter.clear();
        this->rawStringOpened = true;
        throw_if(
            p == pe,
            "invalid end of line, expected to read the raw string delimiter");
        while (*p != '(') {
          this->currentRawStringDelimiter += *p;
          advance(o, p, 1u);
          throw_if(
              p == pe,
              "invalid end of line, expected to read the raw string delimiter");
        }
        advance(o, p, 1u);
        this->parseRawString(o, p, pe, n, ro);
      } else if (*p == '\"') {
        // c-strings
        if (this->treatStrings) {
          this->parseString(o, p, pe, n, '\"');
        } else {
          this->tokens.emplace_back("\"", n, o, Token::Standard);
          advance(o, p, 1u);
        }
      } else if (*p == '\'') {
        // c-char
        if (this->treatStrings) {
          this->parseChar(o, p, pe, n);
        } else {
          this->tokens.emplace_back("\'", n, o, Token::Standard);
          advance(o, p, 1u);
        }
      } else if (*p == '<') {
        this->try_join(o, p, pe, n, '<', '=');
      } else if (*p == '>') {
        this->try_join(o, p, pe, n, '>', '=');
      } else if (*p == ':') {
        this->try_join(o, p, pe, n, ':');
      } else if ((*p == '+') || (*p == '-')) {
        auto pn = std::next(p);
        if ((pn != pe) && (*pn == *p)) {
          this->tokens.emplace_back(std::string{p, std::next(pn)}, n, o,
                                    Token::Standard);
          advance(o, p, 2u);
        } else if ((*p == '-') && ((pn != pe) && (*pn == '>'))) {
          auto pn2 = std::next(pn);
          if ((pn2 != pe) && (*pn2 == '*')) {
            this->tokens.emplace_back("->*", n, o, Token::Standard);
            advance(o, p, 3u);
          } else {
            this->tokens.emplace_back("->", n, o, Token::Standard);
            advance(o, p, 2u);
          }
        } else if (((p == b) ||
                    (is_separator_or_space(
                        this->dotAsSeparator, this->plusAsSeparator,
                        this->minusAsSeparator,
                        this->graveAccentAsSeparator)(*(std::prev(p))))) &&
                   ((pn != pe) && ((*pn == '.') || (is_digit(*pn))))) {
          if (this->treatNumbers) {
            this->parseNumber(o, p, pe, n);
          } else {
            this->tokens.emplace_back(std::string(1u, *p), n, o,
                                      Token::Standard);
            advance(o, p, 1u);
          }
        } else {
          this->try_join(o, p, pe, n, '=');
        }
      } else if (*p == '/') {
        auto pn = std::next(p);
        if ((pn != pe) && (*pn == '/') && (this->treatCxxComments)) {
          this->parseCxxComment(o, p, pe, n);
        } else if ((pn != pe) && (*pn == '*') && (this->treatCComments)) {
          this->parseCComment(o, p, pe, n);
        } else {
          this->try_join(o, p, pe, n, '=');
        }
      } else if ((*p == '*') || (*p == '%') || (*p == '!') || (*p == '=')) {
        this->try_join(o, p, pe, n, '=');
      } else if (*p == '&') {
        this->try_join(o, p, pe, n, '&');
      } else if (*p == '.') {
        auto pn = std::next(p);
        if ((pn != pe) && (is_digit(*pn))) {
          if (this->treatNumbers) {
            this->parseNumber(o, p, pe, n);
          } else {
            this->tokens.emplace_back(std::string(1u, *p), n, o,
                                      Token::Standard);
            advance(o, p, 1u);
          }
        } else {
          this->try_join(o, p, pe, n, '.', '*');
        }
      } else if (*p == '|') {
        this->try_join(o, p, pe, n, '|', '=');
      } else {
        const auto pw = std::find_if(
            p, pe,
            is_separator_or_space(this->dotAsSeparator, this->plusAsSeparator,
                                  this->minusAsSeparator,
                                  this->graveAccentAsSeparator));
        const auto as = findSeparator(p, pw, pe, this->additional_separators);
        if (as.first < pw) {
          if (as.first == p) {
            this->tokens.emplace_back(as.second, n, o, Token::Standard);
            advance(o, p, as.second.size());
          } else {
            this->tokens.emplace_back(std::string{p, as.first}, n, o,
                                      Token::Standard);
            advance(o, p, as.first);
          }
        } else {
          if (p == pw) {
            this->tokens.emplace_back(std::string(1u, *p), n, o,
                                      Token::Standard);
            advance(o, p, 1u);
          } else {
            this->tokens.emplace_back(std::string{p, pw}, n, o,
                                      Token::Standard);
            advance(o, p, pw);
          }
        }
      }
      ignore_space(o, p, pe);
    }
  }

  void CxxTokenizer::splitLine(const std::string& line,
                               const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "CxxTokenizer::splitLine: " + m);
    };
    auto b = line.begin();
    auto p = b;
    const auto pe = line.end();
    // offset in line
    auto o = Token::size_type{};
    if (this->cStyleCommentOpened) {
      const auto pos3 = line.find("*/");
      if (tokens.empty()) {
        this->tokens.emplace_back("", n, o, Token::Comment);
      }
      throw_if(((this->tokens.back().flag != Token::Comment) &&
                (this->tokens.back().flag != Token::DoxygenComment) &&
                (this->tokens.back().flag != Token::DoxygenBackwardComment)),
               "internal error (previous token is not a comment)");
      if (!this->tokens.back().value.empty()) {
        this->tokens.back().value += '\n';
      }
      if (pos3 == std::string::npos) {
        this->tokens.back().value += line;
        advance(o, p, line.size());
        return;
      }
      if (this->bKeepCommentBoundaries) {
        this->tokens.back().value += std::string{p, p + pos3 + 2};
      } else {
        this->tokens.back().value += std::string{p, p + pos3};
      }
      advance(o, p, pos3 + 2);
      this->cStyleCommentOpened = false;
    }
    if (this->rawStringOpened) {
      const auto delimiter = ")" + this->currentRawStringDelimiter + '\"';
      const auto pos3 = line.find(delimiter);
      if (tokens.empty()) {
        // the opening `R"delim(` prefix is restored from the delimiter of the
        // literal, which the tokenizer has kept
        this->tokens.emplace_back("R\"" + this->currentRawStringDelimiter + "(",
                                  n, o, Token::String);
      }
      throw_if((this->tokens.back().flag != Token::String),
               "internal error (previous token is not a string)");
      if (!this->tokens.back().value.empty()) {
        this->tokens.back().value += '\n';
      }
      if (pos3 == std::string::npos) {
        this->tokens.back().value += line;
        advance(o, p, line.size());
        return;
      }
      // the closing `)delim"` suffix is a part of the value
      this->tokens.back().value += std::string{p, p + pos3 + delimiter.size()};
      advance(o, p, pos3 + delimiter.size());
      this->rawStringOpened = false;
      this->currentRawStringDelimiter.clear();
    }
    ignore_space(o, p, pe);
    if ((p != pe) && (*p == '#')) {
      if (this->treatPreprocessorDirectives) {
        this->parsePreprocessorDirective(o, p, b, pe, n);
      } else {
        this->parseStandardLine(o, p, b, pe, n);
      }
    } else {
      this->parseStandardLine(o, p, b, pe, n);
    }
  }  // end of CxxTokenizer::splitLine

  void CxxTokenizer::stripComments() {
    // The tokens which are kept are gathered in a second container instead of
    // being built in place by erasing the comment tokens one at a time: erasing
    // a token moves all the tokens which follow it, which makes the cost of
    // this function quadratic in the size of the file, see issue #1409
    auto t = TokensContainer{};
    t.reserve(this->tokens.size());
    for (size_type i = 0u; i != this->tokens.size(); ++i) {
      const auto& p = this->tokens[i];
      if (p.flag == Token::Comment) {
        // the comments of a line are gathered, separated by a space character.
        // The value of an empty comment is empty: it brings nothing and is then
        // neither gathered nor responsible for a separator
        if (!p.value.empty()) {
          const auto it = this->comments.insert({p.line, std::string{}}).first;
          if (!it->second.empty()) {
            it->second += ' ';
          }
          it->second += p.value;
        }
      } else if (p.flag == Token::DoxygenComment) {
        // the token which follows a comment is not a part of the stripped
        // sequence yet: it is that which follows it in the sequence of the
        // tokens of the file
        if ((i + 1u) < this->tokens.size()) {
          auto& p2 = this->tokens[i + 1u];
          if (p2.flag == Token::Standard) {
            if (!p2.comment.empty()) {
              p2.comment += '\n';
            }
            p2.comment += p.value;
          }
          if (p2.flag == Token::DoxygenComment) {
            p2.value = p.value + "\n" + p2.value;
          }
        }
      } else if (p.flag == Token::DoxygenBackwardComment) {
        // the comments which precede a backward comment have already been
        // removed: the token to consider is the last token of the stripped
        // sequence, which is not that which precedes the comment in the
        // sequence of the tokens of the file
        if (!t.empty()) {
          auto& p2 = t.back();
          if (p2.flag == Token::Standard) {
            p2.comment += p.value;
          }
        }
      } else {
        t.push_back(std::move(this->tokens[i]));
      }
    }
    this->tokens = std::move(t);
  }  // end of CxxTokenizer::stripComments

  void CxxTokenizer::printFileTokens(std::ostream& out) const {
    if (this->tokens.empty()) {
      return;
    }
    auto p = this->tokens.begin();
    auto line = p->line;
    for (const auto& t : this->tokens) {
      if (t.line != line) {
        out << '\n';
        line = t.line;
      }
      out << t.value << " ";
    }
    out << '\n';
  }

  bool CxxTokenizer::isValidIdentifier(std::string_view s, const bool b) {
    return CxxTokenizer::isValidIdentifier(s, {.allowCxxKeywords = b});
  }  // end of isValidIdentifier

  bool CxxTokenizer::isValidIdentifier(std::string_view s,
                                       const IsValidIdentifierOptions& opts) {
    if (s.empty()) {
      return false;
    }
    auto p = s.begin();
    if (is_digit(*p)) {
      return false;
    }
    for (; p != s.end(); ++p) {
      if ((*p == '+') && (opts.allowPlusSign)) {
        continue;
      }
      if ((*p == '-') && (opts.allowMinusSign)) {
        continue;
      }
      if ((*p == '.') && (opts.allowDotCharacter)) {
        continue;
      }
      if ((!is_alpha(*p)) && (!(is_digit(*p))) && (*p != '_')) {
        return false;
      }
      if (is_space(*p)) {
        return false;
      }
    }
    if (!opts.allowCxxKeywords) {
      if (isReservedCxxKeywords(s)) {
        return false;
      }
    }
    return true;
  }  // end of CxxTokenizer::isValidIdentifier

  bool CxxTokenizer::isValidFunctionIdentifier(const std::string& s,
                                               const bool checkCxxKeywords) {
    if (s.empty()) {
      return false;
    }
    auto p = s.begin();
    if (is_digit(*p)) {
      return false;
    }
    for (; p != s.end(); ++p) {
      if ((!is_alpha(*p)) && (!(is_digit(*p))) && (*p != '_') && (*p != ':')) {
        return false;
      }
      if (is_space(*p)) {
        return false;
      }
      if (*p == ':') {
        if (++p == s.end()) {
          return false;
        }
        if (*p != ':') {
          return false;
        }
        if (++p == s.end()) {
          return false;
        }
        if ((!is_alpha(*p)) && (!(is_digit(*p))) && (*p != '_')) {
          return false;
        }
      }
    }
    if (checkCxxKeywords) {
      if (isReservedCxxKeywords(s)) {
        return false;
      }
    }
    return true;
  }  // end of CxxTokenizer::isValidFunctionIdentifier

  const Token& CxxTokenizer::operator[](const size_type i) const {
    raise_if<std::out_of_range>(i >= this->size(), "CxxTokenizer::operator[]");
    const auto p = std::next(this->begin(), i);
    return *p;
  }  // end of CxxTokenizer::operator[]

  CxxTokenizer::const_iterator CxxTokenizer::begin() const {
    return this->tokens.begin();
  }  // end of CxxTokenizer::begin

  CxxTokenizer::const_iterator CxxTokenizer::end() const {
    return this->tokens.end();
  }  // end of CxxTokenizer::end

  CxxTokenizer::const_iterator CxxTokenizer::cbegin() const {
    return this->tokens.begin();
  }  // end of CxxTokenizer::begin

  CxxTokenizer::const_iterator CxxTokenizer::cend() const {
    return this->tokens.end();
  }  // end of CxxTokenizer::end

  void CxxTokenizer::setCStyleCommentOpened(const bool b) {
    this->cStyleCommentOpened = b;
  }

  bool CxxTokenizer::isCStyleCommentOpened() const {
    return this->cStyleCommentOpened;
  }  // end of CxxTokenizer::isCStyleCommentOpened

  void CxxTokenizer::setRawStringOpened(const bool b) {
    this->rawStringOpened = b;
    this->currentRawStringDelimiter = "";
  }

  void CxxTokenizer::setRawStringDelimiter(std::string_view d) {
    this->rawStringOpened = true;
    this->currentRawStringDelimiter = d;
  }  // end of setRawStringDelimiter

  bool CxxTokenizer::isRawStringOpened() const {
    return this->rawStringOpened;
  }  // end of CxxTokenizer::isRawStringOpened

  std::string CxxTokenizer::getCurrentRawStringDelimiter() const noexcept {
    return this->currentRawStringDelimiter;
  }  // end of getCurrentRawStringDelimiter

  void CxxTokenizer::clear() {
    this->cStyleCommentOpened = false;
    this->rawStringOpened = false;
    this->currentRawStringDelimiter.clear();
    this->tokens.clear();
    this->comments.clear();
  }  // end of CxxTokenizer::clear

  void CxxTokenizer::checkNotEndOfLine(const std::string& method,
                                       const_iterator& p,
                                       const const_iterator pe) {
    raise_if(p == pe, method + " : unexpected end of line");
  }  // end of CxxTokenizer::checkNotEndOfLine

  void CxxTokenizer::checkNotEndOfLine(const std::string& method,
                                       const std::string& error,
                                       const_iterator& p,
                                       const const_iterator pe) {
    if (p == pe) {
      auto msg = method + ": unexpected end of line";
      if (!error.empty()) {
        msg += " (" + error + ")";
      }
      raise(msg);
    }
  }  // end of CxxTokenizer::checkNotEndOfLine

  void CxxTokenizer::readSpecifiedToken(const std::string& method,
                                        const std::string& value,
                                        const_iterator& p,
                                        const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine(method, "expected '" + value + "'", p, pe);
    raise_if(p->value != value, method + ": unexpected token '" + p->value +
                                    "' "
                                    "(expected '" +
                                    value + "')");
    ++p;
  }  // end of CxxTokenizer::readSpecifiedToken

  std::string CxxTokenizer::readString(const_iterator& p,
                                       const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readString", "", p, pe);
    raise_if(p->flag != Token::String,
             "CxxTokenizer::readString: "
             "expected to read a string (read '" +
                 p->value + "').");
    raise_if(p->value.size() < 2,
             "CxxTokenizer::readString: "
             "internal error (invalid string size)");
    const auto value = getContents(*p);
    ++p;
    return value;
  }  // end of CxxTokenizer::readString

  double CxxTokenizer::readDouble(const_iterator& p, const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readDouble",
                                    "expected number", p, pe);
    const auto res = CxxTokenizer::readDouble(*p, ParseNumberArgument{});
    ++p;
    return res;
  }  // end of CxxTokenizer::readDouble

  double CxxTokenizer::readDouble(const Token& t,
                                  const ParseNumberArgument& args) {
    const auto value =
        get_number_literal_value(t, args, "CxxTokenizer::readDouble");
    auto o = std::string::size_type{};
    const auto is_float = get_suffix_offset(value, o);
    // the digits of the literal, which may be preceded by a sign
    auto digits = std::string_view{value};
    if ((!digits.empty()) &&
        ((digits.front() == '-') || (digits.front() == '+'))) {
      digits.remove_prefix(1u);
    }
    if ((!is_float) && (!get_literal_prefix(digits).empty())) {
      // the standard conversions do not read the radix which the base prefix of
      // a binary or hexadecimal integer literal introduces: the value of such a
      // literal is read as an integer and converted
      auto i = 0ll;
      if (convertToInteger(i, value)) {
        return static_cast<double>(i);
      }
      auto j = 0ull;
      if (convertToInteger(j, value)) {
        return static_cast<double>(j);
      }
      raise(
          "CxxTokenizer::readDouble: "
          "could not read value from token '" +
          t.value + "'.");
    }
    auto res = double{};
#ifdef __CYGWIN__
    std::istringstream is(value);
    is >> res;
    raise_if((is.fail()) || (!is.eof()),
             "CxxTokenizer::readDouble: "
             "could not read value from token '" +
                 t.value + "'.");
#else
    try {
      res = convert<double>(value);
    } catch (std::exception& e) {
      raise(
          "CxxTokenizer::readDouble: "
          "could not read value from token "
          "'" +
          t.value + "' (" + std::string(e.what()) + ").");
    }
#endif
    return res;
  }  // end of CxxTokenizer::readDouble

  int CxxTokenizer::readInt(const_iterator& p, const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readInt", "expected number",
                                    p, pe);
    const auto res = CxxTokenizer::readInt(*p, ParseNumberArgument{});
    ++p;
    return res;
  }  // end of CxxTokenizer::readInt

  int CxxTokenizer::readInt(const Token& t, const ParseNumberArgument& args) {
    const auto value =
        get_number_literal_value(t, args, "CxxTokenizer::readInt");
    auto res = int{};
    raise_if(!convertToInteger(res, value),
             "CxxTokenizer::readInt: "
             "could not read value from token '" +
                 t.value + "'.");
    return res;
  }  // end of CxxTokenizer::readInt

  unsigned int CxxTokenizer::readUnsignedInt(const_iterator& p,
                                             const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readUnsignedInt",
                                    "expected number", p, pe);
    const auto res = CxxTokenizer::readUnsignedInt(*p, ParseNumberArgument{});
    ++p;
    return res;
  }  // end of CxxTokenizer::readUnsignedInt

  unsigned int CxxTokenizer::readUnsignedInt(const Token& t,
                                             const ParseNumberArgument& args) {
    const auto value =
        get_number_literal_value(t, args, "CxxTokenizer::readUnsignedInt");
    auto res = 0u;
    raise_if(!convertToInteger(res, value),
             "CxxTokenizer::readUnsignedInt: "
             "could not read value from token '" +
                 t.value + "'.");
    return res;
  }  // end of CxxTokenizer::readUnsignedInt

  std::string CxxTokenizer::extractNumberTypeSuffix(const Token& t) {
    raise_if(t.flag != Token::Number,
             "CxxTokenizer::extractNumberTypeSuffix: given value is not a "
             "number (read '" +
                 t.value + "')");
    auto v = t.value;
    replace_all(v, '\'', "");
    auto o = std::string::size_type{};
    const auto is_float = get_suffix_offset(v, o);
    // the type suffix of the literal is that which precedes that of a user
    // defined literal, if any
    const auto suffix = std::string_view{v}.substr(o);
    const auto type_suffix = suffix.substr(0u, suffix.find('_'));
    if (is_float) {
      // the suffix of a floating point literal is `f`, `F`, `l` or `L`
      if ((type_suffix.size() == 1u) &&
          ((type_suffix[0] == 'f') || (type_suffix[0] == 'F') ||
           (type_suffix[0] == 'l') || (type_suffix[0] == 'L'))) {
        return std::string{type_suffix};
      }
      return {};
    }
    if (is_integer_suffix(type_suffix)) {
      return std::string{type_suffix};
    }
    return {};
  }  // end of extractNumberTypeSuffix

  std::string CxxTokenizer::extractNumberUserDefinedLiteral(const Token& t) {
    raise_if(
        t.flag != Token::Number,
        "CxxTokenizer::extractNumberUserDefinedLiteral: given value is not "
        "a number (read '" +
            t.value + "')");
    auto v = t.value;
    replace_all(v, '\'', "");
    auto o = std::string::size_type{};
    get_suffix_offset(v, o);
    const auto suffix = std::string_view{v}.substr(o);
    const auto p = suffix.find('_');
    if (p == std::string_view::npos) {
      return {};
    }
    const auto ud_suffix = suffix.substr(p);
    if (!is_user_defined_literal_suffix(ud_suffix)) {
      return {};
    }
    return std::string{ud_suffix};
  }  // end of extractNumberUserDefinedLiteral

  void CxxTokenizer::readList(std::vector<Token>& l,
                              const std::string& m,
                              const std::string& db,
                              const std::string& de,
                              const_iterator& p,
                              const const_iterator pe) {
    CxxTokenizer::readSpecifiedToken(m, db, p, pe);
    CxxTokenizer::checkNotEndOfLine(m, p, pe);
    l.clear();
    while (p->value != de) {
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      raise_if((p->value == ",") || p->value == de || p->value == db,
               m +
                   " : unexpected token ',' or "
                   "'" +
                   db + "' or '" + de + "'");
      l.push_back(*p);
      ++p;
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      if (p->value != de) {
        CxxTokenizer::readSpecifiedToken(m, ",", p, pe);
        CxxTokenizer::checkNotEndOfLine(m, p, pe);
        raise_if(p->value == ",", m + " : unexpected token ',' or '}'");
      }
    }
    CxxTokenizer::readSpecifiedToken(m, de, p, pe);
  }

  std::vector<Token> CxxTokenizer::readList(const std::string& m,
                                            const std::string& db,
                                            const std::string& de,
                                            const_iterator& p,
                                            const const_iterator pe) {
    std::vector<Token> l;
    CxxTokenizer::readList(l, m, db, de, p, pe);
    return l;
  }

  void CxxTokenizer::readArray(const std::string& m,
                               std::vector<std::string>& v,
                               const_iterator& p,
                               const const_iterator pe) {
    CxxTokenizer::readSpecifiedToken(m, "{", p, pe);
    CxxTokenizer::checkNotEndOfLine(m, p, pe);
    v.clear();
    while (p->value != "}") {
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      raise_if((p->value == ",") || p->value == "}" || p->value == "{",
               m + " : unexpected token ',' or '{' or '}'");
      v.push_back(p->value);
      ++p;
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      if (p->value != "}") {
        CxxTokenizer::readSpecifiedToken(m, ",", p, pe);
        CxxTokenizer::checkNotEndOfLine(m, p, pe);
        raise_if(p->value == ",", m + " : unexpected token ',' or '}'");
      }
    }
    CxxTokenizer::readSpecifiedToken(m, "}", p, pe);
  }  // end of CxxTokenizer::readArray

  std::vector<std::string> CxxTokenizer::readArray(const std::string& m,
                                                   const_iterator& p,
                                                   const const_iterator pe) {
    std::vector<std::string> v;
    CxxTokenizer::readArray(m, v, p, pe);
    return v;
  }  // end of CxxTokenizer::readArray

  std::vector<std::string> CxxTokenizer::readStringArray(
      const_iterator& p, const const_iterator pe) {
    const std::string m = "CxxTokenizer::readStringArray";
    CxxTokenizer::readSpecifiedToken(m, "{", p, pe);
    CxxTokenizer::checkNotEndOfLine(m, p, pe);
    auto r = std::vector<std::string>{};
    while (p->value != "}") {
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      r.push_back(CxxTokenizer::readString(p, pe));
      CxxTokenizer::checkNotEndOfLine(m, p, pe);
      if (p->value != "}") {
        CxxTokenizer::readSpecifiedToken(m, ",", p, pe);
        CxxTokenizer::checkNotEndOfLine(m, p, pe);
        raise_if(p->value == "}",
                 "CxxTokenizer::readStringArray: "
                 "unexpected token '}'");
      }
    }
    CxxTokenizer::readSpecifiedToken(m, "}", p, pe);
    return r;
  }  // end of CxxTokenizer::readStringArray

  bool CxxTokenizer::empty() const {
    return this->tokens.empty();
  }  // end of CxxTokenizer::size

  CxxTokenizer::size_type CxxTokenizer::size() const {
    return this->tokens.size();
  }  // end of CxxTokenizer::size

  void CxxTokenizer::substitute(
      const std::map<std::string, std::string>& substitutions) {
    for (auto& token : this->tokens) {
      for (const auto& [s1, s2] : substitutions) {
        token.value = tfel::utilities::replace_all(token.value, s1, s2);
      }
    }
  }  // end of substitute

  CxxTokenizer::~CxxTokenizer() = default;

}  // end of namespace tfel::utilities
