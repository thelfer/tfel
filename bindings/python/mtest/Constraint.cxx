/*!
 * \file   Constraint.cxx
 * \brief
 * \author th202608
 * \date   13/02/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/raw_function.hpp>
#include "TFEL/Raise.hxx"
#include "MTest/Constraint.hxx"

static mtest::ConstraintOptions makeConstraintOptions(
    const boost::python::tuple a, const boost::python::dict d) {
  tfel::raise_if(boost::python::len(a) != 0, "no unamed argument expected");
  const auto keys = d.keys();
  const auto values = d.values();
  boost::python::stl_input_iterator<boost::python::object> pk(keys);
  boost::python::stl_input_iterator<boost::python::object> pv(values);
  boost::python::stl_input_iterator<boost::python::object> pe;
  mtest::ConstraintOptions opts;
  while (pk != pe) {
    boost::python::extract<std::string> kc(*pk);
    tfel::raise_if(!kc.check(), "makeConstraintOptions: invalid type for key");
    const auto k = kc();
    if (k == "active") {
      boost::python::extract<bool> vc(*pv);
      tfel::raise_if(!vc.check(),
                     "makeConstraintOptions: invalid value type for "
                     "key 'active', expected a boolean value");
      opts.active = vc();
    } else if (k == "activating_events") {
      boost::python::extract<std::vector<std::string>> vc(*pv);
      tfel::raise_if(!vc.check(),
                     "makeConstraintOptions: invalid value type for "
                     "key 'desactivating_events', expected a list of strings");
      opts.activating_events = vc();
    } else if (k == "desactivating_events") {
      boost::python::extract<std::vector<std::string>> vc(*pv);
      tfel::raise_if(!vc.check(),
                     "makeConstraintOptions: invalid value type for "
                     "key 'desactivating_events', expected a list of strings");
      opts.desactivating_events = vc();
    } else {
      tfel::raise("unexpected key '" + k + "'");
    }
    ++pk;
    ++pv;
  }
  return opts;
}  // end of makeConstraintOptions

void declareConstraint();
void declareConstraint() {
  boost::python::class_<mtest::ConstraintOptions>("ConstraintOptions")
      .add_property("active", &mtest::ConstraintOptions::active)
      .add_property("activating_events",
                    &mtest::ConstraintOptions::activating_events)
      .add_property("desactivating_events",
                    &mtest::ConstraintOptions::desactivating_events);
  boost::python::def("makeConstraintOptions",
                     boost::python::raw_function(makeConstraintOptions));
}  // end of declareConstraints
