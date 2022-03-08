/*!
 * \file  mfront/src/CamClayStressPotential.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/11/2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/CamClayStressPotential.hxx"

namespace mfront::bbrick {

  CamClayStressPotential::CamClayStressPotential() = default;

  std::string CamClayStressPotential::getName() const { return "CamClay"; }

  std::vector<CamClayStressPotential::Hypothesis>
  CamClayStressPotential::getSupportedModellingHypotheses(
      const BehaviourDescription&, const AbstractBehaviourDSL& dsl) const {
    const auto dmh = dsl.getDefaultModellingHypotheses();
    std::vector<Hypothesis> mh(dmh.begin(), dmh.end());
    return mh;
  }  // end of getSupportedModellingHypotheses

  std::vector<OptionDescription> CamClayStressPotential::getOptions(
      const BehaviourDescription&, const bool) const {
    auto opts = StressPotentialBase::getGeneralOptions();
    const auto th_opts =
        StressPotentialBase::getIsotropicThermalExpansionOptions();
    std::copy(th_opts.begin(), th_opts.end(), std::back_inserter(opts));
    //
    opts.emplace_back("unloading_reloading_slope", "unloading/reloading slope",
                      OptionDescription::MATERIALPROPERTY,
                      std::vector<std::string>{}, std::vector<std::string>{});
    opts.emplace_back("initial_void_ratio", "initial void ratio",
                      OptionDescription::MATERIALPROPERTY,
                      std::vector<std::string>{}, std::vector<std::string>{});
    opts.emplace_back(
        "poisson_ratio", tfel::glossary::Glossary::PoissonRatio,
        OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
        std::vector<std::string>{"shear_modulus",
                                 "deduce_shear_modulus_from_poisson_ratio"});
    opts.emplace_back(
        "shear_modulus", tfel::glossary::Glossary::ShearModulus,
        OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
        std::vector<std::string>{"poisson_ratio",
                                 "deduce_shear_modulus_from_poisson_ratio"});
    opts.emplace_back(
        "pressure_threshold",
        "threshold below which the behaviour is linear elastic (must be "
        "strictly positive)",
        OptionDescription::REAL, std::vector<std::string>{},
        std::vector<std::string>{});
    opts.emplace_back(
        "deduce_shear_modulus_from_poisson_ratio",
        "boolean stating if the shear modulus shall be deduced from the "
        "Poisson ratio. If this option is true, the Poisson ratio is "
        "automatically declared as a material property. Otherwise, the shear "
        "modulus is automatically declared as a material property.",
        OptionDescription::BOOLEAN, std::vector<std::string>{},
        std::vector<std::string>{"poisson_ratio", "shear_modulus"});
    opts.emplace_back(
        "incremental_deviatoric_part",
        "boolean stating if the deviatoric part of the stress/strain "
        "relationship shall be treated incrementally. This option is only "
        "valid if the shear modulus is deduced from the Poisson ratio. In this "
        "case, an incremental treatment is assumed by default.",
        OptionDescription::BOOLEAN, std::vector<std::string>{},
        std::vector<std::string>{"shear_modulus"});
    return opts;
  }  // end of getOptions

  void CamClayStressPotential::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //
    //
    StressPotentialBase::initialize(bd, dsl, d);
    //
    bd.appendToIncludes("#include \"TFEL/Material/CamClayElasticityStressPotential.hxx\"");
    //
    VariableDescription dsig_deel("StiffnessTensor", "dsig_deel", 1u, 0u);
    dsig_deel.description =
        "derivative of the stress with respect ot the elastic strain";
    bd.addLocalVariable(uh, dsig_deel);
    //
    CodeBlock init;
    auto treat_option = [&init, &bd, &dsl, &d](
                            const std::string& o,  // option
                            const std::string& t,  // type
                            const std::string& n,  // variable name
                            const std::string& e,  // external name,
                            const bool use_glossary) {
      bd.addLocalVariable(uh, VariableDescription(t, n + "_ets", 1u, 0u));
      if (d.count(o) == 0) {
        VariableDescription v(t, n, 1u, 0u);
        if (use_glossary) {
          v.setGlossaryName(e);
        } else {
          v.setEntryName(e);
        }
        bd.addMaterialProperty(uh, v);
        init.code += "this->" + n + "_ets = this->" + n + ";\n";
      } else {
        auto mp = getMaterialProperty(dsl, d, o);
        if (use_glossary) {
          const auto& g = tfel::glossary::Glossary::getGlossary();
          declareParameterOrLocalVariable(bd, mp, t, n, g.getGlossaryEntry(e));
        } else {
          declareParameterOrLocalVariable(bd, mp, t, n, e);
        }
        init.code += generateMaterialPropertyInitializationCode(dsl, bd, n,
                                                                n + "_ets", mp);
      }
    };
    // initial void ratio
    if (d.count("initial_void_ratio") != 0) {
      const auto d_e0 = d.at("initial_void_ratio");
      if (d_e0.is<double>()) {
        const auto e0 = d_e0.get<double>();
        if ((e0 < 0) || (e0 > 1)) {
          tfel::raise("CamClayStressPotential::initialize: invalid e0 value");
        }
      }
      if (d_e0.is<int>()) {
        if (d_e0.get<int>() != 0) {
          tfel::raise("CamClayStressPotential::initialize: invalid e0 value");
        }
      }
    }
    //
    treat_option("initial_void_ratio", "real", "mfront_ccb_e0",
                 "InitialVoidRatio", false);
    // unloading / reloading slope
    treat_option("unloading_reloading_slope", "stress", "mfront_ccb_kappa",
                 "UnloadingReloadingSlope", false);
    // Pressure threshold
    treat_option("pressure_threshold", "stress", "mfront_ccb_pmin",
                 "PressureThreshold", false);
    //
    const auto b = d.count("deduce_shear_modulus_from_poisson_ratio") != 0;
    const auto b_nu = d.count("poisson_ratio") != 0;
    const auto b_mu = d.count("shear_modulus") != 0;
    const auto b_incr = d.count("incremental_deviatoric_part") != 0;
    if ((b && b_nu) || (b && b_mu) || (b_nu && b_mu)) {
      tfel::raise(
          "CamClayStressPotential::initialize: "
          "options `deduce_shear_modulus_from_poisson_ratio`, `poisson_ratio` "
          "and "
          "`shear_modulus` are exclusive");
    }
    if ((!b_nu) && (!b_mu) && (!b)) {
      tfel::raise(
          "CamClayStressPotential::initialize: "
          "one of the options `deduce_shear_modulus_from_poisson_ratio`, "
          "`poisson_ratio` and "
          "`shear_modulus` is required");
    }
    const auto deduce_shear_modulus_from_poisson_ratio = [&d, b_nu, b_mu] {
      if (b_nu) {
        return true;
      }
      if (b_mu) {
        return false;
      }
      return d.at("deduce_shear_modulus_from_poisson_ratio").get<bool>();
    }();
    if ((!deduce_shear_modulus_from_poisson_ratio) && (b_incr)) {
      tfel::raise(
          "CamClayStressPotential::initialize: "
          "option `incremental_deviatoric_part` is only meaningful if the "
          "Poisson ratio is constant");
    }
    const auto incremental_deviatoric_part =
        [&d, deduce_shear_modulus_from_poisson_ratio, b_incr] {
          if (deduce_shear_modulus_from_poisson_ratio) {
            if (!b_incr) {
              return true;
            }
            return d.at("incremental_deviatoric_part").get<bool>();
          }
          return false;
        }();
    // declaring the initial stress and is initialisation, if required
    if (incremental_deviatoric_part) {
      auto s0 = VariableDescription("StressStensor", "mfront_ccb_initial_stress",
                                    1u, 0u);
      s0.description = "stress at the beginning of the time step";
      bd.addLocalVariable(uh, s0);
      init.code += "this->mfront_ccb_initial_stress = this->sig;\n";
    }
    // treating the Poisson ratio or the shear modulus
    if (b) {
      // user declared the deduce_shear_modulus_from_poisson_ratio option, so
      // treat_option will automatically declare the Poisson ratio or the
      // shear modulus as material properties
      if (d.at("deduce_shear_modulus_from_poisson_ratio").get<bool>()) {
        treat_option("poisson_ratio", "real", "nu", "PoissonRatio", true);
      } else {
        treat_option("shear_modulus", "stress", "mu", "ShearModulus", true);
      }
    } else {
      if ((!b_nu) && (!b_mu)) {
        tfel::raise(
            "CamClayStressPotential::initialize: "
            "neither the Poisson ratio nor the shear modulus is specified");
      }
      if (b_nu) {
        treat_option("poisson_ratio", "real", "nu", "PoissonRatio", true);
      }
      if (b_mu) {
        treat_option("shear_modulus", "stress", "mu", "ShearModulus", true);
      }
    }
    bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    // stress computations
    auto m = std::string {}; // elastic prediction
    m = "StressStensor computeElasticPrediction() const{\n"
        "auto mfront_elastic_prediction = StressStensor{};\n"
        "auto mfront_dummy_stress_derivative = "
        "tfel::math::st2tost2<N,stress>{};\n";
    CodeBlock s_mts;  // computeThermodynamicForces at the middle of time step
    CodeBlock s_ets;  // computeThermodynamicForces at the end of  time step
    if (deduce_shear_modulus_from_poisson_ratio) {
      if (incremental_deviatoric_part) {
        m += "std::tie(mfront_elastic_prediction, "
             "mfront_dummy_stress_derivative) = "
             "computeCamClayElasticityII("
             "this->mfront_ccb_initial_stress, "
             "this->eel, "
             "(" +
             bd.getClassName() +
             "::theta) * (this->deto), "
             "this->mfront_ccb_e0, "
             "this->mfront_ccb_kappa, "
             "this->nu, this->mfront_ccb_pmin\n);\n";
        s_mts.code =
            "std::tie(this->sig, this->dsig_deel) = "
            "computeCamClayElasticityII("
            "this->mfront_ccb_initial_stress, "
            "this->eel, "
            "(" +
            bd.getClassName() +
            "::theta) * (this->deel), "
            "this->mfront_ccb_e0, "
            "this->mfront_ccb_kappa, "
            "this->nu, this->mfront_ccb_pmin\n);\n";
        s_ets.code =
            "std::tie(this->sig, this->dsig_deel) = "
            "computeCamClayElasticityII("
            "this->mfront_ccb_initial_stress, "
            "this->eel - this->deel, "
            "this->deel, "
            "this->mfront_ccb_e0_ets, "
            "this->mfront_ccb_kappa_ets, "
            "this->nu_ets, this->mfront_ccb_pmin);\n";
      } else {
        m += "std::tie(mfront_elastic_prediction, "
             "mfront_dummy_stress_derivative) = "
             "computeCamClayElasticity(this->eel + (" +
             bd.getClassName() +
             "::theta) * (this->deto), "
             "this->mfront_ccb_e0, "
             "this->mfront_ccb_kappa, "
             "this->nu, this->mfront_ccb_pmin\n);\n";
        s_mts.code =
            "std::tie(this->sig, this->dsig_deel) = "
            "computeCamClayElasticity(this->eel + (" +
            bd.getClassName() +
            "::theta) * (this->deel), "
            "this->mfront_ccb_e0, "
            "this->mfront_ccb_kappa, "
            "this->nu, this->mfront_ccb_pmin\n);\n";
        s_ets.code =
            "std::tie(this->sig, this->dsig_deel) = "
            "computeCamClayElasticity(this->eel, "
            "this->mfront_ccb_e0_ets, "
            "this->mfront_ccb_kappa_ets, "
            "this->nu_ets, this->mfront_ccb_pmin);\n";
      }
    } else {
      m += "std::tie(mfront_elastic_prediction, "
           "mfront_dummy_stress_derivative) = "
           "computeCamClayElasticityIII("
           "this->eel + (" +
           bd.getClassName() +
           "::theta) * (this->deto), "
           "this->mfront_ccb_e0, "
           "this->mfront_ccb_kappa, "
           "this->mu, this->mfront_ccb_pmin);\n";
      s_mts.code =
          "std::tie(this->sig, this->dsig_deel) = "
          "computeCamClayElasticityIII("
          "this->eel + (" +
          bd.getClassName() +
          "::theta) * (this->deel), "
          "this->mfront_ccb_e0, "
          "this->mfront_ccb_kappa, "
          "this->mu, this->mfront_ccb_pmin);\n";
      s_ets.code =
          "std::tie(this->sig, this->dsig_deel) = "
          "computeCamClayElasticityIII(this->eel, "
          "this->mfront_ccb_e0_ets, "
          "this->mfront_ccb_kappa_ets, "
          "this->mu_ets, this->mfront_ccb_pmin);\n";
    }
    m += "  return mfront_elastic_prediction;\n";
    m += "}\n\n";
    bd.appendToMembers(uh, m, false);
    bd.setCode(uh, BehaviourData::ComputeThermodynamicForces, s_mts,
               BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    bd.setCode(uh, BehaviourData::ComputeFinalThermodynamicForces, s_ets,
               BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
  }  // end of initialize

  void CamClayStressPotential::completeVariableDeclaration(
      BehaviourDescription& bd, const AbstractBehaviourDSL&) const {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "CamClayStressPotential::completeVariableDeclaration: begin\n";
    }
    // elastic strain
    StressPotentialBase::declareElasticStrainIfRequired(bd);
    //
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "CamClayStressPotential::completeVariableDeclaration: end\n";
    }
  }  // end of completeVariableDeclaration

  void CamClayStressPotential::endTreatment(
      BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
    // modelling hypotheses supported by the behaviour
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto bmh = bd.getModellingHypotheses();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "CamClayStressPotentialBase::endTreatment: begin\n";
    }
    // implicit equation associated with the elastic strain
    CodeBlock integrator;
    auto broken_test = getBrokenTest(bd, false);
    if (!broken_test.empty()) {
      integrator.code += "if(" + broken_test + "){\n";
      integrator.code += "  this->deel = -(this->eel);\n";
      integrator.code += "  return true;\n";
      integrator.code += "}\n";
    }
    integrator.code += "feel -= this->deto;\n";
    bd.setCode(uh, BehaviourData::Integrator, integrator,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    //
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
    CodeBlock to;
    if (idsl.getSolver().usesJacobian()) {
      to.attributes["requires_jacobian_decomposition"] = true;
      to.attributes["uses_get_partial_jacobian_invert"] = true;
    }
    to.code += "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n";
    to.code += "this->Dt = this->dsig_deel;\n";
    if (idsl.getSolver().usesJacobian()) {
      to.code +=
          "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
          "  Stensor4 Je;\n"
          "  getPartialJacobianInvert(Je);\n"
          "  this->Dt = (this->dsig_deel) * Je;\n";
    }
    to.code += "} else {\n return false;\n}\n";
    bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                    BehaviourData::hasConsistentTangentOperator, true, true);
    bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               BehaviourData::ComputeTangentOperator, to,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "CamClayStressPotentialBase::endTreatment: end\n";
    }
  }  // end of endTreatment

  std::vector<
      std::tuple<std::string, std::string, mfront::SupportedTypes::TypeFlag>>
  CamClayStressPotential::getStressDerivatives(
      const BehaviourDescription&) const {
    auto r = std::vector<std::tuple<std::string, std::string,
                                    mfront::SupportedTypes::TypeFlag>>{};
    r.push_back(std::make_tuple("theta * dsig_deel", std::string("eel"),
                                SupportedTypes::STENSOR));
    return r;
  }  // end of getStressDerivatives

  std::string CamClayStressPotential::generateImplicitEquationDerivatives(
      const BehaviourDescription&,
      const std::string& t,
      const std::string& v,
      const std::string& dfv_ds,
      const bool) const {
    auto c = std::string{};
    const auto vf = SupportedTypes::getTypeFlag(t);
    c = "df" + v + "_ddeel += ";
    if (vf == SupportedTypes::SCALAR) {
      c += "(this->theta) * ((" + dfv_ds + ") | (this->dsig_deel));\n";
    } else if (vf == SupportedTypes::STENSOR) {
      c += "(this->theta) * (" + dfv_ds + ") * (this->dsig_deel);\n";
    } else {
      tfel::raise(
          "CamClayStressPotential::generateImplicitEquationDerivatives: "
          "unsupported variable type");
    }
    return c;
  }  // end of generateImplicitEquationDerivatives

  void CamClayStressPotential::computeElasticPrediction(
      BehaviourDescription& bd) const {
    CodeBlock i;
    i.code = "const auto sigel = this->computeElasticPrediction();\n";
    bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               BehaviourData::BeforeInitializeLocalVariables, i,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }  // end of computeElasticPrediction

  std::string CamClayStressPotential::getStressNormalisationFactor(
      const BehaviourDescription&) const {
    return "1 / (this->mfront_ccb_kappa_ets)";
  }  // end of getStressNormalisationFactor

  std::string CamClayStressPotential::getEquivalentStressLowerBound(
      const BehaviourDescription&) const {
    return "((this->relative_value_for_the_equivalent_stress_lower_bound)"
           " / (this->mfront_ccb_kappa_ets))";
  }  // end of getEquivalentStressLowerBound

  CamClayStressPotential::~CamClayStressPotential() = default;

}  // end of namespace mfront::bbrick
