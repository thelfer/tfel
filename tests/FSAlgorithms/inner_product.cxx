/*!
 * \file   inner_product.cxx
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

#include<iostream>

int main()
{
  using namespace tfel::fsalgo;
  int A1[] = {1, 2, 4};
  int A2[] = {4, 1, -2};
  std::cout << inner_product<3>::exe(A1, A2, 0) << std::endl;
  assert(inner_product<3>::exe(A1, A2, 0)==-2);
  return EXIT_SUCCESS;
}
