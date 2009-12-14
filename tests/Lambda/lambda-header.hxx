/*!
 * \file   lambda.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_LAMBDA_HEADER_HXX_
#define _LIB_TFEL_LAMBDA_HEADER_HXX_ 

#include<iostream>
#include<cstdlib>
#include"Math/tvector.hxx"
#include"Math/lambda.hxx"
#include"Math/functions.hxx"

template<typename T>
void printLambdaTraits(const T&);

template<typename T>
void printLambdaTraits(const T&)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::meta;
  typedef typename LambdaTraits<T>::Variables Vars;
  cout << "dimension : "  << LambdaTraits<T>::dimension << endl;
  cout << "variables : "  << TypeListGetNames<Vars>::exe() << endl;
}

#endif /* _LIB_TFEL_LAMBDA_HEADER_HXX */

