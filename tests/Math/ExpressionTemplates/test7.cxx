/*!
 * \file   test7.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Oct 2006
 */

#include<iostream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<iterator>

#include"Math/General/ScalarObjectRandomAccessConstIterator.hxx"
#include"Math/General/BasicOperations.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  vector<double> v(4);

  v[0] = 12.12;
  v[1] = 12.234;
  v[2] = 22.4322;
  v[3] = 2.2112;

  ScalarObjectRandomAccessConstIterator<int,vector<double>,OpMult> begin(2,v.begin());
  ScalarObjectRandomAccessConstIterator<int,vector<double>,OpMult> end(2,v.end());
  ScalarObjectRandomAccessConstIterator<int,vector<double>,OpMult> p;

  ObjectScalarRandomAccessConstIterator<vector<double>,int,OpMult> begin2(v.begin(),3);
  ObjectScalarRandomAccessConstIterator<vector<double>,int,OpMult> end2(v.end(),3);
  ObjectScalarRandomAccessConstIterator<vector<double>,int,OpMult> p2;

  for(p=begin;p!=end;++p){
    cout << *p << " ";
  }
  cout << endl;
  
  copy(begin,end,ostream_iterator<double>(cout," "));
  cout << endl;

  for(p2=begin2;p2!=end2;++p2){
    cout << *p2 << " ";
  }
  cout << endl;
  
  copy(begin2,end2,ostream_iterator<double>(cout," "));
  cout << endl;


  return EXIT_SUCCESS;
}
