/*!
 * \file   src/Math/EvaluatorBase.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

#include "TFEL/Math/Parser/EvaluatorBase.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      std::string EvaluatorBase::readNumber(
          std::string::const_iterator& p,
          const std::string::const_iterator pe) {
        std::string word;
        std::string::const_iterator b = p;
        // reading decimal part
        while ((isdigit(*p)) && (p != pe)) {
          ++p;
        }
        if (p != pe) {
          if (*p == '.') {
            ++p;
            while ((isdigit(*p)) && (p != pe)) {
              ++p;
            }
          }
        }
        if (p != pe) {
          if ((*p == 'e') || (*p == 'E')) {
            ++p;
            if (p == pe) {
              --p;
            } else {
              if ((*p == '+') || (*p == '-')) {
                ++p;
                if (p == pe) {
                  --(--p);
                } else if (isdigit(*p)) {
                  while ((isdigit(*p)) && (p != pe)) {
                    ++p;
                  }
                } else {
                  --(--p);
                }
              } else if (isdigit(*p)) {
                while ((isdigit(*p)) && (p != pe)) {
                  ++p;
                }
              } else {
                --p;
              }
            }
          }
        }
        std::copy(b, p, std::back_inserter(word));
        return word;
      }  // end of EvaluatorBase::readNumber

      inline void tokenize(std::vector<std::string>& res,
                           std::string::const_iterator& b,
                           std::string::const_iterator& p2,
                           const char c) {
        char w[2];
        w[0] = c;
        w[1] = '\0';
        std::string word;
        if (p2 != b) {
          std::copy(b, p2, std::back_inserter(word));
          res.push_back(word);
        }
        res.emplace_back(w);
        ++p2;
        b = p2;
      }

      void EvaluatorBase::splitAtTokenSeperator(std::vector<std::string>& t) {
        auto throw_if = [](const bool b, const std::string& m) {
          if (b) {
            throw(std::runtime_error("EvaluatorBase::splitAtTokenSeperator: " +
                                     m));
          }
        };
        auto res = std::vector<std::string>{};
        for (const auto& w : t) {
          auto b = w.begin();
          auto p2 = w.begin();
          const auto p2e = w.end();
          while (p2 != p2e) {
            if (isdigit(*p2) && (p2 == b)) {
              res.push_back(readNumber(p2, p2e));
              b = p2;
            } else if (*p2 == '+') {
              tokenize(res, b, p2, '+');
            } else if (*p2 == '!') {
              tokenize(res, b, p2, '!');
            } else if (*p2 == '-') {
              tokenize(res, b, p2, '-');
            } else if (*p2 == '*') {
              if (p2 != b) {
                res.emplace_back(b, p2);
              }
              ++p2;
              if (p2 != p2e) {
                if (*p2 == '*') {
                  res.emplace_back("**");
                  ++p2;
                } else {
                  res.emplace_back("*");
                }
              } else {
                res.emplace_back("*");
              }
              b = p2;
            } else if (*p2 == '|') {
              if (p2 != b) {
                res.emplace_back(b, p2);
              }
              ++p2;
              throw_if(p2 == p2e, "unexpected end of string");
              throw_if(*p2 != '|',
                       "expected character '|' "
                       "(read '" +
                           std::string(1u, *p2) + "')");
              res.emplace_back("||");
              ++p2;
              b = p2;
            } else if (*p2 == '&') {
              if (p2 != b) {
                res.emplace_back(b, p2);
              }
              ++p2;
              throw_if(p2 == p2e, "unexpected end of string");
              throw_if(*p2 != '&',
                       "expected character "
                       "'&&' (read '" +
                           std::string(1u, *p2) + "')");
              res.emplace_back("&&");
              ++p2;
              b = p2;
            } else if (*p2 == '>') {
              if (p2 != b) {
                res.emplace_back(b, p2);
              }
              ++p2;
              if (p2 != p2e) {
                if (*p2 == '=') {
                  res.emplace_back(">=");
                  ++p2;
                } else {
                  res.emplace_back(">");
                }
              } else {
                res.emplace_back(">");
              }
              b = p2;
            } else if (*p2 == '<') {
              if (p2 != b) {
                res.emplace_back(b, p2);
              }
              ++p2;
              if (p2 != p2e) {
                if (*p2 == '=') {
                  res.emplace_back("<=");
                  ++p2;
                } else {
                  res.emplace_back("<");
                }
              } else {
                res.emplace_back("<");
              }
              b = p2;
            } else if (*p2 == '=') {
              if (p2 != b) {
                res.emplace_back(b, p2);
              }
              ++p2;
              if (p2 != p2e) {
                if (*p2 == '=') {
                  res.emplace_back("==");
                  ++p2;
                } else {
                  res.emplace_back("=");
                }
              } else {
                res.emplace_back("=");
              }
              b = p2;
            } else if (*p2 == '/') {
              tokenize(res, b, p2, '/');
            } else if (*p2 == '(') {
              tokenize(res, b, p2, '(');
            } else if (*p2 == ')') {
              tokenize(res, b, p2, ')');
            } else if (*p2 == ',') {
              tokenize(res, b, p2, ',');
            } else if (*p2 == '?') {
              tokenize(res, b, p2, '?');
            } else if (*p2 == ':') {
              tokenize(res, b, p2, ':');
            } else {
              ++p2;
            }
          }
          if (b != p2e) {
            res.emplace_back(b, p2);
          }
        }

        t.swap(res);
      }

      void EvaluatorBase::analyse(const std::string& expr) {
        std::istringstream tokenizer(expr);
        std::copy(std::istream_iterator<std::string>(tokenizer),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(this->tokens));
        EvaluatorBase::splitAtTokenSeperator(this->tokens);
      }  // end of EvaluatorBase::analyse

      EvaluatorBase::~EvaluatorBase() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
