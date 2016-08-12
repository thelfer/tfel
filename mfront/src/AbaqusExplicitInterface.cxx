/*!
 * \file   AbaqusExplicitInterface.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 mars 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include<iostream>

#include"TFEL/Config/GetInstallPath.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/AbaqusExplicitInterface.hxx"

namespace mfront{
  
  //! copy vumat-sp.cpp and vumat-dp locally
  static void copyVUMATFiles(void)
  {
    std::ofstream out;
    MFrontLockGuard lock;
    for(const std::string f: {"vumat-sp.cpp","vumat-dp.cpp"}){
      out.open("abaqus-explicit/"+f);
      if(out){
	const auto root = tfel::getInstallPath();
	const auto fn = root+"/share/doc/mfront/abaqus/"+f;
	std::ifstream in{fn};
	if(in){
	  out << in.rdbuf();
	  in.close();
	} else {
	  std::cerr << "copyVUMATFiles: could not open file '" << fn << "'\n";
	}
      } else {
	std::cerr << "copyVUMATFiles: could not open file 'abaqus/" << f << "'\n";
      }
      out.close();
    }
  } // end of copyVUMATFiles
  
  static void
  writeVUMATArguments(std::ostream& out,
		      const std::string& type,
		      const bool f)
  {
    if(f){
      out << "(const abaqus::AbaqusInt *const nblock,\n"
	  << "const abaqus::AbaqusInt *const ndir,\n"
	  << "const abaqus::AbaqusInt *const nshr,\n"
	  << "const abaqus::AbaqusInt *const nstatev,\n"
	  << "const abaqus::AbaqusInt *const nfieldv,\n"
	  << "const abaqus::AbaqusInt *const nprops,\n"
	  << "const abaqus::AbaqusInt * const lanneal,\n"
	  << "const " << type << "* const stepTime,\n"
	  << "const " << type << "* const totalTime,\n"
	  << "const " << type << "* const dt,\n"
	  << "const char* const cmname,\n"
	  << "const " << type << "* const coordMp,\n"
	  << "const " << type << "* const charLength,\n"
	  << "const " << type << "* const props,\n"
	  << "const " << type << "* const density,\n"
	  << "      " << type << "* const strainInc,\n"
	  << "const " << type << "* const relSpinInc,\n"
	  << "const " << type << "* const tempOld,\n"
	  << "const " << type << "* const stretchOld,\n"
	  << "const " << type << "* const defgradOld,\n"
	  << "const " << type << "* const fieldOld,\n"
	  << "const " << type << "* const stressOld,\n"
	  << "const " << type << "* const stateOld,\n"
	  << "const " << type << "* const enerInternOld,\n"
	  << "const " << type << "* const enerInelasOld,\n"
	  << "const " << type << "* const tempNew,\n"
	  << "const " << type << "* const stretchNew,\n"
	  << "const " << type << "* const defgradNew,\n"
	  << "const " << type << "* const fieldNew,\n"
	  << type << "* const stressNew,\n"
	  << type << "* const stateNew,\n"
	  << type << "* const enerInternNew,\n"
	  << type << "* const enerInelasNew,\n"
	  << "const int)";
    } else {
      out << "(const abaqus::AbaqusInt *const nblock,\n"
	  << "const abaqus::AbaqusInt *const ndir,\n"
	  << "const abaqus::AbaqusInt *const nshr,\n"
	  << "const abaqus::AbaqusInt *const nstatev,\n"
	  << "const abaqus::AbaqusInt *const nfieldv,\n"
	  << "const abaqus::AbaqusInt *const nprops,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const " << type << "* const stepTime,\n"
	  << "const " << type << "* const totalTime,\n"
	  << "const " << type << "* const dt,\n"
	  << "const char* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const props,\n"
	  << "const " << type << "* const density,\n"
	  << "      " << type << "* const strainInc,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const tempOld,\n"
	  << "const " << type << "* const stretchOld,\n"
	  << "const " << type << "* const defgradOld,\n"
	  << "const " << type << "* const fieldOld,\n"
	  << "const " << type << "* const stressOld,\n"
	  << "const " << type << "* const stateOld,\n"
	  << "const " << type << "* const enerInternOld,\n"
	  << "const " << type << "* const enerInelasOld,\n"
	  << "const " << type << "* const tempNew,\n"
	  << "const " << type << "* const stretchNew,\n"
	  << "const " << type << "* const defgradNew,\n"
	  << "const " << type << "* const fieldNew,\n"
	  << type << "* const stressNew,\n"
	  << type << "* const stateNew,\n"
	  << type << "* const enerInternNew,\n"
	  << type << "* const enerInelasNew,\n"
	  << "const int)";
    }
  } // end of writeVUMATArguments

  static void
  writeVUMATArguments(std::ostream& out,
		      const std::string& type)
  {
      out << "(const abaqus::AbaqusInt *const,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const abaqus::AbaqusInt *const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const char* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "      " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << type << "* const,\n"
	  << type << "* const,\n"
	  << type << "* const,\n"
	  << type << "* const,\n"
	  << "const int)";
  } // end of writeVUMATArguments
  
  std::string
  AbaqusExplicitInterface::getName(void)
  {
    return "abaqusexplicit";
  }

  void
  AbaqusExplicitInterface::getTargetsDescription(TargetsDescription& d,
						 const BehaviourDescription& bd)
  {
    const auto lib  = this->getLibraryName(bd);
    const auto name = bd.getLibrary()+bd.getClassName(); 
#ifdef _WIN32
    const std::string tfel_config = "tfel-config.exe";
#else /* WIN32 */
    const std::string tfel_config = "tfel-config";
