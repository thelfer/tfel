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

#include <array>
#include <cctype>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>

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

namespace tfel {

  namespace utilities {

    CxxTokenizer::CxxTokenizer() = default;

    CxxTokenizer::CxxTokenizer(const std::string& f) : CxxTokenizer() {
      this->openFile(f);
    }

    void CxxTokenizer::treatCharAsString(const bool b) {
      this->charAsString = b;
    }  // end of CxxTokenizer::treatCharAsString

    void CxxTokenizer::treatDotAsSeparator(const bool b) {
      this->dotAsSeparator = b;
    }  // end of CxxTokenizer::treatDotAsSeparator

    void CxxTokenizer::treatMinusAsSeparator(const bool b) {
      this->minusAsSeparator = b;
    }  // end of CxxTokenizer::treatMinusAsSeparator

    void CxxTokenizer::mergeStrings(const bool b) {
      this->shallMergeStrings = b;
    }  // end of CxxTokenizer::mergeStrings

    void CxxTokenizer::extractNumbers(const bool) {
    }  // end of CxxTokenizer::extractNumbers

    void CxxTokenizer::openFile(const std::string& f) {
      std::ifstream file{f};
      raise_if(!file,
               "CxxTokenizer::openFile: "
               "unable to open file '" +
                   f + "'");
      Token::size_type n{0};
      this->parseStream(file, n, " of file '" + f + "'");
    }

    void CxxTokenizer::parseString(const std::string& s) {
      std::istringstream iss(s);
      Token::size_type n{0};
      this->parseStream(iss, n, " of string '" + s + "'");
    }  // end of CxxTokenizer::parseOneString

