/*!
 * \file   for_each.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#include<iostream>
#include<cstdlib>
#include<functional>

#include"FSAlgorithm/FSAlgorithm.hxx"

template<class T> 
struct print 
  : public std::unary_function<T, void>
{
  print(std::ostream& out) : os(out), count(0) {}
  void operator() (T x) { os << x << ' '; ++count; }
  std::ostream& os;
  int count;
};

int main()
{
  using namespace std;
  using namespace tfel::fsalgo;
  using tfel::fsalgo::for_each;

  int A[] = {1, 4, 2, 8, 5, 7};
  const int N = sizeof(A) / sizeof(int);

  print<int> P = for_each<N>::exe(A, print<int>(cout));
  cout << endl << P.count << " objects printed." << endl;

  return EXIT_SUCCESS;
}
