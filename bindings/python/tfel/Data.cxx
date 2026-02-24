/*!
 * \file   Data.cxx
 * \brief
 * \author Thomas Helfer
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
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"

template <typename T>
static bool is(const pybind11::object& o) {
  return pybind11::isinstance<T>(o);
}

static pybind11::object convert_data_to_python_object(
    const tfel::utilities::Data& d) {
  using namespace tfel::utilities;
  if (d.is<int>()) {
    return pybind11::cast(d.get<int>());
  } else if (d.is<double>()) {
    return pybind11::cast(d.get<double>());
  } else if (d.is<std::string>()) {
    return pybind11::cast(d.get<std::string>());
  } else if (d.is<std::vector<Data>>()) {
    return pybind11::cast(d.get<std::vector<Data>>());
  }
  if (!d.is<DataMap>()) {
    tfel::raise(
        "convert_data_to_python_object: "
        "unsupported conversion");
  }
  return pybind11::cast(d.get<DataMap>());
}  // end of convert_data_to_python_object

static void convert_python_object_to_data(tfel::utilities::Data& d,
                                          const pybind11::object& o) {
  using tfel::utilities::Data;
  if (is<int>(o)) {
    d = Data(pybind11::cast<int>(o));
  } else if (is<double>(o)) {
    d = Data(pybind11::cast<double>(o));
  }
  if (!is<std::string>(o)) {
    tfel::raise(
        "convert_python_object_to_data: "
        "unsupported conversion");
  }
  d = Data(pybind11::cast<std::string>(o));
}  // end of convert_python_object_to_data

static std::unique_ptr<tfel::utilities::Data> convert_python_object_to_data_ptr(
    const pybind11::object& o) {
  auto d = std::make_unique<tfel::utilities::Data>();
  convert_python_object_to_data(*d, o);
  return d;
}  // end of convert_python_object_to_data_ptr

template <typename T>
static void data_add_def(pybind11::class_<tfel::utilities::Data>& w,
                         const std::string& n) {
  using namespace tfel::utilities;
  bool (Data::*is_ptr)() const = &Data::is<T>;
  const T& (Data::*get_ptr)() const = &Data::get<T>;
  w.def(("is" + n).c_str(), is_ptr).def(("get" + n).c_str(), get_ptr);
}

void declareData(pybind11::module_&);

void declareData(pybind11::module_& m) {
  using namespace tfel::utilities;
  pybind11::class_<Data> w(m, "Data");
  w.def("get", convert_data_to_python_object);
  w.def(pybind11::init(&convert_python_object_to_data_ptr));
  data_add_def<int>(w, "Int");
  data_add_def<double>(w, "Double");
  data_add_def<std::string>(w, "String");
  data_add_def<std::vector<Data>>(w, "DataVector");
  data_add_def<DataMap>(w, "DataMap");
  //
  //    class_<DataMap>("DataMap").def(map_indexing_suite<DataMap>());
  //    class_<std::vector<Data>>("DataVector")
  //        .def(vector_indexing_suite<std::vector<Data>>());
}
