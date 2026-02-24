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
      if (d.empty()) {
        return {};
      }
      if (!d.is<std::vector<Data>>()) {
        tfel::raise("convert<std::vector<T>>: invalid data type");
      }
      const auto& vd = d.get<std::vector<Data>>();
      auto r = std::vector<T>{};
      r.reserve(vd.size());
      for (const auto& w : vd) {
        r.push_back(tfel::utilities::convert<T>(w));
      }
      return r;
    }  // end of exe
    static bool is_convertible(const Data& d) {
      if (d.empty()) {
        return true;
      }
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
      if (d.empty()) {
        return {};
      }
      if (!d.is<DataMap>()) {
        tfel::raise("convert<std::map<std::string,T>>: invalid data type");
      }
      const auto& m = d.get<DataMap>();
      auto r = std::map<std::string, T>{};
      for (const auto& e : m) {
        r.insert({e.first, tfel::utilities::convert<T>(e.second)});
      }
      return r;
    }  // end of exe
    static bool is_convertible(const Data& d) {
      if (d.empty()) {
        return true;
      }
      if (!d.is<tfel::utilities::DataMap>()) {
        return false;
      }
      const auto& vd = d.get<tfel::utilities::DataMap>();
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

  template <typename T1>
  DataMapValidator&
  DataMapValidator::addDataTypeValidator(const std::string& k) requires(
      tfel::meta::TLCountNbrOfT<std::decay_t<T1>, DataTypes>::value == 1) {
    return this->addDataValidator(k, [](const Data& d) {
      if (!d.template is<T1>()) {
        tfel::raise("invalid type");
      }
    });
  }  // end of addDataTypeValidator

  template <typename T>
  T convert(const Data& d) {
    return tfel::utilities::internals::DataConvertor<T>::exe(d);
  }  // end of convert

  template <typename T>
  bool is_convertible(const Data& d) {
    return tfel::utilities::internals::DataConvertor<T>::is_convertible(d);
  }  // end of convert

  template <typename ResultType>
  bool is(const DataMap& m, std::string_view n) {
    const auto& v = get(m, n);
    return v.template is<ResultType>();
  }  // end of is

  template <typename ResultType>
  const ResultType& get(const DataMap& m, std::string_view n) {
    const auto& v = get(m, n);
    if (!v.template is<ResultType>()) {
      raiseUnmatchedDataType(n);
    }
    return get<ResultType>(v);
  }  // end of get

  template <typename ResultType>
  ResultType get_if(const DataMap& m, std::string_view n, const ResultType& v) {
    if (contains(m, n)) {
      return get<ResultType>(m, n);
    }
    return v;
  }  // end of get

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_DATA_IXX */
