/*! 
 * \file  bindings/python/tfel/ExternalMaterialPropertyDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 june 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>
#include"TFEL/System/ExternalMaterialPropertyDescription.hxx"

static std::vector<std::string> get_args(const tfel::system::ExternalMaterialPropertyDescription& d) {
  return d.arguments;
}

void declareExternalMaterialPropertyDescription();

void declareExternalMaterialPropertyDescription() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;

  class_<ExternalMaterialPropertyDescription>(
      "ExternalMaterialPropertyDescription", init<>())
      .def(init<std::string, std::string>())
      .add_property("arguments", &get_args);

} // end of declareExternalMaterialPropertyDescription
