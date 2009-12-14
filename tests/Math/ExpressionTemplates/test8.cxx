/*!
 * \file   test8.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Oct 2006
 */

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Utilities/Name.hxx"
#include"Math/General/FctObjectRandomAccessConstIterator.hxx"
#include"Math/vector.hxx"
#include"Math/functions.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using tfel::math::vector;
  USING_TFEL_FUNCTIONS;

  vector<double> v(4);

  v(0) = 12.12;
  v(1) = 12.234;
  v(2) = 22.4322;
  v(3) = 2.2112;

  FctObjectRandomAccessConstIterator<vector<double>,FctSin> begin(v.begin(),sin);
  FctObjectRandomAccessConstIterator<vector<double>,FctSin> end(v.end(),sin);
  FctObjectRandomAccessConstIterator<vector<double>,FctSin> p;

  cout << name(p) << endl;

  for(p=begin;p!=end;++p){
    cout << *p << " ";
  }
  cout << endl;
  
  copy(begin,end,ostream_iterator<double>(cout," "));
  cout << endl;  

  return EXIT_SUCCESS;
}
