/*!
 * \file   gentype2.cxx
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

#include"TFEL/Utilities/GenType.hxx"

struct my_function
{
  typedef int return_type;
  static return_type
  apply(const int v)
  {
    return v*2;
  }
  static return_type
  apply(const std::string&)
  {
    return 0;
  }
};

struct my_function2
{
  typedef int return_type;
  static return_type
  apply(const int v1,
	const int v2)
  {
    return v1*v2;
  }
  template<typename T1,
	   typename T2>
  static return_type
  apply(const T1&,
	const T2&)
  {
    return 0;
  }
};

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  
  GenType<int,string> g;
  GenType<int,string> g2;
  GenType<int,string> g3;
  GenType<int,string> g4;
  g = 21;
  g2 = string("toto");
  g3 = 3;

  assert(apply<my_function>(g)==42);
  assert(apply<my_function>(g2)==0);
  assert(apply<my_function2>(g,g2)==0);
  assert(apply<my_function2>(g,g3)==63);
  //   assert(apply<my_function2>(g4,g3)==);


  return EXIT_SUCCESS;
}
