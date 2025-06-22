/*!
 * \file  bindings/python/tfel/ExternalBehaviourDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 june 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/System/ExternalBehaviourDescription.hxx"

void declareExternalBehaviourDescription();

void declareExternalBehaviourDescription() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::system;
  class_<ExternalBehaviourDescription>("ExternalBehaviourDescription", init<>())
      .def(init<std::string, std::string, std::string>())
      .def_readonly("mpnames", &ExternalBehaviourDescription::mpnames)
      .def_readonly("ivnames", &ExternalBehaviourDescription::ivnames)
      .def_readonly("ivtypes", &ExternalBehaviourDescription::ivtypes)
      .def_readonly("evnames", &ExternalBehaviourDescription::evnames)
      .def_readonly("source", &ExternalBehaviourDescription::source)
      .def_readonly("btype", &ExternalBehaviourDescription::btype)
      .def_readonly("stype", &ExternalBehaviourDescription::stype)
      .def_readonly("etype", &ExternalBehaviourDescription::etype)
      .def_readonly("isUPUIR", &ExternalBehaviourDescription::isUPUIR)
      .def_readonly("requiresStiffnessTensor",
                    &ExternalBehaviourDescription::requiresStiffnessTensor)
      .def_readonly("requiresThermalExpansionCoefficientTensor",
                    &ExternalBehaviourDescription::
                        requiresThermalExpansionCoefficientTensor);
}
