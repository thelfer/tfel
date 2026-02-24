/*!
 * \file   src/Utilities/CxxTokenizer.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12 nov 2006
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
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxKeywords.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T &v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace tfel::utilities {

  static std::pair<std::string::const_iterator, std::string> findSeparator(
      const std::string::const_iterator p,
      const std::string::const_iterator pe,
      const std::vector<std::string> &separators) {
    const auto w = std::string{p, pe};
    std::pair<std::string::const_iterator, std::string> r = {pe, ""};
    for (const auto &s : separators) {
      const auto pos = w.find(s);
      if (pos != std::string::npos) {
        if (p + pos < r.first) {
          r = {p + pos, s};
        }
      }
    }
    return r;
  }  // end of findSeparator

  CxxTokenizer::CxxTokenizer() = default;

  CxxTokenizer::CxxTokenizer(const CxxTokenizerOptions &o)
      : CxxTokenizerOptions(o) {}  // end of CxxTokenizer::CxxTokenizer

  CxxTokenizer::CxxTokenizer(const std::string &f) : CxxTokenizer() {
    this->openFile(f);
  }  // end of CxxTokenizer::CxxTokenizer

  CxxTokenizer::CxxTokenizer(const std::string &f, const CxxTokenizerOptions &o)
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

  void CxxTokenizer::treatMinusAsSeparator(const bool b) {
    this->minusAsSeparator = b;
  }  // end of CxxTokenizer::treatMinusAsSeparator

  void CxxTokenizer::mergeStrings(const bool b) {
    this->shallMergeStrings = b;
  }  // end of CxxTokenizer::mergeStrings

  void CxxTokenizer::extractNumbers(const bool b) {
    this->treatNumbers = b;
  }  // end of CxxTokenizer::extractNumbers

  void CxxTokenizer::openFile(const std::string &f) {
    std::ifstream file{f};
    raise_if(!file,
             "CxxTokenizer::openFile: "
             "unable to open file '" +
                 f + "'");
    auto n = Token::size_type{};
    this->parseStream(file, n, " of file '" + f + "'");
  }

  void CxxTokenizer::addSeparator(const std::string &s) {
    if (std::find(this->additional_separators.begin(),
                  this->additional_separators.end(),
                  s) == this->additional_separators.end()) {
      this->additional_separators.push_back(s);
    }
  }  // end of CxxTokenizer::addSeparator

  void CxxTokenizer::parseString(const std::string &s) {
    std::istringstream iss(s);
    auto n = Token::size_type{};
    this->parseStream(iss, n, " of string '" + s + "'");
  }  // end of CxxTokenizer::parseOneString

  void CxxTokenizer::parseStream(std::istream &in,
                                 Token::size_type &n,
                                 const std::string &from) {
    auto throw_if = [](const bool b, const std::string &m) {
      raise_if(b, "CxxTokenizer::parseStream: " + m);
    };
    try {
      while (!in.eof()) {
        throw_if(!in.good(), "error while parsing stream");
        throw_if(!in.good(), "error while parsing stream");
        auto line = std::string{};
        std::getline(in, line);
        ++n;
        try {
          this->splitLine(line, n);
        } catch (std::runtime_error &e) {
          raise(std::string(e.what()) + ".\n" +
                "Error at line: " + std::to_string(n) + from);
        }
      }
    } catch (...) {
      this->clear();
      throw;
    }
    if (this->addCurlyBraces) {
      this->tokens.insert(this->tokens.begin(), Token("{", 0, 0));
      if (!this->tokens.empty()) {
        const auto &b = this->tokens.back();
        this->tokens.insert(this->tokens.end(), Token("}", b.line + 1u, 0u));
      } else {
        this->tokens.insert(this->tokens.end(), Token("}", 0u, 0u));
      }
    }
  }  // end of parseString

  struct is_separator {
    //! constructor
    explicit is_separator(const bool bd, const bool bm, const bool ba)
        : dot_as_separator(bd),
          minus_as_separator(bm),
          grave_accent_as_separator(ba) {}  // end of is_separator
    bool operator()(const std::string::value_type &c) const {
      using ctype = std::string::value_type;
      constexpr std::array<ctype, 24> s = {
          {'?', ';', '/', '!', '&', '*', '|', '{', '}',  '[',  ']', '(',
           ')', '%', '=', '^', ',', ':', '<', '>', '\'', '\"', '+', '\\'}};
      if (c == '.') {
        return this->dot_as_separator;
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
    bool minus_as_separator = true;
    bool grave_accent_as_separator = true;
  };

  struct is_separator_or_space : public is_separator {
    explicit is_separator_or_space(const bool bd, const bool bm, const bool ba)
        : is_separator(bd, bm, ba) {}
    bool operator()(const std::string::value_type &c) const {
      return ((std::isspace(c)) || (is_separator::operator()(c)));
    }
  };  // end of struct is_separator_or_space

  static void advance(Token::size_type &o,
                      std::string::const_iterator &p,
                      Token::size_type n) {
    o += n;
    std::advance(p, n);
  }

  static void advance(Token::size_type &o,
                      std::string::const_iterator &p,
                      const std::string::const_iterator np) {
    o += Token::size_type(np - p);
    p = np;
  }

  static void ignore_space(Token::size_type &o,
                           std::string::const_iterator &p,
                           const std::string::const_iterator pe) {
    auto np = std::find_if_not(
        p, pe, [](const std::string::value_type c) { return std::isspace(c); });
    advance(o, p, np);
  }

  void CxxTokenizer::parseChar(Token::size_type &o,
                               std::string::const_iterator &p,
                               const std::string::const_iterator pe,
                               const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string &m) {
      raise_if(b, "CxxTokenizer::parseCChar: " + m);
    };
    if (this->charAsString) {
      this->parseString(o, p, pe, n, '\'');
    } else {
      const auto pn = std::next(p, 1);
      const auto pn2 = std::next(p, 2);
      const auto pn3 = std::next(p, 3);
      throw_if(pn == pe, "unterminated char");
      if (*pn == '\\') {
        const auto pn4 = std::next(p, 4);
        throw_if((pn2 == pe) || (pn3 == pe), "unterminated char");
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

  void CxxTokenizer::parseString(Token::size_type &o,
                                 std::string::const_iterator &p,
                                 const std::string::const_iterator pe,
                                 const Token::size_type n,
                                 const char e) {
    auto throw_if = [](const bool b, const std::string &m) {
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
      auto &value = this->tokens.back().value;
      this->tokens.back().value =
          value.substr(0, value.size() - 1) + std::string(p + 1, ps);
    } else {
      this->tokens.emplace_back(std::string(p, ps), n, o, Token::String);
    }
    advance(o, p, ps);
  }

  void CxxTokenizer::try_join(Token::size_type &o,
                              std::string::const_iterator &p,
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

  void CxxTokenizer::try_join(Token::size_type &o,
                              std::string::const_iterator &p,
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

  void CxxTokenizer::parseNumber(Token::size_type &o,
                                 std::string::const_iterator &p,
                                 const std::string::const_iterator pe,
                                 const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string &m) {
      raise_if(b, "CxxTokenizer::parseNumber: " + m);
    };
    auto is_binary = [](const char c) { return (c == '0') || (c == '1'); };
    auto is_hex = [](const char c) {
      return (((c >= '0') && (c <= '7')) || ((c >= 'a') && (c <= 'f')) ||
              ((c >= 'A') && (c <= 'F')));
    };
    const auto b = p;
    // if true, reading a float point value, otherwise
    // could be a float or an integer
    auto is_float = false;
    auto is_signed = false;
    auto is_binary_integer = false;
    // auto is_octal_integer  = false;
    auto is_hex_integer = false;
    auto starts_with_zero = false;
    auto has_exponent = false;
    if ((*p == '-') || (*p == '+')) {
      if (*p == '-') {
        is_signed = true;
      }
      // reading decimal part
      throw_if(++p == pe, "invalid number");
    }
    throw_if((!(std::isdigit(*p))) && (*p != '.'),
             std::string("expected digit, read '") + *p + '\'');
    if (*p == '.') {
      is_float = true;
      throw_if(++p == pe, "invalid number");
      throw_if(!std::isdigit(*p),
               std::string("expected digit, read '") + *p + '\'');
    } else if (*p == '0') {
      starts_with_zero = true;
      if (++p != pe) {
        if (*p == 'b') {
          // reading binary integer
          is_binary_integer = true;
          throw_if(++p == pe, "invalid binary integer");
          throw_if(!is_binary(*p), "invalid binary integer");
          while ((p != pe) && (std::isdigit(*p))) {
            throw_if(!is_binary(*p), "invalid binary integer");
            ++p;
          }
        } else if (*p == 'x') {
          // reading hex integer
          is_hex_integer = true;
          throw_if(++p == pe, "invalid hexadecimal integer");
          throw_if(!is_hex(*p), "invalid hexadecimal integer");
          while ((p != pe) && (std::isdigit(*p))) {
            throw_if(!is_hex(*p), "invalid hexadecimal integer");
            ++p;
          }
        } else {
          --p;
        }
      }
    }
    while ((p != pe) && ((std::isdigit(*p)) || (*p == '\''))) {
      if (*p == '\'') {
        throw_if(++p == pe, "invalid number");
        throw_if(!std::isdigit(*p),
                 std::string("expected digit, read '") + *p + '\'');
      }
      ++p;
    }
    if ((p != pe) && (*p == '.')) {
      throw_if(is_hex_integer, "invalid hexadecimal integer");
      throw_if(is_binary_integer, "invalid binary integer");
      throw_if(is_float, "decimal sign multiply defined");
      is_float = true;
      // the decimal part
      ++p;
      while ((p != pe) && ((std::isdigit(*p)) || (*p == '\''))) {
        if (*p == '\'') {
          // C++14 digit separator
          throw_if(++p == pe, "invalid number");
          throw_if(!std::isdigit(*p),
                   std::string("expected digit, read '") + *p + '\'');
        }
        ++p;
      }
    }
    throw_if((p != pe) && (*p == '.'), "invalid number");
    if (p != pe) {
      throw_if(is_hex_integer, "invalid hexadecimal integer");
      throw_if(is_binary_integer, "invalid binary integer");
      if ((*p == 'e') || (*p == 'E')) {
        has_exponent = true;
        throw_if(++p == pe, "invalid number");
        if ((*p == '+') || (*p == '-')) {
          if (*p == '-') {
            is_float = true;
          }
          throw_if(++p == pe, "invalid number");
        }
        throw_if(!(std::isdigit(*p)), "invalid number");
        ++p;
        while ((p != pe) && ((std::isdigit(*p)) || (*p == '\''))) {
          if (*p == '\'') {
            throw_if(++p == pe, "invalid number");
            throw_if(!std::isdigit(*p),
                     std::string("expected digit, read '") + *p + '\'');
          }
          ++p;
        }
      }
    }
    throw_if((p != pe) && (*p == '.'), "invalid number");
    // checking  octal integer
    if ((!is_float) && (!is_hex_integer) && (!is_binary_integer) &&
        (starts_with_zero) && (!has_exponent)) {
      // we may an octal integer
      for (auto c = b; c != p; ++c) {
        throw_if(!((*p >= '0') || (*p <= '7')), "invalid octal number");
      }
      // is_octal_integer=true;
    }
    if (p != pe) {
      if (((is_float) &&
           ((*p == 'l') || (*p == 'f') || (*p == 'L') || (*p == 'F'))) ||
          ((!is_float) &&
           ((*p == 'u') || (*p == 'U') || (*p == 'l') || (*p == 'L')))) {
        throw_if(((*p == 'u') || (*p == 'U')) && (is_signed),
                 "invalid number (unsigned can't be signed)");
        if ((*p == 'f') || (*p == 'F')) {
          ++p;
        } else if ((*p == 'l') || (*p == 'L')) {
          // long suffix
          ++p;
          if ((p != pe) && ((*p == 'l') || (*p == 'L'))) {
            throw_if(is_float, "invalid float suffix");
            // long long suffix
            ++p;
            if ((p != pe) && ((*p == 'u') || (*p == 'U'))) {
              throw_if(is_float,
                       "invalid number (floating points "
                       "can't be unsigned)");
              throw_if(is_signed,
                       "invalid number (unsigned "
                       "can't be signed)");
              ++p;
            }
          } else if ((p != pe) && ((*p == 'u') || (*p == 'U'))) {
            // long unsigned
            throw_if(is_float,
                     "invalid number (floating points "
                     "can't be unsigned)");
            throw_if(is_signed,
                     "invalid number (unsigned "
                     "can't be signed)");
            ++p;
          }
        } else if ((*p == 'u') || (*p == 'U')) {
          // usigned integer
          throw_if(is_signed, "invalid number (unsigned can't be signed)");
          ++p;
          if ((p != pe) && ((*p == 'l') || (*p == 'L'))) {
            // long usigned integer
            ++p;
            if ((p != pe) && ((*p == 'l') || (*p == 'L'))) {
              // long long usigned integer
              ++p;
            }
          }
        } else {
          ++p;
        }
      }
    }
    throw_if((p != pe) && (*p == '.'), "invalid number");
    // C++11 user defined litterals
    if ((p != pe) && (*p == '_')) {
      throw_if(++p == pe, "invalid user litteral");
      while (p != pe) {
        if (!((isalpha(*p)) || std::isdigit(*p) || (*p == '_'))) {
          break;
        }
        ++p;
      }
    }
    throw_if((p != pe) && (*p == '.'), "invalid number");
    this->tokens.emplace_back(replace_all(std::string{b, p}, "\'", ""), n, o,
                              Token::Number);
    const auto d = p - b;
    p = b;
    advance(o, p, d);
  }  // end of CxxTokenizer::parseNumber

  void CxxTokenizer::parseCComment(Token::size_type &o,
                                   std::string::const_iterator &p,
                                   const std::string::const_iterator pe,
                                   const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string &m) {
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
    auto next_token = [this, &p, &o] {
      if (this->bKeepCommentBoundaries) {
        ++p;
      } else {
        advance(o, p, 1u);
      }
    };
    const auto b = p;
    throw_if((p == pe) || (*p != '/'), "invalid call");
    next_token();
    throw_if((p == pe) || (*p != '*'), "invalid call");
    next_token();
    auto flag = Token::Comment;
    if ((p != pe) && (*p == '!')) {
      next_token();
      if ((p != pe) && (*p == '<')) {
        flag = this->tokens.empty() ? Token::Comment
                                    : Token::DoxygenBackwardComment;
        next_token();
      } else {
        flag = this->tokens.empty() ? Token::Comment : Token::DoxygenComment;
      }
    }
    if (!this->bKeepCommentBoundaries) {
      ignore_space(o, p, pe);
    }
    auto e = get_end(p);
    if (this->bKeepCommentBoundaries) {
      if (e == pe) {
        this->tokens.emplace_back(std::string{b, e}, n, o, flag);
      } else {
        this->tokens.emplace_back(std::string{b, e + 2}, n, o, flag);
      }
    } else {
      if (p != e) {
        auto ps = std::prev(e);
        while ((std::isspace(*ps)) && (ps != p)) {
          --ps;
        }
        ++ps;
        this->tokens.emplace_back(std::string{p, ps}, n, o, flag);
      } else {
        this->tokens.emplace_back(std::string{p, e}, n, o, flag);
      }
    }
    advance(o, p, e - p);
    if (e == pe) {
      this->cStyleCommentOpened = true;
    } else {
      advance(o, p, 2u);
    }
  }

  void CxxTokenizer::parseCxxComment(Token::size_type &o,
                                     std::string::const_iterator &p,
                                     const std::string::const_iterator pe,
                                     const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string &m) {
      raise_if(b, "CxxTokenizer::parseCxxComment: " + m);
    };
    auto next_token = [this, &p, &o] {
      if (this->bKeepCommentBoundaries) {
        ++p;
      } else {
        advance(o, p, 1u);
      }
    };
    const auto b = p;
    throw_if((p == pe) || (*p != '/'), "invalid call");
    next_token();
    throw_if((p == pe) || (*p != '/'), "invalid call");
    next_token();
    auto flag = Token::Comment;
    if ((p != pe) && (*p == '!')) {
      next_token();
      if ((p != pe) && (*p == '<')) {
        flag = this->tokens.empty() ? Token::Comment
                                    : Token::DoxygenBackwardComment;
        next_token();
      } else {
        flag = this->tokens.empty() ? Token::Comment : Token::DoxygenComment;
      }
    }
    if (this->bKeepCommentBoundaries) {
      this->tokens.emplace_back(std::string{b, pe}, n, o, flag);
    } else {
      ignore_space(o, p, pe);
      this->tokens.emplace_back(std::string{p, pe}, n, o, flag);
    }
    advance(o, p, pe - p);
  }

  void CxxTokenizer::parseRawString(Token::size_type &o,
                                    std::string::const_iterator &p,
                                    const std::string::const_iterator pe,
                                    const Token::size_type n) {
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
    this->tokens.emplace_back("", n, o, Token::String);
    auto &value = this->tokens.back().value;
    while (p != pe) {
      if (findDelimiter(p)) {
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
      Token::size_type &o,
      std::string::const_iterator &p,
      const std::string::const_iterator,
      const std::string::const_iterator pe,
      const Token::size_type n) {
    auto is_preprocessor_keyword = [](const std::string &k) {
      const std::array<const char *, 13> keys = {
          {"#define", "#undef", "#include", "#line", "#error", "#if", "#ifdef",
           "#ifndef", "#elif", "#else", "#endif", "#pragma", "#warning"}};
      return std::find(keys.begin(), keys.end(), k) != keys.end();
    };
    auto throw_if = [](const bool c, const std::string &m) {
      raise_if(c, "CxxTokenizer::parsePreprocessorDirective: " + m);
    };
    // preprocessor
    throw_if((p == pe) || (*p != '#'), "invalid call");
    this->tokens.emplace_back(std::string(1u, '#'), n, o, Token::Preprocessor);
    advance(o, p, 1);
    ignore_space(o, p, pe);
    throw_if(p == pe, "lonely ‘#’");
    auto pn = std::find_if(
        p, pe,
        is_separator_or_space(this->dotAsSeparator, this->minusAsSeparator,
                              this->graveAccentAsSeparator));
    throw_if(p == pn, "unexpected token '" + std::string(1u, *p) + "'");
    const auto key = std::string{p, pn};
    throw_if(!is_preprocessor_keyword('#' + key),
             "invalid preprocessor keyword '" + key + "'");
    this->tokens.emplace_back(std::string{p, pn}, n, o, Token::Preprocessor);
    advance(o, p, pn);
    this->parseStandardLine(o, p, p, pe, n);
  }  // end of CxxTokenizer::parsePreprocessorDirective

  void CxxTokenizer::parseStandardLine(Token::size_type &o,
                                       std::string::const_iterator &p,
                                       const std::string::const_iterator b,
                                       const std::string::const_iterator pe,
                                       const Token::size_type n) {
    auto throw_if = [](const bool c, const std::string &m) {
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
            throw_if(
                (!this->allowStrayHashCharacter) && (std::isalpha(*pn) == 0),
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
      } else if (std::isdigit(*p)) {
        if (this->treatNumbers) {
          this->parseNumber(o, p, pe, n);
        } else {
          this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
          advance(o, p, 1u);
        }
      } else if ((*p == 'R') &&
                 ((std::next(p) != pe) && ((*(std::next(p)) == '\"')))) {
        // raw strings litteral
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
        this->parseRawString(o, p, pe, n);
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
          auto pn2 = std::next(p);
          if ((pn2 != pe) && (*pn == '*')) {
            this->tokens.emplace_back("->*", n, o, Token::Standard);
            advance(o, p, 3u);
          } else {
            this->tokens.emplace_back("->", n, o, Token::Standard);
            advance(o, p, 2u);
          }
        } else if (((p == b) ||
                    (is_separator_or_space(
                        this->dotAsSeparator, this->minusAsSeparator,
                        this->graveAccentAsSeparator)(*(std::prev(p))))) &&
                   ((pn != pe) && ((*pn == '.') || (std::isdigit(*pn))))) {
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
        if ((pn != pe) && (std::isdigit(*pn))) {
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
        const auto as = findSeparator(p, pe, this->additional_separators);
        const auto pw = std::find_if(
            p, pe,
            is_separator_or_space(this->dotAsSeparator, this->minusAsSeparator,
                                  this->graveAccentAsSeparator));
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

  void CxxTokenizer::splitLine(const std::string &line,
                               const Token::size_type n) {
    auto throw_if = [](const bool b, const std::string &m) {
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
      if (pos3 == std::string::npos) {
      }
      if (tokens.empty()) {
        this->tokens.emplace_back("", n, o, Token::String);
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
      this->tokens.back().value += std::string{p, p + pos3};
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
    for (auto p = this->tokens.begin(); p != this->tokens.end();) {
      if (p->flag == Token::Comment) {
        if (this->comments.find(p->line) != this->comments.end()) {
          this->comments[p->line] += ' ';
        }
        this->comments[p->line] = p->value;
        p = this->tokens.erase(p);
      } else if (p->flag == Token::DoxygenComment) {
        auto p2 = p;
        ++p2;
        if (p2 != this->tokens.end()) {
          if (p2->flag == Token::Standard) {
            if (!p2->comment.empty()) {
              p2->comment += '\n';
            }
            p2->comment += p->value;
          }
          if (p2->flag == Token::DoxygenComment) {
            p2->value = p->value + "\n" + p2->value;
          }
        }
        p = this->tokens.erase(p);
      } else if (p->flag == Token::DoxygenBackwardComment) {
        auto p2 = p;
        --p2;
        if (p2 != this->tokens.begin()) {
          if (p2->flag == Token::Standard) {
            p2->comment += p->value;
          }
        }
        p = this->tokens.erase(p);
      } else {
        ++p;
      }
    }
  }  // end of CxxTokenizer::stripComments

  void CxxTokenizer::printFileTokens(std::ostream &out) const {
    auto p = this->tokens.begin();
    auto line = p->line;
    for (const auto &t : this->tokens) {
      if (t.line != line) {
        out << '\n';
        line = t.line;
      }
      out << t.value << " ";
    }
    out << '\n';
  }

  bool CxxTokenizer::isValidIdentifier(std::string_view s, const bool b) {
    if (s.empty()) {
      return false;
    }
    auto p = s.begin();
    if (std::isdigit(*p)) {
      return false;
    }
    for (; p != s.end(); ++p) {
      if ((!isalpha(*p)) && (!(std::isdigit(*p))) && (*p != '_')) {
        return false;
      }
      if (isspace(*p)) {
        return false;
      }
    }
    if (b) {
      if (isReservedCxxKeywords(s)) {
        return false;
      }
    }
    return true;
  }  // end of CxxTokenizer::isValidIdentifier

  bool CxxTokenizer::isValidFunctionIdentifier(const std::string &s,
                                               const bool checkCxxKeywords) {
    if (s.empty()) {
      return false;
    }
    auto p = s.begin();
    if (std::isdigit(*p)) {
      return false;
    }
    for (; p != s.end(); ++p) {
      if ((!isalpha(*p)) && (!(std::isdigit(*p))) && (*p != '_') &&
          (*p != ':')) {
        return false;
      }
      if (isspace(*p)) {
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
        if ((!isalpha(*p)) && (!(std::isdigit(*p))) && (*p != '_')) {
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

  const Token &CxxTokenizer::operator[](const size_type i) const {
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

  void CxxTokenizer::checkNotEndOfLine(const std::string &method,
                                       const_iterator &p,
                                       const const_iterator pe) {
    raise_if(p == pe, method + " : unexpected end of line");
  }  // end of CxxTokenizer::checkNotEndOfLine

  void CxxTokenizer::checkNotEndOfLine(const std::string &method,
                                       const std::string &error,
                                       const_iterator &p,
                                       const const_iterator pe) {
    if (p == pe) {
      auto msg = method + ": unexpected end of line";
      if (!error.empty()) {
        msg += " (" + error + ")";
      }
      raise(msg);
    }
  }  // end of CxxTokenizer::checkNotEndOfLine

  void CxxTokenizer::readSpecifiedToken(const std::string &method,
                                        const std::string &value,
                                        const_iterator &p,
                                        const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine(method, "expected '" + value + "'", p, pe);
    raise_if(p->value != value, method + ": unexpected token '" + p->value +
                                    "' "
                                    "(expected '" +
                                    value + "')");
    ++p;
  }  // end of CxxTokenizer::readSpecifiedToken

  std::string CxxTokenizer::readString(const_iterator &p,
                                       const const_iterator pe) {
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readString", "", p, pe);
    raise_if(p->flag != Token::String,
             "CxxTokenizer::readString: "
             "expected to read a string (read '" +
                 p->value + "').");
    raise_if(p->value.size() < 2,
             "CxxTokenizer::readString: "
             "internal error (invalid string size)");
    const auto value = p->value.substr(1, p->value.size() - 2);
    ++p;
    return value;
  }  // end of CxxTokenizer::readString

  double CxxTokenizer::readDouble(const_iterator &p, const const_iterator pe)

  {
    double res;
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readDouble",
                                    "expected number", p, pe);
    raise_if(p->flag != Token::Number,
             "CxxTokenizer::readDouble: given value is not a number (read '" +
                 p->value + "')");
    const auto value = [&p] {
      auto v = p->value;
      replace_all(v, '\'', "");
      return v;
    }();
#ifdef __CYGWIN__
    std::istringstream is(value);
    is >> res;
    raise_if(!is && (!is.eof()),
             "CxxTokenizer::readDouble: "
             "could not read value from token '" +
                 p->value + "'.");
#else
    try {
      res = convert<double>(value);
    } catch (std::exception &e) {
      raise(
          "CxxTokenizer::readDouble: "
          "could not read value from token "
          "'" +
          p->value + "' (" + std::string(e.what()) + ").");
    }
#endif
    ++p;
    return res;
  }  // end of CxxTokenizer::readDouble

  int CxxTokenizer::readInt(const_iterator &p, const const_iterator pe)

  {
    int res;
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readInt", "expected number",
                                    p, pe);
    raise_if(p->flag != Token::Number,
             "CxxTokenizer::readInt: given value is not a number (read " +
                 p->value + ")");
    const auto value = [&p] {
      auto v = p->value;
      replace_all(v, '\'', "");
      return v;
    }();
    std::istringstream is(value);
    is >> res;
    raise_if(!is && (!is.eof()),
             "CxxTokenizer::readInt: "
             "could not read value from token '" +
                 p->value + "'.");
    ++p;
    return res;
  }  // end of CxxTokenizer::readInt

  unsigned int CxxTokenizer::readUnsignedInt(const_iterator &p,
                                             const const_iterator pe) {
    unsigned int res;
    CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readUnsignedInt",
                                    "expected number", p, pe);
    raise_if(p->flag != Token::Number,
             "CxxTokenizer::readInt: given value is not a number (read " +
                 p->value + ")");
    const auto value = [&p] {
      auto v = p->value;
      replace_all(v, '\'', "");
      return v;
    }();
    std::istringstream is(value);
    is >> res;
    if (!is && (!is.eof())) {
      tfel::raise(
          "CxxTokenizer::readUnsignedInt: "
          "could not read value from token '" +
          p->value + "'.\n");
    }
    ++p;
    return res;
  }  // end of CxxTokenizer::readUnsignedInt

  void CxxTokenizer::readList(std::vector<Token> &l,
                              const std::string &m,
                              const std::string &db,
                              const std::string &de,
                              const_iterator &p,
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

  std::vector<Token> CxxTokenizer::readList(const std::string &m,
                                            const std::string &db,
                                            const std::string &de,
                                            const_iterator &p,
                                            const const_iterator pe) {
    std::vector<Token> l;
    CxxTokenizer::readList(l, m, db, de, p, pe);
    return l;
  }

  void CxxTokenizer::readArray(const std::string &m,
                               std::vector<std::string> &v,
                               const_iterator &p,
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

  std::vector<std::string> CxxTokenizer::readArray(const std::string &m,
                                                   const_iterator &p,
                                                   const const_iterator pe) {
    std::vector<std::string> v;
    CxxTokenizer::readArray(m, v, p, pe);
    return v;
  }  // end of CxxTokenizer::readArray

  std::vector<std::string> CxxTokenizer::readStringArray(
      const_iterator &p, const const_iterator pe) {
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

  CxxTokenizer::~CxxTokenizer() = default;

}  // end of namespace tfel::utilities