#endif /* WIN32 */
    insert_if(d[lib].cppflags,"$(shell "+tfel_config+" --includes)");
    insert_if(d[lib].sources,"abaqusexplicit"+name+".cxx");
    d.headers.push_back("MFront/Abaqus/abaqusexplicit"+name+".hxx");
    insert_if(d[lib].ldflags,"-lAbaqusInterface");
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    for(const auto h : this->getModellingHypothesesToBeTreated(bd)){
      insert_if(d[lib].epts,this->getFunctionNameForHypothesis(name,h));
    }
  } // end of AbaqusExplicitInterface::getTargetsDescription

  void 
  AbaqusExplicitInterface::writeInterfaceSpecificIncludes(std::ostream& out,
							  const BehaviourDescription&) const
  {
    out << "#include\"MFront/Abaqus/AbaqusExplicitInterface.hxx\"\n\n";
  } // end of AbaqusExplicitInterface::writeInterfaceSpecificIncludes
  
  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  AbaqusExplicitInterface::treatKeyword(const std::string& key,
					CxxTokenizer::TokensContainer::const_iterator current,
					const CxxTokenizer::TokensContainer::const_iterator end)
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AbaqusExplicitInterface::treatKeyword: "+m));}
    };
    auto read = [](const std::string& s){
      if(s=="FiniteRotationSmallStrain"){
	return FINITEROTATIONSMALLSTRAIN;
      } else if(s=="MieheApelLambrechtLogarithmicStrain"){
	return MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN;
      } else {
	throw(std::runtime_error("AbaqusExplicitInterface::treatKeyword: "
				 "unsupported strategy '"+s+"'\n"
				 "The only supported strategies are "
				 "'FiniteRotationSmallStrain' and "
				 "'MieheApelLambrechtLogarithmicStrain'"));
      }
    };
    if (key=="@AbaqusFiniteStrainStrategy"){
      throw_if(this->fss!=UNDEFINEDSTRATEGY,
	       "a finite strain strategy has already been defined");
      throw_if(current==end,"unexpected end of file");
      this->fss = read(current->value);
      throw_if(++current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+'\'');
      ++(current);
      return {true,current};
    }
    return {false,current};
  } // end of AbaqusExplicitInterface::treatKeyword

  void
  AbaqusExplicitInterface::endTreatment(const BehaviourDescription& mb,
					const FileDescription& fd) const
  {
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      throw(std::runtime_error("AbaqusExplicitInterface::endTreatment : "
			       "the abaqus explicit interface only supports small and "
			       "finite strain behaviours"));
    }
    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (this->fss==UNDEFINEDSTRATEGY)){
      throw(std::runtime_error("AbaqusExplicitInterface::endTreatment : "
			       "behaviours written in the small strain framework "
			       "must be embedded in a strain strategy. See the "
			       "'@AbaqusFiniteStrainStrategy' keyword"));
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/Abaqus");
    tfel::system::systemCall::mkdir("abaqus-explicit");
    copyVUMATFiles();
    // header
    auto fname = "abaqusexplicit"+name+".hxx";
    std::ofstream out("include/MFront/Abaqus/"+fname);
    if(!out){
      throw(std::runtime_error("AbaqusExplicitInterface::endTreatment : "
			       "could not open file '"+fname+"'"));
    }

    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file declares the abaqus explicit interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n"
	<< "#define "<< header << "\n\n"
	<< "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/Abaqus.hxx\"\n\n"
	<< "#ifdef __cplusplus\n"
	<< "#include\"MFront/Abaqus/AbaqusTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Abaqus/AbaqusOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
	<< "namespace abaqus{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(mh)){
      this->writeAbaqusBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->writeAbaqusBehaviourTraits(out,mb,h);
      }
    }

    out << "} // end of namespace abaqus\n\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#ifdef __cplusplus\n"
	<< "extern \"C\"{\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    for(const auto & h : mh){
      out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameForHypothesis(name,h) << "_f";
      writeVUMATArguments(out,"float");
      out << ";\n\n";

      out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameForHypothesis(name,h);
      writeVUMATArguments(out,"double");
      out << ";\n\n";
    }

    out << "#ifdef __cplusplus\n"
	<< "}\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#endif /* " << header << " */\n";

    out.close();

    fname  = "abaqusexplicit"+name+".cxx";
    out.open("src/"+fname);
    if(!out){
      throw(std::runtime_error("AbaqusExplicitInterface::endTreatment : "
			       "could not open file '"+fname+"'"));
    }

    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file implements the abaqus explicit interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    out << "#include<cmath>\n"
    	<< "#include<limits>\n"
	<< "#include<cstdlib>\n";
    this->getExtraSrcIncludes(out,mb);

    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
	<< "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/AbaqusExplicitInterface.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/abaqusexplicit" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
    
    out << "extern \"C\"{\n\n";
 
    this->generateUMATxxGeneralSymbols(out,name,mb,fd);
    if(!mb.areAllMechanicalDataSpecialised(mh)){
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out,name,uh,mb,fd);
    }
    for(const auto &h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->generateUMATxxSymbols(out,name,h,mb,fd);
      }
    }
    
    this->writeSetParametersFunctionsImplementations(out,name,mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);

    for(const auto h : mh){
      for(const std::string t : {"float","double"}){
	out << "MFRONT_SHAREDOBJ void\n"
	    << this->getFunctionNameForHypothesis(name,h);
	if(t=="float"){
	  out << "_f";
	}
	writeVUMATArguments(out,t,false);
	out << "{\n"
	    << "using namespace tfel::math;\n"
	    << "using ModellingHypothesis = tfel::material::ModellingHypothesis;\n"
	    << "using abaqus::AbaqusTraits;\n"
	    << "using tfel::material::" << mb.getClassName() << ";\n"
	    << "using AbaqusExplicitData = abaqus::AbaqusExplicitData<" << t << ">;\n"
	    << "static const " << t << " cste = std::sqrt(" << t << "{2});\n"
	    << "auto view = [&nblock](" << t << "* v){\n"
	    << "  return AbaqusExplicitData::strided_iterator(v,*nblock);\n"
	    << "};\n"
	    << "auto cview = [&nblock](const " << t << "* v){\n"
	    << "  return AbaqusExplicitData::strided_const_iterator(v,*nblock);\n"
	    << "};\n"
	    << "auto cdiffview = [&nblock](const " << t << "* v1,\n"
	    << "                           const " << t << "* v2){\n"
	    << "  return AbaqusExplicitData::diff_strided_const_iterator(AbaqusExplicitData::strided_const_iterator(v1,*nblock),\n"
	    << "                                                         AbaqusExplicitData::strided_const_iterator(v2,*nblock));\n"
	    << "};\n";
	if(mb.getAttribute(BehaviourData::profiling,false)){
	  out << "using mfront::BehaviourProfiler;\n"
	      << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	      << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	      << "BehaviourProfiler::TOTALTIME);\n";
	}
	this->writeChecks(out,mb,t,h);
	if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	  if(this->fss==FINITEROTATIONSMALLSTRAIN){
	    this->writeFiniteRotationSmallStrainBehaviourCall(out,mb,t,h);
	  } else if(this->fss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	    this->writeLogarithmicStrainBehaviourCall(out,mb,t,h);
	  } else {
	    throw(std::runtime_error("AbaqusExplicitInterface::writeVUMATFunction: "
				     "unsupported finite strain strategy (internal error)"));
	  }
	} else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	  this->writeFiniteStrainBehaviourCall(out,mb,t,h);
	} else {
	  throw(std::runtime_error("AbaqusExplicitInterface::writeVUMATFunction: "
				   "the abaqus explicit interface only supports small "
				   "and finite strain behaviours"));
	}
	out << "}\n\n";
      }
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb,fd,false);
  }

  static void writeAbaqusExplicitDataInitialisation(std::ostream& out,
						    const std::string& n){
    out << "const AbaqusExplicitData d = {*dt,props,*(density+i),\n"
	<< "                              *(tempOld+i),\n"
	<< "                              cview(fieldOld+i),cview(stateOld+i),\n"
	<< "                              *(enerInternOld+i),\n"
	<< "                              *(enerInelasOld+i),\n"
	<< "                              *(tempNew+i),\n"
	<< "                              cdiffview(fieldNew+i,fieldOld+i),\n"
	<< "                              view(stateNew+i),\n"
	<< "                              *(enerInternNew+i),\n"
	<< "                              *(enerInelasNew+i),\n"
	<< "                              " << n << "_getOutOfBoundsPolicy()" << "};\n";
  } // end of writeAbaqusExplicitDataInitialisation
  
  void
  AbaqusExplicitInterface::writeBehaviourConstructor(std::ostream& out,
						     const BehaviourDescription& mb,
						     const std::string& initStateVarsIncrements) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto av   = this->getBehaviourConstructorsAdditionalVariables();
    const auto abdv = this->getBehaviourDataConstructorAdditionalVariables();
    const auto aidv = this->getIntegrationDataConstructorAdditionalVariables();
    out << "/*\n"
	<< " * \\brief constructor for the abaqus explicit interface\n"
	<< " * \\param[in] " << iprefix << "d : data\n"
	<< " */\n"
	<< mb.getClassName() 
	<< "(const abaqus::AbaqusExplicitData<Type>& " << iprefix << "d)\n";
    if(mb.useQt()){
      out << ": " << mb.getClassName() 
	  << "BehaviourData<hypothesis,Type,use_qt>(" << iprefix << "d),\n"
	  << mb.getClassName() 
	  << "IntegrationData<hypothesis,Type,use_qt>(" << iprefix << "d)\n";
    } else {
      out << ": " << mb.getClassName() 
	  << "BehaviourData<hypothesis,Type,false>(" << iprefix << "d),\n"
	  << mb.getClassName() 
	  << "IntegrationData<hypothesis,Type,false>(" << iprefix << "d)\n";
    }
    if(!initStateVarsIncrements.empty()){
      out << ",\n" << initStateVarsIncrements;
    }
  } // end of AbaqusExplicitInterface::writeBehaviourConstructor

  void 
  AbaqusExplicitInterface::writeBehaviourDataConstructor(std::ostream& out,
							 const Hypothesis h,
							 const BehaviourDescription& mb) const
  {
    const auto& d = mb.getBehaviourData(h);
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto mprops = this->buildMaterialPropertiesList(mb,h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    out << "/*\n"
	<< " * \\brief constructor for the abaqus explicit interface\n"
	<< " * \\param[in] " << iprefix << "d : data\n"
	<< " */\n"
	<< mb.getClassName() << "BehaviourData"
	<< "(const abaqus::AbaqusExplicitData<Type>& " << iprefix << "d)\n"
	<< ": T(" << iprefix << "d.tempOld)";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorI(out,h,mb,mprops.first,
    									 mprops.second,
    									 iprefix+"d.props","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(out,persistentVarsHolder,
    							       iprefix+"d.stateOld","","");
    this->writeVariableInitializersInBehaviourDataConstructorI(out,externalStateVarsHolder,
    							       iprefix+"d.fieldOld","","");
    out << "\n{\n";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorII(out,h,mb,mprops.first,
    									  mprops.second,
    									  iprefix+"d.props","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(out,persistentVarsHolder,
    								iprefix+"d.stateOld","","");
    this->writeVariableInitializersInBehaviourDataConstructorII(out,externalStateVarsHolder,
    								iprefix+"d.fieldOld","","");
    this->completeBehaviourDataConstructor(out,h,mb);
    out << "}\n\n";
  }
  
  void 
  AbaqusExplicitInterface::writeIntegrationDataConstructor(std::ostream& out,
							   const Hypothesis h,
							   const BehaviourDescription& mb) const
  {
    const auto av   = this->getBehaviourConstructorsAdditionalVariables();
    const auto aidv = this->getIntegrationDataConstructorAdditionalVariables();
    const auto& d = mb.getBehaviourData(h);
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    out << "/*\n"
	<< " * \\brief constructor for the abaqus explicit interface\n"
	<< " * \\param[in] " << iprefix << "d : data"
	<< " */\n"
	<< mb.getClassName() << "IntegrationData"
	<< "(const abaqus::AbaqusExplicitData<Type>& " << iprefix << "d)"
	<< ": dt(" << iprefix << "d.dt),\n"
	<< "  dT(" << iprefix << "d.tempNew-" << iprefix << "d.tempOld)";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorI(out,externalStateVarsHolder,
    								 iprefix+"d.dfield","d","");
    }
    out << "\n{\n";
    if(!externalStateVarsHolder.empty()){
      this->writeVariableInitializersInBehaviourDataConstructorII(out,externalStateVarsHolder,
    								  iprefix+"d.dfield","d","");
    }
    out << "}\n\n";
  }

  void 
  AbaqusExplicitInterface::writeBehaviourDataMainVariablesSetters(std::ostream& os,
								  const BehaviourDescription& mb) const
  {
    auto throw_unsupported_hypothesis = [](){
      throw(std::runtime_error("AbaqusExplicitInterface::writeBehaviourDataMainVariablesSetters: "
			       "only small strain or finite behaviours are supported"));
    };
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if((mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (mb.getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw_unsupported_hypothesis();
    }
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      os << "void setBehaviourDataDrivingVariables(const Stensor& " << iprefix << "stran)\n"
	 << "{\n"
	 << "this->eto = " << iprefix << "stran;\n"
	 << "}\n\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      os << "void setBehaviourDataDrivingVariables(const Tensor& " << iprefix << "stran)\n"
	 << "{\n"
	 << "this->F0 = " << iprefix << "stran;\n"
	 << "}\n\n";
    }  else {
      throw_unsupported_hypothesis();
    }
    os << "void setBehaviourDataThermodynamicForces(const Stensor& " << iprefix << "stress)\n"
       << "{\n"
       << "this->sig = " << iprefix << "stress;\n"
       << "}\n\n";
  } // end of AbaqusExplicitInterface::writeBehaviourDataMainVariablesSetters

  void 
  AbaqusExplicitInterface::writeBehaviourDataDrivingVariableSetter(std::ostream&,
								   const DrivingVariable&,
								   const SupportedTypes::TypeSize) const
  {} // end of AbaqusExplicitInterface::writeBehaviourDataDrivingVariableSetter

  void 
  AbaqusExplicitInterface::writeBehaviourDataThermodynamicForceSetter(std::ostream&,
								      const ThermodynamicForce&,
								      const SupportedTypes::TypeSize) const
  {} // end of AbaqusExplicitInterface::writeBehaviourDataThermodynamicForceSetter
  
  void 
  AbaqusExplicitInterface::writeIntegrationDataMainVariablesSetters(std::ostream& os,
								    const BehaviourDescription& mb) const
  {
    auto throw_unsupported_hypothesis = [](){
      throw(std::runtime_error("AbaqusExplicitInterface::writeIntegrationDataMainVariablesSetters: "
			       "only small strain or finite behaviours are supported"));
    };
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if((mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (mb.getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw_unsupported_hypothesis();
    }
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      os << "void setIntegrationDataDrivingVariables(const Stensor& " << iprefix << "dstran)\n"
	 << "{\n"
	 << "this->deto = " << iprefix << "dstran;\n"
	 << "}\n\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      os << "void setIntegrationDataDrivingVariables(const Tensor& " << iprefix << "dstran)\n"
	 << "{\n"
	 << "this->F1 = " << iprefix << "dstran;\n"
	 << "}\n\n";
    } else {
      throw_unsupported_hypothesis();
    }
  } // end of writeIntegrationDataMainVariablesSetters

  void 
  AbaqusExplicitInterface::writeIntegrationDataDrivingVariableSetter(std::ostream&,
								     const DrivingVariable&,
								     const SupportedTypes::TypeSize) const
  {} // end of AbaqusExplicitInterface::writeIntegrationDataDrivingVariableSetter

  void 
  AbaqusExplicitInterface::exportMechanicalData(std::ostream& out,
						const Hypothesis h,
						const BehaviourDescription& mb) const
  {
    const auto& d       = mb.getBehaviourData(h);
    const auto& ivs     = d.getPersistentVariables();
    const auto  iprefix = makeUpperCase(this->getInterfaceName());
    if(!ivs.empty()){
      out << "void exportStateData("
	  << "Stensor& " << iprefix+"s, const abaqus::AbaqusExplicitData<Type>& " << iprefix+"d) const\n";
    } else {
      out << "void exportStateData("
	  << "Stensor& " << iprefix+"s, const abaqus::AbaqusExplicitData<Type>&) const\n";
    }
    out << "{\n"
	<< "using namespace tfel::math;\n"
	<< iprefix+"s = " << "this->sig;\n";
    if(!ivs.empty()){
      this->exportResults(out,ivs,iprefix+"d.stateNew",mb.useQt());
    }
    out << "} // end of " << iprefix << "exportStateData\n\n";
  }
  
  void
  AbaqusExplicitInterface::writeChecks(std::ostream& out,
				       const BehaviourDescription& mb,
				       const std::string& t,
				       const Hypothesis h) const
  {
    out << "using BV = " << mb.getClassName()
	<< "<ModellingHypothesis::"<< ModellingHypothesis::toUpperCaseString(h) << "," << t << ",false>;\n"
	<< "using Traits = tfel::material::MechanicalBehaviourTraits<BV>;\n"
	<< "constexpr const unsigned short offset  = (AbaqusTraits<BV>::elasticPropertiesOffset+\n"
	<< "                                          AbaqusTraits<BV>::thermalExpansionPropertiesOffset);\n"
	<< "constexpr const unsigned short nprops_  = AbaqusTraits<BV>::material_properties_nb;\n"
	<< "constexpr const unsigned short NPROPS_  = offset+nprops_ == 0 ? 1u : offset+nprops_;\n"
	<< "constexpr const unsigned short nstatev_ = Traits::internal_variables_nb;\n"
	<< "constexpr const unsigned short nfieldv_ = Traits::external_variables_nb;\n";
    if((h==tfel::material::ModellingHypothesis::AXISYMMETRICAL)||
       (h==tfel::material::ModellingHypothesis::PLANESTRAIN)||
       (h==tfel::material::ModellingHypothesis::PLANESTRESS)){
      out << "if(*ndir+*nshr!=4){\n"
	  << "std::cerr << \"" << mb.getClassName() << ":"
	  << " invalid number of components for symmetric tensors "
	  << "(\" << *ndir+*nshr << \" given, \" << 4 << \" expected)\\n\";\n"
	  << "::exit(-1);\n"
	  << "}\n";
    } else if(h==tfel::material::ModellingHypothesis::TRIDIMENSIONAL){
      out << "if(*ndir+*nshr!=6){\n"
	  << "std::cerr << \"" << mb.getClassName() << ":"
	  << " invalid number of components for symmetric tensors "
	  << "(\" << *ndir+*nshr << \" given, \" << 6 << \" expected)\\n\";\n"
	  << "::exit(-1);\n"
	  << "}\n";
    } else {
      throw(std::runtime_error("AbaqusExplicitInterface::writeChecks: "
			       "unsupported hypothesis"));
    }
    out << "if(*nprops!=NPROPS_){\n"
	<< "std::cerr << \"" << mb.getClassName() << ":"
	<< " unmatched number of material properties "
	<< "(\" << *nprops << \" given, \" << NPROPS_ << \" expected)\\n\";\n"
	<< "::exit(-1);\n"
	<< "}\n"
	<< "if(*nstatev!=nstatev_){\n"
	<< "std::cerr << \"" << mb.getClassName() << ":"
	<< " unmatched number of internal state variables "
	<< "(\" << *nstatev << \" given, \" << nstatev_ << \" expected)\\n\";\n"
	<< "::exit(-1);\n"
	<< "}\n"
	<< "if(*nfieldv!=nfieldv_){\n"
	<< "std::cerr << \"" << mb.getClassName() << ":"
	<< " unmatched number of external state variables "
	<< "(\" << *nfieldv << \" given, \" << nfieldv_ << \" expected)\\n\";\n"
	<< "::exit(-1);\n"
	<< "}\n";
  } // end of AbaqusExplicitInterface::writeChecks

  void
  AbaqusExplicitInterface::writeComputeElasticPrediction(std::ostream& out,
							 const BehaviourDescription& mb,
							 const std::string& t,
							 const Hypothesis h) const
  {
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    if(mh.find(h)==mh.end()){
      out << "std::cerr << \"" << mb.getClassName() << ": unsupported hypothesis\\n\";\n"
	  << "::exit(-1);\n";
    }
    //omp support    #pragma omp parallel for
    out << "for(int i=0;i!=*nblock;++i){\n";
    writeAbaqusExplicitDataInitialisation(out,this->getFunctionName(name));
    if(h==ModellingHypothesis::PLANESTRESS){
      out << t << " eto[3u]  = {*(strainInc+i),\n"
	  << "*(strainInc+i+(*nblock)),\n"
	  << "2*(*(strainInc+i+2*(*nblock)))};\n"
	  << t << " D[9u];\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << t << " eto[4u]  = {*(strainInc+i),\n"
	  << "*(strainInc+i+  (*nblock)),\n"
	  << "*(strainInc+i+2*(*nblock)),\n"
	  << "2*(*(strainInc+i+3*(*nblock)))};\n"
	  << t << " D[16u];\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << t << " eto[6u]  = {*(strainInc+i),\n"
	  << "*(strainInc+i+  (*nblock)),\n"
	  << "*(strainInc+i+2*(*nblock)),\n"
	  << "2*(*(strainInc+i+3*(*nblock))),\n"
	  << "2*(*(strainInc+i+5*(*nblock))),\n"
	  << "2*(*(strainInc+i+4*(*nblock)))};\n"
	  << t << " D[36u];\n";
    } else {
      throw(std::runtime_error("AbaqusExplicitInterface::writeComputeElasticPrediction: "
			       "unsupported hypothesis '"+ModellingHypothesis::toString(h)+"'"));
    }
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "if(abaqus::AbaqusExplicitInterface<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ","
	  << t << "," << mb.getClassName()
	  << ">::computeElasticPrediction(D,d)!=0){\n"
	  << "std::cerr << \"" << mb.getClassName() << ": elastic loading failed\\n\";\n"
	  << "::exit(-1);\n"
	  << "}\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "if(abaqus::AbaqusExplicitInterface<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ","
	  << t << "," << mb.getClassName()
	  << ">::computeElasticPrediction2(D,d)!=0){\n"
	  << "std::cerr << \"" << mb.getClassName() << ": elastic loading failed\\n\";\n"
	  << "::exit(-1);\n"
	  << "}\n";
    } else {
      throw(std::runtime_error("AbaqusExplicitInterface::writeComputeElasticPrediction: "
			       "unsupported behaviour type"));
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      out << "*(stressNew+i)               = D[0]*eto[0]+D[3]*eto[1]+D[6]*eto[2];\n";
      out << "*(stressNew+i+   *(nblock))  = D[1]*eto[0]+D[4]*eto[1]+D[7]*eto[2];\n";
      out << "*(stressNew+i+2*(*(nblock))) = D[2]*eto[0]+D[5]*eto[1]+D[8]*eto[2];\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "*(stressNew+i)               = D[0]*eto[0]+D[4]*eto[1]+D[8]*eto[2] +D[12]*eto[3];\n";
      out << "*(stressNew+i+   *(nblock))  = D[1]*eto[0]+D[5]*eto[1]+D[9]*eto[2] +D[13]*eto[3];\n";
      out << "*(stressNew+i+2*(*(nblock))) = D[2]*eto[0]+D[6]*eto[1]+D[10]*eto[2]+D[14]*eto[3];\n";
      out << "*(stressNew+i+3*(*(nblock))) = D[3]*eto[0]+D[7]*eto[1]+D[11]*eto[2]+D[15]*eto[3];\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "*(stressNew+i)               = D[0]*eto[0]+D[6]*eto[1] +D[12]*eto[2]+D[18]*eto[3]+D[24]*eto[4]+D[30]*eto[5];\n";
      out << "*(stressNew+i+   *(nblock))  = D[1]*eto[0]+D[7]*eto[1] +D[13]*eto[2]+D[19]*eto[3]+D[25]*eto[4]+D[31]*eto[5];\n";
      out << "*(stressNew+i+2*(*(nblock))) = D[2]*eto[0]+D[8]*eto[1] +D[14]*eto[2]+D[20]*eto[3]+D[26]*eto[4]+D[32]*eto[5];\n";
      out << "*(stressNew+i+3*(*(nblock))) = D[3]*eto[0]+D[9]*eto[1] +D[15]*eto[2]+D[21]*eto[3]+D[27]*eto[4]+D[33]*eto[5];\n";
      out << "*(stressNew+i+4*(*(nblock))) = D[4]*eto[0]+D[10]*eto[1]+D[16]*eto[2]+D[22]*eto[3]+D[28]*eto[4]+D[34]*eto[5];\n";
      out << "*(stressNew+i+5*(*(nblock))) = D[5]*eto[0]+D[11]*eto[1]+D[17]*eto[2]+D[23]*eto[3]+D[29]*eto[4]+D[35]*eto[5];\n";
    } else {
      throw(std::runtime_error("AbaqusExplicitInterface::writeComputeElasticPrediction: "
			       "unsupported hypothesis '"+ModellingHypothesis::toString(h)+"'"));
    }
    out << "}\n";
  } // end of AbaqusExplicitInterface::writeComputeElasticPrediction

  void
  AbaqusExplicitInterface::writeFiniteRotationSmallStrainIntegration(std::ostream& out,
								     const BehaviourDescription& mb,
								     const std::string& t,
								     const Hypothesis h) const
  {
    auto throw_unsupported_hypothesis = [](){
      throw(std::runtime_error("AbaqusExplicitInterface::writeFiniteRotationSmallStrainIntegration: "
			       "internal error, unsupported hypothesis"));
    };
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    if(mh.find(h)==mh.end()){
      out << "std::cerr << \"" << mb.getClassName() << ": unsupported hypothesis\";\n"
  	  << "::exit(-1);\n";
      return;
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      // axial strain !
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if(!v.first){
	// no axial strain
	out << "std::cerr << \"no state variable standing for the axial strain (variable with the "
	    << "glossary name 'AxialStrain')\" << std::endl;\n";
	out << "::exit(-1);\n";
	return;
      }
    }
    //omp support    #pragma omp parallel for
    out << "for(int i=0;i!=*nblock;++i){\n";
    writeAbaqusExplicitDataInitialisation(out,this->getFunctionName(name));
    if(h==ModellingHypothesis::PLANESTRESS){
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      out << "constexpr const " << t << " zero = " << t <<  "(0);\n"
	  << "const " << t << " ezz_old = "
	  << "stateOld[i+" << v.second.getValueForDimension(2) << "*(*nblock)];\n"
	  << "stensor<2u," << t << "> U0 = {*(stretchOld+i),*(stretchOld+i+*nblock),\n"
	  << "                              zero,cste*(*(stretchOld+i+2*(*nblock)))};\n"
	  << "stensor<2u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                              zero,cste*(*(stretchNew+i+2*(*nblock)))};\n"
	  << "const stensor<2u," << t << "> eto  = (square(U0)-stensor<2u," << t << ">::Id)/2;\n"
	  << "const stensor<2u," << t << "> deto = (square(U1)-stensor<2u," << t << ">::Id())/2-eto;\n\n"
	  << "stensor<2u," << t << "> s    = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                                zero,cste*(*(stressOld+i+2*(*nblock)))};\n"
	// on affecte ezz à U0, sa valeur pour U1 étant inconnue à ce moment (ne sert à rien pour le calcul de eto et deto
	  << "U0[2] = std::sqrt(1+2*ezz_old);\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "const stensor<2u," << t << "> U0 = {*(stretchOld+i),*(stretchOld+i+*nblock),\n"
	  << "                                    *(stretchOld+i+2*(*nblock)),cste*(*(stretchOld+i+3*(*nblock)))};\n"
	  << "const stensor<2u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    *(stretchNew+i+2*(*nblock)),cste*(*(stretchNew+i+3*(*nblock)))};\n"
	  << "const stensor<2u," << t << "> eto  = (square(U0)-stensor<2u," << t << ">::Id())/2;\n"
	  << "const stensor<2u," << t << "> deto = (square(U1)-stensor<2u," << t << ">::Id())/2-eto;\n"
	  << "stensor<2u," << t << "> s  = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                             *(stressOld+i+2*(*nblock)),cste*(*(stressOld+i+3*(*nblock)))};\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "const stensor<3u," << t << "> U0 = {*(stretchOld+i),*(stretchOld+i+*nblock),\n"
	  << "                                    *(stretchOld+i+2*(*nblock)),       cste*(*(stretchOld+i+3*(*nblock))),\n"
	  << "                                    cste*(*(stretchOld+i+5*(*nblock))),cste*(*(stretchOld+i+4*(*nblock)))};\n"
	  << "const stensor<3u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    *(stretchNew+i+2*(*nblock)),cste*(*(stretchNew+i+3*(*nblock))),\n"
	  << "                                    cste*(*(stretchNew+i+5*(*nblock))),cste*(*(stretchNew+i+4*(*nblock)))};\n"
	  << "const stensor<3u," << t << "> eto  = (square(U0)-stensor<3u," << t << ">::Id())/2;\n"
	  << "const stensor<3u," << t << "> deto = (square(U1)-stensor<3u," << t << ">::Id())/2-eto;\n"
	  << "stensor<3u," << t << "> s  = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                              *(stressOld+i+2*(*nblock)),cste*(*(stressOld+i+3*(*nblock))),\n"
	  << "                              cste*(*(stressOld+i+5*(*nblock))),cste*(*(stressOld+i+4*(*nblock)))};\n";
    } else {
      throw_unsupported_hypothesis();
    }
    out << "auto sk2 = convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(s,U0);\n";
    if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	(h==ModellingHypothesis::PLANESTRAIN)||
	(h==ModellingHypothesis::PLANESTRESS)){
      out << "auto sfeh = [](tfel::math::stensor<2u,"<< t<< ">& e,\n"
	  << "tfel::math::stensor<2u,"<< t<< ">& de,\n"
	  << "const tfel::math::stensor<2u,"<< t<< ">& dl0_l0,\n"
	  << "const tfel::math::stensor<2u,"<< t<< ">& dl1_l0){\n"
	  << "e -=dl0_l0;\n"
	  << "de-=dl1_l0-dl0_l0;\n"
	  << "};\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "auto sfeh = [](tfel::math::stensor<3u,"<< t<< ">& e,\n"
	  << "tfel::math::stensor<3u,"<< t<< ">& de,\n"
	  << "const tfel::math::stensor<3u,"<< t<< ">& dl0_l0,\n"
	  << "const tfel::math::stensor<3u,"<< t<< ">& dl1_l0){\n"
	  << "e -=dl0_l0;\n"
	  << "de-=dl1_l0-dl0_l0;\n"
	  << "};\n";
    } else {
      throw_unsupported_hypothesis();
    }
    out << "if(abaqus::AbaqusExplicitInterface<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ","
	<< t<< "," << mb.getClassName()
	<< ">::integrate(sk2,d,eto,deto,sfeh)!=0){\n"
	<< "std::cerr << \"" << mb.getClassName() << ": behaviour integration failed\";\n"
	<< "::exit(-1);\n"
	<< "}\n";
    if(h==ModellingHypothesis::PLANESTRESS){
      // axial strain !
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if(v.first){
	out << "const " << t << " ezz_new = "
	    << "stateNew[i+" << v.second.getValueForDimension(2) << "*(*nblock)];\n"
	    << "U1[2] = std::sqrt(1+2*ezz_new);\n";
      } else {
	// no axial strain
	out << "std::cerr << \"no state variable standing for the axial strain (variable with the "
	    << "glossary name 'AxialStrain')\" << std::endl;\n";
	out << "::exit(-1);\n";
      }
    }
    out << "s = convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(sk2,U1);\n";
    if(h==ModellingHypothesis::PLANESTRESS){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[3]/cste;\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[2];\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[2];\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
      out << "*(stressNew+i+4*(*(nblock))) = s[5]/cste;\n";
      out << "*(stressNew+i+5*(*(nblock))) = s[4]/cste;\n";
    }
    out << "}\n";
  }
  
  void
  AbaqusExplicitInterface::writeFiniteRotationSmallStrainBehaviourCall(std::ostream& out,
								       const BehaviourDescription& mb,
								       const std::string& t,
								       const Hypothesis h) const
  {
    // datacheck phase
    out << "static_cast<void>(defgradOld);\n"
	<< "static_cast<void>(defgradNew);\n"
	<< "if((std::abs(*stepTime)<std::numeric_limits<" << t << ">::min())&&\n"
	<< "   (std::abs(*totalTime)<std::numeric_limits<" << t << ">::min())){\n"
	<< "// packaging step\n";
    this->writeComputeElasticPrediction(out,mb,t,h);
    out << "} else {\n"
	<< "// behaviour integration\n";
    this->writeFiniteRotationSmallStrainIntegration(out,mb,t,h);
    out << "}\n";      
  }

  void
  AbaqusExplicitInterface::writeLogarithmicStrainIntegration(std::ostream& out,
							     const BehaviourDescription& mb,
							     const std::string& t,
							     const Hypothesis h) const
  {
    auto throw_unsupported_hypothesis = [](){
      throw(std::runtime_error("AbaqusExplicitInterface::writeLogarithmicStrainIntegration: "
			       "internal error, unsupported hypothesis"));
    };
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    if(mh.find(h)==mh.end()){
      out << "std::cerr << \"" << mb.getClassName() << ": unsupported hypothesis\";\n"
  	  << "::exit(-1);\n";
      return;
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      // axial strain !
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if(!v.first){
	// no axial strain
	out << "std::cerr << \"no state variable standing for the axial strain (variable with the "
	    << "glossary name 'AxialStrain')\" << std::endl;\n";
	out << "::exit(-1);\n";
	return;
      }
    }
    out << "const auto  f = [](const " << t << " x){return std::log(x)/2;};\n"
	<< "const auto df = [](const " << t << " x){return 1/(2*x);};\n"
    //omp support    #pragma omp parallel for
	<< "for(int i=0;i!=*nblock;++i){\n";
    writeAbaqusExplicitDataInitialisation(out,this->getFunctionName(name));
    auto dime = (h==ModellingHypothesis::TRIDIMENSIONAL) ? "3u" : "2u";
    if(h==ModellingHypothesis::PLANESTRESS){
      out << "constexpr const " << t << " zero = " << t <<  "(0);\n"
	  << "constexpr const " << t << " one  = " << t <<  "(1);\n"
	  << "stensor<2u," << t << "> U0 = {*(stretchOld+i),*(stretchOld+i+*nblock),\n"
	  << "                              one,cste*(*(stretchOld+i+3*(*nblock)))};\n"
	  << "stensor<2u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                              one,cste*(*(stretchNew+i+3*(*nblock)))};\n"
	  << "stensor<2u," << t << "> s    = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                                zero,cste*(*(stressOld+i+3*(*nblock)))};\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "const stensor<2u," << t << "> U0 = {*(stretchOld+i),*(stretchOld+i+*nblock),\n"
	  << "                                    *(stretchOld+i+2*(*nblock)),cste*(*(stretchOld+i+3*(*nblock)))};\n"
	  << "const stensor<2u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    *(stretchNew+i+2*(*nblock)),cste*(*(stretchNew+i+3*(*nblock)))};\n"
	  << "stensor<2u," << t << "> s  = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                             *(stressOld+i+2*(*nblock)),cste*(*(stressOld+i+3*(*nblock)))};\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "const stensor<3u," << t << "> U0 = {*(stretchOld+i),*(stretchOld+i+*nblock),\n"
	  << "                                    *(stretchOld+i+2*(*nblock)),       cste*(*(stretchOld+i+3*(*nblock))),\n"
	  << "                                    cste*(*(stretchOld+i+5*(*nblock))),cste*(*(stretchOld+i+4*(*nblock)))};\n"
	  << "const stensor<3u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    *(stretchNew+i+2*(*nblock)),cste*(*(stretchNew+i+3*(*nblock))),\n"
	  << "                                    cste*(*(stretchNew+i+5*(*nblock))),cste*(*(stretchNew+i+4*(*nblock)))};\n"
	  << "stensor<3u," << t << "> s  = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                              *(stressOld+i+2*(*nblock)),cste*(*(stressOld+i+3*(*nblock))),\n"
	  << "                              cste*(*(stressOld+i+5*(*nblock))),cste*(*(stressOld+i+4*(*nblock)))};\n";
    }
    out << "st2tost2<" << dime << "," << t << "> P0;\n"
	<< "st2tost2<" << dime << "," << t << "> P1;\n"
	<< "const stensor<" << dime << "," << t << "> C0  = square(U0);\n"
	<< "const stensor<" << dime << "," << t << "> C1  = square(U1);\n"
	<< "stensor<" << dime << "," << t << "> eto;\n"
	<< "stensor<" << dime << "," << t << "> deto;\n"
	<< "std::tie(eto ,P0) = C0.computeIsotropicFunctionAndDerivative(f,df,std::numeric_limits<" << t << ">::epsilon());\n"
	<< "std::tie(deto,P1) = C1.computeIsotropicFunctionAndDerivative(f,df,std::numeric_limits<" << t << ">::epsilon());\n"
	<< "deto -= eto;\n";
    if(h==ModellingHypothesis::PLANESTRESS){
      // on affecte ezz à U0, sa valeur pour U1 étant inconnue à ce moment (ne sert à rien pour le calcul de eto et deto
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      out << "const " << t << " ezz_old = "
	  << "stateOld[i+" << v.second.getValueForDimension(2) << "*(*nblock)];\n"
	  << "U0[2]    = std::exp(ezz_old);\n"
	  << "P0(2,2) += df(U0[2]*U0[2]);\n";
    }
    out << "const auto iP0 = invert(P0);\n"
	<< "auto sk2 = convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(s,U0);\n"
	<< "stensor<" << dime << ","<< t << "> T = (sk2|iP0)/2;\n";
    if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	(h==ModellingHypothesis::PLANESTRAIN)||
	(h==ModellingHypothesis::PLANESTRESS)){
      out << "auto sfeh = [](tfel::math::stensor<2u,"<< t<< ">& e,\n"
	  << "tfel::math::stensor<2u,"<< t<< ">& de,\n"
	  << "const tfel::math::stensor<2u,"<< t<< ">& dl0_l0,\n"
	  << "const tfel::math::stensor<2u,"<< t<< ">& dl1_l0){\n"
	  << "if((std::abs(dl0_l0[3])>10*std::numeric_limits<" << t << ">::min())||\n"
	  << "   (std::abs(dl1_l0[3])>10*std::numeric_limits<" << t << ">::min())){\n"
	  << "  std::cerr << \"" << mb.getClassName() << ": stress free expansion is assumed to be diagonal\" << std::endl;\n"
	  << "  std::exit(EXIT_FAILURE);\n"
	  << "}\n"
	  << "e[0] -=std::log(1+dl0_l0[0]);\n"
	  << "de[0]-=std::log((1+dl1_l0[0])/(1+dl0_l0[0]));\n"
	  << "e[1] -=std::log(1+dl0_l0[1]);\n"
	  << "de[1]-=std::log((1+dl1_l0[1])/(1+dl0_l0[1]));\n"
	  << "e[2] -=std::log(1+dl0_l0[2]);\n"
	  << "de[2]-=std::log((1+dl1_l0[2])/(1+dl0_l0[2]));\n"
	  << "};\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "auto sfeh = [](tfel::math::stensor<3u,"<< t<< ">& e,\n"
	  << "tfel::math::stensor<3u,"<< t<< ">& de,\n"
	  << "const tfel::math::stensor<3u,"<< t<< ">& dl0_l0,\n"
	  << "const tfel::math::stensor<3u,"<< t<< ">& dl1_l0){\n"
	  << "if((std::abs(dl0_l0[3])>10*std::numeric_limits<" << t << ">::min())||\n"
	  << "   (std::abs(dl1_l0[3])>10*std::numeric_limits<" << t << ">::min())||"
	  << "   (std::abs(dl0_l0[4])>10*std::numeric_limits<" << t << ">::min())||"
	  << "   (std::abs(dl1_l0[4])>10*std::numeric_limits<" << t << ">::min())||"
	  << "   (std::abs(dl0_l0[5])>10*std::numeric_limits<" << t << ">::min())||"
	  << "   (std::abs(dl1_l0[5])>10*std::numeric_limits<" << t << ">::min())){"
	  << "  std::cerr << \"" << mb.getClassName() << ": stress free expansion is assumed to be diagonal\" << std::endl;\n"
	  << "  std::exit(EXIT_FAILURE);\n"
	  << "}\n"
	  << "e[0] -=std::log(1+dl0_l0[0]);\n"
	  << "de[0]-=std::log((1+dl1_l0[0])/(1+dl0_l0[0]));\n"
	  << "e[1] -=std::log(1+dl0_l0[1]);\n"
	  << "de[1]-=std::log((1+dl1_l0[1])/(1+dl0_l0[1]));\n"
	  << "e[2] -=std::log(1+dl0_l0[2]);\n"
	  << "de[2]-=std::log((1+dl1_l0[2])/(1+dl0_l0[2]));\n"
	  << "};\n";
    } else {
      throw_unsupported_hypothesis();
    }
    out << "if(abaqus::AbaqusExplicitInterface<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ","
	<< t<< "," << mb.getClassName()
	<< ">::integrate(T,d,eto,deto,sfeh)!=0){\n"
	<< "std::cerr << \"" << mb.getClassName() << ": behaviour integration failed\";\n"
	<< "::exit(-1);\n"
	<< "}\n";
    if(h==ModellingHypothesis::PLANESTRESS){
      // axial strain !
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if(v.first){
	out << "const " << t << " ezz_new = "
	    << "stateNew[i+" << v.second.getValueForDimension(2) << "*(*nblock)];\n"
	    << "U1[2]    = std::exp(ezz_new);\n"
	    << "P1(2,2) += df(U1[2]*U1[2]);\n"
	    << "//strain update\n"
	    << "*(strainInc+i+2*(*(nblock))) = ezz_new-ezz_old;\n";
      } else {
	// no axial strain
	out << "std::cerr << \"no state variable standing for the axial strain (variable with the "
	    << "glossary name 'AxialStrain')\" << std::endl;\n";
	out << "::exit(-1);\n";
      }
    }
    out << "// stress update\n"
	<< "sk2 = 2*(T|P1);\n"
	<< "s = convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(sk2,U1);\n";
    if(h==ModellingHypothesis::PLANESTRESS){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = zero;\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[2];\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[2];\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
      out << "*(stressNew+i+4*(*(nblock))) = s[5]/cste;\n";
      out << "*(stressNew+i+5*(*(nblock))) = s[4]/cste;\n";
    }
    out << "}\n";
  }
  
  void
  AbaqusExplicitInterface::writeLogarithmicStrainBehaviourCall(std::ostream& out,
							       const BehaviourDescription& mb,
							       const std::string& t,
							       const Hypothesis h) const
  {
    // datacheck phase
    out << "static_cast<void>(defgradOld);\n"
	<< "static_cast<void>(defgradNew);\n"
	<< "if((std::abs(*stepTime)<std::numeric_limits<" << t << ">::min())&&\n"
	<< "   (std::abs(*totalTime)<std::numeric_limits<" << t << ">::min())){\n"
	<< "// packaging step\n";
    this->writeComputeElasticPrediction(out,mb,t,h);
    out << "} else {\n"
	<< "// behaviour integration\n";
    this->writeLogarithmicStrainIntegration(out,mb,t,h);
    out << "}\n";      
  }
  
  void
  AbaqusExplicitInterface::writeFiniteStrainBehaviourCall(std::ostream& out,
							  const BehaviourDescription& mb,
							  const std::string& t,
							  const Hypothesis h) const
  {
    // datacheck phase
    out << "static_cast<void>(stretchOld);\n"
	<< "if((std::abs(*stepTime)<std::numeric_limits<" << t << ">::min())&&\n"
	<< "   (std::abs(*totalTime)<std::numeric_limits<" << t << ">::min())){\n"
	<< "// packaging step\n";
    this->writeComputeElasticPrediction(out,mb,t,h);
    out << "} else {\n"
	<< "// behaviour integration\n";
    this->writeFiniteStrainIntegration(out,mb,t,h);
    out << "}\n";      
  } // end of AbaqusExplicitInterface::endTreatment

  void
  AbaqusExplicitInterface::writeFiniteStrainIntegration(std::ostream& out,
							const BehaviourDescription& mb,
							const std::string& t,
							const Hypothesis h) const
  {
    const auto name =  mb.getLibrary()+mb.getClassName();
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    if(mh.find(h)==mh.end()){
      out << "std::cerr << \"" << mb.getClassName() << ": unsupported hypothesis\";\n"
  	  << "::exit(-1);\n";
      return;
    }
    //omp support    #pragma omp parallel for
    out << "for(int i=0;i!=*nblock;++i){\n";
    writeAbaqusExplicitDataInitialisation(out,this->getFunctionName(name));
    if(h==ModellingHypothesis::PLANESTRESS){
      // les composantes axiales sont mises à l'identité pour pouvoir
      // réaliser le calcul de la rotation
      out << "const stensor<2u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    1,cste*(*(stretchNew+i+2*(*nblock)))};\n"
	  << "const tensor<2u," << t << "> F0 = {*(defgradOld+i),*(defgradOld+i+*nblock),1,\n"
	  << "                                   *(defgradOld+i+2*(*nblock)),\n"
	  << "                                   *(defgradOld+i+3*(*nblock))};\n"
	  << "const tensor<2u," << t << "> F1 = {*(defgradNew+i),*(defgradNew+i+*nblock),1,\n"
	  << "                                   *(defgradNew+i+2*(*nblock)),\n"
	  << "                                   *(defgradNew+i+3*(*nblock))};\n"
	  << "stensor<2u," << t << "> s = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                             0,cste*(*(stressOld+i+2*(*nblock)))};\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "const stensor<2u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    *(stretchNew+i+2*(*nblock)),\n"
	  << "                                    cste*(*(stretchNew+i+3*(*nblock)))};\n"
	  << "const tensor<2u," << t << "> F0 = {*(defgradOld+i),*(defgradOld+i+*nblock),\n"
	  << "                                   *(defgradOld+i+2*(*nblock)),\n"
	  << "                                   *(defgradOld+i+3*(*nblock)),\n"
	  << "                                   *(defgradOld+i+4*(*nblock))};\n"
	  << "const tensor<2u," << t << "> F1 = {*(defgradNew+i),*(defgradNew+i+*nblock),\n"
	  << "                                   *(defgradNew+i+2*(*nblock)),\n"
	  << "                                   *(defgradNew+i+3*(*nblock)),\n"
	  << "                                   *(defgradNew+i+4*(*nblock))};\n"
	  << "stensor<2u," << t << "> s  = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                              *(stressOld+i+2*(*nblock)),\n"
	  << "                              cste*(*(stressOld+i+3*(*nblock)))};\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      // vumat conventions : F11,F22,F33,F12,F23,F31,F21,F32,F13
      //                       0   1   2   3   4   5   6   7   8
      out << "const stensor<3u," << t << "> U1 = {*(stretchNew+i),*(stretchNew+i+*nblock),\n"
	  << "                                    *(stretchNew+i+2*(*nblock)),\n"
	  << "                                    cste*(*(stretchNew+i+3*(*nblock))),\n"
	  << "                                    cste*(*(stretchNew+i+5*(*nblock))),\n"
	  << "                                    cste*(*(stretchNew+i+4*(*nblock)))};\n"
	  << "const tensor<3u," << t << "> F0 = {*(defgradOld+i),\n"
	  << "                                   *(defgradOld+i+*nblock),\n"
	  << "                                   *(defgradOld+i+2*(*nblock)),\n"
	  << "                                   *(defgradOld+i+3*(*nblock)),\n"
	  << "                                   *(defgradOld+i+6*(*nblock)),\n"
	  << "                                   *(defgradOld+i+8*(*nblock)),\n"
	  << "                                   *(defgradOld+i+5*(*nblock)),\n"
	  << "                                   *(defgradOld+i+4*(*nblock)),\n"
	  << "                                   *(defgradOld+i+7*(*nblock))};\n"
	  << "const tensor<3u," << t << "> F1 = {*(defgradNew+i),\n"
	  << "                                   *(defgradNew+i+*nblock),\n"
	  << "                                   *(defgradNew+i+2*(*nblock)),\n"
	  << "                                   *(defgradNew+i+3*(*nblock)),\n"
	  << "                                   *(defgradNew+i+6*(*nblock)),\n"
	  << "                                   *(defgradNew+i+8*(*nblock)),\n"
	  << "                                   *(defgradNew+i+5*(*nblock)),\n"
	  << "                                   *(defgradNew+i+4*(*nblock)),\n"
	  << "                                   *(defgradNew+i+7*(*nblock))};\n"
	  << "stensor<3u," << t << "> s  = {*(stressOld+i),*(stressOld+i+*nblock),\n"
	  << "                              *(stressOld+i+2*(*nblock)),\n"
	  << "                              cste*(*(stressOld+i+3*(*nblock))),\n"
	  << "                              cste*(*(stressOld+i+5*(*nblock))),\n"
	  << "                              cste*(*(stressOld+i+4*(*nblock)))};\n";
    }
    out << "const tmatrix<3u,3u," << t << "> R1 = matrix_view(F1*invert(U1));\n"
      	<< "s.changeBasis(transpose(R1));\n"
	<< "if(abaqus::AbaqusExplicitInterface<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ","
	<< t<< "," << mb.getClassName()
	<< ">::integrate(s,d,F0,F1)!=0){\n"
	<< "std::cerr << \"" << mb.getClassName() << ": behaviour integration failed\";\n"
	<< "::exit(-1);\n"
	<< "}\n"
      	<< "s.changeBasis(R1);\n";
    if(h==ModellingHypothesis::PLANESTRESS){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[3]/cste;\n";
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[2];\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      out << "*(stressNew+i)               = s[0];\n";
      out << "*(stressNew+i+   *(nblock))  = s[1];\n";
      out << "*(stressNew+i+2*(*(nblock))) = s[2];\n";
      out << "*(stressNew+i+3*(*(nblock))) = s[3]/cste;\n";
      out << "*(stressNew+i+4*(*(nblock))) = s[5]/cste;\n";
      out << "*(stressNew+i+5*(*(nblock))) = s[4]/cste;\n";
    }
    out << "}\n";
  }
  
  std::string
  AbaqusExplicitInterface::getInterfaceName(void) const
  {
    return "AbaqusExplicit";
  } // end of AbaqusExplicitInterface::getInterfaceName

  void
  AbaqusExplicitInterface::writeUMATxxBehaviourTypeSymbols(std::ostream& out,
							   const std::string& name,
							   const BehaviourDescription& mb) const
  {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourType = " ;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->fss==UNDEFINEDSTRATEGY){
	throw(std::runtime_error("AbaqusExplicitInterface::writeUMATxxBehaviourTypeSymbols: "
				 "behaviours written in the small strain framework "
				 "must be embedded in a strain strategy"));
      }
      out << "2u;\n\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "2u;\n\n";
    } else {
      throw(std::runtime_error("AbaqusExplicitInterface::writeUMATxxBehaviourTypeSymbols: "
			       "unsupported behaviour type"));
    }
  } // end of AbaqusExplicitInterface::writeUMATxxBehaviourTypeSymbols
  
  AbaqusExplicitInterface::~AbaqusExplicitInterface() = default;
  
} // end of namespace mfront
