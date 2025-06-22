/*!
 * \file   mfront/src/HookeStressPotential.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
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
#include "MFront/MFrontLogStream.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"

namespace mfront {

  namespace bbrick {

    HookeStressPotential::HookeStressPotential() = default;

    void HookeStressPotential::declareComputeElasticPredictionMethod(
        BehaviourDescription& bd) const {
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
                  m += "this->D(1,0),this->D(1,1),this->D(1,2),";
                  m += "this->eel,this->etozz,this->deto,";
                  m += "this->sigzz + this->dsigzz,";
                  m += "real(1),this->epsilon);\n";
                  m += "const StressStensor prediction_stress = ";
                  m += "(this->D)*prediction_strain;\n";
                  m += "return prediction_stress;\n";
                } else {
                  m += "StressStensor prediction_stress;\n";
                  m += "StrainStensor prediction_strain = "
                       "this->eel+(this->theta)*this->deto;\n";
                  m += "prediction_stress(1) = "
                       "this->sigzz+(this->theta)*(this->dsigzz);\n";
                  m += "prediction_stress(0) = "
                       "(this->D(0,0)-((this->D(0,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(0,2)-((this->D(0,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(0,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz));"
                       "\n";
                  m += "prediction_stress(2) = "
                       "(this->D(2,0)-((this->D(2,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(2,2)-((this->D(2,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(2,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz));"
                       "\n";
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
                m += lambda + ",2*(" + mu + ")+" + lambda + "," + lambda + ",";
                m += "this->eel,this->etozz,this->deto,";
                m += "this->sigzz+this->dsigzz,";
                m += "real(1),this->epsilon);\n";
                m += "const StressStensor prediction_stress = ";
                m += "(" + lambda +
                     ") * trace(prediction_strain) * Stensor::Id() + ";
                m += "2 * (" + mu + ") * prediction_strain;\n";
                m += "return prediction_stress;\n";
              } else {
                m += "StressStensor prediction_stress;\n";
                m += "StrainStensor prediction_strain = "
                     "this->eel+(this->theta)*this->deto;\n";
                m += "prediction_stress(0) = 2*(" + mu + ")*((" + lambda +
                     ")/(" + lambda + "+2*(" + mu +
                     "))*(prediction_strain(0)+prediction_strain(2))+"
                     "prediction_"
                     "strain(0))+\n";
                m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                     "))*(this->sigzz+(this->theta)*(this->dsigzz));\n";
                m += "prediction_stress(2) = 2*(" + mu + ")*((" + lambda +
                     ")/(" + lambda + "+2*(" + mu +
                     "))*(prediction_strain(0)+prediction_strain(2))+"
                     "prediction_"
                     "strain(2))+\n";
                m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                     "))*(this->sigzz+(this->theta)*(this->dsigzz));\n";

                m += "prediction_stress(1) = "
                     "this->sigzz+(this->theta)*(this->dsigzz);\n";
                m += "return prediction_stress;\n";
              }
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "HookeStressPotential::"
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
                m += "prediction_stress(0) = "
                     "(this->D(0,0)-((this->D(0,2))*(this->D(2,0)))/"
                     "(this->D(2,2)))*prediction_strain(0)+\n";
                m += "                       "
                     "(this->D(0,1)-((this->D(0,2))*(this->D(2,1)))/"
                     "(this->D(2,2)))*prediction_strain(1);\n";
                m += "prediction_stress(1) = "
                     "(this->D(1,0)-((this->D(1,2))*(this->D(2,0)))/"
                     "(this->D(2,2)))*prediction_strain(0)+\n";
                m += "                       "
                     "(this->D(1,1)-((this->D(1,2))*(this->D(2,1)))/"
                     "(this->D(2,2)))*prediction_strain(1);\n";
                m += "prediction_stress(3) = "  //
                     "(this->D(3,3))*prediction_strain(3);\n";
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
              m += "prediction_stress(0) = 2*(" + mu + ")*((" + lambda + ")/(" +
                   lambda + "+2*(" + mu +
                   "))*(prediction_strain(0)+prediction_strain(1))+prediction_"
                   "strain(0));\n";
              m += "prediction_stress(1) = 2*(" + mu + ")*((" + lambda + ")/(" +
                   lambda + "+2*(" + mu +
                   "))*(prediction_strain(0)+prediction_strain(1))+prediction_"
                   "strain(1));\n";
              m += "prediction_stress(3) = 2*(" + mu +
                   ")*prediction_strain(3);\n";
              m += "prediction_stress(2) = stress(0);\n";
              m += "return prediction_stress;\n";
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "HookeStressPotential::"
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
            m += "return (this->D)*(this->eel+(this->theta)*this->deto);\n";
          } else {
            if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
              auto b = bd.getAttribute(
                  "HookeStressPotentialBase::UseLocalLameCoeficients", false);
              const std::string lambda =
                  b ? "this->sebdata.lambda" : "this->lambda";
              const std::string mu = b ? "this->sebdata.mu" : "this->mu";
              m += "return " + lambda +
                   "*trace(this->eel+(this->theta)*(this->deto))*Stensor::Id("
                   ")+"
                   "2*(" +
                   mu + ")*(this->eel+(this->theta)*(this->deto));\n";
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "HookeStressPotential::"
                             "declareComputeElasticPredictionMethod: "
                             "the stiffness tensor must be defined for "
                             "orthotropic behaviours");
            }
          }
        }
        m += "}\n";
        bd.appendToMembers(h, m, false);
      }
    }  // end of HookeStressPotential::declareComputeElasticPredictionMethod

    void HookeStressPotential::declareComputeStressWhenStiffnessTensorIsDefined(
        BehaviourDescription& bd) const {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      CodeBlock smts, sets;
      const std::string D_tdt =
          bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                false)
              ? "this->D_tdt"
              : "this->D";
      smts.code =
          "this->sig = (this->D)*(this->eel+(this->theta)*(this->deel))"
          ";\n";
      sets.code = "this->sig = (" + D_tdt + ")*(this->eel);\n";
      bd.setCode(h, BehaviourData::ComputeStress, smts, BehaviourData::CREATE,
                 BehaviourData::AT_BEGINNING, false);
      bd.setCode(h, BehaviourData::ComputeFinalStress, sets,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    }  // end of
    // HookeStressPotential::declareComputeStressWhenStiffnessTensorIsDefined

    void HookeStressPotential::declareComputeStressForIsotropicBehaviour(
        BehaviourDescription& bd, LocalDataStructure& d) const {
      using tfel::glossary::Glossary;
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "HookeStressPotential::"
                          "declareComputeStressForIsotropicBehaviour: begin\n";
      }
      CodeBlock smts;
      CodeBlock sets;
      if (bd.areElasticMaterialPropertiesDefined()) {
        smts.code =
            "this->sig=this->lambda*trace(this->eel+(this->theta)*(this->"
            "deel))*Stensor::Id()+"  //
            "2*(this->mu)*(this->eel+(this->theta)*(this->deel));\n";
        sets.code =
            "this->sig=this->lambda_tdt*trace(this->eel)*Stensor::Id()+"  //
            "2*(this->mu_tdt)*this->eel;\n";
      } else {
        bd.setAttribute("HookeStressPotentialBase::UseLocalLameCoeficients",
                        true, false);
        addMaterialPropertyIfNotDefined(bd, "stress", "young",
                                        Glossary::YoungModulus);
        addMaterialPropertyIfNotDefined(bd, "real", "nu",
                                        Glossary::PoissonRatio);
        d.addVariable(uh, {"stress", "lambda"});
        d.addVariable(uh, {"stress", "mu"});
        // local variable initialisation
        CodeBlock init;
        init.code =
            "// initialisation Lame's coefficient\n"
            "this->sebdata.lambda = "  //
            "tfel::material::computeLambda(this->young,this->nu);\n"
            "this->sebdata.mu = "  //
            "tfel::material::computeMu(this->young,this->nu);\n";
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING,
                   true);
        // Hooke law
        smts.code =
            "this->sig = "                                                //
            "(this->sebdata.lambda)*"                                     //
            "trace(this->eel+(this->theta)*(this->deel))*Stensor::Id()+"  //
            "2*(this->sebdata.mu)*(this->eel+(this->theta)*(this->deel));\n";
        sets.code =
            "this->sig = "                                            //
            "(this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+"  //
            "2*(this->sebdata.mu)*this->eel;\n";
      }
      bd.setCode(uh, BehaviourData::ComputeStress, smts, BehaviourData::CREATE,
                 BehaviourData::AT_BEGINNING, false);
      bd.setCode(uh, BehaviourData::ComputeFinalStress, sets,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "HookeStressPotential::"
                          "declareComputeStressForIsotropicBehaviour: end\n";
      }
    }  // end of HookeStressPotential::declareComputeStressForIsotropicBehaviour

    void HookeStressPotential::addGenericTangentOperatorSupport(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(
            b, "HookeStressPotential::addGenericTangentOperatorSupport: " + m);
      };
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
      CodeBlock to;
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
        to.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  this->Dt = " +
            D + ";\n";
        if (idsl.getSolver().usesJacobian()) {
          to.code +=
              "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
              "  Stensor4 Je;\n"
              "  getPartialJacobianInvert(Je);\n"
              "  this->Dt = (" +
              D + ")*Je;\n";
        }
        to.code +=
            "} else {\n"
            "  return false;\n"
            "}";
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda_tdt";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
          to.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
              lambda + "," + mu + ");\n";
          if (idsl.getSolver().usesJacobian()) {
            to.code +=
                "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
                "  StiffnessTensor Hooke;\n"
                "  Stensor4 Je;\n"
                "  computeElasticStiffness<N,Type>::exe(Hooke," +
                lambda + "," + mu +
                ");\n"
                "  getPartialJacobianInvert(Je);\n"
                "  Dt = Hooke*Je;\n";
          }
          to.code +=
              "} else {\n"
              "  return false;\n"
              "}";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::computesStiffnessTensor, false),
                   "orthotropic behaviour shall require the stiffness tensor");
          to.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  this->Dt = this->D_tdt;\n";
          if (idsl.getSolver().usesJacobian()) {
            to.code +=
                "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
                "  Stensor4 Je;\n"
                "  getPartialJacobianInvert(Je);\n"
                "  this->Dt = (this->D_tdt)*Je;\n";
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

    void HookeStressPotential::addGenericPredictionOperatorSupport(
        BehaviourDescription& bd) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(
            b,
            "HookeStressPotential::addGenericPredictionOperatorSupport: " + m);
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
                "plane stress hypotheses requires the use of an "
                "unaltered "
                "stiffness tensor");
          }
        }
        const std::string D =
            bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)
                ? "this->D"
                : "this->D_tdt";
        to.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  this->Dt = " +
            D +
            ";\n"
            "} else {\n"
            "  return FAILURE;\n"
            "}";
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda_tdt";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
          to.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  "
              "computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
              lambda + "," + mu +
              ");\n"
              "} else {\n"
              "  return FAILURE;\n"
              "}";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::computesStiffnessTensor, false),
                   "orthotropic behaviour shall require the stiffness tensor");
          to.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  this->Dt = this->D_tdt;\n"
              "} else {\n"
              "  return FAILURE;\n"
              "}";
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

    std::string HookeStressPotential::computeDerivatives(
        const BehaviourDescription& bd,
        const std::string& t,
        const std::string& v,
        const std::string& dfv_ds,
        const bool b) const {
      auto c = std::string{};
      const auto vf = SupportedTypes::getTypeFlag(t);
      c = "df" + v + "_ddeel += ";
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        if (vf == SupportedTypes::Scalar) {
          c += "(this->theta) * ((" + dfv_ds + ") | (this->D));\n";
        } else if (vf == SupportedTypes::Stensor) {
          c += "(this->theta) * (" + dfv_ds + ") * (this->D);\n";
        } else {
          tfel::raise(
              "HookeStressPotential::computeDerivatives: "
              "unsupported elastic symmetry type");
        }
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          const auto bl = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              bl ? "this->sebdata.lambda" : "this->lambda";
          const std::string mu = bl ? "this->sebdata.mu" : "this->mu";
          if (b) {
            if (vf == SupportedTypes::Scalar) {
              c += "(2 * " + mu + ")*(this->theta) * (" + dfv_ds + ");\n";
            } else if (vf == SupportedTypes::Stensor) {
              c += "(2 * " + mu + ")*(this->theta) * (" + dfv_ds + ");\n";
            } else {
              tfel::raise(
                  "HookeStressPotential::computeDerivatives: "
                  "unsupported elastic symmetry type");
            }
          } else {
            if (vf == SupportedTypes::Scalar) {
              c += "(this->theta) * ((" + dfv_ds + ") | (2 * (" + mu +
                   ") * Stensor4::Id()+(" + lambda + ") * Stensor4::IxI()));\n";
            } else if (vf == SupportedTypes::Stensor) {
              c += "(this->theta) * (" + dfv_ds + ") * (2 * (" + mu +
                   ") * Stensor4::Id()+(" + lambda + ") * Stensor4::IxI());\n";
            } else {
              tfel::raise(
                  "HookeStressPotential::computeDerivatives: "
                  "unsupported elastic symmetry type");
            }
          }
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          if (!bd.getAttribute<bool>(
                  BehaviourDescription::computesStiffnessTensor, false)) {
            tfel::raise(
                "HookeStressPotential::computeDerivatives: "
                "orthotropic behaviour shall require the stiffness tensor");
          }
          if (vf == SupportedTypes::Scalar) {
            c += "(this->theta) * ((" + dfv_ds + ") | (this->D));\n";
          } else if (vf == SupportedTypes::Stensor) {
            c += "(this->theta) * (" + dfv_ds + ") * (this->D);\n";
          } else {
            tfel::raise(
                "HookeStressPotential::computeDerivatives: "
                "unsupported elastic symmetry type");
          }
        } else {
          tfel::raise(
              "HookeStressPotential::computeDerivatives: "
              "unsupported elastic symmetry type");
        }
      }
      return c;
    }  // end of HookeStressPotential::computeDerivatives

    HookeStressPotential::~HookeStressPotential() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
