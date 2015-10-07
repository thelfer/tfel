/*! 
 * \file   mfront/mtest/MTestParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */


#include<map>
#include<cmath>
#include<string>
#include<vector>
#include<memory>
#include<sstream>
#include<cstdlib>
#include<iterator>
#include<iostream>
#include<algorithm>
#include<stdexcept>

#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif

#include"tfel-config.hxx"

#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"

#include"MFront/MFrontLogStream.hxx"
#include"MTest/MTestParser.hxx"
#include"MTest/Behaviour.hxx"

#include"MTest/AnalyticalTest.hxx"
#include"MTest/ReferenceFileComparisonTest.hxx"

#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/CastemEvolution.hxx"

#include"MTest/Constraint.hxx"
#include"MTest/ImposedThermodynamicForce.hxx"
#include"MTest/ImposedDrivingVariable.hxx"

namespace mtest
{

  static unsigned short
  getSTensorSize(const unsigned short d)
  {
    if(d==1){
      return 3;
    } else if(d==2){
      return 4;
    } else if(d==3){
      return 6;
    }
    throw(std::runtime_error("mfront::getSTensorSize : "
			     "invalid dimension"));
  }

  static unsigned short
  getTensorSize(const unsigned short d)
  {
    if(d==1){
      return 3;
    } else if(d==2){
      return 5;
    } else if(d==3){
      return 9;
    }
    throw(std::runtime_error("mfront::getTensorSize : "
			     "invalid dimension"));
  }

  MTestParser::MTestParser()
  {
    this->registerCallBacks();
  }

  void
  MTestParser::parseString(MTest& t,const std::string& f)
  {
    this->file = "user defined string";
    this->treatCharAsString(true);
    CxxTokenizer::parseString(f);
    this->stripComments();
    this->execute(t);
  } // end of MTestParser::parseString

  void
  MTestParser::execute(MTest& t,const std::string& f)
  {
    this->file = f;
    this->treatCharAsString(true);
    this->openFile(f);
    this->stripComments();
    this->execute(t);
  }
  
  void
  MTestParser::execute(MTest& t)
  {
    auto p = this->fileTokens.cbegin();
    while(p!=fileTokens.end()){
      auto pc=this->callbacks.find(p->value);
      if(pc==this->callbacks.end()){
	std::ostringstream msg;
	msg << "MTestParser::execute : invalid keyword '"
	    << p->value << "'. Error at line " 
	    << p->line<< ".";
	throw(std::runtime_error(msg.str()));
      }
      if(mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG){
	auto& log = mfront::getLogStream();
	log << "MTestParser::execute : treating keyword '" << p->value
	    << "' at line '" << p->line << "'\n";
      }
      ++p;
      auto line = p->line;
      try{
	(this->*(pc->second))(t,p);
      } catch(std::exception& e){
	std::ostringstream msg;
	msg << "MTestParser::MTest : error while "
	    << "parsing file '" << this->file << "' at line '"
	    << line << "'.\n" << e.what();
	throw(std::runtime_error(msg.str()));
      }
    }
  } // end of MTestParser::execute

#if defined _WIN32 || defined _WIN64
  static bool
  getValueInRegistry(std::string &value)
  {
    using namespace std;
    HKEY  hKey;
    char  szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG  nError;
    LONG  lRes = RegOpenKeyEx(HKEY_CLASSES_ROOT,"TFELHOME-" VERSION,0,KEY_READ,&hKey);
    if(ERROR_SUCCESS != lRes){
      return false;
    }
    nError = RegQueryValueEx(hKey,"",nullptr,nullptr,
			     reinterpret_cast<LPBYTE>(szBuffer),
			     &dwBufferSize);
    RegCloseKey(hKey);
    if (ERROR_SUCCESS == nError){
      value = szBuffer;
      return true;
    }
    return false;
  }
#endif
  
  static std::string
  handleSpace(const std::string& p)
  {
    using namespace std;
    if(find(p.begin(),p.end(),' ')!=p.end()){
#if defined _WIN32 || defined _WIN64
      string msg("tfel-config handleSpace: "
		 "path to TFEL shall not contain space as "
		 "MinGW can't handle it (Found '"+p+"'). "
		 "Please change TFEL installation directory");
      throw(runtime_error(msg));
#endif
      return '"'+p+'"';
    }
  return p;
  }

