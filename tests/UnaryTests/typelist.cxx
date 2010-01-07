#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cassert>

#include<string>
#include<vector>

#include "Metaprogramming/StaticAssert.hxx"
#include "Metaprogramming/EnableIf.hxx"
#include "Metaprogramming/TypeList.hxx"
#include "Math/General/LogicalOperations.hxx"

template<typename T, template<class> class U>
class generic_container_;

template<template<class> class U>
class generic_container_< tfel::meta::TypeListEndType,U>{};

template<typename T, template<class> class U>
class generic_container_
  : public U<typename T::Current>,
    public generic_container_<typename T::Next, U>
{
};

template<typename T>
struct holder
{
  T value;
  
  void set(const T& src)
  {
    value=src;
  }

};

template<typename T>
class generic_container
  : public generic_container_<T,holder>
{

private:
  
  TFEL_STATIC_ASSERT((tfel::meta::TypeListElementsAreUnique<T>::cond));

public:
  
  template<unsigned int N>
  typename tfel::meta::EnableIf< 
    (N < tfel::meta::TypeListSize<T>::value),  
      const typename tfel::meta::TypeListFindNthElt<T,N>::type >::type
  get(void) const {
    return static_cast<const holder<typename tfel::meta::TypeListFindNthElt<T,N>::type> *>(this)->value;
  }
  
  template<unsigned int N>
  typename tfel::meta::EnableIf< 
    (N < tfel::meta::TypeListSize<T>::value), 
      void >::type
  set(const typename tfel::meta::TypeListFindNthElt<T,N>::type& src){
    static_cast<holder<typename tfel::meta::TypeListFindNthElt<T,N>::type> *>(this)->set(src);
  }

};

template<class T>
struct Is_same{

  template<class T2>
  struct test{
    static const bool cond = tfel::meta::IsSameType<T,T2>::cond;
  };

};

template<typename T, class Test>
class Mytest{

  typedef typename Test::template test<T> my_test_class;

public:

  static const bool cond = my_test_class::cond;

};

int main(void){

  using namespace tfel::meta;

  typedef GenerateTypeList<std::string,double,int>::type my_list;
  assert(TypeListElementsAreUnique<my_list>::cond);

  generic_container<TypeListNode<double, TypeListNode<std::string, TypeListEndType> > > test;
  
  test.set<0>(13.254);
  test.set<1>("Coucou");
  assert(std::abs(test.get<0>()-13.254)<1.e-14);
  assert(test.get<1>()=="Coucou");

  assert(Is_same<double>::test<double>::cond);
  assert(!Is_same<double>::test<int>::cond);
  assert((Mytest<double, Is_same<double> >::cond));

  return EXIT_SUCCESS;
  
}
