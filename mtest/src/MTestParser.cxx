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

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/GenTypeBase.hxx"
#include"MFront/MFrontLogStream.hxx"

#include"MTest/AnalyticalTest.hxx"
#include"MTest/ReferenceFileComparisonTest.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/Constraint.hxx"
#include"MTest/ImposedThermodynamicForce.hxx"
#include"MTest/ImposedDrivingVariable.hxx"
#include"MTest/MTestParser.hxx"

namespace mtest
{

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
  MTestParser::execute(MTest& t,const std::string& f,
		       const std::vector<std::string>& ecmds,
		       const std::map<std::string,std::string>& s)
  {
    this->file = f;
    this->treatCharAsString(true);
    this->openFile(f);
    this->stripComments();
    // substitutions
    const auto pe = s.end();
    for(auto& token: this->tokens){
      auto p = s.find(token.value);
      if(p!=pe){
	token.value = p->second;
	if(((p->second.front()=='\'')&&(p->second.back()=='\''))||
	   ((p->second.front()=='"') &&(p->second.back()=='"'))){
	  token.flag=tfel::utilities::Token::String;
	}
      }
    }
    // treating external commands
    for(const auto& c : ecmds){
      CxxTokenizer tokenizer;
      tokenizer.treatCharAsString(true);
      try{
	tokenizer.parseString(c);
      } catch(std::exception& e){
	throw(std::runtime_error("MTestParser::execute : "
				 "error while parsing external command "
				 "'"+c+"'\n"+std::string(e.what())));
      }
      this->tokens.insert(this->tokens.begin(),tokenizer.begin(),tokenizer.end());
    }
    this->execute(t);
  }
  
  void
  MTestParser::execute(MTest& t)
  {
    auto p = this->tokens.cbegin();
    while(p!=tokens.end()){
      if(MTestParser::treatKeyword(t,p)){
	continue;
      }
      if(SingleStructureSchemeParser::treatKeyword(t,p)){
	continue;
      }
      if(SchemeParserBase::treatKeyword(t,p)){
	continue;
      }
      std::ostringstream msg;
      msg << "MTestParser::execute : invalid keyword '"
	  << p->value << "'. Error at line " 
	  << p->line<< ".";
      throw(std::runtime_error(msg.str()));
    }
  } // end of MTestParser::execute

  bool
  MTestParser::treatKeyword(MTest& t, TokensContainer::const_iterator& p)
  {
    const auto pc = this->callbacks.find(p->value);
    if(pc==this->callbacks.end()){
      return false;
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG){
      auto& log = mfront::getLogStream();
      log << "MTestParser::execute : treating keyword '" << p->value
	  << "' at line '" << p->line << "'\n";
    }
    ++p;
    const auto line = p->line;
    try{
      (this->*(pc->second))(t,p);
    } catch(std::exception& e){
      std::ostringstream msg;
      msg << "MTestParser::MTest : error while "
	  << "parsing file '" << this->file << "' at line '"
	  << line << "'.\n" << e.what();
      throw(std::runtime_error(msg.str()));
    }
    return true;
  } // end of MTestParser::treatKeyword

  std::vector<std::string>
  MTestParser::getKeyWordsList() const{
    auto keys  = SchemeParserBase::getKeyWordsList();
    auto keys2 = SingleStructureSchemeParser::getKeyWordsList();
    keys.insert(keys.end(),keys2.begin(),keys2.end());
    for(const auto& c : this->callbacks){
      keys.push_back(c.first);
    }
    return keys;
  }
  
