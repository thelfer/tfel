/*! 
 * \file  mfront/mtest/ReferenceFileComparisonTest.cxx
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

#include"MTest/ReferenceFileComparisonTest.hxx"

namespace mfront{

  ReferenceFileComparisonTest::ReferenceFileComparisonTest(const tfel::utilities::shared_ptr<tfel::utilities::TextData> d,
								     const std::string& v,
								     const unsigned int c,
								     const MTest::UTest::TestedVariable t,
								     const unsigned short p,
								     const real eps_)
    : values(d->getColumn(c)),
      name(v),
      type(t),
      pos(p),
      eps(eps_)
  {} // ReferenceFileComparisonTest::ReferenceFileComparisonTest

  void
  ReferenceFileComparisonTest::check(const tfel::math::vector<real>& e,
					  const tfel::math::vector<real>& s,
					  const tfel::math::vector<real>& iv,
					  const real t,
					  const real dt,
					  const unsigned int p)
  {
    using namespace std;
    using namespace tfel::utilities;
    using tfel::tests::TestResult;
    real v(0);
    if(this->type==MTest::UTest::INTERNALSTATEVARIABLE){
      v = iv(pos);
    } else if(this->type==MTest::UTest::DRIVINGVARIABLE){
      v = e(pos);
    } else if(this->type==MTest::UTest::THERMODYNAMICFORCE){
      v = s(pos);
    } else {
      string msg("ReferenceFileComparisonTest::check : "
		 "internal error (unsuported type of variable");
      throw(runtime_error(msg));
    }
    if(p>=this->values.size()){
      ostringstream msg;
      msg << "ReferenceFileComparisonTest::check : comparison for variable '"
	  << this->name << "' failed for time '" << t+dt << "' "
	  << "(reference value is not available for period  '" << p << "')";
      this->results.append(TestResult(false,msg.str()));
    } else {
      const real err = abs(v-this->values[p]);
      if(err>this->eps){
	ostringstream msg;
	msg << "ReferenceFileComparisonTest::check : comparison for variable '"
	    << this->name << "' failed for time '" << t+dt << "' "
	    << "(computed value: '" << v << "', "
	    << "expected value: '" << this->values[p] << "', "
	    << "error: '" << err << "', criterium '"
	    << this->eps << "')";
	this->results.append(TestResult(false,msg.str()));
      }
    }
  } // end of ReferenceFileComparisonTest::check

  tfel::tests::TestResult
  ReferenceFileComparisonTest::getResults() const
  {
    using namespace std;
    using tfel::tests::TestResult;
    if(this->results.success()){
      ostringstream msg;
      msg << "ReferenceFileComparisonTest::check : comparison for variable '"
	  << this->name << "' was successfull for all times (" 
	  << "criterium '"<< this->eps << "')";
      return TestResult(true,msg.str());
    }
    return this->results;
  }

  ReferenceFileComparisonTest::~ReferenceFileComparisonTest()
  {} // end of ReferenceFileComparisonTest::~ReferenceFileComparisonTest

}
