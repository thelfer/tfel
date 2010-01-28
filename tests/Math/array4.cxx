/*!
 * \file   array4.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>
#include<algorithm>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Math/Array/ArrayRunTimeProperties.hxx"
#include"TFEL/Math/array.hxx"
#include"TFEL/Math/qt.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  {
    array<1u>::RunTimeProperties RT(4);
    
    array<1u,mass> a(RT,mass(2.));
    array<1u> b(RT,4.);
    array<1u,mass> c(RT,mass(0.));
  
    a(0) = mass(0);
    a(1) = mass(2.5);
    a(2) = mass(3.1234);
    a(3) = mass(5.231);
    
    c += a;
    c += a/b;

  }

  {
    array<2u>::RunTimeProperties RT(4,2);

    array<2u,mass> a(RT,mass(2.));
    array<2u> b(RT,4.);
    array<2u,mass> c(RT,mass(0.));
  
    a(0,0) = mass(0);
    a(0,1) = mass(2.5);
    
    c += a;
    c += a/b;
    
  }

  {
    array<3u>::RunTimeProperties RT(4,2,3);
    
    array<3u,mass> a(RT,mass(2.));
    array<3u> b(RT,4.);
    array<3u,mass> c(RT,mass(0.));
  
    a(0,0,0) = mass(0);
    a(0,0,1) = mass(2.5);
    a(0,0,2) = mass(3.1234);
    
    c += a;
    c += a/b;
  }

  return EXIT_SUCCESS;

}
