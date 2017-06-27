/*!
 * \file   LogarithmicStrainHandlerTest.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   25 juin 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<utility>
#include<iostream>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Material/LogarithmicStrainHandler.hxx"

struct LogarithmicStrainHandlerTest final
  : public tfel::tests::TestCase
{
  LogarithmicStrainHandlerTest()
    : tfel::tests::TestCase("TFEL/Material",
			    "LogarithmicStrainHandlerTest")
  {} // end of LogarithmicStrainHandlerTest
  virtual tfel::tests::TestResult
  execute() override
  {
    return this->result;
  }
 private:
  template<unsigned short N,typename real>
  void check1(const int r){
    using namespace tfel::math;
    using namespace tfel::material;
    using stensor   = tfel::math::stensor<N,real>;
    using tensor    = tfel::math::tensor<N,real>;
    using st2tost2  = tfel::math::st2tost2<N,real>;
    using size_type = unsigned short;
    const real l0  = 1.09465e+11;
    const real m0  = 5.6391e+12;
    const real eps = 4.e-5*(2*m0);
    // changing the rounding mode
    std::fesetround(r);
    // Hencky-Biot law with PK2 result
    auto hb = [&l0,&m0](const LogarithmicStrainHandler<N,real>& h,
			const tensor& F) -> stensor{
      const auto e = h.getHenckyLogarithmicStrain();
      const auto T = eval(l0*trace(e)*stensor::Id()+2*m0*e);
      return h.convertToSecondPiolaKirchhoffStress(T);
    };
    auto getD = [hb](const stensor& C,const real e){
      auto D = st2tost2{};
      auto Cp = C;
      for(size_type j=0;j!=StensorDimeToSize<N>::value;++j){
	Cp[j] += e;
	const auto Sp = hb(Cp);
	Cp[j] -= 2*e;
	const auto Sm = hb(Cp);
	const auto dS = (Sp-Sm)/(2*e);
	for(size_type i=0;i!=StensorDimeToSize<N>::value;++i){
	  D(i,j)=dS(i);
	}
	Cp[j] += e;
      }
      return D;
    };
    const real v1[9u] = {1.03,0.98,1.09,0.03,-0.012,0.04,-0.028,-0.015,0.005};
    const real v2[9u] = {0.70,1.125,1.32,-0.24,-0.32,0.15,-0.14,-0.05,0.08};
    const real v3[9u] = {1.70,0.625,0.625,0.,0.,0.,0.,0.,0.};
    const real v4[9u] = {-0.625,1.70,-0.625,0.,0.,0.,0.,0.,0.};
    const real v5[9u] = {-0.625,-0.625,1.70,0.,0.,0.,0.,0.,0.};
    const real v6[9u] = {0.625,0.625,1.70,0.,0.,0.,0.,0.,0.};
    const auto F0 = tensor::Id();
    for(const tensor& F1: {tensor::Id(),tensor{v1},tensor{v2},
			   tensor{v3},tensor{v4},tensor{v5},
			   tensor{v6}}){
      const LogarithmicStrainHandler<N,real> h(F1);
      const auto  C  = computeRightCauchyGreenTensor(F1);
      const auto nD  = getD(C,5.e-7);
      const auto  D  = eval(l0*st2tost2::IxI()+2*m0*st2tost2::Id());
      const auto  S  = hb(h,F1);
      const auto aD  = h.convertToMaterialModuli(D,h.convertFromSecondPiolaKirchhoffStress(S));
      for(size_type i=0;i!=tfel::math::StensorDimeToSize<N>::value;++i){
	for(size_type j=0;j!=tfel::math::StensorDimeToSize<N>::value;++j){
	  if(std::abs(aD(i,j)-nD(i,j))>eps){
	    std::cout << i << " " << j << " "
		      << nD(i,j) << " " << " " << aD(i,j)
		      << " " << std::abs(aD(i,j)-nD(i,j))
		      << " " << std::abs(aD(i,j)-nD(i,j))/(2*m0) << std::endl;
	  }
	  TFEL_TESTS_ASSERT(std::abs(aD(i,j)-nD(i,j))<eps);	
	}
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(LogarithmicStrainHandlerTest,
			  "LogarithmicStrainHandlerTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LogarithmicStrainHandlerTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main

