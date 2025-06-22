/*!
 * \file   src/Utilities/Data.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   20 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/Data.hxx"

namespace tfel::utilities {

  Data::Data() = default;
  Data::Data(const Data&) = default;
  Data::Data(Data&&) = default;
  Data& Data::operator=(const Data&) = default;
  Data& Data::operator=(Data&&) = default;
  Data::~Data() = default;

  DataStructure::DataStructure() = default;
  DataStructure::DataStructure(const DataStructure&) = default;
  DataStructure::DataStructure(DataStructure&&) = default;
  DataStructure& DataStructure::operator=(const DataStructure&) = default;
  DataStructure& DataStructure::operator=(DataStructure&&) = default;
  DataStructure::~DataStructure() = default;

  DataParsingOptions::DataParsingOptions() = default;
  DataParsingOptions::DataParsingOptions(const DataParsingOptions&) = default;
  DataParsingOptions::DataParsingOptions(DataParsingOptions&&) = default;
  DataParsingOptions& DataParsingOptions::operator=(const DataParsingOptions&) =
      default;
  DataParsingOptions& DataParsingOptions::operator=(DataParsingOptions&&) =
      default;
  DataParsingOptions::~DataParsingOptions() = default;

  static void read_map(std::map<std::string, Data>& r,
                       CxxTokenizer::const_iterator& p,
                       const CxxTokenizer::const_iterator pe,
                       const DataParsingOptions& o) {
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b, "Data::read_map: " + msg);
    };
    CxxTokenizer::checkNotEndOfLine("Data::read_map", p, pe);
    CxxTokenizer::readSpecifiedToken("Data::read_map", "{", p, pe);
    CxxTokenizer::checkNotEndOfLine("Data::read_map", p, pe);
    std::vector<std::string> mkeys;  // keys with multiple values;
    while (p->value != "}") {
      const auto k = [&p, pe]() -> std::string {
        if (p->flag != Token::String) {
          auto c = p;
          ++p;
          return c->value;
        }
        return CxxTokenizer::readString(p, pe);
      }();
      CxxTokenizer::readSpecifiedToken("Data::read_map", ":", p, pe);
      const auto v = Data::read(p, pe, o);
      if (p->value == ",") {
        CxxTokenizer::readSpecifiedToken("Data::read_map", ",", p, pe);
        CxxTokenizer::checkNotEndOfLine("Data::read_map", p, pe);
        throw_if(p->value == "}", "unexpected token '}'");
      } else {
        throw_if(p->value != "}", "unexpected token '" + p->value + "'");
      }
      const auto pv = r.find(k);
      if (pv == r.end()) {
        r.insert({k, v});
      } else {
        throw_if(!o.allowMultipleKeysInMap, "key '" + k + "' multiply defined");
        if (std::find(mkeys.begin(), mkeys.end(), k) == mkeys.end()) {
          std::vector<Data> mdata;
          mdata.push_back(pv->second);
          mdata.push_back(v);
          pv->second = Data(std::move(mdata));
          mkeys.push_back(k);
        } else {
          auto& vd = pv->second.get<std::vector<Data>>();
          vd.push_back(v);
        }
      }
    }
    CxxTokenizer::readSpecifiedToken("Data::read_map", "}", p, pe);
  }

  Data Data::read_value(CxxTokenizer::const_iterator& p,
                        const CxxTokenizer::const_iterator pe,
                        const DataParsingOptions& o) {
    auto throw_if = [](const bool b, const std::string& msg) {
      if (b) {
        tfel::raise("Data::read: " + msg);
      }
    };
    auto is_integer = [&throw_if](const std::string& s) {
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
    auto readDataStructureOrString = [&o, &p, pe](const std::string& name) {
      if (p != pe) {
        if (p->value == "{") {
          DataStructure ds;
          ds.name = name;
          tfel::utilities::read_map(ds.data, p, pe, o);
          return Data(std::move(ds));
        }
      }
      return Data(name);
    };
    if (p->value == "true") {
      ++p;
      return Data(true);
    } else if (p->value == "false") {
      ++p;
      return Data(false);
    } else if (p->flag == Token::String) {
      const auto name = CxxTokenizer::readString(p, pe);
      return readDataStructureOrString(name);
    } else if (CxxTokenizer::isValidIdentifier(p->value)) {
      const auto name = p->value;
      ++p;
      return readDataStructureOrString(name);
    } else if (is_integer(p->value)) {
      std::size_t pos;
      auto v = std::stoi(p->value, &pos);
      throw_if(pos != p->value.size(), "invalid number '" + p->value + "'");
      ++p;
      return Data(v);
    }
    const auto r = convert<double>(p->value);
    ++p;
    return Data(r);
  }

  Data Data::read_vector(CxxTokenizer::const_iterator& p,
                         const CxxTokenizer::const_iterator pe,
                         const DataParsingOptions& o) {
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b, "Data::read_vector: " + msg);
    };
    CxxTokenizer::checkNotEndOfLine("Data::read_vector", p, pe);
    CxxTokenizer::readSpecifiedToken("Data::read_vector", "{", p, pe);
    CxxTokenizer::checkNotEndOfLine("Data::read_vector", p, pe);
    std::vector<Data> v;
    while (p->value != "}") {
      v.push_back(Data::read(p, pe, o));
      if (p->value == ",") {
        CxxTokenizer::readSpecifiedToken("Data::read_vector", ",", p, pe);
        CxxTokenizer::checkNotEndOfLine("Data::read_vector", p, pe);
        throw_if(p->value == "}", "unexpected token '}'");
      } else {
        throw_if(p->value != "}", "unexpected token '" + p->value + "'");
      }
    }
    CxxTokenizer::readSpecifiedToken("Data::read_vector", "}", p, pe);
    return std::move(v);
  }

  Data Data::read_map(CxxTokenizer::const_iterator& p,
                      const CxxTokenizer::const_iterator pe,
                      const DataParsingOptions& opts) {
    auto c = p;
    CxxTokenizer::checkNotEndOfLine("Data::read", c, pe);
    CxxTokenizer::readSpecifiedToken("Data::read_map", "{", c, pe);
    CxxTokenizer::checkNotEndOfLine("Data::read", c, pe);
    if (c->value == "}") {
      return std::map<std::string, Data>{};
    }
    if (c->flag == Token::Number) {
      p = c;
      std::map<double, double> values;
      while (p->value != "}") {
        const auto a = convert<double>(p->value);
        ++p;
        CxxTokenizer::readSpecifiedToken("Data::read_map", ":", p, pe);
        const auto o = convert<double>(p->value);
        ++p;
        values.insert({a, o});
        CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
        if ((p->value != ",") && (p->value != "}")) {
          tfel::raise("Data::read_map: unexpected token '" + p->value + "'");
        }
        if (p->value == ",") {
          ++p;
          CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
          if (p->value == "}") {
            tfel::raise("Data::read_map: unexpected token '}'");
          }
        }
      }
      CxxTokenizer::readSpecifiedToken("Data::read_map", "}", p, pe);
      return values;
    }
    std::map<std::string, Data> r;
    tfel::utilities::read_map(r, p, pe, opts);
    return std::move(r);
  }

  Data Data::read(CxxTokenizer::const_iterator& p,
                  const CxxTokenizer::const_iterator pe,
                  const DataParsingOptions& o) {
    CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
    if (p->value == "{") {
      ++p;
      // vector or map
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      if (p->value == "}") {
        ++p;
        return Data();
      }
      // map or string vector
      ++p;
      CxxTokenizer::checkNotEndOfLine("Data::read", p, pe);
      if (p->value == ":") {
        p = std::prev(p, 2);
        return read_map(p, pe, o);
      }
      if ((p->value != ",") && (p->value != "}")) {
        raise("expected ',' or ':' or '}', read '" + p->value + "'");
      }
      p = std::prev(p, 2);
      return read_vector(p, pe, o);
    }
    // standard value
    return read_value(p, pe, o);
  }  // end of Data::read

  void Data::parse(CxxTokenizer::const_iterator& p,
                   const CxxTokenizer::const_iterator pe,
                   const std::map<std::string, CallBack>& callbacks,
                   const DataParsingOptions& o) {
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b, "Data::parse: " + msg);
    };
    auto get = [&callbacks, throw_if](const std::string& k) {
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
        c(Data::read(p, pe, o));
      } catch (std::exception& e) {
        raise<std::runtime_error>(
            "Data::read: error while "
            "treating key '" +
            k + "'.\n" + std::string(e.what()));
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
      return tfel::math::ieee754::fpclassify(v1 - v2) == FP_ZERO;
    }  // end of apply
  };   // end of struct DataComparator

  bool operator==(const Data& lhs, const Data& rhs) {
    if (&lhs == &rhs) {
      return true;
    }
    return apply<DataComparator>(lhs, rhs);
  }

}  // end of namespace tfel::utilities
