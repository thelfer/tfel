/*!
 * \file  bindings/python/mfront/CurrentState.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/CurrentState.hxx"
#include "TFEL/Python/VectorConverter.hxx"

#define TFEL_PYTHON_CURRENTSTATEGETTER(X)                     \
  static tfel::math::vector<mtest::real> CurrentState_get##X( \
      const mtest::CurrentState& t) {                         \
    return t.X;                                               \
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

static tfel::math::vector<mtest::CurrentState>::iterator v_begin(
    tfel::math::vector<mtest::CurrentState>& s) {
  return s.begin();
}

static tfel::math::vector<mtest::CurrentState>::iterator v_end(
    tfel::math::vector<mtest::CurrentState>& s) {
  return s.end();
}

static void v_setitem(
    tfel::math::vector<mtest::CurrentState>& s,
    const tfel::math::vector<mtest::CurrentState>::size_type i,
    const mtest::CurrentState& v) {
  tfel::raise_if<std::out_of_range>(i >= s.size(),
                                    "tfel::math::vector<mtest::CurrentState>"
                                    "::operator[]: invalid_index");
  s[i] = v;
}

static mtest::CurrentState& v_getitem(
    tfel::math::vector<mtest::CurrentState>& s,
    const tfel::math::vector<mtest::CurrentState>::size_type i) {
  tfel::raise_if<std::out_of_range>(i >= s.size(),
                                    "tfel::math::vector<mtest::CurrentState>"
                                    "::operator[]: invalid_index");
  return s[i];
}

static auto v_size(const tfel::math::vector<mtest::CurrentState>& v)
    -> decltype(v.end() - v.begin()) {
  return v.end() - v.begin();
}

static boost::python::object getInternalStateVariableValue(
    const mtest::CurrentState& s, const std::string& n, const int i) {
  auto throw_if = [](const bool b, const std::string& m) {
    tfel::raise_if(b, "mtest::getInternalStateVariableValue: " + m);
  };
  throw_if(s.behaviour == nullptr, "no behaviour defined");
  const auto& ivsnames = s.behaviour->getInternalStateVariablesNames();
  throw_if(std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
           "the behaviour don't declare an internal state "
           "variable named '" +
               n + "'");
  const auto type = s.behaviour->getInternalStateVariableType(n);
  const auto size = [&s, throw_if,
                     type]() -> std::vector<mtest::real>::size_type {
    if (type == 0) {
      return 1;
    } else if (type == 1) {
      return tfel::material::getStensorSize(s.behaviour->getHypothesis());
    } else if (type == 2) {
      return tfel::material::getSpaceDimension(s.behaviour->getHypothesis());
    } else if (type == 3) {
      return tfel::material::getTensorSize(s.behaviour->getHypothesis());
    }
    throw_if(true, "unsupported variable type");
    return 0;
  }();
  const auto pos = s.behaviour->getInternalStateVariablePosition(n);
  throw_if((s.iv_1.size() < pos + size) || (s.iv0.size() < pos + size) ||
               (s.iv1.size() < pos + size),
           "invalid size for state variables (bad initialization)");
  const auto& iv = [&s, throw_if,
                    i]() -> const tfel::math::vector<mtest::real>& {
    throw_if((i != 1) && (i != 0) && (i != -1), "invalid depth");
    if (i == -1) {
      return s.iv_1;
    } else if (i == 0) {
      return s.iv0;
    }
    return s.iv1;
  }();
  if (type == 0) {
    return boost::python::object(iv[pos]);
  }
  return boost::python::object(
      std::vector<mtest::real>(iv.begin() + pos, iv.begin() + pos + size));
}

static boost::python::object getInternalStateVariableValue(
    const mtest::CurrentState& s, const std::string& n) {
  return getInternalStateVariableValue(s, n, 1);
}  // end of getInternalStateVariableValue

void declareCurrentState();

void declareCurrentState() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::python;

  void (*ptr)(mtest::CurrentState&, const std::string&, const mtest::real) =
      mtest::setInternalStateVariableValue;
  void (*ptr2)(mtest::CurrentState&, const std::string&,
               const std::vector<mtest::real>&) =
      mtest::setInternalStateVariableValue;
  void (*ptr3)(mtest::CurrentState&, const std::string&, const mtest::real,
               const int) = mtest::setInternalStateVariableValue;
  void (*ptr4)(mtest::CurrentState&, const std::string&,
               const std::vector<mtest::real>&, const int) =
      mtest::setInternalStateVariableValue;
  object (*ptr5)(const mtest::CurrentState&, const std::string&) =
      ::getInternalStateVariableValue;
  object (*ptr6)(const mtest::CurrentState&, const std::string&, const int) =
      ::getInternalStateVariableValue;

  class_<mtest::CurrentState>("CurrentState")
      .add_property("s_1", CurrentState_gets_1)
      .add_property("s0", CurrentState_gets0)
      .add_property("s1", CurrentState_gets1)
      .add_property("e0", CurrentState_gete0)
      .add_property("e1", CurrentState_gete1)
      .add_property("e_th0", CurrentState_gete_th0)
      .add_property("e_th1", CurrentState_gete_th1)
      .add_property("mprops1", CurrentState_getmprops1)
      .add_property("iv_1", CurrentState_getiv_1)
      .add_property("iv0", CurrentState_getiv0)
      .add_property("iv1", CurrentState_getiv1)
      .add_property("evs0", CurrentState_getesv0)
      .add_property("desv", CurrentState_getdesv)
      .def("setInternalStateVariableValue", ptr,
           "set the value of a scalar internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: values\n"
           "\n"
           "This overwrites the values of the internal state variables:\n"
           "- at the beginning of the previous time step\n"
           "- at the beginning of the current time step\n"
           "- at the end of the current time step")
      .def("setInternalStateVariableValue", ptr2,
           "set the value of an internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: values\n"
           "\n"
           "This overwrites the values of the internal state variables:\n"
           "- at the beginning of the previous time step\n"
           "- at the beginning of the current time step\n"
           "- at the end of the current time step")
      .def("setInternalStateVariableValue", ptr3,
           "set the value of a scalar internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: value\n"
           "param[in]  d: depth\n"
           "\n"
           "The depth value has the following meaning:\n"
           "- -1 means that we are modifying the internal state variable\n"
           "  value at the beginning of the previous time step\n"
           "- 0 means that we are modifying the internal state variable value\n"
           "  at the beginning of the current time step\n"
           "- 1 means that we are modifying the internal state variable value\n"
           "  at the end of the current time step")
      .def("setInternalStateVariableValue", ptr4,
           "set the value of an internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: value\n"
           "param[in]  d: depth\n"
           "\n"
           "The depth value has the following meaning:\n"
           "- -1 means that we are modifying the internal state variable\n"
           "  value at the beginning of the previous time step\n"
           "- 0 means that we are modifying the internal state variable value\n"
           "  at the beginning of the current time step\n"
           "- 1 means that we are modifying the internal state variable value\n"
           "  at the end of the current time step")
      .def("getInternalStateVariableValue", ptr5,
           "get the value of an internal state variable at the end of the time "
           "\n"
           "step\n"
           "\n"
           "param[in]  n: variable name")
      .def("getInternalStateVariableValue", ptr6,
           "get the value of an internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  d: depth\n"
           "\n"
           "The depth value has the following meaning:\n"
           "- -1 means that we request the value at the beginning of the "
           "previous time step\n"
           "- 0 means that we request the  value at the beginning of the "
           "current time step\n"
           "- 1 means that we request the  value at the end of the current "
           "time step");

  class_<tfel::math::vector<mtest::CurrentState>>("CurrentStateVector")
      .def("__iter__",
           boost::python::range<return_internal_reference<>>(&v_begin, &v_end))
      .def("__len__", &v_size)
      .def("__getitem__", &v_getitem, return_internal_reference<>())
      .def("__setitem__", &v_setitem);
}
