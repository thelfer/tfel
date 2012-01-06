/*!
 * \file   tvector.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
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
