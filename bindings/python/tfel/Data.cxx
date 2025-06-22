/*!
 * \file   Data.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   14 sept. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <memory>
#include <stdexcept>
#include "TFEL/Python/SharedPtr.hxx"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "TFEL/Utilities/Data.hxx"

template <typename T>
static bool is(const boost::python::object& o) {
  boost::python::extract<T> e(o);
  return e.check();
}

static boost::python::object convert_data_to_python_object(
    const tfel::utilities::Data& d) {
  using namespace boost::python;
  using namespace tfel::utilities;
  if (d.is<int>()) {
    return object(d.get<int>());
  } else if (d.is<double>()) {
    return object(d.get<double>());
  } else if (d.is<std::string>()) {
    return object(d.get<std::string>());
  } else if (d.is<std::vector<Data>>()) {
    return object(d.get<std::vector<Data>>());
  } else if (d.is<std::map<std::string, Data>>()) {
    return object(d.get<std::map<std::string, Data>>());
  }
  throw(
      std::runtime_error("convert_data_to_python_object: "
                         "unsupported conversion"));
}  // end of convert_data_to_python_object

static void convert_python_object_to_data(tfel::utilities::Data& d,
                                          const boost::python::object& o) {
  using namespace boost::python;
  using namespace tfel::utilities;
  using boost::python::dict;
  using boost::python::list;
  if (is<int>(o)) {
    d = Data(boost::python::extract<int>(o)());
  } else if (is<double>(o)) {
    d = Data(boost::python::extract<double>(o)());
  } else if (is<std::string>(o)) {
    d = Data(boost::python::extract<std::string>(o)());
  } else {
    throw(
        std::runtime_error("convert_python_object_to_data: "
                           "unsupported conversion"));
  }
}  // end of convert_python_object_to_data

static std::shared_ptr<tfel::utilities::Data> convert_python_object_to_data_ptr(
    const boost::python::object& o) {
  auto d = std::make_shared<tfel::utilities::Data>();
  convert_python_object_to_data(*d, o);
  return d;
}  // end of convert_python_object_to_data_ptr

template <typename T>
static void data_add_def(boost::python::class_<tfel::utilities::Data>& w,
                         const std::string& n) {
  using namespace boost::python;
  using namespace tfel::utilities;
  bool (Data::*is_ptr)() const = &Data::is<T>;
  const T& (Data::*get_ptr)() const = &Data::get<T>;
  w.def(("is" + n).c_str(), is_ptr)
      .def(("get" + n).c_str(), get_ptr,
           return_value_policy<copy_const_reference>());
}

void declareData();

void declareData() {
  using namespace boost::python;
  using namespace tfel::utilities;
  class_<Data> w("Data");
  w.def("get", convert_data_to_python_object)
      .def("__init__", make_constructor(convert_python_object_to_data_ptr));
  data_add_def<int>(w, "Int");
  data_add_def<double>(w, "Double");
  data_add_def<std::string>(w, "String");
  data_add_def<std::vector<Data>>(w, "DataVector");
  data_add_def<std::map<std::string, Data>>(w, "DataMap");

  class_<std::map<std::string, Data>>("DataMap").def(
      map_indexing_suite<std::map<std::string, Data>>());
  class_<std::vector<Data>>("DataVector")
      .def(vector_indexing_suite<std::vector<Data>>());
}
