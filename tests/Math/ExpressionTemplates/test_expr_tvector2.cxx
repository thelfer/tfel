/*!
 * \file   tests/Math/ExpressionTemplates/test_expr_tvector2.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
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

#include<cassert>
#include<cstdlib>
#include<limits>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/qt.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  
  tvector<4,length> v1(length(12.)); 
  tvector<4,length> v2(length(14.)); 
  tvector<4,length> v3;
  
  v3 = v1+v2;

  assert(abs(v3(0)-length(26.))<1.e-14);
  assert(abs(v3(1)-length(26.))<1.e-14);
  assert(abs(v3(2)-length(26.))<1.e-14);
  assert(abs(v3(3)-length(26.))<1.e-14);

  return EXIT_SUCCESS;
}
