/*!
 * \file   test9.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Oct 2006
 */

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Utilities/Name.hxx"
#include"Math/General/ObjectObjectRandomAccessConstIterator.hxx"
#include"Math/vector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using tfel::math::vector;

  vector<double> v1(4);
  vector<double> v2(4);

  v1(0) = 12.12;
  v1(1) = 12.234;
  v1(2) = 22.4322;
  v1(3) = 2.2112;
  v2(0) = 2.;
  v2(1) = 4.;
  v2(2) = 5.;
  v2(3) = 6.;

  ObjectObjectRandomAccessConstIterator<vector<double>,vector<double>,OpMinus> begin(v1.begin(),v2.begin());
  ObjectObjectRandomAccessConstIterator<vector<double>,vector<double>,OpMinus> end(v1.end(),v2.end());
  ObjectObjectRandomAccessConstIterator<vector<double>,vector<double>,OpMinus> p;

  cout << name(p) << endl;

  for(p=begin;p!=end;++p){
    cout << *p << " ";
  }
  cout << endl;
  
  copy(begin,end,ostream_iterator<double>(cout," "));
  cout << endl;

  return EXIT_SUCCESS;
}
