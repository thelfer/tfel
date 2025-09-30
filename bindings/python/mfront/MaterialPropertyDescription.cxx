
/*!
 * \file   bindings/python/mfront/MaterialPropertyDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   9/08/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "MFront/MaterialPropertyDescription.hxx"

static const mfront::VariableDescriptionContainer&
MaterialPropertyDescription_getInputs(
    const mfront::MaterialPropertyDescription& md) {
  return md.inputs;
}

static const mfront::VariableDescriptionContainer&
MaterialPropertyDescription_getParameters(
    const mfront::MaterialPropertyDescription& md) {
  return md.parameters;
}

static const mfront::VariableDescription& MaterialPropertyDescription_getOutput(
    const mfront::MaterialPropertyDescription& md) {
  return md.output;
}

static std::string MaterialPropertyDescription_getLawName(
    const mfront::MaterialPropertyDescription& md) {
  return md.law;
}

static std::string MaterialPropertyDescription_getMaterialName(
    const mfront::MaterialPropertyDescription& md) {
  return md.material;
}

static std::string MaterialPropertyDescription_getClassName(
    const mfront::MaterialPropertyDescription& md) {
  return md.className;
}

void declareMaterialPropertyDescription(pybind11::module_&);

void declareMaterialPropertyDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<MaterialPropertyDescription>(m,
                                                "MaterialPropertyDescription")
      .def("getInputs", MaterialPropertyDescription_getInputs,
           pybind11::return_value_policy::reference,
           "returns the inputs of the material property")
      .def("getParameters", MaterialPropertyDescription_getParameters,
           pybind11::return_value_policy::reference,
           "returns the parameters of the material property")
      .def("getOutput", MaterialPropertyDescription_getOutput,
           pybind11::return_value_policy::reference,
           "returns the output of the material property")
      .def("getLawName", MaterialPropertyDescription_getLawName,
           "returns the name of material property")
      .def("getMaterialName", MaterialPropertyDescription_getMaterialName,
           "returns the name of material described by this material property")
      .def("getClassName", MaterialPropertyDescription_getClassName,
           "returns a class name combining the name of the material property "
           "and the name of the material");
}
