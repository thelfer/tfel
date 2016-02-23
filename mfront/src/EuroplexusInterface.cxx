/*!
 * \file   mfront/src/EuroplexusInterface.cxx
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

#include<fstream>
#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/EuroplexusInterface.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront{
  
  static void
  writeUMATArguments(std::ostream& out,
		     const BehaviourDescription::BehaviourType& t,
		     const bool f)
  {
    if(f){
      out << "(epx::EuroplexusReal *const STRESS,\n"
	  << " epx::EuroplexusReal *const STATEV,\n"
	  << " epx::EuroplexusReal *const DDSDDE,\n"
	  << " epx::EuroplexusReal *const SSE,\n"
	  << " epx::EuroplexusReal *const SPD,\n"
	  << " epx::EuroplexusReal *const SCD,\n"
	  << " epx::EuroplexusReal *const RPL,\n"
	  << " epx::EuroplexusReal *const DDSDDT,\n"
	  << " epx::EuroplexusReal *const DRPLDE,\n"
	  << " epx::EuroplexusReal *const DRPLDT,\n"
	  << " const epx::EuroplexusReal *const STRAN,\n"
	  << " const epx::EuroplexusReal *const DSTRAN,\n"
	  << " const epx::EuroplexusReal *const TIME,\n"
	  << " const epx::EuroplexusReal *const DTIME,\n"
	  << " const epx::EuroplexusReal *const TEMP,\n"
	  << " const epx::EuroplexusReal *const DTEMP,\n"
	  << " const epx::EuroplexusReal *const PREDEF,\n"
	  << " const epx::EuroplexusReal *const DPRED,\n"
	  << " const char           *const CMNAME,\n"
	  << " const epx::EuroplexusInt  *const NDI,\n"
	  << " const epx::EuroplexusInt  *const NSHR,\n"
	  << " const epx::EuroplexusInt  *const NTENS,\n"
	  << " const epx::EuroplexusInt  *const NSTATV,\n"
	  << " const epx::EuroplexusReal *const PROPS,\n"
	  << " const epx::EuroplexusInt  *const NPROPS,\n"
	  << " const epx::EuroplexusReal *const COORDS,\n"
	  << " const epx::EuroplexusReal *const DROT,\n"
	  << "       epx::EuroplexusReal *const PNEWDT,\n"
	  << " const epx::EuroplexusReal *const CELENT,\n"
	  << " const epx::EuroplexusReal *const DFGRD0,\n"
	  << " const epx::EuroplexusReal *const DFGRD1,\n"
	  << " const epx::EuroplexusInt  *const NOEL,\n"
	  << " const epx::EuroplexusInt  *const NPT,\n"
	  << " const epx::EuroplexusInt  *const LAYER,\n"
	  << " const epx::EuroplexusInt  *const KSPT,\n"
	  << " const epx::EuroplexusInt  *const KSTEP,\n"
	  << "       epx::EuroplexusInt  *const KINC,\n"
	  << "const int size)";
    } else {
      out << "(epx::EuroplexusReal *const STRESS,\n"
	  << " epx::EuroplexusReal *const STATEV,\n"
	  << " epx::EuroplexusReal *const DDSDDE,\n"
	  << " epx::EuroplexusReal *const,\n"
	  << " epx::EuroplexusReal *const,\n"
	  << " epx::EuroplexusReal *const,\n"
	  << " epx::EuroplexusReal *const,\n"
	  << " epx::EuroplexusReal *const,\n"
	  << " epx::EuroplexusReal *const,\n"
	  << " epx::EuroplexusReal *const,\n";
      if(t!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const epx::EuroplexusReal *const STRAN,\n"
	    << " const epx::EuroplexusReal *const DSTRAN,\n";
      } else {
	out << " const epx::EuroplexusReal *const,\n"
	    << " const epx::EuroplexusReal *const,\n";
      }
      out << " const epx::EuroplexusReal *const,\n"
	  << " const epx::EuroplexusReal *const DTIME,\n"
	  << " const epx::EuroplexusReal *const TEMP,\n"
	  << " const epx::EuroplexusReal *const DTEMP,\n"
	  << " const epx::EuroplexusReal *const PREDEF,\n"
	  << " const epx::EuroplexusReal *const DPRED,\n"
	  << " const char           *const,\n"
	  << " const epx::EuroplexusInt  *const,\n"
	  << " const epx::EuroplexusInt  *const,\n"
	  << " const epx::EuroplexusInt  *const NTENS,\n"
	  << " const epx::EuroplexusInt  *const NSTATV,\n"
	  << " const epx::EuroplexusReal *const PROPS,\n"
	  << " const epx::EuroplexusInt  *const NPROPS,\n"
	  << " const epx::EuroplexusReal *const,\n"
	  << " const epx::EuroplexusReal *const DROT,\n"
	  << "       epx::EuroplexusReal *const PNEWDT,\n"
	  << " const epx::EuroplexusReal *const,\n";
      if(t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const epx::EuroplexusReal *const F0,\n"
	    << " const epx::EuroplexusReal *const F1,\n";
      } else {
	out << " const epx::EuroplexusReal *const,\n"
	    << " const epx::EuroplexusReal *const,\n";
      }
      out << " const epx::EuroplexusInt  *const,\n"
	  << " const epx::EuroplexusInt  *const,\n"
	  << " const epx::EuroplexusInt  *const,\n"
	  << " const epx::EuroplexusInt  *const,\n"
	  << " const epx::EuroplexusInt  *const,\n"
	  << "       epx::EuroplexusInt  *const,\n"
	  << "const int)";
    }
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    using namespace std;
    out << "(epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const char           *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< "       epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusReal *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< " const epx::EuroplexusInt  *const,\n"
	<< "       epx::EuroplexusInt  *const,\n"
	<< "const int)";
  } // end of writeUMATArguments
  
  std::string
  EuroplexusInterface::getName(void)
  {
    return "europlexus";
  }

  std::string
  EuroplexusInterface::getLibraryName(const BehaviourDescription& mb) const
  {
    auto lib = std::string{};
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "Europlexus"+mb.getMaterialName();
      } else {
	lib = "EuroplexusBehaviour";
      }
    } else {
      lib = "Europlexus"+mb.getLibrary();
    }
    return makeUpperCase(lib);
  } // end of EuroplexusInterface::getLibraryName

  std::string
  EuroplexusInterface::getInterfaceName(void) const
  {
    return "Europlexus";
  } // end of EuroplexusInterface::getInterfaceName

  std::string
  EuroplexusInterface::getFunctionName(const std::string& name) const
  {
    return makeUpperCase(name);
  } // end of EuroplexusInterface::getLibraryName

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  EuroplexusInterface::treatKeyword(const std::string& key,
			       tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
			       const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("EuroplexusInterface::treatKeyword : "+m));}
    };
    if (key=="@EuroplexusGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return {true,current};      
    } else if(key=="@EuroplexusCompareToNumericalTangentOperator"){
      this->compareToNumericalTangentOperator  = this->readBooleanValue(key,current,end);
      return make_pair(true,current);
    } else if ((key=="@EuroplexusTangentOperatorComparisonCriterium")||
	       (key=="@EuroplexusTangentOperatorComparisonCriterion")){
      throw_if(!this->compareToNumericalTangentOperator,
	       "comparison to tangent operator is not enabled at this stage.\n"
	       "Use the @EuroplexusCompareToNumericalTangentOperator directive before "
	       "@EuroplexusTangentOperatorComparisonCriterion");
      throw_if(current==end,"unexpected end of file");
      this->tangentOperatorComparisonCriterion = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    } else if (key=="@EuroplexusStrainPerturbationValue"){
      throw_if(!this->compareToNumericalTangentOperator,
	       "time stepping is not enabled at this stage.\n"
	       "Use the @EuroplexusUseTimeSubStepping directive before "
	       "@EuroplexusStrainPerturbationValue");
      throw_if(current==end,"unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    }
    return {false,current};
  } // end of treatKeyword

  std::set<tfel::material::ModellingHypothesis::Hypothesis>
  EuroplexusInterface::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    // treatment 
    set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    // if(bh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN)!=bh.end()){
    //   h.insert(ModellingHypothesis::GENERALISEDPLANESTRAIN);
    // }
    if(bh.find(ModellingHypothesis::AXISYMMETRICAL)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICAL);
    }
    if(bh.find(ModellingHypothesis::PLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::PLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::PLANESTRESS)!=bh.end()){
      h.insert(ModellingHypothesis::PLANESTRESS);
    }
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)!=bh.end()){
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if(h.empty()){
      throw(std::runtime_error("EuroplexusInterfaceModellingHypothesesToBeTreated : "
			       "no hypotheses selected. This means that the given beahviour "
			       "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
			       "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
			       "make sense to use the Europlexus interface"));
    }
    return h;
  } // end of EuroplexusInterface::getModellingHypothesesToBeTreated

  void
  EuroplexusInterface::endTreatment(const BehaviourDescription& mb,
				const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      throw(std::runtime_error("EuroplexusInterface::endTreatment : "
			       "the europlexus interface only supports small and "
			       "finite strain behaviours"));
    }
    if(this->compareToNumericalTangentOperator){
      if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	throw(std::runtime_error("EuroplexusInterface::endTreatment : "
				 "unsupported feature @EuroplexusSaveTangentOperator "
				 "and @EuroplexusCompareToNumericalTangentOperator : "
				 "those are only valid for small strain beahviours"));
      }
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Europlexus");
    systemCall::mkdir("europlexus");

    // header
    auto fname = "europlexus"+name+".hxx";
    ofstream out("include/MFront/Europlexus/"+fname);
    if(!out){
      throw(std::runtime_error("EuroplexusInterface::endTreatment : "
			       "could not open file '"+fname+"'"));
    }

    out << "/*!\n";
    out << "* \\file   "  << fname << endl;
    out << "* \\brief  This file declares the europlexus interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n";
    out << "#define "<< header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Europlexus/Europlexus.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Europlexus/EuroplexusTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Europlexus/EuroplexusOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace europlexus{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(mh)){
      this->writeEuroplexusBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->writeEuroplexusBehaviourTraits(out,mb,h);
      }
    }

    out << "} // end of namespace europlexus\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    out << "MFRONT_SHAREDOBJ void\n"
	<< getFunctionName(name);
    writeUMATArguments(out);
    out << ";\n\n";

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fname  = "europlexus"+name+".cxx";
    out.open("src/"+fname);
    if(!out){
      throw(std::runtime_error("EuroplexusInterface::endTreatment : "
			       "could not open file '"+fname+"'"));
    }

    string sfeh;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      sfeh = "epx::EuroplexusStandardSmallStrainStressFreeExpansionHandler";
    } else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      sfeh = "nullptr";
    } else {
      throw(runtime_error("EuroplexusInterface::endTreatment: the europlexus interface only "
			  "supports small and finite strain behaviours"));
    }

    out << "/*!\n";
    out << "* \\file   "  << fname << endl;
    out << "* \\brief  This file implements the europlexus interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    this->getExtraSrcIncludes(out,mb);

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>\n"
	  << "#include<vector>\n"
	  << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/Europlexus/EuroplexusStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"MFront/Europlexus/EuroplexusInterface.hxx\"\n\n";
    out << "#include\"MFront/Europlexus/europlexus" << name << ".hxx\"\n\n";

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

    string dv0;
    string dv1;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      dv0 = "STRAN";
      dv1 = "DSTRAN";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      dv0 = "F0";
      dv1 = "F1";
    } else {
      throw(runtime_error("EuroplexusInterface::endTreatment : "
			  "the europlexus interface only supports small "
			  "and finite strain behaviours"));
    }

    out << "MFRONT_SHAREDOBJ void\n"
	<< getFunctionName(name);
    writeUMATArguments(out,mb.getBehaviourType(),false);
    out << "{\n";
    if(((getDebugMode())||(this->compareToNumericalTangentOperator))&&(!this->generateMTestFile)){
      out << "using namespace std;\n";
    }
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    if(this->compareToNumericalTangentOperator){
      out << "vector<epx::EuroplexusReal> deto0(*NTENS);\n";
      out << "vector<epx::EuroplexusReal> sig0(*NTENS);\n";
      out << "vector<epx::EuroplexusReal> sv0(*NSTATV);\n";
      out << "copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n";
      out << "copy(STRESS,STRESS+*(NTENS),sig0.begin());\n";
      out << "copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    out << "if(epx::EuroplexusInterface<tfel::material::" << mb.getClassName() 
	<< ">::exe(NTENS,DTIME,DROT,DDSDDE," << dv0 << "," << dv1 << ",TEMP,DTEMP,PROPS,NPROPS,"
	<< "PREDEF,DPRED,STATEV,NSTATV,STRESS,PNEWDT,"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy(),"
	<< sfeh << ")!=0){\n";
    this->generateMTestFile2(out,mb.getBehaviourType(),
			     name,"",mb);
    out << "*PNEWDT = -1.;\n";
    out << "return;\n";
    out << "}\n";
    if(getDebugMode()){
      out << "cout << \"Dt :\" << endl;\n";
      out << "for(epx::EuroplexusInt i=0;i!=*NTENS;++i){\n";
      out << "for(epx::EuroplexusInt j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "// computing the tangent operator by pertubation\n";
      out << "vector<epx::EuroplexusReal> nD((*NTENS)*(*NTENS));\n";
      out << "vector<epx::EuroplexusReal> deto(*NTENS);\n";
      out << "vector<epx::EuroplexusReal> sigf(*NTENS);\n";
      out << "vector<epx::EuroplexusReal> sigb(*NTENS);\n";
      out << "vector<epx::EuroplexusReal> sv(*NSTATV);\n";
      out << "vector<epx::EuroplexusReal> D((*NTENS)*(*NTENS));\n";
      out << "epx::EuroplexusReal m;\n";
      out << "epx::EuroplexusReal mDt;\n";
      out << "epx::EuroplexusReal mnDt;\n";
      out << "for(epx::EuroplexusInt i=0;i!=*NTENS;++i){\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigf.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] += " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      out << "if(epx::EuroplexusInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0],"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy(),"
	  << sfeh << ")!=0){\n";
      out << "return;\n";
      out << "}\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigb.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] -= " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      out << "if(epx::EuroplexusInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0],"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy(),"
	  << sfeh << ")!=0){\n";
      out << "return;\n";
      out << "}\n";
      out << "for(epx::EuroplexusInt j=0;j!=*NTENS;++j){\n";
      out << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");\n";
      out << "}\n";
      out << "}\n";
      out << "// comparison\n";
      out << "m=0.;\n";
      out << "mDt=0.;\n";
      out << "mnDt=0.;\n";
      out << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n";
      out << "mDt=max(mDt,*(DDSDDE+i));\n";
      out << "mnDt=max(mnDt,nD[i]);\n";
      out << "m=max(m,abs(nD[i]-*(DDSDDE+i)));\n";
      out << "}\n";
      out << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n";
      out << "cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< endl;\n";
      out << "cout << \"Dt :\" << endl;\n";
      out << "for(epx::EuroplexusInt i=0;i!=*NTENS;++i){\n";
      out << "for(epx::EuroplexusInt j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << \"nDt :\" << endl;\n";
      out << "for(epx::EuroplexusInt i=0;i!=*NTENS;++i){\n";
      out << "for(epx::EuroplexusInt j=0;j!=*NTENS;++j){\n";
      out << "cout << nD[j*(*NTENS)+i] << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
    }
    out << "}\n";
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb,fd);
  } // end of EuroplexusInterface::endTreatment

  void
  EuroplexusInterface::writeInputFileExample(const BehaviourDescription& mb,
					 const FileDescription& fd) const{ 
    const auto name =  mb.getLibrary()+mb.getClassName();
    const auto mn = this->getLibraryName(mb)+"_"+mb.getClassName();
    std::ofstream out{"europlexus/"+name+".inp"};
    if(!out){
      throw(std::runtime_error("EuroplexusInterface::writeInputFileExample: "
			       "could not open file 'europlexus/"+name+".inp'"));
    }
    // header
    out << "** \n"
	<< "** File generated by MFront from the "  << fd.fileName << " source\n"
	<< "** Example of how to use the " << mb.getClassName() << " behaviour law\n"
	<< "** Author "  << fd.authorName << '\n'
	<< "** Date   "  << fd.date       << '\n'
	<< "**\n\n";
    // loop over hypothesis
    for(const auto & h : this->getModellingHypothesesToBeTreated(mb)){
      const auto& d = mb.getBehaviourData(h);
      const auto mps = this->buildMaterialPropertiesList(mb,h);
      auto msize = SupportedTypes::TypeSize{};
      if(!mps.first.empty()){
	const auto& m = mps.first.back();
	msize  = m.offset;
	msize += this->getTypeSize(m.type,m.arraySize);
	msize -= mps.second;
      }
      const auto& persistentVarsHolder = d.getPersistentVariables();
      auto vs = SupportedTypes::TypeSize{};
      for(const auto& v : persistentVarsHolder){
	vs+=this->getTypeSize(v.type,v.arraySize);
      }
      const auto vsize = vs.getValueForModellingHypothesis(h);
      const auto& externalStateVarsHolder = d.getExternalStateVariables();
      out << "** Example for the '" << ModellingHypothesis::toString(h) << "' modelling hypothesis\n";
      if(!externalStateVarsHolder.empty()){
	out << "** This behaviour requires " << externalStateVarsHolder.size()
	    <<  " field variables to be defined:\n";
	int i=1;
	for(auto pv=externalStateVarsHolder.begin();pv!=externalStateVarsHolder.end();++pv,++i){
	  out << "** " << i << ": " << mb.getExternalName(h,pv->name);
	}
      }
      out << "*Material, name=" << mn << '\n'
	  << "*Depvar\n" 
	  << (vsize==0 ? 1 : vsize)  << ",\n";
      if(!persistentVarsHolder.empty()){
	int i=1;
	for(const auto& v : persistentVarsHolder){
	  const auto vn = mb.getExternalName(h,v.name);
	  if(v.arraySize==1){
	    this->writeDepvar(out,i,h,v,vn);
	  } else {
	    for(unsigned short a=0;a!=v.arraySize;++a){
	      this->writeDepvar(out,i,h,v,vn+'['+std::to_string(a)+']');
	    }
	  }
	}
      }
      if(!mps.first.empty()){
	out << "** The material properties are given as if we used parameters to explicitly\n"
	    << "** display their names. Users shall replace those declaration by\n"
	    << "** theirs values and/or declare those parameters in the appropriate *parameters\n"
	    << "** section of the input file\n";
      }
      out << "*User Material, constants=" << msize.getValueForModellingHypothesis(h);
      if(!mb.getAttribute(h,BehaviourData::isConsistentTangentOperatorSymmetric,false)){
	out << ", unsymm";
      }
      out << '\n';
      if(!mps.first.empty()){      
	int i=1;
	for(auto pm =mps.first.begin();pm!=mps.first.end();){
	  if(i%9==0){
	    out << "\n";
	    i=1;
	  }
	  out << '<' << pm->name << '>';
	  if(++pm!=mps.first.end()){
	    out << ", ";
	  }
	  ++i;
	}
      }
      out << "\n\n";
    }
  } // end of EuroplexusInterface::writeInputFileExample

  void 
  EuroplexusInterface::writeDepvar(std::ostream& out,
			       int& i,
			       const tfel::material::ModellingHypothesis::Hypothesis& h,
			       const VariableDescription& v,
			       const std::string& n) const {
    if(this->getTypeFlag(v.type)==SupportedTypes::Scalar){
      out << i++ << ", " << n << '\n';
    } else if(this->getTypeFlag(v.type)==SupportedTypes::Stensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	out << i++ << ", " << n << "_13\n";
	out << i++ << ", " << n << "_23\n";
      }
    } else if(this->getTypeFlag(v.type)==SupportedTypes::Tensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      out << i++ << ", " << n << "_21\n";
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	out << i++ << ", " << n << "_13\n";
	out << i++ << ", " << n << "_31\n";
	out << i++ << ", " << n << "_23\n";
	out << i++ << ", " << n << "_32\n";
      }
    } else {
      throw(std::runtime_error("EuroplexusInterface::writeDepvar: "
			       "unsupported variable type"));
    }
  }
  
  void 
  EuroplexusInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
						  const BehaviourDescription&) const
  {
    out << "#include\"MFront/Europlexus/Europlexus.hxx\"\n\n";
  } // end of EuroplexusInterface::writeInterfaceSpecificIncludes

  std::vector<std::pair<std::string,std::string>>
  EuroplexusInterface::getBehaviourDataConstructorAdditionalVariables(void) const{
    return {{"DR","increment of rigid body rotation"}};
  } // end of EuroplexusInterface::getBehaviourDataConstructorAdditionalVariables

  void 
  EuroplexusInterface::writeBehaviourDataMainVariablesSetters(std::ofstream& os,
							  const BehaviourDescription& mb) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov,of;
    os << "void set" << iprefix << "BehaviourDataDrivingVariables(const Type* const " << iprefix << "stran)\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataDrivingVariableSetter(os,v.first,ov);
      ov += this->getTypeSize(v.first.type,1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix << "BehaviourDataThermodynamicForces(const Type* const " << iprefix << "stress_,\n"
       << "                                                          const Type* const )\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataThermodynamicForceSetter(os,v.second,of);
      of += this->getTypeSize(v.second.type,1u);
    }
    os << "}\n\n";
  } // end of EuroplexusInterface::writeBehaviourDataMainVariablesSetters
  
  void 
  EuroplexusInterface::writeBehaviourDataThermodynamicForceSetter(std::ofstream& os,
							      const ThermodynamicForce& f,
							      const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(this->getTypeFlag(f.type)==SupportedTypes::Stensor){
      os << "epx::ImportThermodynamicForces<hypothesis>::exe(this->" << f.name << ",";
      if(!o.isNull()){
	os << iprefix << "stress_+" << o << ");\n";
      } else {
	os << iprefix << "stress_);\n";
      }
      //      os << "tfel::math::change_basis(this->" << f.name << ",europlexus_dr);\n";
    } else {
      throw(std::runtime_error("EuroplexusInterface::writeBehaviourDataMainVariablesSetters : "
			       "unsupported forces type"));
    }
  } // end of EuroplexusInterface::writeBehaviourDataThermodynamicForceSetter
  
  void 
  EuroplexusInterface::completeBehaviourDataConstructor(std::ofstream& out,
						    const Hypothesis h,
						    const BehaviourDescription& mb) const
  {
    const auto& d = mb.getBehaviourData(h);
    bool b = false; // have persistent variables that have to be updated
    for(const auto& v:d.getPersistentVariables()){
	const auto flag = this->getTypeFlag(v.type);
	if((flag==SupportedTypes::Stensor)||
	   (flag==SupportedTypes::Tensor)){
	  b = true;
	  break;
	}
    }
    if(!b){
      out << "static_cast<void>(EUROPLEXUSDR);\n";
    } else {
      out << "const tfel::math::tmatrix<3u,3u,real> europlexus_dr = {EUROPLEXUSDR[0],EUROPLEXUSDR[1],EUROPLEXUSDR[2],\n"
             "                                                   EUROPLEXUSDR[3],EUROPLEXUSDR[4],EUROPLEXUSDR[5],\n"
             "                                                   EUROPLEXUSDR[6],EUROPLEXUSDR[7],EUROPLEXUSDR[8]};\n";
      for(const auto& v:d.getPersistentVariables()){
	const auto flag = this->getTypeFlag(v.type);
	if((flag==SupportedTypes::Stensor)||
	   (flag==SupportedTypes::Tensor)){
	  if(v.arraySize==1u){
	    out << "this->" << v.name << ".changeBasis(europlexus_dr);\n";
	  } else {
	    for(unsigned short i=0;i!=v.arraySize;++i){
	      out << "this->" << v.name << "[" << i << "].changeBasis(europlexus_dr);\n";
	    }
	  }
	}
      }
    }
  } // end of UMATInterfaceBase::completeBehaviourDataConstructor

  void 
  EuroplexusInterface::exportThermodynamicForce(std::ofstream& out,
					    const std::string& a,
					    const ThermodynamicForce& f,
					    const SupportedTypes::TypeSize o) const
   {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = this->getTypeFlag(f.type);
    if(flag==SupportedTypes::Stensor){
      if(!o.isNull()){
	out << "epx::ExportThermodynamicForces<hypothesis>::exe("
	    << a << "+" << o << ",this->sig);\n";
      } else {
	out << "epx::ExportThermodynamicForces<hypothesis>::exe(" << a << ",this->sig);\n";
      }
    } else {
      throw(std::runtime_error("EuroplexusInterface::exportThermodynamicForce: "
			       "unsupported forces type"));
    }
  } // end of EuroplexusInterface::exportThermodynamicForce
    
  void
  EuroplexusInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    using namespace std;
    out << "ModellingHypothesis::Hypothesis h;\n";
    out << "if( *NTENS == 6 ){\n";
    out << "  h = ModellingHypothesis::TRIDIMENSIONAL;\n";
    out << "} else if(*NTENS==3){\n";
    out << "  h = ModellingHypothesis::PLANESTRESS;\n";
    out << "} else if(*NTENS==4){\n";
    out << "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;\n";
    out << "} else {\n";
    out << "  return;\n";
    out << "}\n";
    out << "mg.setModellingHypothesis(h);\n";
  } // end of EuroplexusInterface::writeMTestFileGeneratorSetModellingHypothesis
  
  EuroplexusInterface::~EuroplexusInterface()
  {}

  void
  EuroplexusInterface::getTargetsDescription(TargetsDescription& d,
					const BehaviourDescription& bd)
  {
    const auto lib  = EuroplexusInterface::getLibraryName(bd);
    const auto name = bd.getLibrary()+bd.getClassName(); 
#ifdef _WIN32
    const std::string tfel_config = "tfel-config.exe";
#else /* WIN32 */
    const std::string tfel_config = "tfel-config";
