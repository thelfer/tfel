/*!
 * \file   tests/Math/ExpressionTemplates/include/test10-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 */

#ifndef _LIB_TFEL_TEST10_HEADER_HXX_
#define _LIB_TFEL_TEST10_HEADER_HXX_ 

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/functions.hxx"

template<typename T>
void function(const T&);

template<typename T>
void function(const T&)
{
  using namespace std;
  using namespace tfel::meta;
  using namespace tfel::utilities;
  assert(HasRandomAccessConstIterator<T>::cond);
}

#endif /* _LIB_TFEL_TEST10_HEADER_HXX */
