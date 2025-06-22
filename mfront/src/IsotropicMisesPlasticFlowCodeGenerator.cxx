/*!
 * \file   mfront/src/IsotropicMisesPlasticFlowCodeGenerator.cxx
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
#include "MFront/IsotropicMisesPlasticFlowCodeGenerator.hxx"

namespace mfront {

  IsotropicMisesPlasticFlowCodeGenerator::
      IsotropicMisesPlasticFlowCodeGenerator(const FileDescription& f,
                                             const BehaviourDescription& d,
                                             const BehaviourInterfaceMap& bim)
      : IsotropicBehaviourCodeGeneratorBase(f, d, bim) {
  }  // end of IsotropicMisesPlasticFlowCodeGenerator

  void
  IsotropicMisesPlasticFlowCodeGenerator::writeBehaviourParserSpecificMembers(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    if (!this->bd.hasCode(h, BehaviourData::FlowRule)) {
      const auto msg = [&h]() -> std::string {
        if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          return "no flow rule defined.";
        }
        return "no flow rule defined for hypothesis "
               "'" +
               ModellingHypothesis::toString(h) + "'.";
      }();
      this->throwRuntimeError(
          "IsotropicBehaviourCodeGeneratorBase::"
          "endsInputFileProcessing: ",
          msg);
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
    os << this->bd.getCode(h, BehaviourData::FlowRule) << "return true;\n}\n\n";
    os << "bool NewtonIntegration(){\n"
       << "constexpr auto newton_epsilon = "
          "100*std::numeric_limits<strain>::epsilon();\n"
       << "const auto mfront_internal_3_mu_theta = 3*("
       << this->bd.getClassName() << "::theta)*(this->mu);\n"
       << "bool converge=false;\n"
       << "strain newton_f;\n"
       << "strain newton_df;\n"
       << "auto newton_ddp = strain{}\n; // previous correction of the Newton "
          "algorithm\n"
       << "real mfront_internal_surf;\n"
       << "unsigned int iter = 0u;\n"
       << "this->p_=this->p+this->dp;\n"
       << "while((converge==false) && (iter<this->iterMax)){\n"
       << "this->seq = "
          "std::max(this->seq_e-mfront_internal_3_mu_theta*(this->dp), "
          "stress(0));\n"
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
       << "newton_ddp = (this->seq_e / mfront_internal_3_mu_theta) / 2;\n"
       << "} // end of if(iter==0u)\n"
       << "this->dp += newton_ddp;\n"
       << "this->p_  = this->p + (this->theta)*(this->dp);\n"
       << "iter+=1;\n"
       << "} else {\n"
       << "mfront_internal_surf = (this->f)/(this->young);\n"
       << "if(((mfront_internal_surf>newton_epsilon)&&((this->dp)>=strain(0)))|"
          "|"
       << "   ((this->dp)>newton_epsilon)){\n"
       << "newton_f  = mfront_internal_surf;\n"
       << "newton_df = ((this->theta)*(this->df_dp)"
       << "-mfront_internal_3_mu_theta*(this->df_dseq))/(this->young);\n"
       << "} else {\n"
       << "newton_f  =(this->dp);\n"
       << "newton_df = real(1.);\n"
       << "}\n"
       << "if(tfel::math::abs(newton_df) > newton_epsilon){\n"
       << "newton_ddp = -newton_f/newton_df;\n"
       << "this->dp += newton_ddp;\n"
       << "this->p_  = this->p + (this->theta)*(this->dp);\n"
       << "iter+=1;\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \": \" << tfel::math::abs(newton_f) << '\\n';\n";
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
       << "newton_ddp = (this->seq_e / mfront_internal_3_mu_theta) / 2;\n"
       << "} // end of if(iter==0u)\n"
       << "this->dp += newton_ddp;\n"
       << "this->p_  = this->p + (this->theta)*(this->dp);\n"
       << "iter+=1;\n"
       << "}\n"
       << "}\n\n"
       << "}\n\n"
       << "if(iter==this->iterMax){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : no convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n"
         << "std::cout << *this << std::endl;\n";
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

  void IsotropicMisesPlasticFlowCodeGenerator::writeBehaviourIntegrator(
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
       << "if(this->seq_e > 100 * (this->young) * "
       << "std::numeric_limits<NumericType>::epsilon()){\n"
       << "this->n = 3 * (this->se)/(2 * (this->seq_e));\n"
       << "} else {\n"
       << "this->n = StrainStensor(strain(0));\n"
       << "}\n";
    os << "if(!this->NewtonIntegration()){\n";
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::FAILURE;\n";
    }
    os << "}\n"
       << "if(smt!=NOSTIFFNESSREQUESTED){\n"
       << "if(!this->computeConsistentTangentOperator(smt)){\n";
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::FAILURE;\n";
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
  }  // end of writeBehaviourIntegrator

  void
  IsotropicMisesPlasticFlowCodeGenerator::writeBehaviourComputeTangentOperator(
      std::ostream& os, const Hypothesis) const {
    os << "bool computeConsistentTangentOperator(const SMType smt){\n"
       << "using namespace std;\n"
       << "using tfel::material::computeElasticStiffness;\n"
       << "using tfel::math::st2tost2;\n"
       << "constexpr real prec = "
          "std::numeric_limits<strain>::epsilon()/100;\n"
       << "if(smt==CONSISTENTTANGENTOPERATOR){\n"
       << "computeElasticStiffness<N, "
          "NumericType>::exe(this->Dt,this->lambda_tdt,this-"
          ">mu_tdt);\n"
       << "if(this->dp>prec){\n"
       << "const auto mfront_internal_ccto_tmp_1 =  this->dp/this->seq_e;\n"
       << "const auto& M = st2tost2<N, NumericType>::M();\n"
       << "this->Dt += "
       << "-4*(this->mu_tdt)*(this->mu)*(this->theta)*"
       << "(mfront_internal_ccto_tmp_1 * M-"
       << "(mfront_internal_ccto_tmp_1-this->df_dseq/"
       << "((this->theta)*(3*(this->mu)*(this->df_dseq)-(this->df_dp))))*(("
       << "this->n)^(this->n)));\n"
       << "}\n"
       << "} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
       << "computeElasticStiffness<N, "
          "NumericType>::exe(this->Dt,this->lambda_tdt,this-"
          ">mu_tdt);\n"
       << "} else {\n"
       << "return false;\n"
       << "}\n"
       << "return true;\n"
       << "}\n\n";
  }  // end of writeBehaviourComputeTangentOperator

  IsotropicMisesPlasticFlowCodeGenerator::
      ~IsotropicMisesPlasticFlowCodeGenerator() = default;

}  // end of namespace mfront
