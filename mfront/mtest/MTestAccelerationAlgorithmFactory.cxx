/*! 
 * \file  mfront/mtest/MTestAccelerationAlgorithmFactory.cxx
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

#include"MFront/MTestAccelerationAlgorithm.hxx"
#include"MFront/MTestCastemAccelerationAlgorithm.hxx"
#include"MFront/MTestIronsTuckAccelerationAlgorithm.hxx"
#include"MFront/MTestSteffensenAccelerationAlgorithm.hxx"
#include"MFront/MTestSecantAccelerationAlgorithm.hxx"
#include"MFront/MTestAccelerationAlgorithmFactory.hxx"

namespace mfront
{

  template<typename T>
  static tfel::utilities::shared_ptr<MTestAccelerationAlgorithm>
  buildAlgorithmConstructor(void)
  {
    return tfel::utilities::shared_ptr<MTestAccelerationAlgorithm>(new T());
  } // end of buildAlgoritmConstructor

  MTestAccelerationAlgorithmFactory&
  MTestAccelerationAlgorithmFactory::getMTestAccelerationAlgorithmFactory()
  {
    static MTestAccelerationAlgorithmFactory factory;
    return factory;
  }
  
  tfel::utilities::shared_ptr<MTestAccelerationAlgorithm>
  MTestAccelerationAlgorithmFactory::getAlgorithm(const std::string& a) const
  {
    using namespace std;
    map<string,constructor>::const_iterator p;
    p = this->constructors.find(a);
    if(p==this->constructors.end()){
      string msg("MTestAccelerationAlgorithmFactory::getAlgorithm : "
		 "no algorithm '"+a+"' registred");
      throw(runtime_error(msg));
    }
    return (*(p->second))();
  }

  MTestAccelerationAlgorithmFactory::MTestAccelerationAlgorithmFactory()
  {
    this->registerAlgorithm("Cast3M",buildAlgorithmConstructor<MTestCastemAccelerationAlgorithm>);
    this->registerAlgorithm("Secant",buildAlgorithmConstructor<MTestSecantAccelerationAlgorithm>);
    this->registerAlgorithm("Steffensen",buildAlgorithmConstructor<MTestSteffensenAccelerationAlgorithm>);
    this->registerAlgorithm("Irons-Tuck",buildAlgorithmConstructor<MTestIronsTuckAccelerationAlgorithm>);
  } // end of MTestAccelerationAlgorithmFactory::MTestAccelerationAlgorithmFactory

  void
  MTestAccelerationAlgorithmFactory::registerAlgorithm(const std::string& a,
						       const constructor c)
  {
    using namespace std;
    typedef map<string,constructor>::value_type value_type;
    if(!this->constructors.insert(value_type(a,c)).second){
      string msg("MTestAccelerationAlgorithmFactory::registerAlgorithm : "
		 "algorithm '"+a+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of MTestAccelerationAlgorithmFactory::registerAlgorithm

} // end of namespace mfront