  void
  MTestParser::displayKeyWordsList(void) const
  {
    using namespace std;
    using namespace tfel::utilities;
    auto keys  = this->getKeyWordsList();
    string::size_type msize = 0;
    for(const auto& k : keys){
      msize = max(msize,k.size());
    }
    for(const auto& k : keys){
      const auto f = SchemeParserBase::getDocumentationFilePath("mtest",k);
      string key = k;
      key.resize(msize,' ');
      cout << key << "  ";
      if(!f.empty()){
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
    auto keys  = this->getKeyWordsList();
    std::cout << "% `MTest` keywords\n\n";
    for(const auto& k : keys){
      const auto f = SchemeParserBase::getDocumentationFilePath("mtest",k);
      std::cout << "\n# The `" <<  k << "` keyword\n\n";
      if(!f.empty()){
	std::ifstream desc{f};
	if(desc){
	  std::cout << desc.rdbuf();
	} else {
	  std::cout << "The keyword `" << k << "` is not documented yet\n";
	}
      } else {
	std::cout << "The keyword `" << k << "` is not documented yet\n";
      }
    }
  } // end of MTestParser::displayKeywordsHelp

  void
  MTestParser::displayKeyWordDescription(const std::string& k) const
  {
    auto keys  = this->getKeyWordsList();
    if(std::find(keys.begin(),keys.end(),k)==keys.end()){
      throw(std::runtime_error("MTestParser::displayKeyWordDescription: "
			       "unknown keyword '"+k+"'"));
    }
    const auto f = SchemeParserBase::getDocumentationFilePath("mtest",k);
    if(f.empty()){
      std::cout << "no description available for keyword '"
		<< k << "'" << std::endl;
      return;
    }
    std::ifstream desc{f};
    if(!desc){
      std::cout << "no description available for keyword '"
		<< k << "'" << std::endl;
      return;
    }
    std::cout << desc.rdbuf();
  }
  
  void
  MTestParser::registerCallBacks()
  {
    SchemeParserBase::registerCallBacks();
    SingleStructureSchemeParser::registerCallBacks();
    this->registerCallBack("@Test",
			   &MTestParser::handleTest);
    this->registerCallBack("@RotationMatrix",
			   &MTestParser::handleRotationMatrix);
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
  }

  void
  MTestParser::registerCallBack(const std::string& k,
				const MTestParser::CallBack& p)
  {
    this->callbacks.insert({k,p});
  }

  void MTestParser::handleCompareToNumericalTangentOperator(MTest& t,
							    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("handleCompareToNumericalTangentOperator",
			    p,this->tokens.end());
    if(p->value=="true"){
      t.setCompareToNumericalTangentOperator(true);
    } else if(p->value=="false"){
      t.setCompareToNumericalTangentOperator(false);
    } else {
      throw(std::runtime_error("MTestParser::handleCompareToNumericalTangentOperator: "
			       "unexpected value (expected 'true' or 'false', "
			       "read '"+p->value+"')"));
    }
    ++p;
    this->checkNotEndOfLine("handleCompareToNumericalTangentOperator",
			    p,this->tokens.end());
    this->readSpecifiedToken("MTestParser::handleCompareToNumericalTangentOperator",";",
			     p,this->tokens.end());
  } // end of MTestParser::handleCompareToNumericalTangentOperator

  void MTestParser::handleTangentOperatorComparisonCriterium(MTest& t,
							     TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("handleTangentOperatorComparisonCriterium",
			    p,this->tokens.end());
    t.setTangentOperatorComparisonCriterium(this->readDouble(t,p));
    this->checkNotEndOfLine("handleTangentOperatorComparisonCriterium",
			    p,this->tokens.end());
    this->readSpecifiedToken("MTestParser::handleTangentOperatorComparisonCriterium",";",
			     p,this->tokens.end());
  } // end of MTestParser::handleTangentOperatorComparisonCriterium

  void MTestParser::handleNumericalTangentOperatorPerturbationValue(MTest& t,
								    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("handleNumericalTangentOperatorPerturbationValue",
			    p,this->tokens.end());
    t.setNumericalTangentOperatorPerturbationValue(this->readDouble(t,p));
    this->checkNotEndOfLine("handleNumericalTangentOperatorPerturbationValue",
			    p,this->tokens.end());
    this->readSpecifiedToken("MTestParser::handleNumericalTangentOperatorPerturbationValue",";",
			     p,this->tokens.end());
  } // end of MTestParser::handleNumericalTangentOperatorPerturbationValue  

  void
  MTestParser::handleTest(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    this->readSpecifiedToken("MTestParser::handleTest","<",
			     p,this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleTest",p,
			    this->tokens.end());
    const auto& type = p->value;
    if((type!="function")&&(type!="file")){
      throw(std::runtime_error("MTestParser::handleTest: "
			       "invalid test type '"+type+"'"));
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleTest",">",
			     p,this->tokens.end());
    if(type=="function"){
      this->checkNotEndOfLine("MTestParser::handleTest",p,
			      this->tokens.end());
      map<string,string> functions; 
      if(p->flag==tfel::utilities::Token::String){
	const auto& v = this->readString(p,this->tokens.end());
	const auto& f = this->readString(p,this->tokens.end());
	functions.insert({v,f});
      } else {
	this->readSpecifiedToken("MTestParser::handleTest","{",
				 p,this->tokens.end());
	this->checkNotEndOfLine("MTestParser::handleTest",p,
				this->tokens.end());
	while(p->value!="}"){
	  const auto& v = this->readString(p,this->tokens.end());
	  this->readSpecifiedToken("MTestParser::handleTest",":",
				   p,this->tokens.end());
	  const auto& f = this->readString(p,this->tokens.end());
	  functions.insert({v,f});
	  this->checkNotEndOfLine("MTestParser::handleTest",p,
				  this->tokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("MTestParser::handleTest",",",
				     p,this->tokens.end());	
	    this->checkNotEndOfLine("MTestParser::handleTest",p,
				    this->tokens.end());
	    if(p->value=="}"){
	      string msg("MTestParser::handleTest: "
			 "unexpected token '}'");
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTestParser::handleTest","}",
				 p,this->tokens.end());	
      }
      const real eps = this->readDouble(t,p);
      if(eps<0){
	throw(std::runtime_error("MTestParser::handleTest: "
				 "invalid criterion value"));
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
      const auto& f = this->readString(p,this->tokens.end());
      this->checkNotEndOfLine("MTestParser::handleTest",p,
			      this->tokens.end());
      using gentype=tfel::utilities::GenTypeBase<tfel::meta::GenerateTypeList<std::string,unsigned short>::type>;
      map<string,gentype> columns; 
      if(p->flag==tfel::utilities::Token::String){
	const auto& v = this->readString(p,this->tokens.end());
	const unsigned short c = this->readUnsignedInt(p,this->tokens.end());
	columns.insert({v,gentype(c)});
      } else {
	this->readSpecifiedToken("MTestParser::handleTest","{",
				 p,this->tokens.end());
	this->checkNotEndOfLine("MTestParser::handleTest",p,
				this->tokens.end());
	while(p->value!="}"){
	  const auto& v = this->readString(p,this->tokens.end());
	  this->readSpecifiedToken("MTestParser::handleTest",":",
				   p,this->tokens.end());
	  if(p->flag==tfel::utilities::Token::String){
	    columns.insert({v,p->value.substr(1,p->value.size()-2)});
	  } else {
	    const unsigned short c = this->readUnsignedInt(p,this->tokens.end());
	    columns.insert({v,gentype(c)});
	  }
	  this->checkNotEndOfLine("MTestParser::handleTest",p,
				  this->tokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("MTestParser::handleTest",",",
				     p,this->tokens.end());	
	    this->checkNotEndOfLine("MTestParser::handleTest",p,
				    this->tokens.end());
	    if(p->value=="}"){
	      string msg("MTestParser::handleTest: "
			 "unexpected token '}'");
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTestParser::handleTest","}",
				 p,this->tokens.end());	
      }
      const real eps = this->readDouble(t,p);
      if(eps<0){
	throw(runtime_error("MTestParser::handleTest: "
			    "invalid criterion value"));
      }
      shared_ptr<tfel::utilities::TextData> data(new tfel::utilities::TextData(f));
      for(const auto& c : columns){
	MTest::UTest::TestedVariable ttype;
	unsigned short pos;
	t.getVariableTypeAndPosition(ttype,pos,c.first);
	std::shared_ptr<MTest::UTest> test;
	if(c.second.is<unsigned short>()){
	  const auto cn = static_cast<unsigned short>(c.second);
	  test = shared_ptr<MTest::UTest>(new ReferenceFileComparisonTest(*data,cn,c.first,
									  ttype,pos,eps));
	} else {
	  const auto ef = static_cast<std::string>(c.second);
	  test = shared_ptr<MTest::UTest>(new ReferenceFileComparisonTest(*data,*(t.getEvolutions()),
									  ef,c.first,ttype,pos,eps));
	}
	t.addTest(test);
      }
    } else {
      throw(runtime_error("MTestParser::handleTest: "
			  "invalid test type '"+type+"'"));
    }
    this->readSpecifiedToken("MTestParser::handleTest",";",
			     p,this->tokens.end());
  } // end of MTestParser::handleTest

  void
  MTestParser::handleRotationMatrix(MTest& t,TokensContainer::const_iterator& p)
  {
    auto from_euler = [](tfel::math::tmatrix<3,3,real>& r,
			 const real psi,const real the,const real phi){
      const real cospsi = std::cos(psi);
      const real costhe = std::cos(the);
      const real cosphi = std::cos(phi);
      const real sinpsi = std::sin(psi);
      const real sinthe = std::sin(the);
      const real sinphi = std::sin(phi);
      r(0,0) =  cosphi*cospsi - sinphi*costhe*sinpsi;
      r(0,1) =  cosphi*sinpsi + sinphi*costhe*cospsi;
      r(0,2) =  sinphi*sinthe;
      r(1,0) = -sinphi*cospsi - cosphi*costhe*sinpsi;
      r(1,1) = -sinphi*sinpsi + cosphi*costhe*cospsi;
      r(1,2) =  cosphi*sinthe;
      r(2,0) =  sinthe*sinpsi;
      r(2,1) = -sinthe*cospsi;
      r(2,2) =  costhe;
    };
    enum {
      STANDARD,
      EULER,
      MILLER
    } choice = STANDARD;
    this->checkNotEndOfLine("MTestParser::handleRotationMatrix",p,
			    this->tokens.end());
    if(p->value=="<"){
      ++p;
      this->checkNotEndOfLine("MTestParser::handleRotationMatrix",p,
			      this->tokens.end());
      if(p->value=="Standard"){
	choice = STANDARD;
      } else if(p->value=="Euler"){
	choice = EULER;
      } else if(p->value=="Miller"){
	choice = MILLER;
      } else {
	throw(std::runtime_error("MTestParser::handleRotationMatrix: "
				 "unsupported roation matrix type"));
      }
      ++p;
      this->checkNotEndOfLine("MTestParser::handleRotationMatrix",p,
			      this->tokens.end());
    }
    // saving the read values
    tfel::math::tmatrix<3u,3u,real> rm;
    if(choice==MILLER){
      constexpr const real cste = 180. / M_PI;
      this->readSpecifiedToken("MTestParser::handleRotationMatrix","{",
			       p,this->tokens.end());
      const auto h = this->readInt(p,this->tokens.end());
      this->readSpecifiedToken("MTestParser::handleRotationMatrix",",",
			       p,this->tokens.end());
      const auto k = this->readInt(p,this->tokens.end());
      this->readSpecifiedToken("MTestParser::handleRotationMatrix",",",
			       p,this->tokens.end());
      const auto l = this->readInt(p,this->tokens.end());
      this->readSpecifiedToken("MTestParser::handleRotationMatrix","}",
			       p,this->tokens.end());
      const real n1   = std::sqrt(static_cast<real>(h*h+k*k+l*l));
      const real n2   = std::sqrt(static_cast<real>(h*h+k*k));
      const real phi1 = 0;
      const real phi  = std::acos(l/n1)*cste;
      const real phi2 = std::atan2(h/n2,k/n2)*cste;
      from_euler(rm,phi1,phi,phi2);
    } else if(choice==EULER){
      std::vector<real> v(3);
      this->readSpecifiedToken("MTestParser::handleRotationMatrix","{",
			       p,this->tokens.end());
      this->readArrayOfSpecifiedSize(v,t,p);
      this->readSpecifiedToken("MTestParser::handleRotationMatrix","}",
			       p,this->tokens.end());
      from_euler(rm,v[0],v[1],v[2]);
    } else {
      // standard choice
      this->readSpecifiedToken("MTestParser::handleRotationMatrix","{",
			       p,this->tokens.end());
      std::vector<std::vector<real>> v(3);
      for(unsigned short i=0;i!=3;){
	v[i].resize(3);
	this->readArrayOfSpecifiedSize(v[i],t,p);
	if(++i!=3){
	  this->readSpecifiedToken("MTestParser::handleRotationMatrix",",",
				   p,this->tokens.end());
	}
      }
      this->readSpecifiedToken("MTestParser::handleRotationMatrix","}",
			       p,this->tokens.end());
      for(unsigned short i=0;i!=3;++i){
	for(unsigned short j=0;j!=3;++j){
	  rm(i,j)=v[i][j];
	}
      }
    }
    this->readSpecifiedToken("MTestParser::handleRotationMatrix",";",
			     p,this->tokens.end());
    t.setRotationMatrix(rm);
  } // end of MTestParser::handleRotationMatrix

  void
  MTestParser::handleStrainEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleStrainEpsilon: "
			       "the @StrainEpsilon keyword is only valid "
			       "for small strain behaviours"));
    }
    this->handleDrivingVariableEpsilon(t,p);
  }

  void
  MTestParser::handleDeformationGradientEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleDeformationGradientEpsilon: "
			       "the @DeformationGradientEpsilon keyword is only valid "
			       "for finite strain behaviours"));
    }
    this->handleDrivingVariableEpsilon(t,p);
  } // end of MTestParser::handleDeformationGradientEpsilon

  void
  MTestParser::handleOpeningDisplacementEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::handleOpeningDisplacementEpsilon: "
			       "the @OpeningDisplacementEpsilon keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleDrivingVariableEpsilon(t,p);
  }

  void
  MTestParser::handleDrivingVariableEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setDrivingVariableEpsilon(this->readDouble(t,p));
    this->readSpecifiedToken("MTestParser::handleDrivingVariableEpsilon",";",
			     p,this->tokens.end());
  } // end of MTestParser::handleDrivingVariableEpsilon

  void
  MTestParser::handleStressEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw(std::runtime_error("MTestParser::handleStressEpsilon: "
			       "the @StressEpsilon keyword is only valid "
			       "for small strain behaviours"));
    }
    this->handleThermodynamicForceEpsilon(t,p);
  }

  void
  MTestParser::handleCohesiveForceEpsilon(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
      throw(std::runtime_error("MTestParser::handleCohesiveForceEpsilon: "
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
			     p,this->tokens.end());
  }

  void
  MTestParser::handleModellingHypothesis(MTest& t,TokensContainer::const_iterator& p)
  {
    t.setModellingHypothesis(this->readString(p,this->tokens.end()));
    this->readSpecifiedToken("MTestParser::handleModellingHypothesis",";",p,
			     this->tokens.end());
  }
  
  void
  MTestParser::handleImposedStress(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::utilities;
    using namespace tfel::material;
    if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw(std::runtime_error("MTestParser::handleImposedStress: "
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
      throw(std::runtime_error("MTestParser::handleImposedCohesiveForce: "
			       "the @ImposedCohesiveForce keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleImposedThermodynamicForce(t,p);
  } // end of MTestParser::handleImposedCohesiveForce

  void
  MTestParser::handleImposedThermodynamicForce(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& c = this->readString(p,this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleImposedThermodynamicForce",p,
			    this->tokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    auto sc  = std::make_shared<ImposedThermodynamicForce>(*(t.getBehaviour()),
							   c,sev);
    t.addEvolution(c,sev,false,true);
    t.addConstraint(sc);
    this->readSpecifiedToken("MTestParser::handleImposedThermodynamicForce",";",
			     p,this->tokens.end());
  } // end of MTestParser::handleImposedStress

  void
  MTestParser::handleImposedStrain(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleImposedStrain: "
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
      throw(std::runtime_error("MTestParser::handleImposedDeformationGradient: "
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
      throw(std::runtime_error("MTestParser::ImposedOpeningDisplacement: "
			       "the @ImposedOpeningDisplacement keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleImposedDrivingVariable(t,p);
  }

  void
  MTestParser::handleImposedDrivingVariable(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& c = this->readString(p,this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleImposedDrivingVariable",p,
			    this->tokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    auto sc  = std::make_shared<ImposedDrivingVariable>(*(t.getBehaviour()),
							c,sev);
    this->readSpecifiedToken("MTestParser::handleImposedDrivingVariable",";",
			     p,this->tokens.end());
    t.addEvolution(c,sev,false,true);
    t.addConstraint(sc);
  } // end of MTestParser::handleImposedDrivingVariable
    
  void
  MTestParser::handleStrain(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if(t.getBehaviour()->getBehaviourType()!=
       MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
      throw(std::runtime_error("MTestParser::handleStrain: "
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
      throw(std::runtime_error("MTestParser::handleDeformationGradient: "
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
      throw(std::runtime_error("MTestParser::handleOpeningDisplacement: "
			       "the @OpeningDisplacement keyword is only valid "
			       "for cohesive zone models behaviours"));
    }
    this->handleDrivingVariable(t,p);
  }
      
  void
  MTestParser::handleDrivingVariable(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto N = t.getBehaviour()->getDrivingVariablesSize();
    std::vector<real> e_t0;
    e_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(e_t0,t,p);
    this->readSpecifiedToken("MTestParser::handleDrivingVariable",
			     ";",p,this->tokens.end());
    t.setDrivingVariablesInitialValues(e_t0);
  } // end of MTestParser::handleDrivingVariable

  void
  MTestParser::handleStress(MTest& t,TokensContainer::const_iterator& p)
  {
    using namespace tfel::material;
    if((t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
       (t.getBehaviour()->getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
      throw(std::runtime_error("MTestParser::handleStress: "
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
      throw(std::runtime_error("MTestParser::handleCohesiveForce: "
			       "the @CohesiveForce keyword is only valid "
			       "for cohesive zone model behaviours"));
    }
    this->handleThermodynamicForce(t,p);
  }

  void
  MTestParser::handleThermodynamicForce(MTest& t,TokensContainer::const_iterator& p)
  {
    const auto N = t.getBehaviour()->getThermodynamicForcesSize();
    std::vector<real> s_t0;
    s_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(s_t0,t,p);
    this->readSpecifiedToken("MTestParser::handleThermodynamicForce",
			     ";",p,this->tokens.end());
    t.setThermodynamicForcesInitialValues(s_t0);
  } // end of MTestParser::handleThermodynamicForce

  MTestParser::~MTestParser()
  {} // end of MTestParser::~MTestParser()

} // end namespace mtest
