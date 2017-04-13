/*!
 * \file   bindings/python/mfront/VariableDescription.cxx
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

#include<boost/python.hpp>
#include"MFront/VariableDescription.hxx"
#include"TFEL/Python/VectorConverter.hxx"

static mfront::VariableDescriptionContainer::size_type
VariableDescriptionContainer_size(const mfront::VariableDescriptionContainer& v)
{
  return v.size();
}

void declareVariableDescription();
void declareVariableDescription(){
  using namespace boost::python;
  using namespace mfront;
  class_<VariableDescriptionBase>("VariableDescriptionBase")
    .def_readwrite("type",&VariableDescriptionBase::type)
    .def_readwrite("name",&VariableDescriptionBase::name)
    .def_readwrite("description",&VariableDescriptionBase::description)
    .def_readwrite("arraySize",&VariableDescriptionBase::arraySize)
    .def_readwrite("lineNumber",&VariableDescriptionBase::lineNumber)
    ;
  
  class_<VariableDescription,bases<VariableDescriptionBase>>("VariableDescription")
    .def("hasBounds",&VariableDescription::hasBounds,
	 "Return true if the bounds of the variable has already been set")
    .def("getBounds",&VariableDescription::getBounds,
	 return_internal_reference<>(),
	 "Return the physical bounds of the variable")
    .def("setBounds",&VariableDescription::setBounds,
	 "Set the physical bounds of the variable")
    .def("hasPhysicalBounds",&VariableDescription::hasPhysicalBounds,
	 "Return true if the physical bounds of the variable has already been set")
    .def("getPhysicalBounds",&VariableDescription::getPhysicalBounds,
	 return_internal_reference<>(),
	 "Get the physical bounds of the variable")
    .def("setPhysicalBounds",&VariableDescription::setPhysicalBounds,
	 "Set the physical bounds of the variable")
    .def("setGlossaryName",&VariableDescription::setGlossaryName,
	 "Set the glossary name")
    .def("setEntryName",&VariableDescription::setEntryName,
	 "Set the entry name")
    .def("getExternalName",&VariableDescription::getExternalName,
	 return_value_policy<copy_const_reference>(),
	 "Return the external name")
    .def("hasGlossaryName",&VariableDescription::hasGlossaryName,
	 "Return true if the variable has a glossary name")
    .def("hasEntryName",&VariableDescription::hasEntryName,
	 "Return true if the variable has a entry name")
    ;
  
  class_<VariableDescriptionContainer>("VariableDescriptionContainer")
    .def("__iter__", iterator<VariableDescriptionContainer>())
    .def("__len__",VariableDescriptionContainer_size)
    .def("contains",&VariableDescriptionContainer::contains)
    ;
}

