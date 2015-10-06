/*!
 * \file   mfront/src/MFrontCyranoInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iterator>
#include<algorithm>

#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MFrontCyranoInterface.hxx"

namespace mfront{

  std::string
  MFrontCyranoInterface::getName(void)
  {
    return "cyrano";
  }

  int
  MFrontCyranoInterface::getModellingHypothesisIdentifier(const Hypothesis h)
  {
    using namespace std;
    switch(h){
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
      return 1;
    case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS:
      return 2;
    default:
      ostringstream msg;
      msg << "MFrontCyranoInterface::getModellingHypothesisIdentifier : "
	  << "unsupported hypothesis";
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg << " (default)";
      } else {
	msg << " (" << ModellingHypothesis::toString(h) << "')";
      }
      throw(runtime_error(msg.str()));
    }
    return 0;
  }

  std::string
  MFrontCyranoInterface::getLibraryName(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    string lib;
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "libCyrano"+mb.getMaterialName();
      } else {
	lib = "libCyranoBehaviour";
      }
    } else {
      lib = "libCyrano"+mb.getLibrary();
    }
    return lib;
  } // end of MFrontCyranoInterface::getLibraryName

  std::string
  MFrontCyranoInterface::getInterfaceName(void) const
  {
    return "Cyrano";
  } // end of MFrontCyranoInterface::getInterfaceName

  std::string
  MFrontCyranoInterface::getFunctionName(const std::string& n) const
  {
    return "cyrano"+makeLowerCase(n);
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

  std::pair<std::vector<MFrontUMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  MFrontCyranoInterface::buildMaterialPropertiesList(const MechanicalBehaviourDescription& mb,
						     const Hypothesis h) const
  {
    using namespace std;
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> res;
    vector<UMATMaterialProperty>& mprops = res.first;
    if((h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)&&
       (h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)&&
       (h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)){
      string msg("MFrontCyranoInterface::buildMaterialPropertiesList : "
		 "unsupported modelling hypothesis ");
      throw(runtime_error(msg));
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus","youn",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio","nu",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio12","nu12",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio23","nu23",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio13","nu13",false);
      } else {
	string msg("MFrontCyranoInterface::buildMaterialPropertiesList : "
		   "unsupported behaviour symmetry type");
	throw(runtime_error(msg));
      }
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion","alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3","alp3",false);
      } else {
	string msg("MFrontCyranoInterface::buildMaterialPropertiesList : "
		   "unsupported behaviour symmetry type");
	throw(runtime_error(msg));
      }
    }
    if(!mprops.empty()){
      const UMATMaterialProperty& m = mprops.back();
      res.second  = m.offset;
      res.second += this->getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of MFrontCyranoInterface::buildMaterialPropertiesList

  std::set<tfel::material::ModellingHypothesis::Hypothesis>
  MFrontCyranoInterface::getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    // treatment 
    set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const set<Hypothesis>& bh = mb.getModellingHypotheses();
    // cyrano only supports the AxisymmetricalGeneralisedPlaneStrain
    // and the AxisymmetricalGeneralisedPlaneStress hypotheses
    if(bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
    }
    if(h.empty()){
      string msg("MFrontCyranoInterface::getModellingHypothesesToBeTreated : "
		 "no hypotheses selected. This means that the given beahviour "
		 "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
		 "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
		 "make sense to use the Cyrano interface");
      throw(runtime_error(msg));
    }
    return h;
  } // end of MFrontCyranoInterface::getModellingHypothesesToBeTreated

  void
  MFrontCyranoInterface::writeGetOutOfBoundsPolicyFunctionImplementation(std::ostream& out,
									 const std::string& name) const
  {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy(void){\n"
	<< "using namespace cyrano;\n"
      	<< "using namespace tfel::material;\n"
	<< "static OutOfBoundsPolicy policy = CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy().getOutOfBoundsPolicy();\n"
	<< "return policy;\n"
	<< "}\n\n";
  } // end of MFrontCyranoInterface::writeGetOutOfBoundsPolicyFunctionImplementation    
  
  void
  MFrontCyranoInterface::endTreatment(const MechanicalBehaviourDescription& mb,
				       const MFrontFileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    // check
    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontCyranoInterface::endTreatment : "
		 "the aster interface only supports small strain behaviours");
      throw(runtime_error(msg));
    }
    // get the modelling hypotheses to be treated
    const set<Hypothesis>& h = this->getModellingHypothesesToBeTreated(mb);

    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();

    // some checks
    if(mb.getBehaviourType()!=MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MFrontCyranoInterface::endTreatment : ");
      msg += "only small strain behaviours are supported.\n";
      throw(runtime_error(msg));
    }

    if(mb.getSymmetryType()!=mb.getElasticSymmetryType()){
      string msg("MFrontCyranoInterface::endTreatment : ");
      msg += "the type of the behaviour (isotropic or orthotropic) does not ";
      msg += "match the the type of its elastic behaviour.\n";
      msg += "This is not allowed here :\n";
      msg += "- an isotropic behaviour must have an isotropic elastic behaviour\n";
      msg += "- an orthotropic behaviour must have an orthotropic elastic behaviour";
      throw(runtime_error(msg));
    }
    if(this->useTimeSubStepping){
      if(this->maximumSubStepping==0u){
	string msg("MFrontCyranoInterface::endTreatment : ");
	msg += "use of time sub stepping requested but MaximumSubStepping is zero.\n";
	msg += "Please use the @CyranoMaximumSubStepping directive";
	throw(runtime_error(msg));
      }
    }

    // create the output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Cyrano");

    // opening header file
    string fileName("cyrano");
    fileName += name;
    fileName += ".hxx";

    ofstream out(("include/MFront/Cyrano/"+fileName).c_str());
    if(!out){
      string msg("MFrontCyranoInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }
  
    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file declares the cyrano interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Cyrano/Cyrano.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Cyrano/CyranoTraits.hxx\"\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);
    
    this->writeCyranoFortranFunctionDefine(out,name);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace cyrano{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(h)){
      this->writeCyranoBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	this->writeCyranoBehaviourTraits(out,mb,*ph);
      }
    }

    out << "} // end of namespace cyrano\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetParametersFunctionsDeclarations(out,name,mb);
    this->writeGetIntegrationErrorReportFunctionDeclaration(out,name);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    
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
      string msg("MFrontCyranoInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   "  << fileName << endl;
    out << "* \\brief  This file implements the cyrano interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    this->getExtraSrcIncludes(out,mb);
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "#include\"MFront/MFrontBehaviourProfiler.hxx\"\n\n";
    }
    if(this->generateMTestFile){
      out << "#include\"MFront/Cyrano/CyranoGetModellingHypothesis.hxx\"\n";
    }
    out << "#include\"MFront/Cyrano/CyranoInterface.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx\"\n";
    out << "#include\"MFront/Cyrano/cyrano" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
    
    out << "extern \"C\"{\n\n";

    this->generateUMATxxGeneralSymbols(out,name,mb,fd);
    if(!mb.areAllMechanicalDataSpecialised(h)){
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out,name,uh,mb,fd);
    }
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	this->generateUMATxxSymbols(out,name,*ph,mb,fd);
      }
    }

    out << "MFRONT_SHAREDOBJ unsigned short cyrano"
	<< makeLowerCase(name) << "_Interface = 1u;\n\n";

    this->writeSetParametersFunctionsImplementations(out,name,mb);
    this->writeGetIntegrationErrorReportFunctionImplementation(out,name,mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);

    this->writeStandardCyranoFunction(out,name,mb);

    out << "} // end of extern \"C\"\n";
    out.close();
  } // end of MFrontCyranoInterface::endTreatment

  void
  MFrontCyranoInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "mg.setModellingHypothesis(cyrano::getModellingHypothesis(*NDI));\n";
  }
  
  MFrontCyranoInterface::~MFrontCyranoInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontCyranoInterface::getGlobalIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > incs;
    string lib = MFrontCyranoInterface::getLibraryName(mb);
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
  MFrontCyranoInterface::getGeneratedSources(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > src;
    string lib = MFrontCyranoInterface::getLibraryName(mb);
    if(!mb.getLibrary().empty()){
      src[lib].push_back("cyrano"+mb.getLibrary()+mb.getClassName()+".cxx");
    } else {
      src[lib].push_back("cyrano"+mb.getClassName()+".cxx");
    }
    return src;
  } // end of MFrontCyranoInterface::getGeneratedSources

  std::vector<std::string>
  MFrontCyranoInterface::getGeneratedIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    vector<string> incs;
    if(!mb.getLibrary().empty()){
      incs.push_back("MFront/Cyrano/cyrano"+mb.getLibrary()+mb.getClassName()+".hxx");
    } else {
      incs.push_back("MFront/Cyrano/cyrano"+mb.getClassName()+".hxx");
    }
    return incs;
  } // end of MFrontCyranoInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontCyranoInterface::getLibrariesDependencies(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > deps;
    string lib = MFrontCyranoInterface::getLibraryName(mb);
    deps[lib].push_back("-lCyranoInterface");
    if(this->generateMTestFile){
      deps[lib].push_back("-lMTestFileGenerator");
    }
#ifdef HAVE_CXX11
      deps[lib].push_back("`tfel-config --libs --material --mfront-profiling`");
#else 
      deps[lib].push_back("`tfel-config --libs --material`");
#endif
    return deps;
  } // end of MFrontCyranoInterface::getLibrariesDependencies()

  void 
  MFrontCyranoInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
							const MechanicalBehaviourDescription&) const
  {
    using namespace std;
    out << "#include\"MFront/Cyrano/Cyrano.hxx\"" << endl << endl;
  }

  void
  MFrontCyranoInterface::writeUMATxxAdditionalSymbols(std::ostream&,
						      const std::string&,
						      const Hypothesis,
						      const MechanicalBehaviourDescription&,
						      const MFrontFileDescription&) const
  {} // end of MFrontCyranoInterface::writeUMATxxAdditionalSymbols
  
  // void
  // MFrontCyranoInterface::writeUMATxxSpecificSymbols(std::ostream& out,
  // 						    const std::string& name,
  // 						    const MechanicalBehaviourDescription& mb,
  // 						    const MFrontFileDescription&) const
  // {
  //   if(!mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)){
  // 	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
  // 	    << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";    
  //   } else {
  //     if(!mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)){
  // 	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
  // 	    << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";    
  //     } else {
  // 	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
  // 	    << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";    
  //     }
  //   }
  // }


  void
  MFrontCyranoInterface::writeCyranoFortranFunctionDefine(std::ostream& out,
							  const std::string& name) const
  {
    out << "#define cyrano" << makeUpperCase(name) <<"_F77" << " F77_FUNC(cyrano"
	<< makeLowerCase(name) << ",Cyrano"
	<< makeUpperCase(name) << ")\n\n";
  } // end of MFrontCyranoInterface::writeCyranoFortranFunctionDefine

  void
  MFrontCyranoInterface::writeCyranoFunctionDeclaration(std::ostream& out,
							const std::string& name) const
  {
    using namespace std;
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\ncyrano"
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
						     const std::string& n,
						     const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const string cyranoFortranFunctionName = this->getFunctionName(n)+"_F77";
    out << "MFRONT_SHAREDOBJ void MFRONT_CALLING_CONVENTION\n"
	<< this->getFunctionName(n)
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
    out << "tfel::material::OutOfBoundsPolicy op = " << getFunctionName(n)
	<< "_getOutOfBoundsPolicy();\n";
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      out << "using mfront::MFrontBehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "MFrontBehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "MFrontBehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "cyrano::CyranoInterface<tfel::material::" << mb.getClassName() 
	<< ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
	<< "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC,op);\n";
    if(this->generateMTestFile){
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out,MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
       			       n,"",mb);
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
	<< "cyrano::CyranoInt    *const KINC)\n"
	<< "{\n"
	<< this->getFunctionName(n)
	<< "(NTENS, DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,\n"
	<< "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
	<< "STRESS,NDI,KINC);\n";
    out << "}\n\n";
  }

  void
  MFrontCyranoInterface::writeCyranoBehaviourTraits(std::ostream& out,
						    const MechanicalBehaviourDescription& mb,
						    const Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    const pair<SupportedTypes::TypeSize,
	       SupportedTypes::TypeSize> mvs = mb.getMainVariablesSize();
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> mprops = this->buildMaterialPropertiesList(mb,h);
     if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "template<tfel::material::ModellingHypothesis::Hypothesis H,typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    } else {
      out << "template<typename Type";
      if(mb.useQt()){
	out << ",bool use_qt";
      }
    }
    out << ">\n";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "struct CyranoTraits<tfel::material::" << mb.getClassName() << "<H,Type,";
    } else {
      out << "struct CyranoTraits<tfel::material::"
	  << mb.getClassName() << "<tfel::material::ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,";
    }
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "// modelling hypothesis\n";
      out << "static const tfel::material::ModellingHypothesis::Hypothesis H = "
	  << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ";" << endl;
    }
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
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      out << "static const bool requiresStiffnessTensor = true;\n";
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresUnAlteredStiffnessTensor,false)){
	out << "static const bool requiresUnAlteredStiffnessTensor = true;\n";
      } else {
	out << "static const bool requiresUnAlteredStiffnessTensor = false;\n";
      }
    } else {
      out << "static const bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static const bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static const bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    // computing material properties size
    SupportedTypes::TypeSize msize;
    if(!mprops.first.empty()){
      const UMATMaterialProperty& m = mprops.first.back();
      msize  = m.offset;
      msize += this->getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static const unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
	if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	  out << "static const unsigned short propertiesOffset        = 3u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 2u;\n";
	} else {
	  out << "static const unsigned short propertiesOffset        = 2u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 2u;\n";
	}
      } else {
	if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	  out << "static const unsigned short propertiesOffset        = 1u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
	} else {
	  out << "static const unsigned short propertiesOffset        = 0u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
	}
      }
    } else if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if(mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
	if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	  out << "static const unsigned short propertiesOffset        = 9u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 6u;\n";
	} else {
	  out << "static const unsigned short propertiesOffset        = 6u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 6u;\n";
	}
      } else {
	if(mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	  out << "static const unsigned short propertiesOffset        = 3u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
	} else {
	  out << "static const unsigned short propertiesOffset = 0u;\n";
	  out << "static const unsigned short elasticPropertiesOffset = 0u;\n";
	}
      }
    } else {
      string msg("MFrontCyranoInterface::writeCyranoBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The cyrano interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static const CyranoSymmetryType stype = cyrano::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static const CyranoSymmetryType stype = cyrano::ORTHOTROPIC;\n";
    } else {
      string msg("MFrontCyranoInterface::writeCyranoBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg += "The cyrano interface only support isotropic or orthotropic behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class CyranoTraits\n\n";
  } // end of MFrontCyranoInterface::writeCyranoBehaviourTraits

  std::string
  MFrontCyranoInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    using namespace std;
    ostringstream test;
    test << "*NDI==" << this->getModellingHypothesisIdentifier(h);
    return test.str();
  }

} // end of namespace mfront
