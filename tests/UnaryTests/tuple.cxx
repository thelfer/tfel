/*!
 * \file   tuple.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   24 fév 2007
 */

#include<iostream>
#include<cstdlib>
#include"Utilities/Tuple.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;

  tuple<double,string> t(12.,"toto");
  tuple<double,string> t2;

  cout << t.get<0>() << " : " << t.get<1>() << endl;
  
  t.set<0>(13.);
  t.set<1>("tata");

  cout << t.get<0>() << " : " << t.get<1>() << endl;

  t2=makeTuple(123.,std::string("titi"));
  cout << t2.get<0>() << " : " << t2.get<1>() << endl;

  cout << tuple<double,string>::getName() << endl;

  return EXIT_SUCCESS;
}
