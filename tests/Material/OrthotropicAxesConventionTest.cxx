/*!
 * \file   OrthotropicAxisesConventionTest.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Material/OrthotropicAxesConvention.hxx"

struct OrthotropicAxesConventionTest final
  : public tfel::tests::TestCase
{
  OrthotropicAxesConventionTest()
    : tfel::tests::TestCase("TFEL/Material",
			    "OrthotropicAxesConventionTest")
  {} // end of MyTest
  tfel::tests::TestResult
  execute() override
  {
    this->test1<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
		OrthotropicAxesConvention::UNDEFINED>();
    this->test1<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
		OrthotropicAxesConvention::PIPE>();
    this->test1<ModellingHypothesis::AXISYMMETRICAL,
		OrthotropicAxesConvention::UNDEFINED>();
    this->test1<ModellingHypothesis::AXISYMMETRICAL,
		OrthotropicAxesConvention::PIPE>();
    this->test1<ModellingHypothesis::GENERALISEDPLANESTRAIN,
		OrthotropicAxesConvention::UNDEFINED>();
    this->test2<ModellingHypothesis::GENERALISEDPLANESTRAIN,
		OrthotropicAxesConvention::PIPE>();
    this->test1<ModellingHypothesis::PLANESTRAIN,
		OrthotropicAxesConvention::UNDEFINED>();
    this->test2<ModellingHypothesis::PLANESTRAIN,
		OrthotropicAxesConvention::PIPE>();
    this->test1<ModellingHypothesis::PLANESTRESS,
		OrthotropicAxesConvention::UNDEFINED>();
    this->test2<ModellingHypothesis::PLANESTRESS,
		OrthotropicAxesConvention::PIPE>();
    this->test1<ModellingHypothesis::TRIDIMENSIONAL,
		OrthotropicAxesConvention::UNDEFINED>();
    this->test1<ModellingHypothesis::TRIDIMENSIONAL,
		OrthotropicAxesConvention::PIPE>();
    return this->result;
  } // end of execute()
 private:
  using ModellingHypothesis = tfel::material::ModellingHypothesis;
  using Hypothesis = ModellingHypothesis::Hypothesis;
  using OrthotropicAxesConvention = tfel::material::OrthotropicAxesConvention;
  template<Hypothesis h,OrthotropicAxesConvention c>
  void test1(void){
    using tfel::math::stensor;
    using namespace tfel::material;
    stensor<ModellingHypothesisToSpaceDimension<h>::value,int> s(0);
    s[0]= 1; s[1]=2; s[2]=3;
    convert<h,c>(s);
    TFEL_TESTS_CHECK_EQUAL(s[0],1);
    TFEL_TESTS_CHECK_EQUAL(s[1],2);
    TFEL_TESTS_CHECK_EQUAL(s[2],3);
  }
  template<Hypothesis h,OrthotropicAxesConvention c>
  void test2(void){
    using tfel::math::stensor;
    using namespace tfel::material;
    stensor<ModellingHypothesisToSpaceDimension<h>::value,int> s(0);
    s[0]= 1; s[1]=2; s[2]=3;
    convert<h,c>(s);
    TFEL_TESTS_CHECK_EQUAL(s[0],1);
    TFEL_TESTS_CHECK_EQUAL(s[1],3);
    TFEL_TESTS_CHECK_EQUAL(s[2],2);
  }
};

TFEL_TESTS_GENERATE_PROXY(OrthotropicAxesConventionTest,"OrthotropicAxesConvention");

int main(void)
{
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("OrthotropicAxesConvention.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
