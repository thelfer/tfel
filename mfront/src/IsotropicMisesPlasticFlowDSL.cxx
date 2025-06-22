/*!
 * \file   mfront/src/IsotropicMisesPlasticFlowDSL.cxx
 * \brief
 * \author Helfer Thomas
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
#include "MFront/IsotropicMisesPlasticFlowDSL.hxx"

namespace mfront {

  IsotropicMisesPlasticFlowDSL::IsotropicMisesPlasticFlowDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("IsotropicMisesPlasticFlow");
    // Default state vars
    this->mb.addStateVariable(
        h, VariableDescription("StrainStensor", "eel", 1u, 0u));
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->mb.setGlossaryName(h, "p", "EquivalentPlasticStrain");
    // default local vars
    this->reserveName("mu_3_theta");
    this->reserveName("surf");
    this->mb.addLocalVariable(h, VariableDescription("stress", "f", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("real", "df_dseq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "df_dp", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StressStensor", "se", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("stress", "seq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "seq_e", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StrainStensor", "n", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("strain", "p_", 1u, 0u));
    this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator, true);
    this->mb.setAttribute(
        h, BehaviourData::isConsistentTangentOperatorSymmetric, true);
    this->theta = 1.;
  }

  std::string IsotropicMisesPlasticFlowDSL::getName() {
    return "IsotropicPlasticMisesFlow";
  }

  std::string IsotropicMisesPlasticFlowDSL::getDescription() {
    return "this parser is used for standard plastics behaviours with yield "
           "surface"
           " of the form f(s,p)=0 where p is the equivalent creep strain and s "
           "the "
           "equivalent mises stress";
  }  // end of IsotropicMisesPlasticFlowDSL::getDescription

  void IsotropicMisesPlasticFlowDSL::endsInputFileProcessing() {
    IsotropicBehaviourDSLBase::endsInputFileProcessing();
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
      throw(
          std::runtime_error("IsotropicMisesCreepDSL::"
                             "endsInputFileProcessing: "
                             "no flow rule defined"));
    }
  }  // end of IsotropicMisesPlasticFlowDSL::endsInputFileProcessing

  void IsotropicMisesPlasticFlowDSL::writeBehaviourParserSpecificMembers(
      const Hypothesis h) {
    using namespace std;
    this->checkBehaviourFile();
    if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
      string msg("IsotropicBehaviourDSLBase::endsInputFileProcessing : ");
      msg += "no flow rule defined for hypothesis '" +
             ModellingHypothesis::toString(h) + "'.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "void computeFlow(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using namespace tfel::material;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws(
        "IsotropicMisesPlasticFlowDSL::writeBehaviourParserSpecificMembers",
        this->behaviourFile, this->mb.getMaterialLaws());
    this->behaviourFile << this->mb.getCode(h, BehaviourData::FlowRule) << endl;
    this->behaviourFile << "}\n\n";

    this->behaviourFile << "bool NewtonIntegration(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "bool converge=false;\n";
    this->behaviourFile << "bool inversible=true;\n";
    this->behaviourFile << "strain newton_f;\n";
    this->behaviourFile << "strain newton_df;\n";
    this->behaviourFile
        << "real newton_epsilon = 100*std::numeric_limits<real>::epsilon();\n";
    this->behaviourFile << "stress mu_3_theta = 3*(";
    this->behaviourFile << this->mb.getClassName() << "::theta)*(this->mu);\n";
    this->behaviourFile << "real surf;";
    this->behaviourFile << "\n";
    this->behaviourFile << "unsigned int iter = 0u;\n";
    this->behaviourFile << "this->p_=this->p+this->dp;\n";
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(iter<this->iterMax)&&\n";
    this->behaviourFile << "(inversible==true)){\n";
    this->behaviourFile
        << "this->seq = "
           "std::max(this->seq_e-mu_3_theta*(this->dp),real(0.f));\n";
    this->behaviourFile << "this->computeFlow();\n";
    this->behaviourFile << "surf = (this->f)/(this->young);\n";
    this->behaviourFile << "if(((surf>newton_epsilon)&&((this->dp)>=0))||"
                        << "((this->dp)>newton_epsilon)){";
    this->behaviourFile << "newton_f  = surf;\n";
    this->behaviourFile << "newton_df = ((this->theta)*(this->df_dp)"
                        << "-mu_3_theta*(this->df_dseq))/(this->young);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "newton_f  =(this->dp);\n";
    this->behaviourFile << "newton_df = real(1.);\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(std::abs(base_cast(newton_df))";
    this->behaviourFile << ">newton_epsilon){\n";
    this->behaviourFile << "this->dp -= newton_f/newton_df;\n";
    this->behaviourFile << "this->p_  = this->p + (this->theta)*(this->dp);\n";
    this->behaviourFile << "iter+=1;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::NewtonIntegration() : iteration \" "
          << "<< iter << \" : \" << std::abs(tfel::math::base_cast(newton_f)) "
             "<< endl;\n";
    }
    this->behaviourFile
        << "converge = (std::abs(tfel::math::base_cast(newton_f))<";
    this->behaviourFile << "this->epsilon);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "inversible=false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(inversible==false){\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(iter==this->iterMax){\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::NewtonIntegration() : no convergence after \" "
                          << "<< iter << \" iterations\"<< endl << endl;\n";
      this->behaviourFile << "cout << *this << endl;\n";
    }
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::NewtonIntegration() : convergence after \" "
                          << "<< iter << \" iterations\"<< endl << endl;\n";
    }
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void IsotropicMisesPlasticFlowDSL::writeBehaviourIntegrator(
      const Hypothesis h) {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile
        << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult\n";
    this->behaviourFile
        << "integrate(const SMFlag smflag,const SMType smt) override{\n";
    this->behaviourFile << "using namespace std;\n";
    if (this->mb.useQt()) {
      this->behaviourFile
          << "if(smflag!=MechanicalBehaviour<" << btype
          << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n"
          << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
          << "}\n";
    } else {
      this->behaviourFile
          << "if(smflag!=MechanicalBehaviour<" << btype
          << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n"
          << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
          << "}\n";
    }
    this->behaviourFile << "if(!this->NewtonIntegration()){\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    this->behaviourFile
        << "if(!this->computeConsistentTangentOperator(smt)){\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->deel = this->deto-(this->dp)*(this->n);\n";
    this->behaviourFile << "this->updateStateVariables();\n";
    this->behaviourFile << "this->sig  = "
                           "(this->lambda)*trace(this->eel)*StrainStensor::Id()"
                           "+2*(this->mu)*(this->eel);\n";
    this->behaviourFile << "this->updateAuxiliaryStateVariables();\n";
    for (const auto& elem : d.getBounds()) {
      if (elem.varCategory == BoundsDescription::StateVariable) {
        elem.writeBoundsChecks(this->behaviourFile);
      }
    }
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void IsotropicMisesPlasticFlowDSL::writeBehaviourComputeTangentOperator(
      const Hypothesis) {
    this->behaviourFile
        << "bool computeConsistentTangentOperator(const SMType smt){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using tfel::material::computeElasticStiffness;\n";
    this->behaviourFile << "using tfel::math::st2tost2;\n";
    this->behaviourFile << "TFEL_CONSTEXPR real prec = "
                           "std::numeric_limits<strain>::epsilon()/100;\n";
    this->behaviourFile << "if(smt==CONSISTENTTANGENTOPERATOR){\n";
    this->behaviourFile << "computeElasticStiffness<N,Type>::exe(this->Dt,this-"
                           ">lambda,this->mu);\n";
    this->behaviourFile << "if(this->dp>prec){\n";
    this->behaviourFile << "const real ccto_tmp_1 =  this->dp/this->seq_e;\n";
    this->behaviourFile << "const auto& M = st2tost2<N,Type>::M();\n";
    this->behaviourFile << "this->Dt += "
                           "-4*(this->mu)*(this->mu)*(this->theta)*(ccto_tmp_1*"
                           "M-(ccto_tmp_1-this->df_dseq/"
                           "((this->theta)*(3*(this->mu)*(this->df_dseq)-(this-"
                           ">df_dp))))*((this->n)^(this->n)));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile
        << "} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n";
    this->behaviourFile << "computeElasticStiffness<N,Type>::exe(this->Dt,this-"
                           ">lambda,this->mu);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  }

  void IsotropicMisesPlasticFlowDSL::
      writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis) {
    this->checkBehaviourFile();
    this->behaviourFile
        << "this->se=(real{2})*(this->mu)*(tfel::math::deviator(this->eel+(";
    this->behaviourFile << this->mb.getClassName();
    this->behaviourFile << "::theta)*(this->deto)));\n";
    this->behaviourFile << "this->seq_e = sigmaeq(this->se);\n";
    this->behaviourFile
        << "if(this->seq_e>100*std::numeric_limits<stress>::epsilon()){\n";
    this->behaviourFile
        << "this->n = (real{3}/real{2})*(this->se)/(this->seq_e);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->n = StrainStensor(strain(0));\n";
    this->behaviourFile << "}\n";
  }  // end of
     // IsotropicMisesPlasticFlowDSL::writeBehaviourParserSpecificInitializeMethodPart

  IsotropicMisesPlasticFlowDSL::~IsotropicMisesPlasticFlowDSL() = default;

}  // end of namespace mfront
