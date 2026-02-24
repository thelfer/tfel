/*!
 * \file   bindings/python/mfront/VariableDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mai 2016
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
      m, "VariableDescription")
      .def("hasBounds", hasBounds_1,
           "Return true if the bounds of the variable has already been set")
      .def("hasBounds", hasBounds_2,
           "Return true if the bounds of the variable has already been set")
      .def("getBounds", getBounds_1, pybind11::return_value_policy::reference,
           "Return the physical bounds of the variable")
      .def("getBounds", getBounds_2, pybind11::return_value_policy::reference,
           "Return the physical bounds of the variable")
      .def("setBounds", setBounds_1, "Set the physical bounds of the variable")
      .def("setBounds", setBounds_2, "Set the physical bounds of the variable")
      .def("hasPhysicalBounds", hasPhysicalBounds_1,
           "Return true if the physical bounds of the variable has already "
           "been set")
      .def("hasPhysicalBounds", hasPhysicalBounds_2,
           "Return true if the physical bounds of the variable has already "
           "been set")
      .def("getPhysicalBounds", getPhysicalBounds_1,
           pybind11::return_value_policy::reference,
           "Get the physical bounds of the variable")
      .def("getPhysicalBounds", getPhysicalBounds_2,
           pybind11::return_value_policy::reference,
           "Get the physical bounds of the variable")
      .def("setPhysicalBounds", setPhysicalBounds_1,
           "Set the physical bounds of the variable")
      .def("setPhysicalBounds", setPhysicalBounds_2,
           "Set the physical bounds of the variable")
      .def("setGlossaryName", &VariableDescription::setGlossaryName,
           "Set the glossary name")
      .def("setEntryName", &VariableDescription::setEntryName,
           "Set the entry name")
      .def("getExternalName", &VariableDescription::getExternalName,
           "Return the external name")
      .def("hasGlossaryName", &VariableDescription::hasGlossaryName,
           "Return true if the variable has a glossary name")
      .def("hasEntryName", &VariableDescription::hasEntryName,
           "Return true if the variable has a entry name");

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
      .def("contains", &VariableDescriptionContainer::contains);
}
