/*!
 * \file   bindings/python/mfront/BehaviourDescription.cxx
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
#include<boost/python.hpp>
#include"MFront/BehaviourDescription.hxx"

std::vector<tfel::material::ModellingHypothesis::Hypothesis>
BD_getModellingHypotheses(const mfront::BehaviourDescription& bd){
  const auto& mh = bd.getModellingHypotheses();
  return {mh.begin(),mh.end()};
}

std::vector<tfel::material::ModellingHypothesis::Hypothesis>
BD_getDistinctModellingHypotheses(const mfront::BehaviourDescription& bd){
  const auto& mh = bd.getDistinctModellingHypotheses();
  return {mh.begin(),mh.end()};
}

void
BD_setModellingHypotheses1(mfront::BehaviourDescription& bd,
			   const std::vector<tfel::material::ModellingHypothesis::Hypothesis>& mh){
  using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
  bd.setModellingHypotheses(std::set<Hypothesis>{mh.begin(),mh.end()});
}

void
BD_setModellingHypotheses2(mfront::BehaviourDescription& bd,
			   const std::vector<tfel::material::ModellingHypothesis::Hypothesis>& mh,
					     const bool b){
  using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
  bd.setModellingHypotheses(std::set<Hypothesis>{mh.begin(),mh.end()},b);
}


void declareBehaviourDescription(void){
  using namespace boost::python;
  using namespace mfront;
  class_<BehaviourDescription>("BehaviourDescription")
    .def("setBehaviourName",&BehaviourDescription::setBehaviourName)
    .def("getBehaviourName",&BehaviourDescription::getBehaviourName,
	 return_internal_reference<>())
    .def("setDSLName",&BehaviourDescription::setDSLName)
    .def("getDSLName",&BehaviourDescription::getDSLName,
	 return_internal_reference<>())
    .def("setMaterialName",&BehaviourDescription::setMaterialName)
    .def("getMaterialName",&BehaviourDescription::getMaterialName,
	 return_internal_reference<>())
    .def("setLibrary",&BehaviourDescription::setLibrary)
    .def("getLibrary",&BehaviourDescription::getLibrary,
	 return_internal_reference<>())
    .def("setClassName",&BehaviourDescription::setClassName)
    .def("getClassName",&BehaviourDescription::getClassName,
	 return_internal_reference<>())
    .def("areModellingHypothesesDefined",
	 &BehaviourDescription::areModellingHypothesesDefined)
    .def("isModellingHypothesisSupported",
	 &BehaviourDescription::isModellingHypothesisSupported)
    .def("getModellingHypotheses",
	 BD_getModellingHypotheses)
    .def("getDistinctModellingHypotheses",
	 BD_getDistinctModellingHypotheses)
    .def("setModellingHypotheses",
	 BD_setModellingHypotheses1)
    .def("setModellingHypotheses",
	 BD_setModellingHypotheses2)
    .def("getBehaviourData",&BehaviourDescription::getBehaviourData,
	 return_internal_reference<>());
    ;
}
