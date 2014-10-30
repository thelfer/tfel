/*!
 * \file   tests/MetaProgramming/HasConstIterator.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   19 oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */


#include<iostream>
#include<cstdlib>
#include<vector>

#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/IF.hxx"

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
  static_cast<void>(p);
  return EXIT_SUCCESS;

}
