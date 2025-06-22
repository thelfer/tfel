/*!
 * \file   src/System/ExternalBehaviourDescription.cxx
 * \brief
 * \author Helfer Thomas
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
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "TFEL/System/ExternalBehaviourDescription.hxx"

namespace tfel {
  namespace system {

    ExternalBehaviourData::ExternalBehaviourData() = default;
    ExternalBehaviourData::ExternalBehaviourData(ExternalBehaviourData&&) =
        default;
    ExternalBehaviourData::ExternalBehaviourData(const ExternalBehaviourData&) =
        default;
    ExternalBehaviourData& ExternalBehaviourData::operator=(
        ExternalBehaviourData&&) = default;
    ExternalBehaviourData& ExternalBehaviourData::operator=(
        const ExternalBehaviourData&) = default;

    ExternalBehaviourDescription::ExternalBehaviourDescription(
        const std::string& l, const std::string& f, const std::string& h) {
      auto& elm = ExternalLibraryManager::getExternalLibraryManager();
      const auto hypotheses = elm.getSupportedModellingHypotheses(l, f);
      if (std::find(hypotheses.begin(), hypotheses.end(), h) ==
          hypotheses.end()) {
        throw(std::runtime_error(
            "ExternalBehaviourDescription::ExternalBehaviourDescription : "
            "unsupported hypothesis '" +
            h + "' for behaviour '" + l +
            "'"
            "in library '" +
            l + "'"));
      }
      this->source = elm.getSource(l, f);
      this->btype = elm.getUMATBehaviourType(l, f);
      this->kinematic = elm.getUMATBehaviourKinematic(l, f);
      this->stype = elm.getUMATSymmetryType(l, f);
      this->etype = elm.getUMATElasticSymmetryType(l, f);
      this->isUPUIR =
          elm.isUMATBehaviourUsableInPurelyImplicitResolution(l, f, h);
      this->mpnames = elm.getUMATMaterialPropertiesNames(l, f, h);
      this->ivnames = elm.getUMATInternalStateVariablesNames(l, f, h);
      this->ivtypes = elm.getUMATInternalStateVariablesTypes(l, f, h);
      this->evnames = elm.getUMATExternalStateVariablesNames(l, f, h);
      this->requiresStiffnessTensor =
          elm.getUMATRequiresStiffnessTensor(l, f, h);
      this->requiresThermalExpansionCoefficientTensor =
          elm.getUMATRequiresThermalExpansionCoefficientTensor(l, f, h);
    }

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

  }  // end of namespace system

}  // end of namespace tfel

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
      char* copy = static_cast<char*>(::malloc(length + 1));
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
