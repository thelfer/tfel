/*!
 * \file   mfront/src/IsotropicDamageHookeStressPotentialBase.cxx
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
#include "MFront/BehaviourBrick/IsotropicDamageHookeStressPotentialBase.hxx"

namespace mfront {

  namespace bbrick {

    IsotropicDamageHookeStressPotentialBase::
        IsotropicDamageHookeStressPotentialBase() = default;

    std::vector<OptionDescription>
    IsotropicDamageHookeStressPotentialBase::getOptions(
        const BehaviourDescription& bd, const bool b) const {
      auto opts = HookeStressPotentialBase::getOptions(bd, b);
      opts.emplace_back(
          "damage_thresold",
          "Maximum value of the damage used in the computation of the secant "
          "and consistent tangent operator.",
          OptionDescription::REAL);
      return opts;
    }  // end of IsotropicDamageHookeStressPotentialBase::getOptions()

    void IsotropicDamageHookeStressPotentialBase::initialize(
        BehaviourDescription& bd, AbstractBehaviourDSL& dsl, const DataMap& d) {
      using tfel::glossary::Glossary;
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      HookeStressPotentialBase::initialize(bd, dsl, d);
      // damage
      VariableDescription vd("real", "d", 1u, 0u);
      vd.description = "damage variable";
      bd.addStateVariable(uh, vd);
      bd.setGlossaryName(uh, "d", Glossary::Damage);
      // damage thresold
      const auto dc_n = "damage_thresold";
      const auto dc_v = [&d, dc_n]() -> double {
        constexpr const double dmin = 0.999999;
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
                     "IsotropicDamageHookeStressPotentialBase::initialize: "
                     "invalid default value "
                     "for the 'damage_thresold' parameter");
      VariableDescription dc("real", dc_n, 1u, 0u);
      dc.description =
          "Maximum value of the damage variable for "
          "the computation of the stiffness tensor.";
      bd.addParameter(uh, dc, BehaviourData::UNREGISTRED);
      bd.setParameterDefaultValue(uh, dc_n, dc_v);
      bd.setEntryName(uh, dc_n, "DamageThresold");
    }  // end of IsotropicDamageHookeStressPotentialBase::initialize

    void IsotropicDamageHookeStressPotentialBase::
        declareComputeElasticPredictionMethod(BehaviourDescription& bd) const {
      // damage effect
      const std::string de = "(1-this->d)";
      for (const auto h : bd.getDistinctModellingHypotheses()) {
        std::string m =
            "//! \\brief return an elastic prediction of the stresses\n"
            "StressStensor computeElasticPrediction() const{\n";
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
                  m += de + " * (this->D(1,0)),";
                  m += de + " * (this->D(1,1)),";
                  m += de + " * (this->D(1,2)),";
                  m += "this->eel,this->etozz,this->deto,";
                  m += "this->sigzz+this->dsigzz,";
                  m += "real(1),this->epsilon);\n";
                  m += "const StressStensor prediction_stress = ";
                  m += de + " * (this->D) * prediction_strain;\n";
                  m += "return prediction_stress;\n";
                } else {
                  m += "StressStensor prediction_stress;\n";
                  m += "StrainStensor prediction_strain = "
                       "this->eel+(this->theta)*this->deto;\n";
                  m += "prediction_stress(1) = "
                       "this->sigzz+(this->theta)*(this->dsigzz);\n";
                  m += "prediction_stress(0) = " + de + "*(" +
                       "(this->D(0,0)-((this->D(0,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(0,2)-((this->D(0,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(0,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz)));\n";
                  m += "prediction_stress(2) = " + de + "*(" +
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
                m += de + " * " + lambda + ",";
                m += de + " * (2 * (" + mu + ") + " + lambda + "),";
                m += de + " * " + lambda + ",";
                m += "this->eel,this->etozz,this->deto,";
                m += "this->sigzz + this->dsigzz,";
                m += "real(1),this->epsilon);\n";
                m += "const StressStensor prediction_stress = ";
                m += "(" + de + " * " + lambda +
                     ") * trace(prediction_strain) * Stensor::Id() + ";
                m += "2 * (" + de + " * (" + mu + ")) * prediction_strain;\n";
                m += "return prediction_stress;\n";
              } else {
                m += "StressStensor prediction_stress;\n";
                m += "StrainStensor prediction_strain = "
                     "this->eel+(this->theta)*this->deto;\n";
                m += "prediction_stress(0) = " + de + " * (2 * (" + mu +
                     ") * ((" + lambda + ")/(" + lambda + "+2*(" + mu +
                     "))*(prediction_strain(0)+prediction_strain(2))+"
                     "prediction_"
                     "strain(0))+\n";
                m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                     "))*(this->sigzz+(this->theta)*(this->dsigzz)));\n";
                m += "prediction_stress(2) = " + de + "*(2*(" + mu + ")*((" +
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
                             "IsotropicDamageHookeStressPotentialBase::"
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
                m += "prediction_stress(0) = " + de +
                     "*("
                     "(this->D(0,0)-((this->D(0,2))*(this->D(2,0)))/"
                     "(this->D(2,2)))*prediction_strain(0)+\n";
                m += "                       "
                     "(this->D(0,1)-((this->D(0,2))*(this->D(2,1)))/"
                     "(this->D(2,2)))*prediction_strain(1));\n";
                m += "prediction_stress(1) = " + de +
                     "*("
                     "(this->D(1,0)-((this->D(1,2))*(this->D(2,0)))/"
                     "(this->D(2,2)))*prediction_strain(0)+\n";
                m += "                       "
                     "(this->D(1,1)-((this->D(1,2))*(this->D(2,1)))/"
                     "(this->D(2,2)))*prediction_strain(1));\n";
                m += "prediction_stress(3) = " + de +
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
              m += "prediction_stress(0) = " + de + "*2*(" + mu + ")*((" +
                   lambda + ")/(" + lambda + "+2*(" + mu +
                   "))*(prediction_strain(0)+prediction_strain(1))+"
                   "prediction_"
                   "strain(0));\n";
              m += "prediction_stress(1) = " + de + "*2*(" + mu + ")*((" +
                   lambda + ")/(" + lambda + "+2*(" + mu +
                   "))*(prediction_strain(0)+prediction_strain(1))+"
                   "prediction_"
                   "strain(1));\n";
              m += "prediction_stress(3) = " + de + "*2*(" + mu +
                   ")*prediction_strain(3);\n";
              m += "prediction_stress(2) = stress(0);\n";
              m += "return prediction_stress;\n";
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "IsotropicDamageHookeStressPotentialBase::"
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
            m += "return " + de +
                 "*(this->D)*(this->eel+(this->theta)*this->deto);\n";
          } else {
            if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
              auto b = bd.getAttribute(
                  "HookeStressPotentialBase::UseLocalLameCoeficients", false);
              const std::string lambda =
                  b ? "this->sebdata.lambda" : "this->lambda";
              const std::string mu = b ? "this->sebdata.mu" : "this->mu";
              m += "return " + de + "*(" + lambda +
                   "*trace(this->eel+(this->theta)*(this->deto))*Stensor::Id("
                   ")+";
              m += "2*(" + mu + ")*(this->eel+(this->theta)*(this->deto)));\n";
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "IsotropicDamageHookeStressPotentialBase::"
                             "declareComputeElasticPredictionMethod: "
                             "the stiffness tensor must be defined for "
                             "orthotropic behaviours");
            }
          }
        }
        m += "}\n";
        bd.appendToMembers(h, m, false);
      }
    }  // end of
       // IsotropicDamageHookeStressPotentialBase::declareComputeElasticPredictionMethod

    void IsotropicDamageHookeStressPotentialBase::
        declareComputeStressWhenStiffnessTensorIsDefined(
            BehaviourDescription& bd) const {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      CodeBlock smts, sets;
      const std::string D_tdt =
          bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                false)
              ? "this->D_tdt"
              : "this->D";
      const std::string d_mts = "(1-this->d-(this->theta)*(this->dd))";
      const std::string d_ets = "(1-this->d)";
      smts.code = "this->sig = " + d_mts +
                  "*(this->D)*(this->eel+(this->theta)*(this->deel))"
                  ";\n";
      sets.code = "this->sig = " + d_ets + "*(" + D_tdt + ")*(this->eel);\n";
      bd.setCode(h, BehaviourData::ComputeThermodynamicForces, smts,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
      bd.setCode(h, BehaviourData::ComputeFinalThermodynamicForces, sets,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    }  // end of
    // IsotropicDamageHookeStressPotentialBase::declareComputeStressWhenStiffnessTensorIsDefined

    void IsotropicDamageHookeStressPotentialBase::
        declareComputeStressForIsotropicBehaviour(BehaviourDescription& bd,
                                                  LocalDataStructure&) const {
      using tfel::glossary::Glossary;
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "IsotropicDamageHookeStressPotentialBase::"
                          "declareComputeStressForIsotropicBehaviour: begin\n";
      }
      CodeBlock smts;
      CodeBlock sets;
      if (bd.areElasticMaterialPropertiesDefined()) {
        const std::string d_mts = "(1-this->d-(this->theta)*(this->dd))";
        const std::string d_ets = "(1-this->d)";
        smts.code =
            "this->sig = " + d_mts +
            "*(this->lambda*trace(this->eel+(this->theta)*(this->deel))"  //
            "*Stensor::Id()+"                                             //
            "2*(this->mu)*(this->eel+(this->theta)*(this->deel)));\n";
        sets.code = "this->sig = " + d_ets +
                    "*(this->lambda_tdt*trace(this->eel)*Stensor::Id()+"  //
                    "2*(this->mu_tdt)*this->eel);\n";
      } else {
        // IsotropicDamageHooke law
        // damage effect
        const std::string d_mts = "(1-this->d-(this->theta)*(this->dd))";
        const std::string d_ets = "(1-this->d)";
        smts.code =
            "this->sig = " + d_mts +
            " * ("                                                        //
            "(this->sebdata.lambda)*"                                     //
            "trace(this->eel+(this->theta)*(this->deel))*Stensor::Id()+"  //
            "2*(this->sebdata.mu)*(this->eel+(this->theta)*(this->deel)));\n";
        sets.code = "this->sig = " + d_ets +
                    " * ("                                                    //
                    "(this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+"  //
                    "2*(this->sebdata.mu)*this->eel);\n";
      }
      bd.setCode(uh, BehaviourData::ComputeThermodynamicForces, smts,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
      bd.setCode(uh, BehaviourData::ComputeFinalThermodynamicForces, sets,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "IsotropicDamageHookeStressPotentialBase::"
                          "declareComputeStressForIsotropicBehaviour: end\n";
      }
    }  // end of
       // IsotropicDamageHookeStressPotentialBase::declareComputeStressForIsotropicBehaviour

    void
    IsotropicDamageHookeStressPotentialBase::addGenericTangentOperatorSupport(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b,
                       "IsotropicDamageHookeStressPotentialBase::"
                       "addGenericTangentOperatorSupport: " +
                           m);
      };
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
      CodeBlock to;

      bd.checkVariablePosition("d", "IntegrationVariable", 1u);
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        const std::string D =
            bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)
                ? "this->D"
                : "this->D_tdt";
        to.code += "if(smt==ELASTIC){\n";
        to.code += "  this->Dt = " + D + ";\n";
        to.code += "} else if(smt==SECANTOPERATOR){\n";
        to.code +=
            "  this->Dt = (1-min(this->d,this->damage_thresold))*" + D + ";\n";
        if (idsl.getSolver().usesJacobian()) {
          to.code += "} else if (smt==CONSISTENTTANGENTOPERATOR){\n";
          to.code += "  Stensor4 Je;\n";
          to.code += "  Stensor  Jd;\n";
          to.code += "  getPartialJacobianInvert(Je,Jd);\n";
          to.code += "  Dt = (1-min(this->d,this->damage_thresold)) * (" + D +
                     ") * Je - ";
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
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
              lambda + "," + mu + ");\n";
          to.code += "} else if(smt==SECANTOPERATOR){\n";
          to.code +=
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,";
          to.code +=
              "(1-min(this->d,this->damage_thresold)) * (" + lambda + "),";
          to.code +=
              "(1-min(this->d,this->damage_thresold)) * (" + mu + "));\n";
          if (idsl.getSolver().usesJacobian()) {
            to.code += "} else if (smt==CONSISTENTTANGENTOPERATOR){\n";
            to.code += "  StiffnessTensor IsotropicDamageHooke;\n";
            to.code += "  Stensor4 Je;\n";
            to.code += "  Stensor  Jd;\n";
            to.code += "  getPartialJacobianInvert(Je,Jd);\n";
            to.code +=
                "  computeElasticStiffness<N,Type>::exe(IsotropicDamageHooke," +
                lambda + "," + mu + ");\n";
            to.code +=
                "  Dt = (1-min(this->d,this->damage_thresold)) * "
                "IsotropicDamageHooke * Je "
                "- ";
            to.code += "  ((IsotropicDamageHooke*(this->eel)) ^ (Jd));\n";
          }
          to.code += "} else {\n";
          to.code += "  return false;\n";
          to.code += "}";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::computesStiffnessTensor, false),
                   "orthotropic behaviour shall require the stiffness tensor");
          to.code += "if((smt==ELASTIC){\n";
          to.code += "  this->Dt = this->D_tdt;\n";
          to.code += "} else if(smt==SECANTOPERATOR){\n";
          to.code +=
              "  this->Dt = (1-min(this->d,this->damage_thresold)) * "
              "D_tdt;\n";
          if (idsl.getSolver().usesJacobian()) {
            to.code += "} else if (smt==CONSISTENTTANGENTOPERATOR){\n";
            to.code += "  Stensor4 Je;\n";
            to.code += "  Stensor  Jd;\n";
            to.code += "  getPartialJacobianInvert(Je,Jd);\n";
            to.code +=
                "  Dt = (1-min(this->d,this->damage_thresold)) * "
                "(this->D_tdt) * Je - ";
            to.code += "  (((this->D_tdt) * eel) ^ (Jd));\n";
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

    void IsotropicDamageHookeStressPotentialBase::
        addGenericPredictionOperatorSupport(BehaviourDescription& bd) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b,
                       "IsotropicDamageHookeStressPotentialBase::"
                       "addGenericPredictionOperatorSupport: " +
                           m);
      };
      CodeBlock to;
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
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
            throw_if(
                !bd.getAttribute<bool>(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor),
                "genertic tangent operator support for "
                "plane stress hypotheses requires the use of an unaltered "
                "stiffness tensor");
          }
        }
        const std::string D =
            bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)
                ? "this->D"
                : "this->D_tdt";
        to.code += "if(smt==ELASTIC){\n";
        to.code += "  this->Dt = " + D + ";\n";
        to.code += "} else if(smt==SECANTOPERATOR){\n";
        to.code +=
            "  this->Dt = (1-min(this->d,this->damage_thresold))*" + D + ";\n";
        to.code += "} else {\n";
        to.code += "  return FAILURE;\n";
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
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
              lambda + "," + mu + ");\n";
          to.code += "} else if(smt==SECANTOPERATOR){\n";
          to.code +=
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,";
          to.code +=
              "(1-min(this->d,this->damage_thresold)) * (" + lambda + "),";
          to.code +=
              "(1-min(this->d,this->damage_thresold)) * (" + mu + "));\n";
          to.code += "} else {\n";
          to.code += "  return FAILURE;\n";
          to.code += "}";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::computesStiffnessTensor, false),
                   "orthotropic behaviour shall require the stiffness tensor");
          to.code += "if(smt==ELASTIC){\n";
          to.code += "  this->Dt = this->D_tdt;\n";
          to.code += "} else if(smt==SECANTOPERATOR){\n";
          to.code +=
              "  this->Dt = "
              "(1-min(this->d,this->damage_thresold)x)*(this->D_tdt);\n";
          to.code += "} else {\n";
          to.code += "  return FAILURE;\n";
          to.code += "}";
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

    std::string IsotropicDamageHookeStressPotentialBase::computeDerivatives(
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
              "IsotropicDamageHookeStressPotentialBase::computeDerivatives: "
              "unsupported type for variable '" +
              t + "'");
        }
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          auto bl = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              bl ? "this->sebdata.lambda" : "this->lambda";
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
              c +=
                  "2 * (" + mu +
                  ") * (this->theta) * (1-this->d-(this->theta)*(this->dd)) * ";
              c += "(" + dfv_ds + ");\n";
              c += "df" + v + "_ddd -= ";
              c += "2 * (" + mu + ") *(this->theta) * (" + dfv_ds + ") * ";
              c += "deviator(this->eel + (this->theta) * (this->deel));\n";
            } else {
              tfel::raise(
                  "IsotropicDamageHookeStressPotentialBase::computeDerivatives:"
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
                  "IsotropicDamageHookeStressPotentialBase::computeDerivatives:"
                  " "
                  "unsupported type for variable '" +
                  t + "'");
            }
          }
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          if (!bd.getAttribute<bool>(
                  BehaviourDescription::computesStiffnessTensor, false)) {
            tfel::raise(
                "IsotropicDamageHookeStressPotentialBase::computeDerivatives: "
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
                "IsotropicDamageHookeStressPotentialBase::computeDerivatives: "
                "unsupported type for variable '" +
                t + "'");
          }
        } else {
          tfel::raise(
              "IsotropicDamageHookeStressPotentialBase::computeDerivatives: "
              "unsupported elastic symmetry type");
        }
      }
      return c;
    }  // end of IsotropicDamageHookeStressPotentialBase::computeDerivatives

    IsotropicDamageHookeStressPotentialBase::
        ~IsotropicDamageHookeStressPotentialBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
