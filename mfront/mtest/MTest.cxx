/*! 
 * \file  MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avril 2013
 */


#include<map>
#include<cmath>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<iterator>
#include<iostream>
#include<algorithm>
#include<stdexcept>

#include"tfel-config.hxx"

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/LUSolve.hxx"

#include"MFront/MTest.hxx"
#include"MFront/MTestLogStream.hxx"
#include"MFront/MTestBehaviour.hxx"
#ifdef HAVE_CASTEM
#include"MFront/MTestUmatBehaviour.hxx"
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
#include"MFront/MTestAsterBehaviour.hxx"
#endif /* HAVE_ASTER  */

#include"MFront/MTestAnalyticalTest.hxx"
#include"MFront/MTestReferenceFileComparisonTest.hxx"

#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestFunctionEvolution.hxx"
#include"MFront/MTestCastemEvolution.hxx"

#include"MFront/MTestConstraint.hxx"
#include"MFront/MTestImposedStress.hxx"
#include"MFront/MTestImposedStrain.hxx"

namespace mfront
{

  static void
  checkIfDeclared(const std::vector<std::string>& n,
		  const std::map<std::string,
				 tfel::utilities::shared_ptr<MTestEvolution> > m,
		  const std::string& type)
  {
    using namespace std;
    vector<string>::const_iterator p;
    for(p=n.begin();p!=n.end();++p){
      if(m.find(*p)==m.end()){
	string msg("no '"+type+"' named '"+*p+"' declared");
	throw(runtime_error(msg));
      }
    }
  }

  static unsigned short
  getSTensorSize(const unsigned short d)
  {
    using namespace std;
    if(d==1){
      return 3;
    } else if(d==2){
      return 4;
    } else if(d==3){
      return 6;
    }
    string msg("mfront::getTensorSize : ");
    msg += "";
    throw(runtime_error(msg));
    return 0;
  }

  MTest::UTest::~UTest()
  {}

  MTest::MTest(const std::string& f)
    : file(f),
      oprec(-1),
      rm(real(0)),
      isRmDefined(false),
      evs(new std::map<std::string,
		       tfel::utilities::shared_ptr<MTestEvolution> >()),
      dimension(0u),
      hypothesis(tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS),
      eeps(-1.),
      seps(-1.),
      mSubSteps(-1),
      iterMax(-1),
      ks(MTest::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY),
      ktype(MTestStiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE),
      ppolicy(MTest::UNSPECIFIEDPREDICTIONPOLICY),
      useCastemAcceleration(false),
      cat(-1),
      cap(-1)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    typedef map<string,CallBack>::value_type MVType;
    map<string,CallBack>::const_iterator pc;
    // declare some variables
    this->declareVariable("t");
    this->registerCallBacks();
    this->treatCharAsString(true);
    this->openFile(f);
    this->stripComments();
    TokensContainer::const_iterator p;
    p = this->fileTokens.begin();
    while(p!=fileTokens.end()){
      pc=this->callbacks.find(p->value);
      if(pc==this->callbacks.end()){
	ostringstream msg;
	msg << "MTest::MTest : invalid keyword '"
	    << p->value << "'. Error at line " 
	    << p->line<< ".";
	throw(runtime_error(msg.str()));
      }
      ++p;
      unsigned int line = p->line;
      try{
	(this->*(pc->second))(p);
      } catch(exception& e){
	ostringstream msg;
	msg << "MTest::MTest : error while "
	    << "parsing file '" << f << "' at line '"
	    << line << "'.\n" << e.what();
	throw(runtime_error(msg.str()));
      }
    }
    // some checks
    if(times.empty()){
      string msg("MTest::MTest : no times defined");
      throw(runtime_error(msg));
    }
    if(this->dimension==0u){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::MTest : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    if(this->output.empty()){
      string::size_type pos = f.rfind('.');
      if(pos!=string::npos){
	this->output = f.substr(0,pos);
      } else {
	this->output = f;
      }
      this->output += ".res";
    }
    this->out.open(this->output.c_str());
    if(!this->out){
      string msg("MTest::MTest : ");
      msg += "can't open file '"+this->output+"'";
      throw(runtime_error(msg));
    }
    this->out.exceptions(ofstream::failbit|ofstream::badbit);
    if(this->oprec!=-1){
      this->out.precision(static_cast<streamsize>(this->oprec));
    }
    if(this->eeps<0){
      this->eeps = 1.e-12;
    }
    if(this->seps<0){
      this->seps = 1.e-3;
    }
    // additional constraints
    if(this->hypothesis==MH::PLANESTRAIN){
      shared_ptr<MTestEvolution>  eev(new MTestConstantEvolution(0.));
      shared_ptr<MTestConstraint> ec(new MTestImposedStrain(3,eev));
      this->constraints.push_back(ec);
    }
    if(this->hypothesis==MH::PLANESTRESS){
      shared_ptr<MTestEvolution>  sev(new MTestConstantEvolution(0.));
      shared_ptr<MTestConstraint> sc(new MTestImposedStress(3,sev));
      this->constraints.push_back(sc);
    }
    if(!this->isRmDefined){
      for(unsigned short i=0;i!=3;++i){
	rm(i,i) = real(1);
      }
    }
    if(this->mSubSteps==-1){
      this->mSubSteps=10;
    }
    if(this->iterMax==-1){
      this->iterMax=100;
    }
    // castem acceleration
    if(this->useCastemAcceleration){
      if(this->cap==-1){
	this->cap=2;
      }
      if(this->cat==-1){
	this->cat=4;
      }
    }
    // prediction policy
    if(this->ppolicy==UNSPECIFIEDPREDICTIONPOLICY){
      this->ppolicy=NOPREDICTION;
    }
    // stiffness matrix type
    if(this->ktype==MTestStiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      this->ktype = this->b->getDefaultStiffnessMatrixType();
    }
    // declaring behaviour variables
    this->declareVariables(this->enames);
    this->declareVariables(this->snames);
  } // end of MTest::MTest

  MTest::MTest()
  {
    this->registerCallBacks();
  }

