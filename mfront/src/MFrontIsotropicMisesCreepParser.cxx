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
    this->registerVariable("p");
    this->registerVariable("dp");
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->stateVarsHolder.push_back(VarHandler("StrainStensor","eel",0u));
    this->stateVarsHolder.push_back(VarHandler("strain","p",0u));
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
    this->localVarsHolder.push_back(VarHandler("DstrainDt","f",0u));
    this->localVarsHolder.push_back(VarHandler("DF_DSEQ_TYPE","df_dseq",0u));
    this->localVarsHolder.push_back(VarHandler("StressStensor","se",0u));
    this->localVarsHolder.push_back(VarHandler("stress","seq",0u));
    this->localVarsHolder.push_back(VarHandler("stress","seq_e",0u));
    this->localVarsHolder.push_back(VarHandler("StrainStensor","n",0u));
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

    this->behaviourFile << "void NewtonIntegration(void){\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "unsigned int iter;\n";
    this->behaviourFile << "bool converge=false;\n";
    this->behaviourFile << "bool inversible=true;\n";
    this->behaviourFile << "strain newton_f;\n";
    this->behaviourFile << "strain newton_df;\n";
    this->behaviourFile << "real newton_epsilon = 100*std::numeric_limits<real>::epsilon();\n";
    this->behaviourFile << "stress mu_3_theta = 3*(";
    this->behaviourFile << this->className << "::theta)*(this->mu);\n";
    this->behaviourFile << "";
    this->behaviourFile << "iter=0;\n";    
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(iter<(" << this->className << "::iterMax))&&\n";
    this->behaviourFile << "(inversible==true)){\n";
    this->behaviourFile << "this->seq = std::max(this->seq_e-mu_3_theta*(this->dp),real(0.f));\n";
    this->behaviourFile << "this->computeFlow();\n";
    this->behaviourFile << "newton_f  = this->dp - (this->f)*(this->dt);\n";
    this->behaviourFile << "newton_df = 1+mu_3_theta*(this->df_dseq)*(this->dt);\n";
    this->behaviourFile << "if(std::abs(base_cast(newton_df))";
    this->behaviourFile << ">newton_epsilon){\n";
    this->behaviourFile << "this->dp -= newton_f/newton_df;\n";
    this->behaviourFile << "iter+=1;\n";
    this->behaviourFile << "converge = (std::abs(tfel::math::base_cast(newton_f))<";
    this->behaviourFile << "(" << this->className << "::epsilon));\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "inversible=false;\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(inversible==false){\n";
    this->behaviourFile << "throw(DivergenceException(\"Newton-Raphson: null derivative\"));\n";
    this->behaviourFile << "}\n\n";
    
    this->behaviourFile << "if(iter==" << this->className << "::iterMax){\n";
    this->behaviourFile << "throw(DivergenceException(\"Newton-Raphson: no convergence\"));\n";
    this->behaviourFile << "}\n\n";
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
    this->behaviourFile << "void\n";
    this->behaviourFile << "integrate(void){\n";
    this->behaviourFile << "this->NewtonIntegration();\n";
    this->behaviourFile << "this->deel = this->deto-(this->dp)*(this->n);\n";
    this->behaviourFile << "this->updateStateVars();\n";
    this->behaviourFile << "this->sig  = (this->lambda)*trace(this->eel)*StrainStensor::Id()+2*(this->mu)*(this->eel);\n";
    for(p  = this->boundsDescriptions.begin();
	p != this->boundsDescriptions.end();++p){
      if(p->varCategory==BoundsDescription::StateVar){
	p->writeBoundsChecks(this->behaviourFile);
      }
    }
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
    this->behaviourFile << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(";
    this->behaviourFile << this->className;
    this->behaviourFile << "::theta)*(this->deto)));\n";
    this->behaviourFile << "this->seq_e = sigmaeq(this->se);\n";
    this->behaviourFile << "if(this->seq_e>100*std::numeric_limits<stress>::epsilon()){\n";
    this->behaviourFile << "this->n = 1.5f*(this->se)/(this->seq_e);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->n = StrainStensor(strain(0));\n";
    this->behaviourFile << "}\n";
  } // end of MFrontIsotropicMisesCreepParser::writeBehaviourParserSpecificConstructorPart

  MFrontIsotropicMisesCreepParser::~MFrontIsotropicMisesCreepParser()
  {} // end of ~MFrontIsotropicMisesCreepParser
  
} // end of namespace mfront
