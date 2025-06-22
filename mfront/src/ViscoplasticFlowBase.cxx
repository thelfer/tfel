/*!
 * \file   mfront/src/NortonInelasticFlow.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/ViscoplasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    void ViscoplasticFlowBase::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const std::string& id,
                                          const DataMap& d) {
      using namespace tfel::glossary;
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      InelasticFlowBase::initialize(bd, dsl, id, d);
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      if (id.empty()) {
        addStateVariable(bd, "strain", "p",
                         Glossary::EquivalentViscoplasticStrain);
      } else {
        addStateVariable(bd, "strain", "p" + id,
                         static_cast<const std::string&>(
                             Glossary::EquivalentViscoplasticStrain) +
                             id);
      }
      for (const auto& vn : {"vp", "dvp_dseqe", "dvp_dp"}) {
        bd.reserveName(uh, vn + id);
      }
    }  // end of ViscoplasticFlowBase::initialize

    std::string ViscoplasticFlowBase::buildFlowImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::string& id,
        const bool b) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto c = std::string{};
      if (b) {
        if (!this->ihrs.empty()) {
          c += computeElasticLimitAndDerivative(this->ihrs, id);
        }
        c += this->computeFlowRateAndDerivative(id);
        c += "fp" + id + " -= (this->dt) * vp" + id + ";\n";
        c += sp.generateImplicitEquationDerivatives(
            bd, "strain", "p" + id,
            "-(this->dt) * dvp" + id + "_dseqe" + id + " * dseq" + id + "_ds" +
                id,
            this->sc->isNormalDeviatoric());
        if (!this->ihrs.empty()) {
          c += "dfp" + id + "_ddp" + id + " += (this->dt) * dvp" + id +
               "_dseqe" + id + "*dR" + id + "_ddp" + id + ";\n";
        }
        auto kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          c += khr->generateImplicitEquationDerivatives(
              "p" + id,
              "(this->dt) * dvp" + id + "_dseqe" + id + " * dseq" + id + "_ds" +
                  id,
              id, std::to_string(kid));
          ++kid;
        }
        if (this->isCoupledWithPorosityEvolution()) {
          const auto& f =
              bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                  tfel::glossary::Glossary::Porosity);
          c += "dfp" + id + "_dd" + f.name + " = ";
          c += "- theta * (this->dt) * dvp" + id + "_dseqe" + id + " * ";
          c += "dseq" + id + "_d" + f.name + ";\n";
        }
      } else {
        if (!this->ihrs.empty()) {
          c += computeElasticLimit(this->ihrs, id);
        }
        c += this->computeFlowRate(id);
        c += "fp" + id + " -= (this->dt) * vp" + id + ";\n";
      }
      return c;
    }  // end of ViscoplasticFlowBase::buildFlowImplicitEquations

    ViscoplasticFlowBase::~ViscoplasticFlowBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
