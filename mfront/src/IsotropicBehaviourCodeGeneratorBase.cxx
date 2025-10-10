/*!
 * \file   mfront/src/IsotropicBehaviourCodeGeneratorBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/IsotropicBehaviourDSLBase.hxx"
#include "MFront/IsotropicBehaviourCodeGeneratorBase.hxx"

namespace mfront {

  IsotropicBehaviourCodeGeneratorBase::IsotropicBehaviourCodeGeneratorBase(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim)
      : BehaviourCodeGeneratorBase(f, d, bim) {
  }  // end of IsotropicBehaviourCodeGeneratorBase

  void IsotropicBehaviourCodeGeneratorBase::
      writeBehaviourLocalVariablesInitialisation(std::ostream& os,
                                                 const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier mts = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+(this->theta)*(this->d" + i.name + ')';
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "IsotropicBehaviourCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "IsotropicBehaviourCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    if (this->bd.areElasticMaterialPropertiesDefined()) {
      const auto& emps = this->bd.getElasticMaterialProperties();
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "IsotropicBehaviourCodeGenerator::"
            "writeBehaviourLocalVariablesInitialisation",
            "invalid number of material properties");
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], mts);
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], mts);
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        os << "this->young = stress(";
        this->writeMaterialPropertyEvaluation(os, emps[0], mts);
        os << ");\n";
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        os << "this->nu = real(";
        this->writeMaterialPropertyEvaluation(os, emps[1], mts);
        os << ");\n";
      }
      os << "this->lambda = computeLambda(young,nu);\n";
      os << "this->mu = computeMu(young,nu);\n";
      if (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], ets);
        os << "this->young_tdt = stress(";
        this->writeMaterialPropertyEvaluation(os, emps[0], ets);
        os << ");\n";
      } else {
        os << "this->young_tdt  = this->young;\n";
      }
      if (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[1])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], ets);
        os << "this->nu_tdt = real(";
        this->writeMaterialPropertyEvaluation(os, emps[1], ets);
        os << ");\n";
      } else {
        os << "this->nu_tdt     = this->nu;\n";
      }
      if (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        os << "this->lambda_tdt = computeLambda(young_tdt,nu_tdt);\n";
        os << "this->mu_tdt     = computeMu(young_tdt,nu_tdt);\n";
      } else {
        os << "this->lambda_tdt = this->lambda;\n"
           << "this->mu_tdt     = this->mu;\n";
      }
    } else {
      os << "this->lambda = "
         << "tfel::material::computeLambda(this->young,this->nu);\n"
         << "this->mu=tfel::material::computeMu(this->young,this->nu);\n"
         << "this->lambda_tdt = this->lambda;\n"
         << "this->mu_tdt     = this->mu;\n";
    }
    BehaviourCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(os,
                                                                           h);
  }  // end of writeBehaviourLocalVariablesInitialisation

  void
  IsotropicBehaviourCodeGeneratorBase::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
  }  // end of writeBehaviourParserSpecificIncludes

  void
  IsotropicBehaviourCodeGeneratorBase::writeBehaviourParserSpecificTypedefs(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
  }  // end of writeBehaviourParserSpecificTypedefs

  void
  IsotropicBehaviourCodeGeneratorBase::writeBehaviourComputePredictionOperator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    if (!this->bd.hasCode(h, BehaviourData::ComputePredictionOperator)) {
      os << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult\n"
         << "computePredictionOperator(const SMFlag smflag, const SMType smt) "
         << "override final {\n";
      os << "using namespace std;\n";
      if (this->bd.useQt()) {
        os << "if(smflag!=MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, use_qt>::STANDARDTANGENTOPERATOR){\n";
      } else {
        os << "if(smflag!=MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, false>::STANDARDTANGENTOPERATOR){\n";
      }
      os << "throw(runtime_error(\"" << this->bd.getClassName()
         << "::computePredictionOperator : "
         << "invalid tangent operator flag\"));\n"
         << "}\n"
         << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
         << "Dt = "
            "(this->lambda_tdt)*Stensor4::IxI()+2*(this->mu_tdt)*Stensor4::Id()"
            ";\n"
         << "} else {\n"
         << "string msg(\"" << this->bd.getClassName()
         << "::computePredictionOperator : \");\n"
         << "msg +=\"unimplemented feature\";\n"
         << "throw(runtime_error(msg));\n"
         << "}\n\n"
         << "return SUCCESS;\n"
         << "}\n\n";
    } else {
      BehaviourCodeGeneratorBase::writeBehaviourComputePredictionOperator(os,
                                                                          h);
    }
  }  // end of writeBehaviourComputePredictionOperator

  bool IsotropicBehaviourCodeGeneratorBase::shallComputeTheElasticStrain()
      const {
    if (!this->bd.getAttribute(
            IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
      return false;
    }
    if (!this->bd.areElasticMaterialPropertiesDefined()) {
      return false;
    }
    const auto& emps = this->bd.getElasticMaterialProperties();
    if (emps.size() != 2u) {
      this->throwRuntimeError(
          "IsotropicMisesCreepCodeGenerator::writeBehaviourIntegrator",
          "invalid number of material properties");
    }
    return (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) ||
           (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[1]));
  }  // end of shallComputeTheElasticStrain

  void IsotropicBehaviourCodeGeneratorBase::
      writeBehaviourIntegratorPreprocessingStep(std::ostream& os) const {
    if (this->bd.getAttribute(
            IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
      if (this->shallComputeTheElasticStrain()) {
        using Modifier =
            std::function<std::string(const MaterialPropertyInput&)>;
        Modifier bts = [this](const MaterialPropertyInput& i) -> std::string {
          if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
              (i.category == MaterialPropertyInput::
                                 AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
              (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE) ||
              (i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
              (i.category == MaterialPropertyInput::PARAMETER)) {
            return "this->" + i.name;
          } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
            return this->bd.getClassName() + "::" + i.name;
          } else {
            this->throwRuntimeError(
                "IsotropicMisesCreepCodeGenerator::writeBehaviourIntegrator",
                "unsupported input type for variable '" + i.name + "'");
          }
        };
        const auto& emps = this->bd.getElasticMaterialProperties();
        if (emps.size() != 2u) {
          this->throwRuntimeError(
              "IsotropicMisesCreepCodeGenerator::writeBehaviourIntegrator",
              "invalid number of material properties");
        }
        if (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) {
          this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], bts);
          os << "const auto mfront_young_bts = stress(";
          this->writeMaterialPropertyEvaluation(os, emps[0], bts);
          os << ");\n";
        } else {
          os << "const auto mfront_young_bts = this->young;";
        }
        if (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[1])) {
          this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], bts);
          os << "const auto mfront_nu_bts = stress(";
          this->writeMaterialPropertyEvaluation(os, emps[1], bts);
          os << ");\n";
        } else {
          os << "const auto mfront_nu_bts = this->nu;";
        }
        os << "const auto mfront_eel_bts = eval("
           << "-mfront_nu_bts / mfront_young_bts * trace(this->sig) * "
           << "Stensor::Id()"
           << "+ (1+mfront_nu_bts) / mfront_young_bts * (this->sig));\n"
           << "this->se = 2 * (this->mu) * ("
           << "tfel::math::deviator(mfront_eel_bts + (this->theta) * "
              "(this->deto)));\n";
      } else {
        os << "this->se =  tfel::math::deviator(this->sig + "
           << "2 * (this->mu) * (this->theta) * (this->deto));\n";
      }
    } else {
      os << "this->se = 2 * (this->mu) * ("
         << "tfel::math::deviator(this->eel + (this->theta) * "
            "(this->deto)));\n";
    }
  }  // end of writeBehaviourIntegratorPreprocessingStep

  void IsotropicBehaviourCodeGeneratorBase::
      writeBehaviourIntegratorPostprocessingStep(std::ostream& os) const {
    if (!this->bd.getAttribute(
            IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
      os << "this->deel = this->deto - (this->dp) * (this->n);\n";
    }
    os << "this->updateStateVariables();\n";
    if (this->bd.getAttribute(
            IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
      if (this->shallComputeTheElasticStrain()) {
        os << "const auto mfront_eel_ets = "
           << "eval(mfront_eel_bts + this->deto - (this->dp) * (this->n));\n"
           << "this->sig = "
           << "(this->lambda_tdt) * trace(mfront_eel_ets) * Stensor::Id() + "
           << "2 * (this->mu_tdt) * (mfront_eel_ets);\n";
      } else {
        os << "this->sig += "
           << "(this->lambda_tdt) * trace(this->deto) * Stensor::Id() + "
           << "2 * (this->mu_tdt) * (this->deto - (this->dp) * (this->n));\n";
      }
    } else {
      os << "this->sig = "
         << "(this->lambda_tdt) * trace(this->eel) * Stensor::Id() + "
         << "2 * (this->mu_tdt) * (this->eel);\n";
    }
    os << "this->updateAuxiliaryStateVariables();\n";
  }  // end of writeBehaviourIntegratorPostocessingStep

  void
  IsotropicBehaviourCodeGeneratorBase::writeBehaviourComputeTangentOperator(
      std::ostream&, const Hypothesis) const {
  }  // end of writeBehaviourComputeTangentOperator

  IsotropicBehaviourCodeGeneratorBase::~IsotropicBehaviourCodeGeneratorBase() =
      default;

}  // end of namespace mfront
