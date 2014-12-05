/*!
 * \file   mfront/include/MFront/BehaviourData.ixx
 * \brief  This file declares the BehaviourData class
 * \author Helfer Thomas
 * \date   21 Mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_BEHAVIOURDATA_IXX_
#define _LIB_MFRONT_BEHAVIOURDATA_IXX_ 

namespace mfront{

  template<typename T>
  typename std::enable_if<
    tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
    T&>::type
  BehaviourData::getAttribute(const std::string& n)
  {
    using namespace std;
    typedef typename map<string,BehaviourAttribute>::value_type MVType;
    typename map<string,BehaviourAttribute>::iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      p = this->attributes.insert(MVType(n,BehaviourAttribute(T()))).first;
    }
    return p->second.template get<T>();
  } // end of BehaviourData::getAttribute

  template<typename T>
  typename std::enable_if<
    tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
    const T&>::type
  BehaviourData::getAttribute(const std::string& n) const
  {
    using namespace std;
    map<string,BehaviourAttribute>::const_iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      BehaviourData::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  } // end of BehaviourData::getAttribute

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURDATA_IXX_ */
