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

  static void
  writeVUMATArguments(std::ostream& out,
		      const BehaviourDescription::BehaviourType& t,
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
	  << "const " << type << "* const cmname,\n"
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
	  << "const " << type << "* const cmname,\n"
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
    return "vumat";
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
  
  void
  AbaqusExplicitInterface::endTreatment(const BehaviourDescription& mb,
					const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      throw(std::runtime_error("AbaqusInterface::endTreatment : "
			       "the abaqus explicit interface only supports small and "
			       "finite strain behaviours"));
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Abaqus");
    systemCall::mkdir("abaqus");

    ofstream out;
    {
      // copy vumat.cpp locally
      MFrontLockGuard lock;
      out.open("abaqus/vumat.cpp");
      if(out){
	const auto root = tfel::getInstallPath();
	const auto fn = root+"/share/doc/mfront/abaqus/vumat.cpp";
	std::ifstream in{fn};
	if(in){
	  out << in.rdbuf();
	  in.close();
	  out.close();
	} else {
	  std::cerr << "AbaqusExplicitInterface::endTreatment: "
		    << "could not open file '" << fn << "'" << std::endl;
	}
      } else {
	  std::cerr << "AbaqusExplicitInterface::endTreatment: "
		    << "could not open file 'abaqus/vumat.cpp'" << std::endl;
      }
    }
    
    // header
    auto fname = "abaqusExplicit"+name+".hxx";
    out.open("include/MFront/Abaqus/"+fname);
    if(!out){
      throw(std::runtime_error("AbaqusInterface::endTreatment : "
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
      throw(std::runtime_error("AbaqusInterface::endTreatment : "
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
	<< "#include<limits>\n";
    this->getExtraSrcIncludes(out,mb);

    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
	<< "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/AbaqusInterface.hxx\"\n\n"
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

    out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name) << "_f";
    writeVUMATArguments(out,mb.getBehaviourType(),"float",false);
    this->writeVUMATFunction(out,mb,"float");
        out << "\n"
	<< "MFRONT_SHAREDOBJ void\n" << getFunctionName(name);
    writeVUMATArguments(out,mb.getBehaviourType(),"double",false);
    this->writeVUMATFunction(out,mb,"double");
    out << "\n} // end of extern \"C\"\n";
    out.close();
  }
  
  void
  AbaqusExplicitInterface::writeVUMATFunction(std::ostream& out,
					      const BehaviourDescription& mb,
					      const std::string& t) const
  {
    const auto name =  mb.getLibrary()+mb.getClassName();
    std::string dv0;
    std::string dv1;
    std::string sfeh;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      sfeh = "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    } else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      sfeh = "nullptr";
    } else {
      throw(std::runtime_error("AbaqusInterface::writeVUMATFunction: the abaqus explicit interface "
			       "only supports small and finite strain behaviours"));
    }
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      dv0 = "nullptr";
      dv1 = "strainInc";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      dv0 = "defgradOld";
      dv1 = "defgradNew";
    } else {
      throw(std::runtime_error("AbaqusInterface::writeVUMATFunction: "
				"the abaqus explicit interface only supports small "
				"and finite strain behaviours"));
    }
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    // datacheck phase
    out << "if((std::abs(*stepTime)<std::numeric_limits<" << t << ">::min())&&\n"
	<< "   (std::abs(*totalTime)<std::numeric_limits<" << t << ">::min())){\n"
	// << "if(abaqus::AbaqusExplicitInterface<tfel::material::" << mb.getClassName() 
	// << ">::computeElasticPrediction(*NTENS,dt," << dv0 << "," << dv1 << ","
	// << getFunctionName(name) << "_getOutOfBoundsPolicy(),"
	// << sfeh << ")!=0){\n"
	// << "std::cerr << \"" << mb.getClassName() << "\": integration failed\";\n"
	// << "::exit(-1);\n"
	// << "}\n"
	<< "} else {\n"
	// << "if(abaqus::AbaqusExplicitInterface<tfel::material::" << mb.getClassName() 
	// << ">::exe(NTENS,DTIME,DROT,DDSDDE," << dv0 << "," << dv1 << ",TEMP,DTEMP,PROPS,NPROPS,"
	// << "PREDEF,DPRED,STATEV,NSTATV,STRESS,PNEWDT,"
	// << getFunctionName(name) << "_getOutOfBoundsPolicy(),"
	// << sfeh << ")!=0){\n"
	// << "std::cerr << \"" << mb.getClassName() << "\": integration failed\";\n"
	// << "::exit(-1);\n"
        // << "}\n"
      	<< "}\n"
    	<< "}\n";
  } // end of AbaqusExplicitInterface::endTreatment

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  AbaqusExplicitInterface::treatKeyword(const std::string&,
					CxxTokenizer::TokensContainer::const_iterator p,
					const CxxTokenizer::TokensContainer::const_iterator)
  {
    return {false,p};
  } // end of AbaqusExplicitInterface::treatKeyword

  std::string
  AbaqusExplicitInterface::getInterfaceName(void) const
  {
    return "AbaqusExplicit";
  } // end of AbaqusExplicitInterface::getInterfaceName

  AbaqusExplicitInterface::~AbaqusExplicitInterface() = default;
  
} // end of namespace mfront
