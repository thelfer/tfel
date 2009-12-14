/*!
 * \file   name.cxx
 * 
 * \brief    
 *
 *
 *	
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */

#include<iostream>
#include<cstdlib>
#include<string>
#include<cassert>

#include "Utilities/Name.hxx"
#include "Math/General/Complex.hxx"

#define TEST_NAME(X)                                                                \
  assert(tfel::utilities::Name<X >::getName()==#X)
#define TEST_NAME2(X)                                                               \
  assert(tfel::utilities::Name<X*>::getName()=="pointer to type "#X)         
#define TEST_NAME3(X)                                                               \
  assert(tfel::utilities::Name<const X*>::getName()=="pointer to const type "#X)
#define TEST_NAME4(X)                                                               \
  assert(tfel::utilities::Name<X&>::getName()=="reference to type "#X)
#define TEST_NAME5(X)                                                               \
  assert(tfel::utilities::Name<const X&>::getName()=="const reference to type "#X)


int main(void)
{
  TEST_NAME(void);
  TEST_NAME(char);
  TEST_NAME(unsigned short);
  TEST_NAME(unsigned int);
  TEST_NAME(long unsigned int);
  TEST_NAME(short);
  TEST_NAME(int);
  TEST_NAME(long int);
  TEST_NAME(float);
  TEST_NAME(double);
  TEST_NAME(long double);
  TEST_NAME(tfel::math::Complex<unsigned short>);
  TEST_NAME(tfel::math::Complex<unsigned int>);
  TEST_NAME(tfel::math::Complex<long unsigned int>);
  TEST_NAME(tfel::math::Complex<short>);
  TEST_NAME(tfel::math::Complex<int>);
  TEST_NAME(tfel::math::Complex<long int>);
  TEST_NAME(tfel::math::Complex<float>);
  TEST_NAME(tfel::math::Complex<double>);
  TEST_NAME(tfel::math::Complex<long double>);

  TEST_NAME2(void);
  TEST_NAME2(char);
  TEST_NAME2(unsigned short);
  TEST_NAME2(unsigned int);
  TEST_NAME2(long unsigned int);
  TEST_NAME2(short);
  TEST_NAME2(int);
  TEST_NAME2(long int);
  TEST_NAME2(float);
  TEST_NAME2(double);
  TEST_NAME2(long double);
  TEST_NAME2(tfel::math::Complex<unsigned short>);
  TEST_NAME2(tfel::math::Complex<unsigned int>);
  TEST_NAME2(tfel::math::Complex<long unsigned int>);
  TEST_NAME2(tfel::math::Complex<short>);
  TEST_NAME2(tfel::math::Complex<int>);
  TEST_NAME2(tfel::math::Complex<long int>);
  TEST_NAME2(tfel::math::Complex<float>);
  TEST_NAME2(tfel::math::Complex<double>);
  TEST_NAME2(tfel::math::Complex<long double>);

  TEST_NAME3(void);
  TEST_NAME3(char);
  TEST_NAME3(unsigned short);
  TEST_NAME3(unsigned int);
  TEST_NAME3(long unsigned int);
  TEST_NAME3(short);
  TEST_NAME3(int);
  TEST_NAME3(long int);
  TEST_NAME3(float);
  TEST_NAME3(double);
  TEST_NAME3(long double);
  TEST_NAME3(tfel::math::Complex<unsigned short>);
  TEST_NAME3(tfel::math::Complex<unsigned int>);
  TEST_NAME3(tfel::math::Complex<long unsigned int>);
  TEST_NAME3(tfel::math::Complex<short>);
  TEST_NAME3(tfel::math::Complex<int>);
  TEST_NAME3(tfel::math::Complex<long int>);
  TEST_NAME3(tfel::math::Complex<float>);
  TEST_NAME3(tfel::math::Complex<double>);
  TEST_NAME3(tfel::math::Complex<long double>);

  TEST_NAME4(char);
  TEST_NAME4(unsigned short);
  TEST_NAME4(unsigned int);
  TEST_NAME4(long unsigned int);
  TEST_NAME4(short);
  TEST_NAME4(int);
  TEST_NAME4(long int);
  TEST_NAME4(float);
  TEST_NAME4(double);
  TEST_NAME4(long double);
  TEST_NAME4(tfel::math::Complex<unsigned short>);
  TEST_NAME4(tfel::math::Complex<unsigned int>);
  TEST_NAME4(tfel::math::Complex<long unsigned int>);
  TEST_NAME4(tfel::math::Complex<short>);
  TEST_NAME4(tfel::math::Complex<int>);
  TEST_NAME4(tfel::math::Complex<long int>);
  TEST_NAME4(tfel::math::Complex<float>);
  TEST_NAME4(tfel::math::Complex<double>);
  TEST_NAME4(tfel::math::Complex<long double>);

  TEST_NAME5(char);
  TEST_NAME5(unsigned short);
  TEST_NAME5(unsigned int);
  TEST_NAME5(long unsigned int);
  TEST_NAME5(short);
  TEST_NAME5(int);
  TEST_NAME5(long int);
  TEST_NAME5(float);
  TEST_NAME5(double);
  TEST_NAME5(long double);
  TEST_NAME5(tfel::math::Complex<unsigned short>);
  TEST_NAME5(tfel::math::Complex<unsigned int>);
  TEST_NAME5(tfel::math::Complex<long unsigned int>);
  TEST_NAME5(tfel::math::Complex<short>);
  TEST_NAME5(tfel::math::Complex<int>);
  TEST_NAME5(tfel::math::Complex<long int>);
  TEST_NAME5(tfel::math::Complex<float>);
  TEST_NAME5(tfel::math::Complex<double>);
  TEST_NAME5(tfel::math::Complex<long double>);

  std::cout << "success" << std::endl;

  return EXIT_SUCCESS;
}
