/*!
 * \file   PlasticInelasticFlow.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
#include "MFront/BehaviourBrick/PlasticInelasticFlow.hxx"

namespace mfront {

  namespace bbrick {

    void PlasticInelasticFlow::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const std::string& id,
                                          const DataMap& d) {
      using namespace tfel::glossary;
      InelasticFlowBase::initialize(bd, dsl, id, d);
      tfel::raise_if(this->ihr == nullptr,
                     "PlasticInelasticFlow::initialize:"
                     "no isotropic hardening rule defined");
      if (id.empty()) {
        addStateVariable(bd, "strain", "p", Glossary::EquivalentPlasticStrain);
      } else {
        addStateVariable(
            bd, "strain", "p" + id,
            static_cast<const std::string&>(Glossary::EquivalentPlasticStrain) +
                id);
      }
    }  // end of PlasticInelasticFlow::initialize

    std::string PlasticInelasticFlow::buildFlowImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::string& id,
        const bool b) const {
      auto c = std::string{};
      tfel::raise_if(this->ihr == nullptr,
                     "PlasticInelasticFlow::buildFlowImplicitEquations :"
                     "no isotropic hardening rule defined");
      const auto snf = sp.getStressNormalisationFactor(bd);
      const auto R = "R" + id;
      const auto fp = "fp" + id;
      const auto seq = "seq" + id;
      const auto dseq_ds = "dseq" + id + "_ds" + id;
      if (b) {
        const auto dR_ddp = "dR" + id + "_ddp" + id;
        const auto dfp_ddp = "dfp" + id + "_ddp" + id;
        c += this->ihr->computeElasticLimitAndDerivative(id);
        c += fp + " = (" + seq + "-" + R + ")/("+snf+");\n";
        c += sp.computeDerivatives(bd, "p" + id, dseq_ds + "/(" + snf + ")");
        c += "if(" + dR_ddp + ">0){";
        c += dfp_ddp + " = -1*std::max(1.e-12,(" + dR_ddp +
             ")/("+snf+"));\n";
        c += "} else {\n";
        c += dfp_ddp + " = -1*std::min(-1.e-12,(" + dR_ddp +
             ")/("+snf+"));\n";
        c += "}\n";
        auto kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          c += khr->computeDerivatives("p", "-dseq_ds/(" + snf + ")", id,
                                       std::to_string(kid));
          ++kid;
        }
      } else {
        c += this->ihr->computeElasticLimit(id);
        c += fp + " = (" + seq + "-" + R + ")/("+snf+");\n";
      }
      return c;
    }  // end of PlasticInelasticFlow::buildFlowImplicitEquations

    PlasticInelasticFlow::~PlasticInelasticFlow() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
