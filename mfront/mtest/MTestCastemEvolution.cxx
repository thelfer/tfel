/*! 
 * \file  MTestCastemEvolution.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avril 2013
 */

#include<stdexcept>

#include"MFront/MTestCastemEvolution.hxx"

namespace mfront{

  MTestCastemEvolution::MTestCastemEvolution(const std::string& l,
					     const std::string& fn,
					     const tfel::utilities::shared_ptr<std::map<std::string,
											tfel::utilities::shared_ptr<MTestEvolution> > >& evm_)
    : evm(evm_)
  {
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->f = elm.getCastemFunction(l,fn);
    elm.getCastemFunctionVariables(this->vnames,l,fn);
    this->args.resize(this->vnames.size());
  } // end of MTestCastemEvolution::MTestCastemEvolution

  real
  MTestCastemEvolution::operator()(const real t) const
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string>::size_type i;
    for(i=0;i!=this->vnames.size();++i){
      map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
      pev = evm->find(vnames[i]);
      if(pev==evm->end()){
	string msg("MTestCastemEvolution::operator() : ");
	msg += "can't evaluate argument '"+vnames[i]+"'";
	throw(runtime_error(msg));
      } else {
	const MTestEvolution& ev = *(pev->second);
	args[i] = ev(t);
      }
    }
    return this->f(&args[0]);
  } // end of MTestCastemEvolution::operator()

  bool
  MTestCastemEvolution::isConstant(void) const
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string>::size_type i;
    for(i=0;i!=this->vnames.size();++i){
      map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
      pev = evm->find(vnames[i]);
      if(pev==evm->end()){
	string msg("MTestCastemEvolution::operator() : ");
	msg += "can't evaluate argument '"+vnames[i]+"'";
	throw(runtime_error(msg));
      } else {
	const MTestEvolution& ev = *(pev->second);
	if(!ev.isConstant()){
	  return false;
	}
      }
    }
    return true;
  } // end of MTestCastemEvolution::isConstant
  
  MTestCastemEvolution::~MTestCastemEvolution()
  {} // end of MTestCastemEvolution::~MTestCastemEvolution

}
