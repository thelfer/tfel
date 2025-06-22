/*!
 * \file  src/Utilities/StringAlgorithms.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

namespace tfel::utilities {

  bool starts_with(std::string_view s1, std::string_view s2) {
    return ((s1.size() >= s2.size()) &&
            (std::equal(s2.begin(), s2.end(), s1.begin())));
  }  // end of starts_with

  bool ends_with(std::string_view s1, std::string_view s2) {
    return ((s1.size() >= s2.size()) &&
            (std::equal(s2.rbegin(), s2.rend(), s1.rbegin())));
  }  // end of ends_with

  std::vector<std::string> tokenize(std::string_view s,
                                    const char c,
                                    const bool keep_empty_strings) {
    std::vector<std::string> res;
    auto b = std::string::size_type{};
    auto e = s.find_first_of(c, b);
    while (std::string::npos != e || std::string::npos != b) {
      // Found a token, add it to the vector.
      res.push_back(std::string{s.substr(b, e - b)});
      if (keep_empty_strings) {
        b = e == std::string::npos ? e : e + 1;
      } else {
        b = s.find_first_not_of(c, e);
      }
      e = s.find_first_of(c, b);
    }
    return res;
  }  // end of tokenize

  std::vector<std::string> tokenize(std::string_view s, std::string_view d) {
    std::vector<std::string> res;
    const auto sl = s.length();
    const auto dl = d.length();
    auto b = std::string::size_type{};
    auto e = s.find(d, b);
    while (e != std::string::npos) {
      res.push_back(std::string{s.substr(b, e - b)});
      b = e + dl;
      e = s.find(d, b);
    }
    if (b != sl) {
      res.push_back(std::string{s.substr(b)});
    }
    return res;
  }  // end of tokenize

  std::string replace_all(std::string_view s,
                          std::string_view s1,
                          std::string_view s2,
                          const std::string::size_type ps) {
    std::string r;
    replace_all(r, s, s1, s2, ps);
    return r;
  }

  void replace_all(std::string& r,
                   std::string_view s,
                   std::string_view s1,
                   std::string_view s2,
                   const std::string::size_type ps) {
    using namespace std;
    string::size_type rs;
    string::size_type pos = ps;
    string::size_type p;
    r.clear();
    if (s.empty()) {
      return;
    }
    if (!s1.empty()) {
      p = s.find(s1, pos);
      while (p != string::npos) {
        rs = r.size();
        r.resize(rs + p - pos + s2.size());
        //	  copy(s.begin()+pos,s.begin()+p,r.begin()+rs);
        copy(&s[0] + pos, &s[0] + p, &r[0] + rs);
        copy(s2.begin(), s2.end(), &r[0] + rs + p - pos);
        pos = p + s1.size();
        p = s.find(s1, pos);
      }
    }
    rs = r.size();
    r.resize(rs + s.size() - pos);
    copy(&s[0] + pos, &s[0] + s.size(), &r[0] + rs);
  }

  std::string replace_all(std::string_view c, const char c1, const char c2) {
    std::string s(c);
    std::string::size_type p = 0u;
    if (s.empty()) {
      return "";
    }
    while ((p = s.find(c1, p)) != std::string::npos) {
      s[p] = c2;
      p += 1u;
    }
    return s;
  }

  void replace_all(std::string& s, const char c, std::string_view n) {
    std::string::size_type p = 0u;
    std::string::size_type ns = n.size();
    if (s.empty()) {
      return;
    }
    while ((p = s.find(c, p)) != std::string::npos) {
      s.replace(p, 1u, n);
      p += ns;
    }
  }

  template <>
  double convert<>(const std::string& s) {
    auto throw_if = [&s](const bool b) {
      raise_if<std::invalid_argument>(b,
                                      "tfel::utilities::convert: "
                                      "could not convert '" +
                                          s +
                                          "' "
                                          "to double");
    };
    throw_if(s.empty());
    auto p = std::size_t{};
    auto r = double{};
    try {
      r = std::stod(s, &p);
    } catch (std::exception&) {
      throw_if(true);
    }
    throw_if(p != s.size());
    return r;
  }

  template <>
  long double convert<>(const std::string& s) {
    auto throw_if = [&s](const bool b) {
      raise_if<std::invalid_argument>(b,
                                      "tfel::utilities::convert: "
                                      "could not convert '" +
                                          s +
                                          "' "
                                          "to long double");
    };
    throw_if(s.empty());
    auto p = std::size_t{};
    auto r = static_cast<long double>(0);
    try {
      r = std::stold(s, &p);
    } catch (std::exception&) {
      throw_if(true);
    }
    throw_if(p != s.size());
    return r;
  }

}  // namespace tfel::utilities
