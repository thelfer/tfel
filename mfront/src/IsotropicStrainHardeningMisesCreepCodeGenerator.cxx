/*!
 * \file   mfront/src/IsotropicStrainHardeningMisesCreepDSLCodeGenerator.cxx
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
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/IsotropicBehaviourDSLBase.hxx"

#include "MFront/IsotropicStrainHardeningMisesCreepCodeGenerator.hxx"

namespace mfront {

  IsotropicStrainHardeningMisesCreepCodeGenerator::
      IsotropicStrainHardeningMisesCreepCodeGenerator(
          const FileDescription& f,
          const BehaviourDescription& d,
          const BehaviourInterfaceMap& bim)
      : IsotropicBehaviourCodeGeneratorBase(f, d, bim) {
  }  // end of IsotropicStrainHardeningMisesCreepCodeGenerator

  void IsotropicStrainHardeningMisesCreepCodeGenerator::
      writeBehaviourParserSpecificMembers(std::ostream& os,
                                          const Hypothesis h) const {
    this->checkBehaviourFile(os);
    if (!this->bd.hasCode(h, BehaviourData::FlowRule)) {
      this->throwRuntimeError(
          "IsotropicStrainHardeningMisesCreepCodeGenerator::"
          "writeBehaviourParserSpecificMembers",
          "no flow rule declared "
          "(use the @FlowRule directive)");
    }
    os << "bool computeFlow(){\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "using namespace tfel::material;\n"
       << "using std::vector;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    if (this->bd.hasCode(h, BehaviourData::BeforeFlowRule)) {
      os << this->bd.getCode(h, BehaviourData::BeforeFlowRule);
    }
    os << this->bd.getCode(h, BehaviourData::FlowRule) << "return true;\n"
       << "}\n\n"
       << "bool NewtonIntegration(){\n"
       << "bool converge=false;\n"
       << "strain newton_f;\n"
       << "strain newton_df;\n"
       << "auto newton_ddp = strain{}; // previous correction of the Newton "
          "algorithm\n"
       << "real newton_epsilon = 100*std::numeric_limits<real>::epsilon();\n"
       << "stress mfront_internal_3_mu = 3*(this->mu);\n"
       << ""
       << "unsigned int iter = 0u;\n"
       << "this->p_=this->p+this->dp;\n"
       << "while((converge==false)&&\n"
       << "(iter<(this->iterMax))){\n"
       << "this->seq=std::max(this->seq_e-mfront_internal_3_mu*(this->theta)*("
          "this->dp), "
          "stress(0."
          "f));\n"
       << "const auto compute_flow_r = this->computeFlow();\n"
       << "if(!((compute_flow_r)&&\n"
       << "(tfel::math::ieee754::isfinite(this->f))&&\n"
       << "(tfel::math::ieee754::isfinite(this->df_dp))&&\n"
       << "(tfel::math::ieee754::isfinite(this->df_dseq)))){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": invalid evaluation of the yield surface or one of "
         << "its derivatives\\n\";\n";
    }
    os << "if(iter==0u){\n"
       << "// probably an elastic prediction\n"
       << "newton_ddp = (this->seq_e / (mfront_internal_3_mu * (this->theta))) "
          "/ 2;\n"
       << "} // end of if(iter==0u)\n"
       << "this->dp += newton_ddp;\n"
       << "this->p_  = this->p + (this->theta)*(this->dp);\n"
       << "iter+=1;\n"
       << "} else {\n"
       << "newton_f  = this->dp - (this->f)*(this->dt);\n"
       << "newton_df = "
          "1-(this->theta)*(this->dt)*((this->df_dp)-mfront_internal_3_mu*("
          "this->df_dseq));\n"
       << "if(tfel::math::abs(newton_df)"
       << ">newton_epsilon){\n"
       << "newton_ddp = -newton_f/newton_df;\n"
       << "this->dp += newton_ddp;\n"
       << "this->p_=this->p + (this->theta)*(this->dp);\n"
       << "iter+=1;\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \" : \" << tfel::math::abs(newton_f) << std::endl;\n";
    }
    os << "converge = tfel::math::abs(newton_f) < this->epsilon;\n"
       << "} else {\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": jacobian is singular\\n\";\n";
    }
    os << "if(iter==0u){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": invalid jacobian on the first iteration\\n\";\n";
    }
    os << "// probably an elastic prediction\n"
       << "newton_ddp = (this->seq_e / (mfront_internal_3_mu * (this->theta))) "
          "/ 2;\n"
       << "} // end of if(iter==0u)\n"
       << "this->dp += newton_ddp;\n"
       << "this->p_  = this->p + (this->theta)*(this->dp);\n"
       << "iter+=1;\n"
       << "}\n"
       << "}\n"
       << "}\n"
       << "if(iter==this->iterMax){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : no convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
      os << "std::cout << *this << std::endl;\n";
    }
    os << "return false;\n"
       << "}\n\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
    }
    os << "return true;\n"
       << "}\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void
  IsotropicStrainHardeningMisesCreepCodeGenerator::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& d = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Integrate behaviour law over the time step\n"
       << "*/\n"
       << "[[nodiscard]] IntegrationResult\n"
       << "integrate(const SMFlag smflag,const SMType smt) override{\n"
       << "using namespace std;\n";
    if (this->bd.useQt()) {
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
    this->writeBehaviourIntegratorPreprocessingStep(os);
    os << "this->seq_e = sigmaeq(this->se);\n"
       << "if(this->seq_e> 0.01 * (this->young) * "
       << "std::numeric_limits<NumericType>::epsilon()){\n"
       << "this->n = 3 * (this->se)/(2 * this->seq_e);\n"
       << "} else {\n"
       << "this->n = StrainStensor(strain(0));\n"
       << "}\n";
    os << "if(!this->NewtonIntegration()){\n";
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, false>::FAILURE;\n";
    }
    os << "}\n"
       << "if(smt!=NOSTIFFNESSREQUESTED){\n"
       << "if(!this->computeConsistentTangentOperator(smt)){\n";
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, false>::FAILURE;\n";
    }
    os << "}\n"
       << "}\n";
    this->writeBehaviourIntegratorPostprocessingStep(os);
    if (!areRuntimeChecksDisabled(this->bd)) {
      for (const auto& v : d.getPersistentVariables()) {
        this->writePhysicalBoundsChecks(os, v, false);
      }
      for (const auto& v : d.getPersistentVariables()) {
        this->writeBoundsChecks(os, v, false);
      }
    }
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::SUCCESS;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::SUCCESS;\n";
    }
    os << "}\n\n";
  }

  void IsotropicStrainHardeningMisesCreepCodeGenerator::
      writeBehaviourComputeTangentOperator(std::ostream& os,
                                           const Hypothesis) const {
    os << "bool computeConsistentTangentOperator(const SMType smt){\n"
       << "using tfel::material::computeElasticStiffness;\n"
       << "using tfel::math::st2tost2;\n"
       << "if(smt==CONSISTENTTANGENTOPERATOR){\n"
       << "computeElasticStiffness<N, "
          "NumericType>::exe(this->Dt,this->lambda_tdt,this-"
          ">mu_tdt);\n"
       << "if(this->seq_e>(0.01*(this->young))*std::numeric_limits<NumericType>"
          "::"
          "epsilon()){\n"
       << "const auto mfront_internal_ccto_tmp_1 =  this->dp/this->seq_e;\n"
       << "const auto& M = st2tost2<N, NumericType>::M();\n"
       << "this->Dt += "
       << "-4*(this->mu_tdt)*(this->mu)*(this->theta)*"
       << "(mfront_internal_ccto_tmp_1*M-(mfront_internal_ccto_tmp_1-this->df_"
          "dseq*(this->dt)/"
          "(1+(this->theta)*(this->dt)*(3*(this->mu)*this->df_dseq-"
          "(this->df_dp))))*((this->n)^(this->n)));\n"
       << "}\n"
       << "} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
       << "computeElasticStiffness<N, "
          "stress>::exe(this->Dt,this->lambda_tdt,this-"
          ">mu_tdt);\n"
       << "} else {\n"
       << "return false;"
       << "}\n"
       << "return true;\n"
       << "}\n\n";
  }

  IsotropicStrainHardeningMisesCreepCodeGenerator::
      ~IsotropicStrainHardeningMisesCreepCodeGenerator() = default;

}  // end of namespace mfront
