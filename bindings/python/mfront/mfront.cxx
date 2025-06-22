/*!
 * \file   bindings/python/mfront/mfront.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Python/SharedPtr.hxx"
#include <boost/python.hpp>
#include "TFEL/Raise.hxx"
#include "MFront/InitDSLs.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"

#ifdef _MSC_VER
// explicit specialisation to fix a bug in Visual Studio 2015 update 3
namespace boost {
  template <>
  const volatile mfront::BehaviourDescription*
  get_pointer<const volatile mfront::BehaviourDescription>(
      const volatile mfront::BehaviourDescription* c) {
    return c;
  }
  template <>
  const volatile mfront::AbstractBehaviourDSL*
  get_pointer<const volatile mfront::AbstractBehaviourDSL>(
      const volatile mfront::AbstractBehaviourDSL* c) {
    return c;
  }
  template <>
  const volatile mfront::AbstractDSL*
  get_pointer<const volatile mfront::AbstractDSL>(
      const volatile mfront::AbstractDSL* c) {
    return c;
  }
  template <>
  const volatile mfront::BehaviourData*
  get_pointer<const volatile mfront::BehaviourData>(
      const volatile mfront::BehaviourData* c) {
    return c;
  }
}  // namespace boost
#endif /* _MSC_VER */

void declareAbstractDSL();
void declareDSLFactory();
void declareFileDescription();
void declareCompiledTargetDescriptionBase();
void declareLibraryDescription();
void declareTargetsDescription();
void declareAbstractBehaviourDSL();
void declareBehaviourSymmetryType();
void declareVariableBoundsDescription();
void declareVariableDescription();
void declareBehaviourAttribute();
void declareBehaviourData();
void declareBehaviourDescription();
void declareSearchPathsHandler();
void declareGeneratorOptions();
void declareMakefileGenerator();
void declareCMakeGenerator();

static boost::python::object getDSL(const std::string& f) {
  auto dsl = mfront::MFrontBase::getDSL(f);
  if (dsl->getTargetType() == mfront::AbstractDSL::BEHAVIOURDSL) {
    auto b = std::dynamic_pointer_cast<mfront::AbstractBehaviourDSL>(dsl);
    tfel::raise_if(!b, "getDSL: invalid dsl implementation");
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
  declareCompiledTargetDescriptionBase();
  declareLibraryDescription();
  declareTargetsDescription();
  // behaviours
  declareAbstractBehaviourDSL();
  declareBehaviourAttribute();
  declareVariableBoundsDescription();
  declareVariableDescription();
  declareBehaviourData();
  declareBehaviourSymmetryType();
  declareBehaviourDescription();
  declareSearchPathsHandler();
  // generators
  declareGeneratorOptions();
  declareMakefileGenerator();
  declareCMakeGenerator();
}
