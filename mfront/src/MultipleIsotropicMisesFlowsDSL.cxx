/*!
 * \file   mfront/src/MultipleIsotropicMisesFlowsDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   31 jan 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MultipleIsotropicMisesFlowsDSL.hxx"

namespace mfront {

  MultipleIsotropicMisesFlowsDSL::FlowHandler::~FlowHandler() noexcept =
      default;

  std::string MultipleIsotropicMisesFlowsDSL::getName() {
    return "MultipleIsotropicMisesFlows";
  }

  std::string MultipleIsotropicMisesFlowsDSL::getDescription() {
    return "this parser is used to define behaviours combining several "
           "isotropic flows. Supported flow type are 'Creep' (dp/dt=f(s)) "
           "'StrainHardeningCreep' (dp/dt=f(s,p)) and 'Plasticity' (f(p,s)=0) "
           "where p is the equivalent plastic strain and s the equivalent "
           "mises stress";
  }  // end of getDescription

  MultipleIsotropicMisesFlowsDSL::MultipleIsotropicMisesFlowsDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("MultipleIsotropicMisesFlows");
    // Default state vars
    this->mb.addStateVariable(
        h, VariableDescription("StrainStensor", "εᵉˡ", "eel", 1u, 0u));
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->mb.setGlossaryName(h, "p", "EquivalentStrain");
    // default local vars
    this->reserveName("mu_3_theta");
    this->reserveName("surf");
    this->reserveName("jacobian_inversion_succeeded");
    this->mb.addLocalVariable(
        h, VariableDescription("StressStensor", "se", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("stress", "\u03C3\u2091", "seq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "seq_e", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StrainStensor", "n", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("strain", "p_", 1u, 0u));
  }

  BehaviourDSLDescription
  MultipleIsotropicMisesFlowsDSL::getBehaviourDSLDescription() const {
    auto d = IsotropicBehaviourDSLBase::getBehaviourDSLDescription();
    d.minimalMFrontFileBody = "@FlowRule Plasticity {}\n\n";
    return d;
  }  // end of getBehaviourDSLDescription

  void MultipleIsotropicMisesFlowsDSL::getSymbols(
      std::map<std::string, std::string>& symbols,
      const Hypothesis h,
      const std::string& n) {
    IsotropicBehaviourDSLBase::getSymbols(symbols, h, n);
    mfront::addSymbol(symbols, "\u2202f\u2215\u2202\u03C3\u2091", "df_dseq");
    mfront::addSymbol(symbols, "\u2202f\u2215\u2202p", "df_dp");
  }  // end of getSymbols

  std::string MultipleIsotropicMisesFlowsDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::FlowRule) {
      if (o.useUnicodeSymbols) {
        return "@FlowRule Plasticity {\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "\u2202f\u2215\u2202p            = ;\n"
               "}\n\n"
               "@FlowRule StrainHardeningCreep{\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "\u2202f\u2215\u2202p            = ;\n"
               "}\n\n"
               "@FlowRule Creep {\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "}";
      } else {
        return "@FlowRule Plasticity{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "df_dp   = ;\n"
               "}\n\n"
               "@FlowRule StrainHardeningCreep{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "df_dp   = ;\n"
               "}\n\n"
               "@FlowRule Creep {\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "}";
      }
    }
    return "";
  }  // end of getCodeBlockTemplate

  void MultipleIsotropicMisesFlowsDSL::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "#include\"TFEL/Math/TinyMatrixSolve.hxx\"\n\n";
  }

  void MultipleIsotropicMisesFlowsDSL::writeBehaviourParserSpecificMembers(
      std::ostream& os, const Hypothesis) const {
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
                   "MultipleIsotropicMisesFlowsDSL::"
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
      writeMaterialLaws(os, this->mb.getMaterialLaws());
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
        otheta << "mu_3_theta" << n;
        os << "stress " + otheta.str() + " = 3*(real(";
        os << p->theta << "))*(this->mu);\n";
      } else {
        genericTheta = true;
      }
    }

    if (genericTheta) {
      os << "stress mu_3_theta = 3*(";
      os << this->mb.getClassName() << "::theta)*(this->mu);\n";
    }
    os << "unsigned int iter=0u;\n"
       << "bool converge=false;\n"
       << "while((converge==false)&&\n"
       << "(iter<(" << this->mb.getClassName() << "::iterMax))){\n";
    n = 0;
    for (auto p = this->flows.begin(); p != this->flows.end(); ++p, ++n) {
      os << "this->p_  = this->p" << n << " + (";
      if (p->hasSpecificTheta) {
        os << "real(" << p->theta << ")";
      } else {
        os << this->mb.getClassName() << "::theta";
      }
      os << ") * strain(vdp(" << n << "));\n";
      if (p->hasSpecificTheta) {
        std::ostringstream otheta;
        os << "this->seq = std::max(this->seq_e" << n << "-";
        otheta << "mu_3_theta" << n << "*(";
        os << otheta.str();
      } else {
        os << "this->seq = std::max(this->seq_e-";
        os << "mu_3_theta*(";
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
                 << "-mu_3_theta" << n << "*(this->df_dseq" << n
                 << "))/(this->young);\n";
            } else {
              os << " = ((" << this->mb.getClassName()
                 << "::theta)*(this->df_dp" << n << ")"
                 << "-mu_3_theta*(this->df_dseq" << n << "))/(this->young);\n";
            }
          } else {
            os << "newton_df(" << n << "," << n2 << ")";
            if (p->hasSpecificTheta) {
              os << " = -mu_3_theta" << n << "*(this->df_dseq" << n
                 << ")/(this->young);\n";
            } else {
              os << " = -mu_3_theta*(this->df_dseq" << n
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
          os << "mu_3_theta" << n;
        } else {
          os << "mu_3_theta";
        }
        os << "*(this->df_dseq" << n << ")*(this->dt);\n";
        for (p2 = this->flows.begin(), n2 = 0; p2 != this->flows.end();
             ++p2, ++n2) {
          if (p2 != p) {
            os << "newton_df(" << n << "," << n2 << ") = ";
            if (p->hasSpecificTheta) {
              os << "mu_3_theta" << n;
            } else {
              os << "mu_3_theta";
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
          os << "(" << this->mb.getClassName() << "::theta)";
        }
        os << "*(this->df_dp" << n << ")-";
        if (p->hasSpecificTheta) {
          os << "mu_3_theta" << n;
        } else {
          os << "mu_3_theta";
        }
        os << "*(this->df_dseq" << n << "));\n";
        for (p2 = this->flows.begin(), n2 = 0; p2 != this->flows.end();
             ++p2, ++n2) {
          if (p2 != p) {
            os << "newton_df(" << n << "," << n2 << ") = ";
            if (p->hasSpecificTheta) {
              os << "mu_3_theta" << n;
            } else {
              os << "mu_3_theta";
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
    os << "auto jacobian_inversion_succeeded = true;"
       << "try{\n"
       << "tfel::math::TinyMatrixSolve<" << this->flows.size()
       << ", NumericType>::exe(newton_df,newton_f);\n"
       << "} catch(tfel::math::LUException&){\n"
       << "jacobian_inversion_succeeded = false;\n"
       << "}\n"
       << "if(jacobian_inversion_succeeded){\n"
       << "vdp -= newton_f;\n"
       << "} else {\n "
       << "vdp /= 2;\n"
       << "}\n"
       << "iter+=1;\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : iteration \" "
         << "<< iter << \" : \" << (error/(real(" << this->flows.size()
         << "))) << std::endl;\n";
    }
    os << "converge = ((error)/(real(" << this->flows.size() << "))<"
       << "(" << this->mb.getClassName() << "::epsilon));\n"
       << "}\n\n"
       << "if(iter==" << this->mb.getClassName() << "::iterMax){\n";
    if (getDebugMode()) {
      os << "std::cout << \"" << this->mb.getClassName()
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
      os << "std::cout << \"" << this->mb.getClassName()
         << "::NewtonIntegration() : convergence after \" "
         << "<< iter << \" iterations\\n\\n\";\n";
    }
    os << "return true;\n"
       << "\n}\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void MultipleIsotropicMisesFlowsDSL::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    unsigned short n;
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
    if (this->mb.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::FAILURE;\n";
    }
    os << "}\n"
       << "}\n"
       << "this->deel = this->deto-dp*(this->n);\n"
       << "this->updateStateVariables();\n"
       << "this->sig  = "
          "(this->lambda)*trace(this->eel)*StrainStensor::Id()+2*(this->mu)*("
          "this->eel);\n"
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

  void MultipleIsotropicMisesFlowsDSL::treatFlowRule() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    FlowHandler flow;
    this->checkNotEndOfFile("MultipleIsotropicMisesFlowsDSL::treatFlowRule",
                            "Expected flow rule name.");
    if (this->current->value == "Plasticity") {
      std::ostringstream p;
      std::ostringstream f;
      std::ostringstream df_dseq;
      std::ostringstream df_dp;
      p << "p" << this->flows.size();
      f << "f" << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      df_dp << "df_dp" << this->flows.size();
      this->mb.addStateVariable(h,
                                VariableDescription("strain", p.str(), 1u, 0u),
                                BehaviourData::FORCEREGISTRATION);
      this->mb.addLocalVariable(h,
                                VariableDescription("stress", f.str(), 1u, 0u));
      this->mb.addLocalVariable(
          h, VariableDescription("real", df_dseq.str(), 1u, 0u));
      this->mb.addLocalVariable(
          h, VariableDescription("stress", df_dp.str(), 1u, 0u));
      flow.flow = FlowHandler::PlasticFlow;
    } else if (this->current->value == "Creep") {
      std::ostringstream p;
      std::ostringstream f;
      std::ostringstream df_dseq;
      p << "p" << this->flows.size();
      f << "f" << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      this->mb.addStateVariable(h,
                                VariableDescription("strain", p.str(), 1u, 0u),
                                BehaviourData::FORCEREGISTRATION);
      this->mb.addLocalVariable(
          h, VariableDescription("strainrate", f.str(), 1u, 0u));
      this->mb.addLocalVariable(
          h,
          VariableDescription("tfel::math::derivative_type<strainrate,stress>",
                              df_dseq.str(), 1u, 0u));
      flow.flow = FlowHandler::CreepFlow;
    } else if (this->current->value == "StrainHardeningCreep") {
      std::ostringstream p;
      std::ostringstream f;
      std::ostringstream df_dseq;
      std::ostringstream df_dp;
      p << "p" << this->flows.size();
      f << "f" << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      df_dp << "df_dp" << this->flows.size();
      this->mb.addStateVariable(h,
                                VariableDescription("strain", p.str(), 1u, 0u),
                                BehaviourData::FORCEREGISTRATION);
      this->mb.addLocalVariable(
          h, VariableDescription("strainrate", f.str(), 1u, 0u));
      this->mb.addLocalVariable(
          h,
          VariableDescription("tfel::math::derivative_type<strainrate,stress>",
                              df_dseq.str(), 1u, 0u));
      this->mb.addLocalVariable(
          h, VariableDescription("strainrate", df_dp.str(), 1u, 0u));
      flow.flow = FlowHandler::StrainHardeningCreepFlow;
    } else {
      this->throwRuntimeError("MultipleIsotropicMisesFlowsDSL::treatFlowRule",
                              "Unknown flow rule (read '" +
                                  this->current->value +
                                  "').Valid flow rules are 'Plasticity', "
                                  "'Creep' and 'StrainHardeningCreep'.");
    }
    ++(this->current);
    this->checkNotEndOfFile(
        "MultipleIsotropicMisesFlowsDSL::treatFlowRule",
        "Expected the beginning of a block or a specific theta value.");
    if (this->current->value != "{") {
      std::istringstream converter(this->current->value);
      std::ostringstream otheta;
      std::ostringstream ose;
      std::ostringstream oseq_e;
      flow.hasSpecificTheta = true;
      converter >> flow.theta;
      if (!converter || (!converter.eof())) {
        this->throwRuntimeError("MultipleIsotropicMisesFlowsDSL::treatFlowRule",
                                "Could not read theta value (read '" +
                                    this->current->value + "').");
      }
      otheta << "mu_3_theta" << this->flows.size();
      ose << "se" << this->flows.size();
      oseq_e << "seq_e" << this->flows.size();
      this->reserveName(ose.str());
      this->mb.addLocalVariable(
          h, VariableDescription("stress", oseq_e.str(), 1u, 0u));
      ++(this->current);
    } else {
      flow.hasSpecificTheta = false;
    }
    std::ostringstream cname;
    cname << BehaviourData::FlowRule << flows.size() << '\n';
    this->treatCodeBlock(
        *this, cname.str(),
        &MultipleIsotropicMisesFlowsDSL::flowRuleVariableModifier, true, false);
    flow.flowRule =
        this->mb.getCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS, cname.str());
    this->flows.push_back(flow);
  }  // end of treatFlowRule

  void MultipleIsotropicMisesFlowsDSL::
      writeBehaviourParserSpecificInitializeMethodPart(std::ostream& os,
                                                       const Hypothesis) const {
    this->checkBehaviourFile(os);
    os << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+("
       << this->mb.getClassName() << "::theta)*(this->deto)));\n"
       << "this->seq_e = sigmaeq(this->se);\n";
    unsigned short n = 0;
    for (const auto& f : this->flows) {
      if (f.hasSpecificTheta) {
        os << "StressStensor se" << n
           << "=2*(this->mu)*(tfel::math::deviator(this->eel+(";
        os << f.theta << ")*(this->deto)));\n";
        os << "this->seq_e" << n << " = sigmaeq(se" << n << ");\n";
      }
      ++n;
    }
    os << "if(this->seq_e > 100 * (this->young) * "
       << "std::numeric_limits<NumericType>::epsilon()){\n"
       << "this->n = 3 * (this->se) / (2 * this->seq_e);\n"
       << "} else {\n"
       << "this->n = StrainStensor(strain(0));\n"
       << "}\n";
  }  // end of writeBehaviourParserSpecificInitializeMethodPart

  void MultipleIsotropicMisesFlowsDSL::writeBehaviourComputeTangentOperator(
      std::ostream& os, const Hypothesis) const {
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

  MultipleIsotropicMisesFlowsDSL::~MultipleIsotropicMisesFlowsDSL() = default;

}  // end of namespace mfront
