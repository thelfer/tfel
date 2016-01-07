/*!
 * \file   SchemeParserBase.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   21 déc. 2015
 */

#include<sstream>

#include"TFEL/Config/GetInstallPath.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MTest/MTest.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/FunctionEvolution.hxx"
#include"MTest/SchemeParserBase.hxx"

namespace mtest{

  std::string
  SchemeParserBase::getDocumentationFilePath(const std::string& s,
					     const std::string& k) const
  {
    const auto root = tfel::getInstallPath();
    auto f = root+"/share/doc/mtest/"+s+'/'+k.substr(1)+".md";
    std::ifstream desc{f};
    if(desc){
      return f;
    }
    f = root+"/share/doc/mtest/"+k.substr(1)+".md";
    desc.open(f);
    if(desc){
      return f;
    }
    return "";
  }
  
  void SchemeParserBase::handleAuthor(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setAuthor(this->readUntilEndOfInstruction(p));
  } // end of SchemeParserBase::handleAuthor

  void SchemeParserBase::handleDate(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setDate(this->readUntilEndOfInstruction(p));
  } // end of SchemeParserBase::handleDate
  
  void
  SchemeParserBase::handleDescription(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    this->readSpecifiedToken("SchemeParserBase::handleDescription","{",
			     p,this->fileTokens.end());
    this->checkNotEndOfLine("SchemeParserBase::handleDescription",
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
      throw(runtime_error("SchemeParserBase::handleDescription: "
			  "file ended before the end of description."));
    }
    ++p;
    this->readSpecifiedToken("SchemeParserBase::handleDescription",";",
			     p,this->fileTokens.end());
    t.setDescription(description);
  } // end of SchemeParserBase::Description

  void
  SchemeParserBase::handleOutputFile(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setOutputFileName(this->readString(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleOutputFiles",";",
			     p,this->fileTokens.end());
  } // end of SchemeParserBase::handleOutputFile

  void
  SchemeParserBase::handleOutputFilePrecision(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setOutputFilePrecision(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleOutputFilePrecisions",";",
			     p,this->fileTokens.end());
  } // end of SchemeParserBase::handleOutputFilePrecision

  void
  SchemeParserBase::handleResidualFile(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setResidualFileName(this->readString(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleResidualFiles",";",
			     p,this->fileTokens.end());
  } // end of SchemeParserBase::handleResidualFile

  void
  SchemeParserBase::handleResidualFilePrecision(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setResidualFilePrecision(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleResidualFilePrecisions",";",
			     p,this->fileTokens.end());
  } // end of SchemeParserBase::handleResidualFilePrecision

  void
  SchemeParserBase::handleReal(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    const auto& v = this->readString(p,this->fileTokens.end());
    if(!this->isValidIdentifier(v)){
      throw(runtime_error("SchemeParserBase::handleReal : '"+
			  v+"' is not a valid identifier"));
    }
    const real value = this->readDouble(t,p);
    shared_ptr<Evolution> mpev;
    mpev = shared_ptr<Evolution>(new ConstantEvolution(value));
    this->readSpecifiedToken("SchemeParserBase::handleReal",";",
			     p,this->fileTokens.end());
    t.addEvolution(v,mpev,true,true);
  }
  
  void SchemeParserBase::handlePredictionPolicy(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    PredictionPolicy ppolicy;
    this->checkNotEndOfLine("handlePredictionPolicy",
			    p,this->fileTokens.end());
    const auto& s = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("SchemeParserBase::handlePredictionPolicy",";",
			     p,this->fileTokens.end());
    if(s=="NoPrediction"){
      ppolicy = PredictionPolicy::NOPREDICTION;
    } else if(s=="LinearPrediction"){
      ppolicy = PredictionPolicy::LINEARPREDICTION;
    } else if(s=="ElasticPrediction"){
      ppolicy = PredictionPolicy::ELASTICPREDICTION;
    } else if(s=="ElasticPredictionFromMaterialProperties"){
      ppolicy = PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES;
    } else if(s=="SecantOperatorPrediction"){
      ppolicy = PredictionPolicy::SECANTOPERATORPREDICTION;
    } else if(s=="TangentOperatorPrediction"){
      ppolicy = PredictionPolicy::TANGENTOPERATORPREDICTION;
    } else {
      throw(std::runtime_error("SchemeParserBase::handlePredictionPolicy : "
			       "unsupported prediction policy '"+s+"'"));
    }
    t.setPredictionPolicy(ppolicy);
  }

  void
  SchemeParserBase::handleStiffnessMatrixType(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    StiffnessMatrixType ktype;
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
      throw(runtime_error("SchemeParserBase::handleStiffnessMatrixType : "
			  "unsupported stiffness matrix type '"+type+"'"));
    }
    this->readSpecifiedToken("SchemeParserBase::handleStiffnessMatrixType",";",
			     p,this->fileTokens.end());
    t.setStiffnessMatrixType(ktype);
  }

  void
  SchemeParserBase::handleUseCastemAccelerationAlgorithm(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    bool useCastemAcceleration;
    this->checkNotEndOfLine("SchemeParserBase::handleUseCastemAccelerationAlgorithm",
			    p,this->fileTokens.end());
    if(p->value=="true"){
      useCastemAcceleration = true;
    } else if(p->value=="false"){
      useCastemAcceleration = false;
    } else {
      throw(runtime_error("SchemeParserBase::handleUseCastemAccelerationAlgorithm : "
			  "unexpected token '"+p->value+"'"));
    }
    ++p;
    this->readSpecifiedToken("SchemeParserBase::handleUseCastemAccelerationAlgorithm",
			     ";",p,this->fileTokens.end());
    t.setUseCastemAccelerationAlgorithm(useCastemAcceleration);
  }

  void
  SchemeParserBase::handleCastemAccelerationTrigger(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    int cat = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleCastemAccelerationTrigger",";",
			     p,this->fileTokens.end());
    t.setCastemAccelerationTrigger(cat);
  } // end of SchemeParserBase::handleCastemAccelerationTrigger

