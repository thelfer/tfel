/*!
 * \file   accumulate.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#include<iostream>
#include<cstdlib>
#include<functional>

#include"FSAlgorithm/FSAlgorithm.hxx"

int main()
{
  using namespace std;
  using namespace tfel::fsalgo;

  int A[] = {1, 2, 3, 4, 5};
  const int N = sizeof(A) / sizeof(int);

  cout << "The sum of all elements in A is " 
       << accumulate<N>::exe(A, 0)
       << endl;

  cout << "The product of all elements in A is "
       << accumulate<N>::exe(A, 1, multiplies<int>())
       << endl;

  return EXIT_SUCCESS;
}
