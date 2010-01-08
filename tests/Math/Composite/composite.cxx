/*!
 * \file   composite.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cmath>
#include<cstdlib>

#include"Utilities/Name.hxx"
#include"Math/functions.hxx"
#include"Math/tvector.hxx"
#include"Math/composite.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  typedef CompositeTraits<composite<float,tvector<3,short> > >::Items Items;
  typedef tfel::math::internals::ScalarCompositeComputeResultList<double,Items,OpMinus>::type Res;

  composite<double,tvector<3,double> > c(12.,tvector<3,double>(13));
  composite<double,tvector<3,double> > c2(9.,tvector<3,double>(1.4));
  composite<double,tvector<3,double> > c3;

  assert((Name<ResultType<composite<double,float>,
	 composite<float,float>,OpPlus>::type>::getName()==
	 "composite<double,float>")); 
  assert((Name<ResultType<composite<double,tvector<3,int> >,
	 composite<float,tvector<3,float> >,OpPlus>::type>::getName()==
	 "composite<double,tvector<3,float>>")); 

  assert(tfel::meta::TypeListContainsInvalidType<Res>::cond);

  assert((Name<ResultType<double,composite<float,tvector<3,short> >,OpMult>::type>::getName()==
	  "composite<double,tvector<3,double>>"));
  assert((Name<ResultType<composite<float,tvector<3,short> >,double,OpMult>::type>::getName()==
	  "composite<double,tvector<3,double>>"));
  assert((Name<UnaryResultType<composite<float,tvector<3,short> >,OpNeg>::type>::getName()==
	  "composite<float,tvector<3,short>>"));

  c3 = c2+c*2;
  assert(abs(c3.getComponent<0>()-33.)<1.e-14);
  assert(abs(c3.getComponent<1>()(0)-27.4)<1.e-14);
  assert(abs(c3.getComponent<1>()(1)-27.4)<1.e-14);
  assert(abs(c3.getComponent<1>()(2)-27.4)<1.e-14);

  c3 = -(c2+c*2);
  assert(abs(c3.getComponent<0>()+33.)<1.e-14);
  assert(abs(c3.getComponent<1>()(0)+27.4)<1.e-14);
  assert(abs(c3.getComponent<1>()(1)+27.4)<1.e-14);
  assert(abs(c3.getComponent<1>()(2)+27.4)<1.e-14);


  c3 = sin(c2+c*2);
  assert(abs(c3.getComponent<0>()-sin(33.))<1.e-14);
  assert(abs(c3.getComponent<1>()(0)-sin(27.4))<1.e-14);
  assert(abs(c3.getComponent<1>()(1)-sin(27.4))<1.e-14);
  assert(abs(c3.getComponent<1>()(2)-sin(27.4))<1.e-14);
  
  return EXIT_SUCCESS;
}
