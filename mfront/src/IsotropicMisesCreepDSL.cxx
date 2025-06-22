/*!
 * \file   mfront/src/IsotropicMisesCreepDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/IsotropicMisesCreepDSL.hxx"

namespace mfront {

  std::string IsotropicMisesCreepDSL::getDescription() {
    return "this parser is used for standard creep behaviours of the form  "
           " dp/dt=f(s) where p is the equivalent creep strain and s the "
           "equivalent mises stress";
  }  // end of getDescription

  std::string IsotropicMisesCreepDSL::getName() {
    return "IsotropicMisesCreep";
  }

  IsotropicMisesCreepDSL::IsotropicMisesCreepDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("IsotropicMisesCreepDSL");
    // Default state vars
    this->reserveName("ccto_tmp_1");
    this->reserveName("inv_sqrt2");
    this->reserveName("mu_3_theta");
    this->mb.addStateVariable(
        h, VariableDescription("StrainStensor", "εᵉˡ", "eel", 1u, 0u));
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->mb.setGlossaryName(h, "p", "EquivalentViscoplasticStrain");
    // default local vars
    this->mb.addLocalVariable(h,
                              VariableDescription("strainrate", "f", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription(
               "tfel::math::derivative_type<strainrate, stress>",
               "\u2202f\u2215\u2202\u03C3\u2091", "df_dseq", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StressStensor", "se", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("stress", "\u03C3\u2091", "seq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "seq_e", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StrainStensor", "n", 1u, 0u));
    this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator, true);
    this->mb.setAttribute(
        h, BehaviourData::isConsistentTangentOperatorSymmetric, true);
  }

  std::string IsotropicMisesCreepDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::FlowRule) {
      if (o.useUnicodeSymbols) {
        return "@FlowRule{\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "}";
      } else {
        return "@FlowRule{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "}";
      }
    }
    return "";
  }  // end of getCodeBlockTemplate

  void IsotropicMisesCreepDSL::endsInputFileProcessing() {
    IsotropicBehaviourDSLBase::endsInputFileProcessing();
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
      this->throwRuntimeError("IsotropicMisesCreepDSL::endsInputFileProcessing",
                              "no flow rule defined");
    }
  }  // end of endsInputFileProcessing

  void IsotropicMisesCreepDSL::writeBehaviourParserSpecificMembers(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
      this->throwRuntimeError(
          "IsotropicMisesCreepDSL::writeBehaviourParserSpecificMembers",
          "no flow rule declared (use the @FlowRule directive)");
    }
    os << "bool computeFlow(){\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "using namespace tfel::material;\n"
       << "using std::vector;\n";
    writeMaterialLaws(os, this->mb.getMaterialLaws());
    os << this->mb.getCode(h, BehaviourData::FlowRule) << "return true;\n}\n\n"
       << "bool NewtonIntegration(){\n"
       << "constexpr auto newton_epsilon = "
          "100*std::numeric_limits<strain>::epsilon();\n"
       << "bool converged=false;\n"
       << "strain newton_f;\n"
       << "strain newton_df;\n"
       << "auto newton_ddp = strain{}; // previous correction of the Newton "
          "algorithm\n"
       << "const auto mu_3_theta = 3*(this->theta)*(this->mu);\n"
       << "unsigned int iter=0;\n";
    os << "while((converged==false)&&(iter<(this->iterMax))){\n"
       << "this->seq = std::max(this->seq_e-mu_3_theta*(this->dp),stress(0));\n"
       << "const auto compute_flow_r = this->computeFlow();\n"
       << "if(!((compute_flow_r)&&\n"
       << "(tfel::math::ieee754::isfinite(this->f))&&\n"
       << "(tfel::math::ieee754::isfinite(this->df_dseq)))){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": invalid evaluation of the flow rate or its "
         << "derivative\\n\";\n";
    }
    os << "if(iter==0u){\n"
       << "// probably an elastic prediction\n"
       << "newton_ddp = (this->seq_e / mu_3_theta) / 2;\n"
       << "} // end of if(iter==0u)\n"
       << "this->dp += newton_ddp;\n"
       << "iter+=1;\n"
       << "} else {"
       << "newton_f  = this->dp - (this->f)*(this->dt);\n"
       << "newton_df = 1+mu_3_theta*(this->df_dseq)*(this->dt);\n"
       << "if(tfel::math::abs(newton_df) > newton_epsilon){\n"
       << "newton_ddp = -newton_f/newton_df;\n"
       << "this->dp += newton_ddp;\n"
       << "iter+=1;\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \" : \" << tfel::math::abs(newton_f) "
            "<< '\\n';\n";
    }
    os << "converged = (tfel::math::abs(newton_f) < (this->epsilon));\n"
       << "} else {\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": invalid jacobian\\n\";\n";
    }
    os << "if(iter==0u){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": invalid jacobian on the first iteration\\n\";\n";
    }
    os << "// probably an elastic prediction\n"
       << "newton_ddp = (this->seq_e / mu_3_theta) / 2;\n"
       << "} // end of if(iter==0u)\n"
       << "this->dp += newton_ddp;\n"
       << "iter+=1;\n"
       << "}\n"
       << "}\n\n"
       << "}\n\n"
       << "if(iter==this->iterMax){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : no convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
      os << "std::cout << *this << std::endl;\n";
    }
    os << "return false;\n"
       << "}\n\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
    }
    os << "return true;\n"
       << "}\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void IsotropicMisesCreepDSL::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Integrate behaviour law over the time step\n"
       << "*/\n"
       << "IntegrationResult\n"
       << "integrate(const SMFlag smflag,const SMType smt) override{\n"
       << "using namespace std;\n";
    if (this->mb.useQt()) {
      os << "if(smflag!=MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::STANDARDTANGENTOPERATOR){\n"
         << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
         << "}\n";
    } else {
      os << "if(smflag!=MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::STANDARDTANGENTOPERATOR){\n"
         << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
         << "}\n";
    }
    os << "if(!this->NewtonIntegration()){\n";
    if (this->mb.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::FAILURE;\n";
    }
    os << "}\n"
       << "if(smt!=NOSTIFFNESSREQUESTED){\n"
       << "if(!this->computeConsistentTangentOperator(smt)){\n";
    if (this->mb.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::FAILURE;\n";
    }
    os << "}\n"
       << "}\n"
       << "this->deel = this->deto-(this->dp)*(this->n);\n"
       << "this->updateStateVariables();\n"
       << "this->sig  = "
          "(this->lambda_tdt)*trace(this->eel)*StrainStensor::Id()+2*(this->mu_"
          "tdt)*(this->eel);\n"
       << "this->updateAuxiliaryStateVariables();\n";
    for (const auto& v : d.getPersistentVariables()) {
      this->writePhysicalBoundsChecks(os, v, false);
    }
    for (const auto& v : d.getPersistentVariables()) {
      this->writeBoundsChecks(os, v, false);
    }
    if (this->mb.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::SUCCESS;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::SUCCESS;\n";
    }
    os << "}\n\n";
  }

  void IsotropicMisesCreepDSL::writeBehaviourComputeTangentOperator(
      std::ostream& os, const Hypothesis) const {
    os << "bool computeConsistentTangentOperator(const SMType smt){\n"
       << "using namespace std;\n"
       << "using tfel::material::computeElasticStiffness;\n"
       << "using tfel::math::st2tost2;\n"
       << "if(smt==CONSISTENTTANGENTOPERATOR){\n"
       << "computeElasticStiffness<N, "
          "NumericType>::exe(this->Dt,this->lambda_tdt,this-"
          ">mu_tdt);\n"
       << "if(this->seq_e > real(0.01)*(this->young) *std::numeric_limits<"
          "real>::epsilon()){\n"
       << "constexpr auto M = st2tost2<N, NumericType>::M();\n"
       << "const auto ccto_tmp_1 =  this->dp/this->seq_e;\n"
       << "this->Dt += "
          "-4*(this->mu_tdt)*(this->mu)*(this->theta)*(ccto_tmp_1*M-(ccto_tmp_"
          "1-this->df_dseq*(this->dt)/"
          "(1+3*(this->mu)*(this->theta)*(this->dt)*this->df_dseq))*((this->n)^"
          "(this->n)));\n"
       << "}\n"
       << "} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
       << "computeElasticStiffness<N, "
          "NumericType>::exe(this->Dt,this->lambda_tdt,this-"
          ">mu_tdt);\n"
       << "} else {\n"
       << "return false;"
       << "}\n"
       << "return true;\n"
       << "}\n\n";
  }

  void IsotropicMisesCreepDSL::writeBehaviourParserSpecificInitializeMethodPart(
      std::ostream& os, const Hypothesis) const {
    this->checkBehaviourFile(os);
    os << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(this->theta)*"
          "(this->deto)));\n"
       << "this->seq_e = sigmaeq(this->se);\n"
       << "if(this->seq_e> real(0.01)*(this->young) * std::numeric_limits<"
          "real>::epsilon()){\n"
       << "this->n = 3*(this->se)/(2*this->seq_e);\n"
       << "} else {\n"
       << "this->n = StrainStensor(strain(0));\n"
       << "}\n";
  }  // end of writeBehaviourParserSpecificInitializeMethodPart

  IsotropicMisesCreepDSL::~IsotropicMisesCreepDSL() = default;

}  // end of namespace mfront
