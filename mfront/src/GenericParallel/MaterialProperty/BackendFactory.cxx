/*!
 * \file   mfront/src/GenericParallel/MaterialProperty/BackendFactory.cxx
 *
 * \brief This file implements the `BackendFactory` class
 * \author Thomas Helfer
 * \date   15/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/GenericParallel/MaterialProperty/CUDABackend.hxx"
#include "MFront/GenericParallel/MaterialProperty/HIPBackend.hxx"
#include "MFront/GenericParallel/MaterialProperty/ParallelSTLBackend.hxx"
#include "MFront/GenericParallel/MaterialProperty/BackendFactory.hxx"

namespace mfront::generic_parallel::material_property {

  BackendFactory& BackendFactory::get() noexcept {
    static BackendFactory f;
    return f;
  }

  BackendFactory::BackendFactory() {
    this->add("cuda", [](const DataMap& opts) {
      return std::make_unique<CUDABackend>(opts);
    });
    this->add("hip", [](const DataMap& opts) {
      return std::make_unique<HIPBackend>(opts);
    });
    this->add("stlpar", [](const DataMap& opts) {
      return std::make_unique<ParallelSTLBackend>(opts);
    });
  }  // end of BackendFactory

  void BackendFactory::add(std::string_view n, Generator g) {
    if (!this->generators.insert({std::string{n}, g}).second) {
      tfel::raise("a backend named '" + std::string{n} +
                  "' has already been registred");
    }
  }  // end of add

  std::unique_ptr<AbstractBackend> BackendFactory::generate(
      std::string_view n, const DataMap& opts) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise("no backend named '" + std::string{n} + "' registred");
    }
    const auto& g = p->second;
    return [&g, &n, &opts]() -> std::unique_ptr<AbstractBackend> {
      auto r = std::unique_ptr<AbstractBackend>{};
      try {
        r = g(opts);
      } catch (std::exception& e) {
        tfel::raise("Error while generating backend '" + std::string{n} +
                    "' for the GenericParallelMaterialProperty interface\n" +
                    std::string{e.what()});
      } catch (...) {
        tfel::raise("Error while generating backend '" + std::string{n} +
                    "' for the GenericParallelMaterialProperty interface");
      }
      return r;
    }();
  }  // end of generate

  BackendFactory::~BackendFactory() = default;

}  // end of namespace mfront::generic_parallel::material_property
