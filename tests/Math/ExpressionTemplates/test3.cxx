/*!
 * \file   test3.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"

#include"Math/General/UnaryResultType.hxx"
#include"Math/General/BasicOperations.hxx"
#include"Math/tvector.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;

  tvector<3,unsigned short> v1(1);
  tvector<3,tvector<3,unsigned short> > v2(tvector<3,unsigned short>(1));

  cout << Name<ComputeUnaryResult<tvector<3,unsigned short>,OpNeg>::Result>::getName() << endl;
  cout << Name<ComputeUnaryResult<tvector<3,unsigned short>,OpNeg>::Handle>::getName() << endl;

  cout << name(-v1) << endl;  
  cout << name(-v2) << endl;  

}
