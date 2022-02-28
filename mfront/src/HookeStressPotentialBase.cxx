/*!
 * \file   mfront/src/HookeStressPotentialBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/HookeStressPotentialBase.hxx"

namespace mfront::bbrick {

  const char* const HookeStressPotentialBase::residualStiffnessFactor =
      "residual_stiffness_factor";

  HookeStressPotentialBase::HookeStressPotentialBase() = default;

  std::vector<OptionDescription>
  HookeStressPotentialBase::getIsotropicBehaviourOptions() {
    auto opts = std::vector<OptionDescription>{};
    opts.emplace_back(
        "young_modulus", tfel::glossary::Glossary::YoungModulus,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"poisson_ratio"},
        std::vector<std::string>{
            "young_modulus1", "young_modulus2", "young_modulus3",
            "poisson_ratio12", "poisson_ratio23", "poisson_ratio13",
            "shear_modulus12", "shear_modulus23", "shear_modulus13"});
    opts.emplace_back(
        "poisson_ratio", tfel::glossary::Glossary::PoissonRatio,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus"},
        std::vector<std::string>{
            "young_modulus1", "young_modulus2", "young_modulus3",
            "poisson_ratio12", "poisson_ratio23", "poisson_ratio13",
            "shear_modulus12", "shear_modulus23", "shear_modulus13"});
    for (const auto& o :
         StressPotentialBase::getIsotropicThermalExpansionOptions()) {
      opts.push_back(o);
    }
    return opts;
  }  // end of getIsotropicBehaviourOptions

  std::vector<OptionDescription> HookeStressPotentialBase::
      getOrthotropicBehaviourElasticMaterialPropertiesOptions() {
    auto opts = std::vector<OptionDescription>{};
    opts.emplace_back(
        "young_modulus1", tfel::glossary::Glossary::YoungModulus1,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus2", "young_modulus3",
                                 "poisson_ratio12", "poisson_ratio23",
                                 "poisson_ratio13", "shear_modulus12",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "young_modulus2", tfel::glossary::Glossary::YoungModulus2,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus3",
                                 "poisson_ratio12", "poisson_ratio23",
                                 "poisson_ratio13", "shear_modulus12",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "young_modulus3", tfel::glossary::Glossary::YoungModulus3,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "poisson_ratio12", "poisson_ratio23",
                                 "poisson_ratio13", "shear_modulus12",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "poisson_ratio12", tfel::glossary::Glossary::PoissonRatio12,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "young_modulus3", "poisson_ratio23",
                                 "poisson_ratio13", "shear_modulus12",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "poisson_ratio23", tfel::glossary::Glossary::PoissonRatio23,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "young_modulus3", "poisson_ratio12",
                                 "poisson_ratio13", "shear_modulus12",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "poisson_ratio13", tfel::glossary::Glossary::PoissonRatio13,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "young_modulus3", "poisson_ratio12",
                                 "poisson_ratio23", "shear_modulus12",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "shear_modulus12", tfel::glossary::Glossary::ShearModulus12,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "young_modulus3", "poisson_ratio12",
                                 "poisson_ratio23", "poisson_ratio13",
                                 "shear_modulus23", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "shear_modulus23", tfel::glossary::Glossary::ShearModulus23,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "young_modulus3", "poisson_ratio12",
                                 "poisson_ratio23", "poisson_ratio13",
                                 "shear_modulus12", "shear_modulus13"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    opts.emplace_back(
        "shear_modulus13", tfel::glossary::Glossary::ShearModulus13,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"young_modulus1", "young_modulus2",
                                 "young_modulus3", "poisson_ratio12",
                                 "poisson_ratio23", "poisson_ratio13",
                                 "shear_modulus12", "shear_modulus23"},
        std::vector<std::string>{"young_modulus", "poisson_ratio"});
    return opts;
  }  // end of getOrthotropicBehaviourElasticMaterialPropertiesOptions

  std::vector<OptionDescription>
  HookeStressPotentialBase::getOrthotropicBehaviourOptions() {
    auto opts = HookeStressPotentialBase::
        getOrthotropicBehaviourElasticMaterialPropertiesOptions();
    for (const auto& o :
         StressPotentialBase::getOrthotropicThermalExpansionOptions()) {
      opts.push_back(o);
    }
    return opts;
  }  // end of getOrthotropicBehaviourOptions()

  std::vector<OptionDescription> HookeStressPotentialBase::getGeneralOptions() {
    auto opts = StressPotentialBase::getGeneralOptions();
    opts.emplace_back("plane_stress_support", "", OptionDescription::BOOLEAN);
    opts.emplace_back("generic_tangent_operator", "",
                      OptionDescription::BOOLEAN);
    opts.emplace_back("generic_prediction_operator", "",
                      OptionDescription::BOOLEAN);
    return opts;
  }  // end of getGeneralOptions()

  std::vector<OptionDescription> HookeStressPotentialBase::getOptions(
      const BehaviourDescription& bd, const bool b) const {
    auto opts =
        StressPotentialBase::getThermalExpansionReferenceTemperatureOption();
    if (b || ((!b) && (bd.getSymmetryType() == mfront::ISOTROPIC))) {
      const auto oopts =
          HookeStressPotentialBase::getIsotropicBehaviourOptions();
      opts.insert(opts.end(), oopts.begin(), oopts.end());
    }
    if (b || ((!b) && (bd.getSymmetryType() == mfront::ORTHOTROPIC))) {
      const auto oopts =
          HookeStressPotentialBase::getOrthotropicBehaviourOptions();
      opts.insert(opts.end(), oopts.begin(), oopts.end());
    }
    if (b) {
      const auto gopts = HookeStressPotentialBase::getGeneralOptions();
      opts.insert(opts.end(), gopts.begin(), gopts.end());
    }
    return opts;
  }  // end of getOptions

  void HookeStressPotentialBase::initialize(BehaviourDescription& bd,
                                            AbstractBehaviourDSL& dsl,
                                            const DataMap& d) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "HookeStressPotentialBase::HookeStressPotentialBase: " + m);
    };
    auto check_not = [&d](const char* const n) {
      if (d.count(n) != 0) {
        tfel::raise(
            "HookeStressPotentialBase::HookeStressPotentialBase: entry '" +
            std::string(n) + "' shall not be defined");
      }
    };
    // options
    auto update = [throw_if, &d](bool& b, const char* n) {
      if (d.count(n) != 0) {
        const auto& v = d.at(n);
        throw_if(!v.is<bool>(),
                 "invalid type for option '" + std::string(n) + "'");
        b = v.get<bool>();
      }
    };
    // checking options
    mfront::bbrick::check(d, this->getOptions(bd, true));
    //
    StressPotentialBase::initialize(bd, dsl, d);
    // reserve some specific variables
    bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "sebdata");
    update(this->pss, "plane_stress_support");
    update(this->gto, "generic_tangent_operator");
    update(this->gpo, "generic_tangent_operator");
    if (this->pss) {
      bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "etozz");
      bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "detozz");
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "\u0394etozz");
      bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "sigzz");
      bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "dsigzz");
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "\u0394sigzz");
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     "prediction_stress");
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     "prediction_strain");
    }
    // basic checks
    throw_if(bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "this behaviour brick is only usable for strain based behaviours");
    throw_if(bd.getIntegrationScheme() != BehaviourDescription::IMPLICITSCHEME,
             "this behaviour brick is only usable in implicit schemes");
    if ((d.count("young_modulus") != 0) || (d.count("poisson_ratio") != 0)) {
      check_not("young_modulus1");
      check_not("young_modulus2");
      check_not("young_modulus3");
      check_not("poisson_ratio12");
      check_not("poisson_ratio23");
      check_not("poisson_ratio13");
      check_not("young_modulus12");
      check_not("young_modulus23");
      check_not("young_modulus13");
      std::vector<BehaviourDescription::MaterialProperty> emps = {
          StressPotentialBase::getMaterialProperty(dsl, d, "young_modulus"),
          StressPotentialBase::getMaterialProperty(dsl, d, "poisson_ratio")};
      bd.setElasticMaterialProperties(emps);
    } else if ((d.count("young_modulus1") != 0) ||
               (d.count("young_modulus2") != 0) ||
               (d.count("young_modulus3") != 0) ||
               (d.count("poisson_ratio12") != 0) ||
               (d.count("poisson_ratio23") != 0) ||
               (d.count("poisson_ratio13") != 0) ||
               (d.count("shear_modulus12") != 0) ||
               (d.count("shear_modulus23") != 0) ||
               (d.count("shear_modulus13") != 0)) {
      check_not("young_modulus");
      check_not("poisson_ratio");
      std::vector<BehaviourDescription::MaterialProperty> emps = {
          StressPotentialBase::getMaterialProperty(dsl, d, "young_modulus1"),
          StressPotentialBase::getMaterialProperty(dsl, d, "young_modulus2"),
          StressPotentialBase::getMaterialProperty(dsl, d, "young_modulus3"),
          StressPotentialBase::getMaterialProperty(dsl, d, "poisson_ratio12"),
          StressPotentialBase::getMaterialProperty(dsl, d, "poisson_ratio23"),
          StressPotentialBase::getMaterialProperty(dsl, d, "poisson_ratio13"),
          StressPotentialBase::getMaterialProperty(dsl, d, "shear_modulus12"),
          StressPotentialBase::getMaterialProperty(dsl, d, "shear_modulus23"),
          StressPotentialBase::getMaterialProperty(dsl, d, "shear_modulus13")};
      bd.setElasticMaterialProperties(emps);
      bd.setAttribute(BehaviourDescription::computesStiffnessTensor, true,
                      false);
      bd.setAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,
                      true, false);
    }
  }  // end of initialize

  std::string HookeStressPotentialBase::getName() const {
    return "Hooke";
  }  // end of HookeStressPotentialBase

  void HookeStressPotentialBase::completeVariableDeclaration(
      BehaviourDescription& bd, const AbstractBehaviourDSL&) const {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "HookeStressPotentialBase::completeVariableDeclaration: " + m);
    };
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "HookeStressPotentialBase::completeVariableDeclaration: begin\n";
    }
    LocalDataStructure d;
    d.name = "sebdata";
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    // elastic strain
    StressPotentialBase::declareElasticStrainIfRequired(bd);
    // treating material properties and stress computation
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                         false))) {
      this->declareComputeStressWhenStiffnessTensorIsDefined(bd);
    } else {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        if (!bd.areElasticMaterialPropertiesDefined()) {
          bd.setAttribute("HookeStressPotentialBase::UseLocalLameCoeficients",
                          true, false);
          auto yg = findIfParameterOrMaterialPropertyIsUniformelyDefined(
              bd, Glossary::YoungModulus, "stress");
          auto nu = findIfParameterOrMaterialPropertyIsUniformelyDefined(
              bd, Glossary::PoissonRatio, "real");
          if (yg.empty()) {
            addMaterialPropertyIfNotDefined(bd, "stress", "young",
                                            Glossary::YoungModulus);
            yg = "young";
          }
          if (nu.empty()) {
            addMaterialPropertyIfNotDefined(bd, "real", "nu",
                                            Glossary::PoissonRatio);
            nu = "nu";
          }
          bd.setAttribute("HookeStressPotentialBase::YoungModulus", yg, false);
          bd.setAttribute("HookeStressPotentialBase::PoissonRatio", nu, false);
          d.addVariable(uh, {"stress", "lambda"});
          d.addVariable(uh, {"stress", "mu"});
          // local variable initialisation
          CodeBlock init;
          const auto args = "this->" + yg + ",this->" + nu;
          init.code =
              "// initialisation Lame's coefficients\n"
              "this->sebdata.lambda = "
              "tfel::material::computeLambda(" +
              args +
              ");\n"  //
              "this->sebdata.mu = " +
              "tfel::material::computeMu(" + args + ");\n";
          bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                     BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING,
                     true);
        }
        this->declareComputeStressForIsotropicBehaviour(bd, d);
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        this->declareComputeStressForOrthotropicBehaviour(bd);
      } else {
        throw_if(true, "unsupported elastic symmetry type");
      }
    }
    if (this->pss) {
      const auto agps =
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
      const auto ps = ModellingHypothesis::PLANESTRESS;
      if (bmh.count(agps) != 0) {
        VariableDescription etozz("strain", "etozz", 1u, 0u);
        etozz.description = "axial strain";
        etozz.setGlossaryName(tfel::glossary::Glossary::AxialStrain);
        bd.addStateVariable(agps, etozz, BehaviourData::ALREADYREGISTRED);
        VariableDescription sigzz("strain", "sigzz", 1u, 0u);
        sigzz.description = "axial stress";
        sigzz.setGlossaryName(tfel::glossary::Glossary::AxialStress);
        bd.addExternalStateVariable(agps, sigzz,
                                    BehaviourData::ALREADYREGISTRED);
        d.addVariable(agps, {"stress", "szz"});
        if ((bd.isStrainMeasureDefined()) &&
            (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
          d.addVariable(agps, {"stress", "exp_etozz"});
        }
      }
      if (bmh.count(ps) != 0) {
        VariableDescription etozz("strain", "etozz", 1u, 0u);
        etozz.description = "axial strain";
        bd.addStateVariable(ps, etozz, BehaviourData::ALREADYREGISTRED);
        bd.setGlossaryName(ps, "etozz", tfel::glossary::Glossary::AxialStrain);
        d.addVariable(ps, {"stress", "szz"});
      }
    }
    if (this->gpo) {
      const bool agps =
          bmh.count(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
      const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
      if (agps || ps) {
        if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)) {
          if (!bd.hasAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
            bd.setAttribute(
                BehaviourDescription::requiresUnAlteredStiffnessTensor, true,
                false);
          }
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::requiresUnAlteredStiffnessTensor),
                   "genertic tangent operator support for "
                   "plane stress hypotheses requires the use of an unaltered "
                   "stiffness tensor");
        }
      }
    }
    bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
    // look if the broken variable has defined
    const auto has_broken = [&bd] {
      const auto& bdata = bd.getBehaviourData(uh);
      const auto& asvs = bdata.getAuxiliaryStateVariables();
      const auto& esvs = bdata.getExternalStateVariables();
      const auto pav =
          findByExternalName(asvs, tfel::glossary::Glossary::Broken);
      const auto pev =
          findByExternalName(esvs, tfel::glossary::Glossary::Broken);
      return (pav != asvs.end()) || (pev != esvs.end());
    }();
    if (has_broken) {
      VariableDescription k(
          "real", HookeStressPotentialBase::residualStiffnessFactor, 1u, 0u);
      k.description =
          "Relative value used to define the residual stiffness when broken";
      bd.addParameter(uh, k, BehaviourData::UNREGISTRED);
      bd.setParameterDefaultValue(
          uh, HookeStressPotentialBase::residualStiffnessFactor, 1.e-5);
      bd.setEntryName(uh, HookeStressPotentialBase::residualStiffnessFactor,
                      "ResidualStiffnessFactor");
    }
    //
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "HookeStressPotentialBase::completeVariableDeclaration: end\n";
    }
  }  // end of completeVariableDeclaration

  void HookeStressPotentialBase::endTreatment(
      BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
    // modelling hypotheses supported by the behaviour
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto bmh = bd.getModellingHypotheses();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "HookeStressPotentialBase::endTreatment: begin\n";
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
    // modelling hypotheses supported by the behaviour
    // plane stress support
    if (this->pss) {
      const bool agps =
          bmh.count(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
      const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
      if (ps) {
        this->addPlaneStressSupport(bd, dsl);
      }
      if (agps) {
        this->addAxisymmetricalGeneralisedPlaneStressSupport(bd, dsl);
      }
    }
    // declaring the computeElasticPrediction member
    this->declareComputeElasticPredictionMethod(bd);
    // prediction operator
    // look if the broken variable has defined
    if (this->gpo) {
      this->addBrokenPredictionOperatorSupport(bd);
      this->addGenericPredictionOperatorSupport(bd);
    }
    // tangent operator
    if (this->gto) {
      this->addBrokenTangentOperatorSupport(bd);
      this->addGenericTangentOperatorSupport(bd, dsl);
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "HookeStressPotentialBase::endTreatment: end\n";
    }
  }  // end of endTreatment

  void HookeStressPotentialBase::addBrokenOperatorSupport(
      BehaviourDescription& bd, const std::string& c) const {
    // look if the broken variable has defined
    const auto broken_test = getBrokenTest(bd, true);
    if (broken_test.empty()) {
      return;
    }
    const auto residual_factor =
        "(this->" +
        std::string{HookeStressPotentialBase::residualStiffnessFactor} + ")";
    CodeBlock to;
    to.code += "if(" + broken_test + "){\n";
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                         false))) {
      const std::string D =
          bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)
              ? "this->D"
              : "this->D_tdt";
      to.code += "this->Dt = " + residual_factor + " * (" + D + ");\n";
    } else {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        auto b = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda =
            b ? "this->sebdata.lambda" : "this->lambda_tdt";
        const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
        to.code +=
            "computeAlteredElasticStiffness<hypothesis, NumericType>::exe(Dt," +
            residual_factor + " * ( " + lambda + "), " + residual_factor +
            " * (" + mu + "));\n";
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        tfel::raise_if(
            !bd.getAttribute<bool>(
                BehaviourDescription::computesStiffnessTensor, false),
            "HookeStressPotentialBase::addBrokenOperatorSupport: "
            "orthotropic behaviour shall require the stiffness tensor");
        to.code += "this->Dt = " + residual_factor + " * (this->D_tdt);\n";
      } else {
        tfel::raise(
            "HookeStressPotentialBase::addBrokenOperatorSupport: "
            "unsupported elastic symmetry type");
      }
    }
    if (c == BehaviourData::ComputePredictionOperator) {
      to.code += "return SUCCESS;\n";
    } else {
      to.code += "return true;\n";
    }
    to.code += "} // end of if(" + broken_test + ")\n";
    bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS, c, to,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }  // end of addBrokenOperatorSupport

  void HookeStressPotentialBase::addBrokenPredictionOperatorSupport(
      BehaviourDescription& bd) const {
    this->addBrokenOperatorSupport(bd,
                                   BehaviourData::ComputePredictionOperator);
  }  // end of addBrokenPredictionOperatorSupport

  void HookeStressPotentialBase::addBrokenTangentOperatorSupport(
      BehaviourDescription& bd) const {
    this->addBrokenOperatorSupport(bd, BehaviourData::ComputeTangentOperator);
  }  // end of addBrokenPredictionOperatorSupport

  void HookeStressPotentialBase::declareComputeStressForOrthotropicBehaviour(
      BehaviourDescription&) const {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "HookeStressPotentialBase::"
                        "declareComputeStressForOrthotropicBehaviour: "
                        "begin\n";
    }
    tfel::raise(
        "HookeStressPotentialBase::"
        "declareComputeStressForOrthotropicBehaviour: "
        "the stiffness tensor must be defined for "
        "orthotropic behaviours");
    // if (getVerboseMode() >= VERBOSE_DEBUG) {
    //   getLogStream() << "HookeStressPotentialBase::"
    //                     "declareComputeStressForOrthotropicBehaviour:
    //                     end\n";
    // }
  }  // end of declareComputeStressForOrthotropicBehaviour

  void HookeStressPotentialBase::addAxisymmetricalGeneralisedPlaneStressSupport(
      BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
    const auto agps = ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
    auto c = std::string{};
    // The brick contains a reference to an abstract behaviour dsl.
    // We need to know if we have to define the jacobian terms. So we
    // downcast it to the ImplicitDSLBase class have access to the
    // solver. See the getSolver call below.
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    if ((bd.isStrainMeasureDefined()) &&
        (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
      c += "this->sebdata.exp_etozz = std::exp(this->etozz+this->detozz);\n";
    }
    if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      const std::string D =
          bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                false)
              ? "this->D_tdt"
              : "this->D";
      c += "// the generalised plane stress equation is satisfied\n";
      c += "// at the end of the time step\n";
      c += "this->sebdata.szz = ";
      c += "(" + D + "(1,1))*(this->eel(1)+this->deel(1))+";
      c += "(" + D + "(1,0))*(this->eel(0)+this->deel(0))+";
      c += "(" + D + "(1,2))*(this->eel(2)+this->deel(2));\n";
      if ((bd.isStrainMeasureDefined()) &&
          (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
        c += "fetozz = ";
        c += "(this->sebdata.szz-";
        c += "this->sebdata.exp_etozz * (this->sigzz+this->dsigzz))/";
        c += "(" + D + "(1,1));\n";
      } else {
        c += "fetozz = ";
        c += "(this->sebdata.szz-this->sigzz-this->dsigzz)/";
        c += "(" + D + "(1,1));\n";
      }
      c += "// modification of the partition of strain\n";
      c += "feel(1) -= this->detozz;\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        c += "// jacobian\n";
        c += "dfeel_ddetozz(1) = -1;\n";
        c += "dfetozz_ddetozz  = real(0);\n";
        c += "dfetozz_ddeel(1) = 1;\n";
        c += "dfetozz_ddeel(0) = (" + D + "(1,0))/(" + D + "(1,1));\n";
        c += "dfetozz_ddeel(2) = (" + D + "(1,2))/(" + D + "(1,1));\n";
        if ((bd.isStrainMeasureDefined()) &&
            (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
          c += "dfetozz_ddetozz = ";
          c += "-this->sebdata.exp_etozz * (this->sigzz+this->dsigzz)/";
          c += "(" + D + "(1,1));\n";
        }
      }
    } else {
      if (bd.areElasticMaterialPropertiesDefined()) {
        c += "// the generalised plane stress equation \n";
        c += "// is satisfied at the end of the time step\n";
        c += "this->sebdata.szz = \n";
        c += "(this->lambda_tdt+2*(this->mu_tdt))*";
        c += "(this->eel(1)+this->deel(1))";
        c += "+ (this->lambda_tdt)*";
        c += "(this->eel(0)+this->deel(0)+";
        c += " this->eel(2)+this->deel(2));\n";
        if ((bd.isStrainMeasureDefined()) &&
            (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
          c += "fetozz = ";
          c += "(this->sebdata.szz-";
          c += "this->sebdata.exp_etozz * (this->sigzz+this->dsigzz))/";
          c += "this->young_tdt;\n";
        } else {
          c += "fetozz   = ";
          c += "(this->sebdata.szz-this->sigzz-this->dsigzz)/"
               "this->young_tdt;\n";
        }
        c += "// modification of the partition of strain\n";
        c += "feel(1) -= this->detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          c += "// jacobian\n";
          c += "dfeel_ddetozz(1) = -1;\n";
          c += "dfetozz_ddetozz  = real(0);\n";
          c += "dfetozz_ddeel(1) = ";
          c += "(this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n";
          c += "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n";
          c += "dfetozz_ddeel(2) = this->lambda_tdt/this->young_tdt;\n";
          if ((bd.isStrainMeasureDefined()) &&
              (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
            c += "dfetozz_ddetozz = ";
            c += "-this->sebdata.exp_etozz * (this->sigzz+this->dsigzz)/";
            c += "this->young_tdt;\n";
          }
        }
      } else {
        auto b = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda = b ? "this->sebdata.lambda" : "this->lambda";
        const std::string mu = b ? "this->sebdata.mu" : "this->mu";
        const auto young = [&bd]() -> std::string {
          if (bd.areElasticMaterialPropertiesDefined()) {
            return "this->young_tdt";
          }
          if (bd.hasAttribute("HookeStressPotentialBase::YoungModulus")) {
            return "this->" + bd.getAttribute<std::string>(
                                  "HookeStressPotentialBase::YoungModulus");
          }
          return "this->young";
        }();
        c += "// the generalised plane stress equation \n";
        c += "// is satisfied at the end of the time step\n";
        c += "this->sebdata.szz = ";
        c += "(" + lambda + "+2*(" + mu + "))*(this->eel(1)+this->deel(1))";
        c += " + ";
        c += "(" + lambda + ")*(this->eel(0)+this->deel(0)+";
        c += "this->eel(2)+this->deel(2));\n";
        if ((bd.isStrainMeasureDefined()) &&
            (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
          c += "fetozz = ";
          c += "(this->sebdata.szz-";
          c += "this->sebdata.exp_etozz * (this->sigzz+this->dsigzz)) / ";
          c += young + ";\n";
        } else {
          c += "fetozz = ";
          c += "(this->sebdata.szz-this->sigzz-this->dsigzz) / ";
          c += young + ";\n";
        }
        c += "// modification of the partition of strain\n";
        c += "feel(1) -= this->detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          c += "// jacobian\n";
          c += "dfeel_ddetozz(1) = -1;\n";
          c += "dfetozz_ddetozz  = real(0);\n";
          c += "dfetozz_ddeel(1) = ";
          c += "(" + lambda + "+2*(" + mu + "))/" + young + ";\n";
          c += "dfetozz_ddeel(0) = " + lambda + "/" + young + ";\n";
          c += "dfetozz_ddeel(2) = " + lambda + "/" + young + ";\n";
        }
        if ((bd.isStrainMeasureDefined()) &&
            (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
          c += "dfetozz_ddetozz = ";
          c += "-this->sebdata.exp_etozz * (this->sigzz+this->dsigzz)/";
          c += young + ";\n";
        }
      }
    }
    CodeBlock integrator;
    integrator.code = c;
    bd.setCode(agps, BehaviourData::Integrator, integrator,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }

  void HookeStressPotentialBase::addPlaneStressSupport(
      BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
    const auto ps = ModellingHypothesis::PLANESTRESS;
    CodeBlock integrator;
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      const std::string D =
          bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                false)
              ? "D_tdt"
              : "D";
      integrator.code +=
          "// the plane stress equation is satisfied at the end of the "
          "time "
          "step\n"
          "fetozz   = this->eel(2)+this->deel(2)+"
          "           ((this->" +
          D + "(2,0))/(this->" + D +
          "(2,2)))*(this->eel(0)+this->deel(0))+"
          "           ((this->" +
          D + "(2,1))/(this->" + D +
          "(2,2)))*(this->eel(1)+this->deel(1));\n"
          "// modification of the partition of strain\n"
          "feel(2)          -= this->detozz;\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        integrator.code +=
            "// jacobian\n"
            "dfeel_ddetozz(2)  = -1;\n"
            "dfetozz_ddetozz   = real(0);\n"
            "dfetozz_ddeel(2)  = 1;\n"
            "dfetozz_ddeel(0)  = (this->" +
            D + "(1,0))/(this->" + D +
            "(1,1));\n"
            "dfetozz_ddeel(1)  = (this->" +
            D + "(2,0))/(this->" + D + "(1,1));\n";
      }
    } else {
      if (bd.areElasticMaterialPropertiesDefined()) {
        integrator.code +=
            "// the plane stress equation is satisfied at the end of the "
            "time "
            "step\n"
            "this->sebdata.szz = "
            "(this->lambda_tdt+2*(this->mu_tdt))*(this->eel(2)+this->"
            "deel(2))"
            "+"
            "                   "
            "(this->lambda_tdt)*(this->eel(0)+this->deel(0)+this->eel(1)+"
            "this->"
            "deel(1));\n"
            "fetozz   = this->sebdata.szz/(this->young_tdt);\n"
            "// modification of the partition of strain\n"
            "feel(2) -= detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(2) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(2) = "
              "(this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
              "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
              "dfetozz_ddeel(1) = this->lambda_tdt/this->young_tdt;\n";
        }
      } else {
        auto b = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda = b ? "this->sebdata.lambda" : "this->lambda";
        const std::string mu = b ? "this->sebdata.mu" : "this->mu";
        const auto young = [&bd]() -> std::string {
          if (bd.areElasticMaterialPropertiesDefined()) {
            return "this->young_tdt";
          }
          if (bd.hasAttribute("HookeStressPotentialBase::YoungModulus")) {
            return "this->" + bd.getAttribute<std::string>(
                                  "HookeStressPotentialBase::YoungModulus");
          }
          return "this->young";
        }();
        integrator.code +=
            "// the plane stress equation is satisfied at the end of the "
            "time "
            "step\n"
            "this->sebdata.szz = (" +
            lambda + "+2*(" + mu +
            "))*(this->eel(2)+this->deel(2))+"
            "                   (" +
            lambda +
            ")*(this->eel(0)+this->deel(0)+this->eel(1)+this->deel(1));\n"
            "fetozz   = this->sebdata.szz/(" +
            young +
            ");\n"
            "// modification of the partition of strain\n"
            "feel(2) -= detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(2) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(2) = (" +
              lambda + "+2*(" + mu + "))/" + young +
              ";\n"
              "dfetozz_ddeel(0) = " +
              lambda + "/" + young +
              ";\n"
              "dfetozz_ddeel(1) = " +
              lambda + "/" + young + ";\n";
        }
      }
    }
    bd.setCode(ps, BehaviourData::Integrator, integrator,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }

  std::vector<HookeStressPotentialBase::Hypothesis>
  HookeStressPotentialBase::getSupportedModellingHypotheses(
      const BehaviourDescription&, const AbstractBehaviourDSL& dsl) const {
    auto dmh = dsl.getDefaultModellingHypotheses();
    std::vector<Hypothesis> mh(dmh.begin(), dmh.end());
    if (this->pss) {
      if (dsl.isModellingHypothesisSupported(
              ModellingHypothesis::PLANESTRESS)) {
        mh.push_back(ModellingHypothesis::PLANESTRESS);
      }
      if (dsl.isModellingHypothesisSupported(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        mh.push_back(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
      }
    }
    return mh;
  }  // end of getSupportedModellingHypothesis

  void HookeStressPotentialBase::computeElasticPrediction(
      BehaviourDescription& bd) const {
    CodeBlock i;
    i.code = "const auto sigel = this->computeElasticPrediction();\n";
    bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               BehaviourData::BeforeInitializeLocalVariables, i,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }  // end of computeElasticPrediction

  std::string HookeStressPotentialBase::getStressNormalisationFactor(
      const BehaviourDescription& bd) const {
    if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      return "this->D(0,0)";
    }
    if (bd.getAttribute("HookeStressPotentialBase::UseLocalLameCoeficients",
                        false)) {
      const auto yg = bd.getAttribute<std::string>(
          "HookeStressPotentialBase::YoungModulus");
      return "this->" + yg;
    }
    return "this->young";
  }  // end of getStressNormalisationFactor

  std::string HookeStressPotentialBase::getEquivalentStressLowerBound(
      const BehaviourDescription& bd) const {
    if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      return "(this->relative_value_for_the_equivalent_stress_lower_"
             "bound)*"
             "(this->D(0,0))";
    }
    return "(this->relative_value_for_the_equivalent_stress_lower_bound)"
           " * " +
           this->getStressNormalisationFactor(bd);
  }  // end of getEquivalentStressLowerBound

  HookeStressPotentialBase::~HookeStressPotentialBase() = default;

}  // end of namespace mfront::bbrick
