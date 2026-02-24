/*!
 * \file   PlasticInelasticFlow.cxx
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
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/PlasticInelasticFlow.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> PlasticInelasticFlow::getOptions() const {
    auto opts = InelasticFlowBase::getOptions();

    opts.emplace_back("maximum_equivalent_stress_factor",
                      "a factor alpha which gives the maximal equivalent "
                      "stress allowed before damping the current Newton "
                      "step. This maximal equivalent stress is defined as "
                      "alpha * R where R is the current elastic limit. Alpha "
                      "must be greater than one",
                      OptionDescription::REAL);
    opts.emplace_back(
        "equivalent_stress_check_maximum_iteration_factor",
        "This factor beta gives the maximum iteration, computed by beta * "
        "iterMax where iterMax is maximum number of iterations of the Newton "
        "scheme, below which one checks if the maximal equivalent stress is "
        "significantly greater than the current elastic limit, i.e. if the "
        "current iteration number is greater than beta * iterMax, no checks "
        "is made on the maximum allowed equivalent stress. This option is "
        "only meaningfull if the `maximum_equivalent_stress_factor` option "
        "has been specified. By default, the value of this option is 0.5.",
        OptionDescription::REAL);
    return opts;
  }  // end of getOptions()

  void PlasticInelasticFlow::initialize(BehaviourDescription& bd,
                                        AbstractBehaviourDSL& dsl,
                                        const std::string& id,
                                        const DataMap& d) {
    using namespace tfel::glossary;
    const auto rsmax_n = std::string("maximum_equivalent_stress_factor");
    const auto rimax_n =
        std::string("equivalent_stress_check_maximum_iteration_factor");
    InelasticFlowBase::initialize(bd, dsl, id, d);
    if (d.count(rsmax_n) != 0) {
      const auto rsmax = d.at(rsmax_n);
      this->maximum_equivalent_stress_factor =
          tfel::utilities::convert<double>(rsmax);
      if (this->maximum_equivalent_stress_factor < 1) {
        tfel::raise(
            "PlasticInelasticFlow::initialize: invalid value for the option "
            "`" +
            rsmax_n + "` (value must be greater than one)");
      }
    }
    if (d.count(rimax_n) != 0) {
      if (d.count(rsmax_n) == 0) {
        tfel::raise("PlasticInelasticFlow::initialize: option `" + rimax_n +
                    "` is only meaningful if the option " + rsmax_n +
                    "` is set");
      }
      const auto imax = d.at(rimax_n);
      this->equivalent_stress_check_maximum_iteration_factor =
          tfel::utilities::convert<double>(imax);
      if ((this->equivalent_stress_check_maximum_iteration_factor < 0) ||
          (this->equivalent_stress_check_maximum_iteration_factor > 1)) {
        tfel::raise(
            "PlasticInelasticFlow::initialize: the value for option "
            "`equivalent_stress_check_maximum_iteration_factor` is "
            "invalid (must be positive and lower than 1)");
      }
    }
    tfel::raise_if(this->ihrs.empty(),
                   "PlasticInelasticFlow::initialize: "
                   "no isotropic hardening rule defined");
    if (!this->equivalent_strain_external_name.empty()) {
      addStateVariable(bd, "strain", "p",
                       this->equivalent_strain_external_name);
    } else {
      if (id.empty()) {
        addStateVariable(bd, "strain", "p", Glossary::EquivalentPlasticStrain);
      } else {
        addStateVariable(
            bd, "strain", "p" + id,
            static_cast<const std::string&>(Glossary::EquivalentPlasticStrain) +
                id);
      }
    }
  }  // end of initialize

  std::string PlasticInelasticFlow::buildFlowImplicitEquations(
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const std::string& id,
      const bool b) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    tfel::raise_if(this->ihrs.empty(),
                   "PlasticInelasticFlow::buildFlowImplicitEquations :"
                   "no isotropic hardening rule defined");
    const auto snf = sp.getStressNormalisationFactor(bd);
    const auto R = "R" + id;
    const auto fp = "fp" + id;
    const auto seq = "seq" + id;
    const auto dseq_ds = "dseq" + id + "_ds" + id;
    const auto damping = [this, &bd, &seq, &R]() {
      auto c = std::string{};
      if (this->maximum_equivalent_stress_factor >= 1) {
        const auto rmax =
            std::to_string(this->maximum_equivalent_stress_factor);
        const auto imax = std::to_string(
            this->equivalent_stress_check_maximum_iteration_factor);
        c += "if((" + seq + " > " + rmax + " * " + R + ") && ";
        c += "(this->iter < " + imax + " * (this->iterMax))){\n";
        if (getDebugMode()) {
          c += "std::cout << \"" + bd.getClassName() + "::computeFdF: ";
          c += "equivalent stress (\" << seq << \") is greater than its ";
          c += "threshold (\" << (";
          c += std::to_string(this->maximum_equivalent_stress_factor);
          c += "*" + R + ") << \")\\n\";\n";
        }
        c += "return false;\n";
        c += "}\n";
      }
      return c;
    }();
    auto c = std::string{};
    if (b) {
      const auto dR_dp = "dR" + id + "_dp" + id;
      const auto dfp_ddp = "dfp" + id + "_ddp" + id;
      c += computeElasticLimitAndDerivative(bd, this->ihrs, id);
      c += damping;
      if (this->save_yield_surface_radius) {
        c += "this->mfront_" + yield_surface_radius_external_name + " = " + R +
             ";\n";
      }
      c += fp + " = (" + seq + "-" + R + ")/(" + snf + ");\n";
      c += sp.generateImplicitEquationDerivatives(
          bd, "strain", "p" + id, dseq_ds + "/(" + snf + ")",
          this->sc->isNormalDeviatoric());
      c += "if(" + seq + "<" + sp.getEquivalentStressLowerBound(bd) + "){\n";
      c += "if(" + dR_dp + " > stress{0}){\n";
      c += dfp_ddp + " = -1*std::max(real(1.e-12),((this->theta) * " + dR_dp +
           ")/(" + snf + "));\n";
      c += "} else {\n";
      c += dfp_ddp + " = -1*std::min(-real(1.e-12),((this->theta) * (" + dR_dp +
           "))/(" + snf + "));\n";
      c += "}\n";
      c += "} else {\n";
      c += dfp_ddp + " = -((this->theta) * " + dR_dp + ") / (" + snf + ");\n";
      c += "}\n";
      auto kid = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        c += khr->generateImplicitEquationDerivatives(
            "p" + id, "-dseq" + id + "_ds" + id + "/(" + snf + ")", id,
            std::to_string(kid));
        ++kid;
      }
      if (this->isCoupledWithPorosityEvolution()) {
        const auto& f =
            bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Porosity);
        c += "dfp" + id + "_dd" + f.name + " = ";
        c += "(theta * d" + seq + "_d" + f.name + ")/(" + snf + ");\n";
      }
    } else {
      c += computeElasticLimit(bd, this->ihrs, id);
      c += damping;
      c += fp + " = (" + seq + "-" + R + ")/(" + snf + ");\n";
      if (this->save_yield_surface_radius) {
        c += "this->mfront_" + yield_surface_radius_external_name + " = " + R +
             ";\n";
      }
    }
    return c;
  }  // end of buildFlowImplicitEquations

  PlasticInelasticFlow::~PlasticInelasticFlow() = default;

}  // end of namespace mfront::bbrick
