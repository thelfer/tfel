/*!
 * \file  bindings/python/include/TFEL/Python/MapConverter.hxx
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

#ifndef LIB_TFEL_PYTHON_MAPCONVERTER_H_
#define LIB_TFEL_PYTHON_MAPCONVERTER_H_

#include <map>

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

namespace tfel::python {

  /*!
   * convert map converter to python dict
   */
  template <typename K, typename V>
  struct map_to_python_dict {
    static PyObject* convert(const std::map<K, V>& v) {
      boost::python::dict d;
      for (const auto& kv : v) {
        d[kv.first] = kv.second;
      }
      return boost::python::incref(d.ptr());
    }
  };

  template <typename K, typename V>
  struct map_from_python_dict {
    map_from_python_dict() {
      using boost::python::type_id;
      using namespace boost::python::converter;
      registry::push_back(&convertible, &construct, type_id<std::map<K, V>>());
    }

    static void* convertible(PyObject* ptr) {
      using namespace boost::python;
      if (!PyDict_Check(ptr)) {
        return nullptr;
      }
      handle<> h(borrowed(ptr));
      dict d(h);
      list keys = d.keys();
      list values = d.values();
      stl_input_iterator<object> pk(keys);
      stl_input_iterator<object> pv(values);
      stl_input_iterator<object> pe;
      while (pk != pe) {
        extract<K> k(*pk);
        extract<V> v(*pv);
        if (!k.check()) {
          return nullptr;
        }
        if (!v.check()) {
          return nullptr;
        }
        ++pk;
        ++pv;
      }
      return ptr;
    }

    static void construct(
        PyObject* ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data) {
      using namespace boost::python;
      using namespace boost::python::converter;
      typedef rvalue_from_python_storage<std::map<K, V>> py_storage;
      using std::map;
      assert(PyDict_Check(ptr));
      handle<> h(borrowed(ptr));
      dict d(h);
      list keys = d.keys();
      list values = d.values();
      stl_input_iterator<object> pk(keys);
      stl_input_iterator<object> pv(values);
      stl_input_iterator<object> pe;
      // Grab pointer to memory into which to construct the new map<T>
      void* storage = reinterpret_cast<py_storage*>(data)->storage.bytes;
      // in-place construct the new map<T> using the character data
      // extraced from the python object
      new (storage) map<K, V>();
      map<K, V>& m = *(static_cast<map<K, V>*>(storage));
      while (pk != pe) {
        extract<K> k(*pk);
        extract<V> v(*pv);
        assert(k.check());
        assert(v.check());
        m[k()] = v();
        ++pk;
        ++pv;
      }
      // Stash the memory chunk pointer for later use by boost.python
      data->convertible = storage;
    }
  };

  template <typename K, typename V>
  static void initializeMapConverter() {
    using namespace boost::python;
    // register the to-python converter
    to_python_converter<std::map<K, V>, map_to_python_dict<K, V>>();
    // register the from-python converter
    map_from_python_dict<K, V>();
  }

}  // end of namespace tfel::python

#endif /* LIB_TFEL_PYTHON_MAPCONVERTER_H_ */
