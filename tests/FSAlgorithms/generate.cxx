/*!
 * \file   generate.cxx
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
  using tfel::fsalgo::generate;
  using tfel::fsalgo::copy;
  vector<int> V(10);

  copy<10>::exe(V.begin(),ostream_iterator<int>(cout," "));
  cout << endl;
  generate<10>::exe(V.begin(), rand);
  copy<10>::exe(V.begin(),ostream_iterator<int>(cout," "));
  cout << endl;

  return EXIT_SUCCESS;
}
