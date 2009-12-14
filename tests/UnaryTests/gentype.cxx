/*!
 * \file   gentype.cxx
 * \brief  This file tests some functionalities of the GenType class.
 * \author Helfer Thomas
 * \date   23 Apr 2007
 */

#include<iostream>
#include<cstdlib>
#include<string>

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
  cout << "test : " << test.is<string>() << endl;
  cout << test.get<string>() << endl;
  cout << static_cast<string>(test.get<string>()) << endl;
  cout << test.getString() << endl;

  cout << "test2 : " << test2.is<string>() << endl;
  cout << test2.get<string>() << endl;
  cout << static_cast<string>(test2.get<string>()) << endl;
  cout << test2.getString() << endl;

  cout << "test3 : " << test3.is<string>() << endl;
  cout << test3.get<string>() << endl;
  cout << static_cast<string>(test3.get<string>()) << endl;
  cout << test3.getString() << endl;

  test.set(12);
  cout << "test4 : " << test.is<int>() << endl;
  cout << test.get<int>() << endl;
  cout << static_cast<int>(test.get<int>()) << endl;
  cout << test.getInt() << endl;

  test2 = test;
  cout << "test5 : " << test2.is<int>() << endl;
  cout << test2.get<int>() << endl;
  cout << static_cast<int>(test2.get<int>()) << endl;
  cout << test2.getInt() << endl;

  cout << "sizeof(GenType)         : " << sizeof(MyGenType) << endl;
  cout << "sizeof(int)             : " << sizeof(int) << endl;
  cout << "sizeof(string)          : " << sizeof(string) << endl;
  cout << "sizeof(unsigned short)  : " << sizeof(unsigned short) << endl;

  return EXIT_SUCCESS;
}
