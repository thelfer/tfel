/*!
 * \file  bindings/python/tfel/ModellingHypothesis.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/Python/VectorConverter.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

void declareModellingHypothesis();

void declareModellingHypothesis() {
  using namespace boost::python;
  using namespace tfel::material;
  using namespace tfel::python;
  enum_<ModellingHypothesis::Hypothesis>("ModellingHypothesis")
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
  initializeVectorConverter<std::vector<ModellingHypothesis::Hypothesis>>();
}
