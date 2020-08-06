/*!
 * \file   mfront/src/PorosityNucleationModelBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/PorosityNucleationModelBase.hxx"

namespace mfront {

  namespace bbrick {

    void PorosityNucleationModelBase::initialize(BehaviourDescription& bd,
                                                 AbstractBehaviourDSL&,
                                                 const std::string& id,
                                                 const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto raise = [](const std::string& m) {
        tfel::raise("PorosityNucleationModelBase::initialize: " + m);
      };  // end of raise
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // parsing options
      for (const auto& e : d) {
        if (e.first == "save_individual_porosity_increase") {
          if (!e.second.is<bool>()) {
            raise("'save_individual_porosity_increase' is not a boolean");
          }
          this->save_porosity_increase = e.second.get<bool>();
        } else if (e.first == "porosity_evolution_algorithm") {
          if (!e.second.is<std::string>()) {
            raise("'porosity_evolution_algorithm' is not a boolean");
          }
          const auto& a = e.second.get<std::string>();
          if (a == "standard_implicit_scheme") {
            this->porosity_evolution_algorithm =
                PorosityEvolutionAlgorithm::STANDARD_IMPLICIT_SCHEME;
          } else if (a == "staggered_scheme") {
            this->porosity_evolution_algorithm =
                PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
          } else {
            raise("internal error: unsupported porosity evolution algorithm");
          }
        }  // other options must be treated in child classes
      }
      addLocalVariable(bd, "real", "dfn" + id);
      if ((this->save_porosity_increase) ||
          (this->requiresSavingNucleatedPorosity())) {
        VariableDescription fn("real", "fn" + id, 1u, 0u);
        const auto g =
            tfel::glossary::Glossary::PorosityIncreaseDueToNucleation;
        if (id.empty()) {
          fn.setGlossaryName(g);
        } else {
          fn.setEntryName(g.getKey() + id);
        }
        bd.addAuxiliaryStateVariable(uh, fn);
      }
    }  // end of PorosityNucleationModelBase::initialize

    std::vector<OptionDescription> PorosityNucleationModelBase::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back(
          "save_individual_porosity_increase",
          "if appropriate, save the porosity increase induced "
          "by this inelastic flow in a dedicated auxiliary state variable",
          OptionDescription::BOOLEAN);
      opts.emplace_back("porosity_evolution_algorithm",
                        "reserved for internal use", OptionDescription::STRING);
      return opts;
    }  // end of PorosityNucleationModelBase::getOptions()

    void PorosityNucleationModelBase::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL&,
        const StressPotential&,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
        const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if ((this->save_porosity_increase) ||
          (this->requiresSavingNucleatedPorosity())) {
        CodeBlock uav;
        uav.code = "this->fn" + id + " += this->dfn" + id + ";\n";
        bd.setCode(uh, BehaviourData::UpdateAuxiliaryStateVariables, uav,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of PorosityNucleationModelBase::endTreatment

    PorosityNucleationModelBase::~PorosityNucleationModelBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
