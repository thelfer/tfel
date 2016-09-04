/*!
 * \file   ConvertLogarithmicStrainTangentOperator.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   29 août 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cstdlib>
#include<iostream>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/Math/tensor.hxx"
#include"TFEL/Material/Lame.hxx"
#include"TFEL/Math/ST2toST2/ConvertLogarithmicStrainTangentOperator.hxx"

struct ConvertLogarithmicStrainTangentOperator
  : public tfel::tests::TestCase
{
  
  ConvertLogarithmicStrainTangentOperator()
    : tfel::tests::TestCase("TFEL/Math","ConvertToTangentModuli")
  {} // end of ConvertToTangentModuli
  
  tfel::tests::TestResult
  execute()
  {
    //    this->test<1>();
    // this->test<2>();
    // this->test<3>();
    return this->result;
  } // end of execute
private:
  
  
}; // end of ConvertLogarithmicStrainTangentOperator

TFEL_TESTS_GENERATE_PROXY(ConvertLogarithmicStrainTangentOperator,
			  "ConvertLogarithmicStrainTangentOperator");

int main()
{
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ConvertToTangentModuli.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
