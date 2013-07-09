/*!
 * \file   MFrontAsterInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<iostream>
#include<iterator>
#include<algorithm>

#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontAsterInterface.hxx"

namespace mfront{

  static const std::string&
  MFrontAsterInterfaceGetName(const std::map<std::string,std::string>& glossaryNames,
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
  } // end of MFrontAsterInterfaceGetName

  std::string
  MFrontAsterInterface::getName(void)
  {
    return "aster";
  }

  std::string
  MFrontAsterInterface::getLibraryName(const std::string& library,
				      const std::string& material)
  {
    using namespace std;
    string lib;
    if(library.empty()){
      if(!material.empty()){
	lib = "libAster"+material;
      } else {
	lib = "libAsterBehaviour";
      }
    } else {
      lib = "libAster"+library;
    }
    return lib;
  } // end of MFrontAsterInterface::getLibraryName

  MFrontAsterInterface::MFrontAsterInterface()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {
    this->reset();
  }

  void
  MFrontAsterInterface::reset(void)
  {
    SupportedTypes::reset();
    this->compareToNumericalTangentOperator  = false;
    this->strainPerturbationValue            = 1.e-6;
    this->tangentOperatorComparisonCriterium = 1.e7;
    this->savesTangentOperator               = false;
    this->errorReport                        = true;
  }
  
  void 
  MFrontAsterInterface::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontAsterInterface::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontAsterInterface::setDebugMode(void)
  {
    this->debugMode = true;
  }

  void
  MFrontAsterInterface::allowDynamicallyAllocatedArrays(const bool b)
  {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  } // end of MFrontAsterInterface::allowDynamicallyAllocatedArrays

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontAsterInterface::treatKeyword(const std::string& key,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    if(key=="@AsterCompareToNumericalTangentOperator"){
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterCompareToNumericalTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->compareToNumericalTangentOperator = true;
      } else if(current->value=="false"){
	this->compareToNumericalTangentOperator = false;
      } else {
	string msg("AsterInterface::treatKeyword (@AsterCompareToNumericalTangentOperator) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterCompareToNumericalTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }     
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@AsterTangentOperatorComparisonCriterium"){
      if(!this->compareToNumericalTangentOperator){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : "
		   "comparison to tangent operator is not enabled at this stage.\n"
		   "Use the @AsterCompareToNumericalTangentOperator directive before "
		   "@AsterTangentOperatorComparisonCriterium");
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->tangentOperatorComparisonCriterium;
      if(flux.fail()){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : ");
	msg+="failed to read criterium value.\n";
	throw(runtime_error(msg));
      }
      ++(current);    
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterTangentOperatorComparisonCriterium) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }  
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@AsterStrainPerturbationValue"){
      if(!this->compareToNumericalTangentOperator){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg += "time stepping is not enabled at this stage.\n";
	msg += "Use the @AsterUseTimeSubStepping directive before ";
	msg += "@AsterStrainPerturbationValue";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->strainPerturbationValue;
      if(flux.fail()){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg+="failed to read string perturbation value.\n";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterStrainPerturbationValue) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if(key=="@AsterSaveTangentOperator"){
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterSaveTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->savesTangentOperator = true;
      } else if(current->value=="false"){
	this->savesTangentOperator = false;
      } else {
	string msg("AsterInterface::treatKeyword (@AsterSaveTangentOperator) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterSaveTangentOperator) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if(key=="@AsterErrorReport"){
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterErrorReport) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->errorReport = true;
      } else if(current->value=="false"){
	this->errorReport = false;
      } else {
	string msg("AsterInterface::treatKeyword (@AsterErrorReport) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("AsterInterface::treatKeyword (@AsterErrorReport) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("AsterInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    }
    return make_pair(false,current);
  } // end of treatKeyword

  void 
  MFrontAsterInterface::exportMechanicalData(std::ofstream& behaviourDataFile,
					    const std::string&,
					    const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const VarContainer&   stateVarsHolder        = mb.getStateVariables();
    const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    VarContainer::const_iterator p;
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << "void\n"
			<< "AsterExportStateData("
			<< "Type * const Asterstress_,Type * const Asterstatev) const\n";
    } else {
      behaviourDataFile << "void\n"
			<< "AsterExportStateData("
			<< "Type * const Asterstress_,Type * const) const\n";
    }
    behaviourDataFile << "{\n";
    behaviourDataFile << "using namespace tfel::math;\n";
    behaviourDataFile << "this->sig.exportTab(Asterstress_);\n";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      SupportedTypes::TypeSize o;
      o = this->exportResults(behaviourDataFile,
			      stateVarsHolder,
			      "Asterstatev",
			      mb.useQt());
      this->exportResults(behaviourDataFile,
			  auxiliaryStateVarsHolder,
			  "Asterstatev",
			  mb.useQt(),o);
    }
    behaviourDataFile << "} // end of AsterExportStateData\n";
    behaviourDataFile << endl;
  }
  
  void
  MFrontAsterInterface::writeBehaviourConstructor(std::ofstream& behaviourFile,
						 const std::string& className,
						 const MechanicalBehaviourDescription& characteristic,
						 const std::string& initStateVarsIncrements)
  {
    using namespace std;
    VarContainer::const_iterator p;
    behaviourFile << "/*\n";
    behaviourFile << " * \\brief constructor for the aster interface\n";
    behaviourFile << " *\n";
    behaviourFile << " * \\param const Type *const Asterdt_, time increment\n";
    behaviourFile << " * \\param const Type *const Asterstress_, stress tensor\n";
    behaviourFile << " * \\param const Type *const Asterstran, strain tensor\n";
    behaviourFile << " * \\param const Type *const Asterdstran, strain increment tensor\n";
    behaviourFile << " * \\param const Type *const AsterT_, temperature\n";
    behaviourFile << " * \\param const Type *const AsterdT_, temperature increment\n";
    behaviourFile << " * \\param const Type *const Astermat, material properties\n";
    behaviourFile << " * \\param const Type *const Asterint_vars, state variables\n"; 
    behaviourFile << " * \\param const Type *const Asterext_vars, external state variables\n";
    behaviourFile << " * \\param const Type *const Asterdext_vars,";
    behaviourFile << " external state variables increments\n";
    behaviourFile << " */\n";
    behaviourFile << className 
		  << "(const Type* const Asterdt_,const Type* const Asterstress_,\n"
		  <<  "const Type* const Asterstran, const Type* const Asterdstran,\n" 
		  <<  "const Type* const AsterT_,const Type* const AsterdT_,\n"
		  <<  "const Type* const Astermat,const Type* const Asterint_vars,\n"
		  <<  "const Type* const Asterext_vars,const Type* const Asterdext_vars)\n";
    if(characteristic.useQt()){
      behaviourFile << ": " << className 
		    << "BehaviourData<N,Type,use_qt>(Asterstress_,Asterstran,AsterT_,Astermat,\n"
		    << "Asterint_vars,Asterext_vars),\n";
      behaviourFile << className 
		    << "IntegrationData<N,Type,use_qt>(Asterdt_,Asterdstran,AsterdT_,Asterdext_vars)";
    } else {
      behaviourFile << ": " << className 
		    << "BehaviourData<N,Type,false>(Asterstress_,Asterstran,AsterT_,Astermat,\n"
		    << "Asterint_vars,Asterext_vars),\n";
      behaviourFile << className 
		    << "IntegrationData<N,Type,false>(Asterdt_,Asterdstran,AsterdT_,Asterdext_vars)";
    }
    behaviourFile << initStateVarsIncrements;
  }
  
  void 
  MFrontAsterInterface::writeBehaviourDataConstructor(std::ofstream& behaviourDataFile,
						      const std::string& className,
						      const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const VarContainer& coefsHolder              = mb.getMaterialProperties();
    const VarContainer& stateVarsHolder          = mb.getStateVariables();
    const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const VarContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    VarContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    behaviourDataFile << "/*\n";
    behaviourDataFile << " * \\brief constructor for the aster interface\n";
    behaviourDataFile << " *\n";
    behaviourDataFile << " * \\param const Type *const Asterstress_, stress tensor\n";
    behaviourDataFile << " * \\param const Type *const Asterstran, strain tensor\n";
    behaviourDataFile << " * \\param const Type *const AsterT_, temperature\n";
    behaviourDataFile << " * \\param const Type *const Astermat, material properties\n";
    behaviourDataFile << " * \\param const Type *const Asterint_vars, state variables\n"; 
    behaviourDataFile << " * \\param const Type *const Asterext_vars, external state variables\n";
    behaviourDataFile << " */\n";
    behaviourDataFile << className << "BehaviourData"
		      << "(const Type* const Asterstress_,const Type* const Asterstran,\n" 
		      <<  "const Type* const AsterT_,const Type* const";
    if(!coefsHolder.empty()){
      behaviourDataFile << " Astermat,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile <<  "const Type* const";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << " Asterint_vars,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile << "const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourDataFile << " Asterext_vars";
    }
    behaviourDataFile << ")\n";
    behaviourDataFile << ": T(*AsterT_)";
    SupportedTypes::TypeSize o;
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       coefsHolder,
							       "Astermat","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
								   stateVarsHolder,
								   "Asterint_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       auxiliaryStateVarsHolder,
							       "Asterint_vars","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       externalStateVarsHolder,
							       "Asterext_vars","","");
    behaviourDataFile << "\n{\n";
    SupportedTypes::TypeSize ov;
    SupportedTypes::TypeSize of;
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const DrivingVariable&    v = pm->first;
      const ThermodynamicForce& f = pm->second;
      if(this->getTypeFlag(f.type)==SupportedTypes::Scalar){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << f.name << " =  " << f.type << "(Asterstress_+" << of <<");\n";
	} else {
	  behaviourDataFile << f.name << " =  " << f.type << "(Asterstress_);\n";
	}
      } else if(this->getTypeFlag(f.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "copy("
			    << "static_cast<typename " << f.type << "::value_type *const>(Asterstress_+" << of <<"),\n"
			    << "static_cast<typename " << f.type << "::value_type *const>(Asterstress_+" << of <<"+TVectorSize),\n"
			    << f.name << ".begin())\n;";
	} else {
	  behaviourDataFile << "copy("
			    << "static_cast<typename " << f.type << "::value_type *const>(Asterstress_),\n"
			    << "static_cast<typename " << f.type << "::value_type *const>(Asterstress_+TVectorSize),\n"
			    << f.name << ".begin())\n;";
	}
      } else if(this->getTypeFlag(f.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << f.name << ".importTab(Asterstress_+" << of <<");\n";
	} else {
	  behaviourDataFile << f.name << ".importTab(Asterstress_);\n";
	}
      } else {
	string msg("MFrontAsterInterface::writeBehaviourDataConstructor : ");
	msg += "unsupported forces type";
	throw(runtime_error(msg));
      }
      if(this->getTypeFlag(v.type)==SupportedTypes::Scalar){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << v.name << " = " << v.type << "(Asterstran+" << ov <<");\n";
	} else {
	  behaviourDataFile << v.name << " = " << v.type << "(Asterstran);\n";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << "copy("
			    << "static_cast<typename " << v.type << "::value_type *const>(Asterstran+" << ov <<"),\n"
			    << "static_cast<typename " << v.type << "::value_type *const>(Asterstran+" << ov <<"+TVectorSize),\n"
			    << v.name << ".begin())\n;";
	} else {
	  behaviourDataFile << "copy("
			    << "static_cast<typename " << v.type << "::value_type *const>(Asterstran),\n"
			    << "static_cast<typename " << v.type << "::value_type *const>(Asterstran+TVectorSize),\n"
			    << v.name << ".begin())\n;";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourDataFile << v.name << ".importVoigt(Asterstran+" << ov <<");\n";
	} else {
	  behaviourDataFile << v.name << ".importVoigt(Asterstran);\n";
	}
      } else {
	string msg("MFrontAsterInterface::writeBehaviourDataConstructor : ");
	msg += "unsupported forces type";
	throw(runtime_error(msg));
      }
      ov += this->getTypeSize(v.type,1u);
      of += this->getTypeSize(f.type,1u);
    }
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								coefsHolder,
								"Astermat","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								    stateVarsHolder,
								    "Asterint_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								auxiliaryStateVarsHolder,
								"Asterint_vars","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								externalStateVarsHolder,
								"Asterext_vars","","");
    behaviourDataFile << "}\n\n";
  }
  
  void 
  MFrontAsterInterface::writeIntegrationDataConstructor(std::ofstream& behaviourIntegrationFile,
							const std::string& className,
							const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const VarContainer& externalStateVarsHolder = mb.getExternalStateVariables();
    VarContainer::const_iterator p;
    behaviourIntegrationFile << "/*\n";
    behaviourIntegrationFile << " * \\brief constructor for the aster interface\n";
    behaviourIntegrationFile << " * \\param const Type *const Asterdt_, time increment\n";
    behaviourIntegrationFile << " * \\param const Type *const Asterdstran, strain increment tensor\n";
    behaviourIntegrationFile << " * \\param const Type *const AsterdT_, temperature increment\n";
    behaviourIntegrationFile << " * \\param const Type *const Asterdext_vars,";
    behaviourIntegrationFile << " external state variables increments\n";
    behaviourIntegrationFile << " *\n";
    behaviourIntegrationFile << " */\n";
    behaviourIntegrationFile << className << "IntegrationData"
			     << "(const Type* const Asterdt_,const Type* const Asterdstran,\n" 
			     <<  "const Type* const AsterdT_,const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourIntegrationFile << " Asterdext_vars)\n";
    } else {
      behaviourIntegrationFile << ")\n";
    }
    behaviourIntegrationFile << ": dt(*Asterdt_),dT(*AsterdT_)";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(behaviourIntegrationFile,
								 externalStateVarsHolder,
								 "Asterdext_vars","d","");
    }
    behaviourIntegrationFile << "\n{\n";
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    for(pm=mb.getMainVariables().begin();pm!=mb.getMainVariables().end();++pm){
      const DrivingVariable&    v = pm->first;
      if(!v.increment_known){
	string msg("MFrontAsterInterface::writeIntegrationDataConstructor : ");
	msg += "unsupported driving variable '"+v.name+"'";
	throw(runtime_error(msg));
      }
      if(this->getTypeFlag(v.type)==SupportedTypes::Scalar){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourIntegrationFile << "d" << v.name << " = " << v.type << "(Asterdstran+" << ov <<");\n";
	} else {
	  behaviourIntegrationFile << "d" << v.name << " = " << v.type << "(Asterdstran);\n";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::TVector){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourIntegrationFile << "copy("
				   << "static_cast<typename " << v.type << "::value_type *const>(Asterdstran+" << ov <<"),\n"
				   << "static_cast<typename " << v.type << "::value_type *const>(Asterdstran+" << ov <<"+TVectorSize),\n"
				   << "d" << v.name << ".begin())\n;";
	} else {
	  behaviourIntegrationFile << "copy("
				   << "static_cast<typename " << v.type << "::value_type *const>(Asterdstran),\n"
				   << "static_cast<typename " << v.type << "::value_type *const>(Asterdstran+TVectorSize),\n"
				   << "d" << v.name << ".begin())\n;";
	}
      } else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
	if(pm!=mb.getMainVariables().begin()){
	  behaviourIntegrationFile << "d" << v.name << ".importVoigt(Asterdstran);\n";
	} else {
	  behaviourIntegrationFile << "d" << v.name << ".importVoigt(Asterdstran+" << ov << ");\n";
	}
      } else {
	string msg("MFrontAsterInterface::writeIntegrationDataConstructor : ");
	msg += "unsupported driving variable '"+v.name+"'";
	throw(runtime_error(msg));
      }
      ov += this->getTypeSize(v.type,1u);
    }
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorII(behaviourIntegrationFile,
								  externalStateVarsHolder,
								  "Asterdext_vars","d","");
    }
    behaviourIntegrationFile << "}\n\n";
  }

  std::vector<std::string>
  MFrontAsterInterface::getGlossaryNames(const VarContainer& v,
					const std::map<std::string,std::string>& glossaryNames,
					const std::map<std::string,std::string>& entryNames)
  {
    using namespace std;
    vector<string> n;
    this->appendGlossaryNames(n,v,glossaryNames,entryNames);
    return n;
  }

  void
  MFrontAsterInterface::appendGlossaryNames(std::vector<std::string>& n,
					   const VarContainer& v,
					   const std::map<std::string,std::string>& glossaryNames,
					   const std::map<std::string,std::string>& entryNames)
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      const string name = MFrontAsterInterfaceGetName(glossaryNames,entryNames,p->name);
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
  } // end of MFrontAsterInterface::appendGlossaryNames

  void
  MFrontAsterInterface::writeGlossaryNames(std::ostream& f,
					  const std::vector<std::string>& n,
					  const std::string& name,
					  const std::string& array,
					  const unsigned short o)
  {
    using namespace std;
    if(o>n.size()){
      string msg("MFrontAsterInterface::writeGlossaryNames : ");
      msg += "number of names given is lesser than the offset";
      throw(runtime_error(msg));
    }
    if(n.size()!=o){
      vector<string>::size_type s = 0u;
      vector<string>::const_iterator p = n.begin()+o;      
      f << "MFRONT_SHAREDOBJ const char * aster"
	<< makeLowerCase(name)
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
      f << "MFRONT_SHAREDOBJ const char * const * aster"
	<< makeLowerCase(name)
	<< "_" << array << " = 0;\n\n";
    }      
  } // end of MFrontAsterInterface::writeGlossaryNames

  void
  MFrontAsterInterface::endTreatement(const std::string& file,
				     const std::string& library,
				     const std::string& material,
				     const std::string& className,
				     const std::string& authorName,
				     const std::string& date,
				     const std::map<std::string,std::string>& glossaryNames,
				     const std::map<std::string,std::string>& entryNames,
				     const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    const VarContainer& coefsHolder              = mb.getMaterialProperties();
    const VarContainer& stateVarsHolder          = mb.getStateVariables();
    const VarContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const VarContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    const VarContainer& parametersHolder         = mb.getParameters();
    string header = "Aster";
    string name;
    string asterFctName;
    string tmp;
    VarContainer::const_iterator p;
    VarContainer::const_iterator pp;

    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Aster");

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

    if(!library.empty()){
      name += library;
    }
    name += className;

    string fileName("aster");
    fileName += name;
    fileName += ".hxx";

    ofstream out(("include/MFront/Aster/"+fileName).c_str());

    if(!out){
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    // specific treatment for isotropic behaviour
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      if(mb.getElasticBehaviourType()!=mfront::ISOTROPIC){
	string msg("MFrontAsterInterface::endTreatement : ");
	msg += "an isotropic behaviour must have an isotropic elastic behaviour";
	throw(runtime_error(msg));
      }
      if((mb.requiresStiffnessTensor())||
	 (mb.requiresThermalExpansionTensor())){
	unsigned short min_nprops = 2u;
	if(mb.requiresThermalExpansionTensor()){
	  min_nprops = 4u;
	}
	if(coefsHolder.size()<min_nprops){
	  string msg("MFrontAsterInterface::endTreatement : "
		     "you requested that either (or both) the "
		     "stiffness tensor or the thermal expansion "
		     "tensor to be computed for you. Thus,"
		     "the aster interface requires the ");
	  msg += "following material propertys to be defined (in the right order) ";
	  msg += "- the young modulus     (use @Coef stress           young)\n";
	  msg += "- the poisson ratio     (use @Coef real             nu)\n";
	  if(mb.requiresThermalExpansionTensor()){
	    msg += "- the density           (use @Coef density rho)";
	    msg += "- the thermal expansion (use @Coef thermalexpansion alpha)\n";
	  }
	  throw(runtime_error(msg));
	}
	if(coefsHolder[0].name!="young"){
	  string msg("MFrontASTERInterface::endTreatement : the aster interface requires the ");
	  msg += "first material property to be the young modulus (use @Coef stress young)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[1].name!="nu"){
	  string msg("MFrontASTERInterface::endTreatement : the aster interface requires the ");
	  msg += "second material property to be the poisson ratio (use @Coef real nu)";
	  throw(runtime_error(msg));
	}
	if(mb.requiresThermalExpansionTensor()){
	  if(coefsHolder[2].name!="rho"){
	    string msg("MFrontASTERInterface::endTreatement : the aster interface requires the " );
	    msg += "third material property to be the density (use @Coef density rho)";
	    throw(runtime_error(msg));
	  }
	  if(coefsHolder[3].name!="alpha"){
	    string msg("MFrontASTERInterface::endTreatement : the aster interface requires the" );
	    msg += "fourth material property to be the thermal expansion (use @Coef thermalexpansion alpha)";
	    throw(runtime_error(msg));
	  }
	}
      }
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the aster interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    out << "#ifndef _LIB_"+header+"_HXX_\n";
    out << "#define _LIB_"+header+"_HXX_\n\n";
    
    out << "#ifdef aster" << endl;
    out << "#undef aster" << endl;
    out << "#endif /* aster */" << endl << endl;

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Aster/Aster.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Aster/AsterTraits.hxx\"\n";
    if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Aster/AsterOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

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

    out << "#define aster" 
	<< makeUpperCase(name)
	<< "_F77 \\\n"
	<< "        F77_FUNC(aster"
	<< makeLowerCase(name) << ",Aster"
	<< makeUpperCase(name) << ")\n\n";

    asterFctName = "aster"+makeUpperCase(name)+"_F77";

    out << "#ifdef __cplusplus\n\n";

    out << "namespace aster{\n\n";

    if(mb.useQt()){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
    } else {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
    } 
    out << "struct AsterTraits<tfel::material::" << className << "<H,Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    if(this->errorReport){
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_WRITEONSTDOUT;\n";
    } else {
      out << "static const AsterErrorReportPolicy errorReportPolicy = ASTER_NOERRORREPORT;\n";
    }
    if(mb.requiresStiffnessTensor()){
      out << "static const bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static const bool requiresStiffnessTensor = false;\n";
    }
    if(mb.requiresThermalExpansionTensor()){
      out << "static const bool requiresThermalExpansionTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionTensor = false;\n";
    }
    bool hasElasticMaterialPropertiesOffset          = false;
    bool hasThermalExpansionMaterialPropertiesOffset = false;
    // this indicates that the material properties required to compute
    // the stiffness tensor are part of the material properties
    bool founde = false;
    // this indicates that the material properties required to compute
    // the thermal expansion tensor are part of the material properties
    bool foundt = false;
    if(mb.getElasticBehaviourType()==mfront::ISOTROPIC){
      out << "static const AsterBehaviourType etype = aster::ISOTROPIC;\n";
      if(mb.requiresStiffnessTensor()){
	for(p=coefsHolder.begin();(p!=coefsHolder.end())&&(!founde);++p){
	  if((p->name=="young")||
	     (p->name=="nu")){
	    founde = true;
	  }
	}
	if(founde){
	  /*
	   * Check Aster requirements
	   */
	  if(coefsHolder.size()<2){
	    string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	    msg += "following two material propertys to be defined (in the right order) when the ";
	    msg += "stiffness tensor is required";
	    msg += "(currently only ";
	    msg += toString(static_cast<unsigned short>(coefsHolder.size()));
	    msg += " defined):\n";
	    msg += "- the young modulus     (use @Coef stress young)\n";
	    msg += "- the poisson ratio     (use @Coef real   nu)\n";
	    throw(runtime_error(msg));
	  }
	  if(coefsHolder[0].name!="young"){
	    string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	    msg += "first material property to be the young modulus (use @Coef stress young)";
	    throw(runtime_error(msg));
	  }
	  if(coefsHolder[1].name!="nu"){
	    string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	    msg += "second material property to be the poisson ratio (use @Coef real nu)";
	    throw(runtime_error(msg));
	  }
	  out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
	} else {
	  out << "static const unsigned short elasticPropertiesOffset = 2u;\n";
	  hasElasticMaterialPropertiesOffset = true;
	}
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.requiresThermalExpansionTensor()){
	for(p=coefsHolder.begin();(p!=coefsHolder.end())&&(!foundt);++p){
	  if(p->name=="alpha"){
	    foundt = true;
	  }
	}
	if(foundt){
	  if(mb.requiresStiffnessTensor()){
	    if(coefsHolder.size()<3){
	      string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	      msg += "following three or four material propertys to be defined (in the right order) when the ";
	      msg += "stiffness and the thermal expansion tensors are required";
	      msg += "(currently only ";
	      msg += toString(static_cast<unsigned short>(coefsHolder.size()));
	      msg += " defined):\n";
	      msg += "- the young modulus     (use @Coef stress young)\n";
	      msg += "- the poisson ratio     (use @Coef real   nu)\n";
	      msg += "[- the mass density     (use @Coef density rho)]\n";
	      msg += "- the thermal expansion (use @Coef thermalexpansion alpha)\n";
	      msg += "(the mass density is optionnal and shall be used only if the law ";
	      msg += "has to be compatible with Cast3M.)";
	      throw(runtime_error(msg));
	    }
	    // compatibility with Cast3M
	    if((coefsHolder[2].name!="alpha")&&
	       (coefsHolder[2].name!="rho")){
	      string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	      msg += "third material property to be the thermal expansion or the density";
	      throw(runtime_error(msg));
	    }
	    if(coefsHolder[2].name=="rho"){
	      if(coefsHolder.size()<4){
		string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
		msg += "following three or four material propertys to be defined (in the right order) when the ";
		msg += "stiffness and the thermal expansion tensors are required";
		msg += "(currently only ";
		msg += toString(static_cast<unsigned short>(coefsHolder.size()));
		msg += " defined):\n";
		msg += "- the young modulus     (use @Coef stress young)\n";
		msg += "- the poisson ratio     (use @Coef real   nu)\n";
		msg += "[- the mass density     (use @Coef density rho)]\n";
		msg += "- the thermal expansion (use @Coef thermalexpansion alpha)\n";
		msg += "(the mass density is optionnal and shall be used only if the law ";
		msg += "has to be compatible with Cast3M.)";
		throw(runtime_error(msg));
	      }
	      if(coefsHolder[3].name!="alpha"){
		string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
		msg += "third material property to be the thermal expansion or the density";
		throw(runtime_error(msg));
	      }
	      out << "static const unsigned short massDensityOffsetForThermalExpansion = 1u;\n";
	    } else {
	      out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
	    }
	  } else {
	    if(coefsHolder.empty()){
	      string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	      msg += "thermal expansion to be defined as the first material property";
	      throw(runtime_error(msg));
	    }
	    if(coefsHolder[0].name!="alpha"){
	      string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	      msg += "thermal expansion to be defined as the first material property";
	      throw(runtime_error(msg));
	    }
	    out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
	  }
	  out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
	} else {
	  out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
	  out << "static const unsigned short thermalExpansionPropertiesOffset = 1u;\n"; 
	  hasThermalExpansionMaterialPropertiesOffset = true;
	}
      } else {
	out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
	out << "static const unsigned short thermalExpansionPropertiesOffset     = 0u;\n"; 
      }
    } else if (mb.getElasticBehaviourType()==mfront::ORTHOTROPIC){
      out << "static const AsterBehaviourType etype = aster::ORTHOTROPIC;\n";
      out << "static const unsigned short N = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
      if(mb.requiresStiffnessTensor()){
	out << "static const unsigned short elasticPropertiesOffset "
	    << "= AsterOrthotropicElasticPropertiesOffset<N>::value;\n";
	hasElasticMaterialPropertiesOffset = true;
      } else {
	out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.requiresThermalExpansionTensor()){
	out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
	out << "static const unsigned short thermalExpansionPropertiesOffset = 3u;\n";
	hasThermalExpansionMaterialPropertiesOffset = true;
      } else {
	out << "static const unsigned short massDensityOffsetForThermalExpansion = 0u;\n";
	out << "static const unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
    } else {
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "static const AsterBehaviourType type = aster::ISOTROPIC;\n";
    } else if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "static const AsterBehaviourType type = aster::ORTHOTROPIC;\n";
    } else {
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class AsterTraits\n\n";

    out << "} // end of namespace aster\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "MFRONT_SHAREDOBJ const char *\naster";
    out << makeLowerCase(name) << "_src = \""
	<< tokenize(file,dirSeparator()).back()
	<< "\";\n\n";

    bool rp = false;
    bool ip = false;
    bool up = false;
    if(!parametersHolder.empty()){
      for(pp=parametersHolder.begin();pp!=parametersHolder.end();++pp){
	if(pp->type=="real"){
	  rp = true;
	} else if(pp->type=="int"){
	  ip = true;
	} else if(pp->type=="ushort"){
	  up = true;
	} else {
	  string msg("MFrontAsterInterface::endTreatement : ");
	  msg += "unsupport parameter type '"+p->type+"'.\n";
	  throw(runtime_error(msg));
	} 
      }
      if(rp){
	out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\naster"
	    << makeLowerCase(name)
	    << "_setParameter(const char *const,const double);\n\n";
      }
      if(ip){
	out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\naster"
	    << makeLowerCase(name)
	    << "_setIntegerParameter(const char *const,const int);\n\n";
      }
      if(up){
	out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\naster"
	    << makeLowerCase(name)
	    << "_setIntegerParameter(const char *const,const unsigned short);\n\n";
      }
    }

    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\naster"
	<< makeLowerCase(name) << "("
	<< "aster::AsterReal *const,"       /*< tenseur des contraintes */
	<< "aster::AsterReal *const,"       /*< variables internes */
	<< "aster::AsterReal *const,"       /*< matrice jacobienne du modèle */
	<< "aster::AsterReal *const,"       /*< énergie de déformation élastique */
	<< "aster::AsterReal *const,"       /*< dissipation plastique */
	<< "aster::AsterReal *const,"       /*< dissipation visqueuse */
	<< "aster::AsterReal *const,"       /*< puissance calorifique volumique degagee */
	<< "aster::AsterReal *const,"       /*< derivée des contraintes par rapport à la température */
	<< "aster::AsterReal *const,"       /*< derivées de RPL par rapport aux déformations */
	<< "aster::AsterReal *const,"       /*< derivée de RPL par rapport à la témperature */
	<< "const aster::AsterReal *const," /*< tenseur des déformations totales au début du pas */
	<< "const aster::AsterReal *const," /*< tenseur des incréments de déformation totale */
	<< "const aster::AsterReal *const," /*< temps courant TIME[0] = 0," TIME[1] = t0 */
	<< "const aster::AsterReal *const," /*< incrément de temps */
	<< "const aster::AsterReal *const," /*< température au début du pas */
	<< "const aster::AsterReal *const," /*< incrément de température */
	<< "const aster::AsterReal *const," /*< variables externes au début du pas */
	<< "const aster::AsterReal *const," /*< incréments des variables externes */
	<< "const char     *const," /*< identifiant de la loi de comportement   */
	<< "const aster::AsterInt  *const," /*< entier définissant le type de calcul  */
	<< "const aster::AsterInt  *const," /*< nombre de composantes extradiagonales */
	<< "const aster::AsterInt  *const," /*< nombre de composantes du tenseur des contraintes */
	<< "const aster::AsterInt  *const," /*< nombre de variables internes */
	<< "const aster::AsterReal *const," /*< propriétés du matériaux */
	<< "const aster::AsterInt  *const," /*< nombre de propriétés matériaux */
	<< "const aster::AsterReal *const," /*< coordonnées du point d'intégration */
	<< "const aster::AsterReal *const," /*< matrice de passage du repère local  
						   de l'élement fini massif au repère géneral
						   du maillage */
	<< "aster::AsterReal *const," /*< rapport entre le nouveau pas de temps 
					suggeré et le pas de temps donné en entrée */
	<< "const aster::AsterReal *const," /*< longueur caractéristique de l'élément */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au début du pas */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au fin du pas */
	<< "const aster::AsterInt  *const," /*< numéro de l'élément courant */
	<< "const aster::AsterInt  *const," /*< numéro du point d'intégration courant */
	<< "const aster::AsterInt  *const," /*< numero de couche pour des coques
					      composites ou des solides multi-couches  */
	<< "const aster::AsterInt  *const," /*< numero de section dans la couche courante */
	<< "const aster::AsterInt  *const," /*< entrée non utilisée dans Cast3M  */
	<< "aster::AsterInt  *const" /*< sortie d'erreur */
	<< ");\n\n";

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* __LIB_"+header+"_HXX_ */\n";

    out.close();

    fileName  = "aster";
    fileName += name;
    fileName += ".cxx";

    out.open(("src/"+fileName).c_str());
    if(!out){
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the aster interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    if((!parametersHolder.empty())||(this->debugMode)){
      out << "#include<iostream>\n";
    }

    if(!parametersHolder.empty()){
      out << "#include<stdexcept>\n";
    }

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>\n";
      out << "#include<vector>\n";
    }
    if((this->compareToNumericalTangentOperator)||
       (this->savesTangentOperator)){
      out << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#include\"MFront/Aster/AsterInterface.hxx\"\n\n";
    out << "#include\"MFront/Aster/aster" << name << ".hxx\"\n\n";

    out << "extern \"C\"{\n\n";

    // this->srcFile << "MFRONT_SHAREDOBJ const char *\n";
    // this->srcFile << name << "_src = \""
    // 		  << tokenize(file,systemCall::dirSeparator()).back()
    // 		  << "\";\n\n";

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name)
	<< "_UsableInPurelyImplicitResolution = ";
    if(mb.isUsableInPurelyImplicitResolution()){
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name) << "_BehaviourType = " ;
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else {
      out << "1u;" << endl << endl;
    }

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name);
    unsigned short cs = this->getNumberOfVariables(coefsHolder);
    out << "_nMaterialProperties = " << cs << ";\n";

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name);
    out << "_eoffset = ";
    if(hasElasticMaterialPropertiesOffset){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name);
    out << "_teoffset = ";
    if(hasThermalExpansionMaterialPropertiesOffset){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name);
    out << "_savesTangentOperator = ";
    if(this->savesTangentOperator){
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";

    this->writeGlossaryNames(out,this->getGlossaryNames(coefsHolder,
							glossaryNames,
							entryNames),
			     name,"MaterialProperties");

    const unsigned short nStateVariables = static_cast<unsigned short>(this->getNumberOfVariables(stateVarsHolder) + 
								       this->getNumberOfVariables(auxiliaryStateVarsHolder));
    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name)
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
      out << "MFRONT_SHAREDOBJ int aster"
	  << makeLowerCase(name)
	  << "_InternalStateVariablesTypes [] = {";
      for(p=stateVarsHolder.begin();p!=stateVarsHolder.end();){
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	for(unsigned short is=0;is!=p->arraySize;){
	  switch(flag){
	  case SupportedTypes::Scalar : 
	    out << 0;
	    break;
	  case SupportedTypes::Stensor :
	    out << 1;
	    break;
	  default :
	    string msg("MFrontAsterInterface::endTreatement : ");
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
	SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
	for(unsigned short is=0;is!=p->arraySize;){
	  switch(flag){
	  case SupportedTypes::Scalar : 
	    out << 0;
	    break;
	  case SupportedTypes::Stensor :
	    out << 1;
	    break;
	  default :
	    string msg("MFrontAsterInterface::endTreatement : ");
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
      out << "MFRONT_SHAREDOBJ const int * aster"
	  << makeLowerCase(name)
	  << "_InternalStateVariablesTypes = 0;\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name)
	<< "_nExternalStateVariables = " << this->getNumberOfVariables(externalStateVarsHolder) << ";\n";
    this->writeGlossaryNames(out,this->getGlossaryNames(externalStateVarsHolder,
							glossaryNames,
							entryNames),
			     name,"ExternalStateVariables");
    
    if(rp){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\naster"
	  << makeLowerCase(name)
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
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\naster"
	  << makeLowerCase(name)
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
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\naster"
	  << makeLowerCase(name)
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
    
    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\naster"
	<< makeLowerCase(name) << "("
	<< "aster::AsterReal *const STRESS,"       /*< tenseur des contraintes */
	<< "aster::AsterReal *const STATEV,"       /*< variables internes */
	<< "aster::AsterReal *const DDSOE,"       /*< matrice jacobienne du modèle */
	<< "aster::AsterReal *const,"       /*< énergie de déformation élastique */
	<< "aster::AsterReal *const,"       /*< dissipation plastique */
	<< "aster::AsterReal *const,"       /*< dissipation visqueuse */
	<< "aster::AsterReal *const,"       /*< puissance calorifique volumique degagee */
	<< "aster::AsterReal *const,"       /*< derivée des contraintes par rapport à la température */
	<< "aster::AsterReal *const,"       /*< derivées de RPL par rapport aux déformations */
	<< "aster::AsterReal *const,"       /*< derivée de RPL par rapport à la témperature */
	<< "const aster::AsterReal *const STRAN," /*< tenseur des déformations totales au début du pas */
	<< "const aster::AsterReal *const DSTRAN," /*< tenseur des incréments de déformation totale */
	<< "const aster::AsterReal *const," /*< temps courant TIME[0] = 0," TIME[1] = t0 */
	<< "const aster::AsterReal *const DTIME," /*< incrément de temps */
	<< "const aster::AsterReal *const TEMP," /*< température au début du pas */
	<< "const aster::AsterReal *const DTEMP," /*< incrément de température */
	<< "const aster::AsterReal *const PREDEF," /*< variables externes au début du pas */
	<< "const aster::AsterReal *const DPRED," /*< incréments des variables externes */
	<< "const char     *const," /*< identifiant de la loi de comportement   */
	<< "const aster::AsterInt  *const," /*< entier définissant le type de calcul  */
	<< "const aster::AsterInt  *const," /*< nombre de composantes extradiagonales */
	<< "const aster::AsterInt  *const NTENS," /*< nombre de composantes du tenseur des contraintes */
	<< "const aster::AsterInt  *const NSTATV," /*< nombre de variables internes */
	<< "const aster::AsterReal *const PROPS," /*< propriétés du matériaux */
	<< "const aster::AsterInt  *const NPROPS," /*< nombre de propriétés matériaux */
	<< "const aster::AsterReal *const," /*< coordonnées du point d'intégration */
	<< "const aster::AsterReal *const DROT," /*< matrice de passage du repère local  
						   de l'élement fini massif au repère géneral
						   du maillage */
	<< "aster::AsterReal *const PNEWDT," /*< rapport entre le nouveau pas de temps 
					       suggeré et le pas de temps donné en entrée */
	<< "const aster::AsterReal *const," /*< longueur caractéristique de l'élément */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au début du pas */
	<< "const aster::AsterReal *const," /*< tenseur gradient de déplacement au fin du pas */
	<< "const aster::AsterInt  *const," /*< numéro de l'élément courant */
	<< "const aster::AsterInt  *const," /*< numéro du point d'intégration courant */
	<< "const aster::AsterInt  *const," /*< numero de couche pour des coques
					      composites ou des solides multi-couches  */
	<< "const aster::AsterInt  *const," /*< numero de section dans la couche courante */
	<< "const aster::AsterInt  *const," /*< entrée non utilisée dans Cast3M  */
	<< "aster::AsterInt  *const" /*< sortie d'erreur */
	<< ")\n";
    out << "{\n";
    if((this->debugMode)||
       (this->compareToNumericalTangentOperator)||
       (this->savesTangentOperator)){
      out << "using namespace std;\n";
    }
    if((this->debugMode)||(this->compareToNumericalTangentOperator)){
      out << "using namespace aster;\n";
      out << "const bool computeTangentOperator = (*DDSOE>0.5);\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "vector<AsterReal> deto0(*NTENS);\n";
      out << "vector<AsterReal> sig0(*NTENS);\n";
      out << "vector<AsterReal> sv0(*NSTATV);\n";
      out << "copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n";
      out << "copy(STRESS,STRESS+*(NTENS),sig0.begin());\n";
      out << "copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if(!this->savesTangentOperator){
      out << "if(aster::AsterInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS)!=0){\n";
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
    } else {
      out << "if(*(NSTATV)<(*NTENS)*(*NTENS)){\n"
	  << "string msg(\"aster" << makeLowerCase(name) 
	  << ": invalid number of state variables (can't save tangent operator)\");\n"
	  << "throw(runtime_error(msg));\n"
	  << "}\n";
      out << "aster::AsterInt nNSTATV = max(*(NSTATV)-(*NTENS)*(*NTENS),aster::AsterInt(1));\n";
      out << "if(aster::AsterInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,&nNSTATV,STRESS)!=0){\n";
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
      out << "copy(DDSOE,DDSOE+(*NTENS)*(*NTENS),STATEV+*(NSTATV)-(*NTENS)*(*NTENS));\n";
    }
    if(this->debugMode){
      out << "if(computeTangentOperator){\n";
      out << "AsterInt i;\n";
      out << "AsterInt j;\n";
      out << "cout << \"Dt :\" << endl;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "if(computeTangentOperator){\n";
      out << "// computing the tangent operator by pertubation\n";
      out << "AsterInt i;\n";
      out << "AsterInt j;\n";
      out << "vector<AsterReal> nD((*NTENS)*(*NTENS));\n";
      out << "vector<AsterReal> deto(*NTENS);\n";
      out << "vector<AsterReal> sigf(*NTENS);\n";
      out << "vector<AsterReal> sigb(*NTENS);\n";
      out << "vector<AsterReal> sv(*NSTATV);\n";
      out << "vector<AsterReal> D((*NTENS)*(*NTENS));\n";
      out << "AsterReal m;\n";
      out << "AsterReal mDt;\n";
      out << "AsterReal mnDt;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigf.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] += " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      out << "if(aster::AsterInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0])!=0){\n";
      out << "return;\n";
      out << "}\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigb.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] -= " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      out << "if(aster::AsterInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0])!=0){\n";
      out << "return;\n";
      out << "}\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");\n";
      out << "}\n";
      out << "}\n";
      out << "// comparison\n";
      out << "m=0.;\n";
      out << "mDt=0.;\n";
      out << "mnDt=0.;\n";
      out << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n";
      out << "mDt=max(mDt,*(DDSOE+i));\n";
      out << "mnDt=max(mnDt,nD[i]);\n";
      out << "m=max(m,abs(nD[i]-*(DDSOE+i)));\n";
      out << "}\n";
      out << "if(m>" << this->tangentOperatorComparisonCriterium << "){\n";
      out << "cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< endl;\n";
      out << "cout << \"Dt :\" << endl;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << \"nDt :\" << endl;\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "for(j=0;j!=*NTENS;++j){\n";
      out << "cout << nD[j*(*NTENS)+i] << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "}\n";
    }
    out << "}\n\n";
    out << "} // end of extern \"C\"\n";
    out.close();


  } // end of MFrontAsterInterface::endTreatement
  
  MFrontAsterInterface::~MFrontAsterInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getGlobalDependencies(const std::string&,
					     const std::string&,
					     const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontAsterInterface::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getGlobalIncludes(const std::string& library,
					 const std::string& material,
					 const std::string&)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontAsterInterface::getLibraryName(library,material);
    incs[lib].push_back("`tfel-config --includes`");
#if ASTER_ARCH == 64
    incs[lib].push_back("-DASTER_ARCH=64");
#elif ASTER_ARCH == 32
    incs[lib].push_back("-DASTER_ARCH=32");
#else
#error "MFrontAsterInterface::getGlobalIncludes : unsuported architecture"
#endif
    return incs;
  } // end of MFrontAsterInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getGeneratedSources(const std::string& library,
					   const std::string& material,
					   const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib = MFrontAsterInterface::getLibraryName(library,material);
    if(!library.empty()){
      src[lib].push_back("aster"+library+className+".cxx");
    } else {
      src[lib].push_back("aster"+className+".cxx");
    }
    return src;
  } // end of MFrontAsterInterface::getGeneratedSources

  std::vector<std::string>
  MFrontAsterInterface::getGeneratedIncludes(const std::string& library,
					    const std::string&,
					    const std::string& className)
  {
    using namespace std;
    vector<string> incs;
    if(!library.empty()){
      incs.push_back("MFront/Aster/aster"+library+className+".hxx");
    } else {
      incs.push_back("MFront/Aster/aster"+className+".hxx");
    }
    return incs;
  } // end of MFrontAsterInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontAsterInterface::getLibrariesDependencies(const std::string& library,
						const std::string& material,
						const std::string&)
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontAsterInterface::getLibraryName(library,material);
    deps[lib].push_back("-lAsterInterface");
    deps[lib].push_back("`tfel-config --libs --material`");
    return deps;
  } // end of MFrontAsterInterface::getLibrariesDependencies()

} // end of namespace mfront
