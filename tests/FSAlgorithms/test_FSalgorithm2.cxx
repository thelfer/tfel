/*!
 * \file   test_FSalgorithm2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   15 aoû 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<iterator>
#include<cassert>
#include<cstdlib>
#include<vector>

#include"FSAlgorithm/FSAlgorithm.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::fsalgo;

  vector<unsigned short> v(10);
  vector<unsigned short> v2(10);
  vector<unsigned short>::size_type i;

  tfel::fsalgo::iota<10u>::exe(v.begin(),0);
  tfel::fsalgo::copy<10u>::exe(v.begin(),v2.begin());
  for(i=0;i!=10;++i){
    assert(v2[i]==i);
  }
    
  return EXIT_SUCCESS;
}
