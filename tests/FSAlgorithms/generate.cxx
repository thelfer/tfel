/*!
 * \file   tests/FSAlgorithms/generate.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#include<cstdlib>
#include<vector>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

int main()
{
  using namespace std;
  using tfel::fsalgo::generate;
  vector<int> V(10);
  generate<10>::exe(V.begin(), rand);
  return EXIT_SUCCESS;
}
