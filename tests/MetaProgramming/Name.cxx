/*!
 * \file   Name.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   20 jui 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>
#include<cassert>

#include"Utilities/Name.hxx"

#include"TypeTraits/Promote.hxx"
#include"Math/General/BasicOperations.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::typetraits;
  using namespace tfel::math;
  using namespace tfel::utilities;
  assert((Name<Promote<float,long double>::type>::getName()=="long double"));
  assert((Name<Promote<long double,float>::type>::getName()=="long double"));
  assert((Name<ResultType<long double,float,OpMult>::type>::getName()=="long double"));
  assert((Name<ResultType<float,long double,OpMult>::type>::getName()=="long double"));
  return EXIT_SUCCESS;
}
