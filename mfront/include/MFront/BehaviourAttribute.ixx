/*!
 * \file   BehaviourAttribute.ixx
 * \brief    
 * \author Thomas Helfer
 * \date   10/01/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURATTRIBUTE_IXX
#define LIB_MFRONT_BEHAVIOURATTRIBUTE_IXX

namespace mfront {

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
      T&>::type
  BehaviourAttributesHandler::getAttribute(const std::string& n) {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      p = this->attributes.insert({n, BehaviourAttribute(T())}).first;
    }
    return p->second.template get<T>();
  }  // end of getAttribute

  template <typename T>
  typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
      const T&>::type
  BehaviourAttributesHandler::getAttribute(const std::string& n) const {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      BehaviourAttributesHandler::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  }  // end of getAttribute

} // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURATTRIBUTE_IXX */
