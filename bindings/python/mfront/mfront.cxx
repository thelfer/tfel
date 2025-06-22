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
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/PathSpecifier.hxx"

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
void declareMaterialPropertyDSL();
void declareAbstractBehaviourDSL();
void declareBehaviourSymmetryType();
void declareVariableBoundsDescription();
void declareVariableDescription();
void declareBehaviourAttribute();
void declareBehaviourData();
void declareMaterialPropertyDescription();
void declareBehaviourDescription();
void declareSearchPathsHandler();
void declareGeneratorOptions();
void declareMakefileGenerator();
void declareCMakeGenerator();
void declareOverridableImplementation();

static boost::python::object getDSL(const std::string& f) {
  auto dsl = mfront::MFrontBase::getDSL(f);
  if (dsl->getTargetType() == mfront::AbstractDSL::MATERIALPROPERTYDSL) {
    auto b = std::dynamic_pointer_cast<mfront::MaterialPropertyDSL>(dsl);
    tfel::raise_if(!b, "getDSL: invalid dsl implementation");
    return boost::python::object(b);
  } else if (dsl->getTargetType() == mfront::AbstractDSL::BEHAVIOURDSL) {
    auto b = std::dynamic_pointer_cast<mfront::AbstractBehaviourDSL>(dsl);
    tfel::raise_if(!b, "getDSL: invalid dsl implementation");
    return boost::python::object(b);
  }
  return boost::python::object(dsl);
}

BOOST_PYTHON_MODULE(_mfront) {
  using GetImplementationsPathsPtr = std::vector<std::string> (*)(
      const std::string&, const std::string&, const std::string&,
      const std::string&, const std::string&);
  GetImplementationsPathsPtr ptr = mfront::getImplementationsPaths;
  boost::python::def("initDSLs", mfront::initDSLs);
  boost::python::def("initInterfaces", mfront::initInterfaces);
  boost::python::def("getDSL", getDSL);
  boost::python::def(
      "getImplementationsPaths", ptr,
      (boost::python::args("file"), boost::python::args("material") = "",
       boost::python::args("material_property") = "",
       boost::python::args("behaviour") = "",
       boost::python::args("model") = ""));
  declareDSLFactory();
  declareAbstractDSL();
  declareFileDescription();
  declareCompiledTargetDescriptionBase();
  declareLibraryDescription();
  declareTargetsDescription();
  declareOverridableImplementation();
  // material properties
  declareMaterialPropertyDSL();
  declareMaterialPropertyDescription();
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
