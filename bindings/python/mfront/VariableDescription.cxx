/*!
 * \file   bindings/python/mfront/VariableDescription.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MFront/VariableDescription.hxx"
#include "TFEL/Python/VectorConverter.hxx"

static mfront::VariableDescriptionContainer::size_type
VariableDescriptionContainer_size(
    const mfront::VariableDescriptionContainer& v) {
  return v.size();
}

void declareVariableDescription();
void declareVariableDescription() {
  using namespace boost::python;
  using namespace mfront;
  class_<VariableDescription>("VariableDescription")
      .def_readwrite("type", &VariableDescription::type)
      .def_readwrite("name", &VariableDescription::name)
      .def_readwrite("description", &VariableDescription::description)
      .def_readwrite("arraySize", &VariableDescription::arraySize)
      .def_readwrite("lineNumber", &VariableDescription::lineNumber);
  class_<VariableDescriptionContainer>("VariableDescriptionContainer")
      .def("__iter__", iterator<VariableDescriptionContainer>())
      .def("__len__", VariableDescriptionContainer_size)
      .def("contains", &VariableDescriptionContainer::contains);
}
