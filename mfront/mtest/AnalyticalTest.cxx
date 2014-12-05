/*! 
 * \file  mfront/mtest/AnalyticalTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>

#include"MTest/Evolution.hxx"
#include"MTest/AnalyticalTest.hxx"

namespace mfront
{

  AnalyticalTest::AnalyticalTest(const std::string& f_,
					   const std::string& v,
					   const MTest::UTest::TestedVariable t,
					   const unsigned short p,
					   std::shared_ptr<std::map<std::string,
										std::shared_ptr<Evolution> > > evm_,
					   const real eps_)
    : f(f_),
      name(v),
      type(t),
      pos(p),
      evm(evm_),
      eps(eps_)
  {
    this->vnames = this->f.getVariablesNames();
  } // AnalyticalTest::AnalyticalTest

  void
  AnalyticalTest::check(const tfel::math::vector<real>& e,
			     const tfel::math::vector<real>& s,
			     const tfel::math::vector<real>& iv,
			     const real t,
			     const real dt,
			     const unsigned int)
  {
    using namespace std;
    using tfel::tests::TestResult;
    vector<string>::const_iterator p;
    for(p=this->vnames.begin();p!=this->vnames.end();++p){
      if(*p=="t"){
	this->f.setVariableValue("t",t+dt);
      } else{
	map<string,shared_ptr<Evolution> >::const_iterator pev;
	pev = this->evm->find(*p);
	if(pev==this->evm->end()){
	  string msg("AnalyticalTest::check : "
		     "no evolution named '"+*p+"' defined");
	  throw(runtime_error(msg));
	}
	const Evolution& ev = *(pev->second);
	this->f.setVariableValue(*p,ev(t+dt));
      }
    }
    real v(0);
    if(this->type==MTest::UTest::INTERNALSTATEVARIABLE){
      v = iv(pos);
    } else if(this->type==MTest::UTest::DRIVINGVARIABLE){
      v = e(pos);
    } else if(this->type==MTest::UTest::THERMODYNAMICFORCE){
      v = s(pos);
    } else {
      string msg("AnalyticalTest::check : "
		 "internal error (unsuported type of variable");
      throw(runtime_error(msg));
    }
    const real err = abs(v-this->f.getValue());
    if(err>this->eps){
      ostringstream msg;
      msg << "AnalyticalTest::check : comparison for variable '"
	  << this->name << "' failed for time '" << t+dt << "' "
	  << "(computed value: '" << v << "', "
	  << "expected value: '" << this->f.getValue() << "', "
	  << "error: '" << err << "', criterium '"
	  << this->eps << "')";
      this->results.append(TestResult(false,msg.str()));
    }
  } // end of AnalyticalTest::check

  tfel::tests::TestResult
  AnalyticalTest::getResults() const
  {
    using namespace std;
    using tfel::tests::TestResult;
    if(this->results.success()){
      ostringstream msg;
      msg << "AnalyticalTest::check : comparison for variable '"
	  << this->name << "' was successfull for all times (" 
	  << "criterium '"<< this->eps << "')";
      return TestResult(true,msg.str());
    }
    return this->results;
  }

  AnalyticalTest::~AnalyticalTest()
  {} // end of AnalyticalTest::~AnalyticalTest

} // end of namespace mfront
