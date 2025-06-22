/*!
 * \file  tests/Material/FiniteStrainBehaviourTangentOperator11.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tot2.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/T2toT2/T2toT2ConceptIO.hxx"
#include "TFEL/Math/T2toST2/T2toST2ConceptIO.hxx"
#include "TFEL/Math/ST2toST2/ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator11 final
    : public tfel::tests::TestCase {
  FiniteStrainBehaviourTangentOperator11()
      : tfel::tests::TestCase("TFEL/Material",
                              "FiniteStrainBehaviourTangentOperator11") {
  }  // end of FiniteStrainBehaviourTangentOperator11
  virtual tfel::tests::TestResult execute() override {
    using namespace tfel::material;
    FiniteStrainBehaviourTangentOperator<1u, double> Dt;
    Dt = tfel::math::st2tost2<1u, double>::Id();
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator11,
                          "FiniteStrainBehaviourTangentOperator11");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator11.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
