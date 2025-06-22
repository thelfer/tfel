/*!
 * \file   BehaviourBrickRequirementManager.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   29 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <utility>
#include <algorithm>
#include <stdexcept>
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/BehaviourData.hxx"
#include "MFront/BehaviourBrick/Provider.hxx"
#include "MFront/BehaviourBrick/Requirement.hxx"
#include "MFront/BehaviourBrick/RequirementManager.hxx"

namespace mfront {

  namespace bbrick {

    static void throwIfRequirementIsAlreadyDefined(
        const std::vector<std::shared_ptr<Requirement>>& rqs,
        const Requirement& r) {
      auto test = [&r](const std::shared_ptr<Requirement>& rr) {
        return rr->name == r.name;
      };
      if (std::find_if(rqs.begin(), rqs.end(), test) != rqs.end()) {
        throw(
            std::runtime_error("RequirementManager::addRequirement : "
                               "requirement '" +
                               r.name + "' already registred"));
      }
    }

    static void throwIfRequirementHasAnIncompatibleProvider(
        const std::vector<std::shared_ptr<Provider>>& ps,
        const Requirement& r,
        const bool b) {
      for (const auto& p : ps) {
        if (p->handleRequirement(r, b)) {
          return;
        }
      }
    }

    RequirementManager::RequirementManager(const bool b)
        : checkUnits(b) {}  // end of RequirementManager::RequirementManager

    RequirementManager::RequirementManager(const mfront::BehaviourData& bd,
                                           const bool b)
        : checkUnits(b) {
      for (const auto& v : bd.getMaterialProperties()) {
        this->addMaterialPropertyProvider(v, bd.getExternalName(v.name));
      }
      for (const auto& v : bd.getIntegrationVariables()) {
        this->addIntegrationVariableProvider(v, bd.getExternalName(v.name));
      }
      for (const auto& v : bd.getAuxiliaryStateVariables()) {
        this->addAuxiliaryStateVariableProvider(v, bd.getExternalName(v.name));
      }
      for (const auto& v : bd.getExternalStateVariables()) {
        this->addExternalStateVariableProvider(v, bd.getExternalName(v.name));
      }
      for (const auto& v : bd.getLocalVariables()) {
        this->addLocalVariableProvider(v, bd.getExternalName(v.name));
      }
      for (const auto& v : bd.getStaticVariables()) {
        this->addStaticVariableProvider(v, bd.getExternalName(v.name));
      }
      for (const auto& v : bd.getParameters()) {
        if (v.type == "real") {
          this->addParameterProvider(v, bd.getExternalName(v.name));
        }
      }
    }  // end of RequirementManager::RequirementManager

    std::vector<std::string> RequirementManager::getUnresolvedRequirements()
        const {
      auto rn = std::vector<std::string>{};
      for (const auto& r : this->requirements) {
        if (!this->hasProvider(*r)) {
          rn.push_back(r->name);
        }
      }
      return rn;
    }  // end of Provider::getUnresolvedRequirements

    bool RequirementManager::hasProvider(const std::string& n) const {
      return this->hasProvider(this->getRequirement(n));
    }

    bool RequirementManager::hasProvider(const Requirement& r) const {
      bool found = false;
      for (auto p = this->providers.begin();
           (p != this->providers.end() && (!found)); ++p) {
        found = (*p)->handleRequirement(r, this->checkUnits);
      }
      return found;
    }

    const Requirement& RequirementManager::getRequirement(
        const std::string& n) const {
      auto pr = this->requirements.begin();
      while (pr != this->requirements.end()) {
        if ((*pr)->name == n) {
          break;
        }
        ++pr;
      }
      if (pr == this->requirements.end()) {
        throw(
            std::runtime_error("RequirementManager::getRequirement : "
                               "no requirement named '" +
                               n + "'"));
      }
      return *(*pr);
    }

    void RequirementManager::addRequirement(
        const std::shared_ptr<Requirement>& r) {
      throwIfRequirementIsAlreadyDefined(this->requirements, *r);
      // check if this requirement has a provider and that this
      // provider is compatible with it
      throwIfRequirementHasAnIncompatibleProvider(this->providers, *r,
                                                  this->checkUnits);
      this->requirements.push_back(r);
    }  // end of void RequirementManager::addRequirement

    void RequirementManager::addRequirement(const Requirement& r) {
      throwIfRequirementIsAlreadyDefined(this->requirements, r);
      this->addRequirement(std::make_shared<Requirement>(r));
    }

    void RequirementManager::addRequirement(Requirement&& r) {
      throwIfRequirementIsAlreadyDefined(this->requirements, r);
      this->addRequirement(std::make_shared<Requirement>(std::move(r)));
    }

    std::vector<std::shared_ptr<Provider>>::const_iterator
    RequirementManager::getProviderIterator(const std::string& e) const {
      return std::find_if(this->providers.begin(), this->providers.end(),
                          [&e](const std::shared_ptr<Provider> p) {
                            return p->getExternalName() == e;
                          });
    }

    std::vector<std::shared_ptr<Provider>>::iterator
    RequirementManager::getProviderIterator(const std::string& e) {
      return std::find_if(this->providers.begin(), this->providers.end(),
                          [&e](const std::shared_ptr<Provider> p) {
                            return p->getExternalName() == e;
                          });
    }

    const Provider& RequirementManager::getProvider(
        const std::string& e) const {
      const auto p = this->getProviderIterator(e);
      if (p == this->providers.end()) {
        throw(
            std::runtime_error("RequirementManager::getProvider : "
                               "a provider for quantity '" +
                               e +
                               "' "
                               "declared"));
      }
      return *(*p);
    }

    void RequirementManager::check(const Provider& p) const {
      const auto& e = p.getExternalName();
      if (this->getProviderIterator(e) != this->providers.end()) {
        throw(
            std::runtime_error("RequirementManager::check : "
                               "a provider for quantity '" +
                               e +
                               "' "
                               "has already been declared"));
      }
      for (const auto& r : this->requirements) {
        if (p.handleRequirement(*r, this->checkUnits)) {
          return;
        }
      }
    }  // end of RequirementManager::check

    void RequirementManager::addMaterialPropertyProvider(
        const mfront::VariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<MaterialPropertyProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }

    void RequirementManager::addMaterialPropertyProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s) {
      const auto p = std::make_shared<MaterialPropertyProvider>(t, n, e, s);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addMaterialPropertyProvider

    void RequirementManager::addAuxiliaryStateVariableProvider(
        const VariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<AuxiliaryStateVariableProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addAuxiliaryStateVariableProvider

    void RequirementManager::addAuxiliaryStateVariableProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s) {
      const auto p =
          std::make_shared<AuxiliaryStateVariableProvider>(t, n, e, s);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addAuxiliaryStateVariableProvider

    void RequirementManager::addExternalStateVariableProvider(
        const mfront::VariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<ExternalStateVariableProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addExternalStateVariableProvider

    void RequirementManager::addExternalStateVariableProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s) {
      const auto p =
          std::make_shared<ExternalStateVariableProvider>(t, n, e, s);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addExternalStateVariableProvider

    void RequirementManager::addIntegrationVariableProvider(
        const mfront::VariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<IntegrationVariableProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addIntegrationVariableProvider

    void RequirementManager::addIntegrationVariableProvider(
        const std::string& t,
        const std::string& n,
        const std::string& e,
        const unsigned short s) {
      const auto p = std::make_shared<IntegrationVariableProvider>(t, n, e, s);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addIntegrationVariableProvider

    void RequirementManager::addStaticVariableProvider(
        const mfront::StaticVariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<StaticVariableProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addStaticVariableProvider

    void RequirementManager::addStaticVariableProvider(const std::string& t,
                                                       const std::string& n,
                                                       const std::string& e) {
      const auto p = std::make_shared<StaticVariableProvider>(t, n, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addStaticVariableProvider

    void RequirementManager::addParameterProvider(
        const mfront::VariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<ParameterProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addParameterProvider

    void RequirementManager::addParameterProvider(const std::string& t,
                                                  const std::string& n,
                                                  const std::string& e) {
      const auto p = std::make_shared<ParameterProvider>(t, n, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addParameterProvider

    void RequirementManager::addLocalVariableProvider(
        const mfront::VariableDescription& v, const std::string& e) {
      const auto p = std::make_shared<LocalVariableProvider>(v, e);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addLocalVariableProvider

    void RequirementManager::addLocalVariableProvider(const std::string& t,
                                                      const std::string& n,
                                                      const std::string& e,
                                                      const unsigned short s) {
      const auto p = std::make_shared<LocalVariableProvider>(t, n, e, s);
      this->check(*p);
      this->providers.push_back(p);
    }  // end of RequirementManager::addLocalVariableProvider

    RequirementManager::~RequirementManager() = default;

  }  // namespace bbrick

}  // namespace mfront
