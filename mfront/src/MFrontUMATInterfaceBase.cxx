/*! 
 * \file  MFrontUMATInterfaceBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/System/System.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontUMATInterfaceBase.hxx"

namespace mfront
{

  const std::string&
  MFrontUMATInterfaceBase::getGlossaryName(const std::map<std::string,std::string>& glossaryNames,
					   const std::map<std::string,std::string>& entryNames,
					   const std::string& v)
  {
    using namespace std;
    map<string,string>::const_iterator p2;
    map<string,string>::const_iterator p3;
    if((p2=glossaryNames.find(v))!=glossaryNames.end()){
      return p2->second;
    } else if((p3=entryNames.find(v))!=entryNames.end()){
      return p3->second;
    }
    return v;
  } // end of MFrontUMATInterfaceBase::getGlossaryName

  MFrontUMATInterfaceBase::MFrontUMATInterfaceBase()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {} // end of MFrontUMATInterfaceBase::MFrontUMATInterfaceBase

  void 
  MFrontUMATInterfaceBase::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontUMATInterfaceBase::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontUMATInterfaceBase::setDebugMode(void)
  {
    this->debugMode = true;
  }

  void
  MFrontUMATInterfaceBase::allowDynamicallyAllocatedArrays(const bool b)
  {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  } // end of MFrontUMATInterfaceBase::allowDynamicallyAllocatedArrays

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterfaceBase::getGlobalDependencies(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontUMATInterfaceBase::getGlobalDependencies

  void
  MFrontUMATInterfaceBase::reset(void)
  {
    SupportedTypes::reset();
    this->generateMTestFile = false;
  } // end of MFrontUMATInterfaceBase::reset

  void 
  MFrontUMATInterfaceBase::exportMechanicalData(std::ofstream& behaviourDataFile,
						const std::string&,
						const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const VarContainer& stateVarsHolder          = mb.getStateVariables();
    const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << "void\n"
			<< iprefix+"exportStateData("
			<< "Type * const "+iprefix+"stress_,Type * const "+iprefix+"statev) const\n";
    } else {
      behaviourDataFile << "void\n"
			<< iprefix+"exportStateData("
			<< "Type * const "+iprefix+"stress_,Type * const) const\n";
    }
    behaviourDataFile << "{\n";
    behaviourDataFile << "using namespace tfel::math;\n";
    behaviourDataFile << "this->sig.exportTab("+iprefix+"stress_);\n";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      SupportedTypes::TypeSize o;
      o = this->exportResults(behaviourDataFile,
			      stateVarsHolder,
			      iprefix+"statev",
			      mb.useQt());
      this->exportResults(behaviourDataFile,
			  auxiliaryStateVarsHolder,
			  iprefix+"statev",
			  mb.useQt(),o);
    }
    behaviourDataFile << "} // end of "+iprefix+"ExportStateData\n";
    behaviourDataFile << endl;
  }
  
  void
  MFrontUMATInterfaceBase::writeBehaviourConstructor(std::ofstream& behaviourFile,
						     const std::string& className,
						     const MechanicalBehaviourDescription& mb,
						     const std::string& initStateVarsIncrements)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    behaviourFile << "/*\n";
    behaviourFile << " * \\brief constructor for the umat interface\n";
    behaviourFile << " *\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dt_, time increment\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"stress_, stress tensor\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"stran, strain tensor\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dstran, strain increment tensor\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"T_, temperature\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dT_, temperature increment\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"mat, material properties\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"int_vars, state variables\n"; 
    behaviourFile << " * \\param const Type *const "+iprefix+"ext_vars, external state variables\n";
    behaviourFile << " * \\param const Type *const "+iprefix+"dext_vars,";
    behaviourFile << " external state variables increments\n";
    behaviourFile << " */\n";
    behaviourFile << className 
		  << "(const Type* const "+iprefix+"dt_,const Type* const "+iprefix+"stress_,\n"
		  <<  "const Type* const "+iprefix+"stran, const Type* const "+iprefix+"dstran,\n" 
		  <<  "const Type* const "+iprefix+"T_,const Type* const "+iprefix+"dT_,\n"
		  <<  "const Type* const "+iprefix+"mat,const Type* const "+iprefix+"int_vars,\n"
		  <<  "const Type* const "+iprefix+"ext_vars,const Type* const "+iprefix+"dext_vars)\n";
    if(mb.useQt()){
      behaviourFile << ": " << className 
		    << "BehaviourData<N,Type,use_qt>("+iprefix+"stress_,"+iprefix+"stran,"+iprefix+"T_,"+iprefix+"mat,\n"
		    << iprefix+"int_vars,"+iprefix+"ext_vars),\n";
      behaviourFile << className 
		    << "IntegrationData<N,Type,use_qt>("+iprefix+"dt_,"+iprefix+"dstran,"+iprefix+"dT_,"+iprefix+"dext_vars)";
    } else {
      behaviourFile << ": " << className 
		    << "BehaviourData<N,Type,false>("+iprefix+"stress_,"+iprefix+"stran,"+iprefix+"T_,"+iprefix+"mat,\n"
		    << iprefix+"int_vars,"+iprefix+"ext_vars),\n";
      behaviourFile << className 
		    << "IntegrationData<N,Type,false>("+iprefix+"dt_,"+iprefix+"dstran,"+iprefix+"dT_,"+iprefix+"dext_vars)";
    }
    behaviourFile << initStateVarsIncrements;
  }
  
  void 
  MFrontUMATInterfaceBase::writeBehaviourDataConstructor(std::ofstream& behaviourDataFile,
							 const std::string& className,
							 const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    const VarContainer& coefsHolder              = mb.getMaterialProperties();
    const VarContainer& stateVarsHolder          = mb.getStateVariables();
    const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const VarContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    behaviourDataFile << "/*\n";
    behaviourDataFile << " * \\brief constructor for the umat interface\n";
    behaviourDataFile << " *\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"stress_, stress tensor\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"stran, strain tensor\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"T_, temperature\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"mat, material properties\n";
    behaviourDataFile << " * \\param const Type *const "+iprefix+"int_vars, state variables\n"; 
    behaviourDataFile << " * \\param const Type *const "+iprefix+"ext_vars, external state variables\n";
    behaviourDataFile << " */\n";
    behaviourDataFile << className << "BehaviourData"
		      << "(const Type* const "+iprefix+"stress_,const Type* const "+iprefix+"stran,\n" 
		      <<  "const Type* const "+iprefix+"T_,const Type* const";
    if(!coefsHolder.empty()){
      behaviourDataFile << " "+iprefix+"mat,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile <<  "const Type* const";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << " "+iprefix+"int_vars,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile << "const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourDataFile << " "+iprefix+"ext_vars";
    }
    behaviourDataFile << ")\n";
    behaviourDataFile << ": T(*"+iprefix+"T_)";
    SupportedTypes::TypeSize o;
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       coefsHolder,
							       iprefix+"mat","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
								   stateVarsHolder,
								   iprefix+"int_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       auxiliaryStateVarsHolder,
							       iprefix+"int_vars","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       externalStateVarsHolder,
							       iprefix+"ext_vars","","");
    behaviourDataFile << "\n{\n";
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    SupportedTypes::TypeSize of;
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const DrivingVariable&    v = pm->first;
      const ThermodynamicForce& f = pm->second;
      if(this->getTypeFlag(f.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "tfel::fsalgo::copy<N>("+iprefix+"stress_+" << of <<",this->" << f.name << ".begin());\n";
	} else {
	  behaviourDataFile << "tfel::fsalgo::copy<N>("+iprefix+"stress_,this->" << f.name << ".begin());\n";
	}
      } else if(this->getTypeFlag(f.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "this->" << f.name << ".importTab("+iprefix+"stress_+" << of <<");\n";
	} else {
	  behaviourDataFile << "this->" << f.name << ".importTab("+iprefix+"stress_);\n";
	}
      } else if(this->getTypeFlag(f.type)==SupportedTypes::Tensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "tfel::fsalgo::copy<TensorSize>("+iprefix+"stress_+" << of <<",this->" << f.name << ".begin());\n";
	} else {
	  behaviourDataFile << "tfel::fsalgo::copy<TensorSize>("+iprefix+"stress_,this->" << f.name << ".begin());\n";
	}
      } else {
	string msg("MFrontUMATInterfaceBase::writeBehaviourDataConstructor : ");
	msg += "unsupported forces type";
	throw(runtime_error(msg));
      }
      if(this->getTypeFlag(f.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "tfel::fsalgo::copy<N>("+iprefix+"stran_+" << ov <<",this->" << v.name << ".begin());\n";
	} else {
	  behaviourDataFile << "tfel::fsalgo::copy<N>("+iprefix+"stran_,this->" << v.name << ".begin());\n";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "this->" << v.name << ".importVoigt("+iprefix+"stran+" << ov <<");\n";
	} else {
	  behaviourDataFile << "this->" << v.name << ".importVoigt("+iprefix+"stran);\n";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "tfel::fsalgo::copy<TensorSize>("+iprefix+"stran_+" << ov <<",this->" << v.name << ".begin());\n";
	} else {
	  behaviourDataFile << "tfel::fsalgo::copy<TensorSize>("+iprefix+"stran_,this->" << v.name << ".begin());\n";
	}
      } else {
	string msg("MFrontUMATInterfaceBase::writeBehaviourDataConstructor : ");
	msg += "unsupported forces type";
	throw(runtime_error(msg));
      }
      ov += this->getTypeSize(v.type,1u);
      of += this->getTypeSize(f.type,1u);
    }
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								coefsHolder,
								iprefix+"mat","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								    stateVarsHolder,
								    iprefix+"int_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								auxiliaryStateVarsHolder,
								iprefix+"int_vars","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								externalStateVarsHolder,
								iprefix+"ext_vars","","");
    behaviourDataFile << "}\n\n";
  }
  
  void 
  MFrontUMATInterfaceBase::writeIntegrationDataConstructor(std::ofstream& behaviourIntegrationFile,
							   const std::string& className,
							   const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    const VarContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    behaviourIntegrationFile << "/*\n";
    behaviourIntegrationFile << " * \\brief constructor for the umat interface\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dt_, time increment\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dstran, strain increment tensor\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dT_, temperature increment\n";
    behaviourIntegrationFile << " * \\param const Type *const "+iprefix+"dext_vars,";
    behaviourIntegrationFile << " external state variables increments\n";
    behaviourIntegrationFile << " *\n";
    behaviourIntegrationFile << " */\n";
    behaviourIntegrationFile << className << "IntegrationData"
			     << "(const Type* const "+iprefix+"dt_,const Type* const "+iprefix+"dstran,\n" 
			     <<  "const Type* const "+iprefix+"dT_,const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourIntegrationFile << " "+iprefix+"dext_vars)\n";
    } else {
      behaviourIntegrationFile << ")\n";
    }
    behaviourIntegrationFile << ": dt(*"+iprefix+"dt_),dT(*"+iprefix+"dT_)";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(behaviourIntegrationFile,
								 externalStateVarsHolder,
								 iprefix+"dext_vars","d","");
    }
    behaviourIntegrationFile << "\n{\n";
    map<DrivingVariable,
      ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const DrivingVariable&    v = pm->first;
      if(!v.increment_known){
	string msg("MFrontUMATInterfaceBase::writeIntegrationDataConstructor : ");
	msg += "unsupported driving variable '"+v.name+"'";
	throw(runtime_error(msg));
      }
      if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourIntegrationFile << "tfel::fsalgo::copy<N>("+iprefix+"dstran+" << ov <<",this->" << v.name << ".begin());\n";
	} else {
	  behaviourIntegrationFile << "tfel::fsalgo::copy<N>("+iprefix+"dstran,this->" << v.name << ".begin());\n";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourIntegrationFile << "d" << v.name << ".importVoigt("+iprefix+"dstran);\n";
	} else {
	  behaviourIntegrationFile << "d" << v.name << ".importVoigt("+iprefix+"dstran+" << ov << ");\n";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourIntegrationFile << "tfel::fsalgo::copy<TensorSize>("+iprefix+"dstran+" << ov <<",this->" << v.name << ".begin());\n";
	} else {
	  behaviourIntegrationFile << "tfel::fsalgo::copy<TensorSize>("+iprefix+"dstran,this->" << v.name << ".begin());\n";
	}
      } else {
	string msg("MFrontUMATInterfaceBase::writeIntegrationDataConstructor : ");
	msg += "unsupported driving variable '"+v.name+"'";
	throw(runtime_error(msg));
      }
      ov += this->getTypeSize(v.type,1u);
    }
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorII(behaviourIntegrationFile,
								  externalStateVarsHolder,
								  iprefix+"dext_vars","d","");
    }
    behaviourIntegrationFile << "}\n\n";
  }

  std::vector<std::string>
  MFrontUMATInterfaceBase::getGlossaryNames(const VarContainer& v,
					    const std::map<std::string,std::string>& glossaryNames,
					    const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    vector<string> n;
    this->appendGlossaryNames(n,v,glossaryNames,entryNames);
    return n;
  }

  void
  MFrontUMATInterfaceBase::appendGlossaryNames(std::vector<std::string>& n,
					       const VarContainer& v,
					       const std::map<std::string,std::string>& glossaryNames,
					       const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      const string name = MFrontUMATInterfaceBase::getGlossaryName(glossaryNames,entryNames,p->name);
      if(p->arraySize==1u){
	n.push_back(name);
      } else {
	for(unsigned short i=0;i!=p->arraySize;++i){
	  ostringstream nb;
	  nb << '[' << i << ']';
	  n.push_back(name+nb.str());
	}
      }
    }
  } // end of MFrontUMATInterfaceBase::appendGlossaryNames

  void
  MFrontUMATInterfaceBase::writeGlossaryNames(std::ostream& f,
					      const std::vector<std::string>& n,
					      const std::string& name,
					      const std::string& array,
					      const unsigned short o) const
  {
    using namespace std;
    if(o>n.size()){
      string msg("MFrontUMATInterfaceBase::writeGlossaryNames : ");
      msg += "number of names given is lesser than the offset";
      throw(runtime_error(msg));
    }
    if(n.size()!=o){
      vector<string>::size_type s = 0u;
      vector<string>::const_iterator p = n.begin()+o;      
      f << "MFRONT_SHAREDOBJ const char * " << this->getFunctionName(name)
	<< "_" << array << "[" << n.size()-o <<  "] = {";
      while(p!=n.end()){
	f << '"' << *p << '"';
	if(++p!=n.end()){
	  if(s%5==0){
	    f << ",\n";
	  } else {
	    f << ",";
	  }
	}
	++s;
      }
      f << "};\n";
    } else {
      f << "MFRONT_SHAREDOBJ const char * const * "  << this->getFunctionName(name)
	<< "_" << array << " = 0;\n\n";
    }      
  } // end of MFrontUMATInterfaceBase::writeGlossaryNames

  bool
  MFrontUMATInterfaceBase::readBooleanValue(const std::string& key,
					    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator& current,
					    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end) const
  {
    using namespace std;
    bool b = true;
    if(current==end){
      string msg("MFrontUmatInterfaceBase::readBooleanValue ("+key+") : ");
      msg += "unexpected end of file";
      throw(runtime_error(msg));
    }
    if(current->value=="true"){
      b = true;
    } else if(current->value=="false"){
      b = false;
    } else {
      string msg("MFrontUmatInterfaceBase::readBooleanValue ("+key+") :");
      msg += "expected 'true' or 'false'";
      throw(runtime_error(msg));
    }
    ++(current); 
    if(current==end){
      string msg("MFrontUmatInterfaceBase::readBooleanValue ("+key+") : ");
      msg += "unexpected end of file";
      throw(runtime_error(msg));
    }    
    if(current->value!=";"){
      string msg("MFrontUmatInterfaceBase::readBooleanValue : expected ';', read ");
      msg += current->value;
      throw(runtime_error(msg));
    }
    ++(current);
    return b;
  }

  void
  MFrontUMATInterfaceBase::writeVisibilityDefines(std::ostream& out) const
  {
    out << "#ifdef WIN32\n";
    out << "#include <windows.h>\n";
    out << "#ifndef MFRONT_STDCALL\n";
    out << "#define MFRONT_STDCALL __stdcall\n"; 
    out << "#endif /* MFRONT_STDCALL */\n"; 
    out << "#else\n";
    out << "#ifndef MFRONT_STDCALL\n";
    out << "#define MFRONT_STDCALL\n"; 
    out << "#endif /* MFRONT_STDCALL */\n"; 
    out << "#endif /* WIN32 */\n\n";
    out << "#ifndef MFRONT_SHAREDOBJ\n";
    out << "#define MFRONT_SHAREDOBJ TFEL_VISIBILITY_EXPORT\n"; 
    out << "#endif /* MFRONT_SHAREDOBJ */\n\n"; 
  } // end of MFrontUMATInterfaceBase::writeVisibilityDefines

  void
  MFrontUMATInterfaceBase::checkParametersType(bool& rp,
					       bool& ip,
					       bool& up,
					       const VarContainer& pc) const
  {
    using namespace std;
    VarContainer::const_iterator pp;
    rp = false;
    ip = false;
    up = false;
    for(pp=pc.begin();pp!=pc.end();++pp){
      if(pp->type=="real"){
	rp = true;
      } else if(pp->type=="int"){
	ip = true;
      } else if(pp->type=="ushort"){
	up = true;
      } else {
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "unsupport parameter type '"+pp->type+"'.\n";
	throw(runtime_error(msg));
      } 
    }
  }
  
  void
  MFrontUMATInterfaceBase::writeSetParametersFunctionsDeclarations(std::ostream& out,
								   const std::string& name,
								   const MechanicalBehaviourDescription& mb) const
  {
    const VarContainer& pc = mb.getParameters();
    bool rp,ip,up;
    this->checkParametersType(rp,ip,up,pc);
    if(rp){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	  << this->getFunctionName(name)
	  << "_setParameter(const char *const,const double);\n\n";
    }
    if(ip){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	  << this->getFunctionName(name)
	  << "_setIntegerParameter(const char *const,const int);\n\n";
    }
    if(up){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	  << this->getFunctionName(name)
	  << "_setIntegerParameter(const char *const,const unsigned short);\n\n";
    }
  }

  void
  MFrontUMATInterfaceBase::writeSetParametersFunctionsImplantations(std::ostream& out,
								    const std::string& name,
								    const std::string& className,
								    const MechanicalBehaviourDescription& mb) const
  {
    const VarContainer& pc = mb.getParameters();
    bool rp,ip,up;
    this->checkParametersType(rp,ip,up,pc);
    if(rp){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	  << this->getFunctionName(name)
	  << "_setParameter(const char *const key,const double value){\n"
	  << "using namespace std;\n"
	  << "using namespace tfel::material;\n"
	  << className << "ParametersInitializer& i = " << className << "ParametersInitializer::get();\n"
	  << "try{\n"
	  << "i.set(key,value);\n"
	  << "} catch(runtime_error& e){"
	  << "cerr << e.what() << endl;\n"
	  << "return 0;\n"
	  << "}\n"
	  << "return 1;\n"
	  << "}\n\n";
    }
    if(ip){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	  << this->getFunctionName(name)
	  << "_setIntegerParameter(const char *const key,const int value){\n"
	  << "using namespace std;\n"
	  << "using namespace tfel::material;\n"
	  << className << "ParametersInitializer& i = " << className << "ParametersInitializer::get();\n"
	  << "try{\n"
	  << "i.set(key,value);\n"
	  << "} catch(runtime_error& e){"
	  << "cerr << e.what() << endl;\n"
	  << "return 0;\n"
	  << "}\n"
	  << "return 1;\n"
	  << "}\n\n";
    }
    if(up){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\n"
	  << this->getFunctionName(name)
	  << "_setUnsignedShortParameter(const char *const key,const unsigned short value){\n"
	  << "using namespace std;\n"
	  << "using namespace tfel::material;\n"
	  << className << "ParametersInitializer& i = " << className << "ParametersInitializer::get();\n"
	  << "try{\n"
	  << "i.set(key,value);\n"
	  << "} catch(runtime_error& e){"
	  << "cerr << e.what() << endl;\n"
	  << "return 0;\n"
	  << "}\n"
	  << "return 1;\n"
	  << "}\n\n";
    }
  }

  std::string
  MFrontUMATInterfaceBase::getHeaderDefine(const std::string& library,
					   const std::string& material,
					   const std::string& className) const
  {
    using namespace std;
    string header = "_LIB_"+makeUpperCase(this->getInterfaceName());
    if(!library.empty()){
      header += "_";
      header += makeUpperCase(library);
    }
    if(!material.empty()){
      header += "_";
      header += makeUpperCase(material);
    }
    header += "_";
    header += makeUpperCase(className);
    header += "_HXX";
    return header;
  }

  void
  MFrontUMATInterfaceBase::getExtraSrcIncludes(std::ostream& out,
					       const MechanicalBehaviourDescription& mb)
  {
    const VarContainer& parametersHolder         = mb.getParameters();
    if((!parametersHolder.empty())||(this->debugMode)){
      out << "#include<iostream>\n";
    }
    if(!parametersHolder.empty()){
      out << "#include<stdexcept>\n";
    }
    if(this->generateMTestFile){
      out << "#include<vector>\n";
      out << "#include<sstream>\n";
      out << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n";
      out << "#include\"MFront/SupportedTypes.hxx\"\n";
      out << "#include\"MFront/UMATSmallStrainMTestFileGenerator.hxx\"\n";
    }
  } // end of MFrontUMATInterfaceBase::getExtraSrcIncludes
  
  void
  MFrontUMATInterfaceBase::generateMTestFile1(std::ostream& out) const
  {
    if(this->generateMTestFile){
      out << "using namespace std;\n";
      out << "using tfel::material::ModellingHypothesis;\n";
      out << "using mfront::SupportedTypes;\n";
      out << "// double is used by MTestFileGeneratorBase\n";
      out << "vector<double> mg_STRESS(*NTENS);\n";
      out << "vector<double> mg_NSTATV(*NSTATV);\n";
      out << "copy(STRESS,STRESS+*NTENS,mg_STRESS.begin());\n";
      out << "copy(STATEV,STATEV+*NSTATV,mg_NSTATV.begin());\n";
    }
  } // end of MFrontUMATInterfaceBase::generateMTestFile1

  void
  MFrontUMATInterfaceBase::generateMTestFile2(std::ostream& out,
					      const std::string& library,
					      const std::string& material,
					      const std::string& name,
					      const MechanicalBehaviourDescription& mb,
					      const std::map<std::string,std::string>& glossaryNames,
					      const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    if(this->generateMTestFile){
      const VarContainer& coefsHolder              = mb.getMaterialProperties();
      const VarContainer& stateVarsHolder          = mb.getStateVariables();
      const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
      const VarContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
      VarContainer::const_iterator p;
      const bool mpoffset = this->hasMaterialPropertiesOffset(mb);
      unsigned short i;
      unsigned int offset;
      out << "mfront::UMATSmallStrainMTestFileGenerator mg(\""
	  << makeLowerCase(this->getInterfaceName()) << "\",\""
	  << this->getLibraryName(library,material) <<"\",\"" << this->getFunctionName(name)
	  <<  "\");\n";
      this->writeMTestFileGeneratorSetModellingHypothesis(out);
      out << "const unsigned short TVectorSize = mg.getTVectorSize();\n";
      out << "const unsigned short StensorSize = mg.getStensorSize();\n";
      out << "const unsigned short TensorSize  = mg.getTensorSize();\n";
      out << "static_cast<void>(TVectorSize); // remove gcc warning\n";
      out << "static_cast<void>(StensorSize); // remove gcc warning\n";
      out << "static_cast<void>(TensorSize);  // remove gcc warning\n";
      if(mpoffset){
	out << "unsigned short mg_mpoffset;" << endl;
      }
      out << "mg.addTime(0.);\n";
      out << "mg.addTime(*DTIME);\n";
      out << "mg.setStrainTensor(STRAN);\n";
      out << "mg.setStrainTensorIncrement(DSTRAN);\n";
      out << "mg.setStressTensor(&mg_STRESS[0]);\n";
      this->writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(out,mb);
      for(p=coefsHolder.begin(),offset=0;p!=coefsHolder.end();++p){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	if(flag!=SupportedTypes::Scalar){
	  string msg("MFrontUMATInterfaceBase::generateFile2 : "
		     "unsupported external state variable type "
		     "in mtest file generation");
	  throw(runtime_error(msg));
	}
	const string mpname = MFrontUMATInterfaceBase::getGlossaryName(glossaryNames,entryNames,p->name);
	if(p->arraySize==1u){
	  if(mpoffset){
	    if(offset==0){
	      out << "mg.addMaterialProperty(\"" << mpname << "\",*(PROPS+mg_mpoffset));\n";
	    } else {
	      out << "mg.addMaterialProperty(\"" << mpname << "\",*(PROPS+mg_mpoffset+" << offset<< "));\n";
	    }
	  } else {
	    if(offset==0){
	      out << "mg.addMaterialProperty(\"" << mpname << "\",*PROPS);\n";
	    } else {
	      out << "mg.addMaterialProperty(\"" << mpname << "\",*(PROPS+" << offset<< "));\n";
	    }
	  }
	  ++offset;
	} else {
	  if(p->arraySize>=SupportedTypes::ArraySizeLimit){
	    out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
	    out << "ostringstream name;\n";
	    out << "name << \"" << mpname << "[\" << i << \"]\";\n";
	    if(mpoffset){
	      if(offset==0){
		out << "mg.addMaterialProperty(name.str(),*(PROPS+mg_mpoffset+i));\n";
	      } else {
		out << "mg.addMaterialProperty(name.str(),*(PROPS+mg_mpoffset+" << offset<< "+i));\n";
	      }
	      out << "}\n";
	    } else {
	      if(offset==0){
		out << "mg.addMaterialProperty(name.str(),*(PROPS+i));\n";
	      } else {
		out << "mg.addMaterialProperty(name.str(),*(PROPS+" << offset<< "+i));\n";
	      }
	      out << "}\n";
	    }
	    offset += p->arraySize;
	  } else {
	    for(i=0;i!=p->arraySize;++i,++offset){
	       if(mpoffset){
		 if(offset==0){
		   out << "mg.addMaterialProperty(\"" << mpname
		       << "[" << i << "]\",*(PROPS+mg_mpoffset));\n";
		 } else {
		   out << "mg.addMaterialProperty(\""
		       << mpname << "[" << i << "]\",*(PROPS+mg_mpoffset+" << offset<< "));\n";
		 }
	       } else {
		 if(offset==0){
		   out << "mg.addMaterialProperty(\"" << mpname
		       << "[" << i << "]\",*PROPS);\n";
		 } else {
		   out << "mg.addMaterialProperty(\""
		       << mpname << "[" << i << "]\",*(PROPS+" << offset<< "));\n";
		 }
	       }
	    }
	  }
	}
      }
      SupportedTypes::TypeSize ivoffset;
      for(p=stateVarsHolder.begin();p!=stateVarsHolder.end();++p){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	const string ivname = MFrontUMATInterfaceBase::getGlossaryName(glossaryNames,entryNames,p->name);
	if(p->arraySize==1u){
	  if(ivoffset.isNull()){
	    if(flag==SupportedTypes::Scalar){
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Scalar,STATEV);\n";
	      ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
	    } else {
	      ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Stensor,STATEV);\n";
	    }
	  } else {
	    if(flag==SupportedTypes::Scalar){
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Scalar,STATEV+" << ivoffset<< ");\n";
	      ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
	    } else {
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Stensor,STATEV+" << ivoffset<< ");\n";
	      ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
	    }
	  }
	} else {
	  if(p->arraySize>=SupportedTypes::ArraySizeLimit){
	    out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
	    out << "ostringstream name;\n";
	    out << "name << \"" << ivname << "[\" << i << \"]\";\n";
	    if(ivoffset.isNull()){
	      if(flag==SupportedTypes::Scalar){
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Scalar,STATEV+i);\n";
	      } else {
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Stensor,STATEV+i);\n";
	      }
	    } else {
	      if(flag==SupportedTypes::Scalar){
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Scalar,STATEV+" << ivoffset<< "+i);\n";
	      } else {
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Stensor,STATEV+" << ivoffset<< "+i);\n";
	      }
	    }
	    out << "}\n";
	    if(flag==SupportedTypes::Scalar){
	      ivoffset += SupportedTypes::TypeSize(p->arraySize,0u,0u,0u);
	    } else {
	      ivoffset += SupportedTypes::TypeSize(0u,0u,p->arraySize,0u);
	    }
	  } else {
	    for(i=0;i!=p->arraySize;++i){
	      if(ivoffset.isNull()){
		if(flag==SupportedTypes::Scalar){
		  out << "mg.addInternalStateVariable(\"" << ivname
		      << "[" << i << "]\",SupportedTypes::Scalar,STATEV);\n";
		  ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
		} else {
		  out << "mg.addInternalStateVariable(\"" << ivname
		      << "[" << i << "]\",SupportedTypes::Stensor,STATEV);\n";
		  ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		}
	      } else {
		if(flag==SupportedTypes::Scalar){
		  out << "mg.addInternalStateVariable(\""
		      << ivname << "[" << i << "]\",SupportedTypes::Scalar,STATEV+" << ivoffset<< ");\n";
		  ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
		} else {
		  out << "mg.addInternalStateVariable(\""
		      << ivname << "[" << i << "]\",SupportedTypes::Stensor,STATEV+" << ivoffset<< ");\n";
		  ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		}
	      }
	    }
	  }
	}
      }
      for(p=auxiliaryStateVarsHolder.begin();p!=auxiliaryStateVarsHolder.end();++p){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	const string ivname = MFrontUMATInterfaceBase::getGlossaryName(glossaryNames,entryNames,p->name);
	if(p->arraySize==1u){
	  if(ivoffset.isNull()){
	    if(flag==SupportedTypes::Scalar){
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Scalar,STATEV);\n";
	      ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
	    } else {
	      ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Stensor,STATEV);\n";
	    }
	  } else {
	    if(flag==SupportedTypes::Scalar){
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Scalar,STATEV+" << ivoffset<< ");\n";
	      ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
	    } else {
	      out << "mg.addInternalStateVariable(\"" << ivname << "\",SupportedTypes::Stensor,STATEV+" << ivoffset<< ");\n";
	      ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
	    }
	  }
	} else {
	  if(p->arraySize>=SupportedTypes::ArraySizeLimit){
	    out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
	    out << "ostringstream name;\n";
	    out << "name << \"" << ivname << "[\" << i << \"]\";\n";
	    if(ivoffset.isNull()){
	      if(flag==SupportedTypes::Scalar){
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Scalar,STATEV+i);\n";
	      } else {
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Stensor,STATEV+i);\n";
	      }
	    } else {
	      if(flag==SupportedTypes::Scalar){
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Scalar,STATEV+" << ivoffset<< "+i);\n";
	      } else {
		out << "mg.addInternalStateVariable(name.str(),SupportedTypes::Stensor,STATEV+" << ivoffset<< "+i);\n";
	      }
	    }
	    out << "}\n";
	    if(flag==SupportedTypes::Scalar){
	      ivoffset += SupportedTypes::TypeSize(p->arraySize,0u,0u,0u);
	    } else {
	      ivoffset += SupportedTypes::TypeSize(0u,0u,p->arraySize,0u);
	    }
	  } else {
	    for(i=0;i!=p->arraySize;++i){
	      if(ivoffset.isNull()){
		if(flag==SupportedTypes::Scalar){
		  out << "mg.addInternalStateVariable(\"" << ivname
		      << "[" << i << "]\",SupportedTypes::Scalar,STATEV);\n";
		  ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
		} else {
		  out << "mg.addInternalStateVariable(\"" << ivname
		      << "[" << i << "]\",SupportedTypes::Stensor,STATEV);\n";
		  ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		}
	      } else {
		if(flag==SupportedTypes::Scalar){
		  out << "mg.addInternalStateVariable(\""
		      << ivname << "[" << i << "]\",SupportedTypes::Scalar,STATEV+" << ivoffset<< ");\n";
		  ivoffset += SupportedTypes::TypeSize(1u,0u,0u,0u);
		} else {
		  out << "mg.addInternalStateVariable(\""
		      << ivname << "[" << i << "]\",SupportedTypes::Stensor,STATEV+" << ivoffset<< ");\n";
		  ivoffset += SupportedTypes::TypeSize(0u,0u,1u,0u);
		}
	      }
	    }
	  }
	}
      }
      out << "mg.addExternalStateVariableValue(\"Temperature\",0.,*TEMP);\n";
      out << "mg.addExternalStateVariableValue(\"Temperature\",*DTIME,*TEMP+*DTEMP);\n";
      for(p=externalStateVarsHolder.begin(),offset=0;p!=externalStateVarsHolder.end();++p){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	if(flag!=SupportedTypes::Scalar){
	  string msg("MFrontUMATInterfaceBase::generateFile2 : "
		     "unsupported external state variable type "
		     "in mtest file generation");
	  throw(runtime_error(msg));
	}
	const string evname = MFrontUMATInterfaceBase::getGlossaryName(glossaryNames,entryNames,p->name);
	if(p->arraySize==1u){
	  if(offset==0){
	    out << "mg.addExternalStateVariableValue(\"" << evname 
		<< "\",0,*PREDEF);\n";
	    out << "mg.addExternalStateVariableValue(\"" << evname 
		<< "\",*DTIME,*PREDEF+*DPRED);\n";
	  } else {
	    out << "mg.addExternalStateVariableValue(\"" << evname 
		<< "\",0,*(PREDEF+" << offset<< "));\n";
	    out << "mg.addExternalStateVariableValue(\"" << evname 
		<< "\",*DTIME,*(PREDEF+" << offset<< ")+*(DPRED+" << offset<< "));\n";
	  }
	  ++offset;
	} else {
	  if(p->arraySize>=SupportedTypes::ArraySizeLimit){
	    out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
	    out << "ostringstream name;\n";
	    out << "name << \"" << evname << "[\" << i << \"]\";\n";
	    if(offset==0){
	      out << "mg.addExternalStateVariableValue(name.str(),0,*(PREDEF+i));\n";
	      out << "mg.addExternalStateVariableValue(name.str(),"
		"*DTIME,*(PREDEF+i)+*(DPRED+i));\n";
	    } else {
	      out << "mg.addExternalStateVariableValue(name.str(),"
		"0,*(PREDEF+" << offset<< "+i));\n";
	      out << "mg.addExternalStateVariableValue(name.str(),"
		"*DTIME,*(PREDEF+" << offset << "+i)+*(DPRED+" << offset<< "+i));\n";
	    }
	    out << "}\n";
	    offset += p->arraySize;
	  } else {
	    for(i=0;i!=p->arraySize;++i,++offset){
	      if(offset==0){
		out << "mg.addExternalStateVariableValue(\"" << evname
		    << "[" << i << "]\",0,*PREDEF);\n";
		out << "mg.addExternalStateVariableValue(\"" << evname
		    << "[" << i << "]\",*DTIME,*PREDEF+*DPRED);\n";
	      } else {
		out << "mg.addExternalStateVariableValue(\""
		    << evname << "[" << i << "]\","
		  "0,*(PREDEF+" << offset<< "));\n";
		out << "mg.addExternalStateVariableValue(\""
		    << evname << "[" << i << "]\","
		  "*DTIME,*(PREDEF+" << offset<< ")+*(DPRED+" << offset<< "));\n";
	      }
	    }
	  }
	}
      }
      out << "mg.generate(\""+name+"\");\n";
    }
  }

  void
  MFrontUMATInterfaceBase::generateUMATxxSymbols(std::ostream& out,
						 const std::string& name,
						 const std::string& file,
						 const MechanicalBehaviourDescription& mb,
						 const std::map<std::string,std::string>& glossaryNames,
						 const std::map<std::string,std::string>& entryNames) const
  {
    this->writeUMATxxSourceFileSymbols(out,name,file,mb);
    this->writeUMATxxMaterialPropertiesSymbols(out,name,mb,glossaryNames,entryNames);
    this->writeUMATxxStateVariablesSymbols(out,name,mb,glossaryNames,entryNames);
    this->writeUMATxxExternalStateVariablesSymbols(out,name,mb,glossaryNames,entryNames);
    this->writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(out,name,mb);
    this->writeUMATxxBehaviourTypeSymbols(out,name,mb);
    this->writeUMATxxElasticBehaviourTypeSymbols(out,name,mb);
    this->writeUMATxxAdditionalSymbols(out,name,file,mb,glossaryNames,entryNames);
  }

  void
  MFrontUMATInterfaceBase::writeUMATxxStateVariablesSymbols(std::ostream& out,
							    const std::string& name,
							    const MechanicalBehaviourDescription& mb,
							    const std::map<std::string,std::string>& glossaryNames,
							    const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    const VarContainer& stateVarsHolder          = mb.getStateVariables();
    const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const unsigned short nStateVariables = static_cast<unsigned short>(this->getNumberOfVariables(stateVarsHolder) + 
								       this->getNumberOfVariables(auxiliaryStateVarsHolder));
    VarContainer::const_iterator p;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	<< "_nInternalStateVariables = " << nStateVariables
	<< ";\n";
    vector<string> stateVariablesNames = this->getGlossaryNames(stateVarsHolder,
								glossaryNames,
								entryNames);
    this->appendGlossaryNames(stateVariablesNames,auxiliaryStateVarsHolder,
			      glossaryNames,entryNames);
    this->writeGlossaryNames(out,stateVariablesNames,name,"InternalStateVariables");

    if((stateVarsHolder.size()!=0)||
       (auxiliaryStateVarsHolder.size()!=0)){
      out << "MFRONT_SHAREDOBJ int " << this->getFunctionName(name)
	  << "_InternalStateVariablesTypes [] = {";
      for(p=stateVarsHolder.begin();p!=stateVarsHolder.end();){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	for(unsigned short is=0;is!=p->arraySize;){
	  switch(flag){
	  case SupportedTypes::Scalar : 
	    out << 0;
	    break;
	  case SupportedTypes::Stensor :
	    out << 1;
	    break;
	  case SupportedTypes::TVector :
	    out << 2;
	    break;
	  case SupportedTypes::Tensor :
	    out << 3;
	    break;
	  default :
	    string msg("MFrontUMATInterfaceBase::writeUMATxxStateVariablesSymbols : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	  if(++is!=p->arraySize){
	    out << ",";
	  }
	}
	if(++p!=stateVarsHolder.end()){
	  out << ",";
	}
      }
      if((!stateVarsHolder.empty())&&
	 (auxiliaryStateVarsHolder.size()!=0)){
	out << ",";
      }
      for(p=auxiliaryStateVarsHolder.begin();p!=auxiliaryStateVarsHolder.end();){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	for(unsigned short is=0;is!=p->arraySize;){
	  switch(flag){
	  case SupportedTypes::Scalar : 
	    out << 0;
	    break;
	  case SupportedTypes::Stensor :
	    out << 1;
	    break;
	  default :
	    string msg("MFrontUMATInterfaceBase::writeUMATxxStateVariablesSymbols : ");
	    msg += "internal error, tag unsupported";
	    throw(runtime_error(msg));
	  }
	  if(++is!=p->arraySize){
	    out << ",";
	  }
	}
	if(++p!=auxiliaryStateVarsHolder.end()){
	  out << ",";
	}
      }
      out << "};\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ const int * " << this->getFunctionName(name)
	  << "_InternalStateVariablesTypes = 0;\n\n";
    }
  } // end of 
  
  void
  MFrontUMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols(std::ostream& out,
								    const std::string& name,
								    const MechanicalBehaviourDescription& mb,
								    const std::map<std::string,std::string>& glossaryNames,
								    const std::map<std::string,std::string>& entryNames) const
  {
    const VarContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	<< "_nExternalStateVariables = " << this->getNumberOfVariables(externalStateVarsHolder) << ";\n";
    this->writeGlossaryNames(out,this->getGlossaryNames(externalStateVarsHolder,
							glossaryNames,
							entryNames),
			     name,"ExternalStateVariables");
  } // end of MFrontUMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(std::ostream& out,
										const std::string& name,
										const MechanicalBehaviourDescription& mb) const
  {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	<< "_UsableInPurelyImplicitResolution = ";
    if(mb.isUsableInPurelyImplicitResolution()){
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxIsUsableInPurelyImplicitResolution

  void
  MFrontUMATInterfaceBase::writeUMATxxBehaviourTypeSymbols(std::ostream& out,
						    const std::string& name,
						    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourType = " ;
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else if(mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "1u;" << endl << endl;
    } else {
      string msg("MFrontUMATInterfaceBase::writeUMATxxBehaviourTypeSymbols : ");
      msg += "unsupported behaviour type.\n";
      msg += "only isotropic or orthotropic behaviours are supported at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxBehaviourTypeSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxElasticBehaviourTypeSymbols(std::ostream& out,
								  const std::string& name,
								  const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	<< "_ElasticBehaviourType = " ;
    if(mb.getElasticBehaviourType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else if(mb.getElasticBehaviourType()==mfront::ORTHOTROPIC){
      out << "1u;" << endl << endl;
    } else {
      string msg("MFrontUMATInterfaceBase::writeUMATxxElasticBehaviourTypeSymbols : ");
      msg += "unsupported behaviour type.\n";
      msg += "only isotropic or orthotropic behaviours are supported at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontUMATInterfaceBase::writeUMATxxElasticBehaviourTypeSymbols

  void
  MFrontUMATInterfaceBase::writeUMATxxSourceFileSymbols(std::ostream& out,
							const std::string& name,
							const std::string& file,
							const MechanicalBehaviourDescription& mb) const
  {
    using namespace tfel::system;
    using namespace tfel::utilities;
    out << "MFRONT_SHAREDOBJ const char *\n" 
	<< this->getFunctionName(name) << "_src = \""
	<< tokenize(file,dirSeparator()).back()
	<< "\";\n\n";
  }

  MFrontUMATInterfaceBase::~MFrontUMATInterfaceBase()
  {}

} // end of namespace mfront
