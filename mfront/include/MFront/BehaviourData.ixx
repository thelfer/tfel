/*!
 * \file   mfront/include/MFront/BehaviourData.ixx
 * \brief  This file declares the BehaviourData class
 * \author Helfer Thomas
 * \date   21 Mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDATA_IXX_
#define LIB_MFRONT_BEHAVIOURDATA_IXX_

namespace mfront {

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
      T&>::type
  BehaviourData::getAttribute(const std::string& n) {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      p = this->attributes.insert({n, BehaviourAttribute(T())}).first;
    }
    return p->second.template get<T>();
  }  // end of BehaviourData::getAttribute

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
      const T&>::type
  BehaviourData::getAttribute(const std::string& n) const {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      BehaviourData::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  }  // end of BehaviourData::getAttribute

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDATA_IXX_ */