#endif /* WIN32 */
    insert_if(d[lib].cppflags,"$(shell "+tfel_config+" --includes)");
    insert_if(d[lib].sources,"europlexus"+name+".cxx");
    d.headers.push_back("MFront/Europlexus/europlexus"+name+".hxx");
    insert_if(d[lib].ldflags,"-lEuroplexusInterface");
    if(this->generateMTestFile){
      insert_if(d[lib].ldflags,"-lMTestFileGenerator");
    }
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    // insert_if(d[lib].epts,name);
    insert_if(d[lib].epts,this->getFunctionName(name));
  } // end of EuroplexusInterface::getTargetsDescription

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  EuroplexusInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
					       const Hypothesis h) const
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for(const auto & elem : ah){
	if(!mb.hasSpecialisedMechanicalData(elem)){
	  uh.insert(elem);
	}
      }
      if(uh.empty()){
	string msg("UMATInterface::endTreatment : ");
	msg += "internal error : the mechanical behaviour says that not "
	  "all handled mechanical data are specialised, but we found none.";
	throw(runtime_error(msg));
      }
      // material properties for all the selected hypothesis
      vector<pair<vector<UMATMaterialProperty>,
		  SupportedTypes::TypeSize> > mpositions;
      for(const auto & elem : uh){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,elem));
      }
      auto ph=uh.begin();
      auto pum = mpositions.begin();
      const auto& mfirst = *pum;
      ++ph;
      ++pum;
      for(;ph!=uh.end();++ph,++pum){
	const auto& d = mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	const auto& mps = d.getMaterialProperties();
	for(const auto & mp : mps){
	  const auto& mp1 = findUMATMaterialProperty(mfirst.first,
						     mb.getExternalName(h,mp.name));
	  const auto& mp2 = findUMATMaterialProperty(pum->first,
						     mb.getExternalName(h,mp.name));
	  auto o1 = mp1.offset;
	  o1+=pum->second;
	  auto o2 = mp2.offset;
	  o2+=mfirst.second;
	  if(o1!=o2){
	    throw(runtime_error("UMATInterface::buildMaterialPropertiesList : "
				"incompatible offset for material property '"+mp.name+
				"' (aka '"+mp1.name+"'). This is one pitfall of the umat interface. "
				"To by-pass this limitation, you may want to explicitely "
				"specialise some modelling hypotheses"));
	  }
	}
      }
      return mfirst;
    }
    pair<vector<UMATMaterialProperty>,
	 SupportedTypes::TypeSize> res;
    auto& mprops = res.first;
    if((h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
       (h!=ModellingHypothesis::AXISYMMETRICAL)&&
       (h!=ModellingHypothesis::PLANESTRAIN)&&
       (h!=ModellingHypothesis::PLANESTRESS)&&
       (h!=ModellingHypothesis::TRIDIMENSIONAL)){
      string msg("EuroplexusInterface::buildMaterialPropertiesList : "
		 "unsupported modelling hypothesis");
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	msg += " (default)";
      } else {
	msg += " '"+ModellingHypothesis::toString(h)+"'";
      }
      throw(runtime_error(msg));
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus","youn",false);
	this->appendToMaterialPropertiesList(mprops,"real","PoissonRatio","nu",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus1","yg1",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus2","yg2",false);
	this->appendToMaterialPropertiesList(mprops,"stress","YoungModulus3","yg3",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio12","nu12",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio23","nu23",false);
	this->appendToMaterialPropertiesList(mprops,"real",  "PoissonRatio13","nu13",false);
	this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus12","g12",false);
	if (h==ModellingHypothesis::TRIDIMENSIONAL){
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus23","g23",false);
	  this->appendToMaterialPropertiesList(mprops,"stress","ShearModulus13","g13",false);
	} else if((h!=ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)&&
		  (h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
		  (h!=ModellingHypothesis::AXISYMMETRICAL)&&
		  (h!=ModellingHypothesis::PLANESTRAIN)&&
		  (h!=ModellingHypothesis::PLANESTRESS)){
	  throw(runtime_error("EuroplexusInterface::buildMaterialPropertiesList : "
			      "unsupported modelling hypothesis"));
	}
      } else {
	throw(runtime_error("EuroplexusInterface::buildMaterialPropertiesList : "
			    "unsupported behaviour symmetry type"));
      }
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion","alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1","alp1",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2","alp2",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3","alp3",false);
      } else {
	throw(runtime_error("EuroplexusInterface::buildMaterialPropertiesList : "
			    "unsupported behaviour symmetry type"));
      }
    }
    if(!mprops.empty()){
      const auto& m = mprops.back();
      res.second  = m.offset;
      res.second += this->getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of EuroplexusInterface::buildMaterialPropertiesList
    
  void
  EuroplexusInterface::writeEuroplexusBehaviourTraits(std::ostream& out,
					      const BehaviourDescription& mb,
					      const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb,h);
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
    out << "struct EuroplexusTraits<tfel::material::" << mb.getClassName() << "<";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "H";
    } else {
      out << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h);
    }
    out << ",Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{\n";
    out << "//! behaviour type\n";
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " EuroplexusBehaviourType btype = epx::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " EuroplexusBehaviourType btype = epx::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else {
      throw(std::runtime_error("EuroplexusInterface::writeEuroplexusBehaviourTraits : "
			       "unsupported behaviour type"));
    }
    out << "//! space dimension\n";
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      out << "static " << constexpr_c << " unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    } else {
      out << "static " << constexpr_c << " unsigned short N           = tfel::material::ModellingHypothesisToSpaceDimension<"
	  << "tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h)
	  << ">::value;\n";
    }
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c << " unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c << " unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array\n";
    out << "static " << constexpr_c << " unsigned short DrivingVariableSize = " << mvs.first <<  ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c << " unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
    if(mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresUnAlteredStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresStiffnessTensor = false;\n";
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    if(mb.getSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " EuroplexusSymmetryType type = epx::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " EuroplexusSymmetryType type = epx::ORTHOTROPIC;\n";
    } else {
      throw(std::runtime_error("EuroplexusInterface::endTreatment: unsupported behaviour type.\n"
			       "The europlexus interface only support isotropic or orthotropic "
			       "behaviour at this time."));
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if(!mprops.first.empty()){
      const auto& m = mprops.first.back();
      msize  = m.offset;
      msize += this->getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c << " unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " EuroplexusSymmetryType etype = epx::ISOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 1u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else if (mb.getElasticSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " EuroplexusSymmetryType etype = epx::ORTHOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
    	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset "
    	    << "= EuroplexusOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 3u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else {
      throw(std::runtime_error("EuroplexusInterface::endTreatment: unsupported behaviour type.\n"
			       "The europlexus interface only support isotropic or "
			       "orthotropic behaviour at this time."));
    }
    out << "}; // end of class EuroplexusTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis,std::string>
  EuroplexusInterface::gatherModellingHypothesesAndTests(const BehaviourDescription& mb) const
  {
    auto res = std::map<Hypothesis,std::string>{};
    if((mb.getSymmetryType()==mfront::ORTHOTROPIC)&&
       ((mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false))||
	(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)))){
      auto h = this->getModellingHypothesesToBeTreated(mb);
      for(const auto & mh : h){
	res.insert({mh,this->getModellingHypothesisTest(mh)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  } // end of EuroplexusInterface::gatherModellingHypothesesAndTests

  std::string
  EuroplexusInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "*NTENS==4";
    } else if(h==ModellingHypothesis::PLANESTRESS){
      return "*NTENS==3";
    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "*NTENS==6";
    }
    throw(std::runtime_error("EuroplexusInterface::getModellingHypothesisTest : "
			     "unsupported modelling hypothesis"));
  } // end of EuroplexusInterface::gatherModellingHypothesesAndTests

  void
  EuroplexusInterface::writeUMATxxAdditionalSymbols(std::ostream&,
					      const std::string&,
					      const Hypothesis,
					      const BehaviourDescription&,
					      const FileDescription&) const
  {} // end of EuroplexusInterface::writeUMATxxAdditionalSymbols
  
} // end of namespace mfront
