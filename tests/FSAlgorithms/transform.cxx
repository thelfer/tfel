/*!
 * \file   transform.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#include<iostream>
#include<cstdlib>
#include<vector>
#include<iterator>

#include"FSAlgorithm/FSAlgorithm.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::fsalgo;

  const unsigned short N = 10;
  vector<int> V1(N);
  vector<int> V2(N);
  vector<int> V3(N);
  double A[N];
  tfel::fsalgo::iota<N>::exe(A, 1);
  tfel::fsalgo::transform<N>::exe(A, A, negate<double>());
  tfel::fsalgo::copy<N>::exe(A,ostream_iterator<double>(cout," "));
  cout << endl;

  tfel::fsalgo::iota<N>::exe(V1.begin(), 1);
  tfel::fsalgo::fill<N>::exe(V2.begin(), 75);
  
  tfel::fsalgo::transform<N>::exe(V1.begin(), V2.begin(),V3.begin(),plus<int>());
  
  tfel::fsalgo::copy<N>::exe(V1.begin(),ostream_iterator<int>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<N>::exe(V2.begin(),ostream_iterator<int>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<N>::exe(V3.begin(),ostream_iterator<int>(cout," "));
  cout << endl;

  return EXIT_SUCCESS;
}
