/*!
 * \file   transform.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<cassert>
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
  for(int i=0;i!=N;++i){
    assert(abs(A[i]-i-1)<1.e-14);
  }

  tfel::fsalgo::transform<N>::exe(A, A, negate<double>());
  for(int i=0;i!=N;++i){
    assert(abs(A[i]+i+1)<1.e-14);
  }

  tfel::fsalgo::iota<N>::exe(V1.begin(), 1);
  for(int i=0;i!=N;++i){
    assert(V1[i]==i+1);
  }

  tfel::fsalgo::fill<N>::exe(V2.begin(), 75);
  for(int i=0;i!=N;++i){
    assert(V2[i]==75);
  }
  
  tfel::fsalgo::transform<N>::exe(V1.begin(), V2.begin(),V3.begin(),plus<int>());
  for(int i=0;i!=N;++i){
    assert(V3[i]==76+i);
  }

  return EXIT_SUCCESS;
}
