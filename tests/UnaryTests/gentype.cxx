/*!
 * \file   gentype.cxx
 * \brief  This file tests some functionalities of the GenType class.
 * \author Helfer Thomas
 * \date   23 Apr 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<string>

#include<cassert>

#include"Utilities/GenType.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  
  typedef GenType<int,string> MyGenType;

  MyGenType test;
  MyGenType test2("toto");
  MyGenType test3(test2);

  test = string("tata");
  assert(test.is<string>());
  assert(test.get<string>()=="tata");
  assert(static_cast<const string&>(test)=="tata");
  assert(test.getString()=="tata");

  test2 = string("toto");
  assert(test2.is<string>());
  assert(test2.get<string>()=="toto");
  assert(static_cast<const string&>(test2)=="toto");
  assert(test2.getString()=="toto");

  test3 = string("toto");
  assert(test3.is<string>());
  assert(test3.get<string>()=="toto");
  assert(static_cast<const string&>(test3)=="toto");
  assert(test3.getString()=="toto");

  test.set(12);
  assert(test.is<int>());
  assert(test.get<int>()==12);
  assert(static_cast<const int&>(test.get<int>())==12);
  assert(test.getInt()==12);

  test2 = test;
  assert(test2.is<int>());
  assert(test2.get<int>()==12);
  assert(static_cast<const int&>(test2.get<int>())==12);
  assert(test2.getInt()==12);

#ifdef TFEL_VERBOSE
  cout << "sizeof(GenType)         : " << sizeof(MyGenType) << endl;
  cout << "sizeof(int)             : " << sizeof(int) << endl;
  cout << "sizeof(string)          : " << sizeof(string) << endl;
  cout << "sizeof(unsigned short)  : " << sizeof(unsigned short) << endl;
#endif /* TFEL_VERBOSE */

  return EXIT_SUCCESS;
}
