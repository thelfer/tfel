/*!
 * \file   MFrontUMATInterface.cxx
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
#include"MFront/MFrontUMATInterface.hxx"

namespace mfront{

  static const std::string&
  MFrontUMATInterfaceGetName(const std::map<std::string,std::string>& glossaryNames,
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
  } // end of MFrontUMATInterfaceGetName

  std::string
  MFrontUMATInterface::getName(void)
  {
    return "umat";
  }

  std::string
  MFrontUMATInterface::getLibraryName(const std::string& library,
				      const std::string& material)
  {
    using namespace std;
    string lib;
    if(library.empty()){
      if(!material.empty()){
	lib = "libUmat"+material;
      } else {
	lib = "libUmatBehaviour";
      }
    } else {
      lib = "libUmat"+library;
    }
    return lib;
  } // end of MFrontUMATInterface::getLibraryName

  MFrontUMATInterface::MFrontUMATInterface()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {
    this->reset();
  }

  void
  MFrontUMATInterface::reset(void)
  {
    this->finiteStrainStrategy = NONE;
    this->useTimeSubStepping   = false;
    this->maximumSubStepping   = 0u;
    this->doSubSteppingOnInvalidResults = false;
  }
  
  void 
  MFrontUMATInterface::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontUMATInterface::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontUMATInterface::setDebugMode(void)
  {
    this->debugMode = true;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontUMATInterface::treatKeyword(const std::string& key,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    if(key=="@UMATUseTimeSubStepping"){
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATUseTimeSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->useTimeSubStepping = true;
      } else if(current->value=="false"){
	this->useTimeSubStepping = false;
      } else {
	string msg("UmatInterface::treatKeyword (@UMATUseTimeSubStepping) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current); 
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATUseTimeSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }    
      if(current->value!=";"){
	string msg("UmatInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@UMATMaximumSubStepping"){
      if(!this->useTimeSubStepping){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg += "time sub stepping is not enabled at this stage.\n";
	msg += "Use the @UMATUseTimeSubStepping directive before ";
	msg += "@UMATMaximumSubStepping";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      if(flux.fail()){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg+="failed to read maximum substepping value.\n";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATMaximumSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("UmatInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@UMATDoSubSteppingOnInvalidResults"){
      if(!this->useTimeSubStepping){
	string msg("UmatInterface::treatKeyword (@UMATDoSubSteppingOnInvalidResults) : ");
	msg += "time sub stepping is not enabled at this stage.\n";
	msg += "Use the @UMATUseTimeSubStepping directive before ";
	msg += "@UMATMaximumSubStepping";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATDoSubSteppingOnInvalidResults) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value=="true"){
	this->doSubSteppingOnInvalidResults = true;
      } else if(current->value=="false"){
	this->doSubSteppingOnInvalidResults = false;
      } else {
	string msg("UmatInterface::treatKeyword (@UMATDoSubSteppingOnInvalidResults) :");
	msg += "expected 'true' or 'false'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATDoSubSteppingOnInvalidResults) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value!=";"){
	string msg("UmatInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if (key=="@UMATFiniteStrainStrategy"){
      if(this->finiteStrainStrategy!=NONE){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "a strategy has already been defined.\n";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      const string& fss = current->value;
      if(fss=="FiniteRotationSmallStrain"){
	this->finiteStrainStrategy = FINITEROTATIONSMALLSTRAIN;
      } else {
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "unsupported strategy '"+fss+"'\n";
	msg += "The only supported strategy is 'FiniteRotationSmallStrain'";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("UmatInterface::treatKeyword (@UMATFiniteStrainStrategy) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      if(current->value!=";"){
	string msg("UmatInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
    }

    return make_pair(false,current);

  } // end of treatKeyword

  void 
  MFrontUMATInterface::exportMechanicalData(std::ofstream& behaviourDataFile,
					    const std::string&,
					    const VarContainer&,
					    const VarContainer& stateVarsHolder,
					    const VarContainer& auxiliaryStateVarsHolder,
					    const VarContainer&,
					    const BehaviourCharacteristic behaviourCharacteristic)
  {
    using namespace std;
    VarContainer::const_iterator p;
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << "void\n"
			<< "UMATexportStateData("
			<< "Type * const UMATstress_,Type * const UMATstatev) const\n";
    } else {
      behaviourDataFile << "void\n"
			<< "UMATexportStateData("
			<< "Type * const UMATstress_,Type * const) const\n";
    }
    behaviourDataFile << "{\n";
    behaviourDataFile << "using namespace tfel::math;\n";
    behaviourDataFile << "this->sig.exportTab(UMATstress_);\n";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      SupportedTypes::TypeSize o;
      o = this->exportResults(behaviourDataFile,
			      stateVarsHolder,
			      "UMATstatev",
			      behaviourCharacteristic.useQt());
      this->exportResults(behaviourDataFile,
			  auxiliaryStateVarsHolder,
			  "UMATstatev",
			  behaviourCharacteristic.useQt(),o);
    }
    behaviourDataFile << "} // end of UMATExportStateData\n";
    behaviourDataFile << endl;
  }
  
  void
  MFrontUMATInterface::writeBehaviourConstructor(std::ofstream& behaviourFile,
						 const std::string& className,
						 const VarContainer&,
						 const VarContainer&,
						 const VarContainer&,
						 const VarContainer&,
						 const BehaviourCharacteristic characteristic,
						 const std::string& initStateVarsIncrements,
						 const std::string& initComputedVars)
  {
    using namespace std;
    VarContainer::const_iterator p;
    behaviourFile << "/*\n";
    behaviourFile << " * \\brief constructor for the umat interface\n";
    behaviourFile << " *\n";
    behaviourFile << " * \\param const Type *const UMATdt_, time increment\n";
    behaviourFile << " * \\param const Type *const UMATstress_, stress tensor\n";
    behaviourFile << " * \\param const Type *const UMATstran, strain tensor\n";
    behaviourFile << " * \\param const Type *const UMATdstran, strain increment tensor\n";
    behaviourFile << " * \\param const Type *const UMATT_, temperature\n";
    behaviourFile << " * \\param const Type *const UMATdT_, temperature increment\n";
    behaviourFile << " * \\param const Type *const UMATmat, material properties\n";
    behaviourFile << " * \\param const Type *const UMATint_vars, state variables\n"; 
    behaviourFile << " * \\param const Type *const UMAText_vars, external state variables\n";
    behaviourFile << " * \\param const Type *const UMATdext_vars,";
    behaviourFile << " external state variables increments\n";
    behaviourFile << " */\n";
    behaviourFile << className 
		  << "(const Type* const UMATdt_,const Type* const UMATstress_,\n"
		  <<  "const Type* const UMATstran, const Type* const UMATdstran,\n" 
		  <<  "const Type* const UMATT_,const Type* const UMATdT_,\n"
		  <<  "const Type* const UMATmat,const Type* const UMATint_vars,\n"
		  <<  "const Type* const UMAText_vars,const Type* const UMATdext_vars)\n";
    if(characteristic.useQt()){
      behaviourFile << ": " << className 
		    << "BehaviourData<N,Type,use_qt>(UMATstress_,UMATstran,UMATT_,UMATmat,\n"
		    << "UMATint_vars,UMAText_vars),\n";
      behaviourFile << className 
		    << "IntegrationData<N,Type,use_qt>(UMATdt_,UMATdstran,UMATdT_,UMATdext_vars)";
    } else {
      behaviourFile << ": " << className 
		    << "BehaviourData<N,Type,false>(UMATstress_,UMATstran,UMATT_,UMATmat,\n"
		    << "UMATint_vars,UMAText_vars),\n";
      behaviourFile << className 
		    << "IntegrationData<N,Type,false>(UMATdt_,UMATdstran,UMATdT_,UMATdext_vars)";
    }
    behaviourFile << initStateVarsIncrements;
    behaviourFile << initComputedVars;
  }
  
  void 
  MFrontUMATInterface::writeBehaviourDataConstructor(std::ofstream& behaviourDataFile,
						     const std::string& className,
						     const VarContainer& coefsHolder,
						     const VarContainer& stateVarsHolder,
 						     const VarContainer& auxiliaryStateVarsHolder,
						     const VarContainer& externalStateVarsHolder,
						     const BehaviourCharacteristic)
  {
    using namespace std;
    VarContainer::const_iterator p;
    behaviourDataFile << "/*\n";
    behaviourDataFile << " * \\brief constructor for the umat interface\n";
    behaviourDataFile << " *\n";
    behaviourDataFile << " * \\param const Type *const UMATstress_, stress tensor\n";
    behaviourDataFile << " * \\param const Type *const UMATstran, strain tensor\n";
    behaviourDataFile << " * \\param const Type *const UMATT_, temperature\n";
    behaviourDataFile << " * \\param const Type *const UMATmat, material properties\n";
    behaviourDataFile << " * \\param const Type *const UMATint_vars, state variables\n"; 
    behaviourDataFile << " * \\param const Type *const UMAText_vars, external state variables\n";
    behaviourDataFile << " */\n";
    behaviourDataFile << className << "BehaviourData"
		      << "(const Type* const UMATstress_,const Type* const UMATstran,\n" 
		      <<  "const Type* const UMATT_,const Type* const";
    if(!coefsHolder.empty()){
      behaviourDataFile << " UMATmat,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile <<  "const Type* const";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << " UMATint_vars,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile << "const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourDataFile << " UMAText_vars";
    }
    behaviourDataFile << ")\n";
    behaviourDataFile << ": T(*UMATT_)";
    SupportedTypes::TypeSize o;
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       coefsHolder,
							       "UMATmat","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
								   stateVarsHolder,
								   "UMATint_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       auxiliaryStateVarsHolder,
							       "UMATint_vars","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorI(behaviourDataFile,
							       externalStateVarsHolder,
							       "UMAText_vars","","");
    behaviourDataFile << "\n{\n";
    behaviourDataFile << "sig.importTab(UMATstress_);\n";
    behaviourDataFile << "eto.importVoigt(UMATstran);\n";
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								coefsHolder,
								"UMATmat","","");
    o = this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								    stateVarsHolder,
								    "UMATint_vars","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								auxiliaryStateVarsHolder,
								"UMATint_vars","","",o);
    this->writeVariableInitializersInBehaviourDataConstructorII(behaviourDataFile,
								externalStateVarsHolder,
								"UMAText_vars","","");
    behaviourDataFile << "}\n\n";
  }
  
  void 
  MFrontUMATInterface::writeIntegrationDataConstructor(std::ofstream& behaviourIntegrationFile,
						       const std::string& className,
						       const VarContainer&,
						       const VarContainer&,
						       const VarContainer&,
						       const VarContainer& externalStateVarsHolder,
						       const BehaviourCharacteristic)
  {
    using namespace std;
    VarContainer::const_iterator p;
    behaviourIntegrationFile << "/*\n";
    behaviourIntegrationFile << " * \\brief constructor for the umat interface\n";
    behaviourIntegrationFile << " * \\param const Type *const UMATdt_, time increment\n";
    behaviourIntegrationFile << " * \\param const Type *const UMATdstran, strain increment tensor\n";
    behaviourIntegrationFile << " * \\param const Type *const UMATdT_, temperature increment\n";
    behaviourIntegrationFile << " * \\param const Type *const UMATdext_vars,";
    behaviourIntegrationFile << " external state variables increments\n";
    behaviourIntegrationFile << " *\n";
    behaviourIntegrationFile << " */\n";
    behaviourIntegrationFile << className << "IntegrationData"
			     << "(const Type* const UMATdt_,const Type* const UMATdstran,\n" 
			     <<  "const Type* const UMATdT_,const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourIntegrationFile << " UMATdext_vars)\n";
    } else {
      behaviourIntegrationFile << ")\n";
    }
    behaviourIntegrationFile << ": dt(*UMATdt_),dT(*UMATdT_)";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(behaviourIntegrationFile,
								 externalStateVarsHolder,
								 "UMATdext_vars","d","");
    }
    behaviourIntegrationFile << "\n{\n";
    behaviourIntegrationFile << "deto.importVoigt(UMATdstran);\n";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorII(behaviourIntegrationFile,
								  externalStateVarsHolder,
								  "UMATdext_vars","d","");
    }
    behaviourIntegrationFile << "}\n\n";
  }

  std::string
  MFrontUMATInterface::treatScalar(const std::string& s)
  {
    using namespace std;
    string res("'");
    res += makeUpperCase(s.substr(0,4));
    res += "'";
    return res;
  }

  std::string
  MFrontUMATInterface::treatStensor(const std::string& s,const unsigned short d)
  {
    using namespace std;
    string res;
    string s2 = makeUpperCase(s.substr(0,2));
    switch(d){
    case 1 : 
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ'";
      break;
    case 2 :
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ' "+"'"+s2+"XY'";
      break;
    case 3 :
      res="'"+s2+"XX' "+"'"+s2+"YY' "+"'"+s2+"ZZ' "+"'"+s2+"XY' "+"'"+s2+"XZ' "+"'"+s2+"YZ' ";
      break;
    default :
      string msg("MFrontUMATInterface::treatStensor : ");
      msg += "internal error, dimension not supported";
      throw(runtime_error(msg));
    }
    return res;
  }

  std::string
  MFrontUMATInterface::treatCoefScalar(const std::string& s)
  {
    using namespace std;
    string res("'");
    res += makeUpperCase(s.substr(0,4));
    res += "' x"+ makeLowerCase(s.substr(0,4));
    return res;
  }

  std::string
  MFrontUMATInterface::treatCoefStensor(const std::string& s,const unsigned short d)
  {
    using namespace std;
    string res;
    string s2 = makeUpperCase(s.substr(0,2));
    string s3 = makeLowerCase(s.substr(0,2));
    switch(d){
    case 1 : 
      res="'"+s2+"XX' x"+s3+" "+"'"+s2+"YY' x"+s3+" "+"'"+s2+"ZZ' x"+s3;
      break;
    case 2 :
      res="'"+s2+"XX' x"+s3+" "+"'"+s2+"YY' x"+s3+" "+"'"+s2+"ZZ' x"+s3+" "+"'"+s2+"XY' x"+s3;
      break;
    case 3 :
      res  = "'"+s2+"XX' x"+s3+" "+"'"+s2+"YY' x"+s3+" "+"'"+s2+"ZZ' x"+s3+" ";
      res += "'"+s2+"XY' x"+s3+" "+"'"+s2+"XZ' x"+s3+" "+"'"+s2+"YZ' x"+s3;
      break;
    default :
      string msg("MFrontUMATInterface::treatCoefStensor : ");
      msg += "internal error, dimension not supported";
      throw(runtime_error(msg));
    }
    return res;
  }

  std::string
  MFrontUMATInterface::treatStateVarScalar(const std::string& s)
  {
    using namespace std;
    string res("'");
    res += makeUpperCase(s.substr(0,4));
    res += "' 0.";
    return res;
  }

  std::string
  MFrontUMATInterface::treatStateVarStensor(const std::string& s,const unsigned short d)
  {
    using namespace std;
    string res;
    string s2 = makeUpperCase(s.substr(0,2));
    switch(d){
    case 1 : 
      res="'"+s2+"XX' 0. "+"'"+s2+"YY' 0. "+"'"+s2+"ZZ' 0.";
      break;
    case 2 :
      res="'"+s2+"XX' 0. "+"'"+s2+"YY' 0. "+"'"+s2+"ZZ' 0. "+"'"+s2+"XY' 0.";
      break;
    case 3 :
      res  = "'"+s2+"XX' 0. "+"'"+s2+"YY' 0. "+"'"+s2+"ZZ' 0. "+"'"+s2+"XY' 0. ";
      res += "'"+s2+"XZ' 0. "+"'"+s2+"YZ' 0.";
      break;
    default :
      string msg("MFrontUMATInterface::treatStateVarStensor : ");
      msg += "internal error, dimension not supported";
      throw(runtime_error(msg));
    }
    return res;
  }

  std::vector<std::string>
  MFrontUMATInterface::getGlossaryNames(const VarContainer& v,
					const std::map<std::string,std::string>& glossaryNames,
					const std::map<std::string,std::string>& entryNames)
  {
    using namespace std;
    vector<string> n;
    this->appendGlossaryNames(n,v,glossaryNames,entryNames);
    return n;
  }

  void
  MFrontUMATInterface::appendGlossaryNames(std::vector<std::string>& n,
					   const VarContainer& v,
					   const std::map<std::string,std::string>& glossaryNames,
					   const std::map<std::string,std::string>& entryNames)
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      const string name = MFrontUMATInterfaceGetName(glossaryNames,entryNames,p->name);
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
  } // end of MFrontUMATInterface::appendGlossaryNames


  void
  MFrontUMATInterface::writeGlossaryNames(std::ostream& f,
					  const std::vector<std::string>& n,
					  const std::string& name,
					  const std::string& array,
					  const unsigned short o)
  {
    using namespace std;
    if(o>n.size()){
      string msg("MFrontUMATInterface::writeGlossaryNames : ");
      msg += "number of names given is lesser than the offset";
      throw(runtime_error(msg));
    }
    if(n.size()!=o){
      vector<string>::const_iterator p = n.begin()+o;      
      f << "MFRONT_SHAREDOBJ const char * umat"
	<< makeLowerCase(name)
	<< "_" << array << "[" << n.size()-o <<  "] = {";
      while(p!=n.end()){
	f << '"' << *p << '"';
	if(++p!=n.end()){
	  f << ",\n";
	}
      }
      f << "};\n";
    } else {
      f << "MFRONT_SHAREDOBJ const char * const * umat"
	<< makeLowerCase(name)
	<< "_" << array << " = 0;\n\n";
    }      
  } // end of MFrontUMATInterface::writeGlossaryNames

  void
  MFrontUMATInterface::endTreatement(const std::string& file,
				     const std::string& library,
				     const std::string& material,
				     const std::string& className,
				     const std::string& authorName,
				     const std::string& date,
				     const VarContainer& coefsHolder,
				     const VarContainer& stateVarsHolder,
				     const VarContainer& auxiliaryStateVarsHolder,
				     const VarContainer& externalStateVarsHolder,
				     const VarContainer& parametersHolder,
				     const std::map<std::string,std::string>& glossaryNames,
				     const std::map<std::string,std::string>& entryNames,
				     const BehaviourCharacteristic behaviourCharacteristic)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    string header = "UMAT";
    string name;
    string umatFctName;
    string tmp;
    VarContainer::const_iterator p;
    VarContainer::const_iterator pp;
    unsigned short i;
    bool found;
    
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/UMAT");

    if(behaviourCharacteristic.getBehaviourType()!=
       behaviourCharacteristic.getElasticBehaviourType()){
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "the type of the behaviour (isotropic or orthotropic) does not ";
      msg += "match the the type of its elastic behaviour.\n";
      msg += "This is not allowed here :\n";
      msg += "- an isotropic behaviour must have an isotropic elastic behaviour\n";
      msg += "- an orthotropic behaviour must have an orthotropic elastic behaviour";
      throw(runtime_error(msg));
    }

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

    string fileName("umat");
    fileName += name;
    fileName += ".hxx";

    ofstream out(("include/MFront/UMAT/"+fileName).c_str());

    if(!out){
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    found = false;
    // specific treatment for isotropic behaviour
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      for(p=coefsHolder.begin();(p!=coefsHolder.end())&&(!found);++p){
	if((p->name=="young")||
	   (p->name=="nu")||
	   (p->name=="rho")||
	   (p->name=="alpha")){
	  found = true;
	}
      }
      if(found){
	/*
	 * Check UMAT requirements
	 */
	if(coefsHolder.size()<4){
	  string msg("MFrontUMATInterface::endTreatement : the umat interface requires the ");
	  msg += "following four material properties to be defined (in the right order) ";
	  msg += "(currently only ";
	  msg += toString(static_cast<unsigned short>(coefsHolder.size()));
	  msg += " defined):\n";
	  msg += "- the young modulus     (use @Coef stress           young)\n";
	  msg += "- the poisson ratio     (use @Coef real             nu)\n";
	  msg += "- the density           (use @Coef density rho)";
	  msg += "- the thermal expansion (use @Coef thermalexpansion alpha)\n";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[0].name!="young"){
	  string msg("MFrontUMATInterface::endTreatement : the umat interface requires the ");
	  msg += "first material property to be the young modulus (use @Coef stress young)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[1].name!="nu"){
	  string msg("MFrontUMATInterface::endTreatement : the umat interface requires the ");
	  msg += "second material property to be the poisson ratio (use @Coef real nu)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[2].name!="rho"){
	  string msg("MFrontUMATInterface::endTreatement : the umat interface requires the " );
	  msg += "third material property to be the density (use @Coef density rho)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[3].name!="alpha"){
	  string msg("MFrontUMATInterface::endTreatement : the umat interface requires the" );
	  msg += "fourth material property to be the thermal expansion (use @Coef thermalexpansion alpha)";
	  throw(runtime_error(msg));
	}
      }
    }
  
    if(this->useTimeSubStepping){
      if(this->maximumSubStepping==0u){
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "use of time sub stepping requested but MaximumSubStepping is zero.\n";
	msg += "Please use the @UMATMaximumSubStepping directive";
	throw(runtime_error(msg));
      }
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the umat interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    out << "#ifndef _LIB_"+header+"_HXX_\n";
    out << "#define _LIB_"+header+"_HXX_\n\n";
    
    out << "#include\"castem.h\"\n";
    out << "#ifdef umat" << endl;
    out << "#undef umat" << endl;
    out << "#endif /* umat */" << endl << endl;

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/UMAT/UMAT.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/UMAT/UMATTraits.hxx\"\n";
    if (behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/UMAT/UMATOrthotropicBehaviour.hxx\"\n";
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

    out << "#define umat" 
	<< makeUpperCase(name)
	<< "_F77 \\\n"
	<< "        F77_FUNC(umat"
	<< makeLowerCase(name) << ",UMAT"
	<< makeUpperCase(name) << ")\n\n";

    umatFctName = "umat"+makeUpperCase(name)+"_F77";

    out << "#ifdef __cplusplus\n\n";

    out << "namespace umat{\n\n";

    if(behaviourCharacteristic.useQt()){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
    } else {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
    } 
    out << "struct UMATTraits<tfel::material::" << className << "<H,Type,";
    if(behaviourCharacteristic.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    out << "static const bool useTimeSubStepping = ";
    if(this->useTimeSubStepping){
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static const bool doSubSteppingOnInvalidResults = ";
    if(this->doSubSteppingOnInvalidResults){
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static const unsigned short maximumSubStepping = ";
    if(this->useTimeSubStepping){
      out << this->maximumSubStepping << ";\n";
    } else {
      out << "0u;\n";
    }
    if(behaviourCharacteristic.requiresStiffnessTensor()){
      out << "static const bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static const bool requiresStiffnessTensor = false;\n";
    }
    if(behaviourCharacteristic.requiresThermalExpansionTensor()){
      out << "static const bool requiresThermalExpansionTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionTensor = false;\n";
    }
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      if(!found){
	out << "static const unsigned short propertiesOffset = 4u;\n";
      } else {
	out << "static const unsigned short propertiesOffset = 0u;\n";
      }
    } else if (behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "#warning \"something needs to be done here\"\n";
      out << "static const unsigned short N = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
      out << "static const unsigned short propertiesOffset = UMATOrthotropicOffset<N>::value;\n";
    } else {
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      out << "static const UMATBehaviourType type = umat::ISOTROPIC;\n";
    } else if (behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "static const UMATBehaviourType type = umat::ORTHOTROPIC;\n";
    } else {
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class UMATTraits\n\n";

    out << "} // end of namespace umat\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "MFRONT_SHAREDOBJ const char *\numat";
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
	  string msg("MFrontUMATInterface::endTreatement : ");
	  msg += "unsupport parameter type '"+p->type+"'.\n";
	  throw(runtime_error(msg));
	} 
      }
      if(rp){
	out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\numat"
	    << makeLowerCase(name)
	    << "_setParameter(const char *const,const double);\n\n";
      }
      if(ip){
	out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\numat"
	    << makeLowerCase(name)
	    << "_setIntegerParameter(const char *const,const int);\n\n";
      }
      if(up){
	out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\numat"
	    << makeLowerCase(name)
	    << "_setIntegerParameter(const char *const,const unsigned short);\n\n";
      }
    }

    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\numat"
	<< makeLowerCase(name)
	<< "(const umat::UMATInt *const,const umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,      umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATInt  *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATReal *const,\n"
	<< "      umat::UMATReal *const,const umat::UMATInt  *const,\n"
	<< "      umat::UMATReal *const,const umat::UMATInt  *const,\n"
	<< "      umat::UMATInt *const);\n\n";

    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\n" << umatFctName
	<< "(const umat::UMATInt *const,const umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,      umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATReal *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATInt  *const,\n"
	<< "const umat::UMATReal *const,const umat::UMATReal *const,\n"
	<< "      umat::UMATReal *const,const umat::UMATInt  *const,\n"
	<< "      umat::UMATReal *const,const umat::UMATInt  *const,\n"
	<< "      umat::UMATInt *const);\n\n";

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* __LIB_"+header+"_HXX_ */\n";

    out.close();

    fileName  = "umat";
    fileName += name;
    fileName += ".cxx";

    out.open(("src/"+fileName).c_str());
    if(!out){
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the umat interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    if(!parametersHolder.empty()){
      out << "#include<iostream>\n";
      out << "#include<stdexcept>\n";
    }

    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    if(this->finiteStrainStrategy==FINITEROTATIONSMALLSTRAIN){
      out << "#include\"MFront/UMAT/UMATFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/UMAT/UMATInterface.hxx\"\n\n";
    out << "#include\"MFront/UMAT/umat" << name << ".hxx\"\n\n";

    out << "extern \"C\"{\n\n";

    // this->srcFile << "MFRONT_SHAREDOBJ const char *\n";
    // this->srcFile << name << "_src = \""
    // 		  << tokenize(file,systemCall::dirSeparator()).back()
    // 		  << "\";\n\n";

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name)
	<< "_UsableInPurelyImplicitResolution = ";
    if(behaviourCharacteristic.isUsableInPurelyImplicitResolution()){
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name) << "_BehaviourType = " ;
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else {
      out << "1u;" << endl << endl;
    }

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name);
    unsigned short cs = this->getNumberOfVariables(coefsHolder);
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      // skipping the fourth first material properties
      if(found){
	out << "_nMaterialProperties = " << cs-4 << ";\n";
      } else {
	out << "_nMaterialProperties = " << cs << ";\n";
      }
    } else {
      out << "_nMaterialProperties = " << cs << ";\n";
    }

    if((behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC)&&(found)){
      this->writeGlossaryNames(out,this->getGlossaryNames(coefsHolder,
							  glossaryNames,
							  entryNames),
			       name,"MaterialProperties",4u);
    } else {
      this->writeGlossaryNames(out,this->getGlossaryNames(coefsHolder,
							  glossaryNames,
							  entryNames),
			       name,"MaterialProperties");
    }

    const unsigned short nStateVariables = static_cast<unsigned short>(this->getNumberOfVariables(stateVarsHolder) + 
								       this->getNumberOfVariables(auxiliaryStateVarsHolder));
    out << "MFRONT_SHAREDOBJ unsigned short umat"
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
      out << "MFRONT_SHAREDOBJ int umat"
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
	    string msg("MFrontUMATInterface::endTreatement : ");
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
	    string msg("MFrontUMATInterface::endTreatement : ");
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
      out << "MFRONT_SHAREDOBJ const int * umat"
	  << makeLowerCase(name)
	  << "_InternalStateVariablesTypes = 0;\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name)
	<< "_nExternalStateVariables = " << this->getNumberOfVariables(externalStateVarsHolder) << ";\n";
    this->writeGlossaryNames(out,this->getGlossaryNames(externalStateVarsHolder,
							glossaryNames,
							entryNames),
			     name,"ExternalStateVariables");
    
    if(rp){
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\numat"
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
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\numat"
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
      out << "MFRONT_SHAREDOBJ int MFRONT_STDCALL\numat"
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
    
    if(this->finiteStrainStrategy==FINITEROTATIONSMALLSTRAIN){
      out << "MFRONT_SHAREDOBJ unsigned short umat"
	  << makeLowerCase(name) << "_Interface = 2u;\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short umat"
	  << makeLowerCase(name) << "_Interface = 1u;\n";
    }

    if(this->finiteStrainStrategy==FINITEROTATIONSMALLSTRAIN){
      out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\numat"
	  << makeLowerCase(name)
	  << "(const umat::UMATInt *const NTENS, const umat::UMATReal *const DTIME,\n"
	  << "const umat::UMATReal *const DROT,  umat::UMATReal *const DDSOE,\n"
	  << "const umat::UMATReal *const F0,    const umat::UMATReal *const F1,\n"
	  << "const umat::UMATReal *const TEMP,  const umat::UMATReal *const DTEMP,\n"
	  << "const umat::UMATReal *const PROPS, const umat::UMATInt    *const NPROPS,\n"
	  << "const umat::UMATReal *const PREDEF,const umat::UMATReal *const DPRED,\n"
	  << "umat::UMATReal *const STATEV,const umat::UMATInt    *const NSTATV,\n"
	  << "umat::UMATReal *const STRESS,const umat::UMATInt    *const NDI,\n"
	  << "umat::UMATInt    *const KINC)\n";
      out << "{\n";
      out << "using namespace umat;\n";
      out << "// computing the Green Lagrange strains\n";
      out << "UMATReal eto[6];\n";
      out << "UMATReal deto[6];\n";
      out << "UMATInt  i;\n";
      out << "UMATFiniteStrain::computeGreenLagrangeStrain(eto,F0,*NTENS);\n";
      out << "UMATFiniteStrain::computeGreenLagrangeStrain(deto,F1,*NTENS);\n";
      out << "for(i=0;i!=*NTENS;++i){\n";
      out << "deto[i] -= eto[i]\n";
      out << "}\n";
      out << "umat::UMATInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,eto,deto,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC);\n";
      out << "if(*KINC==0){\n";
      out << "UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,F1,*NTENS);\n";
      out << "}\n";
      out << "}\n\n";
      
      out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\n" << umatFctName
	  << "(const umat::UMATInt *const NTENS, const umat::UMATReal *const DTIME,\n"
	  << "const umat::UMATReal *const DROT,  umat::UMATReal *const DDSOE,\n"
	  << "const umat::UMATReal *const F0, const umat::UMATReal *const F1,\n"
	  << "const umat::UMATReal *const TEMP,  const umat::UMATReal *const DTEMP,\n"
	  << "const umat::UMATReal *const PROPS, const umat::UMATInt    *const NPROPS,\n"
	  << "const umat::UMATReal *const PREDEF,const umat::UMATReal *const DPRED,\n"
	  << "umat::UMATReal *const STATEV,const umat::UMATInt    *const NSTATV,\n"
	  << "umat::UMATReal *const STRESS,const umat::UMATInt    *const NDI,\n"
	  << "umat::UMATInt    *const KINC)\n";
      out << "{\n";
      out << "umat" << makeLowerCase(name)
	  << "(NTENS, DTIME,DROT,DDSOE,F0,F1,TEMP,DTEMP,\n"
	  << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	  << "STRESS,NDI,KINC);\n";
      out << "}\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\numat"
	  << makeLowerCase(name)
	  << "(const umat::UMATInt *const NTENS, const umat::UMATReal *const DTIME,\n"
	  << "const umat::UMATReal *const DROT,  umat::UMATReal *const DDSOE,\n"
	  << "const umat::UMATReal *const STRAN, const umat::UMATReal *const DSTRAN,\n"
	  << "const umat::UMATReal *const TEMP,  const umat::UMATReal *const DTEMP,\n"
	  << "const umat::UMATReal *const PROPS, const umat::UMATInt    *const NPROPS,\n"
	  << "const umat::UMATReal *const PREDEF,const umat::UMATReal *const DPRED,\n"
	  << "umat::UMATReal *const STATEV,const umat::UMATInt    *const NSTATV,\n"
	  << "umat::UMATReal *const STRESS,const umat::UMATInt    *const NDI,\n"
	  << "umat::UMATInt    *const KINC)\n";
      out << "{\n";
      out << "umat::UMATInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC);\n";
      out << "}\n\n";
      
      out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\n" << umatFctName
	  << "(const umat::UMATInt *const NTENS, const umat::UMATReal *const DTIME,\n"
	  << "const umat::UMATReal *const DROT,  umat::UMATReal *const DDSOE,\n"
	  << "const umat::UMATReal *const STRAN, const umat::UMATReal *const DSTRAN,\n"
	  << "const umat::UMATReal *const TEMP,  const umat::UMATReal *const DTEMP,\n"
	  << "const umat::UMATReal *const PROPS, const umat::UMATInt    *const NPROPS,\n"
	  << "const umat::UMATReal *const PREDEF,const umat::UMATReal *const DPRED,\n"
	  << "umat::UMATReal *const STATEV,const umat::UMATInt    *const NSTATV,\n"
	  << "umat::UMATReal *const STRESS,const umat::UMATInt    *const NDI,\n"
	  << "umat::UMATInt    *const KINC)\n";
      out << "{\n";
      out << "umat" << makeLowerCase(name)
	  << "(NTENS, DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,\n"
	  << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	  << "STRESS,NDI,KINC);\n";
      out << "}\n\n";
    }
    out << "} // end of extern \"C\"\n";

    out.close();

    systemCall::mkdir("castem");

    fileName  = "castem/";
    fileName += name;
    fileName += ".dgibi";

    out.open(fileName.c_str());
    if(!out){
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "*\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  example of how to use the " << className << " behaviour law\n"
	<< "* in the Cast3M finite element solver\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*\n\n";

    out << "** 1D Example\n\n";
    out << "coel1D = 'MOTS'";
    i=0;
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      if(!found){
	out << "'YOUN' 'NU' 'RHO' 'ALPH' ";
	i=4u;
      }
    } else if(behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "'YG1' 'YG2' 'YG3' 'NU12' 'NU23' 'NU13' 'RHO' 'ALP1' 'ALP2' 'ALP3' ";
      i=10u;
    }
    for(p=coefsHolder.begin();p!=coefsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,1);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n";

    out << "stav1D = 'MOTS'";
    i=0;
    for(p=stateVarsHolder.begin();p!=stateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,1);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    if(stateVarsHolder.size()!=0){
      out << " ";
    }
    for(p=auxiliaryStateVarsHolder.begin();p!=auxiliaryStateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,1);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n";
    
    out << "para1D = 'MOTS' 'T'";
    i=0;
    for(p=externalStateVarsHolder.begin();p!=externalStateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,1);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n\n";
    
    out << "MODL1D = 'MODELISER' v1D 'MECANIQUE' 'ELASTIQUE'\n";
    out << "'NON_LINEAIRE' 'UTILISATEUR' 'NUME_LOI' 1\n";
    out << "'C_MATERIAU' coel1D\n";
    out << "'C_VARINTER' stav1D\n";
    out << "'PARA_LOI'   para1D\n'CONS' M;\n\n";

    out << "MATR1D = 'MATERIAU' MODL1D";
    i=6;
    if((behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC)){
      if(!found){
	out << "'YOUN' xyoung 'NU' xnu 'RHO' xrho 'ALPH' xalpha ";
	i=9u;
      }
    } else {
      out << "'YG1'  xyg1  'YG2'  xyg2  'YG3'  xyg3  'NU12' xNU12 'NU23' xNU23\n"
	  << "'NU13' xNU13 'RHO' xrho 'ALP1' xapl1 'ALP2' xapl2 'ALP3' xALP3 ";
      i=9u;
    }
    for(p=coefsHolder.begin();p!=coefsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatCoefScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatCoefStensor(p->name,1);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>4){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n\n";
    
    out << "** 2D Example\n\n";
    out << "coel2D = 'MOTS'";
    i=0;
    if((behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC)){
      if(!found){
	out << "'YOUN' 'NU' 'RHO' 'ALPH' ";
	i=3u;
      }
    } else if(behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "'YG1' 'YG2' 'YG3' 'NU12' 'NU23' 'NU13' 'G12' 'V1X' 'V1Y'\n"
	  << "'RHO' 'ALP1' 'ALP2' 'ALP3'";
      i=3u;
    }
    for(p=coefsHolder.begin();p!=coefsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,2);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n";

    out << "stav2D = 'MOTS'";
    i=0;
    for(p=stateVarsHolder.begin();p!=stateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,2);
	i=static_cast<unsigned short>(i+4u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    for(p=auxiliaryStateVarsHolder.begin();
	p!=auxiliaryStateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,2);
	i=static_cast<unsigned short>(i+4u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n";
    
    out << "para2D = 'MOTS' 'T'";
    i=0;
    for(p=externalStateVarsHolder.begin();p!=externalStateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,2);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n\n";
    
    out << "MODL2D = 'MODELISER' v2D 'MECANIQUE' 'ELASTIQUE'\n";
    out << "'NON_LINEAIRE' 'UTILISATEUR' 'NUME_LOI' 2\n";
    out << "'C_MATERIAU' coel2D\n";
    out << "'C_VARINTER' stav2D\n";
    out << "'PARA_LOI'   para2D\n'CONS' M;\n\n";

    out << "MATR2D = 'MATERIAU' MODL2D";
    i=6;
    if((behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC)){
      if(!found){
	out << "'YOUN' xyoung 'NU' xnu 'RHO' xrho 'ALPH' xalpha";
	i=9u;
      }
    } else {
      out << "'YG1'  xyg1  'YG2' xyg2 'YG3'  xyg3  'NU12' xNU12 'NU23' xNU23\n"
	  << "'NU13' xNU13 'G12' xg12 'RHO' xrho 'ALP1' xapl1 'ALP2' xapl2 'ALP3' xALP3";
      i=9u;
    }
    for(p=coefsHolder.begin();p!=coefsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatCoefScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatCoefStensor(p->name,2);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>4){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    if(behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "\n'DIRECTION' 'PARALLELE' (1 0)";
    }
    out << ";\n\n";

    out << "** 3D Example\n\n";
    out << "coel3D = 'MOTS'";
    i=0;
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      if(!found){
	out << "'YOUN' 'NU' 'RHO' 'ALPH'";
	i=3u;
      }
    } else {
      out << "'YG1' 'YG2' 'YG3' 'NU12' 'NU23' 'NU13'\n"
	  << "'G12' 'G23' 'G13' 'V1X'  'V1Y'  'V1Z' \n"
	  << "'V2X' 'V2Y' 'V2Z' 'RHO'  'ALP1' 'ALP2' 'ALP3'";
      i=6u;
    }
    for(p=coefsHolder.begin();p!=coefsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,3);
	i=static_cast<unsigned short>(i+6u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n";

    out << "stav3D = 'MOTS'";
    i=0;
    for(p=stateVarsHolder.begin();p!=stateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,3);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    for(p=auxiliaryStateVarsHolder.begin();
	p!=auxiliaryStateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,3);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n";
    
    out << "para3D = 'MOTS' 'T'";
    i=0;
    for(p=externalStateVarsHolder.begin();
	p!=externalStateVarsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatStensor(p->name,3);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>9){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    out << ";\n\n";
    
    out << "MODL3D = 'MODELISER' v3D 'MECANIQUE' 'ELASTIQUE'\n";
    out << "'NON_LINEAIRE' 'UTILISATEUR' 'NUME_LOI' 3\n";
    out << "'C_MATERIAU' coel3D\n";
    out << "'C_VARINTER' stav3D\n";
    out << "'PARA_LOI'   para3D\n'CONS' M;\n\n";

    out << "MATR3D = 'MATERIAU' MODL3D";
    i=6;
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      if(!found){
	out << "'YOUN' xyoung 'NU' xnu 'ALPH' xalpha";
	i=9u;
      }
    } else {
      out << "'YG1' xyg1 'YG2' xyg2 'YG3' xyg3 'NU12' xNU12 'NU23' xNU23 'NU13' xNU13\n"
	  << "'G12' xg12 'G23' xg23 'G13' xg13 'RHO' xrho 'ALP1' xapl1 'ALP2' xapl2 'ALP3' xALP3\n";
      i=9u;
    }
    for(p=coefsHolder.begin();p!=coefsHolder.end();++p){
      SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
      switch(flag){
      case SupportedTypes::Scalar : 
	tmp = treatCoefScalar(p->name);
	i=static_cast<unsigned short>(i+1u);
	break;
      case SupportedTypes::Stensor :
	tmp = treatCoefStensor(p->name,3);
	i=static_cast<unsigned short>(i+3u);
	break;
      default :
	string msg("MFrontUMATInterface::endTreatement : ");
	msg += "internal error, tag unsupported";
	throw(runtime_error(msg));
      }
      if(i>4){
	out << "\n";
	i=0;
      } else {
	out << " ";
      }
      out << tmp;
    }
    if(behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "\n'DIRECTION' 'PARALLELE' (1 0 0) (0 0 1)";
    }
    out << ";\n\n";

    out.close();

  } // end of MFrontUMATInterface::endTreatement
  
  MFrontUMATInterface::~MFrontUMATInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getGlobalDependencies(const std::string&,
					     const std::string&,
					     const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontUMATInterface::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getGlobalIncludes(const std::string& library,
					 const std::string& material,
					 const std::string&)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontUMATInterface::getLibraryName(library,material);
    incs[lib].push_back("`tfel-config --includes`");
#ifdef CASTEM_CPPFLAGS
    incs[lib].push_back(CASTEM_CPPFLAGS);
#endif /* CASTEM_CPPFLAGS */
#ifndef LOCAL_CASTEM_HEADER
#ifdef CASTEM_ROOT
    char * castem_root = ::getenv("CASTEM_ROOT");
    if(castem_root!=0){
      incs[lib].push_back("-I"+string(castem_root)+"/include");
    } else {
      incs[lib].push_back("-I"+string(CASTEM_ROOT)+"/include");
    }
#else /* CASTEM_ROOT */
    if(castem_root!=0){
      incs[lib].push_back("-I"+string(castem_root)+"/include");
    }
#endif /* CASTEM_ROOT */
#endif /* LOCAL_CASTEM_HEADER_FILE */
    return incs;
  } // end of MFrontUMATInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getGeneratedSources(const std::string& library,
					   const std::string& material,
					   const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib = MFrontUMATInterface::getLibraryName(library,material);
    if(!library.empty()){
      src[lib].push_back("umat"+library+className+".cxx");
    } else {
      src[lib].push_back("umat"+className+".cxx");
    }
    return src;
  } // end of MFrontUMATInterface::getGeneratedSources

  std::vector<std::string>
  MFrontUMATInterface::getGeneratedIncludes(const std::string& library,
					    const std::string&,
					    const std::string& className)
  {
    using namespace std;
    vector<string> incs;
    if(!library.empty()){
      incs.push_back("MFront/UMAT/umat"+library+className+".hxx");
    } else {
      incs.push_back("MFront/UMAT/umat"+className+".hxx");
    }
    return incs;
  } // end of MFrontUMATInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontUMATInterface::getLibrariesDependencies(const std::string& library,
						const std::string& material,
						const std::string&)
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontUMATInterface::getLibraryName(library,material);
    deps[lib].push_back("-lUMATInterface");
    deps[lib].push_back("`tfel-config --libs --material`");
    return deps;
  } // end of MFrontUMATInterface::getLibrariesDependencies()

} // end of namespace mfront
