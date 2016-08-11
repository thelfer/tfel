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

#include<cmath>
#include<sstream>

#include"MTest/TextDataUtilities.hxx"
#include"MTest/ReferenceFileComparisonTest.hxx"

namespace mtest{
  
  ReferenceFileComparisonTest::ReferenceFileComparisonTest(const tfel::utilities::TextData& d,
							   const unsigned int c,
							   const std::string& v,
							   const MTest::UTest::TestedVariable t,
							   const unsigned short p,
							   const real eps_)
    : values(d.getColumn(c)),
      name(v),
      type(t),
      pos(p),
      eps(eps_)
  {} // ReferenceFileComparisonTest::ReferenceFileComparisonTest

  ReferenceFileComparisonTest::ReferenceFileComparisonTest(const tfel::utilities::TextData& d,
							   const EvolutionManager& e,
							   const std::string& f,
							   const std::string& v,
							   const MTest::UTest::TestedVariable t,
							   const unsigned short p,
							   const real eps_)
    : values(eval(d,e,f)),
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
    auto throw_if = [](const bool c, const std::string& m){
      if(c){throw(std::runtime_error("ReferenceFileComparisonTest::check: "+m));}
    };
    real v(0);
    if(this->type==MTest::UTest::INTERNALSTATEVARIABLE){
      v = iv(pos);
    } else if(this->type==MTest::UTest::DRIVINGVARIABLE){
      v = e(pos);
    } else if(this->type==MTest::UTest::THERMODYNAMICFORCE){
      v = s(pos);
    } else {
      throw(std::runtime_error("ReferenceFileComparisonTest::check: "
			       "internal error (unsuported type of variable"));
    }
    if(p>=this->values.size()){
      std::ostringstream msg;
      msg << "ReferenceFileComparisonTest::check : comparison for variable '"
	  << this->name << "' failed for time '" << t+dt << "' "
	  << "(reference value is not available for period  '" << p << "')";
      this->results.append({false,msg.str()});
    } else {
      throw_if(!std::isfinite(v),"invalid result for '"+this->name+"'");
      const real err = std::abs(v-this->values[p]);
      if(err>this->eps){
	std::ostringstream msg;
	msg << "ReferenceFileComparisonTest::check : comparison for variable '"
	    << this->name << "' failed for time '" << t+dt << "' "
	    << "(computed value: '" << v << "', "
	    << "expected value: '" << this->values[p] << "', "
	    << "error: '" << err << "', criterion '"
	    << this->eps << "')";
	this->results.append({false,msg.str()});
      }
    }
  } // end of ReferenceFileComparisonTest::check

  tfel::tests::TestResult
  ReferenceFileComparisonTest::getResults() const
  {
    if(this->results.success()){
      std::ostringstream msg;
      msg << "ReferenceFileComparisonTest::check : comparison for variable '"
	  << this->name << "' was successfull for all times (" 
	  << "criterion '"<< this->eps << "')";
      return {true,msg.str()};
    }
    return this->results;
  }

  ReferenceFileComparisonTest::~ReferenceFileComparisonTest()
  {} // end of ReferenceFileComparisonTest::~ReferenceFileComparisonTest

}
