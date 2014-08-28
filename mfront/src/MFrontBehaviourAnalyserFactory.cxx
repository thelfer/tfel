/*!
 * \file   mfront/src/MFrontBehaviourAnalyserFactory.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>
#include<cassert>

#include"MFront/MFrontBehaviourAnalyserFactory.hxx"

namespace mfront{

  MFrontBehaviourAnalyserFactory&
  MFrontBehaviourAnalyserFactory::getMFrontBehaviourAnalyserFactory()
  {
    static MFrontBehaviourAnalyserFactory f;
    return f;
  } // end of MFrontBehaviourAnalyserFactory::getMFrontBehaviourAnalyserFactory

  MFrontBehaviourAnalyserFactory::AnalyserCreatorsContainer&
  MFrontBehaviourAnalyserFactory::getAnalyserCreatorsMap(void)
  {
    static AnalyserCreatorsContainer map;
    return map;
  } // end of MFrontBehaviourAnalyserFactory::getAnalyserCreatorsMap

  MFrontBehaviourAnalyserFactory::AnalyserContainer&
  MFrontBehaviourAnalyserFactory::getAnalysersMap(void)
  {
    static AnalyserContainer map;
    return map;
  } // end of MFrontBehaviourAnalyserFactory::getAnalysersMap

  MFrontBehaviourAnalyserFactory::MFrontBehaviourAnalyserFactory()
  {}

  std::vector<std::string>
  MFrontBehaviourAnalyserFactory::getRegistredAnalysers(void)
  {
    using namespace std;
    AnalyserCreatorsContainer::const_iterator p;
    vector<string> res;
    for(p  = this->getAnalyserCreatorsMap().begin();
	p != this->getAnalyserCreatorsMap().end();++p){
      res.push_back(p->first);
    }
    return res;
  }

  void 
  MFrontBehaviourAnalyserFactory::registerAnalyserCreator(const std::string& i,
							  const MFrontBehaviourAnalyserFactory::AnalyserCreator f)
  {
    using namespace std;
    AnalyserCreatorsContainer& imap = this->getAnalyserCreatorsMap();
    if(imap.find(i)!=imap.end()){
      string msg("MFrontBehaviourAnalyserFactory::registerAnalyserCreator : ");
      msg += "analyser creator '"+i+"' already declared";
      throw(runtime_error(msg));
    }
    imap.insert(make_pair(i,f));
  }

  void
  MFrontBehaviourAnalyserFactory::registerAnalyserAlias(const std::string& i,
							 const std::string& a)
  {
    using namespace std;
    AliasContainer& amap = this->getAliasesMap();
    if(amap.find(a)!=amap.end()){
      string msg("MFrontBehaviourAnalyserFactory::registerAnalyserCreator : ");
      msg += "interface alias '"+a+"' already declared";
      throw(runtime_error(msg));
    }
    amap.insert(make_pair(a,i));
  }

  MFrontBehaviourAnalyser* 
  MFrontBehaviourAnalyserFactory::getAnalyserPtr(const std::string& analyserName)
  {
    using namespace std;
    AnalyserCreatorsContainer::iterator p;
    AliasContainer::iterator p2;
    AnalyserContainer::iterator m;
    MFrontBehaviourAnalyser *i;
    m = this->getAnalysersMap().find(analyserName);
    if(m==this->getAnalysersMap().end()){
      p2 = this->getAliasesMap().find(analyserName);
      if(p2==this->getAliasesMap().end()){
	string msg = "MFrontBehaviourAnalyserFactory::createNewAnalyser : no analyser named '";
	msg += analyserName+"'.\n";
	msg += "Available analyser are : \n";
	for(p2  = this->getAliasesMap().begin();
	    p2 != this->getAliasesMap().end();++p2){
	  msg += p2->first + " ";
	}
	throw(runtime_error(msg));
      }
      p = this->getAnalyserCreatorsMap().find(p2->second);
      assert(p!=this->getAnalyserCreatorsMap().end());
      AnalyserCreator c = p->second;
      i = c();
      this->getAnalysersMap().insert(make_pair(analyserName,i));
    } else {
      i = m->second;
    }
    return i;
  }

  void
  MFrontBehaviourAnalyserFactory::clear(void)
  {
    AnalyserContainer::iterator m;
    for(m = this->getAnalysersMap().begin();m!= this->getAnalysersMap().end();++m){
      delete m->second;
    }
    this->getAnalysersMap().clear();
  } // end of MFrontBehaviourAnalyserFactory::clear(void)

  MFrontBehaviourAnalyserFactory::~MFrontBehaviourAnalyserFactory()
  {
    assert(this->getAnalysersMap().empty());
  } // end of MFrontBehaviourAnalyserFactory::~MFrontBehaviourAnalyserFactory()
  
  void
  MFrontBehaviourAnalyserFactory::reset(void)
  {
    AnalyserContainer::iterator m;
    for(m = this->getAnalysersMap().begin();m!= this->getAnalysersMap().end();++m){
      m->second->reset();
    }
  } // end of MFrontBehaviourAnalyserFactory::reset

  MFrontBehaviourAnalyserFactory::AliasContainer&
  MFrontBehaviourAnalyserFactory::getAliasesMap(void)
  {
    static AliasContainer map;
    return map;
  } // end of MFrontBehaviourAnalyserFactory::getAliasesMap

} // end of namespace mfront
