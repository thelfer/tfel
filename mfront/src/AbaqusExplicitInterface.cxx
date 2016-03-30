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
      out.open("abaqus/"+f);
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
	  << "const " << type << "* const strainInc,\n"
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
	  << "const " << type << "* stressNew,\n"
	  << "const " << type << "* stateNew,\n"
	  << "const " << type << "* enerInternNew,\n"
	  << "const " << type << "* enerInelasNew,\n"
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
	  << "const char* const cmname,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const,\n"
	  << "const " << type << "* const props,\n"
	  << "const " << type << "* const density,\n"
	  << "const " << type << "* const strainInc,\n"
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
	  << "const " << type << "* stressNew,\n"
	  << "const " << type << "* stateNew,\n"
	  << "const " << type << "* enerInternNew,\n"
	  << "const " << type << "* enerInelasNew,\n"
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
	  << "const " << type << "* const,\n"
	  << "const " << type << "*,\n"
	  << "const " << type << "*,\n"
	  << "const " << type << "*,\n"
	  << "const " << type << "*,\n"
	  << "const int)";
  } // end of writeVUMATArguments
  
  std::string
  AbaqusExplicitInterface::getName(void)
  {
    return "abaqus-vumat";
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
    insert_if(d[lib].sources,"abaqusExplicit"+name+".cxx");
    d.headers.push_back("MFront/Abaqus/abaqusExplicit"+name+".hxx");
    insert_if(d[lib].ldflags,"-lAbaqusInterface");
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    insert_if(d[lib].epts,this->getFunctionName(name));
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
    tfel::system::systemCall::mkdir("abaqus");
    copyVUMATFiles();
    // header
    auto fname = "abaqusExplicit"+name+".hxx";
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

    out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name) << "_f";
    writeVUMATArguments(out,"float");
    out << ";\n\n";

    out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name);
    writeVUMATArguments(out,"double");
    out << ";\n\n"
	<< "#ifdef __cplusplus\n"
	<< "}\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#endif /* " << header << " */\n";

    out.close();

    fname  = "abaqusExplicit"+name+".cxx";
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
	<< "#include\"MFront/Abaqus/abaqusExplicit" << name << ".hxx\"\n\n";

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

    for(const std::string t : {"float","double"}){
      out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name);
      if(t=="float"){
	out << "_f";
      }
      writeVUMATArguments(out,t,false);
      out << "{\n"
	  << "using MH = tfel::material::ModellingHypothesis;\n"
	  << "using abaqus::AbaqusTraits;\n"
	  << "using tfel::material::" << mb.getClassName() << ";\n"
	  << "using AbaqusExplicitData = abaqus::AbaqusExplicitData<" << t << ">;\n";
      if(mb.getAttribute(BehaviourData::profiling,false)){
	out << "using mfront::BehaviourProfiler;\n"
	    << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	    << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	    << "BehaviourProfiler::TOTALTIME);\n";
      }
      out << "const auto ntens = *ndir+*nshr;\n"
	  << "const AbaqusExplicitData d = {*nblock,*ndir,*nshr,*nstatev,\n"
	  << "                              *nfieldv,*nprops,*dt,props,density,\n"
	  << "                              strainInc,relSpinInc,tempOld,\n"
	  << "                              stretchOld,defgradOld,fieldOld,\n"
	  << "                              stressOld,stateOld,enerInternOld,\n"
	  << "                              enerInelasOld,tempNew,stretchNew,\n"
	  << "                              defgradNew,fieldNew,stressNew,\n"
	  << "                              stateNew,enerInternNew,enerInelasNew};\n";
      if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	if(this->fss==FINITEROTATIONSMALLSTRAIN){
	  this->writeFiniteRotationSmallStrainBehaviourCall(out,mb,t);
	} else if(this->fss==MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN){
	  this->writeLogarithmicStrainBehaviourCall(out,mb,t);
	} else {
	  throw(std::runtime_error("AbaqusExplicitInterface::writeVUMATFunction: "
				   "unsupported finite strain strategy (internal error)"));
	}
      } else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	this->writeFiniteStrainBehaviourCall(out,mb,t);
      } else {
	throw(std::runtime_error("AbaqusExplicitInterface::writeVUMATFunction: "
				 "the abaqus explicit interface only supports small "
				 "and finite strain behaviours"));
      }
      out << "}\n\n";
    }
    out << "} // end of extern \"C\"\n";
    out.close();
  }

  void
  AbaqusExplicitInterface::writeChecks(std::ostream& out,
				       const BehaviourDescription& mb,
				       const std::string& t,
				       const Hypothesis h) const
  {
    using MH = tfel::material::ModellingHypothesis;
    out << "using BV = " << mb.getClassName()
	<< "<MH::"<< MH::toUpperCaseString(h) << "," << t << ",false>;\n"
	<< "using Traits = tfel::material::MechanicalBehaviourTraits<BV>;\n"
	<< "constexpr const unsigned short offset  = (AbaqusTraits<BV>::elasticPropertiesOffset+\n"
	<< "                                          AbaqusTraits<BV>::thermalExpansionPropertiesOffset);\n"
	<< "constexpr const unsigned short nprops_  = AbaqusTraits<BV>::material_properties_nb;\n"
	<< "constexpr const unsigned short NPROPS_  = offset+nprops_ == 0 ? 1u : offset+nprops_;\n"
	<< "constexpr const unsigned short nstatev_ = Traits::internal_variables_nb;\n"
	<< "constexpr const unsigned short NSTATEV_ = nstatev_ == 0 ? 1u : nstatev_;\n"
	<< "constexpr const unsigned short nfieldv_ = Traits::external_variables_nb;\n"
	<< "constexpr const unsigned short NFIELDV_ = nfieldv_ == 0 ? 1u : nfieldv_;\n"
	<< "if(*nprops!=NPROPS_){\n"
	<< "std::cerr << \"" << mb.getClassName() << ":"
	<< " unmatched number of material properties "
	<< "(\" << *nprops << \" given, \" << NPROPS_ << \" expected)\";\n"
	<< "::exit(-1);\n"
	<< "}\n"
	<< "if(*nstatev!=NSTATEV_){\n"
	<< "std::cerr << \"" << mb.getClassName() << ":"
	<< " unmatched number of internal state variables "
	<< "(\" << *nstatev << \" given, \" << NSTATEV_ << \" expected)\";\n"
	<< "::exit(-1);\n"
	<< "}\n"
	<< "if(*nfieldv!=NFIELDV_){\n"
	<< "std::cerr << \"" << mb.getClassName() << ":"
	<< " unmatched number of external state variables "
	<< "(\" << *nfieldv << \" given, \" << NFIELDV_ << \" expected)\";\n"
	<< "::exit(-1);\n"
	<< "}\n";
  } // end of AbaqusExplicitInterface::writeChecks
  
  void
  AbaqusExplicitInterface::writeComputeElasticPrediction(std::ostream& out,
							 const BehaviourDescription& mb,
							 const std::string& t,
							 const Hypothesis h) const
  {
    using MH = tfel::material::ModellingHypothesis;
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    if(mh.find(h)!=mh.end()){
      this->writeChecks(out,mb,t,h);
      out << "bool s = true;\n"
	  << "for(int i=0;i!=*nblock;++i){\n"
	  << "s = s && (abaqus::AbaqusExplicitInterface<MH::" << MH::toUpperCaseString(h) << ","
	  << t<< "," << mb.getClassName()
	  << ">::computeElasticPrediction(i,d)==0);\n"
	  << "}\n"
	  << "if(!s){\n"
	  << "std::cerr << \"" << mb.getClassName() << ": elastic loading failed\";\n"
	  << "::exit(-1);\n"
	  << "}\n";
    } else {
      out << "std::cerr << \"" << mb.getClassName() << ": unsupported hypothesis\";\n"
	  << "::exit(-1);\n";
    }
  } // end of AbaqusExplicitInterface::writeComputeElasticPrediction

  
  void
  AbaqusExplicitInterface::writeFiniteRotationSmallStrainBehaviourCall(std::ostream& out,
								       const BehaviourDescription& mb,
								       const std::string& t) const
  {
    using MH = tfel::material::ModellingHypothesis;
    // datacheck phase
    out << "if((std::abs(*stepTime)<std::numeric_limits<" << t << ">::min())&&\n"
	<< "   (std::abs(*totalTime)<std::numeric_limits<" << t << ">::min())){\n"
	<< "if(ntens==3u){\n"
	<< "// plane stress\n";
    this->writeComputeElasticPrediction(out,mb,t,MH::PLANESTRESS);
    out << "} else if(ntens==4u){\n"
	<< "// axisymmetric/plane strain case\n";
    this->writeComputeElasticPrediction(out,mb,t,MH::GENERALISEDPLANESTRAIN);      
    out << "} else if(ntens==6u){\n"
	<< "// tridimensional case\n";
    this->writeComputeElasticPrediction(out,mb,t,MH::TRIDIMENSIONAL);
    out << "} else {\n"
	<< "std::cerr << \"" << mb.getClassName() << " unupported modelling hypothesis\";\n"
	<< "std::exit(-1);\n"
      	<< "}\n"
      	<< "} else {\n"
	<< "// behaviour integration\n"
	<< "if(ntens==3u){\n"
	<< "// plane stress\n"
	<< "} else if(ntens==4u){\n"
	<< "// axisymmetric case\n"
      	<< "} else if(ntens==6u){\n"
      	<< "// tridimensional case\n"
	<< "} else {\n"
	<< "std::cerr << \"" << mb.getClassName() << " unupported modelling hypothesis\";\n"
	<< "std::exit(-1);\n"
      	<< "}\n"
	<< "}\n";      
  }

  void
  AbaqusExplicitInterface::writeLogarithmicStrainBehaviourCall(std::ostream&,
							       const BehaviourDescription&,
							       const std::string&) const
  {
    throw(std::runtime_error("AbaqusExplicitInterface::writeLogarithmicStrainBehaviourCall: "
			     "unsupported feature"));
  }
  
  void
  AbaqusExplicitInterface::writeFiniteStrainBehaviourCall(std::ostream&,
							  const BehaviourDescription&,
							  const std::string&) const
  {
    throw(std::runtime_error("AbaqusExplicitInterface::writeFiniteStrainBehaviourCall: "
			     "unsupported feature"));
  } // end of AbaqusExplicitInterface::endTreatment

  std::string
  AbaqusExplicitInterface::getInterfaceName(void) const
  {
    return "AbaqusExplicit";
  } // end of AbaqusExplicitInterface::getInterfaceName

  AbaqusExplicitInterface::~AbaqusExplicitInterface() = default;
  
} // end of namespace mfront
