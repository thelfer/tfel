/*!
 * \file   accumulate.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */


#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cstdlib>
#include<cassert>
#include<functional>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

int main()
{
  using namespace std;
  using tfel::fsalgo::accumulate;
  int A[] = {1, 2, 3, 4, 5};
  const int N = sizeof(A) / sizeof(int);
  assert(accumulate<N>::exe(A, 0)==15);
  assert(accumulate<N>::exe(A, 1, multiplies<int>())==120);
  return EXIT_SUCCESS;
}
