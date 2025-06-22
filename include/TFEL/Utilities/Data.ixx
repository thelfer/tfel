/*!
 * \file   Data.ixx
 * \brief
 * \author Thomas Helfer
 * \date   25 oct. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_DATA_IXX
#define LIB_TFEL_UTILITIES_DATA_IXX

#include "TFEL/Raise.hxx"

namespace tfel::utilities::internals {

  template <typename T, bool c>
  struct DataConvertorImpl;

  template <typename T>
  struct DataConvertorImpl<T, true> {
    static T exe(const tfel::utilities::Data& d) {
      return d.template get<T>();
    }  // end of exe
    static bool is_convertible(const tfel::utilities::Data& d) {
      return d.template is<T>();
    }  // end of exe
  };   // end of DataConvertorImpl<T,true>;

  /*!
   * \brief an helper class (in C++-11, no `if constexpr`, so we
   * rely on template specialisation)
   */
  template <typename T>
  struct DataConvertor
      : public DataConvertorImpl<T, tfel::utilities::isDataType<T>::value> {
  };  // end of struct DataConvertor

  /*!
   * \brief an helper class (in C++-11, no `if constexpr`, so we
   * rely on template specialisation)
   */
  template <>
  struct DataConvertor<double> {
    static double exe(const Data& d) {
      if (d.is<int>()) {
        return d.get<int>();
      }
      return d.get<double>();
    }
    static bool is_convertible(const Data& d) {
      return d.is<int>() || d.is<double>();
    }
  };  // end of struct DataConvertor

  //! partial specialisation for `std::vector`
  template <typename T>
  struct DataConvertor<std::vector<T>> {
    static std::vector<T> exe(const Data& d) {
      auto throw_if = [](const bool b, const std::string& msg) {
        tfel::raise_if(b, "convert<std::vector<T>>: " + msg);
      };
      throw_if(!d.is<std::vector<Data>>(), "invalid data type");
      const auto& vd = d.get<std::vector<Data>>();
      auto r = std::vector<T>{};
      r.reserve(vd.size());
      for (const auto& w : vd) {
        r.push_back(tfel::utilities::convert<T>(w));
      }
      return r;
    }  // end of exe
    static bool is_convertible(const Data& d) {
      if (!d.is<std::vector<tfel::utilities::Data>>()) {
        return false;
      }
      const auto& vd = d.get<std::vector<tfel::utilities::Data>>();
      for (const auto& w : vd) {
        if (!tfel::utilities::is_convertible<T>(w)) {
          return false;
        }
      }
      return true;
    }
  };  // end of DataConvertor<std::vector<std::string>>

  //! partial specialisation for `std::map`
  template <typename T>
  struct DataConvertor<std::map<std::string, T>> {
    static std::map<std::string, T> exe(const Data& d) {
      auto throw_if = [](const bool b, const std::string& msg) {
        tfel::raise_if(b, "convert<std::map<std::string,T>>: " + msg);
      };
      throw_if(!d.is<std::map<std::string, Data>>(), "invalid data type");
      const auto& m = d.get<std::map<std::string, Data>>();
      auto r = std::map<std::string, T>{};
      for (const auto& e : m) {
        r.insert({e.first, tfel::utilities::convert<T>(e.second)});
      }
      return r;
    }  // end of exe
    static bool is_convertible(const Data& d) {
      if (!d.is<std::map<std::string, tfel::utilities::Data>>()) {
        return false;
      }
      const auto& vd = d.get<std::map<std::string, tfel::utilities::Data>>();
      for (const auto& w : vd) {
        if (!tfel::utilities::is_convertible<T>(w.second)) {
          return false;
        }
      }
      return true;
    }
  };  // end of DataConvertor<std::map<std::string,T>>

}  // end of namespace tfel::utilities::internals

namespace tfel::utilities {

  template <typename T>
  T convert(const Data& d) {
    return tfel::utilities::internals::DataConvertor<T>::exe(d);
  }  // end of convert

  template <typename T>
  bool is_convertible(const Data& d) {
    return tfel::utilities::internals::DataConvertor<T>::is_convertible(d);
  }  // end of convert

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_DATA_IXX */
