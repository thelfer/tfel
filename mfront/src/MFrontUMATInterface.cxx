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

  std::string
  MFrontUMATInterface::getName(void)
  {
    return "umat";
  }

  std::string
  MFrontUMATInterface::getLibraryName(const std::string& library,
				      const std::string& material) const
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

  std::string
  MFrontUMATInterface::getInterfaceName(void) const
  {
    return "Umat";
  } // end of MFrontUMATInterface::getInterfaceName

  std::string
  MFrontUMATInterface::getFunctionName(const std::string& name) const
  {
    return "umat"+makeLowerCase(name);
  } // end of MFrontUMATInterface::getLibraryName

  MFrontUMATInterface::MFrontUMATInterface()
  {
    this->reset();
  }

  void
  MFrontUMATInterface::reset(void)
  {
    MFrontUMATInterfaceBase::reset();
    this->finiteStrainStrategy          = NONE;
    this->useTimeSubStepping            = false;
    this->maximumSubStepping            = 0u;
    this->doSubSteppingOnInvalidResults = false;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontUMATInterface::treatKeyword(const std::string& key,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    if (key=="@UMATGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if(key=="@UMATUseTimeSubStepping"){
      this->useTimeSubStepping = this->readBooleanValue(key,current,end);
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
      this->doSubSteppingOnInvalidResults = this->readBooleanValue(key,current,end);
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

  bool
  MFrontUMATInterface::checkIfElasticPropertiesAreDeclared(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const VarContainer& coefsHolder              = mb.getMaterialProperties();
    VarContainer::const_iterator p;
    bool found = false;
    // specific treatment for isotropic behaviour
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
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
    return found;
  } // end of MFrontUMATInterface::checkIfElasticPropertiesAreDeclared

  void
  MFrontUMATInterface::endTreatement(const std::string& file,
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
    string name;
    string umatFctName;
    string tmp;
    VarContainer::const_iterator p;
    unsigned short i;
    
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/UMAT");

    if(mb.getBehaviourType()!=
       mb.getElasticBehaviourType()){
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "the type of the behaviour (isotropic or orthotropic) does not ";
      msg += "match the the type of its elastic behaviour.\n";
      msg += "This is not allowed here :\n";
      msg += "- an isotropic behaviour must have an isotropic elastic behaviour\n";
      msg += "- an orthotropic behaviour must have an orthotropic elastic behaviour";
      throw(runtime_error(msg));
    }

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

    const string header = this->getHeaderDefine(library,
						material,
						className);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";
    
    out << "#include\"castem.h\"\n";
    out << "#ifdef umat" << endl;
    out << "#undef umat" << endl;
    out << "#endif /* umat */" << endl << endl;

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/UMAT/UMAT.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/UMAT/UMATTraits.hxx\"\n";
    if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/UMAT/UMATOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#define umat" 
	<< makeUpperCase(name)
	<< "_F77 \\\n"
	<< "        F77_FUNC(umat"
	<< makeLowerCase(name) << ",UMAT"
	<< makeUpperCase(name) << ")\n\n";

    umatFctName = "umat"+makeUpperCase(name)+"_F77";

    out << "#ifdef __cplusplus\n\n";

    out << "namespace umat{\n\n";

    if(mb.useQt()){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
    } else {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
    } 
    out << "struct UMATTraits<tfel::material::" << className << "<H,Type,";
    if(mb.useQt()){
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
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
	out << "static const unsigned short propertiesOffset = 4u;\n";
      } else {
	out << "static const unsigned short propertiesOffset = 0u;\n";
      }
    } else if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "#warning \"something needs to be done here\"\n";
      out << "static const unsigned short N = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
      out << "static const unsigned short propertiesOffset = UMATOrthotropicOffset<N>::value;\n";
    } else {
      string msg("MFrontUMATInterface::endTreatement : ");
      msg += "unsupported behaviour type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "static const UMATBehaviourType type = umat::ISOTROPIC;\n";
    } else if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
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

    this->writeSetParametersFunctionsDeclarations(out,name,mb);

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

    out << "#endif /* " << header << " */\n";

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

    this->getExtraSrcIncludes(out,mb);
    if(this->generateMTestFile){
      if(this->finiteStrainStrategy==NONE){
	out << "#include\"MFront/UMAT/UMATGetModellingHypothesis.hxx\"\n";
      } else {
	if(this->generateMTestFile){
	  string msg("MFrontUMATInterface::endTreatement : "
		     "generating mtest file is not supported "
		     "upon large strains");
	  throw(runtime_error(msg));
	}
      }
    }
    if(this->finiteStrainStrategy==FINITEROTATIONSMALLSTRAIN){
      out << "#include\"MFront/UMAT/UMATFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/UMAT/UMATInterface.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#include\"MFront/UMAT/umat" << name << ".hxx\"\n\n";
    out << "extern \"C\"{\n\n";

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name)
	<< "_UsableInPurelyImplicitResolution = ";
    if(mb.isUsableInPurelyImplicitResolution()){
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name) << "_BehaviourType = " ;
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else {
      out << "1u;" << endl << endl;
    }

    out << "MFRONT_SHAREDOBJ unsigned short umat"
      	<< makeLowerCase(name) << "_ElasticBehaviourType = " ;
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "0u;" << endl << endl;
    } else {
      out << "1u;" << endl << endl;
    }

    this->generateUMATxxSymbols(out,name,mb,glossaryNames,entryNames);
    
    this->writeSetParametersFunctionsImplantations(out,name,className,mb);
    
    if(this->finiteStrainStrategy==FINITEROTATIONSMALLSTRAIN){
      out << "MFRONT_SHAREDOBJ unsigned short umat"
	  << makeLowerCase(name) << "_Interface = 2u;\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short umat"
	  << makeLowerCase(name) << "_Interface = 1u;\n";
    }

    if(this->finiteStrainStrategy==FINITEROTATIONSMALLSTRAIN){
      if(this->generateMTestFile){
	string msg("MFrontUMATInterface::endTreatement : "
		   "generating mtest file is not supported "
		   "upon large strains");
	throw(runtime_error(msg));
      }
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
      this->generateMTestFile1(out);
      out << "umat::UMATInterface<tfel::material::" << className 
	  << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC);\n";
      if(this->generateMTestFile){
	out << "if(*KINC!=0){\n";
	this->generateMTestFile2(out,library,material,name,mb,
				 glossaryNames,entryNames);
	out << "}\n";
      }
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
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
	out << "'YOUN' 'NU' 'RHO' 'ALPH' ";
	i=4u;
      }
    } else if(mb.getBehaviourType()==mfront::ORTHOTROPIC){
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
    if((mb.getBehaviourType()==mfront::ISOTROPIC)){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
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
    if((mb.getBehaviourType()==mfront::ISOTROPIC)){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
	out << "'YOUN' 'NU' 'RHO' 'ALPH' ";
	i=3u;
      }
    } else if(mb.getBehaviourType()==mfront::ORTHOTROPIC){
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
    if((mb.getBehaviourType()==mfront::ISOTROPIC)){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
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
    if(mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "\n'DIRECTION' 'PARALLELE' (1 0)";
    }
    out << ";\n\n";

    out << "** 3D Example\n\n";
    out << "coel3D = 'MOTS'";
    i=0;
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
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
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
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
    if(mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "\n'DIRECTION' 'PARALLELE' (1 0 0) (0 0 1)";
    }
    out << ";\n\n";

    out.close();

  } // end of MFrontUMATInterface::endTreatement

  void
  MFrontUMATInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "mg.setModellingHypothesis(umat::getModellingHypothesis(*NDI));\n";
  }
  
  MFrontUMATInterface::~MFrontUMATInterface()
  {}

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

  bool
  MFrontUMATInterface::hasMaterialPropertiesOffset(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      return !this->checkIfElasticPropertiesAreDeclared(mb);
    } else if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
      return true;
    } else {
      string msg("MFrontUMATInterface::hasMaterialPropertiesOffset : ");
      msg += "unsupported behaviour type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    return false;
  } // end of MFrontUMATInterface::hasMaterialPropertiesOffset

  void
  MFrontUMATInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(std::ostream& out,
											 const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(!this->hasMaterialPropertiesOffset(mb)){
      return;
    }
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      out << "mg.addMaterialProperty(\"YoungModulus\",*PROPS);\n"
	  << "mg.addMaterialProperty(\"PoissonRatio\",*(PROPS+1));\n"
	  << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+2));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion\",*(PROPS+3));\n";
    } else if (mb.getBehaviourType()==mfront::ORTHOTROPIC){
      out << "if(*NDI==-1){\n"
	  << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	  << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	  << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+2));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+3));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+4));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+5));\n"
	  << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+6));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+7));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+8));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+9));\n"
	  << "} else if(*NDI==-2){\n"
	  << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	  << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+2));\n"
	  << "mg.addMaterialProperty(\"ShearModulus12\",*(PROPS+3));\n"
	  << "mg.addMaterialProperty(\"V1X\",*(PROPS+4));\n"
	  << "mg.addMaterialProperty(\"V1Y\",*(PROPS+5));\n"
	  << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+6));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+7));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+8));\n"
	  << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+9));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+10));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+11));\n"
	  << "mg.addMaterialProperty(\"PlateWidth\",*(PROPS+12));\n"
	  << "} else if((*NDI==0)||(*NDI==-1)||(*NDI==-3)){\n"
	  << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	  << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	  << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+2));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+3));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+4));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+5));\n"
	  << "mg.addMaterialProperty(\"ShearModulus12\",*(PROPS+6));\n"
	  << "mg.addMaterialProperty(\"V1X\",*(PROPS+7));\n"
	  << "mg.addMaterialProperty(\"V1Y\",*(PROPS+8));\n"
	  << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+9));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+10));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+11));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+12));\n"
	  << "} else if(*NDI==2){\n"
	  << "mg.addMaterialProperty(\"YoungModulus1\",*PROPS);\n"
	  << "mg.addMaterialProperty(\"YoungModulus2\",*(PROPS+1));\n"
	  << "mg.addMaterialProperty(\"YoungModulus3\",*(PROPS+2));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio12\",*(PROPS+3));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio23\",*(PROPS+4));\n"
	  << "mg.addMaterialProperty(\"PoissonRatio13\",*(PROPS+5));\n"
	  << "mg.addMaterialProperty(\"ShearModulus12\",*(PROPS+6));\n"
	  << "mg.addMaterialProperty(\"ShearModulus23\",*(PROPS+7));\n"
	  << "mg.addMaterialProperty(\"ShearModulus13\",*(PROPS+8));\n"
	  << "mg.addMaterialProperty(\"V1X\",*(PROPS+9));\n"
	  << "mg.addMaterialProperty(\"V1Y\",*(PROPS+10));\n"
	  << "mg.addMaterialProperty(\"V1Z\",*(PROPS+11));\n"
	  << "mg.addMaterialProperty(\"V2X\",*(PROPS+12));\n"
	  << "mg.addMaterialProperty(\"V2Y\",*(PROPS+13));\n"
	  << "mg.addMaterialProperty(\"V2Z\",*(PROPS+14));\n"
	  << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+15));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion1\",*(PROPS+16));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion2\",*(PROPS+17));\n"
	  << "mg.addMaterialProperty(\"ThermalExpansion3\",*(PROPS+18));\n"
	  << "}\n";
    } else {
      string msg("MFrontUMATInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation : ");
      msg += "unsupported behaviour type.\n";
      msg += "The umat interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
  } // end of MFrontUMATInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation

  void
  MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbols(std::ostream& out,
							    const std::string& name,
							    const MechanicalBehaviourDescription& mb,
							    const std::map<std::string,std::string>& glossaryNames,
							    const std::map<std::string,std::string>& entryNames) const
  {
    const VarContainer& coefsHolder = mb.getMaterialProperties();
    const unsigned short cs = this->getNumberOfVariables(coefsHolder);
    const bool found        = this->checkIfElasticPropertiesAreDeclared(mb);
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name);
    if(mb.getBehaviourType()==mfront::ISOTROPIC){
      // skipping the fourth first material properties
      if(found){
	out << "_nMaterialProperties = " << cs-4 << ";\n";
      } else {
	out << "_nMaterialProperties = " << cs << ";\n";
      }
    } else {
      out << "_nMaterialProperties = " << cs << ";\n";
    }
    if((mb.getBehaviourType()==mfront::ISOTROPIC)&&(found)){
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
  } // end of MFrontUMATInterface::writeUMATxxMaterialPropertiesSymbol

} // end of namespace mfront
