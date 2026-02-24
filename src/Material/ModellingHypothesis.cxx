/*!
 * \file  src/Material/ModellingHypothesis.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 13 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace tfel::material {

  const std::vector<ModellingHypothesis::Hypothesis>&
  ModellingHypothesis::getModellingHypotheses() {
    static std::vector<ModellingHypothesis::Hypothesis> h{
        AXISYMMETRICALGENERALISEDPLANESTRAIN,
        AXISYMMETRICALGENERALISEDPLANESTRESS,
        AXISYMMETRICAL,
        PLANESTRESS,
        PLANESTRAIN,
        GENERALISEDPLANESTRAIN,
        TRIDIMENSIONAL};
    return h;
  }  // end of ModellingHypothesis::getModellingHypotheses

  bool ModellingHypothesis::isModellingHypothesis(const std::string& h) {
    return ((h == "AxisymmetricalGeneralisedPlaneStrain") ||
            (h == "AxisymmetricalGeneralisedPlaneStress") ||
            (h == "Axisymmetrical") || (h == "PlaneStress") ||
            (h == "PlaneStrain") || (h == "GeneralisedPlaneStrain") ||
            (h == "Tridimensional"));
  }  // end of ModellingHypothesis::isModellingHypothesis

  std::string ModellingHypothesis::toString(const Hypothesis h) {
    if (h == AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      return "AxisymmetricalGeneralisedPlaneStrain";
    } else if (h == AXISYMMETRICALGENERALISEDPLANESTRESS) {
      return "AxisymmetricalGeneralisedPlaneStress";
    } else if (h == AXISYMMETRICAL) {
      return "Axisymmetrical";
    } else if (h == PLANESTRESS) {
      return "PlaneStress";
    } else if (h == PLANESTRAIN) {
      return "PlaneStrain";
    } else if (h == GENERALISEDPLANESTRAIN) {
      return "GeneralisedPlaneStrain";
    } else if (h == TRIDIMENSIONAL) {
      return "Tridimensional";
    }
    raise(
        "ModellingHypothesis::toString : "
        "unsupported modelling hypothesis");
  }

  std::string ModellingHypothesis::toUpperCaseString(const Hypothesis h) {
    if (h == AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      return "AXISYMMETRICALGENERALISEDPLANESTRAIN";
    } else if (h == AXISYMMETRICALGENERALISEDPLANESTRESS) {
      return "AXISYMMETRICALGENERALISEDPLANESTRESS";
    } else if (h == AXISYMMETRICAL) {
      return "AXISYMMETRICAL";
    } else if (h == PLANESTRESS) {
      return "PLANESTRESS";
    } else if (h == PLANESTRAIN) {
      return "PLANESTRAIN";
    } else if (h == GENERALISEDPLANESTRAIN) {
      return "GENERALISEDPLANESTRAIN";
    } else if (h == TRIDIMENSIONAL) {
      return "TRIDIMENSIONAL";
    }
    raise(
        "ModellingHypothesis::toUpperCaseString: "
        "unsupported modelling hypothesis");
  }

  ModellingHypothesis::Hypothesis ModellingHypothesis::fromString(
      const std::string& h) {
    if (h == "AxisymmetricalGeneralisedPlaneStrain") {
      return AXISYMMETRICALGENERALISEDPLANESTRAIN;
    } else if (h == "AxisymmetricalGeneralisedPlaneStress") {
      return AXISYMMETRICALGENERALISEDPLANESTRESS;
    } else if (h == "Axisymmetrical") {
      return AXISYMMETRICAL;
    } else if (h == "PlaneStress") {
      return PLANESTRESS;
    } else if (h == "PlaneStrain") {
      return PLANESTRAIN;
    } else if (h == "GeneralisedPlaneStrain") {
      return GENERALISEDPLANESTRAIN;
    } else if (h == "Tridimensional") {
      return TRIDIMENSIONAL;
    }
    raise(
        "MFrontBehaviourParserCommon::getModellingHypothesisFromString: "
        "invalid or unsupported hypothesis '" +
        h +
        "'. The following "
        "hypotheses are supported:\n"
        "- AxisymmetricalGeneralisedPlaneStrain\n"
        "- AxisymmetricalGeneralisedPlaneStress\n"
        "- Axisymmetrical\n"
        "- PlaneStress\n"
        "- PlaneStrain\n"
        "- GeneralisedPlaneStrain\n"
        "- Tridimensional");
  }

  unsigned short getSpaceDimension(const ModellingHypothesis::Hypothesis h) {
    if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 1u;
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::PLANESTRESS) ||
               (h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 2u;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return 3u;
    }
    raise(
        "tfel::material::getSpaceDimension: "
        "unsupported modelling hypothesis");
  }

  unsigned short getStensorSize(const ModellingHypothesis::Hypothesis h) {
    if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 3u;
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::PLANESTRESS) ||
               (h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 4u;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return 6u;
    }
    raise(
        "tfel::material::getStensorSize: "
        "unsupported modelling hypothesis");
  }  // end of getStensorSize

  unsigned short getTensorSize(const ModellingHypothesis::Hypothesis h) {
    if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 3u;
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::PLANESTRESS) ||
               (h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 5u;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return 9u;
    }
    raise(
        "tfel::material::getTensorSize: "
        "unsupported modelling hypothesis");
  }  // end of getTensorSize

}  // end of namespace tfel::material
