/*! 
 * \file  mfront/mtest/AccelerationAlgorithmFactory.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"MTest/AccelerationAlgorithm.hxx"
#include"MTest/CastemAccelerationAlgorithm.hxx"
#include"MTest/IronsTuckAccelerationAlgorithm.hxx"
#include"MTest/SteffensenAccelerationAlgorithm.hxx"
#include"MTest/SecantAccelerationAlgorithm.hxx"
#include"MTest/AccelerationAlgorithmFactory.hxx"

namespace mfront
{

  template<typename T>
  static tfel::utilities::shared_ptr<AccelerationAlgorithm>
  buildAlgorithmConstructor(void)
  {
    return tfel::utilities::shared_ptr<AccelerationAlgorithm>(new T());
  } // end of buildAlgoritmConstructor

  AccelerationAlgorithmFactory&
  AccelerationAlgorithmFactory::getAccelerationAlgorithmFactory()
  {
    static AccelerationAlgorithmFactory factory;
    return factory;
  }
  
  tfel::utilities::shared_ptr<AccelerationAlgorithm>
  AccelerationAlgorithmFactory::getAlgorithm(const std::string& a) const
  {
    using namespace std;
    map<string,constructor>::const_iterator p;
    p = this->constructors.find(a);
    if(p==this->constructors.end()){
      string msg("AccelerationAlgorithmFactory::getAlgorithm : "
		 "no algorithm '"+a+"' registred");
      throw(runtime_error(msg));
    }
    return (*(p->second))();
  }

  AccelerationAlgorithmFactory::AccelerationAlgorithmFactory()
  {
    this->registerAlgorithm("Cast3M",buildAlgorithmConstructor<CastemAccelerationAlgorithm>);
    this->registerAlgorithm("Secant",buildAlgorithmConstructor<SecantAccelerationAlgorithm>);
    this->registerAlgorithm("Steffensen",buildAlgorithmConstructor<SteffensenAccelerationAlgorithm>);
    this->registerAlgorithm("Irons-Tuck",buildAlgorithmConstructor<IronsTuckAccelerationAlgorithm>);
  } // end of AccelerationAlgorithmFactory::AccelerationAlgorithmFactory

  void
  AccelerationAlgorithmFactory::registerAlgorithm(const std::string& a,
						       const constructor c)
  {
    using namespace std;
    typedef map<string,constructor>::value_type value_type;
    if(!this->constructors.insert(value_type(a,c)).second){
      string msg("AccelerationAlgorithmFactory::registerAlgorithm : "
		 "algorithm '"+a+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of AccelerationAlgorithmFactory::registerAlgorithm

} // end of namespace mfront