    void CxxTokenizer::parseStream(std::istream& in,
                                   Token::size_type& n,
                                   const std::string& from) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "CxxTokenizer::parseStream: " + m);
      };
      auto get_line = [&throw_if](std::istream& file,
                                  Token::size_type& ln) -> std::string {
        auto line = std::string{};
        auto c = true;
        while (c) {
          throw_if(file.eof(), "unexpected end of stream");
          throw_if(!file.good(), "error while parsing stream");
          auto nl = std::string{};
          std::getline(file, nl);
          ++ln;
          if (nl.empty()) {
            c = false;
          } else {
            line += nl;
            if (line.back() == '\\') {
              line.pop_back();
            } else {
              c = false;
            }
          }
        }
        return line;
      };
      this->clear();
      try {
        while (!in.eof()) {
          throw_if(!in.good(), "error while parsing stream");
          auto line = get_line(in, n);
          try {
            this->splitLine(line, n);
          } catch (std::runtime_error& e) {
            raise<std::runtime_error>(std::string(e.what()) + ".\n" +
                                      "Error at line: " + std::to_string(n) +
                                      from);
          }
        }
      } catch (...) {
        this->clear();
        throw;
      }
    }

    struct is_cxx_separator {
      explicit is_cxx_separator(const bool bd, const bool bm)
          : treat_dot_as_separator(bd), treat_minus_as_separator(bm) {}
      bool operator()(const std::string::value_type& c) const {
        using ctype = std::string::value_type;
        constexpr const std::array<ctype, 24> s = {
            {'?', ';', '/', '!', '&', '*', '|', '{', '}',  '[',  ']', '(',
             ')', '%', '=', '^', ',', ':', '<', '>', '\'', '\"', '+', '\\'}};
        if (c == '.') {
          return this->treat_dot_as_separator;
        }
        if (c == '-') {
          return this->treat_minus_as_separator;
        }
        return std::find(std::begin(s), std::end(s), c) != std::end(s);
      }

     protected:
      bool treat_dot_as_separator = true;
      bool treat_minus_as_separator = true;
    };

    struct is_cxx_separator_or_space : public is_cxx_separator {
      explicit is_cxx_separator_or_space(const bool bd, const bool bm)
          : is_cxx_separator(bd, bm) {}
      bool operator()(const std::string::value_type& c) const {
        return ((std::isspace(c)) || (is_cxx_separator::operator()(c)));
      }
    };  // end of struct is_cxx_separator_or_space

    static void advance(Token::size_type& o,
                        std::string::const_iterator& p,
                        Token::size_type n) {
      o += n;
      std::advance(p, n);
    }

    static void advance(Token::size_type& o,
                        std::string::const_iterator& p,
                        const std::string::const_iterator np) {
      o += Token::size_type(np - p);
      p = np;
    }

    static void ignore_space(Token::size_type& o,
                             std::string::const_iterator& p,
                             const std::string::const_iterator pe) {
      auto np = std::find_if_not(p, pe, [](const std::string::value_type c) {
        return std::isspace(c);
      });
      advance(o, p, np);
    }

    void CxxTokenizer::treatChar(Token::size_type& o,
                                 std::string::const_iterator& p,
                                 const std::string::const_iterator pe,
                                 const Token::size_type n) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "CxxTokenizer::treatCChar: " + m);
      };
      if (this->charAsString) {
        this->treatString(o, p, pe, n, '\'');
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

    void CxxTokenizer::treatString(Token::size_type& o,
                                   std::string::const_iterator& p,
                                   const std::string::const_iterator pe,
                                   const Token::size_type n,
                                   const char e) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "CxxTokenizer::treatString: " + m);
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
      const auto pn = std::next(p, 1);
      if ((pn != pe) && (*pn == c)) {
        this->tokens.emplace_back(std::string{*p, *pn}, n, o, Token::Standard);
        advance(o, p, 2u);
      } else {
        this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
        advance(o, p, 1u);
      }
    }

    void CxxTokenizer::try_join(Token::size_type& o,
                                std::string::const_iterator& p,
                                const std::string::const_iterator pe,
                                const Token::size_type n,
                                const char c1,
                                const char c2) {
      const auto pn = std::next(p, 1);
      if ((pn != pe) && ((*pn == c1) || (*pn == c2))) {
        this->tokens.emplace_back(std::string{*p, *pn}, n, o, Token::Standard);
        advance(o, p, 2u);
      } else {
        this->tokens.emplace_back(std::string(1u, *p), n, o, Token::Standard);
        advance(o, p, 1u);
      }
    }

    void CxxTokenizer::readNumber(Token::size_type& o,
                                  std::string::const_iterator& p,
                                  const std::string::const_iterator pe,
                                  const Token::size_type n) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "CxxTokenizer::readNumber: " + m);
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
      throw_if((!(isdigit(*p))) && (*p != '.'),
               std::string("expected digit, read '") + *p + '\'');
      if (*p == '.') {
        is_float = true;
        throw_if(++p == pe, "invalid number");
        throw_if(!isdigit(*p),
                 std::string("expected digit, read '") + *p + '\'');
      } else if (*p == '0') {
        starts_with_zero = true;
        if (++p != pe) {
          if (*p == 'b') {
            // reading binary integer
            is_binary_integer = true;
            throw_if(++p == pe, "invalid binary integer");
            throw_if(!is_binary(*p), "invalid binary integer");
            while ((p != pe) && (isdigit(*p))) {
              throw_if(!is_binary(*p), "invalid binary integer");
              ++p;
            }
          } else if (*p == 'x') {
            // reading hex integer
            is_hex_integer = true;
            throw_if(++p == pe, "invalid hexadecimal integer");
            throw_if(!is_hex(*p), "invalid hexadecimal integer");
            while ((p != pe) && (isdigit(*p))) {
              throw_if(!is_hex(*p), "invalid hexadecimal integer");
              ++p;
            }
          } else {
            --p;
          }
        }
      }
      while ((p != pe) && ((isdigit(*p)) || (*p == '\''))) {
        if (*p == '\'') {
          throw_if(++p == pe, "invalid number");
          throw_if(!isdigit(*p),
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
        while ((p != pe) && ((isdigit(*p)) || (*p == '\''))) {
          if (*p == '\'') {
            // C++14 digit separator
            throw_if(++p == pe, "invalid number");
            throw_if(!isdigit(*p),
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
          throw_if(!(isdigit(*p)), "invalid number");
          ++p;
          while ((p != pe) && ((isdigit(*p)) || (*p == '\''))) {
            if (*p == '\'') {
              throw_if(++p == pe, "invalid number");
              throw_if(!isdigit(*p),
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
              };
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
          if (!((isalpha(*p)) || isdigit(*p) || (*p == '_'))) {
            break;
          }
          ++p;
        }
      }
      throw_if((p != pe) && (*p == '.'), "invalid number");
      this->tokens.emplace_back(std::string{b, p}, n, o, Token::Standard);
      const auto d = p - b;
      p = b;
      advance(o, p, d);
    }  // end of CxxTokenizer::readNumber

    void CxxTokenizer::treatCComment(Token::size_type& o,
                                     std::string::const_iterator& p,
                                     const std::string::const_iterator pe,
                                     const Token::size_type n) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "CxxTokenizer::treatCComment: " + m);
      };
      auto get_end =
          [&pe](std::string::const_iterator e) -> std::string::const_iterator {
        while (e != pe) {
          if (*e == '*') {
            const auto en = std::next(e);
            if (en == pe) {
              return pe;
            } else if (*en == '/') {
              return e;
            }
          }
          ++e;
        }
        return pe;
      };
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
      ignore_space(o, p, pe);
      auto e = get_end(p);
      this->tokens.emplace_back(std::string{p, e}, n, o, flag);
      advance(o, p, e - p);
      if (e == pe) {
        this->cStyleCommentOpened = true;
      } else {
        advance(o, p, 2u);
      }
    }

    void CxxTokenizer::treatCxxComment(Token::size_type& o,
                                       std::string::const_iterator& p,
                                       const std::string::const_iterator pe,
                                       const Token::size_type n) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "CxxTokenizer::treatCxxComment: " + m);
      };
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
      ignore_space(o, p, pe);
      this->tokens.emplace_back(std::string{p, pe}, n, o, flag);
      advance(o, p, pe - p);
    }

    void CxxTokenizer::treatPreprocessorDirective(
        Token::size_type& o,
        std::string::const_iterator& p,
        const std::string::const_iterator,
        const std::string::const_iterator pe,
        const Token::size_type n) {
      auto is_preprocessor_keyword = [](const std::string& k) {
        const std::array<const char*, 11> keys = {
            {"#define", "#undef", "#include", "#line", "#error", "#if",
             "#ifdef", "#ifndef", "elif", "#else", "#endif"}};
        return std::find(keys.begin(), keys.end(), k) != keys.end();
      };
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "CxxTokenizer::treatPreprocessorDirective: " + m);
      };
      // preprocessor
      throw_if((p == pe) || (*p != '#'), "invalid call");
      this->tokens.emplace_back(std::string(1u, '#'), n, o,
                                Token::Preprocessor);
      advance(o, p, 1);
      ignore_space(o, p, pe);
      throw_if(p == pe, "lonely ‘#’");
      auto pn = std::find_if(p, pe,
                             is_cxx_separator_or_space(this->dotAsSeparator,
                                                       this->minusAsSeparator));
      throw_if(p == pn, "unexpected token '" + std::string(1u, *p) + "'");
      const auto key = std::string{p, pn};
      throw_if(!is_preprocessor_keyword('#' + key),
               "invalid preprocessor keyword '" + key + "'");
      this->tokens.emplace_back(std::string{p, pn}, n, o, Token::Preprocessor);
      advance(o, p, pn);
      // this->treatStandardLine(o,p,b,pe,n);
      if (p != pe) {
        this->tokens.emplace_back(std::string{p, pe}, n, o, Token::Standard);
        advance(o, p, pe);
      }
    }  // end of CxxTokenizer::treatPreprocessorDirective

    void CxxTokenizer::treatStandardLine(Token::size_type& o,
                                         std::string::const_iterator& p,
                                         const std::string::const_iterator b,
                                         const std::string::const_iterator pe,
                                         const Token::size_type n) {
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "CxxTokenizer::treatStandardLine: " + m);
      };
      ignore_space(o, p, pe);
      while (p != pe) {
        if (*p == '#') {
          throw_if(true, "stray ‘#’");
        } else if (*p == '\\') {
          throw_if(true, "stray ‘\\’");
        } else if (std::isdigit(*p)) {
          this->readNumber(o, p, pe, n);
        } else if (*p == '\"') {
          // c-strings
          this->treatString(o, p, pe, n, '\"');
        } else if (*p == '\'') {
          // c-char
          this->treatChar(o, p, pe, n);
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
                      (is_cxx_separator_or_space(
                          this->dotAsSeparator,
                          this->minusAsSeparator)(*(std::prev(p))))) &&
                     ((pn != pe) && ((*pn == '.') || (std::isdigit(*pn))))) {
            this->readNumber(o, p, pe, n);
          } else {
            this->try_join(o, p, pe, n, '=');
          }
        } else if (*p == '/') {
          auto pn = std::next(p);
          if ((pn != pe) && (*pn == '/')) {
            this->treatCxxComment(o, p, pe, n);
          } else if ((pn != pe) && (*pn == '*')) {
            this->treatCComment(o, p, pe, n);
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
            this->readNumber(o, p, pe, n);
          } else {
            this->try_join(o, p, pe, n, '.', '*');
          }
        } else if (*p == '|') {
          this->try_join(o, p, pe, n, '|', '=');
        } else {
          auto pw =
              std::find_if(p, pe,
                           is_cxx_separator_or_space(this->dotAsSeparator,
                                                     this->minusAsSeparator));
          if (p == pw) {
            this->tokens.emplace_back(std::string(1u, *p), n, Token::Standard);
            advance(o, p, 1u);
          } else {
            this->tokens.emplace_back(std::string{p, pw}, n, Token::Standard);
            advance(o, p, pw);
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
        throw_if(
            (this->tokens.empty()) ||
                ((this->tokens.back().flag != Token::Comment) &&
                 (this->tokens.back().flag != Token::DoxygenComment) &&
                 (this->tokens.back().flag != Token::DoxygenBackwardComment)),
            "internal error (previous token is not a comment)");
        if (!this->tokens.back().value.empty()) {
          this->tokens.back().value += '\n';
        }
        if (pos3 == std::string::npos) {
          this->tokens.back().value += line;
          //	    stripSpaceAndStarAtBeginningOfCommentLine(line);
          return;
        }
        advance(o, p, pos3 + 2);
        this->cStyleCommentOpened = false;
      }
      ignore_space(o, p, pe);
      if ((p != pe) && (*p == '#')) {
        this->treatPreprocessorDirective(o, p, b, pe, n);
      } else {
        this->treatStandardLine(o, p, b, pe, n);
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

    void CxxTokenizer::printFileTokens(std::ostream& out) const {
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

    bool CxxTokenizer::isValidIdentifier(const std::string& s, const bool b) {
      if (s.empty()) {
        return false;
      }
      auto p = s.begin();
      if (isdigit(*p)) {
        return false;
      }
      for (; p != s.end(); ++p) {
        if ((!isalpha(*p)) && (!(isdigit(*p))) && (*p != '_')) {
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

    bool CxxTokenizer::isValidFunctionIdentifier(const std::string& s,
                                                 const bool checkCxxKeywords) {
      if (s.empty()) {
        return false;
      }
      auto p = s.begin();
      if (isdigit(*p)) {
        return false;
      }
      for (; p != s.end(); ++p) {
        if ((!isalpha(*p)) && (!(isdigit(*p))) && (*p != '_') && (*p != ':')) {
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
          if ((!isalpha(*p)) && (!(isdigit(*p))) && (*p != '_')) {
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
      raise_if<std::out_of_range>(i >= this->size(),
                                  "CxxTokenizer::operator[]");
      const auto p = std::next(this->begin(), i);
      return *p;
    }  // end of CxxTokenizer::begin

    CxxTokenizer::const_iterator CxxTokenizer::begin() const {
      return this->tokens.begin();
    }  // end of CxxTokenizer::begin

    CxxTokenizer::const_iterator CxxTokenizer::end() const {
      return this->tokens.end();
    }  // end of CxxTokenizer::end

    void CxxTokenizer::setCStyleCommentOpened(const bool b) {
      this->cStyleCommentOpened = b;
    }

    void CxxTokenizer::clear() {
      this->cStyleCommentOpened = false;
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
      CxxTokenizer::checkNotEndOfLine(method, "expected '" + value + "'", p,
                                      pe);
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
      const auto value = p->value.substr(1, p->value.size() - 2);
      ++p;
      return value;
    }  // end of CxxTokenizer::readString

    double CxxTokenizer::readDouble(const_iterator& p, const const_iterator pe)

    {
      double res;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readDouble",
                                      "expected number", p, pe);
#ifdef __CYGWIN__
      std::istringstream is(p->value);
      is >> res;
      raise_if(!is && (!is.eof()),
               "CxxTokenizer::readInt: "
               "could not read value from token '" +
                   p->value + "'.");
#else
      try {
        res = convert<double>(p->value);
      } catch (std::exception& e) {
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

    int CxxTokenizer::readInt(const_iterator& p, const const_iterator pe)

    {
      int res;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readInt",
                                      "expected number", p, pe);
      std::istringstream is(p->value);
      is >> res;
      raise_if(!is && (!is.eof()),
               "CxxTokenizer::readInt: "
               "could not read value from token '" +
                   p->value + "'.");
      ++p;
      return res;
    }  // end of CxxTokenizer::readInt

    unsigned int CxxTokenizer::readUnsignedInt(const_iterator& p,
                                               const const_iterator pe) {
      unsigned int res;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readUnsignedInt",
                                      "expected number", p, pe);
      std::istringstream is(p->value);
      is >> res;
      raise_if(!is && (!is.eof()),
               "CxxTokenizer::readUnsignedInt: "
               "could not read value from token '" +
                   p->value + "'.\n");
      ++p;
      return res;
    }  // end of CxxTokenizer::readUnsignedInt

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

    CxxTokenizer::size_type CxxTokenizer::size() const {
      return this->tokens.size();
    }  // end of CxxTokenizer::size

    CxxTokenizer::~CxxTokenizer() = default;

  }  // end of namespace utilities

}  // end of namespace tfel
