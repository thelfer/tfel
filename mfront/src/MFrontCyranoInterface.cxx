/*!
 * \file   MFrontCyranoInterface.cxx
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
#include"MFront/MFrontCyranoInterface.hxx"

namespace mfront{

  std::string
  MFrontCyranoInterface::getName(void)
  {
    return "cyrano";
  }

  std::string
  MFrontCyranoInterface::getLibraryName(const std::string& library,
					const std::string& material) const
  {
    using namespace std;
    string lib;
    if(library.empty()){
      if(!material.empty()){
	lib = "libCyrano"+material;
      } else {
	lib = "libCyranoBehaviour";
      }
    } else {
      lib = "libCyrano"+library;
    }
    return lib;
  } // end of MFrontCyranoInterface::getLibraryName

  std::string
  MFrontCyranoInterface::getInterfaceName(void) const
  {
    return "Cyrano";
  } // end of MFrontCyranoInterface::getInterfaceName

  std::string
  MFrontCyranoInterface::getFunctionName(const std::string& name) const
  {
    return "cyrano"+makeLowerCase(name);
  } // end of MFrontCyranoInterface::getLibraryName

  std::string
  MFrontCyranoInterface::getBehaviourName(const std::string& library,
					const std::string& className) const
  {
    using namespace std;
    string name;
    if(!library.empty()){
      name += library;
    }
    name += className;
    return name;
  } // end of MFrontCyranoInterface::getBehaviourName

  std::string
  MFrontCyranoInterface::getUmatFunctionName(const std::string& library,
					   const std::string& className) const
  {
    return "cyrano"+makeLowerCase(this->getBehaviourName(library,className));
  } // end of MFrontCyranoInterface::getUmatFunctionName

  MFrontCyranoInterface::MFrontCyranoInterface()
  {
    this->reset();
  }

  void
  MFrontCyranoInterface::reset(void)
  {
    MFrontUMATInterfaceBase::reset();
    this->useTimeSubStepping            = false;
    this->maximumSubStepping            = 0u;
    this->doSubSteppingOnInvalidResults = false;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontCyranoInterface::treatKeyword(const std::string& key,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using namespace std;
    using namespace tfel::utilities;
    if((key=="@CyranoGenerateMTestFileOnFailure")||
	(key=="@UMATGenerateMTestFileOnFailure")){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if((key=="@CyranoUseTimeSubStepping")||
	      (key=="@UMATUseTimeSubStepping")){
      this->useTimeSubStepping = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    } else if ((key=="@CyranoMaximumSubStepping")||
	       (key=="@UMATMaximumSubStepping")){
      if(!this->useTimeSubStepping){
	string msg("CyranoInterface::treatKeyword (@CyranoMaximumSubStepping) : ");
	msg += "time sub stepping is not enabled at this stage.\n";
	msg += "Use the @CyranoUseTimeSubStepping directive before ";
	msg += "@CyranoMaximumSubStepping";
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("CyranoInterface::treatKeyword (@CyranoMaximumSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
      istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      if(flux.fail()){
	string msg("CyranoInterface::treatKeyword (@CyranoMaximumSubStepping) : ");
	msg+="failed to read maximum substepping value.\n";
	throw(runtime_error(msg));
      }
      ++(current);
      if(current==end){
	string msg("CyranoInterface::treatKeyword (@CyranoMaximumSubStepping) : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }      
      if(current->value!=";"){
	string msg("CyranoInterface::treatKeyword : expected ';', read ");
	msg += current->value;
	throw(runtime_error(msg));
      }
      ++(current);
      return make_pair(true,current);      
    } else if ((key=="@CyranoDoSubSteppingOnInvalidResults")||
	       (key=="@UMATDoSubSteppingOnInvalidResults")){
      if(!this->useTimeSubStepping){
	string msg("CyranoInterface::treatKeyword (@CyranoDoSubSteppingOnInvalidResults) : ");
	msg += "time sub stepping is not enabled at this stage.\n";
	msg += "Use the @CyranoUseTimeSubStepping directive before ";
	msg += "@CyranoMaximumSubStepping";
	throw(runtime_error(msg));
      }
      this->doSubSteppingOnInvalidResults = this->readBooleanValue(key,current,end);
      return make_pair(true,current);      
    }
    return make_pair(false,current);
  } // end of treatKeyword

  bool
  MFrontCyranoInterface::doElasticPropertiesCheck(const MechanicalBehaviourDescription& mb,
						const std::vector<std::string>& mps) const
  {
    using namespace std;
    const VariableDescriptionContainer& coefsHolder = mb.getMaterialProperties();
    bool found = false;
    // specific treatment for isotropic behaviour
    for(VariableDescriptionContainer::const_iterator p=coefsHolder.begin();
	(p!=coefsHolder.end())&&(!found);++p){
      found = (find(mps.begin(),mps.end(),p->name)!=mps.end());
    }
    if(found){
      /*
       * Check Cyrano requirements
       */
      if(coefsHolder.size()<mps.size()){
	string msg("MFrontCyranoInterface::checkIfElasticPropertiesAreDeclared : "
		   "the cyrano interface requires the '"+toString(mps.size())+
		   "' material properties to be defined "
		   "(currently only "+toString(coefsHolder.size())+" defined)\n");
	throw(runtime_error(msg));
      }
      for(VariableDescriptionContainer::size_type i=0;i!=mps.size();++i){
	if(coefsHolder[i].name!=mps[i]){
	  string msg("MFrontCyranoInterface::checkIfElasticPropertiesAreDeclared : "
		     "the cyrano interface requires the '"+toString(i)+
		     "' material property to be '"+mps[i]+"'");
	  throw(runtime_error(msg));
	}
      }
    }
    return found;
  } // end of MFrontCyranoInterface::doElasticPropertiesCheck

  bool
  MFrontCyranoInterface::checkIfElasticPropertiesAreDeclared(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      vector<string> mps;
      mps.push_back("young");
      mps.push_back("nu");
      mps.push_back("rho");
      mps.push_back("alpha");
      return MFrontCyranoInterface::doElasticPropertiesCheck(mb,mps);
    }
    return false;
  } // end of MFrontCyranoInterface::checkIfElasticPropertiesAreDeclared

  void
  MFrontCyranoInterface::endTreatement(const std::string& file,
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

    string name;
    VariableDescriptionContainer::const_iterator p;    

    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Cyrano");

    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontCyranoInterface::endTreatement : ");
      msg += "only small strain behaviours are supported.\n";
      throw(runtime_error(msg));
    }

    if(mb.getSymmetryType()!=
       mb.getElasticSymmetryType()){
      string msg("MFrontCyranoInterface::endTreatement : ");
      msg += "the type of the behaviour (isotropic or orthotropic) does not ";
      msg += "match the the type of its elastic behaviour.\n";
      msg += "This is not allowed here :\n";
      msg += "- an isotropic behaviour must have an isotropic elastic behaviour\n";
      msg += "- an orthotropic behaviour must have an orthotropic elastic behaviour";
      throw(runtime_error(msg));
    }

    pair<SupportedTypes::TypeSize,
      SupportedTypes::TypeSize> mvs = mb.getMainVariablesSize();
    
    if(!library.empty()){
      name += library;
    }
    name += className;

    string fileName("cyrano");
    fileName += name;
    fileName += ".hxx";

    ofstream out(("include/MFront/Cyrano/"+fileName).c_str());
    if(!out){
      string msg("MFrontCyranoInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }
  
    if(this->useTimeSubStepping){
      if(this->maximumSubStepping==0u){
	string msg("MFrontCyranoInterface::endTreatement : ");
	msg += "use of time sub stepping requested but MaximumSubStepping is zero.\n";
	msg += "Please use the @CyranoMaximumSubStepping directive";
	throw(runtime_error(msg));
      }
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the cyrano interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    const string header = this->getHeaderDefine(library,
						material,
						className);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Cyrano/Cyrano.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Cyrano/CyranoTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Cyrano/CyranoOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);
    
    this->writeCyranoFortranFunctionDefine(out,name);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace cyrano{\n\n";

    if(mb.useQt()){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type,bool use_qt>\n";
    } else {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type>\n";
    } 
    out << "struct CyranoTraits<tfel::material::" << className << "<H,Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    out << "// space dimension\n";
    out << "static const unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    out << "// tiny vector size\n";
    out << "static const unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static const unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static const unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array (STRAN)\n";
    out << "static const unsigned short DrivingVariableSize  = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRAN)\n";
    out << "static const unsigned short ThermodynamicForceVariableSize  = " << mvs.second <<  ";\n";
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
    if(mb.requiresStiffnessOperator()){
      out << "static const bool requiresStiffnessOperator = true;\n";
    } else {
      out << "static const bool requiresStiffnessOperator = false;\n";
    }
    if(mb.requiresThermalExpansionTensor()){
      out << "static const bool requiresThermalExpansionTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionTensor = false;\n";
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      if(!this->checkIfElasticPropertiesAreDeclared(mb)){
	out << "static const unsigned short propertiesOffset = 4u;\n";
      } else {
	out << "static const unsigned short propertiesOffset = 0u;\n";
      }
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const unsigned short propertiesOffset = CyranoOrthotropicOffset::value;\n";
    } else {
      string msg("MFrontCyranoInterface::endTreatement : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The cyrano interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static const CyranoSymmetryType stype = cyrano::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const CyranoSymmetryType stype = cyrano::ORTHOTROPIC;\n";
    } else {
      string msg("MFrontCyranoInterface::endTreatement : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The cyrano interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class CyranoTraits\n\n";

    out << "} // end of namespace cyrano\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetParametersFunctionsDeclarations(out,name,mb);
    this->writeCyranoFunctionDeclaration(out,name);

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName  = "cyrano";
    fileName += name;
    fileName += ".cxx";

    out.open(("src/"+fileName).c_str());
    if(!out){
      string msg("MFrontCyranoInterface::endTreatement : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the cyrano interface for the " 
	<< className << " behaviour law\n";
    out << "* \\author "  << authorName << endl;
    out << "* \\date   "  << date       << endl;
    out << "*/\n\n";

    this->getExtraSrcIncludes(out,mb);
    if(this->generateMTestFile){
      out << "#include\"MFront/Cyrano/CyranoGetModellingHypothesis.hxx\"\n";
    }

    out << "#include\"MFront/Cyrano/CyranoInterface.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << className << ".hxx\"\n";
    out << "#include\"MFront/Cyrano/cyrano" << name << ".hxx\"\n\n";
    out << "extern \"C\"{\n\n";

    this->generateUMATxxSymbols(out,name,file,mb,glossaryNames,entryNames);

    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourType = 1u;\n\n";
    out << "MFRONT_SHAREDOBJ unsigned short cyrano"
	<< makeLowerCase(name) << "_Interface = 1u;\n\n";

    this->writeSetParametersFunctionsImplementations(out,name,className,mb);

    out << "static void \ncyrano"
	<< makeLowerCase(name) << "_base" 
	<< "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal *const DTIME,\n"
	<< "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const DDSOE,\n"
	<< "const cyrano::CyranoReal *const STRAN, const cyrano::CyranoReal *const DSTRAN,\n"
	<< "const cyrano::CyranoReal *const TEMP,  const cyrano::CyranoReal *const DTEMP,\n"
	<< "const cyrano::CyranoReal *const PROPS, const cyrano::CyranoInt    *const NPROPS,\n"
	<< "const cyrano::CyranoReal *const PREDEF,const cyrano::CyranoReal *const DPRED,\n"
	<< "cyrano::CyranoReal *const STATEV,const cyrano::CyranoInt    *const NSTATV,\n"
	<< "cyrano::CyranoReal *const STRESS,const cyrano::CyranoInt    *const NDI,\n"
	<< "cyrano::CyranoInt    *const KINC)\n";
    out << "{\n";
    out << "cyrano::CyranoInterface<tfel::material::" << className 
	<< ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	<< "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC);\n";
    out << "}\n\n";

    
    this->writeStandardCyranoFunction(out,name,library,material,"",
				      glossaryNames,entryNames,mb);

    out << "} // end of extern \"C\"\n";
    out.close();
  } // end of MFrontCyranoInterface::endTreatement

  void
  MFrontCyranoInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "mg.setModellingHypothesis(cyrano::getModellingHypothesis(*NDI));\n";
  }
  
  MFrontCyranoInterface::~MFrontCyranoInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontCyranoInterface::getGlobalIncludes(const std::string& library,
					   const std::string& material,
					   const std::string&)
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontCyranoInterface::getLibraryName(library,material);
    incs[lib].push_back("`tfel-config --includes`");
#if CYRANO_ARCH == 64
    incs[lib].push_back("-DCYRANO_ARCH=64");
#elif CYRANO_ARCH == 32
    incs[lib].push_back("-DCYRANO_ARCH=32");
#else
#error "MFrontCyranoInterface::getGlobalIncludes : unsuported architecture"
#endif
    return incs;
  } // end of MFrontCyranoInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontCyranoInterface::getGeneratedSources(const std::string& library,
					   const std::string& material,
					   const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib = MFrontCyranoInterface::getLibraryName(library,material);
    if(!library.empty()){
      src[lib].push_back("cyrano"+library+className+".cxx");
    } else {
      src[lib].push_back("cyrano"+className+".cxx");
    }
    return src;
  } // end of MFrontCyranoInterface::getGeneratedSources

  std::vector<std::string>
  MFrontCyranoInterface::getGeneratedIncludes(const std::string& library,
					    const std::string&,
					    const std::string& className)
  {
    using namespace std;
    vector<string> incs;
    if(!library.empty()){
      incs.push_back("MFront/Cyrano/cyrano"+library+className+".hxx");
    } else {
      incs.push_back("MFront/Cyrano/cyrano"+className+".hxx");
    }
    return incs;
  } // end of MFrontCyranoInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontCyranoInterface::getLibrariesDependencies(const std::string& library,
						const std::string& material,
						const std::string&)
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontCyranoInterface::getLibraryName(library,material);
    deps[lib].push_back("-lCyranoInterface");
    deps[lib].push_back("`tfel-config --libs --material`");
    return deps;
  } // end of MFrontCyranoInterface::getLibrariesDependencies()

  bool
  MFrontCyranoInterface::hasMaterialPropertiesOffset(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      return !this->checkIfElasticPropertiesAreDeclared(mb);
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      return true;
    } else {
      string msg("MFrontCyranoInterface::hasMaterialPropertiesOffset : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The cyrano interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    return false;
  } // end of MFrontCyranoInterface::hasMaterialPropertiesOffset

  void 
  MFrontCyranoInterface::writeBehaviourDataConstructor(std::ofstream& behaviourDataFile,
							 const std::string& className,
							 const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    const VariableDescriptionContainer& coefsHolder              = mb.getMaterialProperties();
    const VariableDescriptionContainer& stateVarsHolder          = mb.getStateVariables();
    const VariableDescriptionContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const VariableDescriptionContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    behaviourDataFile << "/*\n";
    behaviourDataFile << " * \\brief constructor for the umat interface\n";
    behaviourDataFile << " *\n";
    behaviourDataFile << " * \\param[in] " << iprefix << "stress_, stress tensor\n";
    behaviourDataFile << " * \\param[in] " << iprefix << "stran, strain tensor\n";
    behaviourDataFile << " * \\param[in] " << iprefix << "T_, temperature\n";
    behaviourDataFile << " * \\param[in] " << iprefix << "mat, material properties\n";
    behaviourDataFile << " * \\param[in] " << iprefix << "int_vars, state variables\n"; 
    behaviourDataFile << " * \\param[in] " << iprefix << "ext_vars, external state variables\n";
    behaviourDataFile << " */\n";
    behaviourDataFile << className << "BehaviourData"
		      << "(const Type* const " << iprefix << "stress_,const Type* const " << iprefix << "stran,\n" 
		      <<  "const Type* const " << iprefix << "T_,const Type* const";
    if(!coefsHolder.empty()){
      behaviourDataFile << " " << iprefix << "mat,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile <<  "const Type* const";
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << " " << iprefix << "int_vars,\n";
    } else {
      behaviourDataFile << ",\n";
    }
    behaviourDataFile << "const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourDataFile << " " << iprefix << "ext_vars";
    }
    behaviourDataFile << ")\n";
    behaviourDataFile << ": T(*" << iprefix << "T_)";
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
    behaviourDataFile << "this->eto[0]=" << iprefix << "stran[0];\n";
    behaviourDataFile << "this->eto[1]=" << iprefix << "stran[2];\n";
    behaviourDataFile << "this->eto[2]=" << iprefix << "stran[1];\n";
    behaviourDataFile << "this->sig[0]=" << iprefix << "stress_[0];\n";
    behaviourDataFile << "this->sig[1]=" << iprefix << "stress_[2];\n";
    behaviourDataFile << "this->sig[2]=" << iprefix << "stress_[1];\n";
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
  MFrontCyranoInterface::writeIntegrationDataConstructor(std::ofstream& behaviourIntegrationFile,
							   const std::string& className,
							   const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    const VariableDescriptionContainer& externalStateVarsHolder  = mb.getExternalStateVariables();
    behaviourIntegrationFile << "/*\n";
    behaviourIntegrationFile << " * \\brief constructor for the umat interface\n";
    behaviourIntegrationFile << " * \\param const Type *const " << iprefix << "dt_, time increment\n";
    behaviourIntegrationFile << " * \\param const Type *const " << iprefix << "dstran, strain increment tensor\n";
    behaviourIntegrationFile << " * \\param const Type *const " << iprefix << "dT_, temperature increment\n";
    behaviourIntegrationFile << " * \\param const Type *const " << iprefix << "dext_vars,";
    behaviourIntegrationFile << " external state variables increments\n";
    behaviourIntegrationFile << " *\n";
    behaviourIntegrationFile << " */\n";
    behaviourIntegrationFile << className << "IntegrationData"
			     << "(const Type* const " << iprefix << "dt_,const Type* const " << iprefix << "dstran,\n" 
			     <<  "const Type* const " << iprefix << "dT_,const Type* const";
    if(!externalStateVarsHolder.empty()){
      behaviourIntegrationFile << " " << iprefix << "dext_vars)\n";
    } else {
      behaviourIntegrationFile << ")\n";
    }
    behaviourIntegrationFile << ": dt(*" << iprefix << "dt_),dT(*" << iprefix << "dT_)";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(behaviourIntegrationFile,
								 externalStateVarsHolder,
								 iprefix+"dext_vars","d","");
    }
    behaviourIntegrationFile << "\n{\n";
    behaviourIntegrationFile << "this->deto[0]=" << iprefix << "dstran[0];\n";
    behaviourIntegrationFile << "this->deto[1]=" << iprefix << "dstran[2];\n";
    behaviourIntegrationFile << "this->deto[2]=" << iprefix << "dstran[1];\n";
    behaviourIntegrationFile << "}\n\n";
  }

  void 
  MFrontCyranoInterface::exportMechanicalData(std::ofstream& behaviourDataFile,
					      const std::string&,
					      const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    const VariableDescriptionContainer& stateVarsHolder          = mb.getStateVariables();
    const VariableDescriptionContainer& auxiliaryStateVarsHolder = mb.getAuxiliaryStateVariables();
    const std::string iprefix = makeUpperCase(this->getInterfaceName());
    if((!stateVarsHolder.empty())||
       (!auxiliaryStateVarsHolder.empty())){
      behaviourDataFile << "void\n"
			<< iprefix+"exportStateData("
			<< "Type * const " << iprefix << "stress_,Type * const " << iprefix << "statev) const\n";
    } else {
      behaviourDataFile << "void\n"
			<< iprefix+"exportStateData("
			<< "Type * const " << iprefix << "stress_,Type * const) const\n";
    }
    behaviourDataFile << "{\n";
    behaviourDataFile << "using namespace tfel::math;\n";
    behaviourDataFile << iprefix << "stress_[0] = this->sig[0];\n"
		      << iprefix << "stress_[1] = this->sig[2];\n"
		      << iprefix << "stress_[2] = this->sig[1];" << endl;
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
    behaviourDataFile << "} // end of " << iprefix << "ExportStateData\n";
    behaviourDataFile << endl;
  }

  void
  MFrontCyranoInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(std::ostream& out,
											 const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(!this->hasMaterialPropertiesOffset(mb)){
      return;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	out << "mg.addMaterialProperty(\"YoungModulus\",*PROPS);\n"
	    << "mg.addMaterialProperty(\"PoissonRatio\",*(PROPS+1));\n"
	    << "mg.addMaterialProperty(\"MassDensity\",*(PROPS+2));\n"
	    << "mg.addMaterialProperty(\"ThermalExpansion\",*(PROPS+3));\n";
	out << "mg_mpoffset = 4u;\n";
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	out << "if(*NDI==14){\n" // axisymmetrical generalized plane strain
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
	    << "mg_mpoffset = 10u;\n"
	    << "}\n";
      } else {
	string msg("MFrontCyranoInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation : ");
	msg += "unsupported behaviour symmetry type.\n";
	msg += "The cyrano interface only support isotropic or orthotropic behaviour at this time.";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MFrontCyranoInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
  } // end of MFrontCyranoInterface::writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation

  void
  MFrontCyranoInterface::writeUMATxxBehaviourTypeSymbols(std::ostream&,
						       const std::string&,
						       const MechanicalBehaviourDescription&) const
  {} // end of MFrontCyranoInterface::writeUMATxxBehaviourTypeSymbols

  void
  MFrontCyranoInterface::writeUMATxxMaterialPropertiesSymbols(std::ostream& out,
							    const std::string& name,
							    const MechanicalBehaviourDescription& mb,
							    const std::map<std::string,std::string>& glossaryNames,
							    const std::map<std::string,std::string>& entryNames) const
  {
    const VariableDescriptionContainer& coefsHolder = mb.getMaterialProperties();
    const unsigned short cs = this->getNumberOfVariables(coefsHolder);
    const bool found        = this->checkIfElasticPropertiesAreDeclared(mb);
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name);
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      // skipping the fourth first material properties
      if(found){
	out << "_nMaterialProperties = " << cs-4 << ";\n";
      } else {
	out << "_nMaterialProperties = " << cs << ";\n";
      }
    } else {
      out << "_nMaterialProperties = " << cs << ";\n";
    }
    if((mb.getSymmetryType()==mfront::ISOTROPIC)&&(found)){
      this->writeGlossaryNames(out,coefsHolder.getGlossaryNames(glossaryNames,
								entryNames),
			       name,"MaterialProperties",4u);
    } else {
      this->writeGlossaryNames(out,coefsHolder.getGlossaryNames(glossaryNames,
								entryNames),
			       name,"MaterialProperties");
    }
  } // end of MFrontCyranoInterface::writeUMATxxMaterialPropertiesSymbol

  void
  MFrontCyranoInterface::writeUMATxxAdditionalSymbols(std::ostream&,// out,
						      const std::string&,// name,
						      const std::string&,
						      const MechanicalBehaviourDescription&,// mb,
						      const std::map<std::string,std::string>&,
						      const std::map<std::string,std::string>&) const
  {
    // typedef MechanicalBehaviourDescription::ModellingHypothesis MH;
    // // check if plane stress hypothesis is supported through the generic plane stress algorithm
    // if(mb.getHypotheses().find(MH::PLANESTRESS)==mb.getHypotheses().end()){
    //   if(mb.getHypotheses().find(MH::AXISYMMETRICALGENERALISEDPLANESTRAIN)!=mb.getHypotheses().end()){
    // 	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
    // 	    << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";
    //   }
    // }
  } // end of MFrontCyranoInterface::writeUMATxxAdditionalSymbols

  void
  MFrontCyranoInterface::writeCyranoFortranFunctionDefine(std::ostream& out,
							  const std::string& name)
  {
    out << "#define cyrano" << makeUpperCase(name) <<"_F77" << " F77_FUNC(cyrano"
	<< makeLowerCase(name) << ",Cyrano"
	<< makeUpperCase(name) << ")\n\n";
  } // end of MFrontCyranoInterface::writeCyranoFortranFunctionDefine

  void
  MFrontCyranoInterface::writeCyranoFunctionDeclaration(std::ostream& out,
						    const std::string& name)
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\ncyrano"
    	<< makeLowerCase(name)
    	<< "(const cyrano::CyranoInt *const,const cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,      cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
    	<< "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
    	<< "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
    	<< "      cyrano::CyranoInt *const);\n\n";
    out << "MFRONT_SHAREDOBJ void\n" 
	<< "cyrano" << makeUpperCase(name) <<"_F77"
    	<< "(const cyrano::CyranoInt *const,const cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,      cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
    	<< "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
    	<< "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
    	<< "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
    	<< "      cyrano::CyranoInt *const);\n\n";
  } // end of MFrontCyranoInterface::writeCyranoFunctionDeclaration

  void
  MFrontCyranoInterface::writeStandardCyranoFunction(std::ostream& out,
						 const std::string& name,
						 const std::string& library,
						 const std::string& material,
						 const std::string& suffix,
						 const std::map<std::string,std::string>& glossaryNames,
						 const std::map<std::string,std::string>& entryNames,
						 const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string fname = name;
    if(!suffix.empty()){
      fname += "_"+suffix;
    }
    const string cyranoFortranFunctionName = "cyrano"+makeUpperCase(fname)+"_F77";
    out << "MFRONT_SHAREDOBJ void MFRONT_STDCALL\ncyrano"
	<< makeLowerCase(fname)
	<< "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal *const DTIME,\n"
	<< "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const DDSOE,\n"
	<< "const cyrano::CyranoReal *const STRAN, const cyrano::CyranoReal *const DSTRAN,\n"
	<< "const cyrano::CyranoReal *const TEMP,  const cyrano::CyranoReal *const DTEMP,\n"
	<< "const cyrano::CyranoReal *const PROPS, const cyrano::CyranoInt    *const NPROPS,\n"
	<< "const cyrano::CyranoReal *const PREDEF,const cyrano::CyranoReal *const DPRED,\n"
	<< "cyrano::CyranoReal *const STATEV,const cyrano::CyranoInt    *const NSTATV,\n"
	<< "cyrano::CyranoReal *const STRESS,const cyrano::CyranoInt    *const NDI,\n"
	<< "cyrano::CyranoInt    *const KINC)\n";
    out << "{\n";
    this->generateMTestFile1(out);
    out << "cyrano" << makeLowerCase(name)
	<< "_base(NTENS, DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "STRESS,NDI,KINC);\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
			       library,material,name,suffix,mb,glossaryNames,entryNames);
      out << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n" << cyranoFortranFunctionName
	<< "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal *const DTIME,\n"
	<< "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const DDSOE,\n"
	<< "const cyrano::CyranoReal *const STRAN, const cyrano::CyranoReal *const DSTRAN,\n"
	<< "const cyrano::CyranoReal *const TEMP,  const cyrano::CyranoReal *const DTEMP,\n"
	<< "const cyrano::CyranoReal *const PROPS, const cyrano::CyranoInt    *const NPROPS,\n"
	<< "const cyrano::CyranoReal *const PREDEF,const cyrano::CyranoReal *const DPRED,\n"
	<< "cyrano::CyranoReal *const STATEV,const cyrano::CyranoInt    *const NSTATV,\n"
	<< "cyrano::CyranoReal *const STRESS,const cyrano::CyranoInt    *const NDI,\n"
	<< "cyrano::CyranoInt    *const KINC)\n";
    out << "{\n";
    out << "cyrano" << makeLowerCase(fname)
	<< "(NTENS, DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "STRESS,NDI,KINC);\n";
    out << "}\n\n";
  }

} // end of namespace mfront
