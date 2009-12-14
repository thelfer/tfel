/*!
 * \file   HasConstIterator.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   19 oct 2006
 */


#include<iostream>
#include<cstdlib>
#include<vector>

#include"Metaprogramming/HasConstIterator.hxx"
#include"Metaprogramming/HasIterator.hxx"
#include"Metaprogramming/IF.hxx"

template<typename T>
struct ConstIteratorHolder
{
  typedef typename T::const_iterator const_iterator;
};

struct DoNothing
{};

template<typename T>
struct Expr
  : public tfel::meta::IF<tfel::meta::HasConstIterator<T>::cond,
			  ConstIteratorHolder<T>,
			  DoNothing>::type
{};

int main(void){

  Expr<std::vector<double> >::const_iterator p;

  return EXIT_SUCCESS;

}
