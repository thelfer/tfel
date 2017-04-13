/*!
 * \file   VariableBoundsDescription.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   12 avril 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>
#include"MFront/VariableDescription.hxx"

void declareVariableBoundsDescription();
void declareVariableBoundsDescription(){

  using namespace boost::python;
  using namespace mfront;

  enum_<VariableBoundsDescription::BoundsType>("VariableBoundsTypes")
    .value("Lower",VariableBoundsDescription::LOWER)
    .value("Upper",VariableBoundsDescription::UPPER)
    .value("LowerAndUpper",VariableBoundsDescription::LOWERANDUPPER)
    ;
  
  class_<VariableBoundsDescription>("VariableBoundsDescription")
    .def_readwrite("boundsType",&VariableBoundsDescription::boundsType)
    .def_readwrite("lineNumber",&VariableBoundsDescription::lineNumber)
    .def_readwrite("component",&VariableBoundsDescription::component)
    .def_readwrite("lowerBound",&VariableBoundsDescription::lowerBound)
    .def_readwrite("upperBound",&VariableBoundsDescription::upperBound)
    ;

}

