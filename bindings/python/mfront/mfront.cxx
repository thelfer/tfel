/*!
 * \file   bindings/python/mfront/mfront.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   03 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MFront/InitDSLs.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"

void declareAbstractDSL();
void declareDSLFactory();
void declareFileDescription();
void declareLibraryDescription();
void declareTargetsDescription();
void declareAbstractBehaviourDSL();
void declareVariableDescription();
void declareBehaviourAttribute();
void declareBehaviourData();
void declareBehaviourDescription();
void declareGeneratorOptions();
void declareMakefileGenerator();

static boost::python::object getDSL(const std::string& f) {
  auto dsl = mfront::MFrontBase::getDSL(f);
  if (dsl->getTargetType() == mfront::AbstractDSL::BEHAVIOURDSL) {
    auto b = std::dynamic_pointer_cast<mfront::AbstractBehaviourDSL>(dsl);
    if (!b) {
      throw(std::runtime_error("getDSL: invalid dsl implementation"));
    }
    return boost::python::object(b);
  }
  return boost::python::object(dsl);
}

BOOST_PYTHON_MODULE(_mfront) {
  boost::python::def("initDSLs", mfront::initDSLs);
  boost::python::def("initInterfaces", mfront::initInterfaces);
  boost::python::def("getDSL", getDSL);
  declareDSLFactory();
  declareAbstractDSL();
  declareFileDescription();
  declareLibraryDescription();
  declareTargetsDescription();
  // behaviours
  declareAbstractBehaviourDSL();
  declareBehaviourAttribute();
  declareVariableDescription();
  declareBehaviourData();
  declareBehaviourDescription();
  declareGeneratorOptions();
  declareMakefileGenerator();
}
