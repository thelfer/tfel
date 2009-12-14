/*!
 * \file   test_FSalgorithm2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   15 aoû 2006
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

  vector<unsigned short> v;
  unsigned short i;
  for(i=0;i<11;++i){
    v.push_back(i);
  }

  tfel::fsalgo::copy<1u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<2u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<3u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<4u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<5u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<6u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<7u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<8u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<9u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
  tfel::fsalgo::copy<10u>::exe(v.begin(),ostream_iterator<unsigned short>(cout," "));
  cout << endl;
    
  return EXIT_SUCCESS;
}
