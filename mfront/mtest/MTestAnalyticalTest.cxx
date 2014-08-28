/*! 
 * \file  mfront/mtest/MTestAnalyticalTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 avril 2013
 */

#include<sstream>

#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestAnalyticalTest.hxx"

namespace mfront
{

  MTestAnalyticalTest::MTestAnalyticalTest(const std::string& f_,
					   const std::string& v,
					   const MTest::UTest::TestedVariable t,
					   const unsigned short p,
					   tfel::utilities::shared_ptr<std::map<std::string,
										tfel::utilities::shared_ptr<MTestEvolution> > > evm_,
					   const real eps_)
    : f(f_),
      name(v),
      type(t),
      pos(p),
      evm(evm_),
      eps(eps_)
  {
    this->vnames = this->f.getVariablesNames();
  } // MTestAnalyticalTest::MTestAnalyticalTest

  void
  MTestAnalyticalTest::check(const tfel::math::vector<real>& e,
			     const tfel::math::vector<real>& s,
			     const tfel::math::vector<real>& iv,
			     const real t,
			     const real dt,
			     const unsigned int)
  {
    using namespace std;
    using namespace tfel::utilities;
    using tfel::tests::TestResult;
    using tfel::utilities::shared_ptr;
    vector<string>::const_iterator p;
    for(p=this->vnames.begin();p!=this->vnames.end();++p){
      if(*p=="t"){
	this->f.setVariableValue("t",t+dt);
      } else{
	map<string,shared_ptr<MTestEvolution> >::const_iterator pev;
	pev = this->evm->find(*p);
	if(pev==this->evm->end()){
	  string msg("MTestAnalyticalTest::check : "
		     "no evolution named '"+*p+"' defined");
	  throw(runtime_error(msg));
	}
	const MTestEvolution& ev = *(pev->second);
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
      string msg("MTestAnalyticalTest::check : "
		 "internal error (unsuported type of variable");
      throw(runtime_error(msg));
    }
    const real err = abs(v-this->f.getValue());
    if(err>this->eps){
      ostringstream msg;
      msg << "MTestAnalyticalTest::check : comparison for variable '"
	  << this->name << "' failed for time '" << t+dt << "' "
	  << "(computed value: '" << v << "', "
	  << "expected value: '" << this->f.getValue() << "', "
	  << "error: '" << err << "', criterium '"
	  << this->eps << "')";
      this->results.append(TestResult(false,msg.str()));
    }
  } // end of MTestAnalyticalTest::check

  tfel::tests::TestResult
  MTestAnalyticalTest::getResults() const
  {
    using namespace std;
    using tfel::tests::TestResult;
    if(this->results.success()){
      ostringstream msg;
      msg << "MTestAnalyticalTest::check : comparison for variable '"
	  << this->name << "' was successfull for all times (" 
	  << "criterium '"<< this->eps << "')";
      return TestResult(true,msg.str());
    }
    return this->results;
  }

  MTestAnalyticalTest::~MTestAnalyticalTest()
  {} // end of MTestAnalyticalTest::~MTestAnalyticalTest

} // end of namespace mfront
