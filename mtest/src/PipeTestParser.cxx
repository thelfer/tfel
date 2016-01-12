/*! 
 * \file   mfront/mtest/PipeTestParser.cxx
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

#include<map>
#include<cmath>
#include<string>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<iterator>
#include<iostream>
#include<algorithm>
#include<stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include"TFEL/Utilities/TerminalColors.hxx"
#include"MFront/MFrontLogStream.hxx"

#include"MTest/AnalyticalTest.hxx"
#include"MTest/ReferenceFileComparisonTest.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/Constraint.hxx"
#include"MTest/ImposedThermodynamicForce.hxx"
#include"MTest/ImposedDrivingVariable.hxx"
#include"MTest/PipeTest.hxx"
#include"MTest/PipeTestParser.hxx"

namespace mtest
{

  PipeTestParser::PipeTestParser()
  {
    this->registerCallBacks();
  }

  void
  PipeTestParser::parseString(PipeTest& t,const std::string& f)
  {
    this->file = "user defined string";
    this->treatCharAsString(true);
    CxxTokenizer::parseString(f);
    this->stripComments();
    this->execute(t);
  } // end of PipeTestParser::parseString

  void
  PipeTestParser::execute(PipeTest& t,const std::string& f)
  {
    this->file = f;
    this->treatCharAsString(true);
    this->openFile(f);
    this->stripComments();
    this->execute(t);
  }
  
  void
  PipeTestParser::execute(PipeTest& t)
  {
    auto p = this->fileTokens.cbegin();
    while(p!=fileTokens.end()){
      if(PipeTestParser::treatKeyword(t,p)){
	continue;
      }
      if(SingleStructureSchemeParser::treatKeyword(t,p)){
	continue;
      }
      if(SchemeParserBase::treatKeyword(t,p)){
	continue;
      }
      std::ostringstream msg;
      msg << "PipeTestParser::execute : invalid keyword '"
	  << p->value << "'. Error at line " 
	  << p->line<< ".";
      throw(std::runtime_error(msg.str()));
    }
  } // end of PipeTestParser::execute

  bool
  PipeTestParser::treatKeyword(PipeTest& t, TokensContainer::const_iterator& p)
  {
    auto pc = this->callbacks.find(p->value);
    if(pc==this->callbacks.end()){
      return false;
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG){
      auto& log = mfront::getLogStream();
      log << "PipeTestParser::execute : treating keyword '" << p->value
	  << "' at line '" << p->line << "'\n";
    }
    ++p;
    auto line = p->line;
    try{
      (this->*(pc->second))(t,p);
    } catch(std::exception& e){
      std::ostringstream msg;
      msg << "PipeTestParser::treatKeyword: error while "
	  << "parsing file '" << this->file << "' at line '"
	  << line << "'.\n" << e.what();
      throw(std::runtime_error(msg.str()));
    }
    return true;
  } // end of PipeTestParser::treatKeyword
    
  void
  PipeTestParser::displayKeyWordsList(void) const
  {
    using namespace std;
    using namespace tfel::utilities;
    auto keys  = SchemeParserBase::getKeyWordsList();
    auto keys2 = SingleStructureSchemeParser::getKeyWordsList();
    keys.insert(keys.end(),keys2.begin(),keys2.end());
    for(const auto& c : this->callbacks){
      keys.push_back(c.first);
    }
    string::size_type msize = 0;
    for(const auto& k : keys){
      msize = max(msize,k.size());
    }
    for(const auto& k : keys){
      const auto f = SchemeParserBase::getDocumentationFilePath("ptest",k);
      auto key = k;
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
  } // end of PipeTestParser::displayKeywordsList

  void
  PipeTestParser::displayKeyWordsHelp(void) const
  {
    auto keys  = SchemeParserBase::getKeyWordsList();
    auto keys2 = SingleStructureSchemeParser::getKeyWordsList();
    keys.insert(keys.end(),keys2.begin(),keys2.end());
    for(const auto& c : this->callbacks){
      keys.push_back(c.first);
    }
    std::cout << "% `PipeTest` keywords\n\n";
    for(const auto& k : keys){
      const auto f = SchemeParserBase::getDocumentationFilePath("ptest",k);
      std::cout << "\n# The `" <<  k << "` keyword\n\n";
      if(!f.empty()){
	std::ifstream desc{f};
	if(!desc){
	  std::cout << desc.rdbuf();
	} else {
	  std::cout << "The keyword `" << k << "` is not documented yet\n";
	}
      } else {
	std::cout << "The keyword `" << k << "` is not documented yet\n";
      }
    }
  } // end of PipeTestParser::displayKeywordsHelp

  void
  PipeTestParser::displayKeyWordDescription(const std::string& k) const
  {
    auto keys  = SchemeParserBase::getKeyWordsList();
    auto keys2 = SingleStructureSchemeParser::getKeyWordsList();
    keys.insert(keys.end(),keys2.begin(),keys2.end());
    for(const auto& c : this->callbacks){
      keys.push_back(c.first);
    }
    if(std::find(keys.begin(),keys.end(),k)==keys.end()){
      throw(std::runtime_error("PipeTestParser::displayKeyWordDescription: "
			       "unknown keyword '"+k+"'"));
    }
    const auto f = SchemeParserBase::getDocumentationFilePath("ptest",k);
    if(f.empty()){
      std::cout << "no description available for keyword '"
		<< k << "'" << std::endl;
      return;
    }
    std::ifstream desc{f};
    if(!desc){
      std::cout << "no description available for keyword '"
		<< k << "'" << std::endl;
    } else {
      std::cout << desc.rdbuf();
    }
  }
  
  void
  PipeTestParser::registerCallBacks()
  {
    SchemeParserBase::registerCallBacks();
    SingleStructureSchemeParser::registerCallBacks();
    this->registerCallBack("@RadialLoading",
			   &PipeTestParser::handleRadialLoading);
    this->registerCallBack("@AxialLoading",
			   &PipeTestParser::handleAxialLoading);
    this->registerCallBack("@InnerRadius",
			   &PipeTestParser::handleInnerRadius);
    this->registerCallBack("@OuterRadius",
			   &PipeTestParser::handleOuterRadius);
    this->registerCallBack("@NumberOfElements",
			   &PipeTestParser::handleNumberOfElements);
    this->registerCallBack("@ElementType",&PipeTestParser::handleElementType);
    this->registerCallBack("@InnerPressureEvolution",
    			   &PipeTestParser::handleInnerPressureEvolution);
    this->registerCallBack("@OuterPressureEvolution",
    			   &PipeTestParser::handleOuterPressureEvolution);
    this->registerCallBack("@AxialForceEvolution",
    			   &PipeTestParser::handleAxialForceEvolution);
    this->registerCallBack("@AxialGrowthEvolution",
    			   &PipeTestParser::handleAxialGrowthEvolution);
    this->registerCallBack("@OuterRadiusEvolution",
    			   &PipeTestParser::handleOuterRadiusEvolution);
    this->registerCallBack("@FillingPressure",
    			   &PipeTestParser::handleFillingPressure);
    this->registerCallBack("@FillingTemperature",
    			   &PipeTestParser::handleFillingTemperature);
    this->registerCallBack("@PerformSmallStrainAnalysis",
    			   &PipeTestParser::handlePerformSmallStrainAnalysis);
    this->registerCallBack("@DisplacementEpsilon",
    			   &PipeTestParser::handleDisplacementEpsilon);
    this->registerCallBack("@ResidualEpsilon",&PipeTestParser::handleResidualEpsilon);
    this->registerCallBack("@Profile",&PipeTestParser::handleProfile);
    this->registerCallBack("@Test",&PipeTestParser::handleTest);
  }

  void
  PipeTestParser::registerCallBack(const std::string& k,
				   const PipeTestParser::CallBack& p)
  {
    this->callbacks.insert({k,p});
  }

  void
  PipeTestParser::handleRadialLoading(PipeTest& t,
				    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleRadialLoading",
			    p,this->fileTokens.end());
    const auto& h = this->readString(p,this->fileTokens.end());
    if(h=="ImposedPressure"){
      t.setRadialLoading(mtest::PipeTest::IMPOSEDPRESSURE);
    } else if(h=="TightPipe"){
      t.setRadialLoading(mtest::PipeTest::TIGHTPIPE);
    } else if(h=="ImposedOuterRadius"){
      t.setRadialLoading(mtest::PipeTest::IMPOSEDOUTERRADIUS);
    } else {
      throw(std::runtime_error("PipeTestParser::handleRadialLoading: "
			       "invalid loading type ('"+h+"').\n"
			       "Valid modelling hypothesis are "
			       "'ImposedPressure' and 'ImposedOuterRadius'"));
    }
    this->checkNotEndOfLine("PipeTestParser::handleRadialLoading",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleRadialLoading",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleRadialLoading
  
  void
  PipeTestParser::handleAxialLoading(PipeTest& t,
				     TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleAxialLoading",
			    p,this->fileTokens.end());
    const auto& h = this->readString(p,this->fileTokens.end());
    if(h=="None"){
      t.setAxialLoading(mtest::PipeTest::NONE);
    } else if(h=="EndCapEffect"){
      t.setAxialLoading(mtest::PipeTest::ENDCAPEFFECT);
    } else if(h=="ImposedAxialForce"){
      t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALFORCE);
    } else if(h=="ImposedAxialGrowth"){
      t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALGROWTH);
    } else {
      throw(std::runtime_error("PipeTestParser::handleAxialLoading: "
			       "invalid pipe modelling hypothesis ('"+h+"').\n"
			       "Valid modelling hypothesis are "
			       "'None', 'ImposedAxialGrowth', "
			       "'ImposedAxialForce' and 'EndCapEffect'"));
    }
    this->checkNotEndOfLine("PipeTestParser::handleAxialLoading",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleAxialLoading",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleAxialLoading

  void
  PipeTestParser::handleInnerRadius(PipeTest& t,
				    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleInnerRadius",
			    p,this->fileTokens.end());
    t.setInnerRadius(this->readDouble(t,p));
    this->checkNotEndOfLine("PipeTestParser::handleInnerRadius",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleInnerRadius",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleInnerRadius

  void
  PipeTestParser::handleOuterRadius(PipeTest& t,
				    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadius",
			    p,this->fileTokens.end());
    t.setOuterRadius(this->readDouble(t,p));
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadius",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleOuterRadius",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleOuterRadius
  
  void
  PipeTestParser::handleNumberOfElements(PipeTest& t,
					 TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleNumberOfElements",
			    p,this->fileTokens.end());
    t.setNumberOfElements(this->readInt(p,this->fileTokens.end()));
    this->checkNotEndOfLine("PipeTestParser::handleNumberOfElements",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleNumberOfElements",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleNumberOfElements

  void
  PipeTestParser::handleElementType(PipeTest& t,
				    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleElementType",
			    p,this->fileTokens.end());
    const auto& e = this->readString(p,this->fileTokens.end());
    if(e=="Linear"){
      t.setElementType(PipeMesh::LINEAR);
    } else if(e=="Quadratic"){
      t.setElementType(PipeMesh::QUADRATIC);
    } else if(e=="Cubic"){
      t.setElementType(PipeMesh::CUBIC);
    } else {
      throw(std::runtime_error("PipeTest::setElementType: "
			       "invalid element type ('"+e+"').\n"
			       "Valid element type are "
			       "'Linear' and 'Quadratic'"));
    }
    this->checkNotEndOfLine("PipeTestParser::handleElementType",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleElementType",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleElementType

  void
  PipeTestParser::handlePerformSmallStrainAnalysis(PipeTest& t,
						   TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handlePerformSmallStrainAnalysis",
			    p,this->fileTokens.end());
    if(p->value=="true"){
      t.performSmallStrainAnalysis();
    } else if(p->value!="false"){
      throw(std::runtime_error("PipeTestParser::handlePerformSmallStrainAnalysis: "
			       "unexpected token '"+p->value+"'"));
    }
    ++p;
    this->checkNotEndOfLine("PipeTestParser::handlePerformSmallStrainAnalysis",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handlePerformSmallStrainAnalysis",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handlePerformSmallStrainAnalysis

  void
  PipeTestParser::handleInnerPressureEvolution(PipeTest& t,
					       TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleInnerPressureEvolution",p,
			    this->fileTokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    t.setInnerPressureEvolution(sev);
    this->checkNotEndOfLine("PipeTestParser::handleInnerPressureEvolution",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleInnerPressureEvolution",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleOuterPressureEvolution(PipeTest& t,
					       TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleOuterPressureEvolution",p,
			    this->fileTokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    t.setOuterPressureEvolution(sev);
    this->checkNotEndOfLine("PipeTestParser::handleOuterPressureEvolution",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleOuterPressureEvolution",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleOuterRadiusEvolution(PipeTest& t,
					       TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadiusEvolution",p,
			    this->fileTokens.end());
    auto rev = this->parseEvolution(t,evt,p);
    t.setOuterRadiusEvolution(rev);
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadiusEvolution",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleOuterRadiusEvolution",";",
			     p,this->fileTokens.end());
  }
  
  void
  PipeTestParser::handleAxialForceEvolution(PipeTest& t,
					       TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleAxialForceEvolution",p,
			    this->fileTokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    t.setAxialForceEvolution(sev);
    this->checkNotEndOfLine("PipeTestParser::handleAxialForceEvolution",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleAxialForceEvolution",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleAxialGrowthEvolution(PipeTest& t,
					     TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleAxialGrowthEvolution",p,
			    this->fileTokens.end());
    auto sev = this->parseEvolution(t,evt,p);
    t.setAxialGrowthEvolution(sev);
    this->checkNotEndOfLine("PipeTestParser::handleAxialGrowthEvolution",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleAxialGrowthEvolution",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleFillingPressure(PipeTest& t,
					TokensContainer::const_iterator& p)
  {
    t.setFillingPressure(this->readDouble(t,p));
    this->checkNotEndOfLine("PipeTestParser::handleFillingPressure",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleFillingPressure",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleFillingPressure

  void
  PipeTestParser::handleFillingTemperature(PipeTest& t,
					   TokensContainer::const_iterator& p)
  {
    t.setFillingTemperature(this->readDouble(t,p));
    this->checkNotEndOfLine("PipeTestParser::handleFillingTemperature",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleFillingTemperature",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleFillingTemperature
  
  void
  PipeTestParser::handleDisplacementEpsilon(PipeTest& t,
					    TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleDisplacementEpsilon",p,
			    this->fileTokens.end());
    t.setDisplacementEpsilon(this->readDouble(t,p));
    this->checkNotEndOfLine("PipeTestParser::handleDisplacementEpsilon",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleDisplacementEpsilon",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleResidualEpsilon(PipeTest& t,
					TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleResidualEpsilon",p,
			    this->fileTokens.end());
    t.setResidualEpsilon(this->readDouble(t,p));
    this->checkNotEndOfLine("PipeTestParser::handleResidualEpsilon",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleResidualEpsilon",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleProfile(PipeTest& t,
				TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("PipeTestParser::handleProfile",p,
			    this->fileTokens.end());
    const auto f = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("PipeTestParser::handleProfile",p,
			    this->fileTokens.end());
    auto v = std::vector<std::string>{};
    if(p->flag==tfel::utilities::Token::String){
      v.push_back(this->readString(p,this->fileTokens.end()));
    } else {
      this->readSpecifiedToken("PipeTestParser::handleProfile","{",
			       p,this->fileTokens.end());
      this->checkNotEndOfLine("PipeTestParser::handleProfile",p,
			      this->fileTokens.end());
      while(p->value!="}"){
	v.push_back(this->readString(p,this->fileTokens.end()));
	this->checkNotEndOfLine("PipeTestParser::handleProfile",p,
				this->fileTokens.end());
	if(p->value!="}"){
	  this->readSpecifiedToken("PipeTestParser::handleProfile",",",
				   p,this->fileTokens.end());	
	  this->checkNotEndOfLine("PipeTestParser::handleProfile",p,
				  this->fileTokens.end());
	  if(p->value=="}"){
	    throw(std::runtime_error("PipeTestParser::handleProfile: "
				     "unexpected token '}'"));
	  }
	}
      }
      this->readSpecifiedToken("PipeTestParser::handleProfile","}",
			       p,this->fileTokens.end());	
    }
    t.addProfile(f,v);
    this->checkNotEndOfLine("PipeTestParser::handleProfile",
			    p,this->fileTokens.end());
    this->readSpecifiedToken("PipeTestParser::handleProfile",";",
			     p,this->fileTokens.end());
  }

  void
  PipeTestParser::handleTest(PipeTest& t,TokensContainer::const_iterator& p)
  {
    this->readSpecifiedToken("PipeTestParser::handleTest","<",
			     p,this->fileTokens.end());
    this->checkNotEndOfLine("PipeTestParser::handleTest",p,
			    this->fileTokens.end());
    const auto& type = p->value;
    bool profile = false;
    if(type!="file"){
      throw(std::runtime_error("PipeTestParser::handleTest: "
			       "invalid test type '"+type+"'"));
    }
    ++p;
    if(p->value==","){
      ++p;
      this->checkNotEndOfLine("PipeTestParser::handleTest",p,
			      this->fileTokens.end());
      if(p->value=="integral"){
	profile = false;
      } else if(p->value=="profile"){
	profile = true;
      } else {
	throw(std::runtime_error("PipeTestParser::handleTest: "
				 "invalid test option '"+p->value+"'"));
      }
      ++p;
      this->checkNotEndOfLine("PipeTestParser::handleTest",p,
			      this->fileTokens.end());
    }
    this->readSpecifiedToken("PipeTestParser::handleTest",">",
			     p,this->fileTokens.end());
    if (type=="file"){
      const auto& f = this->readString(p,this->fileTokens.end());
      this->checkNotEndOfLine("PipeTestParser::handleTest",p,
			      this->fileTokens.end());
      auto variables = std::map<std::string,unsigned int>{}; 
      if(p->flag==tfel::utilities::Token::String){
	const auto& v = this->readString(p,this->fileTokens.end());
	const unsigned int c = this->readUnsignedInt(p,this->fileTokens.end());
	variables.insert({v,c});
      } else {
	this->readSpecifiedToken("PipeTestParser::handleTest","{",
				 p,this->fileTokens.end());
	this->checkNotEndOfLine("PipeTestParser::handleTest",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  const auto& v = this->readString(p,this->fileTokens.end());
	  this->readSpecifiedToken("PipeTestParser::handleTest",":",
				   p,this->fileTokens.end());
	  const auto c = this->readUnsignedInt(p,this->fileTokens.end());
	  variables.insert({v,c});
	  this->checkNotEndOfLine("PipeTestParser::handleTest",p,
				  this->fileTokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("PipeTestParser::handleTest",",",
				     p,this->fileTokens.end());	
	    this->checkNotEndOfLine("PipeTestParser::handleTest",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      throw(std::runtime_error("PipeTestParser::handleTest: "
				       "unexpected token '}'"));
	    }
	  }
	}
	this->readSpecifiedToken("PipeTestParser::handleTest","}",
				 p,this->fileTokens.end());	
      }
      const real eps = this->readDouble(t,p);
      if(eps<0){
	throw(std::runtime_error("PipeTestParser::handleTest: "
				 "invalid criterion value"));
      }
      const tfel::utilities::TextData data(f);
      if(profile){
	for(const auto& v : variables){
	  t.addProfileTest(v.first,data,
			   v.second,eps);
	}
      } else {
	for(const auto& v : variables){
	  t.addIntegralTest(v.first,data,
			    v.second,eps);
	}
      }
    } else {
      throw(std::runtime_error("PipeTestParser::handleTest: "
			       "invalid test type '"+type+"'"));
    }
    this->readSpecifiedToken("PipeTestParser::handleTest",";",
			     p,this->fileTokens.end());
  } // end of PipeTestParser::handleTest
  
  PipeTestParser::~PipeTestParser()
  {} // end of PipeTestParser::~PipeTestParser()

} // end namespace mtest
