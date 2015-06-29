/*! 
 * \file   tests/Math/stensor.cxx
 * \brief
 * \author Helfer Thomas
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

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/stensor.hxx"

int main(void){
  using namespace std;  
  using namespace tfel::math;  
  typedef stensor<1> stensor1;
  typedef stensor<1,int> stensor2;
  const stensor1 v1 = {4.,1.,25.};
  const stensor2 v2 = {2,1,5};
  const auto v3 = v1 + v2;
  assert(abs(v3(0)-6)<1.e-14);
  assert(abs(v3(1)-2)<1.e-14);
  assert(abs(v3(2)-30)<1.e-14);
  return EXIT_SUCCESS;
}
