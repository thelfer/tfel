/*!
 * \file   inner_product.cxx
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
  using tfel::fsalgo::inner_product;
  using tfel::fsalgo::copy;

  int A1[] = {1, 2, 3};
  int A2[] = {4, 1, -2};
  const int N1 = sizeof(A1) / sizeof(int);

  cout << "The inner product of A1 and A2 is " 
       << inner_product<N1>::exe(A1, A2, 0)
       << endl;

  return EXIT_SUCCESS;
}
