/*!
 * \file   test10-header.hxx
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

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Utilities/Name.hxx"
#include"Math/vector.hxx"
#include"Math/functions.hxx"

template<typename T>
void function(const T src);

template<typename T>
void function2(const T src);

template<typename T>
void function(const T src)
{
  using namespace std;
  using namespace tfel::utilities;

  cout << "Treating : " << T::getName() << endl;
  if(tfel::meta::HasRandomAccessConstIterator<T>::cond){
    cout << T::getName() << " has a random access const iterator" << endl;
  } else {
    cout << T::getName() << " don't have a random access const iterator" << endl;
  }
  cout << "Iterator name : " << name(src.begin()) << endl;
  copy(src.begin(),src.end(),ostream_iterator<double>(cout," "));
  cout << endl;
}

template<typename T>
void function2(const T src)
{
  using namespace std;
  using namespace tfel::utilities;
  cout << name(src) << endl;
  cout << Name<typename std::iterator_traits<T>::value_type>::getName() << endl;
}

#endif /* _LIB_TFEL_TEST10_HEADER_HXX */
