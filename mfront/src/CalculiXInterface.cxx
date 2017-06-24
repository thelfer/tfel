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
		     const CalculiXInterface::FiniteStrainStrategy fss,
		     const bool f)
  {
    if(f){
      out << "(calculix::CalculiXReal *const STRESS,\n"
	  << " calculix::CalculiXReal *const STATEV,\n"
	  << " calculix::CalculiXReal *const DDSDDE,\n"
	  << " calculix::CalculiXReal *const SSE,\n"
	  << " calculix::CalculiXReal *const SPD,\n"
	  << " calculix::CalculiXReal *const SCD,\n"
	  << " calculix::CalculiXReal *const RPL,\n"
	  << " calculix::CalculiXReal *const DDSDDT,\n"
	  << " calculix::CalculiXReal *const DRPLDE,\n"
	  << " calculix::CalculiXReal *const DRPLDT,\n"
	  << " const calculix::CalculiXReal *const STRAN,\n"
	  << " const calculix::CalculiXReal *const DSTRAN,\n"
	  << " const calculix::CalculiXReal *const TIME,\n"
	  << " const calculix::CalculiXReal *const DTIME,\n"
	  << " const calculix::CalculiXReal *const TEMP,\n"
	  << " const calculix::CalculiXReal *const DTEMP,\n"
	  << " const calculix::CalculiXReal *const PREDEF,\n"
	  << " const calculix::CalculiXReal *const DPRED,\n"
	  << " const char           *const CMNAME,\n"
	  << " const calculix::CalculiXInt  *const NDI,\n"
	  << " const calculix::CalculiXInt  *const NSHR,\n"
	  << " const calculix::CalculiXInt  *const NTENS,\n"
	  << " const calculix::CalculiXInt  *const NSTATV,\n"
	  << " const calculix::CalculiXReal *const PROPS,\n"
	  << " const calculix::CalculiXInt  *const NPROPS,\n"
	  << " const calculix::CalculiXReal *const COORDS,\n"
	  << " const calculix::CalculiXReal *const DROT,\n"
	  << "       calculix::CalculiXReal *const PNEWDT,\n"
	  << " const calculix::CalculiXReal *const CELENT,\n"
	  << " const calculix::CalculiXReal *const DFGRD0,\n"
	  << " const calculix::CalculiXReal *const DFGRD1,\n"
	  << " const calculix::CalculiXInt  *const NOEL,\n"
	  << " const calculix::CalculiXInt  *const NPT,\n"
	  << " const calculix::CalculiXInt  *const LAYER,\n"
	  << " const calculix::CalculiXInt  *const KSPT,\n"
	  << " const calculix::CalculiXInt  *const KSTEP,\n"
	  << "       calculix::CalculiXInt  *const KINC,\n"
	  << "const int size)";
    } else {
      out << "(calculix::CalculiXReal *const STRESS,\n"
	  << " calculix::CalculiXReal *const STATEV,\n"
	  << " calculix::CalculiXReal *const DDSDDE,\n"
	  << " calculix::CalculiXReal *const,\n"
	  << " calculix::CalculiXReal *const,\n"
	  << " calculix::CalculiXReal *const,\n"
	  << " calculix::CalculiXReal *const,\n"
	  << " calculix::CalculiXReal *const,\n"
	  << " calculix::CalculiXReal *const,\n"
	  << " calculix::CalculiXReal *const,\n";
      if(t==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	out << " const calculix::CalculiXReal *const STRAN,\n"
	    << " const calculix::CalculiXReal *const DSTRAN,\n";
      } else {
	out << " const calculix::CalculiXReal *const,\n"
	    << " const calculix::CalculiXReal *const,\n";
      }
      out << " const calculix::CalculiXReal *const,\n"
	  << " const calculix::CalculiXReal *const DTIME,\n"
	  << " const calculix::CalculiXReal *const TEMP,\n"
	  << " const calculix::CalculiXReal *const DTEMP,\n"
	  << " const calculix::CalculiXReal *const PREDEF,\n"
	  << " const calculix::CalculiXReal *const DPRED,\n"
	  << " const char           *const,\n"
	  << " const calculix::CalculiXInt  *const,\n"
	  << " const calculix::CalculiXInt  *const,\n"
	  << " const calculix::CalculiXInt  *const NTENS,\n"
	  << " const calculix::CalculiXInt  *const NSTATV,\n"
	  << " const calculix::CalculiXReal *const PROPS,\n"
	  << " const calculix::CalculiXInt  *const NPROPS,\n"
	  << " const calculix::CalculiXReal *const,\n"
	  << " const calculix::CalculiXReal *const DROT,\n"
	  << "       calculix::CalculiXReal *const PNEWDT,\n"
	  << " const calculix::CalculiXReal *const,\n";
      if(t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const calculix::CalculiXReal *const F0,\n"
	    << " const calculix::CalculiXReal *const F1,\n";
      } else {
	out << " const calculix::CalculiXReal *const,\n"
	    << " const calculix::CalculiXReal *const,\n";
      }
      out << " const calculix::CalculiXInt  *const,\n"
	  << " const calculix::CalculiXInt  *const,\n"
	  << " const calculix::CalculiXInt  *const,\n"
	  << " const calculix::CalculiXInt  *const,\n";
      if((t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)||
	 ((fss!=CalculiXInterface::NATIVEFINITESTRAINSTRATEGY)&&
	  (fss!=CalculiXInterface::UNDEFINEDSTRATEGY))){
	out << " const calculix::CalculiXInt  *const KSTEP,\n";
      } else {
	out << " const calculix::CalculiXInt  *const,\n";
      }
      out << "       calculix::CalculiXInt  *const,\n"
	  << "const int)";
    }
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    out << "(calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const char           *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< "       calculix::CalculiXReal *const,\n"
        << " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXReal *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< " const calculix::CalculiXInt  *const,\n"
	<< "       calculix::CalculiXInt  *const,\n"
	<< "const int)";
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
	  "@CalculiXOrthotropyManagementPolicy",
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
    } else if (key=="@CalculiXOrthotropyManagementPolicy"){
      auto read = [&throw_if](const std::string& s){
	if(s=="Native"){
	  return NATIVEORTHOTROPYMANAGEMENTPOLICY;
	} else if(s=="MFront"){
	  return MFRONTORTHOTROPYMANAGEMENTPOLICY;
	}
	throw_if(true,"unsupported orthotropy management policy '"+s+"'\n"
		 "The only supported policies are "
		 "'MFront' and 'Native'");
      };
      throw_if(this->omp!=UNDEFINEDORTHOTROPYMANAGEMENTPOLICY,
	       "an orthotropy management policy has already been defined");
      throw_if(current==end,"unexpected end of file");
      this->omp = read(current->value);
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

  void
  CalculiXInterface::endTreatment(const BehaviourDescription& mb,
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
    if(this->compareToNumericalTangentOperator){
      throw_if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
	       "unsupported feature @CalculiXSaveTangentOperator "
	       "and @CalculiXCompareToNumericalTangentOperator : "
	       "those are only valid for small strain beahviours");
    }
    throw_if((mb.getSymmetryType()!=mfront::ORTHOTROPIC)&&
	     (this->omp!=UNDEFINEDORTHOTROPYMANAGEMENTPOLICY),
	     "orthotropy management policy is only valid "
	     "for orthotropic behaviour");
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if(((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	  (this->fss!=UNDEFINEDSTRATEGY))||
	 (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
	throw_if(this->omp!=MFRONTORTHOTROPYMANAGEMENTPOLICY,
		 "orthotropic finite strain behaviours are only supported with the "
		 "'MFront' othotropy management policy. See the "
		 "'@CalculiXOrthotropyManagementPolicy' for details");
      }
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/CalculiX");
    systemCall::mkdir("calculix");

    std::ofstream out;
    {
      // copy umat.cpp locally
      MFrontLockGuard lock;
      out.open("calculix/umat.cpp");
      if(out){
	const auto root = tfel::getInstallPath();
	const auto fn = root+"/share/doc/mfront/calculix/umat.cpp";
	std::ifstream in{fn};
	if(in){
	  out << in.rdbuf();
	  in.close();
	} else {
	  std::cerr << "CalculiXInterface::endTreatment: "
		    << "could not open file '" << fn << "'" << std::endl;
	}
      } else {
	  std::cerr << "CalculiXInterface::endTreatment: "
		    << "could not open file 'calculix/umat.cpp'" << std::endl;
      }
      out.close();
    }
    
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
	<< "#include\"MFront/CalculiX/CalculiXData.hxx\"\n\n";
    out << "#ifdef __cplusplus\n"
	<< "#include\"MFront/CalculiX/CalculiXTraits.hxx\"\n";
    if(this->fss!=UNDEFINEDSTRATEGY){
      out << "#include\"MFront/CalculiX/CalculiXFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
	<< "namespace calculix{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(mh)){
      this->writeCalculiXBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->writeCalculiXBehaviourTraits(out,mb,h);
      }
    }

    out << "} // end of namespace calculix\n\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#ifdef __cplusplus\n"
	<< "extern \"C\"{\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    for(const auto h: mh){
      out << "MFRONT_SHAREDOBJ void\n"
	  << this->getFunctionNameForHypothesis(name,h);
      writeUMATArguments(out);
      out << ";\n\n";
    }

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
    if(this->omp!=UNDEFINEDORTHOTROPYMANAGEMENTPOLICY){
      out << "#include\"MFront/CalculiX/CalculiXRotation.hxx\"\n\n";
    }
    out << "#include\"MFront/CalculiX/CalculiXStressFreeExpansionHandler.hxx\"\n\n"
	<< "#include\"MFront/CalculiX/CalculiXInterface.hxx\"\n\n"
	<< "#include\"MFront/CalculiX/calculix" << name << ".hxx\"\n\n";

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

    for(const auto h: mh){
      if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	 ((this->fss==UNDEFINEDSTRATEGY)||(this->fss==NATIVEFINITESTRAINSTRATEGY))){
	this->writeUMATSmallStrainFunction(out,mb,name,h);
      } else if((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
		((this->fss!=UNDEFINEDSTRATEGY)&&(this->fss!=NATIVEFINITESTRAINSTRATEGY))){
	if(this->fss==FINITEROTATIONSMALLSTRAIN){
	  this->writeUMATFiniteRotationSmallStrainFunction(out,mb,name,h);
	} else {
	  throw_if(true,"unsupported finite strain strategy !");
	}
      } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	this->writeUMATFiniteStrainFunction(out,mb,name,h);
      } else {
	throw_if(true,"the calculix interface only supports small "
		 "and finite strain behaviours");
      }
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb,fd,true);
  } // end of CalculiXInterface::endTreatment
  
  void CalculiXInterface::writeUMATFunctionBase(std::ostream& out,
					      const BehaviourDescription& mb,
					      const std::string& name,
					      const std::string& sfeh,
					      const Hypothesis h) const
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("CalculiXInterface::writeUMATFunctionBase: "+m));}
    };
    std::string dv0,dv1,sig,statev,nstatev;
    const auto btype = mb.getBehaviourType();
    out << "static void\n" << name << "_base" << this->getFunctionNameForHypothesis("",h);
    writeUMATArguments(out,btype,this->fss,false);
    out << "{\n";
    if((btype==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)||
       ((btype==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)&&
	((this->fss!=CalculiXInterface::NATIVEFINITESTRAINSTRATEGY)&&
	 (this->fss!=CalculiXInterface::UNDEFINEDSTRATEGY)))){
      out << "#ifndef MFRONT_CALCULIX_NORUNTIMECHECKS\n"
	  << "if(KSTEP[2]!=1){\n"
	  << "std::cerr << \"the " << name << " behaviour is only "
	  << "valid in finite strain analysis\\n\";\n"
	  << "*PNEWDT=-1;\n"
	  << "return;\n"
	  << "}\n"
	  << "#endif /* MFRONT_CALCULIX_NORUNTIMECHECKS */\n";
    }
    if(this->omp==MFRONTORTHOTROPYMANAGEMENTPOLICY){
      if(btype==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
    	// turning the deformation and the deformation gradient
    	// increment to the material frame
	out << "#ifndef MFRONT_CALCULIX_NORUNTIMECHECKS\n"
	    << "if(*NSTATV<6){\n"
	    << "std::cerr << \"" << name << this->getFunctionNameForHypothesis("",h) << ": \"\n"
	    << "          << \"invalid number of state variables\\n\";\n"
	    << "*PNEWDT = -1.;\n"
	    << "return;\n"
	    << "}\n"
	    << "#endif /* MFRONT_CALCULIX_NORUNTIMECHECKS */\n"
	    << "calculix::CalculiXRotation R(STATEV);\n"
	    << "calculix::CalculiXReal e[6u];\n"
	    << "calculix::CalculiXReal de[6u];\n"
	    << "calculix::CalculiXReal sm[6u];\n"
	    << "R.rotateStrainsForward(STRAN,e);\n"
	    << "R.rotateStrainsForward(DSTRAN,de);\n"
	    << "R.rotateStressesForward(STRESS,sm);\n"
	    << "const calculix::CalculiXInt nstatev = *NSTATV-6;\n";
	statev="STATEV+6";	    
    	dv0 = "e";
    	dv1 = "de";
    	sig = "sm";
	nstatev = "nstatev";
      } else if(btype==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << "#ifndef MFRONT_CALCULIX_NORUNTIMECHECKS\n"
	    << "if(*NSTATV<6){\n"
	    << "std::cerr << \"" << name << this->getFunctionNameForHypothesis("",h) << ": \"\n"
	    << "          << \"invalid number of state variables\\n\";\n"
	    << "*PNEWDT = -1.;\n"
	    << "return;\n"
	    << "}\n"
	    << "#endif /* MFRONT_CALCULIX_NORUNTIMECHECKS */\n"
	    << "calculix::CalculiXRotation R(STATEV);\n"
	    << "calculix::CalculiXReal Fm0[9u];\n"
	    << "calculix::CalculiXReal Fm1[9u];\n"
	    << "calculix::CalculiXReal sm[6u];\n"
	    << "R.rotateDeformationGradientForward(F0,Fm0);\n"
	    << "R.rotateDeformationGradientForward(F1,Fm1);\n"
	    << "R.rotateStressesForward(STRESS,sm);\n"
	    << "const calculix::CalculiXInt nstatev = *NSTATV-6;\n";
	statev="STATEV+6";
	dv0 = "Fm0";
	dv1 = "Fm1";
	sig = "sm";
	nstatev = "nstatev";
      } else {
    	throw_if(true,"the calculix interface only supports small "
    		 "and finite strain behaviours");
      }
    } else {
      if(btype==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	dv0 = "STRAN";
	dv1 = "DSTRAN";
      } else if(btype==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	dv0 = "F0";
	dv1 = "F1";
      } else {
    	throw_if(true,"the calculix interface only supports small "
    		 "and finite strain behaviours");
      }
      sig = "STRESS";
      statev="STATEV";
      nstatev="*NSTATV";
    }
    out << "calculix::CalculiXData d = {" << sig << ",PNEWDT,DDSDDE," << statev << ",\n"
	<< "                        *NTENS,*NPROPS," << nstatev << ",*DTIME,\n"
	<< "                        DROT," << dv0 << "," << dv1 << ",TEMP,DTEMP,\n"
	<< "                        PROPS,PREDEF,DPRED,\n"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	<< "if(calculix::CalculiXInterface<tfel::material::ModellingHypothesis::"
	<< ModellingHypothesis::toUpperCaseString(h) << ",tfel::material::" << mb.getClassName() 
	<< ">::exe(d)!=0){\n";
    out << "*PNEWDT = -1.;\n"
	<< "return;\n"
	<< "}\n";
    if(this->omp==MFRONTORTHOTROPYMANAGEMENTPOLICY){
      if(h==ModellingHypothesis::PLANESTRESS){
	out << "calculix::CalculiXReal D[16u] = {DDSDDE[0],DDSDDE[3],0,DDSDDE[6],\n"
	    << "                             DDSDDE[1],DDSDDE[4],0,DDSDDE[7],\n"
	    << "                             0,0,0,0,\n"
	    << "                             DDSDDE[2],DDSDDE[5],0,DDSDDE[8]};\n"
	    << "sm[3]=sm[2];\n"
	    << "sm[2]=0;\n"
	    << "R.rotateStressesBackward(sm,sg);\n"
	    << "R.rotateTangentOperatorBackward(D);\n"
	    << "STRESS[0]=sg[0];\n"
	    << "STRESS[1]=sg[1];\n"
	    << "STRESS[2]=sg[3];\n"
	    << "DDSDDE[0]=D[0];\n"
	    << "DDSDDE[1]=D[4];\n"
	    << "DDSDDE[2]=D[12];\n"
	    << "DDSDDE[3]=D[1];\n"
	    << "DDSDDE[4]=D[5];\n"
	    << "DDSDDE[5]=D[13];\n"
	    << "DDSDDE[6]=D[3];\n"
	    << "DDSDDE[7]=D[7];\n"
	    << "DDSDDE[8]=D[15];\n";
      } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
		 (h==ModellingHypothesis::PLANESTRAIN)||
		 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "R.rotateStressesBackward(sm,STRESS);\n"
	    << "R.rotateTangentOperatorBackward(DDSDDE);\n";
      } else {
	throw_if(true,"unsupported hypothesis");
      }
    }
    if(getDebugMode()){
      out << "std::cout << \"Dt :\" << std::endl;\n"
	  << "for(calculix::CalculiXInt i=0;i!=*NTENS;++i){\n"
	  << "for(calculix::CalculiXInt j=0;j!=*NTENS;++j){\n"
	  << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
	  << "}\n"
	  << "std::cout << std::endl;\n"
	  << "}\n"
	  << "std::cout << std::endl;\n";
    }
    out << "}\n\n";
  } // end of CalculiXInterface::writeUMATFunctionBase

  void CalculiXInterface::writeUMATFiniteStrainFunction(std::ostream& out,
						     const BehaviourDescription& mb,
						     const std::string& name,
						     const Hypothesis h) const
  {
    const std::string sfeh = "nullptr";
    this->writeUMATFunctionBase(out,mb,name,sfeh,h);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionNameForHypothesis(name,h);
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,this->fss,true);
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("",h)
	<< "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
	<< "STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
	<< "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
	<< "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n"
	<< "}\n\n";
  }
  
  void CalculiXInterface::writeUMATSmallStrainFunction(std::ostream& out,
						     const BehaviourDescription& mb,
						     const std::string& name,
						     const Hypothesis h) const
  {
    const std::string sfeh = "calculix::CalculiXStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out,mb,name,sfeh,h);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionNameForHypothesis(name,h);
    writeUMATArguments(out,BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,this->fss,true);
    out << "{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "calculix::CalculiXReal pnewdt0(*PNEWDT);\n"
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
    out << name << "_base" << this->getFunctionNameForHypothesis("",h)
	<< "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
	<< "STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
	<< "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
	<< "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n";
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
	  << "calculix::CalculiXReal pnewdt(pnewdt0);\n"
	  << "calculix::CalculiXReal m;\n"
	  << "calculix::CalculiXReal mDt;\n"
	  << "calculix::CalculiXReal mnDt;\n"
	  << "for(calculix::CalculiXInt i=0;i!=*NTENS;++i){\n"
	  << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "std::copy(sig0.begin(),sig0.end(),sigf.begin());\n"
	  << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] += " << this->strainPerturbationValue << ";\n"
	  << "D[0] = 0.;\n"
	  << "calculix::CalculiXData d2 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	  << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
	  << "                         PROPS,PREDEF,DPRED,\n"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	  << "if(calculix::CalculiXInterface<tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ","
	  << "tfel::material::" << mb.getClassName() << ">::exe(d2)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "calculix::CalculiXReal pnewdt(pnewdt0);\n"
	  << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
	  << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] -= " << this->strainPerturbationValue << ";\n"
	  << "D[0] = 0.;\n"
	  << "calculix::CalculiXData d3 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	  << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
	  << "                         PROPS,PREDEF,DPRED,\n"
	  << "if(calculix::CalculiXInterface<tfel::material::ModellingHypothesis::"
	  << ModellingHypothesis::toUpperCaseString(h) << ","
	  << "tfel::material::" << mb.getClassName() << ">::exe(d3)!=0){\n"
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
							      const std::string& name,
							      const Hypothesis h) const
  {
    if(h==ModellingHypothesis::PLANESTRESS){
      throw(std::runtime_error("CalculiXInterface::writeUMATFiniteRotationSmallStrainFunction: "
			       "plane stress is not supported yet"));
    }
    const auto ps = h==ModellingHypothesis::PLANESTRESS ? "true" : "false";
    const std::string sfeh = "calculix::CalculiXStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out,mb,name,sfeh,h);
    out << "MFRONT_SHAREDOBJ void\n"
	<< this->getFunctionNameForHypothesis(name,h);
    writeUMATArguments(out,BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,this->fss,true);
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
	<< "CalculiXFiniteStrain::computeGreenLagrangeStrain(eto,DFGRD0,*NTENS," << ps << ");\n"
	<< "CalculiXFiniteStrain::computeGreenLagrangeStrain(deto,DFGRD1,*NTENS," << ps << ");\n"
	<< "CalculiXFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,DFGRD0,*NTENS," << ps << ",0);\n"
	<< "for(int i=0;i!=*NTENS;++i){\n"
	<< "deto[i] -= eto[i];\n"
	<< "}\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "}\n";
    }
    out	<< name << "_base" << this->getFunctionNameForHypothesis("",h)
	<< "(STRESS,STATEV,CSE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
	<< "eto,deto,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
	<< "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
	<< "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n";
    out << "if(*PNEWDT>=0.99999){\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName() << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "CalculiXFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,DFGRD1,*NTENS," << ps << ",0);\n";
    out << "CalculiXFiniteStrain::computeCalculiXTangentOperatorFromCSE(DDSDDE,CSE,DFGRD1,STRESS,*NTENS," << ps << ");\n";
    out << "}\n";
    out << "}\n\n";
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

  std::vector<std::pair<std::string,std::string>>
  CalculiXInterface::getBehaviourDataConstructorAdditionalVariables() const{
    return {{"DR","increment of rigid body rotation"}};
  } // end of CalculiXInterface::getBehaviourDataConstructorAdditionalVariables

  void 
  CalculiXInterface::writeBehaviourDataMainVariablesSetters(std::ostream& os,
							  const BehaviourDescription& mb) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov,of;
    os << "void set"
       << iprefix << "BehaviourDataDrivingVariables(const Type* const " << iprefix << "stran)\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataDrivingVariableSetter(os,v.first,ov);
      ov += SupportedTypes::getTypeSize(v.first.type,1u);
    }
    os << "}\n\n";
    os << "void set"
       << iprefix << "BehaviourDataThermodynamicForces(const Type* const " << iprefix << "stress_,\n"
       << "                                                        const Type* const )\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataThermodynamicForceSetter(os,v.second,of);
      of += SupportedTypes::getTypeSize(v.second.type,1u);
    }
    os << "}\n\n";
  } // end of CalculiXInterface::writeBehaviourDataMainVariablesSetters

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
      os << "calculix::UMATImportDrivingVariables<hypothesis>::exe(this->" << v.name << ","
	 << iprefix << "stran);\n";
    } else {
      os << "calculix::UMATImportDrivingVariables<hypothesis>::exe(this->" << v.name << "0,"
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
      os << "calculix::UMATImportDrivingVariables<hypothesis>::exe(this->d" << v.name << ","
	 << iprefix << "dstran);\n";
    } else {
      os << "calculix::UMATImportDrivingVariables<hypothesis>::exe(this->" << v.name << "1,"
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
      os << "calculix::UMATImportThermodynamicForces<hypothesis>::exe(this->" << f.name << ",";
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
  CalculiXInterface::completeBehaviourDataConstructor(std::ostream& out,
						    const Hypothesis h,
						    const BehaviourDescription& mb) const
  {
    auto do_nothing = [&out](){
      out << "static_cast<void>(CALCULIXDR);\n";
    };
    /* 
     * We apply the rotation associated to the Jauman corotationnal frame only if:
     * - the behaviour symmetry is isotropic
     * - the behaviour is written in small strain
     * - the finite strain strategy is either undefined or `Native`
     */
    const bool c = ((mb.getSymmetryType()==mfront::ISOTROPIC)&&
		    (mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
		    ((this->fss==UNDEFINEDSTRATEGY)||(this->fss==NATIVEFINITESTRAINSTRATEGY)));
    if(!c){
      do_nothing();
      return;
    }
    // checking if there are variables that need to be rotated
    const auto& d = mb.getBehaviourData(h);
    bool b = false; // have persistent variables that have to be updated
    for(const auto& v:d.getPersistentVariables()){
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if((flag==SupportedTypes::Stensor)||
	 (flag==SupportedTypes::Tensor)){
	b = true;
	break;
      }
    }
    if(!b){
      do_nothing();
      return;
    }
    // rotate variables
    out << "const tfel::math::tmatrix<3u,3u,real> calculix_dr = {CALCULIXDR[0],CALCULIXDR[1],CALCULIXDR[2],\n"
      "                                                        CALCULIXDR[3],CALCULIXDR[4],CALCULIXDR[5],\n"
      "                                                        CALCULIXDR[6],CALCULIXDR[7],CALCULIXDR[8]};\n";
    for(const auto& v:d.getPersistentVariables()){
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if((flag==SupportedTypes::Stensor)||
	 (flag==SupportedTypes::Tensor)){
	if(v.arraySize==1u){
	  out << "this->" << v.name << ".changeBasis(calculix_dr);\n";
	} else {
	  for(unsigned short i=0;i!=v.arraySize;++i){
	    out << "this->" << v.name << "[" << i << "].changeBasis(calculix_dr);\n";
	  }
	}
      }
    }
  } // end of UMATInterface::completeBehaviourDataConstructor

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
	out << "calculix::UMATExportThermodynamicForces<hypothesis>::exe("
	    << a << "+" << o << ",this->sig);\n";
      } else {
	out << "calculix::UMATExportThermodynamicForces<hypothesis>::exe(" << a << ",this->sig);\n";
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
    for(const auto h : this->getModellingHypothesesToBeTreated(bd)){
      insert_if(d[lib].epts,this->getFunctionNameForHypothesis(name,h));
    }
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

  unsigned short
  CalculiXInterface::getStateVariablesOffset(const BehaviourDescription&,
					       const Hypothesis h) const{
    if(this->omp==MFRONTORTHOTROPYMANAGEMENTPOLICY){
      if((h==ModellingHypothesis::AXISYMMETRICAL)||
	 (h==ModellingHypothesis::PLANESTRAIN)||
	 (h==ModellingHypothesis::PLANESTRESS)){
	return 2u;
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return 6u;
      }
      throw(std::runtime_error("CalculiXInterface::getStateVariablesOffset: "
			       "invalid hypothesis"));
    }
    return 0u;
  }
  
  std::string CalculiXInterface::getFunctionName(const std::string& name) const
  {
    return makeUpperCase(name);
  } // end of CalculiXInterface::getFunctionName

  std::string CalculiXInterface::getFunctionNameForHypothesis(const std::string& name,
								const Hypothesis h) const
  {
    const auto s = [h]() -> std::string {
      if(h==ModellingHypothesis::AXISYMMETRICAL){
	return "AXIS";
      } else if(h==ModellingHypothesis::PLANESTRAIN){
	return "PSTRAIN";
      } else if(h==ModellingHypothesis::PLANESTRESS){
	return "PSTRESS";
      } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	return "3D";
      }
      throw(std::runtime_error("CalculiXInterface::getFunctionNameForHypothesis: "
			       "invalid hypothesis."));
    }();
    return makeUpperCase(name)+"_"+s;
  } // end of CalculiXInterface::getFunctionNameForHypothesis
  
  std::set<CalculiXInterface::Hypothesis>
  CalculiXInterface::getModellingHypothesesToBeTreated(const BehaviourDescription& mb) const
  {
    auto h = std::set<Hypothesis>{};
    const auto& bh = mb.getModellingHypotheses();
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
      throw(std::runtime_error("CalculiXInterface::getModellingHypothesesToBeTreated : "
			       "no hypotheses selected. This means that the given beahviour "
			       "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
			       "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
			       "make sense to use the CalculiX interface"));
    }
    return h;
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
						  const BehaviourDescription& mb,
						  const Hypothesis h) const
  {
    using namespace std;
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
    out << "struct CalculiXTraits<tfel::material::" << mb.getClassName() << "<";
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
      out << "static " << constexpr_c << " CalculiXBehaviourType btype = calculix::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      out << "static " << constexpr_c << " CalculiXBehaviourType btype = calculix::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else {
      throw(std::runtime_error("CalculiXInterface::writeCalculiXBehaviourTraits : "
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

  std::string
  CalculiXInterface::getModellingHypothesisTest(const Hypothesis h) const
  {
    if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "*NTENS==4";
    } else if(h==ModellingHypothesis::PLANESTRESS){
      return "*NTENS==3";
    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "*NTENS==6";
    }
    throw(std::runtime_error("CalculiXInterface::getModellingHypothesisTest : "
			     "unsupported modelling hypothesis"));
  } // end of CalculiXInterface::gatherModellingHypothesesAndTests

  void
  CalculiXInterface::writeUMATxxAdditionalSymbols(std::ostream&,
					      const std::string&,
					      const Hypothesis,
					      const BehaviourDescription&,
					      const FileDescription&) const
  {} // end of CalculiXInterface::writeUMATxxAdditionalSymbols

  void CalculiXInterface::writeUMATxxSpecificSymbols(std::ostream& out,
						       const std::string& name,
						       const BehaviourDescription& mb,
						       const FileDescription&) const
  {
    if(mb.getSymmetryType()==mfront::ORTHOTROPIC){
      if(this->omp==MFRONTORTHOTROPYMANAGEMENTPOLICY){
	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	    << "_OrthotropyManagementPolicy = 2u;\n\n";    
      } else if(this->omp==NATIVEORTHOTROPYMANAGEMENTPOLICY){
	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	    << "_OrthotropyManagementPolicy = 1u;\n\n";    
      } else if(this->omp==UNDEFINEDORTHOTROPYMANAGEMENTPOLICY){
	out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
	    << "_OrthotropyManagementPolicy = 0u;\n\n";    
      } else {
	throw(std::runtime_error("CalculiXInterface::writeUMATxxSpecificSymbols: "
				 "unsupported orthotropy management policy"));
      }
    }
  }
  
  void
  CalculiXInterface::writeMTestFileGeneratorSetModellingHypothesis(std::ostream& out) const
  {
    out << "ModellingHypothesis::Hypothesis h;\n"
	<< "if( *NTENS == 6 ){\n"
	<< "  h = ModellingHypothesis::TRIDIMENSIONAL;\n"
	<< "} else if(*NTENS==3){\n"
	<< "  h = ModellingHypothesis::PLANESTRESS;\n"
	<< "} else if(*NTENS==4){\n"
	<< "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;\n"
	<< "} else {\n"
	<< "  return;\n"
	<< "}\n"
	<< "mg.setModellingHypothesis(h);\n";
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
      const auto vsize = vs.getValueForModellingHypothesis(h)+ 
	this->getStateVariablesOffset(mb,h);
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
      out << "*Material, name="
	  << this->getFunctionNameForHypothesis(mn,h) << '\n';
      if(!b){
	out << "*DENSITY\n<density>\n";
      }
      if(vsize!=0){
	out << "*Depvar\n" << vsize  << ",\n";
	int i=1;
	if(this->omp!=UNDEFINEDORTHOTROPYMANAGEMENTPOLICY){
	  if(this->omp==MFRONTORTHOTROPYMANAGEMENTPOLICY){
	    if((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRAIN)||
	       (h==ModellingHypothesis::PLANESTRESS)){
	      out << i++ << ", FirstOrthotropicAxis_1\n";
	      out << i++ << ", FirstOrthotropicAxis_2\n";
	    } else if(h==ModellingHypothesis::TRIDIMENSIONAL){
	      out << i++ << ", FirstOrthotropicAxis_1\n";
	      out << i++ << ", FirstOrthotropicAxis_2\n";
	      out << i++ << ", FirstOrthotropicAxis_3\n";
	      out << i++ << ", SecondOrthotropicAxis_1\n";
	      out << i++ << ", SecondOrthotropicAxis_2\n";
	      out << i++ << ", SecondOrthotropicAxis_3\n";
	    }
	  } else {
	    throw_if(this->omp!=NATIVEORTHOTROPYMANAGEMENTPOLICY,
		     "unsupported orthotropy management policy");
	  }
	}
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

  void 
  CalculiXInterface::writeDepvar(std::ostream& out,
				   int& i,
				   const Hypothesis& h,
				   const VariableDescription& v,
				   const std::string& n) const {
    if(SupportedTypes::getTypeFlag(v.type)==SupportedTypes::Scalar){
      out << i++ << ", " << n << '\n';
    } else if(SupportedTypes::getTypeFlag(v.type)==SupportedTypes::Stensor){
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      if(h==ModellingHypothesis::TRIDIMENSIONAL){
	out << i++ << ", " << n << "_13\n";
	out << i++ << ", " << n << "_23\n";
      }
    } else if(SupportedTypes::getTypeFlag(v.type)==SupportedTypes::Tensor){
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
      throw(std::runtime_error("CalculiXInterface::writeDepvar: "
			       "unsupported variable type"));
    }
  }
  
  CalculiXInterface::~CalculiXInterface() = default;
  
} // end of namespace mfront
