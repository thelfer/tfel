/*!
 * \file   tests/Material/Eshelby.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/06/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif NDEBUG

#include <limits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Material/Eshelby.hxx"
#include "TFEL/Material/EshelbyAnisotropic.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct EshelbyAnisotropicTest final : public tfel::tests::TestCase {
  EshelbyAnisotropicTest()
      : tfel::tests::TestCase("TFEL/Material", "EshelbyAnisotropic") {}  // end of EshelbyAnisotropicTest
  
  tfel::tests::TestResult execute() override {
    this->template compile_Eshelby_tensors<double, true>();
    this->template errors_Eshelby_tensors<double, true>();
    this->template test_Eshelby_tensors<double, true>();
    //this->template test1<long double, false>();
    //this->template test1<float, true>();
    //this->template test1<double, true>();
    //this->template test1<long double, true>();
    return this->result;
  }

 
//tests compilation of the Eshelby tensor functions
private:
  template <typename NumericType, bool use_qt>
  void compile_Eshelby_tensors() {
    using lg =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    const auto nu = real{0.3};

    using namespace tfel::material;
    {const auto S = computeSphereEshelbyTensor(nu);}
    
    {const auto S1 = computeAxisymmetricalEshelbyTensor(nu,real{0.5});
    const auto S2 = computeAxisymmetricalEshelbyTensor(nu,real{10});
    const auto S3 = computeAxisymmetricalEshelbyTensor(nu,real{1});
    }
    
    {const auto S1 = computeEshelbyTensor(nu,lg{1},lg{3},lg{2});
    const auto S2 = computeEshelbyTensor(nu,lg{1},lg{1},lg{2});
    const auto S3 = computeEshelbyTensor(nu,lg{1},lg{1},lg{1});
    }
 };
    
//must return a warning
private:
  template <typename NumericType, bool use_qt>
  void errors_Eshelby_tensors() {
    using lg =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    const auto nu = real{0.3};
    
    using namespace tfel::material;
    {//const auto S1 = computeSphereEshelbyTensor(real{1});
    //const auto S2 = computeAxisymmetricalEshelbyTensor(nu,real{0});
    //const auto S3 = computeAxisymmetricalEshelbyTensor(nu,real{-10});
    //const auto S4 = computeEshelbyTensor<real,lg>(nu,lg{-1},lg{3},lg{2});
    //const auto S5 = computeEshelbyTensor<real,lg>(nu,lg{-1},lg{-1},lg{1});
    }
  };
    
 
 //These functions must return the same thing : the value displayed must be one
  private:
  template <typename NumericType, bool use_qt>
  void test_Eshelby_tensors() {
    using lg =
        typename tfel::config::Types<1u, NumericType, use_qt>::length;
    using real = NumericType;
    static constexpr auto eps = std::numeric_limits<real>::epsilon();
    const auto nu = real{0.3};
    
    
    using namespace tfel::material;
    {const auto S1 = computeSphereEshelbyTensor(nu);
    const auto S2 = computeAxisymmetricalEshelbyTensor(nu,real{1});
    const auto S3 = computeEshelbyTensor(nu,lg{1},lg{2},lg{2});
    const auto S4 = computeEshelbyTensor(nu,lg{3},lg{3},lg{3});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (std::abs(S1(i,j)-S2(i,j))<eps);
    		//std::cout << S1(i,j) << " " << S2(i,j) << " " << value << '\n';
    		value = value and (std::abs(S1(i,j)-S3(i,j))<eps);
    		//std::cout << "13" << value << '\n';
    		value = value and (std::abs(S1(i,j)-S4(i,j))<eps);
    		//std::cout << "14" << value << '\n';
    	};
    };
   if (value){
    std::cout << "Eshelby 3d ok" << '\n';
    }
    else{std::cout << "Eshelby 3d does not work" << '\n';};
    }
    
    {const auto S1 = computeAxisymmetricalEshelbyTensor(nu,real{10});
    const auto S2 = computeEshelbyTensor(nu,lg{30},lg{3},lg{3});
    const auto S3 = computeEshelbyTensor(nu,lg{3},lg{3},lg{30});
    bool value = true;
    for (int i :{0,1,2,3,4,5}){
    	for (int j:{0,1,2,3,4,5}){
    		value = value and (std::abs(S1(i,j)-S2(i,j))<eps);
    		value = value and (std::abs(S1(i,j)-S3(i,j))<eps);
    	};
    };
    if (value){
    std::cout << "Eshelby 3d bis ok" << '\n';
    }
    else{std::cout << "Eshelby 3d bis does not work" << '\n';};
    }
    
    //These functions must return a very similar thing
    
    
    
    
 
  };
  
  
};  // end of struct EshelbyAnisotropicTest

TFEL_TESTS_GENERATE_PROXY(EshelbyAnisotropicTest, "EshelbyAnisotropic");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("EshelbyAnisotropic.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
