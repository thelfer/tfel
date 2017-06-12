/*!
 * \file   CrystalStructure.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 juin 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>
#include"TFEL/Material/CrystalStructure.hxx"

void declareCrystalStructure();

void declareCrystalStructure()
{
  using tfel::material::CrystalStructure;
  boost::python::enum_<CrystalStructure>("CrystalStructure")
    .value("Cubic",CrystalStructure::Cubic)
    .value("FCC",CrystalStructure::FCC)
    .value("HCP",CrystalStructure::HCP)
    ;
}
