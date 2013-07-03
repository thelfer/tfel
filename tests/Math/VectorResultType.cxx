/*!
 * \file   VectorResultType.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;
  using tfel::math::vector;

  vector<tvector<3> > v(3,tvector<3>(4.));
  vector<tvector<3> > v2(3,tvector<3>(2.));

  assert(name((v+v2)(2))=="VectorExpr<tvector<3,double>,MathObjectMathObjectExpr<tvector<3,double>,tvector<3,double>,+>>");

  assert(abs((v+v2)(0)(0)-6.)<1.e-14);
  assert(abs((v+v2)(0)(1)-6.)<1.e-14);
  assert(abs((v+v2)(0)(2)-6.)<1.e-14);

  assert(abs((v+v2)(1)(0)-6.)<1.e-14);
  assert(abs((v+v2)(1)(1)-6.)<1.e-14);
  assert(abs((v+v2)(1)(2)-6.)<1.e-14);

  assert(abs((v+v2)(2)(0)-6.)<1.e-14);
  assert(abs((v+v2)(2)(1)-6.)<1.e-14);
  assert(abs((v+v2)(2)(2)-6.)<1.e-14);


  return EXIT_SUCCESS;
}
