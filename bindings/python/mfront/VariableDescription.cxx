/*!
 * \file   bindings/python/mfront/VariableDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date 04/05/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/VariableDescription.hxx"

static mfront::VariableDescriptionContainer::size_type
VariableDescriptionContainer_size(
    const mfront::VariableDescriptionContainer& v) {
  return v.size();
}

void declareVariableDescription(pybind11::module_&);

void declareVariableDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<VariableDescriptionBase>(m, "VariableDescriptionBase")
      .def_readwrite("type", &VariableDescriptionBase::type)
      .def_readwrite("name", &VariableDescriptionBase::name)
      .def_readwrite("description", &VariableDescriptionBase::description)
      .def_readwrite("arraySize", &VariableDescriptionBase::arraySize)
      .def_readwrite("lineNumber", &VariableDescriptionBase::lineNumber);

  bool (VariableDescription::*hasBounds_1)() const =
      &VariableDescription::hasBounds;
  bool (VariableDescription::*hasBounds_2)(const unsigned short) const =
      &VariableDescription::hasBounds;
  const VariableBoundsDescription& (VariableDescription::*getBounds_1)() const =
      &VariableDescription::getBounds;
  const VariableBoundsDescription& (VariableDescription::*getBounds_2)(
      const unsigned short) const = &VariableDescription::getBounds;
  void (VariableDescription::*setBounds_1)(const VariableBoundsDescription&) =
      &VariableDescription::setBounds;
  void (VariableDescription::*setBounds_2)(const VariableBoundsDescription&,
                                         const unsigned short) =
      &VariableDescription::setBounds;
  bool (VariableDescription::*hasPhysicalBounds_1)() const =
      &VariableDescription::hasPhysicalBounds;
  bool (VariableDescription::*hasPhysicalBounds_2)(const unsigned short) const =
      &VariableDescription::hasPhysicalBounds;
  const VariableBoundsDescription& (VariableDescription::*getPhysicalBounds_1)()
      const = &VariableDescription::getPhysicalBounds;
  const VariableBoundsDescription& (VariableDescription::*getPhysicalBounds_2)(
      const unsigned short) const = &VariableDescription::getPhysicalBounds;
  void (VariableDescription::*setPhysicalBounds_1)(
      const VariableBoundsDescription&) =
      &VariableDescription::setPhysicalBounds;
  void (VariableDescription::*setPhysicalBounds_2)(
      const VariableBoundsDescription&, const unsigned short) =
      &VariableDescription::setPhysicalBounds;

  pybind11::class_<VariableDescription, VariableDescriptionBase>(
      m, "VariableDescription",
      "Structure standing for a standard (non static) variable")
      .def("hasBounds", hasBounds_1,
           "Return true if bounds has been specified for this variable")
      .def("hasBounds", hasBounds_2,
           pybind11::arg("i"),
           "Return true if bounds has been specified for this variable\n"
           "i: position")
      .def("getBounds", getBounds_1, pybind11::return_value_policy::reference,
           "Return the bounds of this variable")
      .def("getBounds", getBounds_2, pybind11::return_value_policy::reference,
           pybind11::arg("i"),
           "Return the bounds of this variable\n"
           "i: position")
      .def("setBounds", setBounds_1,
           pybind11::arg("b"),
           "Set bounds on this variable\n"
           "b: bounds")
      .def("setBounds", setBounds_2,
           pybind11::arg("b"), pybind11::arg("i"),
           "Set bounds on this variable\n"
           "b: bounds\n"
           "i: position")
      .def("hasPhysicalBounds", hasPhysicalBounds_1,
           "Return true if physical bounds has been specified for this variable")
      .def("hasPhysicalBounds", hasPhysicalBounds_2,
           pybind11::arg("i"),
           "Return true if physical bounds has been specified for this variable\n"
           "i: position")
      .def("getPhysicalBounds", getPhysicalBounds_1,
           pybind11::return_value_policy::reference,
           "Return the physical bounds of this variable")
      .def("getPhysicalBounds", getPhysicalBounds_2,
           pybind11::return_value_policy::reference,
           pybind11::arg("i"),
           "Return the physical bounds of this variable\n"
           "i: position")
      .def("setPhysicalBounds", setPhysicalBounds_1,
           pybind11::arg("b"),
           "Set physical bounds on this variable\n"
           "b: bounds")
      .def("setPhysicalBounds", setPhysicalBounds_2,
           pybind11::arg("b"), pybind11::arg("i"),
           "Set physical bounds on this variable\n"
           "b: bounds\n"
           "i: position")
      .def("setGlossaryName", &VariableDescription::setGlossaryName,
          pybind11::arg("g"))
      .def("setEntryName", &VariableDescription::setEntryName,
          pybind11::arg("e"))
      .def("getExternalName", &VariableDescription::getExternalName)
      .def("hasGlossaryName", &VariableDescription::hasGlossaryName)
      .def("hasEntryName", &VariableDescription::hasEntryName);

  pybind11::class_<VariableDescriptionContainer>(m,
                                                 "VariableDescriptionContainer")
      .def(
          "__iter__",
          [](const VariableDescriptionContainer& v) {
            return pybind11::make_iterator(v.begin(), v.end());
          },
          pybind11::keep_alive<0,
                               1>())  // keep object alive while iterator exists
      .def("__len__", VariableDescriptionContainer_size)
      .def("contains", &VariableDescriptionContainer::contains,
          pybind11::arg("n"));
}
