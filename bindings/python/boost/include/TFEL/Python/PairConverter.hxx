/*!
 * \file  bindings/python/include/TFEL/Python/PairConverter.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_PYTHON_PAIRCONVERTER_H_
#define LIB_TFEL_PYTHON_PAIRCONVERTER_H_

#include <utility>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

namespace tfel::python {

  /*!
   * convert pair converter to python tuple
   */
  template <typename T1, typename T2>
  struct pair_to_python_tuple {
    static PyObject* convert(const std::pair<T1, T2>& pair) {
      using namespace boost::python;
      return incref(make_tuple(pair.first, pair.second).ptr());
    }
  };

  template <typename T1, typename T2>
  struct pair_from_python_tuple {
    pair_from_python_tuple() {
      using boost::python::type_id;
      using namespace boost::python::converter;
      registry::push_back(&convertible, &construct,
                          type_id<std::pair<T1, T2>>());
    }

    static void* convertible(PyObject* ptr) {
      using namespace boost::python;
      if (!PyTuple_Check(ptr)) {
        return nullptr;
      }
      if (PyTuple_Size(ptr) != 2) {
        return nullptr;
      }
      handle<> h(borrowed(ptr));
      tuple l(h);
      stl_input_iterator<object> p(l);
      extract<T1> e1(*p);
      if (!e1.check()) {
        return nullptr;
      }
      ++p;
      extract<T2> e2(*p);
      if (!e2.check()) {
        return nullptr;
      }
      return ptr;
    }

    static void construct(
        PyObject* ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data) {
      using namespace boost::python;
      using namespace boost::python::converter;
      using py_storage = rvalue_from_python_storage<std::pair<T1, T2>>;
      using std::pair;
      assert(PyTuple_Check(ptr));
      handle<> h(borrowed(ptr));
      tuple l(h);
      stl_input_iterator<object> p(l);
      // Grab pointer to memory into which to construct the new pair<T1,T2>
      void* storage = reinterpret_cast<py_storage*>(data)->storage.bytes;
      // in-place construct the new pair<T1,T2> using the character data
      // extraced from the python object
      new (storage) pair<T1, T2>();
      auto& v = *(static_cast<pair<T1, T2>*>(storage));
      extract<T1> e1(*p);
      assert(e1.check());
      ++p;
      extract<T2> e2(*p);
      assert(e2.check());
      v.first = e1;
      v.second = e2;
      // Stash the memory chunk pointer for later use by boost.python
      data->convertible = storage;
    }
  };

  template <typename T1, typename T2>
  static void initializePairConverter() {
    using namespace boost::python;
    // register the to-python converter
    to_python_converter<std::pair<T1, T2>, pair_to_python_tuple<T1, T2>>();
    // register the from-python converter
    pair_from_python_tuple<T1, T2>();
  }

}  // end of namespace tfel::python

#endif /* LIB_TFEL_PYTHON_PAIRCONVERTER_H_ */
