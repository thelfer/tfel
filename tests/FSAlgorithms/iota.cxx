/*!
 * \file   iota.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#include<iostream>
#include<cstdlib>
#include<functional>
#include<vector>

#include"FSAlgorithm/FSAlgorithm.hxx"

int main()
{
  using namespace std;
  using namespace tfel::fsalgo;
  using tfel::fsalgo::iota;
  using tfel::fsalgo::copy;

  int A1[] = {1, 2, 3};
  const int N1 = sizeof(A1) / sizeof(int);

  iota<N1>::exe(A1,12);
  copy<N1>::exe(A1,ostream_iterator<int>(cout," "));
  cout << endl;

  return EXIT_SUCCESS;
}
