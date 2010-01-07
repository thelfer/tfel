/*!
 * \file   for_each.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cstdlib>
#include<cassert>

#include"FSAlgorithm/FSAlgorithm.hxx"

template<class T> 
struct count 
  : public std::unary_function<T, void>
{
  count()
    : c(0)
  {}
  void operator() (const T&)
  {
    ++c;
  }
  int c;
};

int main()
{
  using namespace tfel::fsalgo;
  int A[] = {1, 4, 2, 8, 5, 7};
  count<int> P = for_each<6>::exe(A,count<int>());
  assert(P.c==6);
  return EXIT_SUCCESS;
}
