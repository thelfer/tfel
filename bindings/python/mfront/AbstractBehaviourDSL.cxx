/*!
 * \file   bindings/python/mfront/AbstractBehaviourDSL.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<set>
#include<memory>
#include"TFEL/Python/SharedPtr.hxx"
#include<boost/python.hpp>
#include"MFront/BehaviourDescription.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"

void declareAbstractBehaviourDSL(void);
void declareAbstractBehaviourDSL(void){
  using namespace boost::python;
  using namespace mfront;
  class_<AbstractBehaviourDSL,std::shared_ptr<AbstractBehaviourDSL>,
	 bases<AbstractDSL>,
	 boost::noncopyable>("AbstractBehaviourDSL",no_init)
    .def("getBehaviourDescription",&AbstractBehaviourDSL::getBehaviourDescription,
	 return_internal_reference<>())
    ;

}
