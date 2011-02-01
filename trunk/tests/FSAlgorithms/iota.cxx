/*!
 * \file   iota.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

int main()
{
  using namespace tfel::fsalgo;

  int A1[3];
  iota<3>::exe(A1,12);
  assert(A1[0]==12);
  assert(A1[1]==13);
  assert(A1[2]==14);

  return EXIT_SUCCESS;
}
