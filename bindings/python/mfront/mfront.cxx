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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "TFEL/Macros.hxx"
#include "MFront/InitDSLs.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/PathSpecifier.hxx"

#define MFRONT_MODULE_NAME \
  TFEL_PP_JOIN(_mfront_, TFEL_SUFFIX_FOR_PYTHON_MODULES)

void declareMadnexSupport(pybind11::module_&);
void declareAbstractDSL(pybind11::module_&);
void declareDSLFactory(pybind11::module_&);
void declareFileDescription(pybind11::module_&);
void declareCompiledTargetDescriptionBase(pybind11::module_&);
void declareLibraryDescription(pybind11::module_&);
void declareTargetsDescription(pybind11::module_&);
void declareMaterialPropertyDSL(pybind11::module_&);
void declareAbstractBehaviourDSL(pybind11::module_&);
void declareBehaviourSymmetryType(pybind11::module_&);
void declareVariableBoundsDescription(pybind11::module_&);

void declareVariableDescription(pybind11::module_&);
void declareMaterialKnowledgeAttribute(pybind11::module_&);
void declareBehaviourData(pybind11::module_&);
void declareMaterialPropertyDescription(pybind11::module_&);
void declareBehaviourDescription(pybind11::module_&);
void declareSearchPathsHandler(pybind11::module_&);
void declareGeneratorOptions(pybind11::module_&);
void declareMakefileGenerator(pybind11::module_&);
void declareCMakeGenerator(pybind11::module_&);
void declareOverridableImplementation(pybind11::module_&);

static pybind11::object getDSL(const std::string& f) {
  auto dsl = mfront::MFrontBase::getDSL(f);
  if (dsl->getTargetType() == mfront::AbstractDSL::MATERIALPROPERTYDSL) {
    auto b = std::dynamic_pointer_cast<mfront::MaterialPropertyDSL>(dsl);
    tfel::raise_if(b.get() == nullptr, "getDSL: invalid dsl implementation");
    return pybind11::cast(b);
  } else if (dsl->getTargetType() == mfront::AbstractDSL::BEHAVIOURDSL) {
    auto b = std::dynamic_pointer_cast<mfront::AbstractBehaviourDSL>(dsl);
    tfel::raise_if(b.get() == nullptr, "getDSL: invalid dsl implementation");
    return pybind11::cast(b);
  }
  return pybind11::cast(dsl);
}

#ifdef TFEL_SUFFIX_FOR_PYTHON_MODULES
PYBIND11_MODULE(MFRONT_MODULE_NAME, m) {
#else
PYBIND11_MODULE(_mfront, m) {
#endif
  using namespace pybind11::literals;
  using GetImplementationsPathsPtr = std::vector<std::string> (*)(
      const std::string&, const std::string&, const std::string&,
      const std::string&, const std::string&);
  GetImplementationsPathsPtr ptr = mfront::getImplementationsPaths;
  m.def("initDSLs", mfront::initDSLs);
  m.def("initInterfaces", mfront::initInterfaces);
  m.def("setDebugMode", mfront::setDebugMode);
  m.def("getDSL", getDSL);
  m.def("getImplementationsPaths", ptr, "file"_a, "material"_a = "",
        "material_property"_a = "", "behaviour"_a = "", "model"_a = "");
  declareMadnexSupport(m);
  declareAbstractDSL(m);
  declareDSLFactory(m);
  declareFileDescription(m);
  declareCompiledTargetDescriptionBase(m);
  declareLibraryDescription(m);
  declareTargetsDescription(m);
  declareOverridableImplementation(m);
  declareMaterialKnowledgeAttribute(m);
  // material properties
  declareMaterialPropertyDSL(m);
  declareMaterialPropertyDescription(m);
  // behaviours
  declareAbstractBehaviourDSL(m);
  declareVariableBoundsDescription(m);
  declareVariableDescription(m);
  declareBehaviourData(m);
  declareBehaviourSymmetryType(m);
  declareBehaviourDescription(m);
  declareSearchPathsHandler(m);
  // generators
  declareGeneratorOptions(m);
  declareMakefileGenerator(m);
  declareCMakeGenerator(m);
}
