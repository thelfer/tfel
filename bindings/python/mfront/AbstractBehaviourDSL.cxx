
/*!
 * \file   bindings/python/mfront/AbstractBehaviourDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/05/2016
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
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"

void declareAbstractBehaviourDSL(pybind11::module_&);

void declareAbstractBehaviourDSL(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<AbstractBehaviourDSL, AbstractDSL,
                   std::shared_ptr<AbstractBehaviourDSL>>(
      m, "AbstractBehaviourDSL")
      .def("getBehaviourDescription",
           &AbstractBehaviourDSL::getBehaviourDescription,
           pybind11::return_value_policy::reference);
}
