/*!
 * \file   MechanicalBehaviourData.ixx
 * \brief  This file declares the MechanicalBehaviourData class
 * \author Helfer Thomas
 * \date   21 Mars 2014
 */

#ifndef _LIB_MFRONT_MECHANICALBEHAVIOURDATA_IXX_
#define _LIB_MFRONT_MECHANICALBEHAVIOURDATA_IXX_ 

namespace mfront{

  template<typename T>
  typename tfel::meta::EnableIf<
    tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
    T&>::type
  MechanicalBehaviourData::getAttribute(const std::string& n)
  {
    using namespace std;
    typedef typename map<string,MechanicalBehaviourAttribute>::value_type MVType;
    typename map<string,MechanicalBehaviourAttribute>::iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      p = this->attributes.insert(MVType(n,MechanicalBehaviourAttribute(T()))).first;
    }
    return p->second.template get<T>();
  } // end of MechanicalBehaviourData::getAttribute

  template<typename T>
  typename tfel::meta::EnableIf<
    tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
    const T&>::type
  MechanicalBehaviourData::getAttribute(const std::string& n) const
  {
    using namespace std;
    map<string,MechanicalBehaviourAttribute>::const_iterator p;
    p = this->attributes.find(n);
    if(p==this->attributes.end()){
      MechanicalBehaviourData::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  } // end of MechanicalBehaviourData::getAttribute

} // end of namespace mfront

#endif /* _LIB_MFRONT_MECHANICALBEHAVIOURDATA_IXX_ */
