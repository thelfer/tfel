/*!
 * \file   mfront/include/MFront/MechanicalBehaviourData.ixx
 * \brief  This file declares the MechanicalBehaviourData class
 * \author Helfer Thomas
 * \date   21 Mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
