/*!
 * \file   MFrontMultipleIsotropicMisesFlows.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   31 jan 2008
 */

#include<string>
#include<stdexcept>
#include<sstream>

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontParserFactory.hxx"
#include"MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx"

namespace mfront{

  MFrontMultipleIsotropicMisesFlowsParser::MFrontMultipleIsotropicMisesFlowsParser()
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    // Default state vars
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->registerVariable("p");
    this->mb.getStateVariables().push_back(VarHandler("StrainStensor","eel",1u,0u));
    this->mb.getStateVariables().push_back(VarHandler("strain","p",1u,0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
    this->glossaryNames.insert(MVType("p","EquivalentStrain"));
    // default local vars
    this->registerVariable("se");
    this->registerVariable("seq");
    this->registerVariable("seq_e");
    this->registerVariable("n");
    this->registerVariable("mu_3_theta");
    this->registerVariable("surf");
    this->registerVariable("error");
    this->registerVariable("iter");
    this->registerVariable("p_");
    this->mb.getLocalVariables().push_back(VarHandler("StressStensor","se",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("stress","seq",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("stress","seq_e",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("StrainStensor","n",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("strain","p_",1u,0u));
    this->flowRule = "unused";
  }

  std::string
  MFrontMultipleIsotropicMisesFlowsParser::getName(void)
  {
    return "MultipleIsotropicMisesFlows";
  }

  std::string
  MFrontMultipleIsotropicMisesFlowsParser::getDescription()
  {
    return "this parser is used to define behaviours combining several "
           "isotropic flows. Supported flow type are 'Creep' (dp/dt=f(s)) "
           "'StrainHardeningCreep' (dp/dt=f(s,p)) and 'Plasticity' (f(p,s)=0) "
           "where p is the equivalent plastic strain and s the equivalent "
           "mises stress";
  } // end of MFrontMultipleIsotropicMisesFlowsParser::getDescription

  void
  MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourParserSpecificIncludes(void)
  {
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/General/BaseCast.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/TinyMatrixSolve.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Material/Lame.hxx\"\n\n";
  }

  void
  MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourParserSpecificMembers(void)
  {
    using namespace std;
    vector<FlowHandler>::const_iterator p;
    vector<FlowHandler>::const_iterator p2;
    bool found;
    unsigned short n;
    unsigned short n2;
    bool genericTheta;

    this->checkBehaviourFile();

    if(this->flowRule.empty()){
      string msg("MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourParserSpecificMembers : ");
      msg += "no flow rule declared (use the @FlowRule directive)";
      throw(runtime_error(msg));
    }

    for(p=this->flows.begin(),n=0;p!=this->flows.end();++p,++n){    
      if(p->flow==FlowHandler::PlasticFlow){
	this->behaviourFile << "void computeFlow" << n << "(stress& f,\n"
			    << "real& df_dseq,\n"
			    << "stress& df_dp){\n";
      } else if(p->flow==FlowHandler::CreepFlow){
	this->behaviourFile << "void computeFlow" << n << "(DstrainDt& f,\n"
			    << "DF_DSEQ_TYPE& df_dseq){\n";
      } else if(p->flow==FlowHandler::StrainHardeningCreepFlow){
	this->behaviourFile << "void computeFlow" << n << "(DstrainDt& f,\n"
			    << "DF_DSEQ_TYPE& df_dseq,\n"
			    << "DstrainDt& df_dp){\n";
      }
      this->behaviourFile << "using namespace std;\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "using namespace tfel::material;\n";
      writeMaterialLaws("MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourParserSpecificMembers",
			this->behaviourFile,this->materialLaws);
      this->behaviourFile << p->flowRule << endl;
      this->behaviourFile << "}\n\n";
    }
    this->behaviourFile << "bool NewtonIntegration(void){\n";
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    this->behaviourFile << "real error;\n";
    this->behaviourFile << "unsigned int iter;\n";
    this->behaviourFile << "bool converge=false;\n";

    this->behaviourFile << "tvector<" << this->flows.size() << ",strain> vdp(strain(real(0.)));\n";
    this->behaviourFile << "tvector<" << this->flows.size() << ",strain> newton_f;\n";
    this->behaviourFile << "tmatrix<" << this->flows.size() 
			<< ","  << this->flows.size() << ",strain> newton_df;\n";

    genericTheta=false;
    for(p=this->flows.begin(),n=0;p!=this->flows.end();++p,++n){
      if(p->hasSpecificTheta){
	ostringstream otheta;
	otheta << "mu_3_theta" << n;
	this->behaviourFile << "stress "+otheta.str()+" = 3*(real(";
	this->behaviourFile << p->theta << "))*(this->mu);\n";
      } else {
	genericTheta=true;
      }
    }

    if(genericTheta){
      this->behaviourFile << "stress mu_3_theta = 3*(";
      this->behaviourFile << this->className << "::theta)*(this->mu);\n";
    }

    found=false;
    for(p=this->flows.begin();(p!=this->flows.end())&&!(found);++p){
      if(p->flow==FlowHandler::PlasticFlow){
	this->behaviourFile << "real surf;\n";
	this->behaviourFile << "real newton_epsilon = 100*std::numeric_limits<real>::epsilon();\n";
	found = true;
      }
    }
    this->behaviourFile << "iter=0;\n";
    this->behaviourFile << "while((converge==false)&&\n";
    this->behaviourFile << "(iter<(" << this->className << "::iterMax))){\n";
    for(p=this->flows.begin(),n=0;p!=this->flows.end();++p,++n){
      this->behaviourFile << "this->p_  = this->p" << n << " + (";
      if(p->hasSpecificTheta){
	this->behaviourFile << "real(" << p->theta << ")";
      } else {
	this->behaviourFile << this->className << "::theta";
      }
      this->behaviourFile << ")*(vdp(" << n << "));\n";
      if(p->hasSpecificTheta){
	ostringstream otheta;
	this->behaviourFile << "this->seq = std::max(this->seq_e" << n << "-";
	otheta << "mu_3_theta" << n << "*(";
	this->behaviourFile << otheta.str();
      } else {
	ostringstream otheta;
	this->behaviourFile << "this->seq = std::max(this->seq_e-";
	this->behaviourFile << "mu_3_theta*(";
      }
      p2=this->flows.begin();
      n2=0;
      while(p2!=this->flows.end()){
	this->behaviourFile << "vdp(" << n2 << ")";
	++p2;
	++n2;
	if(p2!=this->flows.end()){
	  this->behaviourFile << "+";
	}
      }
      this->behaviourFile << "),real(0.f));\n";
      if(p->flow==FlowHandler::PlasticFlow){
	this->behaviourFile << "this->computeFlow" << n << "("
			    << "this->f"       << n << ","
			    << "this->df_dseq" << n << ","
			    << "this->df_dp"   << n << ");\n";
	this->behaviourFile << "surf = (this->f" << n << ")/(this->young);\n";
	this->behaviourFile << "if(((surf>newton_epsilon)&&((vdp(" << n << "))>=0))||"
			    << "((vdp(" << n << "))>newton_epsilon)){";
	this->behaviourFile << "newton_f(" << n << ")  = surf;\n";
	for(p2=this->flows.begin(),n2=0;p2!=this->flows.end();++p2,++n2){
	  if(p2==p){
	    this->behaviourFile << "newton_df(" << n << "," << n << ")";
	    if(p->hasSpecificTheta){
	      this->behaviourFile << " = ((real("<< p->theta << "))*(this->df_dp" << n << ")"
				  << "-mu_3_theta" << n << "*(this->df_dseq" << n <<"))/(this->young);\n";
	    } else {
	      this->behaviourFile << " = (("<< this->className << "::theta)*(this->df_dp" << n << ")"
				  << "-mu_3_theta*(this->df_dseq" << n <<"))/(this->young);\n";
	    }
	  } else {
	    this->behaviourFile << "newton_df(" << n << "," << n2 << ")";
	    if(p->hasSpecificTheta){
	      this->behaviourFile << " = -mu_3_theta" << n << "*(this->df_dseq" << n <<")/(this->young);\n";
	    } else {
	      this->behaviourFile << " = -mu_3_theta*(this->df_dseq" << n <<")/(this->young);\n";
	    }
	  }
	}
	this->behaviourFile << "} else {\n";
	this->behaviourFile << "newton_f("  << n << ")  =(vdp(" << n << "));\n";
	this->behaviourFile << "newton_df(" << n << "," << n << ") = real(1.);\n";
	for(p2=this->flows.begin(),n2=0;p2!=this->flows.end();++p2,++n2){
	  if(p2!=p){
	    this->behaviourFile << "newton_df(" << n << "," << n2 << ") = real(0.);\n";
	  }
	}	
	this->behaviourFile << "}\n";
      } else  if (p->flow==FlowHandler::CreepFlow){
	this->behaviourFile << "this->computeFlow" << n << "("
			    << "this->f" << n << ","
			    << "this->df_dseq" << n << ""
			    << ");\n";
	this->behaviourFile << "newton_f(" << n << ")  = vdp(" << n << ") - (this->f" << n << ")*(this->dt);\n";
	this->behaviourFile << "newton_df(" << n << "," << n << ") = 1+";
	if(p->hasSpecificTheta){
	  this->behaviourFile <<  "mu_3_theta" << n;
	} else {
	  this->behaviourFile <<  "mu_3_theta";
	}
	this->behaviourFile << "*(this->df_dseq" << n << ")*(this->dt);\n";
	for(p2=this->flows.begin(),n2=0;p2!=this->flows.end();++p2,++n2){
	  if(p2!=p){
	    this->behaviourFile << "newton_df(" << n << "," << n2 << ") = ";
	    if(p->hasSpecificTheta){
	      this->behaviourFile <<  "mu_3_theta" << n;
	    } else {
	      this->behaviourFile <<  "mu_3_theta";
	    }
	    this->behaviourFile << "*(this->df_dseq" << n << ")*(this->dt);\n";
	  }
	}
      } else {
	this->behaviourFile << "this->computeFlow" << n << "("
			    << "this->f" << n << ","
			    << "this->df_dseq" << n << ","
			    << "this->df_dp"   << n << ");\n";
	this->behaviourFile << "newton_f(" << n << ")  = vdp(" << n << ") - (this->f" << n << ")*(this->dt);\n";
	this->behaviourFile << "newton_df(" << n << "," << n << ") = 1-(this->dt)*(";
	if(p->hasSpecificTheta){
	  this->behaviourFile <<  "(real(" << p->theta << "))";
	} else {
	  this->behaviourFile <<  "(" << this->className << "::theta)";
	}
	this->behaviourFile << "*(this->df_dp" << n << ")-";
	if(p->hasSpecificTheta){
	  this->behaviourFile <<  "mu_3_theta" << n;
	} else {
	  this->behaviourFile <<  "mu_3_theta";
	}
	this->behaviourFile << "*(this->df_dseq" << n << "));\n";
	for(p2=this->flows.begin(),n2=0;p2!=this->flows.end();++p2,++n2){
	  if(p2!=p){
	    this->behaviourFile << "newton_df(" << n << "," << n2 << ") = ";
	    if(p->hasSpecificTheta){
	      this->behaviourFile <<  "mu_3_theta" << n;
	    } else {
	      this->behaviourFile <<  "mu_3_theta";
	    }
	    this->behaviourFile << "*(this->df_dseq" << n << ")*(this->dt);\n";
	  }
	}
      }
    }
    this->behaviourFile << "error=static_cast<real>(0.);\n";
    for(p=this->flows.begin(),n=0;p!=this->flows.end();++p,++n){
      this->behaviourFile << "error+=std::abs(tfel::math::base_cast(newton_f(" << n << ")));\n";
    }
    this->behaviourFile << "try{" << endl;
    this->behaviourFile << "TinyMatrixSolve<" << this->flows.size() << "," << "real>::exe(newton_df,newton_f);\n";
    this->behaviourFile << "} catch(LUException&){" << endl;
    this->behaviourFile << "return false;" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "vdp -= newton_f;\n";
    this->behaviourFile << "iter+=1;\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::NewtonIntegration() : iteration \" "
			  << "<< iter << \" : \" << (error/(real(" << this->flows.size() << "))) << endl;\n";
    }
    this->behaviourFile << "converge = ((error)/(real(" << this->flows.size() << "))<";
    this->behaviourFile << "(" << this->className << "::epsilon));\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(iter==" << this->className << "::iterMax){\n";
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::NewtonIntegration() : no convergence after \" "
			  << "<< iter << \" iterations\"<< endl << endl;\n";
      this->behaviourFile << "cout << *this << endl;\n";
    }
    this->behaviourFile << "return false;" << endl;
    this->behaviourFile << "}\n\n";
    for(p=this->flows.begin(),n=0;p!=this->flows.end();++p,++n){
      this->behaviourFile << "this->dp"<< n << " = " << "vdp(" << n<< ");\n";
    }
    if(this->debugMode){
      this->behaviourFile << "cout << \"" << this->className
			  << "::NewtonIntegration() : convergence after \" "
			  << "<< iter << \" iterations\"<< endl << endl;\n";
    }
    this->behaviourFile << "return true;" << endl;
    this->behaviourFile << "\n}\n\n";
  } // end of writeBehaviourParserSpecificMembers

  void MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourIntegrator(void)
  {
    using namespace std;
    vector<BoundsDescription>::const_iterator p;
    vector<FlowHandler>::const_iterator p2;
    unsigned short n;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult" << endl;
    this->behaviourFile << "integrate(const SMType smt){\n";
    this->behaviourFile << "if(!this->NewtonIntegration()){\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->dp = ";
    p2=this->flows.begin();
    n=0;
    while(p2!=this->flows.end()){
      this->behaviourFile << "this->dp" << n << "";
      ++n;
      ++p2;
      if(p2!=this->flows.end()){
	this->behaviourFile << "+";
      }
    }
    this->behaviourFile << ";\n";
    this->behaviourFile << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    this->behaviourFile << "if(!this->computeConsistantTangentOperator(smt)){\n";
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "this->deel = this->deto-dp*(this->n);\n";
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
  MFrontMultipleIsotropicMisesFlowsParser::treatFlowRule(void)
  {
    using namespace std;
    FlowHandler flow;
    this->checkNotEndOfFile("MFrontMultipleIsotropicMisesFlowsParser::treatFlowRule",
			    "Expected flow rule name.");
    if(this->current->value=="Plasticity"){
      ostringstream p;
      ostringstream f;
      ostringstream df_dseq;
      ostringstream df_dp;
      p       << "p"       << this->flows.size();
      f       << "f"       << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      df_dp   << "df_dp"   << this->flows.size();
      this->registerVariable(p.str());
      this->registerVariable("d"+p.str());
      this->registerVariable(f.str());
      this->registerVariable(df_dseq.str());
      this->registerVariable(df_dp.str());
      this->mb.getStateVariables().push_back(VarHandler("strain",p.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("stress",f.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("real",df_dseq.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("stress",df_dp.str(),1u,0u));
      flow.flow = FlowHandler::PlasticFlow;
    } else if(this->current->value=="Creep"){
      ostringstream p;
      ostringstream f;
      ostringstream df_dseq;
      p       << "p"       << this->flows.size();
      f       << "f"       << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      this->mb.getStateVariables().push_back(VarHandler("strain",p.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("DstrainDt",f.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("DF_DSEQ_TYPE",df_dseq.str(),1u,0u));
      this->registerVariable(p.str());
      this->registerVariable(f.str());
      this->registerVariable(df_dseq.str());
      flow.flow = FlowHandler::CreepFlow;
    } else if(this->current->value=="StrainHardeningCreep"){
      ostringstream p;
      ostringstream f;
      ostringstream df_dseq;
      ostringstream df_dp;
      p       << "p"       << this->flows.size();
      f       << "f"       << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      df_dp   << "df_dp"   << this->flows.size();
      this->mb.getStateVariables().push_back(VarHandler("strain",p.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("DstrainDt",f.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("DF_DSEQ_TYPE",df_dseq.str(),1u,0u));
      this->mb.getLocalVariables().push_back(VarHandler("DstrainDt",df_dp.str(),1u,0u));
      this->registerVariable(p.str());
      this->registerVariable(f.str());
      this->registerVariable(df_dseq.str());
      this->registerVariable(df_dp.str());
      flow.flow = FlowHandler::StrainHardeningCreepFlow;
    } else {
      this->throwRuntimeError("MFrontMultipleIsotropicMisesFlowsParser::treatFlowRule",
			      "Unknown flow rule (read '"+this->current->value+
			      "').Valid flow rules are 'Plasticity', 'Creep' and 'StrainHardeningCreep'.");
    }
    ++(this->current);
    this->checkNotEndOfFile("MFrontMultipleIsotropicMisesFlowsParser::treatFlowRule",
			    "Expected the beginning of a block or a specific theta value.");
    if(this->current->value!="{"){
      istringstream converter(this->current->value);
      ostringstream otheta;
      ostringstream ose;
      ostringstream oseq_e;
      flow.hasSpecificTheta = true;
      converter >> flow.theta;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontMultipleIsotropicMisesFlowsParser::treatFlowRule",
				"Could not read theta value (read '"+this->current->value+"').");
      }
      otheta << "mu_3_theta" << this->flows.size();
      ose    << "se"         << this->flows.size();
      oseq_e << "seq_e"      << this->flows.size();
      this->reserveName(ose.str());
      this->registerVariable(otheta.str());
      this->registerVariable(oseq_e.str());
      this->mb.getLocalVariables().push_back(VarHandler("stress",oseq_e.str(),1u,0u));
      ++(this->current);
    } else {
      flow.hasSpecificTheta = false;
    }
    MFrontIsotropicBehaviourParserBase::treatFlowRuleBase(flow.flowRule);
    this->flows.push_back(flow);
  } // end of MFrontMultipleIsotropicMisesFlowsParser::treatFlowRule

  void
  MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourParserSpecificConstructorPart(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    vector<FlowHandler>::const_iterator p;
    unsigned short n;
    if(this->varNames.find("young")==this->varNames.end()){
      string msg("MFrontMultipleIsotropicMisesFlowsParser");
      msg+="::writeBehaviourParserSpecificConstructorPart : ";
      msg += "young (the young modulus) has not been defined.";
      throw(runtime_error(msg));
    }
    if(this->varNames.find("nu")==this->varNames.end()){
      string msg("MFrontMultipleIsotropicMisesFlowsParser");
      msg += "::writeBehaviourParserSpecificConstructorPart : ";
      msg += "nu (the poisson ratio) has not been defined.";
      throw(runtime_error(msg));
    }
    this->behaviourFile << "this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(";
    this->behaviourFile << this->className;
    this->behaviourFile << "::theta)*(this->deto)));\n";
    this->behaviourFile << "this->seq_e = sigmaeq(this->se);\n";
    for(p=this->flows.begin(),n=0;p!=this->flows.end();++p,++n){
      if(p->hasSpecificTheta){
	this->behaviourFile << "StressStensor se" << n << "=2*(this->mu)*(tfel::math::deviator(this->eel+(";
	this->behaviourFile << p->theta << ")*(this->deto)));\n";
	this->behaviourFile << "this->seq_e" << n << " = sigmaeq(se" << n << ");\n";
      }
    }
    this->behaviourFile << "if(this->seq_e>100*std::numeric_limits<stress>::epsilon()){\n";
    this->behaviourFile << "this->n = 1.5f*(this->se)/(this->seq_e);\n";
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "this->n = StrainStensor(strain(0));\n";
    this->behaviourFile << "}\n";
  } // end of MFrontMultipleIsotropicMisesFlowsParser::writeBehaviourParserSpecificConstructorPart

  MFrontMultipleIsotropicMisesFlowsParser::~MFrontMultipleIsotropicMisesFlowsParser()
  {} // end of ~MFrontMultipleIsotropicMisesFlowsParser

} // end of namespace mfront

