/*! 
 * \file  MTestFunctionEvolution.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avril 2013
 */

#include<stdexcept>

#include"MFront/MTestFunctionEvolution.hxx"

namespace mfront{

  MTestFunctionEvolution::MTestFunctionEvolution(const std::string& f_,
						 const tfel::utilities::shared_ptr<std::map<std::string,
											tfel::utilities::shared_ptr<MTestEvolution> > >& evm_)
    : evm(evm_),
      f(f_)
  {} // end of MTestFunctionEvolution::MTestFunctionEvolution

  real
  MTestFunctionEvolution::operator()(const real t) const
  {
    using namespace std;
    using namespace tfel::utilities;
    using tfel::utilities::shared_ptr;
    const vector<string>& args = this->f.getVariablesNames();
    vector<string>::size_type i;
    for(i=0;i!=args.size();++i){
      if(args[i]=="t"){
	this->f.setVariableValue("t",t);
      } else {
	map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
	pev = evm->find(args[i]);
	if(pev==evm->end()){
	  string msg("MTestFunctionEvolution::operator() : ");
	  msg += "can't evaluate argument '"+args[i]+"'";
	  throw(runtime_error(msg));
	} else {
	  const MTestEvolution& ev = *(pev->second);
	  this->f.setVariableValue(args[i],ev(t));
	}
      }
    }
    return this->f.getValue();
  } // end of MTestFunctionEvolution::operator()

  bool
  MTestFunctionEvolution::isConstant(void) const
  {
    using namespace std;
    using namespace tfel::utilities;
    using tfel::utilities::shared_ptr;
    const vector<string>& args = this->f.getVariablesNames();
    vector<string>::size_type i;
    for(i=0;i!=args.size();++i){
      if(args[i]=="t"){
	return false;
      } else {
	map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
	pev = evm->find(args[i]);
	if(pev==evm->end()){
	  string msg("MTestFunctionEvolution::operator() : ");
	  msg += "can't evaluate argument '"+args[i]+"'";
	  throw(runtime_error(msg));
	} else {
	  const MTestEvolution& ev = *(pev->second);
	  if(!ev.isConstant()){
	    return false;
	  }
	}
      }
    }
    return true;
  } // end of MTestFunctionEvolution::isConstant

  void  MTestFunctionEvolution::setValue(const real,
					 const real)
  {
    using namespace std;
    string msg("MTestFunctionEvolution::setValue : "
	       "this method does not makes sense for function evolution");
    throw(runtime_error(msg));
  }
  
  MTestFunctionEvolution::~MTestFunctionEvolution()
  {} // end of MTestFunctionEvolution::~MTestFunctionEvolution

}
