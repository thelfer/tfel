/*! 
 * \file   mfront/include/MFront/BehaviourDescription.ixx
 * \brief
 * \author Helfer Thomas
 * \brief  20 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_BEHAVIOURDESCRIPTION_IXX_
#define _LIB_MFRONT_BEHAVIOURDESCRIPTION_IXX_ 

namespace mfront
{

  template<typename T>
  const T&
  BehaviourDescription::getAttribute(const Hypothesis h,
				     const std::string& n) const
  {
    const BehaviourData& bdata = this->getBehaviourData(h);
    return bdata.template getAttribute<T>(n);
  } // end of BehaviourDescription::getAttribute

  template<typename T>
  const T
  BehaviourDescription::getAttribute(const Hypothesis   h,
					       const std::string& n,
					       const T& v) const
  {
    const BehaviourData& bdata = this->getBehaviourData(h);
    if(!bdata.hasAttribute(n)){
      return v;
    }
    return bdata.template getAttribute<T>(n);
  }

  template<typename T>
  typename tfel::meta::EnableIf<
    tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
    T&>::type
  BehaviourDescription::getAttribute(const std::string& n)
  {
    using namespace std;
    typedef typename map<string,BehaviourAttribute>::value_type MVType;
    typename map<string,BehaviourAttribute>::iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      p = this->attributes.insert(MVType(n,BehaviourAttribute(T()))).first;
    }
    return p->second.template get<T>();
  } // end of BehaviourDescription::getAttribute

  template<typename T>
  typename tfel::meta::EnableIf<
    tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
    const T&>::type
  BehaviourDescription::getAttribute(const std::string& n) const
  {
    using namespace std;
    map<string,BehaviourAttribute>::const_iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      BehaviourDescription::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  } // end of BehaviourDescription::getAttribute

  template<typename T>
  typename tfel::meta::EnableIf<
    tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
    T>::type
  BehaviourDescription::getAttribute(const std::string& n,
					       const T& v) const
  {
    using namespace std;
    map<string,BehaviourAttribute>::const_iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      return v;
    }
    return p->second.template get<T>();
  } // end of BehaviourDescription::getAttribute

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURDESCRIPTION_IXX_ */
