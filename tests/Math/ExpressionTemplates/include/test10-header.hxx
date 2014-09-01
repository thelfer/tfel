/*!
 * \file   tests/Math/ExpressionTemplates/include/test10-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
