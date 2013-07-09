/*!
 * \file   MFrontIsotropicMisesCreepParser.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<string>
#include<stdexcept>

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontIsotropicMisesCreepParser.hxx"

namespace mfront{

  MFrontIsotropicMisesCreepParser::MFrontIsotropicMisesCreepParser()
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    // Default state vars
    this->reserveName("ccto_tmp_1");
    this->reserveName("inv_sqrt2");
    this->registerVariable("p");
    this->registerVariable("dp");
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->mb.getStateVariables().push_back(VarHandler("StrainStensor","eel",1u,0u));
    this->mb.getStateVariables().push_back(VarHandler("strain","p",1u,0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
    this->glossaryNames.insert(MVType("p","EquivalentViscoplasticStrain"));
    // default local vars
    this->registerVariable("f");
    this->registerVariable("df_dseq");
    this->registerVariable("se");
    this->registerVariable("seq");
    this->registerVariable("seq_e");
    this->registerVariable("n");
    this->registerVariable("mu_3_theta");
    this->mb.getLocalVariables().push_back(VarHandler("DstrainDt","f",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("DF_DSEQ_TYPE","df_dseq",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("StressStensor","se",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("stress","seq",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("stress","seq_e",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("StrainStensor","n",1u,0u));
    this->hasConsistantTangentOperator = true;
    this->isConsistantTangentOperatorSymmetric = true;
  }

  std::string
  MFrontIsotropicMisesCreepParser::getDescription()
  {
    return "this parser is used for standard creep behaviours of the form  "
           " dp/dt=f(s) where p is the equivalent creep strain and s the "
           "equivalent mises stress";
  } // end of MFrontIsotropicMisesCreepParser::getDescription

  std::string
  MFrontIsotropicMisesCreepParser::getName(void)
  {
    return "IsotropicMisesCreep";
  }

  void
  MFrontIsotropicMisesCreepParser::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;

    this->checkBehaviourFile();

    if(this->flowRule.empty()){
      string msg("MFrontIsotropicMisesCreepParser::writeBehaviourParserSpecificMembers : ");
      msg += "no flow rule declared (use the @FlowRule directive)";
      throw(runtime_error(msg));
    }

    this->behaviourFile << "void computeFlow(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "using namespace tfel::material;\n";
    writeMaterialLaws("MFrontIsotropicMisesCreepParser::writeBehaviourParserSpecificMembers",
		      this->behaviourFile,this->materialLaws);
    this->behaviourFile << this->flowRule << endl;
    this->behaviourFile << "}\n\n";

    this->behaviourFile << "bool NewtonIntegration(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "unsigned int iter;\n";
    this->behaviourFile << "bool converge=false;\n";
    this->behaviourFile << "bool inversible=true;\n";
    this->behaviourFile << "strain newton_f;\n";
    this->behaviourFile << "strain newton_df;\n";
    this->behaviourFile << "real newton_epsilon = 100.*std::numeric_limits<real>::epsilon();\n";
    this->behaviourFile << "stress mu_3_theta = 3*(this->theta)*(this->mu);\n";
    this->behaviourFile << "";
    this->behaviourFile << "iter=0;\n";    
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(iter<(this->iterMax))&&\n";
    this->behaviourFile << "(inversible==true)){\n";
    this->behaviourFile << "this->seq = std::max(this->seq_e-mu_3_theta*(this->dp),real(0.f));\n";
    this->behaviourFile << "this->computeFlow();\n";
    this->behaviourFile << "newton_f  = this->dp - (this->f)*(this->dt);\n";
    this->behaviourFile << "newton_df = 1+mu_3_theta*(this->df_dseq)*(this->dt);\n";
    this->behaviourFile << "if(std::abs(base_cast(newton_df))";
    this->behaviourFile << ">newton_epsilon){\n";
    this->behaviourFile << "this->dp -= newton_f/newton_df;\n";
    this->behaviourFile << "iter+=1;\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::NewtonIntegration() : iteration \" "
			  << "<< iter << \" : \" << std::abs(tfel::math::base_cast(newton_f)) << endl;\n";
    }
    this->behaviourFile << "converge = (std::abs(tfel::math::base_cast(newton_f))<";
    this->behaviourFile << "(this->epsilon));\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "inversible=false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(inversible==false){\n";
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(iter==this->iterMax){\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::NewtonIntegration() : no convergence after \" "
			  << "<< iter << \" iterations\"<< endl << endl;\n";
      this->behaviourFile << "cout << *this << endl;\n";
    }
    this->behaviourFile << "return false;\n";
    this->behaviourFile << "}\n\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::NewtonIntegration() : convergence after \" "
			  << "<< iter << \" iterations\"<< endl << endl;\n";
    }
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  } // end of writeBehaviourParserSpecificMembers

  void MFrontIsotropicMisesCreepParser::writeBehaviourIntegrator(void)
  {
    using namespace std;
    std::vector<BoundsDescription>::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult\n";
    this->behaviourFile << "integrate(const SMType smt){\n";
    this->behaviourFile << "if(!this->NewtonIntegration()){\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    this->behaviourFile << "if(!this->computeConsistantTangentOperator(smt)){\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->deel = this->deto-(this->dp)*(this->n);\n";
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->sig  = (this->lambda)*trace(this->eel)*StrainStensor::Id()+2*(this->mu)*(this->eel);\n";
    this->behaviourFile << "this->updateAuxiliaryStateVars();\n";
    for(p  = this->mb.getBounds().begin();
	p != this->mb.getBounds().end();++p){
      if(p->varCategory==BoundsDescription::StateVar){
	p->writeBoundsChecks(this->behaviourFile);
      }
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "}\n\n";
  }

  void
  MFrontIsotropicMisesCreepParser::writeBehaviourComputeTangentOperator(void)
  {
    this->behaviourFile << "bool computeConsistantTangentOperator(const SMType smt){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using tfel::material::lame::computeElasticStiffness;\n";
    this->behaviourFile << "using tfel::math::st2tost2;\n";
    this->behaviourFile << "if(smt==CONSISTANTTANGENTOPERATOR){\n";
    this->behaviourFile << "computeElasticStiffness<N,Type>::exe(this->Dt,this->lambda,this->mu);\n";
    this->behaviourFile << "if(this->seq_e>(0.01*(this->young))*std::numeric_limits<stress>::epsilon()){\n";
    this->behaviourFile << "const real ccto_tmp_1 =  this->dp/this->seq_e;\n";
    this->behaviourFile << "const st2tost2<N,Type>& M = st2tost2<N,Type>::M();\n";
    this->behaviourFile << "this->Dt += -4*(this->mu)*(this->mu)*(this->theta)*(ccto_tmp_1*M-(ccto_tmp_1-this->df_dseq*(this->dt)/(1+3*(this->mu)*(this->theta)*(this->dt)*this->df_dseq))*((this->n)^(this->n)));\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n";
    this->behaviourFile << "computeElasticStiffness<N,Type>::exe(this->Dt,this->lambda,this->mu);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "return false;";
    this->behaviourFile << "}\n";
    this->behaviourFile << "return true;\n";
    this->behaviourFile << "}\n\n";
  }

  void
  MFrontIsotropicMisesCreepParser::writeBehaviourParserSpecificConstructorPart(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    if(this->varNames.find("young")==this->varNames.end()){
      string msg("MFrontIsotropicMisesCreepParser");
      msg+="::writeBehaviourParserSpecificConstructorPart : ";
      msg += "young (the young modulus) has not been defined.";
      throw(runtime_error(msg));
    }
    if(this->varNames.find("nu")==this->varNames.end()){
      string msg("MFrontIsotropicMisesCreepParser");
      msg += "::writeBehaviourParserSpecificConstructorPart : ";
      msg += "nu (the poisson ratio) has not been defined.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(this->theta)*(this->deto)));\n";
    this->behaviourFile << "this->seq_e = sigmaeq(this->se);\n";
    this->behaviourFile << "if(this->seq_e>(0.01*(this->young))*std::numeric_limits<stress>::epsilon()){\n";
    this->behaviourFile << "this->n = 1.5f*(this->se)/(this->seq_e);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->n = StrainStensor(strain(0));\n";
    this->behaviourFile << "}\n";
  } // end of MFrontIsotropicMisesCreepParser::writeBehaviourParserSpecificConstructorPart

  MFrontIsotropicMisesCreepParser::~MFrontIsotropicMisesCreepParser()
  {} // end of ~MFrontIsotropicMisesCreepParser
  
} // end of namespace mfront
