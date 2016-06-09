/*! 
 * \file  bindings/python/mfront/CurrentState.cxx
 * \brief
 * \author Helfer Thomas
 * \date   23 oct. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>
#include"MTest/CurrentState.hxx"
#include"TFEL/Python/VectorConverter.hxx"

static tfel::math::vector<mtest::CurrentState>::iterator
v_begin(tfel::math::vector<mtest::CurrentState>& s){
  return s.begin();
}

static tfel::math::vector<mtest::CurrentState>::iterator
v_end(tfel::math::vector<mtest::CurrentState>& s){
  return s.end();
}

static void
v_setitem(tfel::math::vector<mtest::CurrentState>& s,
	  const tfel::math::vector<mtest::CurrentState>::size_type i,
	  const mtest::CurrentState& v){
  if(i>=s.size()){
    throw(std::out_of_range("tfel::math::vector<mtest::CurrentState>::operator[]: invalid_index"));
  }
  s[i] = v;
}

static mtest::CurrentState&
v_getitem(tfel::math::vector<mtest::CurrentState>& s,
	  const tfel::math::vector<mtest::CurrentState>::size_type i){
  if(i>=s.size()){
    throw(std::out_of_range("tfel::math::vector<mtest::CurrentState>::operator[]: invalid_index"));
  }
  return s[i];
}


static auto v_size(const tfel::math::vector<mtest::CurrentState>& v)
  -> decltype(v.end()-v.begin())
{
  return v.end()-v.begin();
}

void declareCurrentState(void);

void declareCurrentState(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::python;

  class_<mtest::CurrentState>("CurrentState")
    .add_property("s_1",&mtest::CurrentState::s_1)
    .add_property("s0",&mtest::CurrentState::s0)
    .add_property("s1",&mtest::CurrentState::s1)
    .add_property("e0",&mtest::CurrentState::e0)
    .add_property("e1",&mtest::CurrentState::e1)
    .add_property("e_th0",&mtest::CurrentState::e_th0)
    .add_property("e_th1",&mtest::CurrentState::e_th1)
    .add_property("mprops1",&mtest::CurrentState::mprops1)
    .add_property("iv_1",&mtest::CurrentState::iv_1)
    .add_property("iv0",&mtest::CurrentState::iv0)
    .add_property("iv1",&mtest::CurrentState::iv1)
    .add_property("esv0",&mtest::CurrentState::esv0)
    .add_property("desv",&mtest::CurrentState::desv)
    ;

  class_<tfel::math::vector<mtest::CurrentState>>("CurrentStateVector")
    .def("__iter__",boost::python::range(&v_begin,&v_end))
    .def("__len__",&v_size)
    .def("__getitem__",&v_getitem,
	 return_internal_reference<>())
    .def("__setitem__",&v_setitem)
    ;
  
}
