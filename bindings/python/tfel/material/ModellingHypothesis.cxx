
/*!
 * \file  bindings/python/tfel/ModellingHypothesis.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/ModellingHypothesis.hxx"

void declareModellingHypothesis(pybind11::module_&);

void declareModellingHypothesis(pybind11::module_& m) {
  using namespace tfel::material;
  pybind11::enum_<ModellingHypothesis::Hypothesis>(m, "ModellingHypothesis")
      .value("AXISYMMETRICALGENERALISEDPLANESTRAIN",
             ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)
      .value("AXISYMMETRICALGENERALISEDPLANESTRESS",
             ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)
      .value("AXISYMMETRICAL", ModellingHypothesis::AXISYMMETRICAL)
      .value("PLANESTRESS", ModellingHypothesis::PLANESTRESS)
      .value("PLANESTRAIN", ModellingHypothesis::PLANESTRAIN)
      .value("GENERALISEDPLANESTRAIN",
             ModellingHypothesis::GENERALISEDPLANESTRAIN)
      .value("TRIDIMENSIONAL", ModellingHypothesis::TRIDIMENSIONAL)
      .value("UNDEFINEDHYPOTHESIS", ModellingHypothesis::UNDEFINEDHYPOTHESIS);
}
