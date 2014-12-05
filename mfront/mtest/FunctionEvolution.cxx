/*! 
 * \file  mfront/mtest/FunctionEvolution.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"MTest/FunctionEvolution.hxx"

namespace mfront{

  FunctionEvolution::FunctionEvolution(const std::string& f_,
						 const std::shared_ptr<std::map<std::string,
											std::shared_ptr<Evolution> > >& evm_)
    : evm(evm_),
      f(f_)
  {} // end of FunctionEvolution::FunctionEvolution

  real
  FunctionEvolution::operator()(const real t) const
  {
    using namespace std;
    const vector<string>& args = this->f.getVariablesNames();
    vector<string>::size_type i;
    for(i=0;i!=args.size();++i){
      if(args[i]=="t"){
	this->f.setVariableValue("t",t);
      } else {
	map<string,shared_ptr<Evolution> >::const_iterator pev;
	pev = evm->find(args[i]);
	if(pev==evm->end()){
	  string msg("FunctionEvolution::operator() : ");
	  msg += "can't evaluate argument '"+args[i]+"'";
	  throw(runtime_error(msg));
	} else {
	  const Evolution& ev = *(pev->second);
	  this->f.setVariableValue(args[i],ev(t));
	}
      }
    }
    return this->f.getValue();
  } // end of FunctionEvolution::operator()

  bool
  FunctionEvolution::isConstant(void) const
  {
    using namespace std;
    const vector<string>& args = this->f.getVariablesNames();
    vector<string>::size_type i;
    for(i=0;i!=args.size();++i){
      if(args[i]=="t"){
	return false;
      } else {
	map<string,shared_ptr<Evolution> >::const_iterator pev;
	pev = evm->find(args[i]);
	if(pev==evm->end()){
	  string msg("FunctionEvolution::operator() : ");
	  msg += "can't evaluate argument '"+args[i]+"'";
	  throw(runtime_error(msg));
	} else {
	  const Evolution& ev = *(pev->second);
	  if(!ev.isConstant()){
	    return false;
	  }
	}
      }
    }
    return true;
  } // end of FunctionEvolution::isConstant

  void  FunctionEvolution::setValue(const real,
					 const real)
  {
    using namespace std;
    string msg("FunctionEvolution::setValue : "
	       "this method does not makes sense for function evolution");
    throw(runtime_error(msg));
  }
  
  FunctionEvolution::~FunctionEvolution()
  {} // end of FunctionEvolution::~FunctionEvolution

}