  void
  MTest::displayKeyWordsList(void) const
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
      string root;
      const char * const path = getenv("TFELHOME");
      if(path!=0){
	root = string(path);
      } else {
	root = PREFIXDIR;
      }
      const string f = root+"/share/doc/mtest/"+pk->first.substr(1)+".txt";
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
  } // end of MTest::displayKeywordsList

  void
  MTest::registerCallBacks()
  {
    this->registerCallBack("@UseCastemAccelerationAlgorithm",
			   &MTest::handleUseCastemAccelerationAlgorithm);
    this->registerCallBack("@CastemAccelerationPeriod",
			   &MTest::handleCastemAccelerationPeriod);
    this->registerCallBack("@CastemAccelerationTrigger",
			   &MTest::handleCastemAccelerationTrigger);
    this->registerCallBack("@StiffnessMatrixType",
			   &MTest::handleStiffnessMatrixType);
    this->registerCallBack("@StiffnessUpdatePolicy",
			   &MTest::handleStiffnessUpdatePolicy);
    this->registerCallBack("@PredictionPolicy",
			   &MTest::handlePredictionPolicy);
    this->registerCallBack("@Author",
			   &MTest::handleAuthor);
    this->registerCallBack("@Date",
			   &MTest::handleDate);
    this->registerCallBack("@Description",
			   &MTest::handleDescription);
    this->registerCallBack("@OutputFile",
			   &MTest::handleOutputFile);
    this->registerCallBack("@OutputFilePrecision",
			   &MTest::handleOutputFilePrecision);
    this->registerCallBack("@Test",
			   &MTest::handleTest);
    this->registerCallBack("@Real",
			   &MTest::handleReal);
    this->registerCallBack("@Evolution",
			   &MTest::handleEvolution);
    this->registerCallBack("@RotationMatrix",
			   &MTest::handleRotationMatrix);
    this->registerCallBack("@MaximumNumberOfIterations",
			   &MTest::handleMaximumNumberOfIterations);
    this->registerCallBack("@MaximumNumberOfSubSteps",
			   &MTest::handleMaximumNumberOfSubSteps);
    this->registerCallBack("@StrainEpsilon",
			   &MTest::handleStrainEpsilon);
    this->registerCallBack("@StressEpsilon",
			   &MTest::handleStressEpsilon);
    this->registerCallBack("@ModellingHypothesis",
			   &MTest::handleModellingHypothesis);
    this->registerCallBack("@Strain",&MTest::handleStrain);
    this->registerCallBack("@Stress",&MTest::handleStress);
    this->registerCallBack("@Times",&MTest::handleTimes);
    this->registerCallBack("@Behaviour",&MTest::handleBehaviour);
    this->registerCallBack("@MaterialProperty",
			   &MTest::handleMaterialProperty);
    this->registerCallBack("@InternalStateVariable",
			   &MTest::handleInternalStateVariable);
    this->registerCallBack("@ExternalStateVariable",
			   &MTest::handleExternalStateVariable);
    this->registerCallBack("@ImposedStrain",
			   &MTest::handleImposedStrain);
    this->registerCallBack("@ImposedStress",
			   &MTest::handleImposedStress);
  }

  void
  MTest::registerCallBack(const std::string& k,
			  const MTest::CallBack& p)
  {
    using namespace std;
    typedef map<string,CallBack>::value_type MVType;
    this->callbacks.insert(MVType(k,p));
  }

  void
  MTest::displayKeyWordDescription(const std::string& k) const
  {
    using namespace std;
    if(this->callbacks.find(k)==this->callbacks.end()){
      string msg("MTest::displayKeyWordDescription : "
		 "unknowns keyword '"+k+"'");
      throw(runtime_error(msg));
    }
    string root;
    const char * const path = getenv("TFELHOME");
    if(path!=0){
      root = string(path);
    } else {
      root = PREFIXDIR;
    }
    const string f = root+"/share/doc/mtest/"+k.substr(1)+".txt";
    ifstream desc(f.c_str());
    if(!desc){
      cout << "no description available for keyword '"
	   << k << "'" << endl;
    }
    cout << desc.rdbuf();
  }

  std::string
  MTest::name(void) const
  {
    return this->file;
  } // end of MTest::name
  
  std::string
  MTest::classname(void) const
  {
    return "MTest";
  }

  std::string
  MTest::readUntilEndOfInstruction(TokensContainer::const_iterator& p)
  {
    using namespace std;
    string res;
    this->checkNotEndOfLine("MTest::readUntilEndOfInstruction",
			    p,this->fileTokens.end());
    while((p!=this->fileTokens.end())&&
	  (p->value != ";")){
      if(!p->value.empty()){
	if(p->value[0]=='@'){
	  string msg("MTest::readUntilEndOfInstruction : ");
	  msg += "no word beginning with '@' are allowed here";
	  throw(runtime_error(msg));
	}
	res+=p->value;
	res+=" ";
      }
      ++p;
    }
    this->readSpecifiedToken("MTest::readUntilEndOfInstruction",";",
			     p,this->fileTokens.end());
    return res;
  }

  void
  MTest::handleDescription(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    unsigned short currentLine;
    unsigned short openedBrackets;
    if(!this->description.empty()){
      string msg("MTest::handleDescription : ");
      msg += "@Description shall only be called once.";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleDescription","{",
			     p,this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleDescription",
			    p,this->fileTokens.end());
    currentLine = p->line;
    openedBrackets = 1u;
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
	this->description+="\n";
	currentLine=p->line;
      }
      if(p->flag==Token::String){
	this->description+=p->value.substr(1,p->value.size()-2);
      } else {
	this->description+=p->value;
      }
      this->description+=" ";
      ++p;
    }
    if(p==this->fileTokens.end()){
      --p;
      string msg("MTest::handleDescription",
		 "File ended before the end of description.");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleDescription",";",
			     p,this->fileTokens.end());
  } // end of MTest::Description

  void MTest::handleAuthor(TokensContainer::const_iterator& p)
  {
    this->author = this->readUntilEndOfInstruction(p);
  } // end of MTest::handleAuthor

  void MTest::handlePredictionPolicy(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->ppolicy!=UNSPECIFIEDPREDICTIONPOLICY){
      string msg("MTest::handlePredictionPolicy : "
		 "prediction policy already declared");
      throw(runtime_error(msg));
    }
    this->checkNotEndOfLine("handlePredictionPolicy",
			    p,this->fileTokens.end());
    const string& s = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handlePredictionPolicy",";",
			     p,this->fileTokens.end());
    if(s=="NoPrediction"){
      this->ppolicy = NOPREDICTION;
    } else if(s=="LinearPrediction"){
      this->ppolicy = LINEARPREDICTION;
    } else if(s=="ElasticPrediction"){
      this->ppolicy = ELASTICPREDICTION;
    } else if(s=="SecantOperatorPrediction"){
      this->ppolicy = SECANTOPERATORPREDICTION;
    } else if(s=="TangentOperatorPrediction"){
      this->ppolicy = TANGENTOPERATORPREDICTION;
    } else {
      string msg("MTest::handlePredictionPolicy : "
		 "unsupported prediction policy '"+s+"'");
      throw(runtime_error(msg));
    }
  }

  void MTest::handleDate(TokensContainer::const_iterator& p)
  {
    this->date = this->readUntilEndOfInstruction(p);
  } // end of MTest::handleDate

  void
  MTest::handleStiffnessMatrixType(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->ktype!=MTestStiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE){
      string msg("MTest::handleStiffnessMatrixType : "
		 "stiffness matrix type already specificed");
      throw(runtime_error(msg));
    }
    const string& type = this->readString(p,this->fileTokens.end());
    if(type=="Elastic"){
      this->ktype=MTestStiffnessMatrixType::ELASTIC;
    } else if(type=="SecantOperator"){
      this->ktype=MTestStiffnessMatrixType::SECANTOPERATOR;
    } else if(type=="TangentOperator"){
      this->ktype=MTestStiffnessMatrixType::TANGENTOPERATOR;
    } else if(type=="ConsistantTangentOperator"){
      this->ktype=MTestStiffnessMatrixType::CONSISTANTTANGENTOPERATOR;
    } else {
      string msg("MTest::handleStiffnessMatrixType : "
		 "unsupported stiffness matrix type '"+type+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleStiffnessMatrixType",";",
			     p,this->fileTokens.end());
  }

  void
  MTest::handleUseCastemAccelerationAlgorithm(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->useCastemAcceleration){
      string msg("MTest::handleUseCastemAccelerationAlgorithm : "
		 "@UseCastemAcceleration already called");
      throw(runtime_error(msg));
    }
    this->checkNotEndOfLine("MTest::handleUseCastemAccelerationAlgorithm",
			    p,this->fileTokens.end());
    if(p->value=="true"){
      this->useCastemAcceleration = true;
    } else if(p->value=="false"){
            this->useCastemAcceleration = false;
    } else {
      string msg("MTest::handleUseCastemAccelerationAlgorithm : "
		 "unexpected token '"+p->value+"'");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleUseCastemAccelerationAlgorithm",
			     ";",p,this->fileTokens.end());
  }

  void
  MTest::handleCastemAccelerationTrigger(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->cat!=-1){
      string msg("MTest::handleCastemAccelerationTrigger : "
		 "the castem acceleration trigger has already "
		 "been defined");
      throw(runtime_error(msg));
    }
    this->cat = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    if(this->cat<3){
      string msg("MTest::handleCastemAccelerationTrigger",
		 "invalid acceleration trigger value.");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleCastemAccelerationTrigger",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleCastemAccelerationTrigger

  void
  MTest::handleCastemAccelerationPeriod(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->cap!=-1){
      string msg("MTest::handleCastemAccelerationPeriod : "
		 "the castem acceleration period has already "
		 "been defined");
      throw(runtime_error(msg));
    }
    this->cap = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    if(this->cap==0){
      string msg("MTest::handleCastemAccelerationPeriod",
		 "invalid acceleration period value.");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleCastemAccelerationPeriod",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleCastemAccelerationPeriod

  void
  MTest::handleStiffnessUpdatePolicy(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->ks!=MTest::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY){
      string msg("MTest::handleStiffnessUpdatePolicy : "
		 "stiffness matrix type already specificed");
      throw(runtime_error(msg));
    }
    const string& type = this->readString(p,this->fileTokens.end());
    if(type=="ConstantStiffness"){
      this->ks=CONSTANTSTIFFNESS;
    } else if(type=="SecantOperator"){
      this->ks=CONSTANTSTIFFNESSBYPERIOD;
    } else if(type=="TangentOperator"){
      this->ks=UPDATEDSTIFFNESSMATRIX;
    } else {
      string msg("MTest::handleStiffnessUpdatePolicy : "
		 "unsupported stiffness matrix policy '"+type+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleStiffnessUpdatePolicy",";",
			     p,this->fileTokens.end());
  }

  void
  MTest::handleTest(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,string>::value_type MVType;
    typedef map<string,unsigned int>::value_type MVType2;
    this->readSpecifiedToken("MTest::handleTest","<",
			     p,this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleTest",p,
			    this->fileTokens.end());
    const string& t = p->value;
    if((t!="function")&&
       (t!="file")){
      string msg("MTest::handleTest : "
		 "invalid test type '"+t+"'");
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleTest",">",
			     p,this->fileTokens.end());
    if(t=="function"){
      this->checkNotEndOfLine("MTest::handleTest",p,
			      this->fileTokens.end());
      map<string,string> functions; 
      if(p->flag==Token::String){
	const string& v = this->readString(p,this->fileTokens.end());
	const string& f = this->readString(p,this->fileTokens.end());
	functions.insert(MVType(v,f));
      } else {
	this->readSpecifiedToken("MTest::handleTest","{",
				 p,this->fileTokens.end());
	this->checkNotEndOfLine("MTest::handleTest",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  const string& v = this->readString(p,this->fileTokens.end());
	  this->readSpecifiedToken("MTest::handleTest",":",
				   p,this->fileTokens.end());
	  const string& f = this->readString(p,this->fileTokens.end());
	  functions.insert(MVType(v,f));
	  this->checkNotEndOfLine("MTest::handleTest",p,
				  this->fileTokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("MTest::handleTest",",",
				     p,this->fileTokens.end());	
	    this->checkNotEndOfLine("MTest::handleTest",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("MTest::handleTest : "
			 "unexpected token '}'");
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTest::handleTest","}",
				 p,this->fileTokens.end());	
      }
      const real eps = this->readDouble(p);
      if(eps<0){
	string msg("MTest::handleTest : "
		   "invalid criterium value");
	throw(runtime_error(msg));
      }
      map<string,string>::const_iterator pf;
      for(pf=functions.begin();pf!=functions.end();++pf){
	UTest::TestedVariable type;
	unsigned short pos;
	this->getVariableTypeAndPosition(type,pos,pf->first);
	shared_ptr<UTest> test;
	test = shared_ptr<UTest>(new MTestAnalyticalTest(pf->second,pf->first,
							 type,pos,this->evs,eps));
	this->tests.push_back(test);
      }
    } else if (t=="file"){
      const string& f = this->readString(p,this->fileTokens.end());
      this->checkNotEndOfLine("MTest::handleTest",p,
			      this->fileTokens.end());
      map<string,unsigned int> columns; 
      if(p->flag==Token::String){
	const string& v      = this->readString(p,this->fileTokens.end());
	const unsigned int c = this->readUnsignedInt(p,this->fileTokens.end());
	columns.insert(MVType2(v,c));
      } else {
	this->readSpecifiedToken("MTest::handleTest","{",
				 p,this->fileTokens.end());
	this->checkNotEndOfLine("MTest::handleTest",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  const string& v = this->readString(p,this->fileTokens.end());
	  this->readSpecifiedToken("MTest::handleTest",":",
				   p,this->fileTokens.end());
	  const unsigned int c = this->readUnsignedInt(p,this->fileTokens.end());
	  columns.insert(MVType2(v,c));
	  this->checkNotEndOfLine("MTest::handleTest",p,
				  this->fileTokens.end());
	  if(p->value!="}"){
	    this->readSpecifiedToken("MTest::handleTest",",",
				     p,this->fileTokens.end());	
	    this->checkNotEndOfLine("MTest::handleTest",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("MTest::handleTest : "
			 "unexpected token '}'");
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTest::handleTest","}",
				 p,this->fileTokens.end());	
      }
      const real eps = this->readDouble(p);
      if(eps<0){
	string msg("MTest::handleTest : "
		   "invalid criterium value");
	throw(runtime_error(msg));
      }
      map<string,unsigned int>::const_iterator pf;
      shared_ptr<TextData> data(new TextData(f));
      for(pf=columns.begin();pf!=columns.end();++pf){
	UTest::TestedVariable type;
	unsigned short pos;
	this->getVariableTypeAndPosition(type,pos,pf->first);
	shared_ptr<UTest> test;
	test = shared_ptr<UTest>(new MTestReferenceFileComparisonTest(data,pf->first,
								      pf->second,
								      type,pos,eps));
	this->tests.push_back(test);
      }
    } else {
      string msg("MTest::handleTest : "
		 "invalid test type '"+t+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleTest",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleTest

  void
  MTest::getVariableTypeAndPosition(UTest::TestedVariable& type,
				    unsigned short& pos,
				    const std::string& n)
  {
    using namespace std;
    vector<string>::const_iterator p;
    p=find(this->enames.begin(),this->enames.end(),n);
    if(p!=this->enames.end()){
      pos  = static_cast<unsigned short>(p-this->enames.begin());
      type = MTest::UTest::STRAINS;
      return;
    } 
    p=find(this->snames.begin(),this->snames.end(),n);
    if(p!=this->snames.end()){
      pos  = static_cast<unsigned short>(p-this->snames.begin());
      type = MTest::UTest::STRESSES;
      return;
    } 
    p=find(this->ivfullnames.begin(),this->ivfullnames.end(),n);
    if(p!=this->ivfullnames.end()){
      pos  = static_cast<unsigned short>(p-this->ivfullnames.begin());
      type = MTest::UTest::INTERNAL_STATE_VARIABLES;
      return;
    } 
    string msg("MTest::getVariableTypeAndPosition : "
	       "no variable name '"+n+"'");
    throw(runtime_error(msg));
  } // end of MTest::getVariableTypeAndPosition

  void
  MTest::handleReal(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef map<string,real>::value_type MVType;
    const string& v = this->readString(p,this->fileTokens.end());
    if(!this->isValidIdentifier(v)){
      string msg("MTest::handleReal : '"+
		 v+"' is not a valid identifier");
      throw(runtime_error(msg));
    }
    this->declareVariable(v);
    const real value =
      this->readDouble(p);
    shared_ptr<MTestEvolution> mpev;
    mpev = shared_ptr<MTestEvolution>(new MTestConstantEvolution(value));
    (*(this->evs))[v] = mpev;
    this->readSpecifiedToken("MTest::handleReal",";",
			     p,this->fileTokens.end());
  }

  void
  MTest::handleMaximumNumberOfIterations(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->iterMax!=-1){
      string msg("MTest::handleMaximumNumberOfIterations : "
		 "the maximum number of sub steps "
		 "has already been declared");
      throw(runtime_error(msg));
    }
    this->iterMax = this->readUnsignedInt(p,this->fileTokens.end());
    if(this->iterMax==0){
      string msg("MTest::handleMaximumNumberOfIterations : ");
      msg += "invalid number of intervals";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleMaximumNumberOfIterations",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleMaximumNumberOfIterations

  void
  MTest::handleMaximumNumberOfSubSteps(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->mSubSteps!=-1){
      string msg("MTest::handleMaximumNumberOfSubSteps : "
		 "the maximum number of sub steps "
		 "has already been declared");
      throw(runtime_error(msg));
    }
    this->mSubSteps = this->readUnsignedInt(p,this->fileTokens.end());
    if(this->mSubSteps==0){
      string msg("MTest::handleMaximumNumberOfSubSteps : ");
      msg += "invalid number of intervals";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleMaximumNumberOfSubSteps",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleMaximumNumberOfSubSteps

  void
  MTest::handleRotationMatrix(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::math;
    using std::vector;
    if(this->isRmDefined){
      string msg("MTest::handleRotationMatrix : "
		 "rotation matrix already defined");
      throw(runtime_error(msg));
    }
    if(this->b.get()==0){
      string msg("MTest::handleRotationMatrix : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    if(this->b->getBehaviourType()!=1){
      string msg("MTest::handleRotationMatrix : "
		 "rotation matrix may only be defined "
		 "for orthotropic behaviours");
      throw(runtime_error(msg));
    }
    this->isRmDefined = true;
    this->readSpecifiedToken("MTest::handleRotationMatrix","{",
			     p,this->fileTokens.end());
    vector<vector<real> > v(3);
    for(unsigned short i=0;i!=3;){
      v[i].resize(3);
      this->readArrayOfSpecifiedSize(v[i],p);
      if(++i!=3){
	this->readSpecifiedToken("MTest::handleRotationMatrix",",",
				 p,this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("MTest::handleRotationMatrix","}",
			     p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleRotationMatrix",";",
			     p,this->fileTokens.end());
    // saving the read values
    for(unsigned short i=0;i!=3;++i){
      for(unsigned short j=0;j!=3;++j){
	this->rm(i,j)=v[i][j];
      }
    }
    // checking that the given matrix is a rotation one
    tvector<3u,real> c0 = rm.column_view<0>();
    tvector<3u,real> c1 = rm.column_view<1>();
    tvector<3u,real> c2 = rm.column_view<2>();
    if((abs(norm(c0)-real(1))>100*numeric_limits<real>::epsilon())||
       (abs(norm(c1)-real(1))>100*numeric_limits<real>::epsilon())||
       (abs(norm(c2)-real(1))>100*numeric_limits<real>::epsilon())){
      string msg("MTest::handleRotationMatrix : "
		 "at least one column is not normalised");
      throw(runtime_error(msg));
    }
    if((abs(c0|c1)>100*numeric_limits<real>::epsilon())||
       (abs(c0|c2)>100*numeric_limits<real>::epsilon())||
       (abs(c1|c2)>100*numeric_limits<real>::epsilon())){
      string msg("MTest::handleRotationMatrix : "
		 "at least two columns are not orthogonals");
      throw(runtime_error(msg));
    }
  } // end of MTest::handleRotationMatrix

  void
  MTest::handleStrainEpsilon(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->eeps>0){
      string msg("MTest::handleStrainEpsilon : the epsilon "
		 "value has already been declared");
      throw(runtime_error(msg));
    }
    this->eeps = this->readDouble(p);
    if(this->eeps < 100*numeric_limits<real>::min()){
      string msg("MTest::handleStrainEpsilon : invalid value");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleStrainEpsilon",";",
			     p,this->fileTokens.end());
  }

  void
  MTest::handleStressEpsilon(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->seps>0){
      string msg("MTest::handleStressEpsilon : the epsilon "
		 "value has already been declared");
      throw(runtime_error(msg));
    }
    this->seps = this->readDouble(p);
    if(this->seps < 100*numeric_limits<real>::min()){
      string msg("MTest::handleStressEpsilon : invalid value");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleStressEpsilon",";",
			     p,this->fileTokens.end());
  }

  void
  MTest::handleOutputFile(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->output.empty()){
      string msg("MTest::handleOutputFile : ");
      msg += "message already defined";
      throw(runtime_error(msg));
    }
    this->output = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleOutputFiles",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleOutputFile

  void
  MTest::handleOutputFilePrecision(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(this->oprec!=-1){
      string msg("MTest::handleOutputFilePrecision : ");
      msg += "output file precision already defined";
      throw(runtime_error(msg));
    }
    this->oprec = static_cast<int>(this->readUnsignedInt(p,this->fileTokens.end()));
    if(this->oprec<=0){
      string msg("MTest::handleOutputFilePrecision : ");
      msg += "invalid value for the output file precision";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleOutputFilePrecisions",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleOutputFilePrecision

  void
  MTest::handleModellingHypothesis(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    static const string exts[6u]  = {"XX","YY","ZZ",
				     "XY","XZ","YZ"};
    static const string aexts[4u] = {"RR","ZZ","TT","RZ"};  
    if(this->dimension!=0){
      string msg("MTest::handleModellingHypothesis : ");
      msg += "the modelling hypothesis is already defined";
      throw(runtime_error(msg));
    }
    const string& h = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleTimes",";",p,
			     this->fileTokens.end());
    if(h=="AxisymmetricalGeneralisedPlaneStrain"){
      this->dimension=1u;
      this->hypothesis = MH::AXISYMMETRICALGENERALISEDPLANESTRAIN;
      copy(aexts,aexts+3u,
	   back_inserter(this->sexts));
    } else if(h=="Axisymmetrical"){
      this->dimension  = 2u;
      this->hypothesis = MH::AXISYMMETRICAL;
      copy(aexts,aexts+4u,
	   back_inserter(this->sexts));
    } else if(h=="PlaneStress"){
      this->dimension  = 2u;
      this->hypothesis = MH::PLANESTRESS;
      copy(exts,exts+4u,
	   back_inserter(this->sexts));
    } else if(h=="PlaneStrain"){
      this->dimension  = 2u;
      this->hypothesis = MH::PLANESTRAIN;
      copy(exts,exts+4u,
	   back_inserter(this->sexts));
    } else if(h=="GeneralisedPlaneStrain"){
      this->dimension  = 2u;
      this->hypothesis = MH::GENERALISEDPLANESTRAIN;
      copy(exts,exts+4u,
	   back_inserter(this->sexts));
    } else if(h=="Tridimensional"){
      this->dimension  = 3u;
      this->hypothesis = MH::TRIDIMENSIONAL;
      copy(exts,exts+6u,
	   back_inserter(this->sexts));
    } else {
      string msg("MTest::handleModellingHypothesis : ");
      msg += "unsupported hypothesis '"+h+"'";
      throw(runtime_error(msg));
    }
    for(vector<string>::size_type i=0;i!=this->sexts.size();++i){
      this->enames.push_back("E"+sexts[i]);
      this->snames.push_back("S"+sexts[i]);
    }
  }

  void
  MTest::handleTimes(TokensContainer::const_iterator& p)
  {
    using namespace std;
    if(!times.empty()){
      string msg("MTest::handleTimes : ");
      msg += "times already defined";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleTimes","{",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleTimes",p,
			    this->fileTokens.end());
    while(p->value!="}"){
      const real t_dt = this->readTime(p);
      this->checkNotEndOfLine("MTest::handleTimes",p,
			      this->fileTokens.end());
      if(!this->times.empty()){
	if(p->value=="in"){
	  ++p;
	  unsigned int n = this->readUnsignedInt(p,this->fileTokens.end());
	  if(n==0){
	    string msg("MTest::handleTimes : ");
	    msg += "invalid number of intervals";
	    throw(runtime_error(msg));
	  }
	  const real t = this->times.back();
	  real dt = (t_dt-t)/(static_cast<real>(n));
	  for(unsigned int i=1;i!=n;++i){
	    this->times.push_back(t+i*dt);
	  }
	}
	this->checkNotEndOfLine("MTest::handleTimes",p,
				this->fileTokens.end());
      }
      this->times.push_back(t_dt);
      if(p->value==","){
	++p;
	this->checkNotEndOfLine("MTest::handleTimes",p,
				this->fileTokens.end());
	if(p->value=="}"){
	  string msg("MTest::handleTimes : ");
	  msg += "unexpected token '}'";
	  throw(runtime_error(msg));
	}
      } else {
	if(p->value!="}"){
	  string msg("MTest::handleTimes : ");
	  msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	  throw(runtime_error(msg));
	}
      }
    }
    this->readSpecifiedToken("MTest::handleTimes","}",p,
			     this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleTimes",";",p,
			     this->fileTokens.end());
    if(times.empty()){
      string msg("MTest::handleTimes : ");
      msg += "no time defined";
      throw(runtime_error(msg));
    }
    if(times.size()==1){
      string msg("MTest::handleTimes : ");
      msg += "at least two different times must be defined";
      throw(runtime_error(msg));
    }
    vector<real>::const_iterator pt  = times.begin();
    vector<real>::const_iterator pt2 = pt+1u;
    while(pt2!=times.end()){
      if((*pt2<=*pt)||abs(*pt2-*pt)<1.e-3){
	ostringstream msg;
	msg << "MTest::handleTimes : times '" << *pt2 
	    << "' is lesser than or too close to  time '"
	    << *pt << "'";
	throw(runtime_error(msg.str()));
      }
      ++pt2;
      ++pt;
    }
  }

  void
  MTest::setDefaultHypothesis(void)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(getVerboseMode()>=VERBOSE_LEVEL1){
      ostream& log = getLogStream();
      log << "No hypothesis defined, using default" << endl;
    }
    if(this->hypothesis!=MH::UNDEFINEDHYPOTHESIS){
      string msg("MTest::setDefaultHypothesis : "
		 "internal error : the modelling "
		 "hypothesis is already defined");
      throw(runtime_error(msg));
    }
    this->hypothesis = MH::TRIDIMENSIONAL;
    this->dimension  = 3u;
    this->sexts.resize(6u);
    this->sexts[0] = "XX";this->sexts[1] = "YY";this->sexts[2] = "ZZ";
    this->sexts[3] = "XY";this->sexts[4] = "XZ";this->sexts[5] = "YZ";
    for(vector<string>::size_type i=0;i!=this->sexts.size();++i){
      this->enames.push_back("E"+sexts[i]);
      this->snames.push_back("S"+sexts[i]);
    }
  }
  
  void
  MTest::handleImposedStress(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    const string& evt = this->readEvolutionType(p);
    const string& c   = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleImposedStress",p,
			    this->fileTokens.end());
    shared_ptr<MTestEvolution> sev = this->parseEvolution(evt,p);
    shared_ptr<MTestConstraint> sc(new MTestImposedStress(this->hypothesis,c,sev));
    this->constraints.push_back(sc);
    (*(this->evs))[c] = sev;
    this->readSpecifiedToken("MTest::handleImposedStress",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleImposedStress

  void
  MTest::handleImposedStrain(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    const string& evt = this->readEvolutionType(p);
    const string& c   = this->readString(p,this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleImposedStrain",p,
			    this->fileTokens.end());
    shared_ptr<MTestEvolution> sev = this->parseEvolution(evt,p);
    shared_ptr<MTestConstraint> sc(new MTestImposedStrain(this->hypothesis,c,sev));
    (*(this->evs))[c] = sev;
    this->constraints.push_back(sc);
    this->readSpecifiedToken("MTest::handleImposedStrain",";",
			     p,this->fileTokens.end());
  } // end of MTest::handleImposedStrain

  void
  MTest::handleBehaviour(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    string interface;
    this->readSpecifiedToken("MTest::handleBehaviour","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleBehaviour",p,
			    this->fileTokens.end());
#ifdef HAVE_CASTEM
    if(p->value=="umat"){
      interface = p->value;
    }
#endif /* HAVE_CASTEM */
#ifdef HAVE_ASTER
    if(p->value=="aster"){
      interface = p->value;
    }
#endif /* HAVE_ASTER */
    if(interface.empty()){
      string msg("MTest::handleBehaviour : ");
      msg += "unknown interface '"+p->value+"'";
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleBehaviour",">",p,
			     this->fileTokens.end());
    const string& l = this->readString(p,this->fileTokens.end());
    const string& f = this->readString(p,this->fileTokens.end());
    this->readSpecifiedToken("MTest::handleBehaviour",";",p,
			     this->fileTokens.end());
#ifdef HAVE_CASTEM
    if(interface=="umat"){
      this->b = shared_ptr<MTestBehaviour>(new MTestUmatBehaviour(this->hypothesis,l,f));
    }
#endif
#ifdef HAVE_ASTER
    if(interface=="aster"){
      this->b = shared_ptr<MTestBehaviour>(new MTestAsterBehaviour(this->hypothesis,l,f));
    }
#endif
    if(this->b.get()==0){
      string msg("MTest::handleBehaviour : ");
      msg += "unknown interface '"+interface+"'";
      throw(runtime_error(msg));
    }
    const vector<string>& ivnames = this->b->getInternalStateVariablesNames();
    this->declareVariables(this->b->getMaterialPropertiesNames());
    this->declareVariables(ivnames);
    this->declareVariables(this->b->getExternalStateVariablesNames());
    vector<string>::const_iterator pn;
    for(pn=ivnames.begin();pn!=ivnames.end();++pn){
      unsigned short t = this->b->getInternalStateVariableType(*pn);
      if(t==0){
	this->ivfullnames.push_back(*pn);
      } else if(t==1){
	for(vector<string>::size_type i=0;i!=this->sexts.size();++i){
	  const string& vn = *pn+this->sexts[i];
	  this->declareVariable(vn);
	  this->ivfullnames.push_back(vn);
	}
      } else {
	string msg("MTest::handleBehaviour : "
		   "unsupported variable type for variable '"+*pn+"'");
	throw(runtime_error(msg));
      }
    }
  } // end of MTest::handleBehaviour

  void
  MTest::handleMaterialProperty(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    string interface;
    if(this->b.get()==0){
      string msg("MTest::handleMaterialProperty : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleMaterialProperty","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::handleMaterialProperty",p,
			    this->fileTokens.end());
    if((p->value=="constant")||
       (p->value=="castem")||
       (p->value=="function")){
      interface = p->value;
    } else {
      string msg("MTest::handleMaterialProperty : ");
      msg += "unknown interface '"+interface+"'";
      throw(runtime_error(msg));
    }
    ++p;
    this->readSpecifiedToken("MTest::handleMaterialProperty",">",p,
			     this->fileTokens.end());
    const string& n = this->readString(p,this->fileTokens.end());
    const vector<string>& mpnames = this->b->getMaterialPropertiesNames();
    if((find(mpnames.begin(),mpnames.end(),n)==mpnames.end())&&
       (n!="ThermalExpansion") &&(n!="ThermalExpansion1")&&
       (n!="ThermalExpansion2")&&(n!="ThermalExpansion3")){
      string msg("MTest::handleMaterialProperty : ");
      msg += "the behaviour don't declare a material property '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    if(this->evs->find(n)!=this->evs->end()){
      string msg("MTest::handleMaterialProperty : ");
      msg += "'"+n+"' already declared";
      throw(runtime_error(msg));
    }
    if(interface=="constant"){
      shared_ptr<MTestEvolution> mpev;
      const real v = this->readDouble(p);
      mpev = shared_ptr<MTestEvolution>(new MTestConstantEvolution(v));
      (*(this->evs))[n] = mpev;
    } else if(interface=="function"){
      shared_ptr<MTestEvolution> mpev;
      const string f = this->readString(p,this->fileTokens.end());
      mpev = shared_ptr<MTestEvolution>(new MTestFunctionEvolution(f,this->evs));
      (*(this->evs))[n] = mpev;
    } else if(interface=="castem"){
      shared_ptr<MTestEvolution> mpev;
      const string l = this->readString(p,this->fileTokens.end());
      const string f = this->readString(p,this->fileTokens.end());
      mpev = shared_ptr<MTestEvolution>(new MTestCastemEvolution(l,f,this->evs));
      (*(this->evs))[n] = mpev;
    } else {
      string msg("MTest::handleMaterialProperty : ");
      msg += "unknown interface '"+interface+"'";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleMaterialProperty",";",p,
			     this->fileTokens.end());
  }
    
  void
  MTest::handleStrain(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::handleStrain : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    if(!this->e_t0.empty()){
      string msg("MTest::handleStrain : ");
      msg += "the initial values of the strains have already been declared";
      throw(runtime_error(msg));
    }
    const unsigned short N = getSTensorSize(this->dimension);
    this->e_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(this->e_t0,p);
    this->readSpecifiedToken("MTest::handleStrain",
			     ";",p,this->fileTokens.end());
  }

  void
  MTest::handleStress(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::handleStress : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    if(!this->s_t0.empty()){
      string msg("MTest::handleStress : ");
      msg += "the initial values of the stresses have already been declared";
      throw(runtime_error(msg));
    }
    const unsigned short N = getSTensorSize(this->dimension);
    this->s_t0.resize(N,0);
    this->readArrayOfSpecifiedSize(this->s_t0,p);
    this->readSpecifiedToken("MTest::handleStress",
			     ";",p,this->fileTokens.end());
  }

  void
  MTest::handleInternalStateVariable(TokensContainer::const_iterator& p)
  {
    using namespace std;
    typedef tfel::material::ModellingHypothesis MH;
    if(this->hypothesis==MH::UNDEFINEDHYPOTHESIS){
      this->setDefaultHypothesis();
    }
    if(this->b.get()==0){
      string msg("MTest::handleInternalStateVariable : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const string& n = this->readString(p,this->fileTokens.end());
    const vector<string>& ivsnames = this->b->getInternalStateVariablesNames();
    if(find(ivsnames.begin(),ivsnames.end(),n)==ivsnames.end()){
      string msg("MTest::handleInternalStateVariable : ");
      msg += "the behaviour don't declare an internal state variable named '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    const int type           = b->getInternalStateVariableType(n);
    const unsigned short pos = b->getInternalStateVariablePosition(this->dimension,n);
    if(type==0){
      const real v = this->readDouble(p);
      if(this->iv_t0.size()<=pos){
	this->iv_t0.resize(pos+1,0.);
      }
      this->iv_t0[pos] = v;
    } else if(type==1){
      const unsigned short N = getSTensorSize(this->dimension);
      vector<real> v(N);
      this->readArrayOfSpecifiedSize(v,p);
      if(this->iv_t0.size()<pos+N){
	this->iv_t0.resize(pos+N,0.);
      }
      copy(v.begin(),v.end(),
	   this->iv_t0.begin()+pos);
    } else {
      string msg("MTest::handleInternalStateVariable : "
		 "unsupported state variable type for "
		 "internal state variable '"+n+"'");
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MTest::handleInternalStateVariable",
			     ";",p,this->fileTokens.end());
  }

  void
  MTest::handleExternalStateVariable(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(this->b.get()==0){
      string msg("MTest::handleExternalStateVariable : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    const string& evt = this->readEvolutionType(p);
    const string& n   = this->readString(p,this->fileTokens.end());
    const vector<string>& evsnames = this->b->getExternalStateVariablesNames();
    if(find(evsnames.begin(),evsnames.end(),n)==evsnames.end()){
      string msg("MTest::handleExternalStateVariable : ");
      msg += "the behaviour don't declare an external state variable '";
      msg += n+"'";
      throw(runtime_error(msg));
    }
    if(this->evs->find(n)!=this->evs->end()){
      string msg("MTest::handleExternalStateVariable : ");
      msg += "'"+n+"' already declared";
      throw(runtime_error(msg));
    }
    (*(this->evs))[n] = this->parseEvolution(evt,p);
    this->readSpecifiedToken("MTest::handleExternalStateVariable",";",p,
			     this->fileTokens.end());
  }

  void
  MTest::handleEvolution(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    const string& evt = this->readEvolutionType(p);
    const string& n   = this->readString(p,this->fileTokens.end());
    if(this->evs->find(n)!=this->evs->end()){
      string msg("MTest::handleEvolution : ");
      msg += "'"+n+"' already declared";
      throw(runtime_error(msg));
    }
    (*(this->evs))[n] = this->parseEvolution(evt,p);
    this->readSpecifiedToken("MTest::handleEvolution",";",p,
			     this->fileTokens.end());
  }

  tfel::tests::TestResult
  MTest::execute(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::math;
    using tfel::tests::TestResult;
    using tfel::math::vector;
    vector<shared_ptr<MTestConstraint> >::const_iterator pc;
    vector<string>::const_iterator p;
    vector<real>::const_iterator pt;
    vector<real>::const_iterator pt2;
    vector<shared_ptr<UTest> >::iterator ptest;
    TestResult tr;
    const unsigned short N = getSTensorSize(this->dimension);
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev2;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev3;
    map<string,shared_ptr<MTestEvolution> >::const_iterator pev4;
    //! stiffness matrix
    matrix<real> K;
    // residual
    vector<real> r;
    // vector of unknows at 
    // the beginning of the
    // previous time step.
    vector<real> u_1;
    // vector of unknows at 
    // the beginning of the
    // time step.
    vector<real> u0;
    // vector of unknows at 
    // the end of the
    // time step
    vector<real> u1;
    // material properties at
    // the beginning of the
    // time step
    vector<real> mprops0;
    // material properties at
    // the end of the
    // time step
    vector<real> mprops1;
    // internal variables at
    // the beginning of the
    // previous time step
    vector<real> iv_1;
    // internal variables at
    // the beginning of the
    // time step
    vector<real> iv0;
    // internal variables at
    // the end of the
    // time step
    vector<real> iv1;
    // external variables at
    // the beginning of the
    // time step
    vector<real> esv0;
    // external variables increments
    vector<real> desv;
    // stresses at the previous of the time step
    stensor<3u,real> s_1(0.);
    // stresses at the beginning of the time step
    stensor<3u,real> s0(0.);
    // stiffness tensor
    matrix<real> Kt(N,N);
    // prediction tensor
    matrix<real> Kp(N,N);
    // getting the names of the materials properties
    std::vector<string> mpnames;
    if(this->b.get()==0){
      string msg("MTest::execute : ");
      msg += "no behaviour defined";
      throw(runtime_error(msg));
    }
    mpnames = this->b->getMaterialPropertiesNames();
    checkIfDeclared(mpnames,*(this->evs),"material property");
    // getting the names of the internal state variables
    std::vector<string> ivnames;
    ivnames = this->b->getInternalStateVariablesNames();
    // getting the names of the external state variables
    std::vector<string> esvnames;
    esvnames = this->b->getExternalStateVariablesNames();
    checkIfDeclared(esvnames,*(this->evs),"external state variable");
    // getting the total number of unknowns
    size_t s = N;
    for(pc =this->constraints.begin();
	pc!=this->constraints.end();++pc){
      const MTestConstraint& c = *(*pc);
      s += c.getNumberOfLagrangeMultipliers();
    }
    // permuation matrix
    Permutation<matrix<real>::size_type> p_lu(s);
    // temporary vector used by the LUSolve::exe function
    vector<real> x(s);
    // castem acceleration algorithm
    vector<real> ca_u0;
    vector<real> ca_u1;
    vector<real> ca_u2;
    vector<real> ca_r0;
    vector<real> ca_r1;
    vector<real> ca_r2;
    vector<real> ca_r;
    vector<real> ca_n0;
    vector<real> ca_n1;
    vector<real> ca_tmp0;
    vector<real> ca_tmp1;
    // allocating memory
    if(this->useCastemAcceleration){
      ca_u0.resize(s,0.);      
      ca_u1.resize(s,0.);
      ca_u2.resize(s,0.);
      ca_r0.resize(s,0.);
      ca_r1.resize(s,0.);
      ca_r2.resize(s,0.);
      ca_n0.resize(s,0.);
      ca_n1.resize(s,0.);
      ca_tmp0.resize(s,0.);
      ca_tmp1.resize(s,0.);
      ca_r.resize(s,0.);
    }
    u_1.resize(s,0.);
    u0.resize(s,0.);
    u1.resize(s);
    r.resize(s);
    K.resize(s,s);
    mprops0.resize(mpnames.size());
    mprops1.resize(mpnames.size());
    iv_1.resize(this->b->getInternalStateVariablesSize(this->dimension),0.);
    iv0.resize(iv_1.size());
    iv1.resize(iv0.size());
    esv0.resize(esvnames.size());
    desv.resize(esvnames.size());
    this->b->allocate(N,iv0.size());
    // setting the intial  values of strains
    copy(this->e_t0.begin(),this->e_t0.end(),u_1.begin());
    u0 = u_1;
    // setting the intial  values of stresses
    copy(this->s_t0.begin(),this->s_t0.end(),s0.begin());
    // getting the initial values of internal state variables
    if(this->iv_t0.size()>iv0.size()){
      string msg("MTest::execute : the number of initial values declared "
		 "by the user for the internal state variables exceeds the "
		 "number of internal state variables declared by the behaviour");
      throw(runtime_error(msg));
    }
    copy(this->iv_t0.begin(),this->iv_t0.end(),iv_1.begin());
    copy(iv_1.begin(),iv_1.end(),iv0.begin());
    this->iv_t0.clear();
    // integrating over the loading path
    pt  = this->times.begin();
    pt2 = pt+1;
    // thermal expansion reference temperature
    real Tref(293.15);
    pev = this->evs->find("ThermalExpansionReferenceTemperature");
    if(pev!=this->evs->end()){
      const MTestEvolution& ev = *(pev->second);
      if(!ev.isConstant()){
	string msg("MTest::execute : 'ThermalExpansionReferenceTemperature' "
		   "must be a constant evolution");
	throw(runtime_error(msg));
      }
      Tref = ev(0);
    }
    // options selected
    if(getVerboseMode()>=VERBOSE_LEVEL1){
      ostream& log = getLogStream();
      if(this->useCastemAcceleration){
	log << "mtest : castem acceleration selected" << endl;
      }
      if(this->ppolicy==LINEARPREDICTION){
	log << "mtest : using linear prediction" << endl;
      } else if(this->ppolicy==ELASTICPREDICTION){
	log << "mtest : prediction using elastic stiffness" << endl;
      } else if(this->ppolicy==TANGENTOPERATORPREDICTION){
	log << "mtest : prediction using tangent operator" << endl;
      } else {
	if(this->ppolicy!=NOPREDICTION){
	  string msg("MTest::execute : "
		     "internal error, unsupported "
		     "prediction policy");
	  throw(runtime_error(msg));
	}	    
	log << "mtest : no prediction" << endl;
      }
    }
    // post-processing
    unsigned short cnbr = 2;
    this->out << "# first column : time" << endl;
    for(unsigned short i=0;i!=N;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the total strain" << endl;
      ++cnbr;
    }
    for(unsigned short i=0;i!=N;++i){
      this->out << "# " << cnbr << " column : " << i+1 << "th component of the stresses" << endl;
      ++cnbr;
    }
    const std::vector<string>& ivdes =
      this->b->getInternalStateVariablesDescriptions(this->dimension);
    if(ivdes.size()!=iv0.size()){
      string msg("MTest::execute : internal error "
		 "(the number of descriptions given by "
		 "the mechanical behaviour don't match "
		 "the number of internal state variables)");
      throw(runtime_error(msg));
    }
    for(std::vector<string>::size_type i=0;i!=ivdes.size();++i){
      this->out << "# " << cnbr << " column : " << ivdes[i] << endl;
      ++cnbr;
    }
    this->printOutput(*pt,N,u0,s0,iv0);
    bool first = true;
    real a(0);
    unsigned short period = 1u;
    // previous time step
    real dt_1(real(0));
    while(pt2!=this->times.end()){
      // allowing subdivisions of the time step
      const real te = *pt2;
      real t  = *pt;
      real dt = te-t;
      unsigned short subStep = 0;
      while((abs(te-t)>0.5*dt)&&
	    (subStep!=this->mSubSteps)){
	unsigned short i,j;
	// stresses at the end of the time step
	stensor<3u,real> s1(real(0));
	// strain at the beginning of the time step
	stensor<3u,real> e0(real(0));
	// thermal expansion at the beginning of the time step
	stensor<3u,real> e_th0(real(0));
	// thermal expansion at the end of the time step
	stensor<3u,real> e_th1(real(0));
	// strain increment
	stensor<3u,real> de(real(0));
	// evaluations of the materials properties at the end of the
	// time step
	for(i=0,p=mpnames.begin();p!=mpnames.end();++p,++i){
	  const MTestEvolution& ev = *(this->evs->at(*p));
	  mprops1[i] = ev(t+dt);
	}
	for(i=0,p=esvnames.begin();p!=esvnames.end();++p,++i){
	  const MTestEvolution& ev = *(this->evs->at(*p));
	  const real evt = ev(t);
	  esv0[i] = evt;
	  desv[i] = ev(t+dt)-evt;
	}
	// thermal expansion (isotropic case)
	pev   = this->evs->find("Temperature");
	pev2  = this->evs->find("ThermalExpansion");
	if((pev!=this->evs->end())&&
	   (pev2!=this->evs->end())){
	  const MTestEvolution& T_ev = *(pev->second);
	  const MTestEvolution& a_ev = *(pev2->second);
	  const real eth0 = a_ev(t)*(T_ev(t)-Tref);
	  const real eth1 = a_ev(t+dt)*(T_ev(t+dt)-Tref);
	  for(i=0;i!=3;++i){
	    e_th0[i] = eth0;
	    e_th1[i] = eth1;
	  }
	}
	pev2  = this->evs->find("ThermalExpansion1");
	pev3  = this->evs->find("ThermalExpansion2");
	pev4  = this->evs->find("ThermalExpansion3");
	if((pev!=this->evs->end())&&
	   ((pev2!=this->evs->end())||
	    (pev3!=this->evs->end())||
	    (pev4!=this->evs->end()))){
	  if((pev2==this->evs->end())||
	     (pev3==this->evs->end())||
	     (pev4==this->evs->end())){
	    string msg("MTest::execute : at least one of the three "
		       "thermal expansion coefficient is defined and "
		       "at least one is not");
	    throw(runtime_error(msg));
	  }
	  const MTestEvolution& T_ev  = *(pev->second);
	  const MTestEvolution& a1_ev = *(pev2->second);
	  const MTestEvolution& a2_ev = *(pev3->second);
	  const MTestEvolution& a3_ev = *(pev4->second);
	  if(this->dimension==1u){
	    e_th0[0u] = a1_ev(t)*(T_ev(t)-Tref);
	    e_th1[0u] = a1_ev(t+dt)*(T_ev(t+dt)-Tref);
	    e_th0[1u] = a2_ev(t)*(T_ev(t)-Tref);
	    e_th1[1u] = a2_ev(t+dt)*(T_ev(t+dt)-Tref);
	    e_th0[2u] = a3_ev(t)*(T_ev(t)-Tref);
	    e_th1[2u] = a3_ev(t+dt)*(T_ev(t+dt)-Tref);
	  } else if((this->dimension==2u)||
		    (this->dimension==3u)){
	    // thermal expansion tensors in the material referential
	    e_th0[0u] = a1_ev(t)*(T_ev(t)-Tref);
	    e_th1[0u] = a1_ev(t+dt)*(T_ev(t+dt)-Tref);
	    e_th0[1u] = a2_ev(t)*(T_ev(t)-Tref);
	    e_th1[1u] = a2_ev(t+dt)*(T_ev(t+dt)-Tref);
	    e_th0[2u] = a3_ev(t)*(T_ev(t)-Tref);
	    e_th1[2u] = a3_ev(t+dt)*(T_ev(t+dt)-Tref);
	    // backward rotation matrix
	    tmatrix<3u,3u,real> brm = transpose(this->rm);
	    e_th0.changeBasis(brm);
	    e_th1.changeBasis(brm);
	  } else {
	    string msg("MTest::execute : invalid dimension");
	    throw(runtime_error(msg));
	  }
	}
	// strain at the beginning of the time step
	for(i=0;i!=N;++i){
	  e0[i] = u0[i]-e_th0[i];
	}
	// resolution
	bool converged = false;
	unsigned short iter = 0;
	if(getVerboseMode()>=VERBOSE_LEVEL1){
	  ostream& log = getLogStream();
	  log << "resolution from " << t << " to " << t+dt << endl;
	}
	real ne  = 0.;  // norm of the increment of the strains
	real nep = 0.;  // norm of the increment of the strains at the
		        // previous iteration
	real nep2 = 0.; // norm of the increment of the strains two
		        // iterations before...
	/* prediction */
	if(this->ppolicy==LINEARPREDICTION){
	  if(period>1){
	    u1  = u0+(u0-u_1)*dt/dt_1;
	    iv1 = iv0+(iv0-iv_1)*dt/dt_1;
	    s1  = s0+(s0-s_1)*dt/dt_1;
	  }
	} else if((this->ppolicy==ELASTICPREDICTION)||
		  (this->ppolicy==SECANTOPERATORPREDICTION)||
		  (this->ppolicy==TANGENTOPERATORPREDICTION)){
	  // evaluations of the materials properties at the beginning
	  // of the time step
	  for(i=0,p=mpnames.begin();p!=mpnames.end();++p,++i){
	    const MTestEvolution& ev = *(this->evs->at(*p));
	    mprops0[i] = ev(t);
	  }
	  fill(Kp.begin(),Kp.end(),0.);
	  bool sp(true);
	  if(this->ppolicy==ELASTICPREDICTION){
	    sp = this->b->computePredictionOperator(Kp,this->rm,e0,
						    s0,mprops0,iv0,esv0,
						    this->hypothesis,
						    MTestStiffnessMatrixType::ELASTIC);
	  } else if (this->ppolicy==SECANTOPERATORPREDICTION){
	    sp = this->b->computePredictionOperator(Kp,this->rm,e0,
						    s0,mprops0,iv0,esv0,
						    this->hypothesis,
						    MTestStiffnessMatrixType::SECANTOPERATOR);
	  } else if (this->ppolicy==TANGENTOPERATORPREDICTION){
	    sp = this->b->computePredictionOperator(Kp,this->rm,e0,
						    s0,mprops0,iv0,esv0,
						    this->hypothesis,
						    MTestStiffnessMatrixType::TANGENTOPERATOR);
	  }
	  if(sp){
	    fill(K.begin(),K.end(),0.);
	    fill(r.begin(),r.end(),0.);
	    u1 = u0;
	    for(i=0;i!=N;++i){
	      r(i) += s0(i);
	      for(j=0;j!=N;++j){
		K(i,j)+=Kp(i,j);
		// free dilatation treatment
		r(i) -= Kp(i,j)*(e_th1[j]-e_th0[j]);
	      }
	    }
	    if(first){
	      a = *(max_element(K.begin(),K.end()));
	      first = false;
	    }
	    unsigned short pos = N;
	    for(pc =this->constraints.begin();
		pc!=this->constraints.end();++pc){
	      const MTestConstraint& c = *(*pc);
	      c.setValues(K,r,u0,u1,pos,
			  this->dimension,t,dt,a);
	      pos = static_cast<unsigned short>(pos+c.getNumberOfLagrangeMultipliers());
	    }
	    LUSolve::exe(K,r,x,p_lu);
	    u1 -= r;
	  } else {
	    if(getVerboseMode()>VERBOSE_QUIET){
	      ostream& log = getLogStream();
	      log << "MTest::execute : behaviour compute prediction matrix failed" << endl;
	    }
	    u1  = u0;
	    s1  = s0;
	    iv1 = iv0;
	  }
	} else {
	  if(this->ppolicy != NOPREDICTION){
	    string msg("MTest::execute : "
		       "internal error, unsupported "
		       "prediction policy");
	    throw(runtime_error(msg));
	  }	    
	  u1  = u0;
	  s1  = s0;
	  iv1 = iv0;
	}
	/* resolution */
	bool cont = true;
	while((!converged)&&(iter!=this->iterMax)&&(cont)){
	  ++iter;
	  nep2 = nep;
	  nep  = ne;
	  fill(K.begin(),K.end(),0.);
	  fill(Kt.begin(),Kt.end(),0.);
	  fill(r.begin(),r.end(),0.);
	  for(i=0;i!=N;++i){
	    de[i] = u1[i]-e_th1[i]-u0[i]+e_th0[i];
	  }
	  const bool bi = this->b->integrate(Kt,s1,iv1,this->rm,e0,
					     de,s0,mprops1,iv0,esv0,desv,
					     this->hypothesis,dt,
					     this->ktype);
	  if(bi){
	    for(i=0;i!=N;++i){
	      r(i) += s1(i);
	      for(j=0;j!=N;++j){
		K(i,j)+=Kt(i,j);
	      }
	    }
	    if(first){
	      a = *(max_element(K.begin(),K.end()));
	      first = false;
	    }
	    unsigned short pos = N;
	    for(pc =this->constraints.begin();
		pc!=this->constraints.end();++pc){
	      const MTestConstraint& c = *(*pc);
	      c.setValues(K,r,u0,u1,pos,
			  this->dimension,t,dt,a);
	      pos = static_cast<unsigned short>(pos+c.getNumberOfLagrangeMultipliers());
	    }
	    if(getVerboseMode()>=VERBOSE_DEBUG){
	      ostream& log = getLogStream();
	      for(i=0;i!=K.getNbRows();++i){
		for(j=0;j!=K.getNbCols();++j){
		  log << K(i,j) << " ";
		}
		log << endl;
	      }
	      log << endl;
	    }
	    if(this->useCastemAcceleration){
	      ca_r = r;
	    }
	    real nr = 0.;
	    for(i=0;i!=N;++i){
	      nr += abs(r(i));
	    }
	    LUSolve::exe(K,r,x,p_lu);
	    u1 -= r;
	    ne = 0;
	    for(i=0;i!=N;++i){
	      ne += abs(r(i));
	    }
	    if(getVerboseMode()>=VERBOSE_LEVEL2){
	      ostream& log = getLogStream();
	      log << "iteration " << iter << " : " << ne << " " 
		  << nr << " (";
	      for(i=0;i!=N;){
		log << u1(i);
		if(++i!=N){
		  log << " ";
		}
	      }
	      log << ")" << endl;
	    }
	    converged = (ne<this->eeps)&&(nr<this->seps);
	    for(pc =this->constraints.begin();
		(pc!=this->constraints.end())&&(converged);++pc){
	      const MTestConstraint& c = *(*pc);
	      converged = c.checkConvergence(N,u1,s1,
					     this->eeps,
					     this->seps,
					     t,dt);
	    }
	    if(this->ppolicy == NOPREDICTION){
	      converged = (converged) && (iter>1);
	    }
	    if((!converged)&&(this->useCastemAcceleration)){
	      const real ca_eps = 100*(this->seps*numeric_limits<real>::epsilon());
	      ca_u0.swap(ca_u1);
	      ca_u1.swap(ca_u2);
	      ca_r0.swap(ca_r1);
	      ca_r1.swap(ca_r2);
	      ca_u2 = u1;
	      ca_r2 = ca_r;
	      if((iter>=this->cat)&&((iter-this->cat)%this->cap==0)){
		if(getVerboseMode()>=VERBOSE_LEVEL1){
		  ostream& log = getLogStream();
		  log << "castem acceleration convergence" << endl;
		}
		bool c  = true;
		ca_tmp0 = ca_r1-ca_r0;
		ca_tmp1 = ca_r2-ca_r0;
		const real nr0 = norm(ca_tmp0);
		c = nr0>ca_eps;
		if(c){
		  ca_n0   = ca_tmp0/nr0;
		  const real ntmp1  = ca_tmp1|ca_n0;
		  ca_tmp1          -= ntmp1*ca_n0;
		  const real nr1    = norm(ca_tmp1);
		  c = nr1>0.1*abs(ntmp1);
		  if(c){
		    ca_n1      =   ca_tmp1/nr1;
		    const real ca_p0 = -(ca_r0|ca_n0);
		    const real ca_p1 = -(ca_r0|ca_n1);
		    // La projection du vecteur nul est
		    // donnée par
		    // (1-ca_c0-ca_c1)*r0+ca_c0*ca_r1+ca_c1*r2;
		    // avec - ca_c0 = (ca_p0/nr0-ca_p1/nr1)
		    //      - ca_c1 = ca_p1/nr1
		    // maintenant on applique la même relation
		    // linéaire
		    // aux inconnues..
		    const real ca_c0 = (ca_p0/nr0-ca_p1/nr1);
		    const real ca_c1 = ca_p1/nr1;
		    u1 = (1-ca_c0-ca_c1)*ca_u0+ca_c0*ca_u1+ca_c1*ca_u2;
		  } else {
		    // the previous iterations were (almost) colinear
		    const real ca_c0 = -(ca_r0|ca_n0)/nr0;
		    u1 = (1-ca_c0)*ca_u0+ca_c0*ca_u1;
		  }
		}
	      }
	    }
	  } else {
	    if(getVerboseMode()>VERBOSE_QUIET){
	      ostream& log = getLogStream();
	      log << "MTest::execute : behaviour intregration failed" << endl;
	    }
	    cont = false;
	  }
	}
	if((iter==this->iterMax)||(!cont)){
	  // no convergence
	  ++subStep;
	  if(subStep==this->mSubSteps){
	    string msg("MTest::execute : behaviour "
		       "maximum number of sub stepping reached");
	    throw(runtime_error(msg));
	  }
	  if(getVerboseMode()>=VERBOSE_LEVEL1){
	    ostream& log = getLogStream();
	    log << "non convergence, dividing time "
		<< "step by two" << endl << endl;
	  }
	  dt *= 0.5;
	  u1  = u0;
	  s1  = s0;
	  iv1 = iv0;
	} else {
	  if(getVerboseMode()>=VERBOSE_LEVEL1){
	    if(iter==1u){
	      ostream& log = getLogStream();
	      log << "convergence, after one iteration "
		  << endl << endl;
	    } else {
	      ostream& log = getLogStream();
	      if((iter>=3)&&
		 (nep >100*numeric_limits<real>::min())&&
		 (nep2>100*numeric_limits<real>::min())){
		log << "convergence, after " << iter << " iterations, "
		    << "order " << std::log(ne/nep)/std::log(nep/nep2)
		    << endl << endl;
	      } else {
		log << "convergence, after " << iter << " iterations, "
		    << "order undefined"
		    << endl << endl;
	      }
	    }
	  }
	  // testing
	  for(ptest=this->tests.begin();ptest!=this->tests.end();++ptest){
	    UTest& test = *(*ptest);
	    test.check(u1,s1,iv1,t,dt,period);
	  }
	  // post-processing
	  this->printOutput(t+dt,N,u1,s1,iv1);
	  // update variables
	  u_1  = u0;
	  s_1  = s0;
	  iv_1 = iv0;
	  dt_1 = dt;
	  u0   = u1;
	  s0   = s1;
	  iv0  = iv1;
	  t+=dt;
	  ++period;
	}
      }
      ++pt;
      ++pt2;
    }
    for(ptest=this->tests.begin();ptest!=this->tests.end();++ptest){
      const UTest& test = *(*ptest);
      tr.append(test.getResults());
    }
    return tr;
  }

  real
  MTest::readDouble(TokensContainer::const_iterator& p)
  {
    using namespace std;
    using tfel::utilities::Token;
    using tfel::utilities::shared_ptr;
    using tfel::math::Evaluator;
    this->checkNotEndOfLine("MTest::readDouble",p,
			    this->fileTokens.end());
    real v(0);
    if(p->flag==Token::String){
      const string&f = this->readString(p,this->fileTokens.end());
      Evaluator ev(f);
      const vector<string>& vn = ev.getVariablesNames();
      vector<string>::const_iterator pv;
      for(pv=vn.begin();pv!=vn.end();++pv){
	map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
	pev = this->evs->find(*pv);
	if(pev==this->evs->end()){
	  string msg("MTest::readDouble : "
		     "no evolution named '"+*pv+"' defined");
	  throw(runtime_error(msg));
	}
	const MTestEvolution& e = *(pev->second);
	if(!e.isConstant()){
	  string msg("MTest::readDouble : formulae '"+f+"' shall "
		     "only depend on constant evolutions "
		     "(evolution '"+*pv+"' is not constant");
	  throw(runtime_error(msg));
	}
	ev.setVariableValue(*pv,e(0));
      }
      v = ev.getValue();
    } else {
      v = CxxTokenizer::readDouble(p,this->fileTokens.end());
    }
    return v;
  } // end of MTest::readDouble

  real
  MTest::readTime(TokensContainer::const_iterator& p)
  {
    using namespace std;
    return this->readDouble(p);
  } // end of MTest::readTime

  void
  MTest::readArrayOfSpecifiedSize(std::vector<real>& v,
				  TokensContainer::const_iterator& p)
  {
    using namespace std;
    vector<real>::size_type i = 0;
    this->readSpecifiedToken("MTest::readArrayOfSpecifiedSize","{",p,
			     this->fileTokens.end());
    while(i!=v.size()){
      v[i] = this->readDouble(p);
      if(++i!=v.size()){
	this->readSpecifiedToken("MTest::readArrayOfSpecifiedSize",",",p,
				 this->fileTokens.end());
      }
    }
    this->readSpecifiedToken("MTest::readArrayOfSpecifiedSize","}",p,
			     this->fileTokens.end());
  } // end of MTest::readArrayOfSpecifiedSize

  void
  MTest::printOutput(const real t,
		     const unsigned short N,
		     const tfel::math::vector<real>& u,
		     const tfel::math::stensor<3u,real>& s,
		     const tfel::math::vector<real>& iv)
  {
    using namespace std;
    unsigned short i;
    this->out << t << " ";
    for(i=0;i!=N;++i){
      this->out << u(i) << " ";
    }
    for(i=0;i!=N;++i){
      this->out << s(i) << " ";
    }
    copy(iv.begin(),iv.end(),
	 ostream_iterator<real>(this->out," "));
    this->out << endl;
  }
  
  tfel::utilities::shared_ptr<MTestEvolution>
  MTest::parseEvolution(const std::string& t,
			TokensContainer::const_iterator& p)
  {
    using namespace std;
    using namespace tfel::utilities;
    shared_ptr<MTestEvolution> ev;
    this->checkNotEndOfLine("MTest::parseEvolution",p,
			    this->fileTokens.end());
    if(t.empty()||t=="evolution"){
      if(p->value=="{"){
	vector<real> tvalues;
	vector<real> values;
	this->readSpecifiedToken("MTest::parseEvolution","{",p,
				 this->fileTokens.end());
	this->checkNotEndOfLine("MTest::parseEvolution",p,
				this->fileTokens.end());
	while(p->value!="}"){
	  tvalues.push_back(this->readTime(p));
	  this->readSpecifiedToken("MTest::parseEvolution",":",p,
				   this->fileTokens.end());
	  values.push_back(this->readDouble(p));
	  this->checkNotEndOfLine("MTest::parseEvolution",p,
				  this->fileTokens.end());
	  if(p->value==","){
	    ++p;
	    this->checkNotEndOfLine("MTest::parseEvolution",p,
				    this->fileTokens.end());
	    if(p->value=="}"){
	      string msg("MTest::parseEvolution : ");
	      msg += "unexpected token '}'";
	      throw(runtime_error(msg));
	    }
	  } else {
	    if(p->value!="}"){
	      string msg("MTest::parseEvolution : ");
	      msg += "unexpected token '"+p->value+"', expected ',' or '}'";
	      throw(runtime_error(msg));
	    }
	  }
	}
	this->readSpecifiedToken("MTest::parseEvolution","}",p,
				 this->fileTokens.end());
	ev = shared_ptr<MTestEvolution>(new MTestLPIEvolution(tvalues,values));
      } else {
	const real s = this->readDouble(p);
	ev = shared_ptr<MTestEvolution>(new MTestConstantEvolution(s));
      }
    } else if(t=="function"){
      const string& f = this->readString(p,this->fileTokens.end());
      ev = shared_ptr<MTestEvolution>(new MTestFunctionEvolution(f,this->evs));
    } else {
      string msg("MTest::parseEvolution : ");
      msg += "invalid evolution type '"+t+"'";
      throw(runtime_error(msg));
    }
    return ev;
  } // end of MTest::parseEvolution

  std::string
  MTest::readEvolutionType(TokensContainer::const_iterator& p)
  {
    using namespace std;
    this->checkNotEndOfLine("MTest::readEvolutionType",p,
			    this->fileTokens.end());
    if(p->value!="<"){
      return "";
    }
    this->readSpecifiedToken("MTest::parseEvolution","<",p,
			     this->fileTokens.end());
    this->checkNotEndOfLine("MTest::readEvolutionType",p,
			    this->fileTokens.end());
    string evt = p->value;
    ++p;
    this->readSpecifiedToken("MTest::parseEvolution",">",p,
			     this->fileTokens.end());
    return evt;
  } // end of MTest::readEvolutionType

  void
  MTest::declareVariable(const std::string& v)
  {
    using namespace std;
    if(find(this->vnames.begin(),
	    this->vnames.end(),v)!=this->vnames.end()){
      string msg("MTest::declareVariable : ");
      msg += "variable '"+v+"' already declared";
      throw(runtime_error(msg));
    }
    this->vnames.push_back(v);
  }
  
  void
  MTest::declareVariables(const std::vector<std::string>& v)
  {
    using namespace std;
    vector<string>::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      this->declareVariable(*p);
    }
  }

} // end namespace mfront
