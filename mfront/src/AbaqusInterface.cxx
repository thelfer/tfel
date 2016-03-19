/*!
 * \file   mfront/src/AbaqusInterface.cxx
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
#include"MFront/AbaqusInterface.hxx"

namespace mfront{
  
  static void
  writeUMATArguments(std::ostream& out,
		     const BehaviourDescription::BehaviourType& t,
		     const bool f)
  {
    if(f){
      out << "(abaqus::AbaqusReal *const STRESS,\n"
	  << " abaqus::AbaqusReal *const STATEV,\n"
	  << " abaqus::AbaqusReal *const DDSDDE,\n"
	  << " abaqus::AbaqusReal *const SSE,\n"
	  << " abaqus::AbaqusReal *const SPD,\n"
	  << " abaqus::AbaqusReal *const SCD,\n"
	  << " abaqus::AbaqusReal *const RPL,\n"
	  << " abaqus::AbaqusReal *const DDSDDT,\n"
	  << " abaqus::AbaqusReal *const DRPLDE,\n"
	  << " abaqus::AbaqusReal *const DRPLDT,\n"
	  << " const abaqus::AbaqusReal *const STRAN,\n"
	  << " const abaqus::AbaqusReal *const DSTRAN,\n"
	  << " const abaqus::AbaqusReal *const TIME,\n"
	  << " const abaqus::AbaqusReal *const DTIME,\n"
	  << " const abaqus::AbaqusReal *const TEMP,\n"
	  << " const abaqus::AbaqusReal *const DTEMP,\n"
	  << " const abaqus::AbaqusReal *const PREDEF,\n"
	  << " const abaqus::AbaqusReal *const DPRED,\n"
	  << " const char           *const CMNAME,\n"
	  << " const abaqus::AbaqusInt  *const NDI,\n"
	  << " const abaqus::AbaqusInt  *const NSHR,\n"
	  << " const abaqus::AbaqusInt  *const NTENS,\n"
	  << " const abaqus::AbaqusInt  *const NSTATV,\n"
	  << " const abaqus::AbaqusReal *const PROPS,\n"
	  << " const abaqus::AbaqusInt  *const NPROPS,\n"
	  << " const abaqus::AbaqusReal *const COORDS,\n"
	  << " const abaqus::AbaqusReal *const DROT,\n"
	  << "       abaqus::AbaqusReal *const PNEWDT,\n"
	  << " const abaqus::AbaqusReal *const CELENT,\n"
	  << " const abaqus::AbaqusReal *const DFGRD0,\n"
	  << " const abaqus::AbaqusReal *const DFGRD1,\n"
	  << " const abaqus::AbaqusInt  *const NOEL,\n"
	  << " const abaqus::AbaqusInt  *const NPT,\n"
	  << " const abaqus::AbaqusInt  *const LAYER,\n"
	  << " const abaqus::AbaqusInt  *const KSPT,\n"
	  << " const abaqus::AbaqusInt  *const KSTEP,\n"
	  << "       abaqus::AbaqusInt  *const KINC,\n"
	  << "const int size)";
    } else {
      out << "(abaqus::AbaqusReal *const STRESS,\n"
	  << " abaqus::AbaqusReal *const STATEV,\n"
	  << " abaqus::AbaqusReal *const DDSDDE,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n";
      if(t!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const abaqus::AbaqusReal *const STRAN,\n"
	    << " const abaqus::AbaqusReal *const DSTRAN,\n";
      } else {
	out << " const abaqus::AbaqusReal *const,\n"
	    << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusReal *const,\n"
	  << " const abaqus::AbaqusReal *const DTIME,\n"
	  << " const abaqus::AbaqusReal *const TEMP,\n"
	  << " const abaqus::AbaqusReal *const DTEMP,\n"
	  << " const abaqus::AbaqusReal *const PREDEF,\n"
	  << " const abaqus::AbaqusReal *const DPRED,\n"
	  << " const char           *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const NTENS,\n"
	  << " const abaqus::AbaqusInt  *const NSTATV,\n"
	  << " const abaqus::AbaqusReal *const PROPS,\n"
	  << " const abaqus::AbaqusInt  *const NPROPS,\n"
	  << " const abaqus::AbaqusReal *const,\n"
	  << " const abaqus::AbaqusReal *const DROT,\n"
	  << "       abaqus::AbaqusReal *const PNEWDT,\n"
	  << " const abaqus::AbaqusReal *const,\n";
      if(t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const abaqus::AbaqusReal *const F0,\n"
	    << " const abaqus::AbaqusReal *const F1,\n";
      } else {
	out << " const abaqus::AbaqusReal *const,\n"
	    << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << "       abaqus::AbaqusInt  *const,\n"
	  << "const int)";
    }
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    using namespace std;
    out << "(abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const char           *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< "       abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< "       abaqus::AbaqusInt  *const,\n"
	<< "const int)";
  } // end of writeUMATArguments
  
  std::string
  AbaqusInterface::getName(void)
  {
    return "abaqus";
  }
  
  std::string
  AbaqusInterface::getInterfaceName(void) const
  {
    return "Abaqus";
  } // end of AbaqusInterface::getInterfaceName


  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  AbaqusInterface::treatKeyword(const std::string& key,
			       tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
			       const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AbaqusInterface::treatKeyword : "+m));}
    };
    if (key=="@AbaqusGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return {true,current};      
    } else if(key=="@AbaqusCompareToNumericalTangentOperator"){
      this->compareToNumericalTangentOperator  = this->readBooleanValue(key,current,end);
      return make_pair(true,current);
    } else if ((key=="@AbaqusTangentOperatorComparisonCriterium")||
	       (key=="@AbaqusTangentOperatorComparisonCriterion")){
      throw_if(!this->compareToNumericalTangentOperator,
	       "comparison to tangent operator is not enabled at this stage.\n"
	       "Use the @AbaqusCompareToNumericalTangentOperator directive before "
	       "@AbaqusTangentOperatorComparisonCriterion");
      throw_if(current==end,"unexpected end of file");
      this->tangentOperatorComparisonCriterion = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    } else if (key=="@AbaqusStrainPerturbationValue"){
      throw_if(!this->compareToNumericalTangentOperator,
	       "time stepping is not enabled at this stage.\n"
	       "Use the @AbaqusUseTimeSubStepping directive before "
	       "@AbaqusStrainPerturbationValue");
      throw_if(current==end,"unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    }
    return {false,current};
  } // end of treatKeyword

  void
  AbaqusInterface::endTreatment(const BehaviourDescription& mb,
				const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      throw(std::runtime_error("AbaqusInterface::endTreatment : "
			       "the abaqus interface only supports small and "
			       "finite strain behaviours"));
    }
    if(this->compareToNumericalTangentOperator){
      if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	throw(std::runtime_error("AbaqusInterface::endTreatment : "
				 "unsupported feature @AbaqusSaveTangentOperator "
				 "and @AbaqusCompareToNumericalTangentOperator : "
				 "those are only valid for small strain beahviours"));
      }
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Abaqus");
    systemCall::mkdir("abaqus");

    // header
    auto fname = "abaqus"+name+".hxx";
    ofstream out("include/MFront/Abaqus/"+fname);
    if(!out){
      throw(std::runtime_error("AbaqusInterface::endTreatment : "
			       "could not open file '"+fname+"'"));
    }

    out << "/*!\n";
    out << "* \\file   "  << fname << endl;
    out << "* \\brief  This file declares the abaqus interface for the " 
	<< mb.getClassName() << " behaviour law\n";
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n";
    out << "#define "<< header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Abaqus/Abaqus.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Abaqus/AbaqusTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Abaqus/AbaqusOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace abaqus{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(mh)){
      this->writeAbaqusBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->writeAbaqusBehaviourTraits(out,mb,h);
      }
    }

    out << "} // end of namespace abaqus\n\n";

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

    fname  = "abaqus"+name+".cxx";
    out.open("src/"+fname);
    if(!out){
      throw(std::runtime_error("AbaqusInterface::endTreatment : "
			       "could not open file '"+fname+"'"));
    }

    string sfeh;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      sfeh = "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    } else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      sfeh = "nullptr";
    } else {
      throw(runtime_error("AbaqusInterface::endTreatment: the abaqus interface only "
			  "supports small and finite strain behaviours"));
    }

    out << "/*!\n";
    out << "* \\file   "  << fname << endl;
    out << "* \\brief  This file implements the abaqus interface for the " 
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
    out << "#include\"MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"MFront/Abaqus/AbaqusInterface.hxx\"\n\n";
    out << "#include\"MFront/Abaqus/abaqus" << name << ".hxx\"\n\n";

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
      throw(runtime_error("AbaqusInterface::endTreatment : "
			  "the abaqus interface only supports small "
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
      out << "vector<abaqus::AbaqusReal> deto0(*NTENS);\n";
      out << "vector<abaqus::AbaqusReal> sig0(*NTENS);\n";
      out << "vector<abaqus::AbaqusReal> sv0(*NSTATV);\n";
      out << "copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n";
      out << "copy(STRESS,STRESS+*(NTENS),sig0.begin());\n";
      out << "copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    out << "if(abaqus::AbaqusInterface<tfel::material::" << mb.getClassName() 
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
      out << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n";
      out << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << endl;\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "// computing the tangent operator by pertubation\n";
      out << "vector<abaqus::AbaqusReal> nD((*NTENS)*(*NTENS));\n";
      out << "vector<abaqus::AbaqusReal> deto(*NTENS);\n";
      out << "vector<abaqus::AbaqusReal> sigf(*NTENS);\n";
      out << "vector<abaqus::AbaqusReal> sigb(*NTENS);\n";
      out << "vector<abaqus::AbaqusReal> sv(*NSTATV);\n";
      out << "vector<abaqus::AbaqusReal> D((*NTENS)*(*NTENS));\n";
      out << "abaqus::AbaqusReal m;\n";
      out << "abaqus::AbaqusReal mDt;\n";
      out << "abaqus::AbaqusReal mnDt;\n";
      out << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n";
      out << "copy(deto0.begin(),deto0.end(),deto.begin());\n";
      out << "copy(sig0.begin(),sig0.end(),sigf.begin());\n";
      out << "copy(sv0.begin(),sv0.end(),sv.begin());\n";
      out << "deto[i] += " << this->strainPerturbationValue << ";\n";
      out << "D[0] = 0.;\n";
      out << "if(abaqus::AbaqusInterface<tfel::material::" << mb.getClassName() 
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
      out << "if(abaqus::AbaqusInterface<tfel::material::" << mb.getClassName() 
	  << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,NPROPS,"
	  << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0],"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy(),"
	  << sfeh << ")!=0){\n";
      out << "return;\n";
      out << "}\n";
      out << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n";
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
      out << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n";
      out << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n";
      out << "cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n";
      out << "}\n";
      out << "cout << endl;\n";
      out << "}\n";
      out << "cout << \"nDt :\" << endl;\n";
      out << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n";
      out << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n";
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
  } // end of AbaqusInterface::endTreatment

  void
  AbaqusInterface::writeInputFileExample(const BehaviourDescription& mb,
					 const FileDescription& fd) const{ 
    const auto name =  mb.getLibrary()+mb.getClassName();
    const auto mn = this->getLibraryName(mb)+"_"+mb.getClassName();
    std::ofstream out{"abaqus/"+name+".inp"};
    if(!out){
      throw(std::runtime_error("AbaqusInterface::writeInputFileExample: "
			       "could not open file 'abaqus/"+name+".inp'"));
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
  } // end of AbaqusInterface::writeInputFileExample

  void 
  AbaqusInterface::writeDepvar(std::ostream& out,
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
      throw(std::runtime_error("AbaqusInterface::writeDepvar: "
			       "unsupported variable type"));
    }
  }
  
  void 
  AbaqusInterface::writeInterfaceSpecificIncludes(std::ostream& out,
						  const BehaviourDescription&) const
  {
    out << "#include\"MFront/Abaqus/Abaqus.hxx\"\n\n";
  } // end of AbaqusInterface::writeInterfaceSpecificIncludes

  std::vector<std::pair<std::string,std::string>>
  AbaqusInterface::getBehaviourDataConstructorAdditionalVariables(void) const{
    return {{"DR","increment of rigid body rotation"}};
  } // end of AbaqusInterface::getBehaviourDataConstructorAdditionalVariables

  void 
  AbaqusInterface::writeBehaviourDataMainVariablesSetters(std::ostream& os,
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
  } // end of AbaqusInterface::writeBehaviourDataMainVariablesSetters
  
  void 
  AbaqusInterface::writeBehaviourDataThermodynamicForceSetter(std::ostream& os,
							      const ThermodynamicForce& f,
							      const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(this->getTypeFlag(f.type)==SupportedTypes::Stensor){
      os << "abaqus::UMATImportThermodynamicForces<hypothesis>::exe(this->" << f.name << ",";
      if(!o.isNull()){
	os << iprefix << "stress_+" << o << ");\n";
      } else {
	os << iprefix << "stress_);\n";
      }
    } else {
      throw(std::runtime_error("AbaqusInterface::writeBehaviourDataMainVariablesSetters : "
			       "unsupported forces type"));
    }
  } // end of AbaqusInterface::writeBehaviourDataThermodynamicForceSetter
  
  void 
  AbaqusInterface::completeBehaviourDataConstructor(std::ostream& out,
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
      out << "static_cast<void>(ABAQUSDR);\n";
    } else {
      out << "const tfel::math::tmatrix<3u,3u,real> abaqus_dr = {ABAQUSDR[0],ABAQUSDR[1],ABAQUSDR[2],\n"
             "                                                   ABAQUSDR[3],ABAQUSDR[4],ABAQUSDR[5],\n"
             "                                                   ABAQUSDR[6],ABAQUSDR[7],ABAQUSDR[8]};\n";
      for(const auto& v:d.getPersistentVariables()){
	const auto flag = this->getTypeFlag(v.type);
	if((flag==SupportedTypes::Stensor)||
	   (flag==SupportedTypes::Tensor)){
	  if(v.arraySize==1u){
	    out << "this->" << v.name << ".changeBasis(abaqus_dr);\n";
	  } else {
	    for(unsigned short i=0;i!=v.arraySize;++i){
	      out << "this->" << v.name << "[" << i << "].changeBasis(abaqus_dr);\n";
	    }
	  }
	}
      }
    }
  } // end of UMATInterfaceBase::completeBehaviourDataConstructor

  void 
  AbaqusInterface::exportThermodynamicForce(std::ostream& out,
					    const std::string& a,
					    const ThermodynamicForce& f,
					    const SupportedTypes::TypeSize o) const
   {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = this->getTypeFlag(f.type);
    if(flag==SupportedTypes::Stensor){
      if(!o.isNull()){
	out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe("
	    << a << "+" << o << ",this->sig);\n";
      } else {
	out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe(" << a << ",this->sig);\n";
      }
    } else {
      throw(std::runtime_error("AbaqusInterface::exportThermodynamicForce: "
			       "unsupported forces type"));
    }
  } // end of AbaqusInterface::exportThermodynamicForce

  void
  AbaqusInterface::getTargetsDescription(TargetsDescription& d,
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
    insert_if(d[lib].sources,"abaqus"+name+".cxx");
    d.headers.push_back("MFront/Abaqus/abaqus"+name+".hxx");
    insert_if(d[lib].ldflags,"-lAbaqusInterface");
    if(this->generateMTestFile){
      insert_if(d[lib].ldflags,"-lMTestFileGenerator");
    }
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    insert_if(d[lib].epts,this->getFunctionName(name));
  } // end of AbaqusInterface::getTargetsDescription

  AbaqusInterface::~AbaqusInterface() = default;
  
} // end of namespace mfront
