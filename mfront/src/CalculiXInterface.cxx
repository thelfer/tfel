/*!
 * \file   mfront/src/CalculiXInterface.cxx
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

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Config/GetInstallPath.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/CalculiXInterface.hxx"

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
      out << "(const char * const amat,"
	  << " const calculix::CalculiXInt* const iel,"
	  << " const calculix::CalculiXInt* const iint,"
	  << " const calculix::CalculiXInt* const NPROPS,"
	  << " const calculix::CalculiXReal* const MPROPS,"
	  << " const calculix::CalculiXReal* const STRAN1,"
	  << " const calculix::CalculiXReal* const STRAN0,"
	  << " const calculix::CalculiXReal* const beta,"
	  << " const calculix::CalculiXReal* const F0,"
	  << " const calculix::CalculiXReal* const voj,"
	  << " const calculix::CalculiXReal* const F1,"
	  << " const calculix::CalculiXReal* const vj,"
	  << " const calculix::CalculiXInt* const ithermal,"
	  << " const calculix::CalculiXReal* const TEMP1,"
	  << " const calculix::CalculiXReal* const DTIME,"
	  << " const calculix::CalculiXReal* const time,"
	  << " const calculix::CalculiXReal* const ttime,"
	  << " const calculix::CalculiXInt* const icmd,"
	  << " const calculix::CalculiXInt* const ielas,"
	  << " const calculix::CalculiXInt* const mi,"
	  << " const calculix::CalculiXInt* const NSTATV,"
	  << " const calculix::CalculiXReal* const STATEV0,"
	  << "       calculix::CalculiXReal* const STATEV1,"
	  << "       calculix::CalculiXReal* const STRESS,"
	  << "       calculix::CalculiXReal* const DDSDDE,"
	  << " const calculix::CalculiXInt* const iorien,"
	  << " const calculix::CalculiXReal* const pgauss,"
	  << " const calculix::CalculiXReal* const orab,"
	  << "       calculix::CalculiXReal* const PNEWDT,"
	  << " const calculix::CalculiXInt* const ipkon,"
	  << " const int size)";
    } else {
      out << "(const char * const,"
	  << " const calculix::CalculiXInt* const,"
	  << " const calculix::CalculiXInt* const,"
	  << " const calculix::CalculiXInt* const  NPROPS,"
	  << " const calculix::CalculiXReal* const MPROPS,";
      if(t==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	out << " const calculix::CalculiXReal *const STRAN1,\n"
	    << " const calculix::CalculiXReal *const STRAN0,\n";
      } else {
	out << " const calculix::CalculiXReal *const,\n"
	    << " const calculix::CalculiXReal *const,\n";
      }
      out << " const calculix::CalculiXReal* const,";
      if(t==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	out << " const calculix::CalculiXReal* const F0,"
	    << " const calculix::CalculiXReal* const ,"
	    << " const calculix::CalculiXReal* const F1,"
	    << " const calculix::CalculiXReal* const,";
      } else {
	out << " const calculix::CalculiXReal* const,"
	    << " const calculix::CalculiXReal* const ,"
	    << " const calculix::CalculiXReal* const,"
	    << " const calculix::CalculiXReal* const,";
      }
      out << " const calculix::CalculiXInt* const ithermal,"
	  << " const calculix::CalculiXReal* const TEMP1,"
	  << " const calculix::CalculiXReal* const DTIME,"
	  << " const calculix::CalculiXReal* const,"
	  << " const calculix::CalculiXReal* const,"
	  << " const calculix::CalculiXInt* const icmd,"
	  << " const calculix::CalculiXInt* const ielas,"
	  << " const calculix::CalculiXInt* const,"
	  << " const calculix::CalculiXInt* const  NSTATV,"
	  << " const calculix::CalculiXReal* const STATEV0,"
	  << "       calculix::CalculiXReal* const STATEV1,"
	  << "       calculix::CalculiXReal* const STRESS,"
	  << "       calculix::CalculiXReal* const DDSDDE,"
	  << " const calculix::CalculiXInt* const,"
	  << " const calculix::CalculiXReal* const,"
	  << " const calculix::CalculiXReal* const,"
	  << "       calculix::CalculiXReal* const PNEWDT,"
	  << " const calculix::CalculiXInt* const,"
	  << " const int size)";
    }
  } // end of writeUMATArguments

  static void writeUMATArguments(std::ostream& out)
  {
    out << "(const char * const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXReal* const,"
	<< "       calculix::CalculiXReal* const,"
	<< "       calculix::CalculiXReal* const,"
	<< "       calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXReal* const,"
	<< "       calculix::CalculiXReal* const,"
	<< " const calculix::CalculiXInt* const,"
	<< " const int)";
  } // end of writeUMATArguments

  std::string CalculiXInterface::getName()
  {
    return "calculix";
  }
  
  std::string CalculiXInterface::getInterfaceName() const
  {
    return "CalculiX";
  } // end of CalculiXInterface::getInterfaceName

  std::pair<bool,CalculiXInterface::tokens_iterator>
  CalculiXInterface::treatKeyword(BehaviourDescription&,
				  const std::string& key,
				  const std::vector<std::string>& i,
				  tokens_iterator current,
				  const tokens_iterator end)
  {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CalculiXInterface::treatKeyword: "+m));}
    };
    if(!i.empty()){
      if(std::find(i.begin(),i.end(),this->getName())!=i.end()){
	const auto keys =  std::vector<std::string>{
	  "@CalculiXFiniteStrainStrategy",
	  "@CalculiXGenerateMTestFileOnFailure",
	  "@CalculiXCompareToNumericalTangentOperator",
	  "@CalculiXTangentOperatorComparisonCriterium",
	  "@CalculiXTangentOperatorComparisonCriterion",
	  "@CalculiXStrainPerturbationValue"};
	throw_if(std::find(keys.begin(),keys.end(),key)==keys.end(),
		 "unsupported key '"+key+"'");
      } else {
	return {false,current};
      }
    }
    if (key=="@CalculiXFiniteStrainStrategy"){
      auto read = [&throw_if](const std::string& s){
	if(s=="Native"){
	  return NATIVEFINITESTRAINSTRATEGY;
	} else if(s=="FiniteRotationSmallStrain"){
	  return FINITEROTATIONSMALLSTRAIN;
	} else if(s=="MieheApelLambrechtLogarithmicStrain"){
	  return MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN;
	}
	throw_if(true,"unsupported strategy '"+s+"'\n"
		 "The only supported strategies are 'Native',"
		 "'FiniteRotationSmallStrain' and "
		 "'MieheApelLambrechtLogarithmicStrain'");
      };
      throw_if(this->fss!=UNDEFINEDSTRATEGY,
	       "a finite strain strategy has already been defined");
      throw_if(current==end,"unexpected end of file");
      this->fss = read(current->value);
      throw_if(++current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+'\'');
      ++(current);
      return {true,current};
    } else if (key=="@CalculiXGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return {true,current};      
    } else if(key=="@CalculiXCompareToNumericalTangentOperator"){
      this->compareToNumericalTangentOperator  = this->readBooleanValue(key,current,end);
      return make_pair(true,current);
    } else if ((key=="@CalculiXTangentOperatorComparisonCriterium")||
	       (key=="@CalculiXTangentOperatorComparisonCriterion")){
      throw_if(!this->compareToNumericalTangentOperator,
	       "comparison to tangent operator is not enabled at this stage.\n"
	       "Use the @CalculiXCompareToNumericalTangentOperator directive before "
	       "@CalculiXTangentOperatorComparisonCriterion");
      throw_if(current==end,"unexpected end of file");
      this->tangentOperatorComparisonCriterion = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    } else if (key=="@CalculiXStrainPerturbationValue"){
      throw_if(!this->compareToNumericalTangentOperator,
	       "time stepping is not enabled at this stage.\n"
	       "Use the @CalculiXUseTimeSubStepping directive before "
	       "@CalculiXStrainPerturbationValue");
      throw_if(current==end,"unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    }
    return {false,current};
  } // end of treatKeyword

  void CalculiXInterface::endTreatment(const BehaviourDescription& mb,
				       const FileDescription& fd) const
  {
    using namespace tfel::system;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CalculiXInterface::endTreatment: "+m));}
    };
    throw_if((mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	     (this->fss!=UNDEFINEDSTRATEGY),
	     "finite strain strategy is only supported for small strain behaviours");
    throw_if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)),
	     "the calculix interface only supports small and "
	     "finite strain behaviours");
    // the only supported modelling hypothesis
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    // checks
    if(this->compareToNumericalTangentOperator){
      throw_if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
	       "unsupported feature @CalculiXSaveTangentOperator "
	       "and @CalculiXCompareToNumericalTangentOperator : "
	       "those are only valid for small strain beahviours");
    }
    // get the modelling hypotheses to be treated
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/CalculiX");
    systemCall::mkdir("calculix");

    std::ofstream out;

    // header
    auto fname = "calculix"+name+".hxx";
    out.open("include/MFront/CalculiX/"+fname);
    throw_if(!out,"could not open file '"+fname+"'");
    
    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file declares the calculix interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n"
	<< "#define "<< header << "\n\n"
	<< "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
	<< "#include\"MFront/CalculiX/CalculiX.hxx\"\n"
	<< "#include\"MFront/CalculiX/CalculiXData.hxx\"\n\n"
	<< "#ifdef __cplusplus\n"
	<< "#include\"MFront/CalculiX/CalculiXTraits.hxx\"\n";
    if(this->fss!=UNDEFINEDSTRATEGY){
      out << "#include\"MFront/CalculiX/CalculiXFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
	<< "namespace calculix{\n\n";

    this->writeCalculiXBehaviourTraits(out,mb);

    out << "} // end of namespace calculix\n\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#ifdef __cplusplus\n"
	<< "extern \"C\"{\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionName(name);
    writeUMATArguments(out);
    out << ";\n\n";

    out << "#ifdef __cplusplus\n"
	<< "}\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#endif /* " << header << " */\n";

    out.close();

    fname  = "calculix"+name+".cxx";
    out.open("src/"+fname);
    throw_if(!out,"could not open file '"+fname+"'");

    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file implements the calculix interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    this->getExtraSrcIncludes(out,mb);

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>\n"
	  << "#include<vector>\n"
	  << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
	<< "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/CalculiX/CalculiXStressFreeExpansionHandler.hxx\"\n\n"
	<< "#include\"MFront/CalculiX/CalculiXInterface.hxx\"\n\n"
	<< "#include\"MFront/CalculiX/calculix" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
    
    out << "extern \"C\"{\n\n";
 
    this->generateUMATxxGeneralSymbols(out,name,mb,fd);
    this->generateUMATxxSymbols(out,name,h,mb,fd);
    
    this->writeSetParametersFunctionsImplementations(out,name,mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);

    if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       ((this->fss==UNDEFINEDSTRATEGY)||(this->fss==NATIVEFINITESTRAINSTRATEGY))){
      this->writeUMATSmallStrainFunction(out,mb,name);
    } else if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	      ((this->fss!=UNDEFINEDSTRATEGY)&&(this->fss!=NATIVEFINITESTRAINSTRATEGY))){
      if(this->fss==FINITEROTATIONSMALLSTRAIN){
	this->writeUMATFiniteRotationSmallStrainFunction(out,mb,name);
      } else {
	throw_if(true,"unsupported finite strain strategy !");
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      this->writeUMATFiniteStrainFunction(out,mb,name);
    } else {
      throw_if(true,"the calculix interface only supports small "
	       "and finite strain behaviours");
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb,fd,true);
  } // end of CalculiXInterface::endTreatment
  
  void CalculiXInterface::writeUMATFunctionBase(std::ostream& out,
						const BehaviourDescription& mb,
						const std::string& name,
						const std::string& sfeh) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CalculiXInterface::writeUMATFunctionBase: "+m));}
    };
    std::string dv0,dv1,sig,statev,nstatev;
    const auto btype = mb.getBehaviourType();
    out << "static void\n" << name << "_base";
    writeUMATArguments(out,btype,false);
    out << "{\n";
    if(btype==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      dv0 = "STRAN0";
      dv1 = "STRAN1";
    } else if(btype==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      dv0 = "F0";
      dv1 = "F1";
    } else {
      throw_if(true,"the calculix interface only supports small "
	       "and finite strain behaviours");
    }
    out << "using calculix::CalculiXData;\n"
	<< "CalculiXData d = {STRESS,PNEWDT,DDSDDE,STATEV1,*DTIME,STATEV1,\n"
	<< "                  " << dv0 << "," << dv1 << ",TEMP1,MPROPS,\n"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	<< "if(calculix::CalculiXInterface<tfel::material::" << mb.getClassName() 
	<< ">::exe(d)!=0){\n";
    out << "*PNEWDT = 0.2;\n"
	<< "return;\n"
	<< "}\n";
    // if(getDebugMode()){
    //   out << "std::cout << \"Dt :\" << std::endl;\n"
    // 	  << "for(calculix::CalculiXInt i=0;i!=6;++i){\n"
    // 	  << "for(calculix::CalculiXInt j=0;j!=6;++j){\n"
    // 	  << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
    // 	  << "}\n"
    // 	  << "std::cout << std::endl;\n"
    // 	  << "}\n"
    // 	  << "std::cout << std::endl;\n";
    // }
    out << "}\n\n";
  } // end of CalculiXInterface::writeUMATFunctionBase

  void CalculiXInterface::writeUMATFiniteStrainFunction(std::ostream& out,
							const BehaviourDescription& mb,
							const std::string& name) const
  {
    const std::string sfeh = "nullptr";
    this->writeUMATFunctionBase(out,mb,name,sfeh);
    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionName(name);
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,true);
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << name << "_base"
	<< "(amat,iel,iint,NPROPS,MPROPS,STRAN1,STRAN0,beta,F0,"
	<< " voj,F1,vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
	<< " ielas,mi,NSTATV,STATEV0,STATEV1,STRESS,DDSDDE,"
	<< "iorien,pgauss,orab,PNEWDT,ipkon,size);\n"
	<< "}\n\n";
  }
  
  void CalculiXInterface::writeUMATSmallStrainFunction(std::ostream& out,
						       const BehaviourDescription& mb,
						       const std::string& name) const
  {
    const std::string sfeh = "calculix::CalculiXStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out,mb,name,sfeh);
    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionName(name);
    writeUMATArguments(out,BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,true);
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "calculix::CalculiXReal PNEWDT0(*PNEWDT);\n"
	  << "std::vector<calculix::CalculiXReal> deto0(*NTENS);\n"
	  << "std::vector<calculix::CalculiXReal> sig0(*NTENS);\n"
	  << "std::vector<calculix::CalculiXReal> sv0(*NSTATV);\n"
	  << "std::copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n"
	  << "std::copy(STRESS,STRESS+*(NTENS),sig0.begin());\n"
	  << "std::copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if(this->generateMTestFile){
      this->generateMTestFile1(out);
    }
    out << "calculix::CalculiXReal DSTRAN[6] = {STRAN1[0]-STRAN0[0],\n"
	<< "                                    STRAN1[1]-STRAN0[1],\n"
      	<< "                                    STRAN1[2]-STRAN0[2],\n"
      	<< "                                    STRAN1[3]-STRAN0[3],\n"
      	<< "                                    STRAN1[4]-STRAN0[4],\n"
	<< "                                    STRAN1[5]-STRAN0[5]};\n" 
	<< name << "_base"
	<< "(amat,iel,iint,NPROPS,MPROPS,DSTRAN,STRAN0,beta,F0,"
	<< " voj,F1,vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
	<< " ielas,mi,NSTATV,STATEV0,STATEV1,STRESS,DDSDDE,"
	<< "iorien,pgauss,orab,PNEWDT,ipkon,size);\n";
    if(this->generateMTestFile){
      out << "if(*PNEWDT<1){\n";
      this->generateMTestFile2(out,mb.getBehaviourType(),
			       name,"",mb);
      out << "}\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "// computing the tangent operator by pertubation\n"
	  << "std::vector<calculix::CalculiXReal> nD((*NTENS)*(*NTENS));\n"
	  << "std::vector<calculix::CalculiXReal> deto(*NTENS);\n"
	  << "std::vector<calculix::CalculiXReal> sigf(*NTENS);\n"
	  << "std::vector<calculix::CalculiXReal> sigb(*NTENS);\n"
	  << "std::vector<calculix::CalculiXReal> sv(*NSTATV);\n"
	  << "std::vector<calculix::CalculiXReal> D((*NTENS)*(*NTENS));\n"
	  << "calculix::CalculiXReal PNEWDT(PNEWDT0);\n"
	  << "calculix::CalculiXReal m;\n"
	  << "calculix::CalculiXReal mDt;\n"
	  << "calculix::CalculiXReal mnDt;\n"
	  << "for(calculix::CalculiXInt i=0;i!=*NTENS;++i){\n"
	  << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "std::copy(sig0.begin(),sig0.end(),sigf.begin());\n"
	  << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] += " << this->strainPerturbationValue << ";\n"
	  << "D[0] = 0.;\n"
	  << "calculix::CalculiXData d2 = {&sigf[0],&PNEWDT0,&D[0],&sv[0],\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	  << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
	  << "                         PROPS,PREDEF,DPRED,\n"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	  << "if(calculix::CalculiXInterface<tfel::material::"
	  << mb.getClassName() << ">::exe(d2)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "calculix::CalculiXReal PNEWDT(PNEWDT0);\n"
	  << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
	  << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] -= " << this->strainPerturbationValue << ";\n"
	  << "D[0] = 0.;\n"
	  << "calculix::CalculiXData d3 = {&sigf[0],&PNEWDT0,&D[0],&sv[0],\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	  << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
	  << "                         PROPS,PREDEF,DPRED,\n"
	  << "if(calculix::CalculiXInterface<tfel::material::" << mb.getClassName() << ">::exe(d3)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "for(calculix::CalculiXInt j=0;j!=*NTENS;++j){\n"
	  << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");\n"
	  << "}\n"
	  << "}\n"
	  << "// comparison\n"
	  << "m=0.;\n"
	  << "mDt=0.;\n"
	  << "mnDt=0.;\n"
	  << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n"
	  << "mDt=std::max(mDt,*(DDSDDE+i));\n"
	  << "mnDt=std::max(mnDt,nD[i]);\n"
	  << "m=std::max(m,std::abs(nD[i]-*(DDSDDE+i)));\n"
	  << "}\n"
	  << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
	  << "std::cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< std::endl;\n"
	  << "std::cout << \"Dt :\" << std::endl;\n"
	  << "for(calculix::CalculiXInt i=0;i!=*NTENS;++i){\n"
	  << "for(calculix::CalculiXInt j=0;j!=*NTENS;++j){\n"
	  << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n"
	  << "std::cout << \"nDt :\" << std::endl;\n"
	  << "for(calculix::CalculiXInt i=0;i!=*NTENS;++i){\n"
	  << "for(calculix::CalculiXInt j=0;j!=*NTENS;++j){\n"
	  << "std::cout << nD[j*(*NTENS)+i] << \" \";\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n";
    }
    if(this->fss==NATIVEFINITESTRAINSTRATEGY){
      out << "calculix::CalculiXFiniteStrain::applyNativeFiniteStrainCorrection(DDSDDE,DFGRD1,STRESS,*NTENS);\n";
    }
    out << "}\n\n";
  }
  
  void
  CalculiXInterface::writeUMATFiniteRotationSmallStrainFunction(std::ostream& out,
								const BehaviourDescription& mb,
								const std::string& name) const
  {
    const std::string sfeh = "calculix::CalculiXStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out,mb,name,sfeh);
    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionName(name);
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,true);
    out << "{\n"
	<< "using namespace calculix;\n"
	<< "CalculiXReal eto[6];\n"
      	<< "CalculiXReal deto[6];\n"
	<< "CalculiXReal CSE[36];\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
      out << "{\n"
	  << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    
    out << "static_cast<void>(STRAN);\n"
	<< "static_cast<void>(DSTRAN);\n"
	<< "CalculiXFiniteStrain::computeGreenLagrangeStrain(eto,DFGRD0);\n"
	<< "CalculiXFiniteStrain::computeGreenLagrangeStrain(deto,DFGRD1);\n"
	<< "CalculiXFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,DFGRD0);\n"
	<< "for(int i=0;i!=*NTENS;++i){\n"
	<< "deto[i] -= eto[i];\n"
	<< "}\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "}\n";
    }
    out	<< name << "_base"
	<< "(amat,iel,iint,NPROPS,MPROPS,STRAN1,STRAN0,beta,F0,"
	<< " voj,F1,vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
	<< " ielas,mi,NSTATV,STATEV0,STATEV1,STRESS,DDSDDE,"
	<< "iorien,pgauss,orab,PNEWDT,ipkon,size);\n"
	<< "if(*PNEWDT>=0.99999){\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "CalculiXFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,DFGRD1);\n"
	<< "CalculiXFiniteStrain::computeCalculiXTangentOperatorFromCSE(DDSDDE,CSE,DFGRD1,STRESS);\n"
	<< "}\n"
	<< "}\n\n";
  }
  
  void
  CalculiXInterface::writeUMATxxBehaviourTypeSymbols(std::ostream& out,
						     const std::string& name,
						     const BehaviourDescription& mb) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CalculiXInterface::writeUMATxxBehaviourTypeSymbols: "+m));}
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourType = " ;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->fss==UNDEFINEDSTRATEGY){
	out << "1u;\n\n";
      } else {
	out << "2u;\n\n";
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "2u;\n\n";
    } else {
      throw_if(true,"unsupported behaviour type");
    }
  } // end of CalculiXInterface::writeUMATxxBehaviourTypeSymbols

  void
  CalculiXInterface::writeUMATxxBehaviourKinematicSymbols(std::ostream& out,
							  const std::string& name,
							  const BehaviourDescription& mb) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CalculiXInterface::writeUMATxxBehaviourKinematicSymbols: "+m));}
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name) 
	<< "_BehaviourKinematic = " ;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      if(this->fss==UNDEFINEDSTRATEGY){
	out << "1u;\n\n";
      } else {
	out << "3u;\n\n";
      }
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "3u;\n\n";
    } else {
      throw_if(true,"unsupported behaviour type");
    }
  } // end of CalculiXInterface::writeUMATxxBehaviourKinematicSymbols
  
  void 
  CalculiXInterface::writeInterfaceSpecificIncludes(std::ostream& out,
						    const BehaviourDescription&) const
  {
    out << "#include\"MFront/CalculiX/CalculiX.hxx\"\n"
	<< "#include\"MFront/CalculiX/CalculiXConvert.hxx\"\n\n";
  } // end of CalculiXInterface::writeInterfaceSpecificIncludes

  void 
  CalculiXInterface::writeBehaviourDataDrivingVariableSetter(std::ostream& os,
							     const DrivingVariable& v,
							     const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(!o.isNull()){
      throw(std::runtime_error("CalculiXInterface::writeBehaviourDataMainVariablesSetter : "
			       "only one driving variable supported"));
    }
    if(v.increment_known){
      os << "calculix::ImportDrivingVariables::exe(this->" << v.name << ","
	 << iprefix << "stran);\n";
    } else {
      os << "calculix::ImportDrivingVariables::exe(this->" << v.name << "0,"
	 << iprefix << "stran);\n";
    }
  } // end of CalculiXInterface::writeBehaviourDataDrivingVariableSetter

  void 
  CalculiXInterface::writeIntegrationDataDrivingVariableSetter(std::ostream& os,
							       const DrivingVariable& v,
							       const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(!o.isNull()){
      throw(std::runtime_error("CalculiXInterface::writeIntegrationDataMainVariablesSetter : "
			       "only one driving variable supported"));
    }
    if(v.increment_known){
      os << "calculix::ImportDrivingVariables::exe(this->d" << v.name << ","
	 << iprefix << "dstran);\n";
    } else {
      os << "calculix::ImportDrivingVariables::exe(this->" << v.name << "1,"
	 << iprefix << "dstran);\n";
    }
  } // end of CalculiXInterface::writeIntegrationDataDrivingVariableSetter
  
  void 
  CalculiXInterface::writeBehaviourDataThermodynamicForceSetter(std::ostream& os,
								const ThermodynamicForce& f,
								const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(SupportedTypes::getTypeFlag(f.type)==SupportedTypes::Stensor){
      os << "calculix::ImportThermodynamicForces::exe(this->" << f.name << ",";
      if(!o.isNull()){
	os << iprefix << "stress_+" << o << ");\n";
      } else {
	os << iprefix << "stress_);\n";
      }
    } else {
      throw(std::runtime_error("CalculiXInterface::writeBehaviourDataMainVariablesSetters : "
			       "unsupported forces type"));
    }
  } // end of CalculiXInterface::writeBehaviourDataThermodynamicForceSetter
  
  void 
  CalculiXInterface::exportThermodynamicForce(std::ostream& out,
					      const std::string& a,
					      const ThermodynamicForce& f,
					      const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if(flag==SupportedTypes::Stensor){
      if(!o.isNull()){
	out << "calculix::ExportThermodynamicForces::exe("
	    << a << "+" << o << ",this->sig);\n";
      } else {
	out << "calculix::ExportThermodynamicForces::exe(" << a << ",this->sig);\n";
      }
    } else {
      throw(std::runtime_error("CalculiXInterface::exportThermodynamicForce: "
			       "unsupported forces type"));
    }
  } // end of CalculiXInterface::exportThermodynamicForce

  void
  CalculiXInterface::getTargetsDescription(TargetsDescription& d,
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
    insert_if(d[lib].sources,"calculix"+name+".cxx");
    d.headers.push_back("MFront/CalculiX/calculix"+name+".hxx");
    insert_if(d[lib].ldflags,"-lCalculiXInterface");
    if(this->generateMTestFile){
      insert_if(d[lib].ldflags,"-lMTestFileGenerator");
    }
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    insert_if(d[lib].epts,this->getFunctionName(name));
  } // end of CalculiXInterface::getTargetsDescription

  std::string
  CalculiXInterface::getLibraryName(const BehaviourDescription& mb) const
  {
    auto lib = std::string{};
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = this->getInterfaceName()+mb.getMaterialName();
      } else {
	lib = this->getInterfaceName()+"Behaviour";
      }
    } else {
      lib = this->getInterfaceName()+mb.getLibrary();
    }
    return makeUpperCase(lib);
  } // end of CalculiXInterface::getLibraryName
  
  std::string CalculiXInterface::getFunctionName(const std::string& name) const
  {
    return makeUpperCase(name);
  } // end of CalculiXInterface::getFunctionName
  
  std::set<CalculiXInterface::Hypothesis>
  CalculiXInterface::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    const auto& bh = mb.getModellingHypotheses();
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)==bh.end()){
      throw(std::runtime_error("CalculiXInterface::getModellingHypothesesToBeTreated : "
			       "the 'Tridimensional' hypothesis is not supported, "
			       "which is required for the CalculiX interface"));
    }
    return {ModellingHypothesis::TRIDIMENSIONAL};
  } // end of CalculiXInterface::getModellingHypothesesToBeTreated

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
	    SupportedTypes::TypeSize>
  CalculiXInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
						 const Hypothesis h) const
  {
    using namespace std;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for(const auto &lh : ah){
	if(!mb.hasSpecialisedMechanicalData(lh)){
	  uh.insert(lh);
	}
      }
      if(uh.empty()){
	throw(runtime_error("CalculiXInterface::buildMaterialPropertiesList: "
			    "internal error : the mechanical behaviour says that not "
			    "all handled mechanical data are specialised, but we found none."));
      }
      // material properties for all the selected hypothesis
      auto mpositions = vector<pair<vector<UMATMaterialProperty>,
				    SupportedTypes::TypeSize>>{};
      for(const auto & lh : uh){
	mpositions.push_back(this->buildMaterialPropertiesList(mb,lh));
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
	    throw(runtime_error("CalculiXInterface::buildMaterialPropertiesList : "
				"incompatible offset for material property '"+mp.name+
				"' (aka '"+mp1.name+"'). This is one pitfall of the umat interface. "
				"To by-pass this limitation, you may want to explicitely "
				"specialise some modelling hypotheses"));
	  }
	}
      }
      return mfirst;
    }
    auto res = pair<vector<UMATMaterialProperty>,
		    SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    if((h!=ModellingHypothesis::GENERALISEDPLANESTRAIN)&&
       (h!=ModellingHypothesis::AXISYMMETRICAL)&&
       (h!=ModellingHypothesis::PLANESTRAIN)&&
       (h!=ModellingHypothesis::PLANESTRESS)&&
       (h!=ModellingHypothesis::TRIDIMENSIONAL)){
      string msg("CalculiXInterface::buildMaterialPropertiesList : "
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
	  throw(std::runtime_error("CalculiXInterface::buildMaterialPropertiesList : "
				   "unsupported modelling hypothesis"));
	}
      } else {
	throw(std::runtime_error("CalculiXInterface::buildMaterialPropertiesList : "
				 "unsupported behaviour symmetry type"));
      }
    }
    if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      if(mb.getSymmetryType()==mfront::ISOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion",
					     "alph",false);
      } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion1",
					     "alp1",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion2",
					     "alp2",false);
	this->appendToMaterialPropertiesList(mprops,"thermalexpansion","ThermalExpansion3",
					     "alp3",false);
      } else {
	throw(std::runtime_error("CalculiXInterface::buildMaterialPropertiesList : "
				 "unsupported behaviour symmetry type"));
      }
    }
    if(!mprops.empty()){
      const auto& m = mprops.back();
      res.second  = m.offset;
      res.second += SupportedTypes::getTypeSize(m.type,m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops,mb,h);
    return res;
  } // end of CalculiXInterface::buildMaterialPropertiesList

  void
  CalculiXInterface::writeCalculiXBehaviourTraits(std::ostream& out,
						  const BehaviourDescription& mb) const
  {
    using namespace std;
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb,h);
    out << "template<typename Type";
    if(mb.useQt()){
      out << ",bool use_qt";
    }
    out << ">\n"
	<< "struct CalculiXTraits<tfel::material::"
	<< mb.getClassName() << "<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,";
    out << "Type,";
    if(mb.useQt()){
      out << "use_qt";
    } else {
      out << "false";
    }
    out << ">>\n{\n"
	<< "//! behaviour type\n";
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " CalculiXBehaviourType btype = calculix::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " CalculiXBehaviourType btype = calculix::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else {
      throw(std::runtime_error("CalculiXInterface::writeCalculiXBehaviourTraits : "
			       "unsupported behaviour type"));
    }
    out << "//! space dimension\n";
    out << "static " << constexpr_c << " unsigned short N "
	<< "= tfel::material::ModellingHypothesisToSpaceDimension<tfel::material::ModellingHypothesis::TRIDIMENSIONAL>::value;\n";
    out << "// tiny vector size\n"
	<< "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
	<< "// symmetric tensor size\n"
	<< "static " << constexpr_c << " unsigned short StensorSize = tfel::math::StensorDimeToSize<N>::value;\n"
	<< "// tensor size\n"
	<< "static " << constexpr_c << " unsigned short TensorSize  = tfel::math::TensorDimeToSize<N>::value;\n"
	<< "// size of the driving variable array\n"
	<< "static " << constexpr_c << " unsigned short DrivingVariableSize = " << mvs.first <<  ";\n"
	<< "// size of the thermodynamic force variable array (STRESS)\n"
	<< "static " << constexpr_c << " unsigned short ThermodynamicForceVariableSize = " << mvs.second <<  ";\n";
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
      out << "static " << constexpr_c << " CalculiXSymmetryType type = calculix::ISOTROPIC;\n";
    } else if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "static " << constexpr_c << " CalculiXSymmetryType type = calculix::ORTHOTROPIC;\n";
    } else {
      throw(std::runtime_error("CalculiXInterface::writeCalculiXBehaviourTraits: "
			       "unsupported behaviour type.\n"
			       "The calculix interface only support isotropic or orthotropic "
			       "behaviour at this time."));
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if(!mprops.first.empty()){
      const auto& m = mprops.first.back();
      msize  = m.offset;
      msize += SupportedTypes::getTypeSize(m.type,m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c << " unsigned short material_properties_nb = " << msize << ";\n";
    if(mb.getElasticSymmetryType()==mfront::ISOTROPIC){
      out << "static " << constexpr_c << " CalculiXSymmetryType etype = calculix::ISOTROPIC;\n";
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
      out << "static " << constexpr_c << " CalculiXSymmetryType etype = calculix::ORTHOTROPIC;\n";
      if(mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,false)){
    	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset "
    	    << "= 9u;\n";
      } else {
	out << "static " << constexpr_c << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if(mb.getAttribute(BehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 3u;\n"; 
      } else {
	out << "static " << constexpr_c << " unsigned short thermalExpansionPropertiesOffset = 0u;\n"; 
      }
    } else {
      throw(std::runtime_error("CalculiXInterface::writeCalculiXBehaviourTraits: "
			       "unsupported behaviour type.\n"
			       "The calculix interface only support isotropic or "
			       "orthotropic behaviour at this time."));
    }
    out << "}; // end of class CalculiXTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis,std::string>
  CalculiXInterface::gatherModellingHypothesesAndTests(const BehaviourDescription& mb) const
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
  } // end of CalculiXInterface::gatherModellingHypothesesAndTests

  std::string CalculiXInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "true";
    }
    throw(std::runtime_error("CalculiXInterface::getModellingHypothesisTest : "
			     "unsupported modelling hypothesis"));
  } // end of CalculiXInterface::gatherModellingHypothesesAndTests

  bool CalculiXInterface::areExternalStateVariablesSupported() const{
    return false;
  } // end of CalculiXInterface::areExternalStateVariablesSupported()

  bool CalculiXInterface::isTemperatureIncrementSupported() const{
    return false;
  } // end of CalculiXInterface::isTemperatureIncrementSupported()
  
  void CalculiXInterface::writeUMATxxAdditionalSymbols(std::ostream&,
						       const std::string&,
						       const Hypothesis,
						       const BehaviourDescription&,
						       const FileDescription&) const
  {} // end of CalculiXInterface::writeUMATxxAdditionalSymbols

  void CalculiXInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "mg.setModellingHypothesis(ModellingHypothesis::TRIDIMENSIONAL);\n";
  } // end of CalculiXInterface::writeMTestFileGeneratorSetModellingHypothesis

  void
  CalculiXInterface::writeInputFileExample(const BehaviourDescription& mb,
					   const FileDescription& fd,
					   const bool b) const{ 
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("CalculiXInterface::writeInputFileExample: "+m));}
    };
    const auto name =  mb.getLibrary()+mb.getClassName();
    const auto mn = this->getLibraryName(mb)+"_"+mb.getClassName();
    const auto fn = (b ? "calculix/" : "calculix-explicit/") +name+".inp";
    std::ofstream out{fn};
    throw_if(!out,"could not open file '"+fn+"'");
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
	msize += SupportedTypes::getTypeSize(m.type,m.arraySize);
      }
      const auto& persistentVarsHolder = d.getPersistentVariables();
      auto vs = SupportedTypes::TypeSize{};
      for(const auto& v : persistentVarsHolder){
	vs+=SupportedTypes::getTypeSize(v.type,v.arraySize);
      }
      const auto vsize = vs.getValueForModellingHypothesis(h);
      const auto& externalStateVarsHolder = d.getExternalStateVariables();
      out << "** Example for the '" << ModellingHypothesis::toString(h) << "' modelling hypothesis\n";
      if(!externalStateVarsHolder.empty()){
	out << "** This behaviour requires " << externalStateVarsHolder.size()
	    <<  " field variables to be defined:\n";
	int i=1;
	for(auto pv=std::next(externalStateVarsHolder.begin()); // skipping the temperature
	    pv!=externalStateVarsHolder.end();++pv,++i){
	  out << "** " << i << ": " << mb.getExternalName(h,pv->name);
	}
      }
      out << "*Material, name=" << this->getFunctionName(mn) << '\n';
      if(!b){
	out << "*DENSITY\n<density>\n";
      }
      if(vsize!=0){
	out << "*Depvar\n" << vsize  << ",\n";
	int i=1;
	for(const auto& v : persistentVarsHolder){
	  const auto vn = mb.getExternalName(h,v.name);
	  if(v.arraySize==1){
	    this->writeDepvar(out,i,v,vn);
	  } else {
	    for(unsigned short a=0;a!=v.arraySize;++a){
	      this->writeDepvar(out,i,v,vn+'['+std::to_string(a)+']');
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
	auto write = [&i,&out](const std::string& n){
	  if(i%9==0){
	    out << "\n";
	    i=1;
	  }
	  out << '<' << n << '>';
	  ++i;
	};
	for(auto pm =mps.first.begin();pm!=mps.first.end();){
	  if(pm->arraySize==1u){
	    write(pm->name);
	  } else {
	    for(unsigned short a=0;a!=pm->arraySize;){
	      write(pm->name+"_"+std::to_string(a));
	      if(++a!=pm->arraySize){
		out << ", ";
	      }
	    }
	  }
	  if(++pm!=mps.first.end()){
	    out << ", ";
	  }
	}
      }
      out << "\n\n";
    }
  } // end of CalculiXInterface::writeInputFileExample

  void CalculiXInterface::writeDepvar(std::ostream& out,
				      int& i,
				      const VariableDescription& v,
				      const std::string& n) const {
    if(SupportedTypes::getTypeFlag(v.type)==SupportedTypes::Scalar){
      out << i++ << ", " << n << '\n';
    } else if(SupportedTypes::getTypeFlag(v.type)==SupportedTypes::Stensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      out << i++ << ", " << n << "_13\n";
      out << i++ << ", " << n << "_23\n";
    } else if(SupportedTypes::getTypeFlag(v.type)==SupportedTypes::Tensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      out << i++ << ", " << n << "_21\n";
      out << i++ << ", " << n << "_13\n";
      out << i++ << ", " << n << "_31\n";
      out << i++ << ", " << n << "_23\n";
      out << i++ << ", " << n << "_32\n";
    } else {
      throw(std::runtime_error("CalculiXInterface::writeDepvar: "
			       "unsupported variable type"));
    }
  }
  
  CalculiXInterface::~CalculiXInterface() = default;
  
} // end of namespace mfront