  static std::string
  getTFELHOME(void)
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64
    // check in the registry (installation through NSIS)
    string rpath;
    if(getValueInRegistry(rpath)){
      return handleSpace(rpath);
    }
#endif
    const char * const path = getenv("TFELHOME");
    if(path!=nullptr){
      return handleSpace(path);
    }
#if defined _WIN32 || defined _WIN64
    string msg("tfel-config getTFELHOME: "
	       "no TFELHOME registry key defined and no TFEHOME "
	       "environment variable defined");
    throw(runtime_error(msg));
#endif
    return "";
  }
  
  void
  MTestParser::displayKeyWordsList(void) const
  {
    using namespace std;
    using namespace tfel::utilities;
    map<string,CallBack>::const_iterator pk;
    string::size_type msize = 0;
    for(pk=this->callbacks.begin();
	pk!=this->callbacks.end();++pk){
      msize = max(msize,pk->first.size());
    }
    for(pk=this->callbacks.begin();pk!=this->callbacks.end();++pk){
      string root = getTFELHOME();
      if(root.empty()){
	root = PREFIXDIR;
      }
      const string f = root+"/share/doc/mtest/"+pk->first.substr(1)+".md";
      ifstream desc(f.c_str());
      bool exists(true);
      if(!desc){
	exists = false;
      }
      string key = pk->first;
      key.resize(msize,' ');
      cout << key << "  ";
      if(exists){
	cout.write(TerminalColors::Green,sizeof(TerminalColors::Green));
	cout << "(documented)";
      } else {
	cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
	cout << "(undocumented)";
      }
      cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
      cout << endl;
    }
  } // end of MTestParser::displayKeywordsList

  void
  MTestParser::displayKeyWordsHelp(void) const
  {
    std::cout << "% `MTest` keywords\n\n";
    for(const auto& c  : this->callbacks){
      const auto& k  = c.first; 
      auto root = getTFELHOME();
      if(root.empty()){
	root = PREFIXDIR;
      }
      std::cout << "\n# The `" <<  k << "` keyword\n\n";
      std::ifstream desc{root+"/share/doc/mtest/"+k.substr(1)+".md"};
      if(desc){
	std::cout << desc.rdbuf();
      } else {
	std::cout << "The keyword `" <<  k << "` is not documented yet\n";
      }
    }
  } // end of MTestParser::displayKeywordsHelp
  
  void
  MTestParser::registerCallBacks()
  {
    this->registerCallBack("@AccelerationAlgorithm",
			   &MTestParser::handleAccelerationAlgorithm);
    this->registerCallBack("@AccelerationAlgorithmParameter",
			   &MTestParser::handleAccelerationAlgorithmParameter);
    this->registerCallBack("@UseCastemAccelerationAlgorithm",
			   &MTestParser::handleUseCastemAccelerationAlgorithm);
    this->registerCallBack("@CastemAccelerationPeriod",
			   &MTestParser::handleCastemAccelerationPeriod);
    this->registerCallBack("@CastemAccelerationTrigger",
			   &MTestParser::handleCastemAccelerationTrigger);
    this->registerCallBack("@StiffnessMatrixType",
			   &MTestParser::handleStiffnessMatrixType);
    this->registerCallBack("@StiffnessUpdatePolicy",
			   &MTestParser::handleStiffnessUpdatePolicy);
    this->registerCallBack("@PredictionPolicy",
			   &MTestParser::handlePredictionPolicy);
    this->registerCallBack("@Author",
			   &MTestParser::handleAuthor);
    this->registerCallBack("@Date",
			   &MTestParser::handleDate);
    this->registerCallBack("@Description",
			   &MTestParser::handleDescription);
    this->registerCallBack("@OutOfBoundsPolicy",
			   &MTestParser::handleOutOfBoundsPolicy);
    this->registerCallBack("@Parameter",
			   &MTestParser::handleParameter);
    this->registerCallBack("@IntegerParameter",
			   &MTestParser::handleIntegerParameter);
    this->registerCallBack("@UnsignedIntegerParameter",
			   &MTestParser::handleUnsignedIntegerParameter);
    this->registerCallBack("@OutputFile",
			   &MTestParser::handleOutputFile);
    this->registerCallBack("@OutputFilePrecision",
			   &MTestParser::handleOutputFilePrecision);
    this->registerCallBack("@ResidualFile",
			   &MTestParser::handleResidualFile);
    this->registerCallBack("@ResidualFilePrecision",
			   &MTestParser::handleResidualFilePrecision);
    this->registerCallBack("@Test",
			   &MTestParser::handleTest);
    this->registerCallBack("@Real",
			   &MTestParser::handleReal);
    this->registerCallBack("@Evolution",
			   &MTestParser::handleEvolution);
    this->registerCallBack("@RotationMatrix",
			   &MTestParser::handleRotationMatrix);
    this->registerCallBack("@MaximumNumberOfIterations",
			   &MTestParser::handleMaximumNumberOfIterations);
    this->registerCallBack("@MaximumNumberOfSubSteps",
			   &MTestParser::handleMaximumNumberOfSubSteps);
    this->registerCallBack("@StrainEpsilon",
			   &MTestParser::handleStrainEpsilon);
    this->registerCallBack("@DeformationGradientEpsilon",
			   &MTestParser::handleDeformationGradientEpsilon);
    this->registerCallBack("@OpeningDisplacementEpsilon",
			   &MTestParser::handleOpeningDisplacementEpsilon);
    this->registerCallBack("@DrivingVariableEpsilon",
			   &MTestParser::handleDrivingVariableEpsilon);
    this->registerCallBack("@StressEpsilon",
			   &MTestParser::handleStressEpsilon);
    this->registerCallBack("@CohesiveForceEpsilon",
			   &MTestParser::handleCohesiveForceEpsilon);
    this->registerCallBack("@ThermodynamicForceEpsilon",
			   &MTestParser::handleThermodynamicForceEpsilon);
    this->registerCallBack("@ModellingHypothesis",
			   &MTestParser::handleModellingHypothesis);
    this->registerCallBack("@Strain",&MTestParser::handleStrain);
    this->registerCallBack("@OpeningDisplacement",&MTestParser::handleOpeningDisplacement);
    this->registerCallBack("@DrivingVariable",&MTestParser::handleDrivingVariable);
    this->registerCallBack("@DeformationGradient",&MTestParser::handleDeformationGradient);
    this->registerCallBack("@Stress",&MTestParser::handleStress);
    this->registerCallBack("@CohesiveForce",&MTestParser::handleCohesiveForce);
    this->registerCallBack("@ThermodynamicForce",&MTestParser::handleThermodynamicForce);
    this->registerCallBack("@Times",&MTestParser::handleTimes);
    this->registerCallBack("@Behaviour",&MTestParser::handleBehaviour);
    this->registerCallBack("@MaterialProperty",
			   &MTestParser::handleMaterialProperty);
    this->registerCallBack("@InternalStateVariable",
			   &MTestParser::handleInternalStateVariable);
    this->registerCallBack("@ExternalStateVariable",
			   &MTestParser::handleExternalStateVariable);
    this->registerCallBack("@ImposedStrain",
			   &MTestParser::handleImposedStrain);
    this->registerCallBack("@ImposedOpeningDisplacement",
			   &MTestParser::handleImposedOpeningDisplacement);
    this->registerCallBack("@ImposedDeformationGradient",
			   &MTestParser::handleImposedDeformationGradient);
    this->registerCallBack("@ImposedDrivingVariable",
			   &MTestParser::handleImposedDrivingVariable);
    this->registerCallBack("@ImposedStress",
			   &MTestParser::handleImposedStress);
    this->registerCallBack("@ImposedCohesiveForce",
			   &MTestParser::handleImposedCohesiveForce);
    this->registerCallBack("@ImposedThermodynamicForce",
			   &MTestParser::handleImposedThermodynamicForce);
    this->registerCallBack("@CompareToNumericalTangentOperator",
			   &MTestParser::handleCompareToNumericalTangentOperator);
    this->registerCallBack("@TangentOperatorComparisonCriterium",
			   &MTestParser::handleTangentOperatorComparisonCriterium);
    this->registerCallBack("@NumericalTangentOperatorPerturbationValue",
			   &MTestParser::handleNumericalTangentOperatorPerturbationValue);
    this->registerCallBack("@HandleThermalExpansion",
			   &MTestParser::handleHandleThermalExpansion);
  }

  void
  MTestParser::registerCallBack(const std::string& k,
				const MTestParser::CallBack& p)
  {
    this->callbacks.insert({k,p});
  }

  void
  MTestParser::displayKeyWordDescription(const std::string& k) const
  {
    using namespace std;
    if(this->callbacks.find(k)==this->callbacks.end()){
      string msg("MTestParser::displayKeyWordDescription : "
		 "unknowns keyword '"+k+"'");
      throw(runtime_error(msg));
    }
    const char * const path = getenv("TFELHOME");
    const auto root = (path!=nullptr) ? string(path) : PREFIXDIR;
    const auto f = root+"/share/doc/mtest/"+k.substr(1)+".md";
    ifstream desc{f};
    if(!desc){
      cout << "no description available for keyword '"
	   << k << "'" << endl;
      return;
    }
    cout << desc.rdbuf();
  }

  std::string
  MTestParser::readUntilEndOfInstruction(TokensContainer::const_iterator& p)
  {
    using namespace std;
    string res;
    this->checkNotEndOfLine("MTestParser::readUntilEndOfInstruction",
			    p,this->fileTokens.end());
    while((p!=this->fileTokens.end())&&
	  (p->value != ";")){
      if(!p->value.empty()){
	if(p->value[0]=='@'){
	  string msg("MTestParser::readUntilEndOfInstruction : ");
	  msg += "no word beginning with '@' are allowed here";
	  throw(runtime_error(msg));
	}
	res+=p->value;
	res+=" ";
      }
      ++p;
    }
    this->readSpecifiedToken("MTestParser::readUntilEndOfInstruction",";",
			     p,this->fileTokens.end());
    return res;
  }

  void
  MTestParser::handleDescription(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    this->readSpecifiedToken("MTestParser::handleDescription","{",
			     p,this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleDescription",
			    p,this->fileTokens.end());
    auto currentLine = p->line;
    auto openedBrackets = 1u;
    string description;
    while((!((p->value=="}")&&
	     (openedBrackets==1u)))&&
	  (p!=this->fileTokens.end())){
      if(p->value=="{"){
	TokensContainer::const_iterator previous = p;
	--previous;
	if((previous->value.size()>0)&&
	   (previous->value[previous->value.size()-1]!='\\')){
	  ++openedBrackets;
	}
      }
      if(p->value=="}"){
	TokensContainer::const_iterator previous = p;
	--previous;
	if((previous->value.size()>0)&&
	   (previous->value[previous->value.size()-1]!='\\')){
	  --openedBrackets;
	}
      }
      if(currentLine!=p->line){
	description+="\n";
	currentLine=p->line;
      }
      if(p->flag==Token::String){
	description+=p->value.substr(1,p->value.size()-2);
      } else {
	description+=p->value;
      }
      description+=" ";
      ++p;
    }
    if(p==this->fileTokens.end()){
      --p;
      string msg("MTestParser::handleDescription",
		 "File ended before the end of description.");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleDescription",";",
			     p,this->fileTokens.end());
    t.setDescription(description);
  } // end of MTestParser::Description

  void MTestParser::handleCompareToNumericalTangentOperator(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    this->checkNotEndOfLine("handleCompareToNumericalTangentOperator",
			    p,this->fileTokens.end());
    if(p->value=="true"){
      t.setCompareToNumericalTangentOperator(true);
    } else if(p->value=="false"){
      t.setCompareToNumericalTangentOperator(false);
    } else {
      string msg("MTestParser::handleCompareToNumericalTangentOperator : "
		 "unexpected value (expected 'true' or 'false', read '"+p->value+"')");
      throw(runtime_error(msg));
    }
    ++p;
    this->checkNotEndOfLine("handleCompareToNumericalTangentOperator",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleCompareToNumericalTangentOperator",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleCompareToNumericalTangentOperator

  void MTestParser::handleTangentOperatorComparisonCriterium(MTest& t,TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("handleTangentOperatorComparisonCriterium",
			    p,this->fileTokens.end());
    t.setTangentOperatorComparisonCriterium(this->readDouble(t,p));
    this->checkNotEndOfLine("handleTangentOperatorComparisonCriterium",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleTangentOperatorComparisonCriterium",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleTangentOperatorComparisonCriterium

  void MTestParser::handleNumericalTangentOperatorPerturbationValue(MTest& t,TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("handleNumericalTangentOperatorPerturbationValue",
			    p,this->fileTokens.end());
    t.setNumericalTangentOperatorPerturbationValue(this->readDouble(t,p));
    this->checkNotEndOfLine("handleNumericalTangentOperatorPerturbationValue",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleNumericalTangentOperatorPerturbationValue",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleNumericalTangentOperatorPerturbationValue

  void MTestParser::handleAuthor(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setAuthor(this->readUntilEndOfInstruction(p));
  } // end of MTestParser::handleAuthor

  void MTestParser::handleOutOfBoundsPolicy(MTest& t,TokensContainer::const_iterator& p)
  {
    const std::string& s = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handlePredictionPolicy",";",
			     p,this->fileTokens.end());
    if(s=="None"){
      t.setOutOfBoundsPolicy(tfel::material::None);
    } else if(s=="Warning"){
      t.setOutOfBoundsPolicy(tfel::material::Warning);
    } else if(s=="Strict"){
      t.setOutOfBoundsPolicy(tfel::material::Strict);
    } else {
      throw(std::runtime_error("MTestParser::handleOutOfBoundsPolicy: "
			       "unsupported policy '"+s+"'"));
    }
  } // end of MTestParser::handleOutOfBoundsPolicy
  
  void MTestParser::handlePredictionPolicy(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    MTest::PredictionPolicy ppolicy;
    this->checkNotEndOfLine("handlePredictionPolicy",
			    p,this->fileTokens.end());
    const auto& s = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handlePredictionPolicy",";",
			     p,this->fileTokens.end());
    if(s=="NoPrediction"){
      ppolicy = MTest::NOPREDICTION;
    } else if(s=="LinearPrediction"){
      ppolicy = MTest::LINEARPREDICTION;
    } else if(s=="ElasticPrediction"){
      ppolicy = MTest::ELASTICPREDICTION;
    } else if(s=="ElasticPredictionFromMaterialProperties"){
      ppolicy = MTest::ELASTICPREDICTIONFROMMATERIALPROPERTIES;
    } else if(s=="SecantOperatorPrediction"){
      ppolicy = MTest::SECANTOPERATORPREDICTION;
    } else if(s=="TangentOperatorPrediction"){
      ppolicy = MTest::TANGENTOPERATORPREDICTION;
    } else {
      string msg("MTestParser::handlePredictionPolicy : "
		 "unsupported prediction policy '"+s+"'");
      throw(runtime_error(msg));
    }
    t.setPredictionPolicy(ppolicy);
  }

  void MTestParser::handleDate(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setDate(this->readUntilEndOfInstruction(p));
  } // end of MTestParser::handleDate

  void
  MTestParser::handleStiffnessMatrixType(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    StiffnessMatrixType::mtype ktype;
    const auto& type = this->readString(p,this->fileTokens.end());
    if(type=="Elastic"){
      ktype=StiffnessMatrixType::ELASTIC;
    } else if(type=="SecantOperator"){
      ktype=StiffnessMatrixType::SECANTOPERATOR;
    } else if(type=="TangentOperator"){
      ktype=StiffnessMatrixType::TANGENTOPERATOR;
    } else if(type=="ConsistentTangentOperator"){
      ktype=StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
    } else {
      string msg("MTestParser::handleStiffnessMatrixType : "
		 "unsupported stiffness matrix type '"+type+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTestParser::handleStiffnessMatrixType",";",
			     p,this->fileTokens.end());
    t.setStiffnessMatrixType(ktype);
  }

  void
  MTestParser::handleHandleThermalExpansion(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    bool b;
    this->checkNotEndOfLine("MTestParser::handleHandleThermalExpansion",
			    p,this->fileTokens.end());
    if(p->value=="true"){
      b = true;
    } else if(p->value=="false"){
      b = false;
    } else {
      string msg("MTestParser::handleHandleThermalExpansion : "
		 "unexpected token '"+p->value+"'");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleHandleThermalExpansion",
			     ";",p,this->fileTokens.end());
    t.setHandleThermalExpansion(b);
  }

  void
  MTestParser::handleUseCastemAccelerationAlgorithm(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    bool useCastemAcceleration;
    this->checkNotEndOfLine("MTestParser::handleUseCastemAccelerationAlgorithm",
			    p,this->fileTokens.end());
    if(p->value=="true"){
      useCastemAcceleration = true;
    } else if(p->value=="false"){
      useCastemAcceleration = false;
    } else {
      string msg("MTestParser::handleUseCastemAccelerationAlgorithm : "
		 "unexpected token '"+p->value+"'");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleUseCastemAccelerationAlgorithm",
			     ";",p,this->fileTokens.end());
    t.setUseCastemAccelerationAlgorithm(useCastemAcceleration);
  }

  void
  MTestParser::handleCastemAccelerationTrigger(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    int cat = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleCastemAccelerationTrigger",";",
			     p,this->fileTokens.end());
    t.setCastemAccelerationTrigger(cat);
  } // end of MTestParser::handleCastemAccelerationTrigger

  void
  MTestParser::handleCastemAccelerationPeriod(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    int cap = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleCastemAccelerationPeriod",";",
			     p,this->fileTokens.end());
    t.setCastemAccelerationPeriod(cap);
  } // end of MTestParser::handleCastemAccelerationPeriod

  void
  MTestParser::handleAccelerationAlgorithm(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    this->checkNotEndOfLine("MTestParser::handleAccelerationAlgorithm",
			    p,this->fileTokens.end());
    const auto& a = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleAccelerationAlgorithm",
			     ";",p,this->fileTokens.end());
    t.setAccelerationAlgorithm(a);
  }

  void
  MTestParser::handleAccelerationAlgorithmParameter(MTest& t,TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("MTestParser::handleAccelerationAlgorithmParameter",
			    p,this->fileTokens.end());
    const auto& pn = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleAccelerationAlgorithmParameter",
			    p,this->fileTokens.end());
    const auto& v = p->value;
    ++p;
    this->readSpecifiedToken("MTestParser::handleAccelerationAlgorithmParameter",";",
			     p,this->fileTokens.end());
    t.setAccelerationAlgorithmParameter(pn,v);
  } // end of MTestParser::handleIronsTuckAccelerationTrigger


  void
  MTestParser::handleStiffnessUpdatePolicy(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    MTest::StiffnessUpdatingPolicy ks;
    const auto& type = this->readString(p,this->fileTokens.end());
    if(type=="ConstantStiffness"){
      ks=MTest::CONSTANTSTIFFNESS;
    } else if(type=="SecantOperator"){
      ks=MTest::CONSTANTSTIFFNESSBYPERIOD;
    } else if(type=="TangentOperator"){
      ks=MTest::UPDATEDSTIFFNESSMATRIX;
    } else {
      string msg("MTestParser::handleStiffnessUpdatePolicy : "
		 "unsupported stiffness matrix policy '"+type+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTestParser::handleStiffnessUpdatePolicy",";",
			     p,this->fileTokens.end());
    t.setStiffnessUpdatingPolicy(ks);
  }

  void
  MTestParser::handleTest(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    this->readSpecifiedToken("MTestParser::handleTest","<",
			     p,this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleTest",p,
			    this->fileTokens.end());
    const auto& type = p->value;
    if((type!="function")&&
       (type!="file")){
      string msg("MTestParser::handleTest : "
		 "invalid test type '"+type+"'");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleTest",">",
			     p,this->fileTokens.end());
    if(type=="function"){
      this->checkNotEndOfLine("MTestParser::handleTest",p,
			      this->fileTokens.end());
      map<string,string> functions; 
      if(p->flag==Token::String){
	const auto& v = this->readString(p,this->fileTokens.end());
	const auto& f = this->readString(p,this->fileTokens.end());
	functions.insert({v,f});
      } else {
	this->readSpecifiedToken("MTestParser::handleTest","{",
				 p,this->fileTokens.end());
	this->checkNotEndOfLine("MTestParser::handleTest",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  const auto& v = this->readString(p,this->fileTokens.end());
	  this->readSpecifiedToken("MTestParser::handleTest",":",
				   p,this->fileTokens.end());
	  const auto& f = this->readString(p,this->fileTokens.end());
	  functions.insert({v,f});
	  this->checkNotEndOfLine("MTestParser::handleTest",p,
				  this->fileTokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("MTestParser::handleTest",",",
				     p,this->fileTokens.end());	
	    this->checkNotEndOfLine("MTestParser::handleTest",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("MTestParser::handleTest : "
			 "unexpected token '}'");
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTestParser::handleTest","}",
				 p,this->fileTokens.end());	
      }
      const real eps = this->readDouble(t,p);
      if(eps<0){
	string msg("MTestParser::handleTest : "
		   "invalid criterium value");
	throw(runtime_error(msg));
      }
      map<string,string>::const_iterator pf;
      for(pf=functions.begin();pf!=functions.end();++pf){
	MTest::UTest::TestedVariable ttype;
	unsigned short pos;
	t.getVariableTypeAndPosition(ttype,pos,pf->first);
	shared_ptr<MTest::UTest> test;
	test = shared_ptr<MTest::UTest>(new AnalyticalTest(pf->second,pf->first,
								ttype,pos,t.getEvolutions(),eps));
	t.addTest(test);
      }
    } else if (type=="file"){
      const auto& f = this->readString(p,this->fileTokens.end());
      this->checkNotEndOfLine("MTestParser::handleTest",p,
			      this->fileTokens.end());
      map<string,unsigned int> columns; 
      if(p->flag==Token::String){
	const auto& v = this->readString(p,this->fileTokens.end());
	const unsigned int c = this->readUnsignedInt(p,this->fileTokens.end());
	columns.insert({v,c});
      } else {
	this->readSpecifiedToken("MTestParser::handleTest","{",
				 p,this->fileTokens.end());
	this->checkNotEndOfLine("MTestParser::handleTest",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  const auto& v = this->readString(p,this->fileTokens.end());
	  this->readSpecifiedToken("MTestParser::handleTest",":",
				   p,this->fileTokens.end());
	  const unsigned int c = this->readUnsignedInt(p,this->fileTokens.end());
	  columns.insert({v,c});
	  this->checkNotEndOfLine("MTestParser::handleTest",p,
				  this->fileTokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("MTestParser::handleTest",",",
				     p,this->fileTokens.end());	
	    this->checkNotEndOfLine("MTestParser::handleTest",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("MTestParser::handleTest : "
			 "unexpected token '}'");
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTestParser::handleTest","}",
				 p,this->fileTokens.end());	
      }
      const real eps = this->readDouble(t,p);
      if(eps<0){
	string msg("MTestParser::handleTest : "
		   "invalid criterium value");
	throw(runtime_error(msg));
      }
      map<string,unsigned int>::const_iterator pf;
      shared_ptr<TextData> data(new TextData(f));
      for(pf=columns.begin();pf!=columns.end();++pf){
	MTest::UTest::TestedVariable ttype;
	unsigned short pos;
	t.getVariableTypeAndPosition(ttype,pos,pf->first);
	shared_ptr<MTest::UTest> test;
	test = shared_ptr<MTest::UTest>(new ReferenceFileComparisonTest(data,pf->first,
									     pf->second,
									     ttype,pos,eps));
	t.addTest(test);
      }
    } else {
      string msg("MTestParser::handleTest : "
		 "invalid test type '"+type+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTestParser::handleTest",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleTest

  void
  MTestParser::handleReal(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    const auto& v = this->readString(p,this->fileTokens.end());
    if(!this->isValidIdentifier(v)){
      string msg("MTestParser::handleReal : '"+
		 v+"' is not a valid identifier");
      throw(runtime_error(msg));
    }
    const real value = this->readDouble(t,p);
    shared_ptr<Evolution> mpev;
    mpev = shared_ptr<Evolution>(new ConstantEvolution(value));
    this->readSpecifiedToken("MTestParser::handleReal",";",
			     p,this->fileTokens.end());
    t.addEvolution(v,mpev,true,true);
  }

  void
  MTestParser::handleMaximumNumberOfIterations(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    t.setMaximumNumberOfIterations(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleMaximumNumberOfIterations",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleMaximumNumberOfIterations

  void
  MTestParser::handleMaximumNumberOfSubSteps(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    t.setMaximumNumberOfSubSteps(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleMaximumNumberOfSubSteps",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleMaximumNumberOfSubSteps

  void
  MTestParser::handleRotationMatrix(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::math;
    using std::vector;
    this->readSpecifiedToken("MTestParser::handleRotationMatrix","{",
			     p,this->fileTokens.end());
    vector<vector<real> > v(3);
    for(unsigned short i=0;i!=3;){
      v[i].resize(3);
      this->readArrayOfSpecifiedSize(v[i],t,p);
      if(++i!=3){
	this->readSpecifiedToken("MTestParser::handleRotationMatrix",",",
				 p,this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("MTestParser::handleRotationMatrix","}",
			     p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleRotationMatrix",";",
			     p,this->fileTokens.end());
    // saving the read values
    tmatrix<3u,3u,real> rm;
    for(unsigned short i=0;i!=3;++i){
      for(unsigned short j=0;j!=3;++j){
	rm(i,j)=v[i][j];
      }
    }
    t.setRotationMatrix(rm);
  } // end of MTestParser::handleRotationMatrix

  void
  MTestParser::handleStrainEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      string msg("MTestParser::handleStrainEpsilon : "
		 "the @StrainEpsilon keyword is only valid "
		 "for small strain behaviours");
      throw(runtime_error(msg));
    }
    this->handleDrivingVariableEpsilon(t,p);
  }

  void
  MTestParser::handleDeformationGradientEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
      string msg("MTestParser::handleDeformationGradientEpsilon : "
		 "the @DeformationGradientEpsilon keyword is only valid "
		 "for finite strain behaviours");
      throw(runtime_error(msg));
    }
    this->handleDrivingVariableEpsilon(t,p);
  } // end of MTestParser::handleDeformationGradientEpsilon

  void
  MTestParser::handleOpeningDisplacementEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      string msg("MTestParser::handleOpeningDisplacementEpsilon : "
		 "the @OpeningDisplacementEpsilon keyword is only valid "
		 "for cohesive zone model behaviours");
      throw(runtime_error(msg));
    }
    this->handleDrivingVariableEpsilon(t,p);
  }

  void
  MTestParser::handleDrivingVariableEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setDrivingVariableEpsilon(this->readDouble(t,p));
    this->readSpecifiedToken("MTestParser::handleDrivingVariableEpsilon",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleDrivingVariableEpsilon

  void
  MTestParser::handleStressEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::material;
    if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
      string msg("MTestParser::handleStressEpsilon : "
		 "the @StressEpsilon keyword is only valid "
		 "for small strain behaviours");
      throw(runtime_error(msg));
    }
    this->handleThermodynamicForceEpsilon(t,p);
  }

  void
  MTestParser::handleCohesiveForceEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::handleCohesiveForceEpsilon : "
			       "the @CohesiveForceEpsilon keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleThermodynamicForceEpsilon(t,p);
  }

  void
  MTestParser::handleThermodynamicForceEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setThermodynamicForceEpsilon(this->readDouble(t,p));
    this->readSpecifiedToken("MTestParser::handleThermodynamicForceEpsilon",";",
			     p,this->fileTokens.end());
  }

  void
  MTestParser::handleParameter(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto n = this->readString(p,this->fileTokens.end());
    const real v = this->readDouble(t,p);
    t.setParameter(n,v);
    this->readSpecifiedToken("MTestParser::handleParameter",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleParameter

  void
  MTestParser::handleIntegerParameter(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    const string&n  = this->readString(p,this->fileTokens.end());
    const int v     = this->readInt(p,this->fileTokens.end());
    t.setIntegerParameter(n,v);
    this->readSpecifiedToken("MTestParser::handleIntegerParameter",";",
  			     p,this->fileTokens.end());
  } // end of MTestParser::handleIntegerParameter
  
  void
  MTestParser::handleUnsignedIntegerParameter(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    const string&n       = this->readString(p,this->fileTokens.end());
    const unsigned int v = this->readUnsignedInt(p,this->fileTokens.end());
    t.setUnsignedIntegerParameter(n,v);
    this->readSpecifiedToken("MTestParser::handleUnsignedIntegerParameter",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleUnsignedIntegerParameteru

  void
  MTestParser::handleOutputFile(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    t.setOutputFileName(this->readString(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleOutputFiles",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleOutputFile

  void
  MTestParser::handleOutputFilePrecision(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setOutputFilePrecision(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleOutputFilePrecisions",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleOutputFilePrecision

  void
  MTestParser::handleResidualFile(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    t.setResidualFileName(this->readString(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleResidualFiles",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleResidualFile

  void
  MTestParser::handleResidualFilePrecision(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setResidualFilePrecision(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleResidualFilePrecisions",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleResidualFilePrecision

  void
  MTestParser::handleModellingHypothesis(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setModellingHypothesis(this->readString(p,this->fileTokens.end()));
    this->readSpecifiedToken("MTestParser::handleModellingHypothesis",";",p,
			     this->fileTokens.end());
  }

  void
  MTestParser::handleTimes(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    vector<real> times;
    this->readSpecifiedToken("MTestParser::handleTimes","{",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleTimes",p,
			    this->fileTokens.end());
    while(p->value!="}"){
      const real t_dt = this->readTime(t,p);
      this->checkNotEndOfLine("MTestParser::handleTimes",p,
			      this->fileTokens.end());
      if(!times.empty()){
	if(p->value=="in"){
	  ++p;
	  unsigned int n = this->readUnsignedInt(p,this->fileTokens.end());
	  if(n==0){
	    throw(runtime_error("MTestParser::handleTimes : "
				"invalid number of intervals"));
	  }
	  const real tt = times.back();
	  real dt = (t_dt-tt)/(static_cast<real>(n));
	  for(unsigned int i=1;i!=n;++i){
	    times.push_back(tt+i*dt);
	  }
	}
	this->checkNotEndOfLine("MTestParser::handleTimes",p,
				this->fileTokens.end());
      }
      times.push_back(t_dt);
      if(p->value==","){
	++p;
	this->checkNotEndOfLine("MTestParser::handleTimes",p,
				this->fileTokens.end());
	if(p->value=="}"){
	  string msg("MTestParser::handleTimes : ");
	  msg += "unexpected token '}'";
	  throw(runtime_error(msg));
	}
      } else {
	if(p->value!="}"){
	  string msg("MTestParser::handleTimes : ");
	  msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	  throw(runtime_error(msg));
	}
      }
    }
    this->readSpecifiedToken("MTestParser::handleTimes","}",p,
			     this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleTimes",";",p,
			     this->fileTokens.end());
    if(times.empty()){
      string msg("MTestParser::handleTimes : ");
      msg += "no time defined";
      throw(runtime_error(msg));
    }
    if(times.size()==1){
      string msg("MTestParser::handleTimes : ");
      msg += "at least two different times must be defined";
      throw(runtime_error(msg));
    }
    vector<real>::const_iterator pt  = times.begin();
    real mt(0);
    while(pt!=times.end()){
      mt = max(mt,abs(*pt));
      ++pt;
    }
    if(mt<100*numeric_limits<real>::min()){
      string msg("MTestParser::handleTimes : maximal "
		 "absolute value of times is too low");
      throw(runtime_error(msg));
    }
    const real eps = 100*mt*numeric_limits<real>::epsilon();
    pt  = times.begin();
    vector<real>::const_iterator pt2 = pt+1u;
    while(pt2!=times.end()){
      if((*pt2<=*pt)||abs(*pt2-*pt)<eps){
	ostringstream msg;
	msg << "MTestParser::handleTimes : times '" << *pt2 
	    << "' is lesser than or too close to  time '"
	    << *pt << "'";
	throw(runtime_error(msg.str()));
      }
      ++pt2;
      ++pt;
    }
    t.setTimes(times);
  }
  
  void
  MTestParser::handleImposedStress(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::utilities;
    using namespace tfel::material;
    if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw(std::runtime_error("MTestParser::handleImposedStress : "
			       "the @ImposedStress keyword is only valid "
			       "for standard behaviours"));
    }
    this->handleImposedThermodynamicForce(t,p);
  } // end of MTestParser::handleImposedStress

  void
  MTestParser::handleImposedCohesiveForce(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::utilities;
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::handleImposedCohesiveForce : "
			       "the @ImposedCohesiveForce keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleImposedThermodynamicForce(t,p);
  } // end of MTestParser::handleImposedCohesiveForce

  void
  MTestParser::handleImposedThermodynamicForce(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& c = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleImposedThermodynamicForce",p,
			    this->fileTokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    auto sc  = std::make_shared<ImposedThermodynamicForce>(*(t.getBehaviour()),
							   t.getModellingHypothesis(),
							   c,sev);
    t.addEvolution(c,sev,false,true);
    t.addConstraint(sc);
    this->readSpecifiedToken("MTestParser::handleImposedThermodynamicForce",";",
			     p,this->fileTokens.end());
  } // end of MTestParser::handleImposedStress

  void
  MTestParser::handleImposedStrain(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleImposedStrain : "
			       "the @ImposedStrain keyword is only valid "
			       "for small strain behaviours"));
    }
    this->handleImposedDrivingVariable(t,p);
  }

  void
  MTestParser::handleImposedDeformationGradient(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleImposedDeformationGradient : "
			       "the @ImposedDeformationGradient keyword is only valid "
			       "for finite strain behaviours"));
    }
    this->handleImposedDrivingVariable(t,p);
  }

  void
  MTestParser::handleImposedOpeningDisplacement(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::ImposedOpeningDisplacement : "
			       "the @ImposedOpeningDisplacement keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleImposedDrivingVariable(t,p);
  }

  void
  MTestParser::handleImposedDrivingVariable(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& c = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleImposedDrivingVariable",p,
			    this->fileTokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    auto sc  = std::make_shared<ImposedDrivingVariable>(*(t.getBehaviour()),
							t.getModellingHypothesis(),
							c,sev);
    this->readSpecifiedToken("MTestParser::handleImposedDrivingVariable",";",
			     p,this->fileTokens.end());
    t.addEvolution(c,sev,false,true);
    t.addConstraint(sc);
  } // end of MTestParser::handleImposedDrivingVariable

  void
  MTestParser::handleBehaviour(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::system;
    string i;
    this->readSpecifiedToken("MTestParser::handleBehaviour","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleBehaviour",p,
			    this->fileTokens.end());
#ifdef HAVE_CASTEM
    if((p->value=="umat")||
       (p->value=="castem")){
      i = "castem";
    }
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
    if(p->value=="aster"){
      i = p->value;
    }
#endif /* HAVE_ASTER */
#ifdef HAVE_CYRANO
    if(p->value=="cyrano"){
      i = p->value;
    }
#endif /* HAVE_CYRANO */
    if(i.empty()){
      string msg("MTestParser::handleBehaviour : ");
      msg += "unknown interface '"+p->value+"'";
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleBehaviour",">",p,
			     this->fileTokens.end());
    const auto& l = this->readString(p,this->fileTokens.end());
    const auto& f = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTestParser::handleBehaviour",";",p,
			     this->fileTokens.end());
    t.setBehaviour(i,l,f);
  } // end of MTestParser::handleBehaviour

  void
  MTestParser::handleMaterialProperty(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    string i;
    this->readSpecifiedToken("MTestParser::handleMaterialProperty","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::handleMaterialProperty",p,
			    this->fileTokens.end());
    if((p->value=="constant")||
       (p->value=="castem")||
       (p->value=="function")){
      i = p->value;
    } else {
      string msg("MTestParser::handleMaterialProperty : ");
      msg += "unknown interface '"+p->value+"'";
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleMaterialProperty",">",p,
			     this->fileTokens.end());
    const auto& n = this->readString(p,this->fileTokens.end());
    if(i=="constant"){
      shared_ptr<Evolution> mpev;
      this->checkNotEndOfLine("MTestParser::handleMaterialProperty",p,
			      this->fileTokens.end());
      const real v = this->readDouble(t,p);
      mpev = shared_ptr<Evolution>(new ConstantEvolution(v));
      t.setMaterialProperty(n,mpev,true);
    } else if(i=="function"){
      shared_ptr<Evolution> mpev;
      const string f = this->readString(p,this->fileTokens.end());
      mpev = shared_ptr<Evolution>(new FunctionEvolution(f,t.getEvolutions()));
      t.setMaterialProperty(n,mpev,true);
    } else if(i=="castem"){
      shared_ptr<Evolution> mpev;
      const string l = this->readString(p,this->fileTokens.end());
      const string f = this->readString(p,this->fileTokens.end());
      mpev = shared_ptr<Evolution>(new CastemEvolution(l,f,t.getEvolutions()));
      t.setMaterialProperty(n,mpev,true);
    } else {
      string msg("MTestParser::handleMaterialProperty : ");
      msg += "unknown interface '"+i+"'";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTestParser::handleMaterialProperty",";",p,
			     this->fileTokens.end());
  }
    
  void
  MTestParser::handleStrain(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviour()->getBehaviourType()!=
       MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleStrain : "
			       "the @Strain keyword is only valid "
			       "for small strain behaviours"));
    }
    this->handleDrivingVariable(t,p);
  }

  void
  MTestParser::handleDeformationGradient(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleDeformationGradient : "
			       "the @DeformationGradient keyword is only valid "
			       "for finite strain behaviours"));
    }
    this->handleDrivingVariable(t,p);
  }

  void
  MTestParser::handleOpeningDisplacement(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::handleOpeningDisplacement : "
			       "the @OpeningDisplacement keyword is only valid "
			       "for cohesive zone models behaviours"));
    }
    this->handleDrivingVariable(t,p);
  }
      
  void
  MTestParser::handleDrivingVariable(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    const unsigned short N = t.getBehaviour()->getDrivingVariablesSize(t.getModellingHypothesis());
    vector<real> e_t0;
    e_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(e_t0,t,p);
    this->readSpecifiedToken("MTestParser::handleDrivingVariable",
			     ";",p,this->fileTokens.end());
    t.setDrivingVariablesInitialValues(e_t0);
  } // end of MTestParser::handleDrivingVariable

  void
  MTestParser::handleStress(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if((t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw(std::runtime_error("MTestParser::handleStress : "
			       "the @Stress keyword is only valid "
			       "for small strain behaviours"));
    }
    this->handleThermodynamicForce(t,p);
  }

  void
  MTestParser::handleCohesiveForce(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::handleCohesiveForce : "
			       "the @CohesiveForce keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleThermodynamicForce(t,p);
  }

  void
  MTestParser::handleThermodynamicForce(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto N = t.getBehaviour()->getThermodynamicForcesSize(t.getModellingHypothesis());
    std::vector<real> s_t0;
    s_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(s_t0,t,p);
    this->readSpecifiedToken("MTestParser::handleThermodynamicForce",
			     ";",p,this->fileTokens.end());
    t.setThermodynamicForcesInitialValues(s_t0);
  } // end of MTestParser::handleThermodynamicForce

  static void
  selectVariables(std::vector<std::string>& r,
		  const std::vector<std::string>& names,
		  const std::string& n)
  {
    using namespace std;
    r.clear();    
    if(find(names.begin(),names.end(),n)!=names.end()){
      r.push_back(n);
    } else {
      // checking for an array of internal state variables
      for(const auto& vn : names){
	if(vn.compare(0,n.length(),n)==0){
	  if(!(vn.size()>=n.length()+3u)){
	    continue;
	  }
	  auto pn  = vn.crbegin();
	  const auto pne = vn.crbegin()+(vn.size()-n.size()-1);
	  if((vn[n.size()]!='[')||(*pn!=']')){
	    continue;
	  }
	  ++pn;
	  bool ok = true;
	  while((pn!=pne)&&(ok)){
	    ok = ::isdigit(*pn)!=0;
	    ++pn;
	  }
	  if(ok){
	    r.push_back(vn);
	  }
	}
      }
    }
  } // end of selectVariables
    
  void
  MTestParser::setInternalStateVariableValue(MTest& t,
					     TokensContainer::const_iterator& p,
					     const std::string& n){
    using namespace std;
    const int type = t.getBehaviour()->getInternalStateVariableType(n);
    if(type==0){
      t.setScalarInternalStateVariableInitialValue(n,this->readDouble(t,p));
    } else if(type==1){
      const unsigned short N = getSTensorSize(t.getDimension());
      vector<real> v(N);
      this->readArrayOfSpecifiedSize(v,t,p);
      t.setStensorInternalStateVariableInitialValues(n,v);
    } else if(type==3){
      const unsigned short N = getTensorSize(t.getDimension());
      vector<real> v(N);
      this->readArrayOfSpecifiedSize(v,t,p);
      t.setTensorInternalStateVariableInitialValues(n,v);
    } else {
      string msg("MTestParser::setInternalStateVariableValue : "
		 "unsupported state variable type for "
		 "internal state variable '"+n+"'");
      throw(runtime_error(msg));
    }
  }

  void
  MTestParser::handleInternalStateVariable(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    shared_ptr<Behaviour> b(t.getBehaviour());
    const string& n = this->readString(p,this->fileTokens.end());
    const vector<string>& ivsnames = b->getInternalStateVariablesNames();
    vector<string> ivs;
    selectVariables(ivs,ivsnames,n);
    if(ivs.empty()){
      string msg("MTestParser::handleInternalStateVariable : ");
      msg += "the behaviour don't declare an internal state variable named '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    if(ivs.size()==1){
      this->setInternalStateVariableValue(t,p,ivs[0]);
    } else {
      const int type = b->getInternalStateVariableType(ivs[0]);
      bool uniform = false;
      if(type==0){
	uniform = p->value!="{";
      } else {
	if(p->value!="{"){
	  string msg("MTestParser::handleInternalStateVariable : ");
	  msg += "unexpected token '"+n+"'";
	  throw(runtime_error(msg));
	}
	++p;
	this->checkNotEndOfLine("MTestParser::handleInternalStateVariable",p,
				this->fileTokens.end());
	uniform = p->value!="{";
	--p;
      }
      if(uniform){
	vector<string>::const_iterator pn;
	const TokensContainer::const_iterator p2 = p;
	for(pn=ivs.begin();pn!=ivs.end();++pn){
	  p=p2;
	  this->setInternalStateVariableValue(t,p,*pn);
	}
      } else {
	this->readSpecifiedToken("MTestParser::handleInternalStateVariable",
				 "{",p,this->fileTokens.end());
	vector<string>::const_iterator pn;
	for(pn=ivs.begin();pn!=ivs.end();){
	  this->setInternalStateVariableValue(t,p,*pn);
	  if(++pn!=ivs.end()){
	    this->readSpecifiedToken("MTestParser::handleInternalStateVariable",
				     ",",p,this->fileTokens.end());
	  }
	}
	this->readSpecifiedToken("MTestParser::handleInternalStateVariable",
				 "}",p,this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("MTestParser::handleInternalStateVariable",
			     ";",p,this->fileTokens.end());
  }

  void
  MTestParser::handleExternalStateVariable(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& n = this->readString(p,this->fileTokens.end());
    t.setExternalStateVariable(n,this->parseEvolution(t,evt,p),true);
    this->readSpecifiedToken("MTestParser::handleExternalStateVariable",";",p,
			     this->fileTokens.end());
  }

  void
  MTestParser::handleEvolution(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& n = this->readString(p,this->fileTokens.end());
    t.addEvolution(n,this->parseEvolution(t,evt,p),true,true);
    this->readSpecifiedToken("MTestParser::handleEvolution",";",p,
			     this->fileTokens.end());
  }

  real
  MTestParser::readDouble(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using tfel::utilities::Token;
    using tfel::math::Evaluator;
    this->checkNotEndOfLine("MTestParser::readDouble",p,
			    this->fileTokens.end());
    real v(0);
    if(p->flag==Token::String){
      const string&f = this->readString(p,this->fileTokens.end());
      Evaluator ev(f);
      const auto& vn = ev.getVariablesNames();
      vector<string>::const_iterator pv;
      for(pv=vn.begin();pv!=vn.end();++pv){
	map<string,shared_ptr<Evolution> >::const_iterator pev;
	shared_ptr<map<string,shared_ptr<Evolution> > > evs;
	evs = t.getEvolutions();
	pev = evs->find(*pv);
	if(pev==evs->end()){
	  string msg("MTestParser::readDouble : "
		     "no evolution named '"+*pv+"' defined");
	  throw(runtime_error(msg));
	}
	const Evolution& e = *(pev->second);
	if(!e.isConstant()){
	  string msg("MTestParser::readDouble : formula '"+f+"' shall "
		     "only depend on constant evolutions "
		     "(evolution '"+*pv+"' is not constant)");
	  throw(runtime_error(msg));
	}
	ev.setVariableValue(*pv,e(0));
      }
      v = ev.getValue();
    } else {
      v = CxxTokenizer::readDouble(p,this->fileTokens.end());
    }
    return v;
  } // end of MTestParser::readDouble

  real
  MTestParser::readTime(MTest& t,
			TokensContainer::const_iterator& p)
  {
    return this->readDouble(t,p);
  } // end of MTestParser::readTime

  void
  MTestParser::readArrayOfSpecifiedSize(std::vector<real>& v,
					MTest& t,
					TokensContainer::const_iterator& p)
  {
    this->readSpecifiedToken("MTestParser::readArrayOfSpecifiedSize","{",p,
			     this->fileTokens.end());
    std::vector<real>::size_type i = 0;
    while(i!=v.size()){
      v[i] = this->readDouble(t,p);
      if(++i!=v.size()){
	this->readSpecifiedToken("MTestParser::readArrayOfSpecifiedSize",",",p,
				 this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("MTestParser::readArrayOfSpecifiedSize","}",p,
			     this->fileTokens.end());
  } // end of MTestParser::readArrayOfSpecifiedSize
  
  std::shared_ptr<Evolution>
  MTestParser::parseEvolution(MTest& t,
			      const std::string& type,
			      TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    shared_ptr<Evolution> ev;
    this->checkNotEndOfLine("MTestParser::parseEvolution",p,
			    this->fileTokens.end());
    if(type.empty()||type=="evolution"){
      if(p->value=="{"){
	vector<real> tvalues;
	vector<real> values;
	this->readSpecifiedToken("MTestParser::parseEvolution","{",p,
				 this->fileTokens.end());
	this->checkNotEndOfLine("MTestParser::parseEvolution",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  tvalues.push_back(this->readTime(t,p));
	  this->readSpecifiedToken("MTestParser::parseEvolution",":",p,
				   this->fileTokens.end());
	  values.push_back(this->readDouble(t,p));
	  this->checkNotEndOfLine("MTestParser::parseEvolution",p,
				  this->fileTokens.end());
	  if(p->value==","){
	    ++p;
	    this->checkNotEndOfLine("MTestParser::parseEvolution",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("MTestParser::parseEvolution : ");
	      msg += "unexpected token '}'";
	      throw(runtime_error(msg));
	    }
	  } else {
	    if(p->value!="}"){
	      string msg("MTestParser::parseEvolution : ");
	      msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTestParser::parseEvolution","}",p,
				 this->fileTokens.end());
	ev = shared_ptr<Evolution>(new LPIEvolution(tvalues,values));
      } else {
	const real s = this->readDouble(t,p);
	ev = shared_ptr<Evolution>(new ConstantEvolution(s));
      }
    } else if(type=="function"){
      const auto& f = this->readString(p,this->fileTokens.end());
      ev = shared_ptr<Evolution>(new FunctionEvolution(f,t.getEvolutions()));
    } else {
      string msg("MTestParser::parseEvolution : ");
      msg += "invalid evolution type '"+type+"'";
      throw(runtime_error(msg));
    }
    return ev;
  } // end of MTestParser::parseEvolution

  std::string
  MTestParser::readEvolutionType(TokensContainer::const_iterator& p)
  {
    using namespace std;
    this->checkNotEndOfLine("MTestParser::readEvolutionType",p,
			    this->fileTokens.end());
    if(p->value!="<"){
      return "";
    }
    this->readSpecifiedToken("MTestParser::parseEvolution","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTestParser::readEvolutionType",p,
			    this->fileTokens.end());
    string evt = p->value;
    ++p;
    this->readSpecifiedToken("MTestParser::parseEvolution",">",p,
			     this->fileTokens.end());
    return evt;
  } // end of MTestParser::readEvolutionType

  MTestParser::~MTestParser()
  {} // end of MTestParser::~MTestParser()

} // end namespace mtest
