/*!
 * \file   mfront/src/StrainRateSensitiveIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx"
#include "MFront/BehaviourBrick/StrainRateSensitivityFactorFactory.hxx"
#include "MFront/BehaviourBrick/StrainRateSensitiveIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  static std::string getIsotropicHardeningRuleId(const std::string& id,
                                                 const int n) {
    return (!id.empty() ? id + "_" : "") + "riihr_" + std::to_string(n);
  }  // end of getIsotropicHardeningRuleId

  static std::string getIsotropicHardeningRuleValue(const std::string& name,
                                                    const std::string& fid,
                                                    const std::string& id,
                                                    const int n) {
    return (id.empty() ? name + fid : name + fid + "_" + id) + "_riihr_" +
           std::to_string(n);
  }  // end of getIsotropicHardeningRuleId

  std::vector<OptionDescription>
  StrainRateSensitiveIsotropicHardeningRule::getOptions() const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("rate_independent_isotropic_hardening",
                      "choice of an isotropic hardening rule",
                      OptionDescription::DATASTRUCTURES);
    opts.emplace_back("rate_sensitivity_factor",
                      "choice of an rate sensitivity factor. "
                      "The following corrections are available: "
                      "JohnsonCook and CowperSymonds",
                      OptionDescription::DATASTRUCTURE);
    return opts;
  }  // end of getOptions

  void StrainRateSensitiveIsotropicHardeningRule::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id,
      const DataMap& d) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto raise = [](const std::string& m) {
      tfel::raise("InelasticFlowBase::initialize: " + m);
    };  // end of raise
    auto getDataStructure = [&raise](const std::string& n, const Data& ds) {
      if (ds.is<std::string>()) {
        tfel::utilities::DataStructure nds;
        nds.name = ds.get<std::string>();
        return nds;
      }
      if (!ds.is<tfel::utilities::DataStructure>()) {
        raise("invalid data type for entry '" + n + "'");
      }
      return ds.get<tfel::utilities::DataStructure>();
    };  // end of getDataStructure
    mfront::bbrick::check(d, this->getOptions());
    for (const auto& e : d) {
      if (e.first == "rate_independent_isotropic_hardening") {
        auto add_isotropic_hardening_rule =
            [this, &bd, &dsl, &id,
             &fid](const tfel::utilities::DataStructure& ds) {
              if (getVerboseMode() >= VERBOSE_DEBUG) {
                getLogStream()
                    << "adding isotropic hardening rule '" + ds.name + "'"
                    << std::endl;
              }
              auto& rf = IsotropicHardeningRuleFactory::getFactory();
              const auto ihr = rf.generate(ds.name);
              ihr->initialize(bd, dsl, fid,
                              getIsotropicHardeningRuleId(
                                  id, static_cast<int>(this->ihrs.size())),
                              ds.data);
              this->ihrs.push_back(ihr);
            };
        if (e.second.is<std::vector<Data>>()) {
          for (const auto& ird : e.second.get<std::vector<Data>>()) {
            add_isotropic_hardening_rule(getDataStructure(e.first, ird));
          }
        } else {
          const auto ds = getDataStructure(e.first, e.second);
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream()
                << "adding rate independent isotropic hardening rule '"
                << ds.name << "'" << std::endl;
          }
          auto& rf = IsotropicHardeningRuleFactory::getFactory();
          const auto ihr = rf.generate(ds.name);
          ihr->initialize(bd, dsl, fid, getIsotropicHardeningRuleId(id, 0),
                          ds.data);
          this->ihrs.push_back(ihr);
        }
      } else if (e.first == "rate_sensitivity_factor") {
        const auto ds = getDataStructure(e.first, e.second);
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          getLogStream() << "adding rate sensitivity factor '" << ds.name << "'"
                         << std::endl;
        }
        if (this->rsf != nullptr) {
          raise(
              "a rate independant isotropic hardening rule "
              "has already been defined");
        }
        auto& rf = StrainRateSensitivityFactorFactory::getFactory();
        this->rsf = rf.generate(ds.name);
        this->rsf->initialize(bd, dsl, id, "", ds.data);
      } else {
        raise("invalid option '" + e.first + "'");
      }
    }
    if (this->ihrs.empty()) {
      raise("no rate independant isotropic hardening rule defined");
    }
    if (this->rsf == nullptr) {
      raise("no strain rate sensitivity factor defined");
    }
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_ddp" + fid;
    bd.reserveName(uh, Rel);
    bd.reserveName(uh, R);
    bd.reserveName(uh, dR);
  }  // end of initialize

  std::string
  StrainRateSensitiveIsotropicHardeningRule::computeElasticPrediction(
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    auto c = std::string{};
    for (int idx = 0; const auto& ihr : this->ihrs) {
      c += ihr->computeElasticPrediction(bd, fid,
                                         getIsotropicHardeningRuleId(id, idx));
      ++idx;
    }
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    c += "const auto " + Rel + " = ";
    bool first = true;
    for (int idx = 0; idx != static_cast<int>(this->ihrs.size()); ++idx) {
      const auto Rid = getIsotropicHardeningRuleValue("Rel", fid, id, idx);
      if (!first) {
        c += " + ";
      }
      c += Rid;
      first = false;
    }
    c += ";\n";
    return c;
  }  // end of computeElasticPrediction

  std::string StrainRateSensitiveIsotropicHardeningRule::computeElasticLimit(
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    auto c = std::string{};
    for (int idx = 0; const auto& ihr : this->ihrs) {
      c += ihr->computeElasticLimit(bd, fid,
                                    getIsotropicHardeningRuleId(id, idx));
      ++idx;
    }
    c += this->rsf->computeStrainRateSensitivityFactor(bd, fid, id);
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    c += "const auto " + R + " = (";
    bool first = true;
    for (int idx = 0; idx != static_cast<int>(this->ihrs.size()); ++idx) {
      const auto Rid = getIsotropicHardeningRuleValue("R", fid, id, idx);
      if (!first) {
        c += " + ";
      }
      c += Rid;
      first = false;
    }
    c += ") * (";
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    c += Rs;
    c += ");\n";
    return c;
  }  // end of computeElasticLimit

  std::string
  StrainRateSensitiveIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    auto c = std::string{};
    for (int idx = 0; const auto& ihr : this->ihrs) {
      c += ihr->computeElasticLimitAndDerivative(
          bd, fid, getIsotropicHardeningRuleId(id, idx));
      ++idx;
    }
    c +=
        this->rsf->computeStrainRateSensitivityFactorAndDerivative(bd, fid, id);
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    c += "const auto " + R + " = (";
    bool first = true;
    for (int idx = 0; idx != static_cast<int>(this->ihrs.size()); ++idx) {
      const auto Rid = getIsotropicHardeningRuleValue("R", fid, id, idx);
      if (!first) {
        c += " + ";
      }
      c += Rid;
      first = false;
    }
    c += ") * (";
    c += Rs;
    c += ");\n";
    const auto dR = "d" + R + "_ddp" + fid;
    const auto dRs = "d" + Rs + "_ddp" + fid;
    c += "const auto " + dR + " = (";
    first = true;
    for (int idx = 0; idx != static_cast<int>(this->ihrs.size()); ++idx) {
      const auto Rid = getIsotropicHardeningRuleValue("R", fid, id, idx);
      const auto dRid = "d" + Rid + "_ddp" + fid;
      if (!first) {
        c += " + ";
      }
      c += dRid;
      first = false;
    }
    c += ") * " + Rs + " + (";
    first = true;
    for (int idx = 0; idx != static_cast<int>(this->ihrs.size()); ++idx) {
      const auto Rid = getIsotropicHardeningRuleValue("R", fid, id, idx);
      if (!first) {
        c += " + ";
      }
      c += Rid;
      first = false;
    }
    c += ") * " + dRs + ";\n";
    return c;
  }  // end of computeElasticLimitAndDerivative

  void StrainRateSensitiveIsotropicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id) const {
    for (int idx = 0; const auto& ihr : this->ihrs) {
      ihr->endTreatment(bd, dsl, fid, getIsotropicHardeningRuleId(id, idx));
      ++idx;
    }
    this->rsf->endTreatment(bd, dsl, fid, id);
  }  // end of endTreatment

  StrainRateSensitiveIsotropicHardeningRule::
      ~StrainRateSensitiveIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