  void
  SchemeParserBase::handleCastemAccelerationPeriod(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    int cap = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleCastemAccelerationPeriod",";",
			     p,this->fileTokens.end());
    t.setCastemAccelerationPeriod(cap);
  } // end of SchemeParserBase::handleCastemAccelerationPeriod

  void
  SchemeParserBase::handleAccelerationAlgorithm(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("SchemeParserBase::handleAccelerationAlgorithm",
			    p,this->fileTokens.end());
    const auto& a = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("SchemeParserBase::handleAccelerationAlgorithm",
			     ";",p,this->fileTokens.end());
    t.setAccelerationAlgorithm(a);
  }

  void
  SchemeParserBase::handleAccelerationAlgorithmParameter(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("SchemeParserBase::handleAccelerationAlgorithmParameter",
			    p,this->fileTokens.end());
    const auto& pn = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("SchemeParserBase::handleAccelerationAlgorithmParameter",
			    p,this->fileTokens.end());
    const auto& v = p->value;
    ++p;
    this->readSpecifiedToken("SchemeParserBase::handleAccelerationAlgorithmParameter",";",
			     p,this->fileTokens.end());
    t.setAccelerationAlgorithmParameter(pn,v);
  } // end of SchemeParserBase::handleIronsTuckAccelerationTrigger


  void
  SchemeParserBase::handleStiffnessUpdatePolicy(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    StiffnessUpdatingPolicy ks;
    const auto& type = this->readString(p,this->fileTokens.end());
    if(type=="ConstantStiffness"){
      ks=StiffnessUpdatingPolicy::CONSTANTSTIFFNESS;
    } else if(type=="SecantOperator"){
      ks=StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD;
    } else if(type=="TangentOperator"){
      ks=StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX;
    } else {
      throw(std::runtime_error("SchemeParserBase::handleStiffnessUpdatePolicy : "
			       "unsupported stiffness matrix policy '"+type+"'"));
    }
    this->readSpecifiedToken("SchemeParserBase::handleStiffnessUpdatePolicy",";",
			     p,this->fileTokens.end());
    t.setStiffnessUpdatingPolicy(ks);
  }

  void
  SchemeParserBase::handleMaximumNumberOfIterations(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    t.setMaximumNumberOfIterations(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleMaximumNumberOfIterations",";",
			     p,this->fileTokens.end());
  } // end of SchemeParserBase::handleMaximumNumberOfIterations

  void
  SchemeParserBase::handleMaximumNumberOfSubSteps(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    t.setMaximumNumberOfSubSteps(this->readUnsignedInt(p,this->fileTokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleMaximumNumberOfSubSteps",";",
			     p,this->fileTokens.end());
  } // end of SchemeParserBase::handleMaximumNumberOfSubSteps
  
  std::string
  SchemeParserBase::readUntilEndOfInstruction(TokensContainer::const_iterator& p)
  {
    using namespace std;
    string res;
    this->checkNotEndOfLine("SchemeParserBase::readUntilEndOfInstruction",
			    p,this->fileTokens.end());
    while((p!=this->fileTokens.end())&&
	  (p->value != ";")){
      if(!p->value.empty()){
	if(p->value[0]=='@'){
	  string msg("SchemeParserBase::readUntilEndOfInstruction : ");
	  msg += "no word beginning with '@' are allowed here";
	  throw(runtime_error(msg));
	}
	res+=p->value;
	res+=" ";
      }
      ++p;
    }
    this->readSpecifiedToken("SchemeParserBase::readUntilEndOfInstruction",";",
			     p,this->fileTokens.end());
    return res;
  }

  void
  SchemeParserBase::handleTimes(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    vector<real> times;
    this->readSpecifiedToken("SchemeParserBase::handleTimes","{",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("SchemeParserBase::handleTimes",p,
			    this->fileTokens.end());
    while(p->value!="}"){
      const real t_dt = this->readTime(t,p);
      this->checkNotEndOfLine("SchemeParserBase::handleTimes",p,
			      this->fileTokens.end());
      if(!times.empty()){
	if(p->value=="in"){
	  ++p;
	  unsigned int n = this->readUnsignedInt(p,this->fileTokens.end());
	  if(n==0){
	    throw(runtime_error("SchemeParserBase::handleTimes : "
				"invalid number of intervals"));
	  }
	  const real tt = times.back();
	  real dt = (t_dt-tt)/(static_cast<real>(n));
	  for(unsigned int i=1;i!=n;++i){
	    times.push_back(tt+i*dt);
	  }
	}
	this->checkNotEndOfLine("SchemeParserBase::handleTimes",p,
				this->fileTokens.end());
      }
      times.push_back(t_dt);
      if(p->value==","){
	++p;
	this->checkNotEndOfLine("SchemeParserBase::handleTimes",p,
				this->fileTokens.end());
	if(p->value=="}"){
	  string msg("SchemeParserBase::handleTimes : ");
	  msg += "unexpected token '}'";
	  throw(runtime_error(msg));
	}
      } else {
	if(p->value!="}"){
	  string msg("SchemeParserBase::handleTimes : ");
	  msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	  throw(runtime_error(msg));
	}
      }
    }
    this->readSpecifiedToken("SchemeParserBase::handleTimes","}",p,
			     this->fileTokens.end());
    this->readSpecifiedToken("SchemeParserBase::handleTimes",";",p,
			     this->fileTokens.end());
    if(times.empty()){
      string msg("SchemeParserBase::handleTimes : ");
      msg += "no time defined";
      throw(runtime_error(msg));
    }
    if(times.size()==1){
      string msg("SchemeParserBase::handleTimes : ");
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
      string msg("SchemeParserBase::handleTimes : maximal "
		 "absolute value of times is too low");
      throw(runtime_error(msg));
    }
    const real eps = 100*mt*numeric_limits<real>::epsilon();
    pt  = times.begin();
    vector<real>::const_iterator pt2 = pt+1u;
    while(pt2!=times.end()){
      if((*pt2<=*pt)||abs(*pt2-*pt)<eps){
	ostringstream msg;
	msg << "SchemeParserBase::handleTimes : times '" << *pt2 
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
  SchemeParserBase::handleEvolution(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    const auto& evt = this->readEvolutionType(p);
    const auto& n = this->readString(p,this->fileTokens.end());
    t.addEvolution(n,this->parseEvolution(t,evt,p),true,true);
    this->readSpecifiedToken("SchemeParserBase::handleEvolution",";",p,
			     this->fileTokens.end());
  }

  real
  SchemeParserBase::readDouble(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    using namespace std;
    using tfel::utilities::Token;
    using tfel::math::Evaluator;
    this->checkNotEndOfLine("SchemeParserBase::readDouble",p,
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
	  string msg("SchemeParserBase::readDouble : "
		     "no evolution named '"+*pv+"' defined");
	  throw(runtime_error(msg));
	}
	const Evolution& e = *(pev->second);
	if(!e.isConstant()){
	  string msg("SchemeParserBase::readDouble : formula '"+f+"' shall "
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
  } // end of SchemeParserBase::readDouble

  real
  SchemeParserBase::readTime(SchemeBase& t,
			     TokensContainer::const_iterator& p)
  {
    return this->readDouble(t,p);
  } // end of SchemeParserBase::readTime

  void
  SchemeParserBase::readArrayOfSpecifiedSize(std::vector<real>& v,
					     SchemeBase& t,
					     TokensContainer::const_iterator& p)
  {
    this->readSpecifiedToken("SchemeParserBase::readArrayOfSpecifiedSize","{",p,
			     this->fileTokens.end());
    std::vector<real>::size_type i = 0;
    while(i!=v.size()){
      v[i] = this->readDouble(t,p);
      if(++i!=v.size()){
	this->readSpecifiedToken("SchemeParserBase::readArrayOfSpecifiedSize",",",p,
				 this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("SchemeParserBase::readArrayOfSpecifiedSize","}",p,
			     this->fileTokens.end());
  } // end of SchemeParserBase::readArrayOfSpecifiedSize
  
  std::shared_ptr<Evolution>
  SchemeParserBase::parseEvolution(SchemeBase& t,
				   const std::string& type,
				   TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    shared_ptr<Evolution> ev;
    this->checkNotEndOfLine("SchemeParserBase::parseEvolution",p,
			    this->fileTokens.end());
    if(type.empty()||type=="evolution"){
      if(p->value=="{"){
	vector<real> tvalues;
	vector<real> values;
	this->readSpecifiedToken("SchemeParserBase::parseEvolution","{",p,
				 this->fileTokens.end());
	this->checkNotEndOfLine("SchemeParserBase::parseEvolution",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  tvalues.push_back(this->readTime(t,p));
	  this->readSpecifiedToken("SchemeParserBase::parseEvolution",":",p,
				   this->fileTokens.end());
	  values.push_back(this->readDouble(t,p));
	  this->checkNotEndOfLine("SchemeParserBase::parseEvolution",p,
				  this->fileTokens.end());
	  if(p->value==","){
	    ++p;
	    this->checkNotEndOfLine("SchemeParserBase::parseEvolution",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("SchemeParserBase::parseEvolution : ");
	      msg += "unexpected token '}'";
	      throw(runtime_error(msg));
	    }
	  } else {
	    if(p->value!="}"){
	      string msg("SchemeParserBase::parseEvolution : ");
	      msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("SchemeParserBase::parseEvolution","}",p,
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
      string msg("SchemeParserBase::parseEvolution : ");
      msg += "invalid evolution type '"+type+"'";
      throw(runtime_error(msg));
    }
    return ev;
  } // end of SchemeParserBase::parseEvolution

  std::string
  SchemeParserBase::readEvolutionType(TokensContainer::const_iterator& p)
  {
    this->checkNotEndOfLine("SchemeParserBase::readEvolutionType",p,
			    this->fileTokens.end());
    if(p->value!="<"){
      return "";
    }
    this->readSpecifiedToken("SchemeParserBase::parseEvolution","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("SchemeParserBase::readEvolutionType",p,
			    this->fileTokens.end());
    const auto evt = p->value;
    ++p;
    this->readSpecifiedToken("SchemeParserBase::parseEvolution",">",p,
			     this->fileTokens.end());
    return evt;
  } // end of SchemeParserBase::readEvolutionType

  void
  SchemeParserBase::registerCallBack(const std::string& k,
				     const SchemeParserBase::CallBack& p)
  {
    this->callbacks.insert({k,p});
  }
  
  void
  SchemeParserBase::registerCallBacks(void)
  {
    this->registerCallBack("@Author",&SchemeParserBase::handleAuthor);
    this->registerCallBack("@Date",&SchemeParserBase::handleDate);
    this->registerCallBack("@Description",&SchemeParserBase::handleDescription);
    this->registerCallBack("@OutputFile",
			   &SchemeParserBase::handleOutputFile);
    this->registerCallBack("@OutputFilePrecision",
			   &SchemeParserBase::handleOutputFilePrecision);
    this->registerCallBack("@ResidualFile",
			   &SchemeParserBase::handleResidualFile);
    this->registerCallBack("@ResidualFilePrecision",
			   &SchemeParserBase::handleResidualFilePrecision);
    this->registerCallBack("@Times",&SchemeParserBase::handleTimes);
    this->registerCallBack("@AccelerationAlgorithm",
			   &SchemeParserBase::handleAccelerationAlgorithm);
    this->registerCallBack("@AccelerationAlgorithmParameter",
			   &SchemeParserBase::handleAccelerationAlgorithmParameter);
    this->registerCallBack("@UseCastemAccelerationAlgorithm",
			   &SchemeParserBase::handleUseCastemAccelerationAlgorithm);
    this->registerCallBack("@CastemAccelerationPeriod",
			   &SchemeParserBase::handleCastemAccelerationPeriod);
    this->registerCallBack("@CastemAccelerationTrigger",
			   &SchemeParserBase::handleCastemAccelerationTrigger);
    this->registerCallBack("@StiffnessMatrixType",
			   &SchemeParserBase::handleStiffnessMatrixType);
    this->registerCallBack("@StiffnessUpdatePolicy",
			   &SchemeParserBase::handleStiffnessUpdatePolicy);
    this->registerCallBack("@PredictionPolicy",
			   &SchemeParserBase::handlePredictionPolicy);
    this->registerCallBack("@Real",&SchemeParserBase::handleReal);
    this->registerCallBack("@Evolution",&SchemeParserBase::handleEvolution);
    this->registerCallBack("@MaximumNumberOfIterations",
			   &SchemeParserBase::handleMaximumNumberOfIterations);
    this->registerCallBack("@MaximumNumberOfSubSteps",
			   &SchemeParserBase::handleMaximumNumberOfSubSteps);
  } // end of SchemeParserBase::registerCallBacks

  bool
  SchemeParserBase::treatKeyword(SchemeBase& t,TokensContainer::const_iterator& p)
  {
    auto pc = this->callbacks.find(p->value);
    if(pc==this->callbacks.end()){
      return false;
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG){
      auto& log = mfront::getLogStream();
      log << "SchemeParserBase::execute : treating keyword '" << p->value
	  << "' at line '" << p->line << "'\n";
    }
    ++p;
    auto line = p->line;
    try{
      (this->*(pc->second))(t,p);
    } catch(std::exception& e){
      std::ostringstream msg;
      msg << "SchemeParserBase::MTest : error while "
	  << "parsing file '" << this->file << "' at line '"
	  << line << "'.\n" << e.what();
      throw(std::runtime_error(msg.str()));
    }
    return true;
  } // end of SchemeParserBase::treatKeyword

  std::vector<std::string>
  SchemeParserBase::getKeyWordsList(void) const
  {
    auto keys = std::vector<std::string>{};
    for(const auto& k : this->callbacks){
      keys.push_back(k.first);
    }
    return keys;
  } // end of SchemeParserBase::getKeyWordsList
    
  SchemeParserBase::~SchemeParserBase()
  {} // end of SchemeParserBase::~SchemeParserBase()
  
} // end of namespace mtest
