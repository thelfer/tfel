/*!
 * \file   mfront/src/ScalarDamageHookeStressPotentialBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/ScalarDamageHookeStressPotentialBase.hxx"

namespace mfront::bbrick {

  ScalarDamageHookeStressPotentialBase::ScalarDamageHookeStressPotentialBase() =
      default;

  std::vector<OptionDescription>
  ScalarDamageHookeStressPotentialBase::getOptions(
      const BehaviourDescription& bd, const bool b) const {
    auto opts = HookeStressPotentialBase::getOptions(bd, b);
    opts.emplace_back(
        "damage_thresold",
        "Maximum value of the damage used in the computation of the secant "
        "and consistent tangent operator.",
        OptionDescription::REAL);
    return opts;
  }  // end of getOptions

  void ScalarDamageHookeStressPotentialBase::initialize(
      BehaviourDescription& bd, AbstractBehaviourDSL& dsl, const DataMap& d) {
    using tfel::glossary::Glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    HookeStressPotentialBase::initialize(bd, dsl, d);
    // damage
    VariableDescription vd("real", "d", 1u, 0u);
    vd.description = "damage variable";
    bd.addStateVariable(uh, vd);
    bd.setGlossaryName(uh, "d", Glossary::Damage);
    // damage thresold
    const auto dc_n = "damage_thresold";
    const auto dc_v = [&d, dc_n]() -> double {
      constexpr double dmin = 0.999999;
      if (d.count(dc_n) != 0) {
        const auto dc_d = d.at(dc_n);
        if (dc_d.is<int>()) {
          return static_cast<double>(dc_d.get<int>());
        }
        return dc_d.get<double>();
      }
      return dmin;
    }();
    tfel::raise_if((dc_v > 1) || (dc_v < 0),
                   "ScalarDamageHookeStressPotentialBase::initialize: "
                   "invalid default value "
                   "for the 'damage_thresold' parameter");
    VariableDescription dc("real", dc_n, 1u, 0u);
    dc.description =
        "Maximum value of the damage variable for "
        "the computation of the stiffness tensor.";
    bd.addParameter(uh, dc, BehaviourData::UNREGISTRED);
    bd.setParameterDefaultValue(uh, dc_n, dc_v);
    bd.setEntryName(uh, dc_n, "DamageThresold");
  }  // end of initialize

  void
  ScalarDamageHookeStressPotentialBase::declareComputeElasticPredictionMethod(
      BehaviourDescription& bd) const {
    // damage effect
    const std::string gde = "(1 - this->d)";
    for (const auto h : bd.getDistinctModellingHypotheses()) {
      std::string m =
          "//! \\brief return an elastic prediction of the stresses\n"
          "StressStensor computeElasticPrediction() const{\n";
      const auto broken_test = getBrokenTest(bd, false);
      if (!broken_test.empty()) {
        m += "if(" + broken_test + "){\n";
        m += "  return StressStensor(stress(0));\n";
        m += "}\n";
      }
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
        if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                             false)) ||
            (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                             false))) {
          if ((bd.getElasticSymmetryType() != mfront::ISOTROPIC) &&
              (bd.getElasticSymmetryType() != mfront::ORTHOTROPIC)) {
            m += "throw(std::runtime_error(\"computeElasticPrediction: "
                 "unsupported case\"));\n";
          } else {
            if (bd.hasAttribute(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
              if ((bd.isStrainMeasureDefined()) &&
                  (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
                m += "StrainStensor prediction_strain = "
                     "this->eel+(this->theta)*this->deto;\n";
                m += "prediction_strain(1) += ";
                m += "(this->theta)*";
                m += "computeAxialStrainIncrementElasticPrediction(";
                m += gde + " * (this->D(1,0)),";
                m += gde + " * (this->D(1,1)),";
                m += gde + " * (this->D(1,2)),";
                m += "this->eel,this->etozz,this->deto,";
                m += "this->sigzz+this->dsigzz,";
                m += "real(1),this->epsilon);\n";
                m += "const StressStensor prediction_stress = ";
                m += gde + " * (this->D) * prediction_strain;\n";
                m += "return prediction_stress;\n";
              } else {
                m += "StressStensor prediction_stress;\n";
                m += "StrainStensor prediction_strain = "
                     "this->eel+(this->theta)*this->deto;\n";
                m += "prediction_stress(1) = "
                     "this->sigzz+(this->theta)*(this->dsigzz);\n";
                m += "prediction_stress(0) = " + gde + "*(" +
                     "(this->D(0,0)-((this->D(0,1))*(this->D(1,0)))/"
                     "(this->D(1,1)))*prediction_strain(0)+\n";
                m += "                       "
                     "(this->D(0,2)-((this->D(0,1))*(this->D(1,2)))/"
                     "(this->D(1,1)))*prediction_strain(2)+\n";
                m += "                       "
                     "(this->D(0,1))/"
                     "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                     "dsigzz)));\n";
                m += "prediction_stress(2) = " + gde + "*(" +
                     "(this->D(2,0)-((this->D(2,1))*(this->D(1,0)))/"
                     "(this->D(1,1)))*prediction_strain(0)+\n";
                m += "                       "
                     "(this->D(2,2)-((this->D(2,1))*(this->D(1,2)))/"
                     "(this->D(1,1)))*prediction_strain(2)+\n";
                m += "                       "
                     "(this->D(2,1))/"
                     "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                     "dsigzz)));\n";
                m += "return prediction_stress;\n";
              }
            } else {
              m += "throw(std::runtime_error(\"computeElasticPrediction: "
                   "unsupported case\"));\n";
            }
          }
        } else {
          if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
            auto b = bd.getAttribute(
                "HookeStressPotentialBase::UseLocalLameCoeficients", false);
            const std::string lambda =
                b ? "this->sebdata.lambda" : "this->lambda";
            const std::string mu = b ? "this->sebdata.mu" : "this->mu";
            if ((bd.isStrainMeasureDefined()) &&
                (bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
              m += "StrainStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
              m += "prediction_strain(1) += ";
              m += "(this->theta)*";
              m += "computeAxialStrainIncrementElasticPrediction(";
              m += gde + " * " + lambda + ",";
              m += gde + " * (2 * (" + mu + ") + " + lambda + "),";
              m += gde + " * " + lambda + ",";
              m += "this->eel,this->etozz,this->deto,";
              m += "this->sigzz + this->dsigzz,";
              m += "real(1),this->epsilon);\n";
              m += "const StressStensor prediction_stress = ";
              m += "(" + gde + " * " + lambda +
                   ") * trace(prediction_strain) * Stensor::Id() + ";
              m += "2 * (" + gde + " * (" + mu + ")) * prediction_strain;\n";
              m += "return prediction_stress;\n";
            } else {
              m += "StressStensor prediction_stress;\n";
              m += "StrainStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
              m += "prediction_stress(0) = " + gde + " * (2 * (" + mu +
                   ") * ((" + lambda + ")/(" + lambda + "+2*(" + mu +
                   "))*(prediction_strain(0)+prediction_strain(2))+"
                   "prediction_"
                   "strain(0))+\n";
              m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                   "))*(this->sigzz+(this->theta)*(this->dsigzz)));\n";
              m += "prediction_stress(2) = " + gde + "*(2*(" + mu + ")*((" +
                   lambda + ")/(" + lambda + "+2*(" + mu +
                   "))*(prediction_strain(0)+prediction_strain(2))+"
                   "prediction_"
                   "strain(2))+\n";
              m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                   "))*(this->sigzz+(this->theta)*(this->dsigzz)));\n";
              m += "prediction_stress(1) = "
                   "this->sigzz+(this->theta)*(this->dsigzz);\n";
              m += "return prediction_stress;\n";
            }
          } else {
            tfel::raise_if(!bd.getAttribute<bool>(
                               BehaviourDescription::requiresStiffnessTensor),
                           "ScalarDamageHookeStressPotentialBase::"
                           "declareComputeElasticPredictionMethod: "
                           "the stiffness tensor must be defined for "
                           "orthotropic behaviours");
          }
        }
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                             false)) ||
            (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                             false))) {
          if ((bd.getElasticSymmetryType() != mfront::ISOTROPIC) &&
              (bd.getElasticSymmetryType() != mfront::ORTHOTROPIC)) {
            m += "throw(std::runtime_error(\"computeElasticPrediction: "
                 "unsupported material symmetry (neither isotropic "
                 "or orthotropic)\"));\n";
          } else {
            if (bd.hasAttribute(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
              m += "StressStensor prediction_stress;\n";
              m += "StrainStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
              m += "prediction_stress(0) = " + gde +
                   "*("
                   "(this->D(0,0)-((this->D(0,2))*(this->D(2,0)))/"
                   "(this->D(2,2)))*prediction_strain(0)+\n";
              m += "                       "
                   "(this->D(0,1)-((this->D(0,2))*(this->D(2,1)))/"
                   "(this->D(2,2)))*prediction_strain(1));\n";
              m += "prediction_stress(1) = " + gde +
                   "*("
                   "(this->D(1,0)-((this->D(1,2))*(this->D(2,0)))/"
                   "(this->D(2,2)))*prediction_strain(0)+\n";
              m += "                       "
                   "(this->D(1,1)-((this->D(1,2))*(this->D(2,1)))/"
                   "(this->D(2,2)))*prediction_strain(1));\n";
              m += "prediction_stress(3) = " + gde +
                   "*(this->D(3,3))*prediction_strain(3);\n";
              m += "prediction_stress(2) = stress(0);\n";
              m += "return prediction_stress;\n";
            } else {
              m += "return (this->D)*(this->eel+(this->theta)*this->deto);";
            }
          }
        } else {
          if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
            auto b = bd.getAttribute(
                "HookeStressPotentialBase::UseLocalLameCoeficients", false);
            const std::string lambda =
                b ? "this->sebdata.lambda" : "this->lambda";
            const std::string mu = b ? "this->sebdata.mu" : "this->mu";
            m += "StressStensor prediction_stress;\n";
            m += "StressStensor prediction_strain = "
                 "this->eel+(this->theta)*this->deto;\n";
            m += "prediction_stress(0) = " + gde + "*2*(" + mu + ")*((" +
                 lambda + ")/(" + lambda + "+2*(" + mu +
                 "))*(prediction_strain(0)+prediction_strain(1))+"
                 "prediction_"
                 "strain(0));\n";
            m += "prediction_stress(1) = " + gde + "*2*(" + mu + ")*((" +
                 lambda + ")/(" + lambda + "+2*(" + mu +
                 "))*(prediction_strain(0)+prediction_strain(1))+"
                 "prediction_"
                 "strain(1));\n";
            m += "prediction_stress(3) = " + gde + "*2*(" + mu +
                 ")*prediction_strain(3);\n";
            m += "prediction_stress(2) = stress(0);\n";
            m += "return prediction_stress;\n";
          } else {
            tfel::raise_if(!bd.getAttribute<bool>(
                               BehaviourDescription::requiresStiffnessTensor),
                           "ScalarDamageHookeStressPotentialBase::"
                           "declareComputeElasticPredictionMethod: "
                           "the stiffness tensor must be defined for "
                           "orthotropic behaviours");
          }
        }
      } else {
        if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                             false)) ||
            (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                             false))) {
          m += "return " + gde +
               "*(this->D)*(this->eel+(this->theta)*this->deto);\n";
        } else {
          if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
            auto b = bd.getAttribute(
                "HookeStressPotentialBase::UseLocalLameCoeficients", false);
            const std::string lambda =
                b ? "this->sebdata.lambda" : "this->lambda";
            const std::string mu = b ? "this->sebdata.mu" : "this->mu";
            m += "return " + gde + "*(" + lambda +
                 "*trace(this->eel+(this->theta)*(this->deto))*Stensor::Id("
                 ")+";
            m += "2*(" + mu + ")*(this->eel+(this->theta)*(this->deto)));\n";
          } else {
            tfel::raise_if(!bd.getAttribute<bool>(
                               BehaviourDescription::requiresStiffnessTensor),
                           "ScalarDamageHookeStressPotentialBase::"
                           "declareComputeElasticPredictionMethod: "
                           "the stiffness tensor must be defined for "
                           "orthotropic behaviours");
          }
        }
      }
      m += "}\n";
      bd.appendToMembers(h, m, false);
    }
  }  // end of declareComputeElasticPredictionMethod

  void ScalarDamageHookeStressPotentialBase::
      declareComputeStressWhenStiffnessTensorIsDefined(
          BehaviourDescription& bd) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock smts, sets;
    const std::string D_tdt =
        bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                              false)
            ? "this->D_tdt"
            : "this->D";
    const std::string gd_mts = "(1 - this->d - (this->theta) * (this->dd))";
    const std::string gd_ets = "(1 - this->d)";
    const std::string eel_mts = "this->eel + (this->theta) * (this->deel)";
    smts.code = "this->sig = " + gd_mts + "*(this->D)*(" + eel_mts + ");\n";
    sets.code = "this->sig = " + gd_ets + "*(" + D_tdt + ")*(this->eel);\n";
    bd.setCode(h, BehaviourData::ComputeThermodynamicForces, smts,
               BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    bd.setCode(h, BehaviourData::ComputeFinalThermodynamicForces, sets,
               BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
  }  // end of declareComputeStressWhenStiffnessTensorIsDefined

  void ScalarDamageHookeStressPotentialBase::
      declareComputeStressForIsotropicBehaviour(BehaviourDescription& bd,
                                                LocalDataStructure&) const {
    using tfel::glossary::Glossary;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "ScalarDamageHookeStressPotentialBase::"
                        "declareComputeStressForIsotropicBehaviour: begin\n";
    }
    CodeBlock smts;
    CodeBlock sets;
    const std::string gd_mts = "(1-this->d-(this->theta)*(this->dd))";
    const std::string gd_ets = "(1-this->d)";
    const std::string eel_mts = "this->eel + (this->theta) * (this->deel)";
    if (bd.areElasticMaterialPropertiesDefined()) {
      smts.code = "this->sig = " + gd_mts + "*(this->lambda*trace(" + eel_mts +
                  ")*Stensor::Id()+2*(this->mu)*(" + eel_mts + "));\n";
      sets.code = "this->sig = " + gd_ets +
                  "*(this->lambda_tdt*trace(this->eel)*Stensor::Id()+"  //
                  "2*(this->mu_tdt)*this->eel);\n";
    } else {
      // ScalarDamageHooke law
      // damage effect
      smts.code = "this->sig = " + gd_mts +
                  " * ((this->sebdata.lambda) * trace(" + eel_mts +
                  ")*Stensor::Id() + 2*(this->sebdata.mu)*(" + eel_mts +
                  "));\n";
      sets.code =
          "this->sig = " + gd_ets +
          " * ((this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+"  //
          "2*(this->sebdata.mu)*this->eel);\n";
    }
    addBrokenStateSupportToComputeStress(smts.code, bd, false);
    addBrokenStateSupportToComputeStress(sets.code, bd, true);
    bd.setCode(uh, BehaviourData::ComputeThermodynamicForces, smts,
               BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    bd.setCode(uh, BehaviourData::ComputeFinalThermodynamicForces, sets,
               BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "ScalarDamageHookeStressPotentialBase::"
                        "declareComputeStressForIsotropicBehaviour: end\n";
    }
  }  // end of declareComputeStressForIsotropicBehaviour

  void ScalarDamageHookeStressPotentialBase::addGenericTangentOperatorSupport(
      BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "ScalarDamageHookeStressPotentialBase::"
                     "addGenericTangentOperatorSupport: " +
                         m);
    };
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
    CodeBlock to;
    if (idsl.getSolver().usesJacobian()) {
      to.attributes["requires_jacobian_decomposition"] = true;
      to.attributes["uses_get_partial_jacobian_invert"] = true;
    }
    bd.checkVariablePosition("d", "IntegrationVariable", 1u);
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    const std::string de = "min(this->d, this->damage_thresold)";
    const std::string ge = "(1 - " + de + ")";
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                         false))) {
      const std::string D =
          bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)
              ? "this->D"
              : "this->D_tdt";
      to.code += "if(smt == ELASTIC){\n";
      to.code += "  this->Dt = " + D + ";\n";
      to.code += "} else if(smt == SECANTOPERATOR){\n";
      to.code += "  this->Dt = " + ge + " * " + D + ";\n";
      if (idsl.getSolver().usesJacobian()) {
        to.code += "} else if (smt == CONSISTENTTANGENTOPERATOR){\n";
        to.code += "  Stensor4 Je;\n";
        to.code += "  Stensor  Jd;\n";
        to.code += "  getPartialJacobianInvert(Je,Jd);\n";
        to.code += "  Dt = " + ge + " * (" + D + ") * Je - ";
        to.code += "  (((" + D + ")*(this->eel)) ^ (Jd));\n";
      }
      to.code += "} else {\n";
      to.code += "  return false;\n";
      to.code += "}";
    } else {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        auto b = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda =
            b ? "this->sebdata.lambda" : "this->lambda_tdt";
        const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
        to.code += "if(smt==ELASTIC){\n";
        to.code +=
            "  computeAlteredElasticStiffness<hypothesis, stress>::exe(Dt," +
            lambda + "," + mu + ");\n";
        to.code += "} else if(smt==SECANTOPERATOR){\n";
        to.code +=
            "  computeAlteredElasticStiffness<hypothesis, stress>::exe(Dt,";
        to.code += ge + " * (" + lambda + "),";
        to.code += ge + " * (" + mu + "));\n";
        if (idsl.getSolver().usesJacobian()) {
          to.code += "} else if (smt==CONSISTENTTANGENTOPERATOR){\n";
          to.code += "StiffnessTensor mfront_ScalarDamageHooke;\n";
          to.code += "Stensor4 mfront_Je;\n";
          to.code += "Stensor  mfront_Jd;\n";
          to.code += "getPartialJacobianInvert(mfront_Je, mfront_Jd);\n";
          to.code +=
              "computeElasticStiffness<N, "
              "stress>::exe(mfront_ScalarDamageHooke," +
              lambda + "," + mu + ");\n";
          to.code +=
              " Dt = " + ge + " * mfront_ScalarDamageHooke * mfront_Je - ";
          to.code +=
              "((mfront_ScalarDamageHooke * (this->eel)) ^ (mfront_Jd));\n";
        }
        to.code += "} else {\n";
        to.code += "  return false;\n";
        to.code += "}";
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        throw_if(!bd.getAttribute<bool>(
                     BehaviourDescription::computesStiffnessTensor, false),
                 "orthotropic behaviour shall require the stiffness tensor");
        to.code += "if((smt == ELASTIC){\n";
        to.code += "  this->Dt = this->D_tdt;\n";
        to.code += "} else if(smt == SECANTOPERATOR){\n";
        to.code += "  this->Dt = " + ge + " * (this->D_tdt);\n";
        if (idsl.getSolver().usesJacobian()) {
          to.code += "} else if (smt==CONSISTENTTANGENTOPERATOR){\n";
          to.code += "  Stensor4 mfront_Je;\n";
          to.code += "  Stensor  mfront_Jd;\n";
          to.code += "  getPartialJacobianInvert(mfront_Je, mfront_Jd);\n";
          to.code += "  Dt = " + ge + " * (this->D_tdt) * mfront_Je - ";
          to.code += "  (((this->D_tdt) * eel) ^ (mfront_Jd));\n";
        }
        to.code +=
            "} else {\n"
            "  return false;\n"
            "}";
      } else {
        throw_if(true, "unsupported elastic symmetry type");
      }
    }
    bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                    BehaviourData::hasConsistentTangentOperator, true, true);
    bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               BehaviourData::ComputeTangentOperator, to,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }

  void
  ScalarDamageHookeStressPotentialBase::addGenericPredictionOperatorSupport(
      BehaviourDescription& bd) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "ScalarDamageHookeStressPotentialBase::"
                     "addGenericPredictionOperatorSupport: " +
                         m);
    };
    CodeBlock to;
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    const std::string de = "min(this->d, this->damage_thresold)";
    const std::string ge = "(1 - " + de + ")";
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                         false))) {
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
      const std::string D =
          bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)
              ? "this->D"
              : "this->D_tdt";
      to.code += "if(smt == ELASTIC){\n";
      to.code += "  this->Dt = " + D + ";\n";
      to.code += "} else if(smt == SECANTOPERATOR){\n";
      to.code += "  this->Dt = " + ge + " * " + D + ";\n";
      to.code += "} else {\n";
      to.code += "  return FAILURE;\n";
      to.code += "}\n";
    } else {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        auto b = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda =
            b ? "this->sebdata.lambda" : "this->lambda_tdt";
        const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
        to.code += "if(smt==ELASTIC){\n";
        to.code +=
            "computeAlteredElasticStiffness<hypothesis, NumericType>::exe(Dt," +
            lambda + "," + mu + ");\n";
        to.code += "} else if(smt==SECANTOPERATOR){\n";
        to.code +=
            "computeAlteredElasticStiffness<hypothesis, "
            "NumericType>::exe(Dt,";
        to.code += ge + " * (" + lambda + "),";
        to.code += ge + " * (" + mu + "));\n";
        to.code += "} else {\n";
        to.code += "  return FAILURE;\n";
        to.code += "}\n";
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        throw_if(!bd.getAttribute<bool>(
                     BehaviourDescription::computesStiffnessTensor, false),
                 "orthotropic behaviour shall require the stiffness tensor");
        to.code += "if(smt == ELASTIC){\n";
        to.code += "  this->Dt = this->D_tdt;\n";
        to.code += "} else if(smt == SECANTOPERATOR){\n";
        to.code += "  this->Dt = " + ge + " * (this->D_tdt);\n";
        to.code += "} else {\n";
        to.code += "  return FAILURE;\n";
        to.code += "}\n";
      } else {
        throw_if(true, "unsupported elastic symmetry type");
      }
    }
    bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                    BehaviourData::hasPredictionOperator, true, true);
    bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
               BehaviourData::ComputePredictionOperator, to,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }

  std::vector<
      std::tuple<std::string, std::string, mfront::SupportedTypes::TypeFlag>>
  ScalarDamageHookeStressPotentialBase::getStressDerivatives(
      const BehaviourDescription& bd) const {
    const auto d = [&bd]() -> std::string {
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        return "(this->D)";
      }
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        const auto bl = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda = bl ? "this->sebdata.lambda" : "this->lambda";
        const std::string mu = bl ? "this->sebdata.mu" : "this->mu";
        return "(2 * (" + mu + ") * Stensor4::Id()+(" + lambda +
               ") * Stensor4::IxI())";
      }
      if (bd.getElasticSymmetryType() != mfront::ORTHOTROPIC) {
        tfel::raise(
            "HookeStressPotential::getStressDerivatives: "
            "unsupported elastic symmetry type");
      }
      if (!bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                 false)) {
        tfel::raise(
            "HookeStressPotential::getStressDerivatives: "
            "orthotropic behaviour shall require the stiffness tensor");
      }
      return "(this->D)";
    }();
    auto r = std::vector<std::tuple<std::string, std::string,
                                    mfront::SupportedTypes::TypeFlag>>{};
    r.push_back(std::make_tuple(
        "((this->theta) * (1-this->d-(this->theta)*(this->dd))" + d + ")",
        std::string("eel"), SupportedTypes::STENSOR));
    r.push_back(
        std::make_tuple("(-(this->theta) * (" + d +
                            ") * (this->eel + (this->theta) * (this->deel)))",
                        std::string("d"), SupportedTypes::SCALAR));
    return r;
  }  // end of getStressDerivatives

  std::string
  ScalarDamageHookeStressPotentialBase::generateImplicitEquationDerivatives(
      const BehaviourDescription& bd,
      const std::string& t,
      const std::string& v,
      const std::string& dfv_ds,
      const bool b) const {
    auto c = std::string{};
    const auto vf = SupportedTypes::getTypeFlag(t);
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                         false))) {
      if (vf == SupportedTypes::SCALAR) {
        c = "df" + v + "_ddeel += ";
        c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
        c += "((" + dfv_ds + ") | (this->D));\n";
        c += "df" + v + "_ddd -= ";
        c += "(this->theta) * ((" + dfv_ds + ") | ((this->D) * ";
        c += "(this->eel + (this->theta) * (this->deel))));\n";
      } else if (vf == SupportedTypes::STENSOR) {
        c = "df" + v + "_ddeel += ";
        c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
        c += "(" + dfv_ds + ") * (this->D);\n";
        c += "df" + v + "_ddd -= ";
        c += "(this->theta) * (" + dfv_ds + ") * (this->D) * ";
        c += "(this->eel + (this->theta) * (this->deel));\n";
      } else {
        tfel::raise(
            "ScalarDamageHookeStressPotentialBase::"
            "generateImplicitEquationDerivatives: "
            "unsupported type for variable '" +
            t + "'");
      }
    } else {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        auto bl = bd.getAttribute(
            "HookeStressPotentialBase::UseLocalLameCoeficients", false);
        const std::string lambda = bl ? "this->sebdata.lambda" : "this->lambda";
        const std::string mu = bl ? "this->sebdata.mu" : "this->mu";
        if (b) {
          if (vf == SupportedTypes::SCALAR) {
            c = "df" + v + "_ddeel += ";
            c += "2 * (" + mu + ") * ";
            c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
            c += "(" + dfv_ds + ");\n";
            c += "df" + v + "_ddd -= ";
            c += "2 * (" + mu + ") * (this->theta) * (" + dfv_ds + ") | ";
            c += "deviator(this->eel + (this->theta) * (this->deel));\n";
          } else if (vf == SupportedTypes::STENSOR) {
            c = "df" + v + "_ddeel += ";
            c += "2 * (" + mu +
                 ") * (this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
            c += "(" + dfv_ds + ");\n";
            c += "df" + v + "_ddd -= ";
            c += "2 * (" + mu + ") *(this->theta) * (" + dfv_ds + ") * ";
            c += "deviator(this->eel + (this->theta) * (this->deel));\n";
          } else {
            tfel::raise(
                "ScalarDamageHookeStressPotentialBase::"
                "generateImplicitEquationDerivatives:"
                " unsupported type for variable '" +
                t + "'");
          }
        } else {
          if (vf == SupportedTypes::SCALAR) {
            c = "df" + v + "_ddeel += ";
            c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
            c += "((" + dfv_ds + ") | (2 * (" + mu + ") * Stensor4::Id()+(" +
                 lambda + ") * Stensor4::IxI()));\n";
            c += "df" + v + "_ddd -= ";
            c += "(this->theta) * ((" + dfv_ds + ") | ";
            c += "(2 * (" + mu +
                 ") * (this->eel + (this->theta) * (this->deel)) + ";
            c += "(" + lambda +
                 ") * trace(this->eel + (this->theta) * (this->deel)) * "
                 "Stensor::Id()));\n";
          } else if (vf == SupportedTypes::STENSOR) {
            c = "df" + v + "_ddeel += ";
            c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
            c += "(" + dfv_ds + ") * (2 * (" + mu + ") * Stensor4::Id()+(" +
                 lambda + ") * Stensor4::IxI());\n";
            c += "df" + v + "_ddd -= ";
            c += "(this->theta) * (" + dfv_ds + ") * ";
            c += "(2 * (" + mu +
                 ") * (this->eel + (this->theta) * (this->deel)) + ";
            c += "(" + lambda +
                 ") * trace(this->eel + (this->theta) * (this->deel)) * "
                 "Stensor::Id());\n";
          } else {
            tfel::raise(
                "ScalarDamageHookeStressPotentialBase::"
                "generateImplicitEquationDerivatives:"
                " "
                "unsupported type for variable '" +
                t + "'");
          }
        }
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        if (!bd.getAttribute<bool>(
                BehaviourDescription::computesStiffnessTensor, false)) {
          tfel::raise(
              "ScalarDamageHookeStressPotentialBase::"
              "generateImplicitEquationDerivatives: "
              "orthotropic behaviour shall require the stiffness tensor");
        }
        if (vf == SupportedTypes::SCALAR) {
          c = "df" + v + "_ddeel += ";
          c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
          c += "((" + dfv_ds + ") | (this->D));\n";
          c += "df" + v + "_ddd -= ";
          c += "(this->theta) * ((" + dfv_ds + ") | ((this->D) * ";
          c += "(this->eel + (this->theta) * (this->deel))));\n";
        } else if (vf == SupportedTypes::STENSOR) {
          c = "df" + v + "_ddeel += ";
          c += "(this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
          c += "(" + dfv_ds + ") * (this->D);\n";
          c += "df" + v + "_ddd -= ";
          c += "(this->theta) * (" + dfv_ds + ") * (this->D) * ";
          c += "(this->eel + (this->theta) * (this->deel));\n";
        } else {
          tfel::raise(
              "ScalarDamageHookeStressPotentialBase::"
              "generateImplicitEquationDerivatives: "
              "unsupported type for variable '" +
              t + "'");
        }
      } else {
        tfel::raise(
            "ScalarDamageHookeStressPotentialBase::"
            "generateImplicitEquationDerivatives: "
            "unsupported elastic symmetry type");
      }
    }
    return c;
  }  // end of generateImplicitEquationDerivatives

  ScalarDamageHookeStressPotentialBase::
      ~ScalarDamageHookeStressPotentialBase() = default;

}  // end of namespace mfront::bbrick
