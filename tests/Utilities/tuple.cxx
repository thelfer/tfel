/*!
 * \file   tuple.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   24 fév 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<string>
#include<cmath>

#include<cassert>

#include"TFEL/Utilities/Tuple.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;

  tuple<double,string> t(12.,"toto");
  tuple<double,string> t2;

  assert(abs(t.get<0>()-12.)<1.e-14);
  assert(t.get<1>()=="toto");
  
  t.set<0>(13.);
  t.set<1>("tata");

  assert(abs(t.get<0>()-13.)<1.e-14);
  assert(t.get<1>()=="tata");

  t2=makeTuple(123.,std::string("titi"));
  assert(abs(t2.get<0>()-123.)<1.e-14);
  assert(t2.get<1>()=="titi");
  assert((tuple<double,string>::getName()=="tuple<double,std::string>"));

  return EXIT_SUCCESS;
}
