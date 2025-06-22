/*!
 * \file   src/Utilities/Data.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   20 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */
#include <iostream>
#include <algorithm>

#include "TFEL/Utilities/Data.hxx"

namespace tfel {

  namespace utilities {

    Data::Data() = default;
    Data::Data(const Data&) = default;
    Data::Data(Data&&) = default;
    Data& Data::operator=(const Data&) = default;
    Data& Data::operator=(Data&&) = default;
    Data::~Data() = default;

    Data Data::read_value(CxxTokenizer::const_iterator& p,
                          const CxxTokenizer::const_iterator pe) {
      auto throw_if = [](const bool b, const std::string& msg) {
        if (b) {
          throw(std::runtime_error("Data::read: " + msg));
        }
      };
      auto is_integer = [&throw_if, &pe](const std::string& s) {
        auto ps = s.cbegin();
        const auto pse = s.cend();
        if (*ps == '-') {
          ++ps;
        }
        throw_if(ps == pse, "invalid number '" + s + "'");
        while (ps != pse) {
          if (!::isdigit(*ps)) {
            return false;
          }
          ++ps;
        }
        return true;
      };
      if (p->value == "true") {
        ++p;
        return Data(true);
      }
      if (p->value == "false") {
        ++p;
        return Data(false);
      }
      if (p->flag == Token::String) {
        return Data(CxxTokenizer::readString(p, pe));
      }
      if (is_integer(p->value)) {
        std::size_t pos;
        auto v = std::stoi(p->value, &pos);
        throw_if(pos != p->value.size(), "invalid number '" + p->value + "'");
        ++p;
        return Data(v);
      }
      std::size_t pos;
      const double r = std::stod(p->value, &pos);
      throw_if(pos != p->value.size(), "invalid number '" + p->value + "'");
      ++p;
      return Data(r);
    }

    Data Data::read_vector(CxxTokenizer::const_iterator& p,
                           const CxxTokenizer::const_iterator pe) {
      auto throw_if = [](const bool b, const std::string& msg) {
        if (b) {
          throw(std::runtime_error("Data::read: " + msg));
        }
      };
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      CxxTokenizer::readSpecifiedToken("Data::read", "{", p, pe);
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      std::vector<Data> v;
      while (p->value != "}") {
        v.push_back(Data::read(p, pe));
        if (p->value == ",") {
          CxxTokenizer::readSpecifiedToken("Data::read", ",", p, pe);
          CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
          throw_if(p->value == "}", "unexpected token '}'");
        } else {
          throw_if(p->value != "}", "unexpected token '" + p->value + "'");
        }
      }
      CxxTokenizer::readSpecifiedToken("Data::read", "}", p, pe);
      return std::move(v);
    }

    Data Data::read_map(CxxTokenizer::const_iterator& p,
                        const CxxTokenizer::const_iterator pe) {
      auto throw_if = [](const bool b, const std::string& msg) {
        if (b) {
          throw(std::runtime_error("Data::read: " + msg));
        }
      };
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      CxxTokenizer::readSpecifiedToken("Data::read", "{", p, pe);
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      std::map<std::string, Data> r;
      while (p->value != "}") {
        const auto k = [&p, pe]() -> std::string {
          if (p->flag != Token::String) {
            auto c = p;
            ++p;
            return c->value;
          }
          return CxxTokenizer::readString(p, pe);
        }();
        CxxTokenizer::readSpecifiedToken("Data::read", ":", p, pe);
        const auto v = Data::read(p, pe);
        if (p->value == ",") {
          CxxTokenizer::readSpecifiedToken("Data::read", ",", p, pe);
          CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
          throw_if(p->value == "}", "unexpected token '}'");
        } else {
          throw_if(p->value != "}", "unexpected token '" + p->value + "'");
        }
        throw_if(!r.insert({k, v}).second, "key '" + k + "' multiply defined");
      }
      CxxTokenizer::readSpecifiedToken("Data::read", "}", p, pe);
      return std::move(r);
    }

    Data Data::read(CxxTokenizer::const_iterator& p,
                    const CxxTokenizer::const_iterator pe) {
      auto throw_if = [](const bool b, const std::string& msg) {
        if (b) {
          throw(std::runtime_error("Data::read: " + msg));
        }
      };
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      if (p->value == "{") {
        ++p;
        // vector or map
        CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
        // map or string vector
        ++p;
        CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
        if (p->value == ":") {
          p = std::prev(p, 2);
          return read_map(p, pe);
        } else {
          throw_if((p->value != ",") && (p->value != "}"),
                   "expected ',' or ':' or '}', read '" + p->value + "'");
          p = std::prev(p, 2);
          return read_vector(p, pe);
        }
      } else {
        // standard value
        return read_value(p, pe);
      }
    }

