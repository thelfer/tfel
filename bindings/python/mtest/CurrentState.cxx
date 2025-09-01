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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/CurrentState.hxx"

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

static pybind11::object getInternalStateVariableValue(
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
    return pybind11::cast(iv[pos]);
  }
  return pybind11::cast(
      std::vector<mtest::real>(iv.begin() + pos, iv.begin() + pos + size));
}

static pybind11::object getInternalStateVariableValue(
    const mtest::CurrentState& s, const std::string& n) {
  return getInternalStateVariableValue(s, n, 1);
}  // end of getInternalStateVariableValue

void declareCurrentState(pybind11::module_&);

void declareCurrentState(pybind11::module_& m) {
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
  pybind11::object (*ptr5)(const mtest::CurrentState&, const std::string&) =
      ::getInternalStateVariableValue;
  pybind11::object (*ptr6)(const mtest::CurrentState&, const std::string&,
                           const int) = ::getInternalStateVariableValue;

  pybind11::class_<mtest::CurrentState>(m, "CurrentState")
      .def_property_readonly("s_1", CurrentState_gets_1)
      .def_property_readonly("s0", CurrentState_gets0)
      .def_property_readonly("s1", CurrentState_gets1)
      .def_property_readonly("e0", CurrentState_gete0)
      .def_property_readonly("e1", CurrentState_gete1)
      .def_property_readonly("e_th0", CurrentState_gete_th0)
      .def_property_readonly("e_th1", CurrentState_gete_th1)
      .def_property_readonly("mprops1", CurrentState_getmprops1)
      .def_property_readonly("iv_1", CurrentState_getiv_1)
      .def_property_readonly("iv0", CurrentState_getiv0)
      .def_property_readonly("iv1", CurrentState_getiv1)
      .def_property_readonly("evs0", CurrentState_getesv0)
      .def_property_readonly("desv", CurrentState_getdesv)
      .def_readonly("position", &mtest::CurrentState::position)
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

  pybind11::class_<tfel::math::vector<mtest::CurrentState>>(
      m, "CurrentStateVector")
      .def("__len__",
           [](const tfel::math::vector<mtest::CurrentState>& v) {
             return v.size();
           })
      .def(
          "__getitem__",
          [](const tfel::math::vector<mtest::CurrentState>& v,
             const unsigned short idx) {
            if (idx >= v.size()) {
              tfel::raise<std::range_error>(
                  "invalid index '" + std::to_string(static_cast<int>(idx)) +
                  "'");
            }
            return v[idx];
          },
          pybind11::return_value_policy::reference)
      .def(
          "__iter__",
          [](const tfel::math::vector<mtest::CurrentState>& v) {
            return pybind11::make_iterator(v.begin(), v.end());
          },
          pybind11::keep_alive<0,
                               1>())  // keep object alive while iterator exists

      ;
}
