/*!
 * \file   bindings/python/mfront/BehaviourDescription.cxx
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

#include <set>
#include <memory>
#include <boost/python.hpp>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourDescription.hxx"

static std::vector<tfel::material::ModellingHypothesis::Hypothesis>
getModellingHypotheses(const mfront::BehaviourDescription& bd) {
  const auto& mh = bd.getModellingHypotheses();
  return {mh.begin(), mh.end()};
}

static std::vector<tfel::material::ModellingHypothesis::Hypothesis>
getDistinctModellingHypotheses(const mfront::BehaviourDescription& bd) {
  const auto& mh = bd.getDistinctModellingHypotheses();
  return {mh.begin(), mh.end()};
}

static void setModellingHypotheses1(
    mfront::BehaviourDescription& bd,
    const std::vector<tfel::material::ModellingHypothesis::Hypothesis>& mh) {
  using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
  bd.setModellingHypotheses(std::set<Hypothesis>{mh.begin(), mh.end()});
}

static void setModellingHypotheses2(
    mfront::BehaviourDescription& bd,
    const std::vector<tfel::material::ModellingHypothesis::Hypothesis>& mh,
    const bool b) {
  using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
  bd.setModellingHypotheses(std::set<Hypothesis>{mh.begin(), mh.end()}, b);
}

static std::string getStringAttribute(const mfront::BehaviourDescription& d,
                                      const std::string& n) {
  return d.getAttribute<std::string>(n);
}

static std::string getStringAttribute2(const mfront::BehaviourDescription& d,
                                       const std::string& n,
                                       const std::string& v) {
  return d.getAttribute<std::string>(n, v);
}

static unsigned short getUnsignedShortAttribute(
    const mfront::BehaviourDescription& d, const std::string& n) {
  return d.getAttribute<unsigned short>(n);
}

static unsigned short getUnsignedShortAttribute2(
    const mfront::BehaviourDescription& d,
    const std::string& n,
    const unsigned short v) {
  return d.getAttribute<unsigned short>(n, v);
}

static bool getBooleanAttribute(const mfront::BehaviourDescription& d,
                                const std::string& n) {
  return d.getAttribute<bool>(n);
}

static bool getBooleanAttribute2(const mfront::BehaviourDescription& d,
                                 const std::string& n,
                                 const bool v) {
  return d.getAttribute<bool>(n, v);
}

static std::string getStringAttribute3(
    const mfront::BehaviourDescription& d,
    const mfront::BehaviourDescription::Hypothesis h,
    const std::string& n) {
  return d.getAttribute<std::string>(h, n);
}

static unsigned short getUnsignedShortAttribute3(
    const mfront::BehaviourDescription& d,
    const mfront::BehaviourDescription::Hypothesis h,
    const std::string& n) {
  return d.getAttribute<unsigned short>(h, n);
}

static bool getBooleanAttribute3(
    const mfront::BehaviourDescription& d,
    const mfront::BehaviourDescription::Hypothesis h,
    const std::string& n) {
  return d.getAttribute<bool>(h, n);
}

static std::string getStringAttribute4(
    const mfront::BehaviourDescription& d,
    const mfront::BehaviourDescription::Hypothesis h,
    const std::string& n,
    const std::string& v) {
  return d.getAttribute<std::string>(h, n, v);
}

static unsigned short getUnsignedShortAttribute4(
    const mfront::BehaviourDescription& d,
    const mfront::BehaviourDescription::Hypothesis h,
    const std::string& n,
    const unsigned short v) {
  return d.getAttribute<unsigned short>(h, n, v);
}

static bool getBooleanAttribute4(
    const mfront::BehaviourDescription& d,
    const mfront::BehaviourDescription::Hypothesis h,
    const std::string& n,
    const bool v) {
  return d.getAttribute<bool>(h, n, v);
}

static bool hasAttribute(const mfront::BehaviourDescription& d,
                         const mfront::BehaviourDescription::Hypothesis h,
                         const std::string& n) {
  return d.hasAttribute(h, n);
}

static void setStrainMeasure(mfront::BehaviourDescription& d,
                             const std::string& s) {
  if (s == "Linearised") {
    d.setStrainMeasure(mfront::BehaviourDescription::LINEARISED);
  } else if (s == "Green-Lagrange") {
    d.setStrainMeasure(mfront::BehaviourDescription::GREENLAGRANGE);
  }
  tfel::raise_if(s != "Hencky",
                 "BehaviourDescription::setStrainMeasure: "
                 "unsupported strain measure '" +
                     s + "'");
  d.setStrainMeasure(mfront::BehaviourDescription::HENCKY);
}  // end of getStrainMeasure

static std::string getStrainMeasure(const mfront::BehaviourDescription& d) {
  tfel::raise_if(
      !d.isStrainMeasureDefined(),
      "BehaviourDescription::getStrainMeasure: no strain measure defined");
  const auto ss = d.getStrainMeasure();
  if (ss == mfront::BehaviourDescription::LINEARISED) {
    return "Linearised";
  } else if (ss == mfront::BehaviourDescription::GREENLAGRANGE) {
    return "Green-Lagrange";
  }
  tfel::raise_if(
      ss != mfront::BehaviourDescription::HENCKY,
      "BehaviourDescription::getStrainMeasure: unsupported strain measure");
  return "Hencky";
}  // end of getStrainMeasure

void declareBehaviourDescription();
void declareBehaviourDescription() {
  using namespace boost::python;
  using namespace mfront;

  class_<BehaviourDescription>("BehaviourDescription")
      .def("setBehaviourName", &BehaviourDescription::setBehaviourName)
      .def("getBehaviourName", &BehaviourDescription::getBehaviourName,
           return_internal_reference<>())
      .def("setDSLName", &BehaviourDescription::setDSLName)
      .def("getDSLName", &BehaviourDescription::getDSLName,
           return_internal_reference<>())
      .def("setMaterialName", &BehaviourDescription::setMaterialName)
      .def("getMaterialName", &BehaviourDescription::getMaterialName,
           return_internal_reference<>())
      .def("setLibrary", &BehaviourDescription::setLibrary)
      .def("getLibrary", &BehaviourDescription::getLibrary,
           return_internal_reference<>())
      .def("setClassName", &BehaviourDescription::setClassName)
      .def("getClassName", &BehaviourDescription::getClassName,
           return_internal_reference<>())
      .def("areModellingHypothesesDefined",
           &BehaviourDescription::areModellingHypothesesDefined)
      .def("isModellingHypothesisSupported",
           &BehaviourDescription::isModellingHypothesisSupported)
      .def("getModellingHypotheses", getModellingHypotheses)
      .def("getDistinctModellingHypotheses", getDistinctModellingHypotheses)
      .def("setModellingHypotheses", setModellingHypotheses1)
      .def("setModellingHypotheses", setModellingHypotheses2)
      .def("getBehaviourData", &BehaviourDescription::getBehaviourData,
           return_internal_reference<>())
      .def(
          "hasAttribute",
          static_cast<bool (BehaviourDescription::*)(const std::string&) const>(
              &BehaviourDescription::hasAttribute))
      .def("getSymmetryType", &BehaviourDescription::getSymmetryType)
      .def("getElasticSymmetryType",
           &BehaviourDescription::getElasticSymmetryType)
      .def("hasAttribute", ::hasAttribute)
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute)
      .def("getStringAttribute", getStringAttribute)
      .def("getBooleanAttribute", getBooleanAttribute)
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute2)
      .def("getStringAttribute", getStringAttribute2)
      .def("getBooleanAttribute", getBooleanAttribute2)
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute3)
      .def("getStringAttribute", getStringAttribute3)
      .def("getBooleanAttribute", getBooleanAttribute3)
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute4)
      .def("getStringAttribute", getStringAttribute4)
      .def("getBooleanAttribute", getBooleanAttribute4)
      .def("hasCrystalStructure", &BehaviourDescription::hasCrystalStructure,
           "return if a crystal structure has been defined")
      .def("getCrystalStructure", &BehaviourDescription::getCrystalStructure,
           "return the crystal structure")
      .def("setCrystalStructure", &BehaviourDescription::setCrystalStructure,
           "set the crystal structure")
      .def("getAttributes", &BehaviourDescription::getAttributes,
           return_value_policy<copy_const_reference>())
      .def("isStrainMeasureDefined",
           &BehaviourDescription::isStrainMeasureDefined,
           "return if a strain measure has been defined")
      .def("setStrainMeasure", setStrainMeasure,
           "set the strain measure. The following strain measures are "
           "supported:\n"
           "- Linearised\n"
           "- Green-Lagrange\n"
           "- Hencky\n")
      .def("getStrainMeasure", getStrainMeasure,
           "return the strain measure, which can have one of the following "
           "values:\n"
           "- Linearised\n"
           "- Green-Lagrange\n"
           "- Hencky\n");
}
