/*!
 * \file   mfront/include/MFront/VariableDescription.ixx
 * \brief  This file declares the VariableDescription class
 * \author Thomas Helfer
 * \date   9 Jan 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARDESCRIPTION_IXX
#define LIB_MFRONT_VARDESCRIPTION_IXX

namespace mfront {

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1,
      T&>::type
  VariableDescription::getAttribute(const std::string& n) {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      p = this->attributes.insert({n, VariableAttribute(T())}).first;
    }
    return p->second.template get<T>();
  }  // end of VariableDescription::getAttribute

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1,
      const T&>::type
  VariableDescription::getAttribute(const std::string& n) const {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      VariableDescription::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  }  // end of VariableDescription::getAttribute

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, VariableAttributeTypes>::value == 1,
      T>::type
  VariableDescription::getAttribute(const std::string& n, const T& v) const {
    auto p = this->attributes.find(n);
    if (p != this->attributes.end()) {
      return p->second.template get<T>();
    }
    return v;
  }  // end of VariableDescription::getAttribute

}  // end of namespace mfront

#endif /* LIB_MFRONT_VARDESCRIPTION_IXX */
