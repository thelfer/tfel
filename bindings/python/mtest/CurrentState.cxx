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

#define TFEL_PYTHON_CURRENTSTATEGETTER( X )		\
  static tfel::math::vector<mtest::real>		\
  CurrentState_get##X(const mtest::CurrentState& t)	\
  {                                                     \
    return t. X ;                                       \
  }

TFEL_PYTHON_CURRENTSTATEGETTER(s_1)
TFEL_PYTHON_CURRENTSTATEGETTER(s0)
TFEL_PYTHON_CURRENTSTATEGETTER(s1)
TFEL_PYTHON_CURRENTSTATEGETTER(e0)
TFEL_PYTHON_CURRENTSTATEGETTER(e1)
TFEL_PYTHON_CURRENTSTATEGETTER(e_th0)
TFEL_PYTHON_CURRENTSTATEGETTER(e_th1)
TFEL_PYTHON_CURRENTSTATEGETTER(mprops1)
TFEL_PYTHON_CURRENTSTATEGETTER(iv_1)
TFEL_PYTHON_CURRENTSTATEGETTER(iv0)
TFEL_PYTHON_CURRENTSTATEGETTER(iv1)
TFEL_PYTHON_CURRENTSTATEGETTER(esv0)
TFEL_PYTHON_CURRENTSTATEGETTER(desv)

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


void declareCurrentState(void);

void declareCurrentState(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::python;

  class_<mtest::CurrentState>("CurrentState")
    .add_property("s_1",CurrentState_gets_1)
    .add_property("s0",CurrentState_gets0)
    .add_property("s1",CurrentState_gets1)
    .add_property("e0",CurrentState_gete0)
    .add_property("e1",CurrentState_gete1)
    .add_property("e_th0",CurrentState_gete_th0)
    .add_property("e_th1",CurrentState_gete_th1)
    .add_property("mprops1",CurrentState_getmprops1)
    .add_property("iv_1",CurrentState_getiv_1)
    .add_property("iv0",CurrentState_getiv0)
    .add_property("iv1",CurrentState_getiv1)
    .add_property("evs0",CurrentState_getesv0)
    .add_property("desv",CurrentState_getdesv)
    ;

  class_<tfel::math::vector<mtest::CurrentState>>("CurrentStateVector")
    .def("__iter__",boost::python::range(&v_begin,&v_end))
    .def("__getitem__",&v_getitem,return_internal_reference<>())
    .def("__setitem__",&v_setitem)
    ;
  
}
