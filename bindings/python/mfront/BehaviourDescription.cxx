
/*!
 * \file   bindings/python/mfront/BehaviourDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date 04/05/2016
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
}

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
}

static bool BehaviourDescription_hasAttribute(
    const mfront::BehaviourDescription& d, const std::string& s) {
  return d.hasAttribute(s);
}

void declareBehaviourDescription(pybind11::module_&);

void declareBehaviourDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<BehaviourDescription>(
      m, "BehaviourDescription",
      "This structure describes a mechanical behaviour\n"
      "\n"
      "This class handles the fact that the mechanical behaviour\n"
      "data may depend on the modelling hypothesis considered,\n"
      "altough most of the time they won't.\n"
      "\n"
      "This class thus stores a default mechanical behaviour data\n"
      "which is intendend to be valid for most hypotheses, and specific\n"
      "mechanical behaviour datas when required.")
      .def("setBehaviourName", &BehaviourDescription::setBehaviourName)
      .def("getBehaviourName", &BehaviourDescription::getBehaviourName)
      .def("setDSLName", &BehaviourDescription::setDSLName)
      .def("getDSLName", &BehaviourDescription::getDSLName)
      .def("setMaterialName", &BehaviourDescription::setMaterialName)
      .def("getMaterialName", &BehaviourDescription::getMaterialName)
      .def("setLibrary", &BehaviourDescription::setLibrary)
      .def("getLibrary", &BehaviourDescription::getLibrary)
      .def("setClassName", &BehaviourDescription::setClassName)
      .def("getClassName", &BehaviourDescription::getClassName)
      .def("areModellingHypothesesDefined",
           &BehaviourDescription::areModellingHypothesesDefined)
      .def("isModellingHypothesisSupported",
           &BehaviourDescription::isModellingHypothesisSupported)
      .def("getModellingHypotheses", getModellingHypotheses)
      .def("getDistinctModellingHypotheses", getDistinctModellingHypotheses)
      .def("setModellingHypotheses", setModellingHypotheses1,
          pybind11::arg("mh"))
      .def("setModellingHypotheses", setModellingHypotheses2,
          pybind11::arg("mh"), pybind11::arg("b"))
      .def("getBehaviourData", &BehaviourDescription::getBehaviourData,
           pybind11::return_value_policy::reference)
      .def("hasAttribute", &BehaviourDescription_hasAttribute,
          pybind11::arg("s"))
      .def("getSymmetryType", &BehaviourDescription::getSymmetryType)
      .def("getElasticSymmetryType",
           &BehaviourDescription::getElasticSymmetryType)
      .def("hasAttribute", ::hasAttribute,
          pybind11::arg("h"), pybind11::arg("n"))
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute,
          pybind11::arg("n"))
      .def("getStringAttribute", getStringAttribute,
          pybind11::arg("n"))
      .def("getBooleanAttribute", getBooleanAttribute,
          pybind11::arg("n"))
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute2,
          pybind11::arg("n"), pybind11::arg("v"))
      .def("getStringAttribute", getStringAttribute2,
          pybind11::arg("n"), pybind11::arg("v"))
      .def("getBooleanAttribute", getBooleanAttribute2,
          pybind11::arg("n"), pybind11::arg("v"))
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute3,
          pybind11::arg("h"), pybind11::arg("n"))
      .def("getStringAttribute", getStringAttribute3,
          pybind11::arg("h"), pybind11::arg("n"))
      .def("getBooleanAttribute", getBooleanAttribute3,
          pybind11::arg("h"), pybind11::arg("n"))
      .def("getUnsignedShortAttribute", getUnsignedShortAttribute4,
          pybind11::arg("h"), pybind11::arg("n"), pybind11::arg("v"))
      .def("getStringAttribute", getStringAttribute4,
          pybind11::arg("h"), pybind11::arg("n"), pybind11::arg("v"))
      .def("getBooleanAttribute", getBooleanAttribute4,
          pybind11::arg("h"), pybind11::arg("n"), pybind11::arg("v"))
      .def("hasCrystalStructure", &BehaviourDescription::hasCrystalStructure)
      .def("getCrystalStructure", &BehaviourDescription::getCrystalStructure)
      .def("setCrystalStructure", &BehaviourDescription::setCrystalStructure)
      .def("getAttributes", &BehaviourDescription::getAttributes)
      .def("isStrainMeasureDefined",
           &BehaviourDescription::isStrainMeasureDefined)
      .def("setStrainMeasure", setStrainMeasure,
          pybind11::arg("s"))
      .def("getStrainMeasure", getStrainMeasure);
}
