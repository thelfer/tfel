/*!
 * \file   src/System/ExternalBehaviourDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 June 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "TFEL/System/ExternalBehaviourDescription.hxx"

namespace tfel::system {

  ExternalBehaviourDescription::ExternalBehaviourDescription(
      const std::string& l, const std::string& f, const std::string& h)
      : ExternalMaterialKnowledgeDescription(l, f) {
    auto throw_if = [l, f](const bool c, const std::string& m) {
      if (c) {
        tfel::raise(
            "ExternalBehaviourDescription::"
            "ExternalBehaviourDescription: " +
            m + " for behaviour '" + f +
            "' "
            "in library '" +
            l + "'");
      }
    };
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto hypotheses = elm.getSupportedModellingHypotheses(l, f);
    throw_if(
        std::find(hypotheses.begin(), hypotheses.end(), h) == hypotheses.end(),
        "unsupported hypothesis");
    this->behaviour = f;
    this->hypothesis = h;
    if (elm.contains(l, f + "_ElasticMaterialPropertiesEntryPoints")) {
      this->elastic_material_properties_epts =
          elm.getUMATElasticMaterialPropertiesEntryPoints(l, f);
    }
    if (elm.contains(l, f + "_LinearThermalExpansionCoefficientsEntryPoints")) {
      this->linear_thermal_expansion_coefficients_epts =
          elm.getUMATLinearThermalExpansionCoefficientsEntryPoints(l, f);
    }
    this->btype = elm.getUMATBehaviourType(l, f);
    this->kinematic = elm.getUMATBehaviourKinematic(l, f);
    this->stype = elm.getUMATSymmetryType(l, f);
    this->etype = elm.getUMATElasticSymmetryType(l, f);
    this->tangent_operator_blocks = elm.getUMATTangentOperatorBlocksNames(l, f);
    this->gnames = elm.getUMATGradientsNames(l, f);
    this->dvnames = this->gnames;
    this->gtypes = elm.getUMATGradientsTypes(l, f);
    this->dvtypes = this->gtypes;
    this->thnames = elm.getUMATThermodynamicForcesNames(l, f);
    this->thtypes = elm.getUMATThermodynamicForcesTypes(l, f);
    this->isUPUIR =
        elm.isUMATBehaviourUsableInPurelyImplicitResolution(l, f, h);
    this->computesInternalEnergy =
        elm.isUMATBehaviourAbleToComputeInternalEnergy(l, f, h);
    this->computesDissipatedEnergy =
        elm.isUMATBehaviourAbleToComputeDissipatedEnergy(l, f, h);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l, f, h);
    this->ivnames = elm.getUMATInternalStateVariablesNames(l, f, h);
    this->ivtypes = elm.getUMATInternalStateVariablesTypes(l, f, h);
    this->evnames = elm.getUMATExternalStateVariablesNames(l, f, h);
    this->evtypes = elm.getUMATExternalStateVariablesTypes(l, f, h);
    this->hasTemperatureBeenRemovedFromExternalStateVariables =
        elm.hasTemperatureBeenRemovedFromExternalStateVariables(l, f);
    // parameters
    const auto pn = elm.getUMATParametersNames(l, f, h);
    const auto pt = elm.getUMATParametersTypes(l, f, h);
    throw_if(
        pn.size() != pt.size(),
        "inconsistent size between parameters' names and parameters' sizes");
    for (decltype(pn.size()) i = 0; i != pn.size(); ++i) {
      if (pt[i] == 0) {
        this->pnames.push_back(pn[i]);
      } else if (pt[i] == 1) {
        this->ipnames.push_back(pn[i]);
      } else if (pt[i] == 2) {
        this->upnames.push_back(pn[i]);
      } else {
        throw_if(true,
                 "unsupported parameter type for parameter '" + pn[i] + "'");
      }
    }
    //! additional parameters
    this->requiresStiffnessTensor = elm.getUMATRequiresStiffnessTensor(l, f, h);
    this->requiresThermalExpansionCoefficientTensor =
        elm.getUMATRequiresThermalExpansionCoefficientTensor(l, f, h);
  }

  double ExternalBehaviourDescription::getRealParameterDefaultValue(
      const std::string& p) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getRealParameterDefaultValue(this->library, this->behaviour,
                                            this->hypothesis, p);
  }  // end of getRealParameterDefaultValue

  int ExternalBehaviourDescription::getIntegerParameterDefaultValue(
      const std::string& p) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getIntegerParameterDefaultValue(this->library, this->behaviour,
                                               this->hypothesis, p);
  }  // end of getIntegerParameterDefaultValue

  unsigned short
  ExternalBehaviourDescription::getUnsignedShortParameterDefaultValue(
      const std::string& p) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUnsignedShortParameterDefaultValue(
        this->library, this->behaviour, this->hypothesis, p);
  }  // end of getUnsignedShortParameterDefaultValue

  bool ExternalBehaviourDescription::hasBounds(const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasBounds(this->library, this->behaviour, this->hypothesis, v);
  }  // end of hasBounds

  bool ExternalBehaviourDescription::hasLowerBound(const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of hasLowerBound

  bool ExternalBehaviourDescription::hasUpperBound(const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of hasUpperBound

  long double ExternalBehaviourDescription::getLowerBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of  ExternalBehaviourDescription::getLowerBound

  long double ExternalBehaviourDescription::getUpperBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperBound(this->library, this->behaviour, this->hypothesis,
                             v);
  }  // end of getUpperBound

  bool ExternalBehaviourDescription::hasPhysicalBounds(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasPhysicalBounds(this->library, this->behaviour,
                                 this->hypothesis, v);
  }  // end of hasPhysicalBounds

  bool ExternalBehaviourDescription::hasLowerPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasLowerPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of hasLowerPhysicalBound

  bool ExternalBehaviourDescription::hasUpperPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.hasUpperPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of hasUpperPhysicalBound

  long double ExternalBehaviourDescription::getLowerPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getLowerPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of getLowerPhysicalBound

  long double ExternalBehaviourDescription::getUpperPhysicalBound(
      const std::string& v) const {
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    return elm.getUpperPhysicalBound(this->library, this->behaviour,
                                     this->hypothesis, v);
  }  // end of getUpperPhysicalBound

  ExternalBehaviourDescription::ExternalBehaviourDescription() = default;
  ExternalBehaviourDescription::ExternalBehaviourDescription(
      ExternalBehaviourDescription&&) = default;
  ExternalBehaviourDescription::ExternalBehaviourDescription(
      const ExternalBehaviourDescription&) = default;
  ExternalBehaviourDescription& ExternalBehaviourDescription::operator=(
      const ExternalBehaviourDescription&) = default;
  ExternalBehaviourDescription& ExternalBehaviourDescription::operator=(
      ExternalBehaviourDescription&&) = default;
  ExternalBehaviourDescription::~ExternalBehaviourDescription() = default;

}  // end of namespace tfel::system

extern "C" {

char* getExternalBehaviourData(tfel::system::ExternalBehaviourData* const d,
                               const char* const l,
                               const char* const f,
                               const char* const h) {
  auto report = [](const char* e) {
    auto strndup = [](const char* s, size_t n) -> char* {
      if (s == nullptr) {
        return nullptr;
      }
      auto length = ::strlen(s);
      if (length > n) {
        length = n;
      }
      auto copy = static_cast<char*>(::malloc(length + 1));
      if (copy == nullptr) {
        return nullptr;
      }
      ::memcpy(copy, s, length);
      copy[length] = '\0';
      return copy;
    };
    const auto msg = strndup(e, 255);
    if (msg == nullptr) {
      ::fprintf(stderr,
                "getExternalBehaviourData: generating "
                "error message failed, aborting\n");
      ::exit(EXIT_FAILURE);
    }
    return msg;
  };
  try {
    *d = tfel::system::ExternalBehaviourDescription(l, f, h);
  } catch (std::exception& e) {
    return report(e.what());
  } catch (...) {
    return report("getExternalBehaviourData: unknown exception thrown");
  }
  return nullptr;
}

tfel::system::ExternalBehaviourData* newExternalBehaviourData() {
  return new tfel::system::ExternalBehaviourData;
}

void freeExternalBehaviourData(tfel::system::ExternalBehaviourData* ptr) {
  delete ptr;
}

}  // end of extern "C"
