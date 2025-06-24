/*!
 * \file   mfront/src/MultipleIsotropicMisesFlowsCodeGenerator.cxx
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
#include "MFront/MultipleIsotropicMisesFlowsCodeGenerator.hxx"

namespace mfront {

  MultipleIsotropicMisesFlowsCodeGenerator::
      MultipleIsotropicMisesFlowsCodeGenerator(
          const FileDescription& f,
          const BehaviourDescription& d,
          const BehaviourInterfaceMap& bim,
          const std::vector<FlowHandler>& fhs)
      : IsotropicBehaviourCodeGeneratorBase(f, d, bim),
        flows(fhs) {}  // end of MultipleIsotropicMisesFlowsCodeGenerator

  void MultipleIsotropicMisesFlowsCodeGenerator::
      writeBehaviourParserSpecificIncludes(std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "#include\"TFEL/Math/TinyMatrixSolve.hxx\"\n\n";
  }

  void
  MultipleIsotropicMisesFlowsCodeGenerator::writeBehaviourParserSpecificMembers(
      std::ostream& os, const Hypothesis h) const {
    const auto has_plastic_flow = [this] {
      for (auto p = this->flows.begin(); p != this->flows.end(); ++p) {
        if (p->flow == FlowHandler::PlasticFlow) {
          return true;
        }
      }
      return false;
    }();
    bool genericTheta;
    this->checkBehaviourFile(os);
    tfel::raise_if(this->flows.empty(),
                   "MultipleIsotropicMisesFlowsCodeGenerator::"
                   "writeBehaviourParserSpecificMembers : "
                   "no flow rule defined");
    unsigned short n = 0;
    for (auto p = this->flows.begin(); p != this->flows.end(); ++p, ++n) {
      if (p->flow == FlowHandler::PlasticFlow) {
        os << "void computeFlow" << n << "(stress& f,\n"
           << "real& df_dseq,\n"
           << "stress& df_dp){\n";
      } else if (p->flow == FlowHandler::CreepFlow) {
        os << "void computeFlow" << n << "(strainrate& f,\n"
           << "tfel::math::derivative_type<strainrate,stress>& df_dseq){\n";
      } else if (p->flow == FlowHandler::StrainHardeningCreepFlow) {
        os << "void computeFlow" << n << "(strainrate& f,\n"
           << "tfel::math::derivative_type<strainrate,stress>& df_dseq,\n"
           << "strainrate& df_dp){\n";
      }
      os << "using namespace std;\n";
      os << "using namespace tfel::math;\n";
      os << "using namespace tfel::material;\n";
      os << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      const auto flow_id = std::to_string(n);
      if (this->bd.hasCode(h, BehaviourData::BeforeFlowRule + flow_id)) {
        os << this->bd.getCode(h, BehaviourData::BeforeFlowRule + flow_id);
      }
      os << p->flowRule << '\n';
      os << "}\n\n";
    }
    os << "bool NewtonIntegration(){\n";
    if (has_plastic_flow) {
      os << "constexpr auto newton_epsilon = "
            "100 * std::numeric_limits<NumericType>::epsilon();\n";
    }
    os << "tfel::math::tvector<" << this->flows.size()
       << ", NumericType> vdp(NumericType(0));\n"
       << "tfel::math::tvector<" << this->flows.size()
       << ", NumericType> newton_f;\n"
       << "tfel::math::tmatrix<" << this->flows.size() << ","
       << this->flows.size() << ", NumericType> newton_df;\n";

    genericTheta = false;
    n = 0;
    for (auto p = this->flows.begin(); p != this->flows.end(); ++p, ++n) {
      if (p->hasSpecificTheta) {
        std::ostringstream otheta;
        otheta << "mfront_internal_3_mu_theta" << n;
        os << "stress " + otheta.str() + " = 3*(real(";
        os << p->theta << "))*(this->mu);\n";
      } else {
        genericTheta = true;
      }
    }

    if (genericTheta) {
      os << "stress mfront_internal_3_mu_theta = 3*(";
      os << this->bd.getClassName() << "::theta)*(this->mu);\n";
    }
    os << "unsigned int iter=0u;\n"
       << "bool converge=false;\n"
       << "while((converge==false)&&\n"
       << "(iter<(" << this->bd.getClassName() << "::iterMax))){\n";
    n = 0;
    for (auto p = this->flows.begin(); p != this->flows.end(); ++p, ++n) {
      os << "this->p_  = this->p" << n << " + (";
      if (p->hasSpecificTheta) {
        os << "real(" << p->theta << ")";
      } else {
        os << this->bd.getClassName() << "::theta";
      }
      os << ") * strain(vdp(" << n << "));\n";
      if (p->hasSpecificTheta) {
        std::ostringstream otheta;
        os << "this->seq = std::max(this->seq_e" << n << "-";
        otheta << "mfront_internal_3_mu_theta" << n << "*(";
        os << otheta.str();
      } else {
        os << "this->seq = std::max(this->seq_e-";
        os << "mfront_internal_3_mu_theta*(";
      }
      auto p2 = this->flows.begin();
      unsigned short n2 = 0u;
      while (p2 != this->flows.end()) {
        os << "strain(vdp(" << n2 << "))";
        ++p2;
        ++n2;
        if (p2 != this->flows.end()) {
          os << "+";
        }
      }
      os << "), stress(0.f));\n";
      if (p->flow == FlowHandler::PlasticFlow) {
        os << "this->computeFlow" << n << "("
           << "this->f" << n << ","
           << "this->df_dseq" << n << ","
           << "this->df_dp" << n << ");\n";
        os << "const auto surf = (this->f" << n << ")/(this->young);\n";
        os << "if(((surf > strain(newton_epsilon)) && ((vdp(" << n
           << "))>=NumericType(0)))||"
           << "((vdp(" << n << ")) > newton_epsilon)){";
        os << "newton_f(" << n << ")  = surf;\n";
        for (p2 = this->flows.begin(), n2 = 0; p2 != this->flows.end();
             ++p2, ++n2) {
          if (p2 == p) {
            os << "newton_df(" << n << "," << n << ")";
            if (p->hasSpecificTheta) {
              os << " = ((real(" << p->theta << "))*(this->df_dp" << n << ")"
                 << "-mfront_internal_3_mu_theta" << n << "*(this->df_dseq" << n
                 << "))/(this->young);\n";
            } else {
              os << " = ((" << this->bd.getClassName()
                 << "::theta)*(this->df_dp" << n << ")"
                 << "-mfront_internal_3_mu_theta*(this->df_dseq" << n
                 << "))/(this->young);\n";
            }
          } else {
            os << "newton_df(" << n << "," << n2 << ")";
            if (p->hasSpecificTheta) {
              os << " = -mfront_internal_3_mu_theta" << n << "*(this->df_dseq"
                 << n << ")/(this->young);\n";
            } else {
              os << " = -mfront_internal_3_mu_theta*(this->df_dseq" << n
                 << ")/(this->young);\n";
            }
          }
        }
        os << "} else {\n";
        os << "newton_f(" << n << ")  =(vdp(" << n << "));\n";
        os << "newton_df(" << n << "," << n << ") = real(1.);\n";
        for (p2 = this->flows.begin(), n2 = 0; p2 != this->flows.end();
             ++p2, ++n2) {
          if (p2 != p) {
            os << "newton_df(" << n << "," << n2 << ") = real(0.);\n";
          }
        }
        os << "}\n";
      } else if (p->flow == FlowHandler::CreepFlow) {
        os << "this->computeFlow" << n << "("
           << "this->f" << n << ","
           << "this->df_dseq" << n << ""
           << ");\n";
        os << "newton_f(" << n << ")  = vdp(" << n << ") - (this->f" << n
           << ")*(this->dt);\n";
        os << "newton_df(" << n << "," << n << ") = 1+";
        if (p->hasSpecificTheta) {
          os << "mfront_internal_3_mu_theta" << n;
        } else {
          os << "mfront_internal_3_mu_theta";
        }
        os << "*(this->df_dseq" << n << ")*(this->dt);\n";
        for (p2 = this->flows.begin(), n2 = 0; p2 != this->flows.end();
             ++p2, ++n2) {
          if (p2 != p) {
            os << "newton_df(" << n << "," << n2 << ") = ";
            if (p->hasSpecificTheta) {
              os << "mfront_internal_3_mu_theta" << n;
            } else {
              os << "mfront_internal_3_mu_theta";
            }
            os << "*(this->df_dseq" << n << ")*(this->dt);\n";
          }
        }
      } else {
        os << "this->computeFlow" << n << "("
           << "this->f" << n << ","
           << "this->df_dseq" << n << ","
           << "this->df_dp" << n << ");\n";
        os << "newton_f(" << n << ")  = vdp(" << n << ") - (this->f" << n
           << ") * (this->dt);\n";
        os << "newton_df(" << n << "," << n << ") = 1-(this->dt)*(";
        if (p->hasSpecificTheta) {
          os << "(real(" << p->theta << "))";
        } else {
          os << "(" << this->bd.getClassName() << "::theta)";
        }
        os << "*(this->df_dp" << n << ")-";
        if (p->hasSpecificTheta) {
          os << "mfront_internal_3_mu_theta" << n;
        } else {
          os << "mfront_internal_3_mu_theta";
        }
        os << "*(this->df_dseq" << n << "));\n";
        for (p2 = this->flows.begin(), n2 = 0; p2 != this->flows.end();
             ++p2, ++n2) {
          if (p2 != p) {
            os << "newton_df(" << n << "," << n2 << ") = ";
            if (p->hasSpecificTheta) {
              os << "mfront_internal_3_mu_theta" << n;
            } else {
              os << "mfront_internal_3_mu_theta";
            }
            os << "*(this->df_dseq" << n << ")*(this->dt);\n";
          }
        }
      }
    }
    os << "auto error = NumericType(0);\n";
    n = 0;
    for (auto p = this->flows.begin(); p != this->flows.end(); ++p, ++n) {
      os << "error += tfel::math::abs(newton_f(" << n << "));\n";
    }
    os << "auto jacobian_inversion_succeeded = "
       << "tfel::math::TinyMatrixSolve<" << this->flows.size()
       << ", NumericType, false>::exe(newton_df,newton_f);\n"
       << "if(jacobian_inversion_succeeded){\n"
       << "vdp -= newton_f;\n"
       << "} else {\n "
       << "vdp /= 2;\n"
       << "}\n"
       << "iter+=1;\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \" : \" << (error/(real(" << this->flows.size()
         << "))) << std::endl;\n";
    }
    os << "converge = ((error)/(real(" << this->flows.size() << "))<"
       << "(" << this->bd.getClassName() << "::epsilon));\n"
       << "}\n\n"
       << "if(iter==" << this->bd.getClassName() << "::iterMax){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : no convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
      os << "std::cout << *this << std::endl;\n";
    }
    os << "return false;\n"
       << "}\n\n";
    n = 0;
    for (auto p = this->flows.begin(); p != this->flows.end(); ++p, ++n) {
      os << "this->dp" << n << " = "
         << "vdp(" << n << ");\n";
    }
    if (getDebugMode()) {
      os << "std::cout << \"" << this->bd.getClassName()
         << "::NewtonIntegration() : convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
    }
    os << "return true;\n"
       << "\n}\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void MultipleIsotropicMisesFlowsCodeGenerator::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& d = this->bd.getBehaviourData(h);
    unsigned short n;
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Integrate behaviour law over the time step\n"
       << "*/\n"
       << "[[nodiscard]] IntegrationResult\n"
       << "integrate(const SMFlag smflag,const SMType smt) override final{\n"
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
    if (this->bd.hasCode(h, BehaviourData::ComputePredictor)) {
      os << this->bd.getCode(h, BehaviourData::ComputePredictor) << '\n';
    }
    this->writeBehaviourIntegratorPreprocessingStep(os);
    os << "this->seq_e = sigmaeq(this->se);\n";
    n = 0;
    for (const auto& f : this->flows) {
      if (f.hasSpecificTheta) {
        if (this->bd.getAttribute(
                IsotropicBehaviourDSLBase::useStressUpdateAlgorithm, false)) {
          if (this->shallComputeTheElasticStrain()) {
            os << "const StressStensor mfront_se" << n << " = "
               << "2 * (this->mu) * (tfel::math::deviator(mfront_eel_bts + ("
               << f.theta << ") * (this->deto)));\n";
          } else {
            os << "const StressStensor mfront_se" << n << " = "
               << "tfel::math::deviator(this->sig) + "
               << "2 * (this->mu) * (" << f.theta << ") * (this->deto);\n";
          }
        } else {
          os << "const StressStensor mfront_se" << n << " = "
             << "2 * (this->mu) * (tfel::math::deviator(this->eel + ("
             << f.theta << ") * (this->deto)));\n";
        }
        os << "this->seq_e" << n << " = sigmaeq(mfront_se" << n << ");\n";
      }
      ++n;
    }
    os << "if(this->seq_e > 100 * (this->young) * "
       << "std::numeric_limits<NumericType>::epsilon()){\n"
       << "this->n = 3 * (this->se) / (2 * this->seq_e);\n"
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
       << "this->dp = ";
    auto p2 = this->flows.begin();
    n = 0;
    while (p2 != this->flows.end()) {
      os << "this->dp" << n << "";
      ++n;
      ++p2;
      if (p2 != this->flows.end()) {
        os << "+";
      }
    }
    os << ";\n"
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
  }

  void MultipleIsotropicMisesFlowsCodeGenerator::
      writeBehaviourComputeTangentOperator(std::ostream& os,
                                           const Hypothesis) const {
    os << "bool computeConsistentTangentOperator(const SMType smt){\n"
       << "using namespace std;\n"
       << "using tfel::material::computeElasticStiffness;\n"
       << "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
       << "computeElasticStiffness<N, "
          "NumericType>::exe(this->Dt,this->lambda,this->mu)"
          ";\n"
       << "return true;\n"
       << "}\n"
       << "return false;\n"
       << "}\n\n";
  }

  MultipleIsotropicMisesFlowsCodeGenerator::
      ~MultipleIsotropicMisesFlowsCodeGenerator() = default;

}  // end of namespace mfront
