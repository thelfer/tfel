/*!
 * \file   mfront/src/IsotropicStrainHardeningMisesCreepDSL.cxx
 * \brief
 * \author Helfer Thomas
 * \date   02 jui 2007
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
#include "MFront/IsotropicStrainHardeningMisesCreepDSL.hxx"

namespace mfront {

  IsotropicStrainHardeningMisesCreepDSL::
      IsotropicStrainHardeningMisesCreepDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("IsotropicStrainHardeningMisesCreep");
    // Default state vars
    this->mb.addStateVariable(
        h, VariableDescription("StrainStensor", "eel", 1u, 0u));
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->mb.setGlossaryName(h, "p", "EquivalentViscoplasticStrain");
    // default local vars
    this->reserveName("mu_3");
    this->mb.addLocalVariable(h, VariableDescription("DstrainDt", "f", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("DF_DSEQ_TYPE", "df_dseq", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("DstrainDt", "df_dp", 1u, 0u));
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
  }

  std::string IsotropicStrainHardeningMisesCreepDSL::getName() {
    return "IsotropicStrainHardeningMisesCreep";
  }

  std::string IsotropicStrainHardeningMisesCreepDSL::getDescription() {
    return "this parser is used for standard strain hardening creep behaviours "
           "of the form dp/dt=f(s,p) where p is the equivalent creep strain "
           "and s the equivalent mises stress";
  }  // end of IsotropicStrainHardeningMisesCreepDSL::getDescription

  void IsotropicStrainHardeningMisesCreepDSL::endsInputFileProcessing() {
    IsotropicBehaviourDSLBase::endsInputFileProcessing();
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
        throw(std::runtime_error(
            "IsotropicMisesCreepDSL::endsInputFileProcessing: "
            "no flow rule defined"));
      }
    }
  }  // end of IsotropicStrainHardeningMisesCreepDSL::endsInputFileProcessing

  void
  IsotropicStrainHardeningMisesCreepDSL::writeBehaviourParserSpecificMembers(
      const Hypothesis h) {
    using namespace std;
    this->checkBehaviourFile();
    if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
      string msg(
          "IsotropicStrainHardeningMisesCreepDSL::"
          "writeBehaviourParserSpecificMembers : ");
      msg += "no flow rule declared (use the @FlowRule directive)";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "void computeFlow(){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using namespace tfel::material;\n";
    this->behaviourFile << "using std::vector;\n";
    writeMaterialLaws(
        "IsotropicStrainHardeningMisesCreepDSL::"
        "writeBehaviourParserSpecificMembers",
        this->behaviourFile, this->mb.getMaterialLaws());
    this->behaviourFile
        << this->mb.getCode(h, BehaviourData::FlowRule) << endl
        << "}\n\n"
        << "bool NewtonIntegration(void){\n"
        << "using namespace std;\n"
        << "using namespace tfel::math;\n"
        << "bool converge=false;\n"
        << "bool inversible=true;\n"
        << "strain newton_f;\n"
        << "strain newton_df;\n"
        << "real newton_epsilon = 100*std::numeric_limits<real>::epsilon();\n"
        << "stress mu_3 = 3*(this->mu);\n"
        << ""
        << "unsigned int iter = 0u;\n"
        << "this->p_=this->p+this->dp;\n"
        << "while((converge==false)&&\n"
        << "(iter<(this->iterMax))&&\n"
        << "(inversible==true)){\n"
        << "this->seq=std::max(this->seq_e-mu_3*(this->theta)*(this->dp),real("
           "0.f));\n"
        << "this->computeFlow();\n"
        << "newton_f  = this->dp - (this->f)*(this->dt);\n"
        << "newton_df = "
           "1-(this->theta)*(this->dt)*((this->df_dp)-mu_3*(this->df_dseq));\n"
        << "if(std::abs(base_cast(newton_df))"
        << ">newton_epsilon){\n"
        << "this->dp -= newton_f/newton_df;\n"
        << "this->p_=this->p + (this->theta)*(this->dp);\n"
        << "iter+=1;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::NewtonIntegration() : iteration \" "
          << "<< iter << \" : \" << std::abs(tfel::math::base_cast(newton_f)) "
             "<< endl;\n";
    }
    this->behaviourFile
        << "converge = (std::abs(tfel::math::base_cast(newton_f))<";
    this->behaviourFile << "(this->epsilon));\n";
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

  void IsotropicStrainHardeningMisesCreepDSL::writeBehaviourIntegrator(
      const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
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

  void
  IsotropicStrainHardeningMisesCreepDSL::writeBehaviourComputeTangentOperator(
      const Hypothesis) {
    this->behaviourFile
        << "bool computeConsistentTangentOperator(const SMType smt){\n";
    this->behaviourFile << "using tfel::material::computeElasticStiffness;\n";
    this->behaviourFile << "using tfel::math::st2tost2;\n";
    this->behaviourFile << "if(smt==CONSISTENTTANGENTOPERATOR){\n";
    this->behaviourFile << "computeElasticStiffness<N,Type>::exe(this->Dt,this-"
                           ">lambda,this->mu);\n";
    this->behaviourFile << "if(this->seq_e>(0.01*(this->young))*std::numeric_"
                           "limits<stress>::epsilon()){\n";
    this->behaviourFile << "const real ccto_tmp_1 =  this->dp/this->seq_e;\n";
    this->behaviourFile << "const auto& M = st2tost2<N,Type>::M();\n";
    this->behaviourFile
        << "this->Dt += "
           "-Type(4)*(this->mu)*(this->mu)*(this->theta)*(ccto_tmp_1*M-(ccto_"
           "tmp_1-this->df_dseq*(this->dt)/"
           "(Type(1)+(this->theta)*(this->dt)*(Type(3)*(this->mu)*this->df_"
           "dseq-(this->df_dp))))*((this->n)^(this->n)));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile
        << "} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n";
    this->behaviourFile << "computeElasticStiffness<N,Type>::exe(this->Dt,this-"
                           ">lambda,this->mu);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "return false;";
    this->behaviourFile << "}\n";
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  }

  void IsotropicStrainHardeningMisesCreepDSL::
      writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis) {
    this->checkBehaviourFile();
    this->behaviourFile
        << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(";
    this->behaviourFile << this->mb.getClassName();
    this->behaviourFile << "::theta)*(this->deto)));\n";
    this->behaviourFile << "this->seq_e = sigmaeq(this->se);\n";
    this->behaviourFile << "if(this->seq_e>(0.01*(this->young))*std::numeric_"
                           "limits<stress>::epsilon()){\n";
    this->behaviourFile << "this->n = 1.5f*(this->se)/(this->seq_e);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->n = StrainStensor(strain(0));\n";
    this->behaviourFile << "}\n";
  }  // end of
     // IsotropicStrainHardeningMisesCreepDSL::writeBehaviourParserSpecificInitializeMethodPart

  IsotropicStrainHardeningMisesCreepDSL::
      ~IsotropicStrainHardeningMisesCreepDSL() = default;

}  // end of namespace mfront
