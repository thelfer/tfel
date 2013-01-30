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
  {}

  void
  MFrontAsterInterface::reset(void)
  {}
  
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

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontAsterInterface::treatKeyword(const std::string&,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  void 
  MFrontAsterInterface::exportMechanicalData(std::ofstream& behaviourDataFile,
					    const std::string&,
					    const VarContainer&,
					    const VarContainer& stateVarsHolder,
					    const VarContainer& auxiliaryStateVarsHolder,
					    const VarContainer&,
					    const BehaviourCharacteristic behaviourCharacteristic)
  {
    using namespace std;
    VarContainer::const_iterator p;
    behaviourDataFile << "void\n"
		      << "AsterExportStateData("
		      << "Type * const Asterstress_,Type * const Asterstatev) const\n";
    behaviourDataFile << "{\n";
    behaviourDataFile << "using namespace tfel::math;\n";
    behaviourDataFile << "this->sig.exportTab(Asterstress_);\n";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      SupportedTypes::TypeSize o;
      o = this->exportResults(behaviourDataFile,
			      stateVarsHolder,
			      "Asterstatev",
			      behaviourCharacteristic.useQt());
      this->exportResults(behaviourDataFile,
			  auxiliaryStateVarsHolder,
			  "Asterstatev",
			  behaviourCharacteristic.useQt(),o);
    }
    behaviourDataFile << "} // end of AsterExportStateData\n";
    behaviourDataFile << endl;
  }
  
  void
  MFrontAsterInterface::writeBehaviourConstructor(std::ofstream& behaviourFile,
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
		  <<  "const Type* const Asterext_vars,const Type* const Asterdext_vars,"
		  <<  "const ModellingHypothesis::Hypothesis hypothesis_)\n";
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
    behaviourFile << initComputedVars;
    behaviourFile << ",\nhypothesis(hypothesis_)";
  }
  
  void 
  MFrontAsterInterface::writeBehaviourDataConstructor(std::ofstream& behaviourDataFile,
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
    behaviourDataFile << "sig.importTab(Asterstress_);\n";
    behaviourDataFile << "eto.importVoigt(Asterstran);\n";
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
						       const VarContainer&,
						       const VarContainer&,
						       const VarContainer&,
						       const VarContainer& externalStateVarsHolder,
						       const BehaviourCharacteristic)
  {
    using namespace std;
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
    behaviourIntegrationFile << "deto.importVoigt(Asterdstran);\n";
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
      vector<string>::const_iterator p = n.begin()+o;      
      f << "MFRONT_SHAREDOBJ const char * aster"
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
    string header = "Aster";
    string name;
    string asterFctName;
    string tmp;
    VarContainer::const_iterator p;
    VarContainer::const_iterator pp;
    VarContainer::size_type nb;
    bool found;

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
	 * Check Aster requirements
	 */
	if(coefsHolder.size()<4){
	  string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	  msg += "following three coefficients to be defined (in the right order) ";
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
	  string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	  msg += "first coefficient to be the young modulus (use @Coef stress young)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[1].name!="nu"){
	  string msg("MFrontAsterInterface::endTreatement : the aster interface requires the ");
	  msg += "second coefficient to be the poisson ratio (use @Coef real nu)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[2].name!="rho"){
	  string msg("MFrontAsterInterface::endTreatement : the aster interface requires the " );
	  msg += "third coefficient to be the density (use @Coef density rho)";
	  throw(runtime_error(msg));
	}
	if(coefsHolder[3].name!="alpha"){
	  string msg("MFrontAsterInterface::endTreatement : the aster interface requires the" );
	  msg += "fourth coefficient to be the thermal expansion (use @Coef thermalexpansion alpha)";
	  throw(runtime_error(msg));
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
    if (behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
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

    if(behaviourCharacteristic.useQt()){
      out << "template<unsigned short N,typename Type,bool use_qt>\n";
    } else {
      out << "template<unsigned short N,typename Type>\n";
    } 
    out << "struct AsterTraits<tfel::material::" << className << "<N,Type,";
    if(behaviourCharacteristic.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
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
      out << "static const unsigned short propertiesOffset = AsterOrthotropicOffset<N>::value;\n";
    } else {
      string msg("MFrontAsterInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The aster interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      out << "static const AsterBehaviourType type = aster::ISOTROPIC;\n";
    } else if (behaviourCharacteristic.getBehaviourType()==mfront::ORTHOTROPIC){
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

    if(!parametersHolder.empty()){
      out << "#include<iostream>\n";
      out << "#include<stdexcept>\n";
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
    if(behaviourCharacteristic.isUsableInPurelyImplicitResolution()){
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short aster"
      	<< makeLowerCase(name);
    unsigned short cs = this->getNumberOfVariables(coefsHolder);
    if(behaviourCharacteristic.getBehaviourType()==mfront::ISOTROPIC){
      // skipping the fourth first coefficients
      if(found){
	nb = cs-4;
	out << "_nMaterialProperties = " << cs-4 << ";\n";
      } else {
	nb = cs;
	out << "_nMaterialProperties = " << cs << ";\n";
      }
    } else {
      nb = cs;
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
      if(auxiliaryStateVarsHolder.size()!=0){
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
    out << "if(aster::AsterInterface<tfel::material::" << className 
	<< ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	<< "PREDEF,DPRED,STATEV,NSTATV,STRESS)!=0){\n";
    out << "*PNEWDT = -1.;\n";
    out << "}\n";
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