    void Data::parse(CxxTokenizer::const_iterator& p,
                     const CxxTokenizer::const_iterator pe,
                     const std::map<std::string, CallBack>& callbacks) {
      auto throw_if = [](const bool b, const std::string& msg) {
        if (b) {
          throw(std::runtime_error("Data::parse: " + msg));
        }
      };
      auto get = [&callbacks, throw_if](const std::string& k) {
        std::for_each(callbacks.begin(), callbacks.end(),
                      [](std::map<std::string, CallBack>::const_reference& v) {
                        std::cout << "'" << v.first << "'" << std::endl;
                      });
        const auto pc = callbacks.find(k);
        throw_if(pc == callbacks.end(), "unsupported key '" + k + "'");
        return pc->second;
      };
      CxxTokenizer::readSpecifiedToken("Data::parse", "{", p, pe);
      CxxTokenizer::checkNotEndOfLine("Data::parse", p, pe);
      while (p->value != "}") {
        const auto k = [&p, pe]() -> std::string {
          if (p->flag != Token::String) {
            auto c = p;
            ++p;
            return c->value;
          }
          return CxxTokenizer::readString(p, pe);
        }();
        auto c = get(k);
        CxxTokenizer::readSpecifiedToken("Data::parse", ":", p, pe);
        CxxTokenizer::checkNotEndOfLine("Data::parse", p, pe);
        try {
          c(Data::read(p, pe));
        } catch (std::exception& e) {
          throw(std::runtime_error("Data::read: error while treating key '" +
                                   k + "'.\n" + std::string(e.what())));
        }
        CxxTokenizer::checkNotEndOfLine("Data::parse", p, pe);
        if (p->value != "}") {
          CxxTokenizer::readSpecifiedToken("Data::parse", ",", p, pe);
          CxxTokenizer::checkNotEndOfLine("Data::parse", p, pe);
          throw_if(p->value == "}", "unexpected token '" + p->value + "'");
        }
      }
      CxxTokenizer::readSpecifiedToken("Data::parse", "}", p, pe);
    }  // end of Data::read

    struct TFEL_VISIBILITY_LOCAL DataComparator {
      using return_type = bool;
      // default implementation
      template <typename T1, typename T2>
      static typename std::enable_if<!std::is_same<T1, T2>::value, bool>::type
      apply(const T1&, const T2&) {
        return false;
      }  // end of apply
      static bool apply(const std::string& x1, const std::string& x2) {
        return x1 == x2;
      }  // end of apply
      static bool apply(const int x1, const int x2) {
        return x1 == x2;
      }  // end of apply
      static bool apply(const Data& x1, const Data& x2) {
        return x1 == x2;
      }  // end of apply
      static bool apply(const std::vector<Data>& v1,
                        const std::vector<Data>& v2) {
        bool e = true;
        if (v1.size() != v2.size()) {
          return false;
        }
        auto p1 = v1.begin();
        auto p2 = v2.begin();
        for (; (p1 != v1.end()) && (e); ++p1, ++p2) {
          e = DataComparator::apply(*p1, *p2);
        }
        return e;
      }  // end of apply
      static bool apply(const std::map<std::string, Data>& v1,
                        const std::map<std::string, Data>& v2) {
        bool e = true;
        if (v1.size() != v2.size()) {
          return false;
        }
        auto p1 = v1.begin();
        auto p2 = v2.begin();
        for (; (p1 != v1.end()) && (e); ++p1, ++p2) {
          e = ((DataComparator::apply(p1->first, p2->first)) &&
               (DataComparator::apply(p1->second, p2->second)));
        }
        return e;
      }  // end of apply
      static bool apply(const double v1, const double v2) {
        return v1 == v2;
      }  // end of apply
    };   // end of struct DataComparator

    bool operator==(const Data& lhs, const Data& rhs) {
      if (&lhs == &rhs) {
        return true;
      }
      return apply<DataComparator>(lhs, rhs);
    }

    /*!
     * \param[in] d: data extract a vector of string from a Data
     *               structure.
     * \return the extracted value.
     * \pre The Data must hold a vector of Data and each Data of the
     *      vector must be a string
     */
    template <>
    std::vector<std::string> extract<std::vector<std::string>>(const Data& d) {
      auto throw_if = [](const bool b, const std::string& msg) {
        if (b) {
          throw(
              std::runtime_error("extract<std::vector<std::string>>: " + msg));
        }
      };
      throw_if(!d.is<std::vector<Data>>(), "invalid data type");
      const auto& vd = d.get<std::vector<Data>>();
      auto r = std::vector<std::string>{};
      r.reserve(vd.size());
      for (const auto& w : vd) {
        throw_if(!w.is<std::string>(), "invalid data type");
        r.push_back(w.get<std::string>());
      }
      return r;
    }  // end of extract<std::vector<std::string>>

  }  // end of namespace utilities

}  // end of namespace tfel
