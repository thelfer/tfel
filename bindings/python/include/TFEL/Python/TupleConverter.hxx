/*!
 * \file  bindings/python/include/TFEL/Python/TupleConverter.hxx
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

#ifndef LIB_TFEL_PYTHON_TUPLECONVERTER_H_
#define LIB_TFEL_PYTHON_TUPLECONVERTER_H_

#include <iostream>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

namespace tfel {

  namespace python {

    namespace internals {

      using iterator = boost::python::stl_input_iterator<boost::python::object>;

      template <int...>
      struct integer_sequence {};  // end of struct integer_sequence

      template <int N, int... S>
      struct make_integer_sequence : make_integer_sequence<N - 1, N - 1, S...> {
      };  // end of struct make_integer_sequence

      template <int... S>
      struct make_integer_sequence<0, S...> {
        using type = integer_sequence<S...>;
      };  // end of struct make_integer_sequence

      template <typename... Types>
      struct is_convertible_to_tuple;

      template <typename T, typename... Types>
      struct is_convertible_to_tuple<T, Types...> {
        static bool exe(iterator& p) {
          if (!boost::python::extract<T>(*p).check()) {
            return false;
          }
          ++p;
          return is_convertible_to_tuple<Types...>::exe(p);
        }  // is_convertible_to_tuple
      };

      template <typename T>
      struct is_convertible_to_tuple<T> {
        static bool exe(iterator& p) {
          return boost::python::extract<T>(*p).check();
        }
      };  // end of is_convertible_to_tuple

      template <typename... Types>
      struct build_python_tuple {
        static PyObject* exe(const std::tuple<Types...>& t) {
          using integer_sequence =
              typename make_integer_sequence<sizeof...(Types)>::type;
          return build_python_tuple::exe(t, integer_sequence());
        }  // end of exe
       private:
        template <int... S>
        static PyObject* exe(const std::tuple<Types...>& t,
                             const integer_sequence<S...>&) {
          using namespace boost::python;
          return incref(make_tuple(std::get<S>(t)...).ptr());
        }  // end of exe
      };   // end of build_python_tuple

      template <typename T, int n>
      T extract_nth_elt(const boost::python::tuple& l) {
        iterator p(l);
        std::advance(p, n);
        boost::python::extract<T> e(*p);
        return e;
      }  // end of extract

      template <typename... Types>
      struct build_tuple {
        static void exe(void* s, const boost::python::tuple l) {
          using integer_sequence =
              typename make_integer_sequence<sizeof...(Types)>::type;
          return build_tuple::exe(s, l, integer_sequence());
        }  // end of exe
       private:
        template <int... S>
        static void exe(void* s,
                        const boost::python::tuple l,
                        const integer_sequence<S...>&) {
          new (s) std::tuple<Types...>(extract_nth_elt<Types, S>(l)...);
        }  // end of exe
      };   // end of build_tuple

      /*!
       * \brief convert tuple converter to python tuple
       */
      template <typename... Types>
      struct tuple_to_python_tuple {
        static PyObject* convert(const std::tuple<Types...>& t) {
          return build_python_tuple<Types...>::exe(t);
        }  // end of convert
      };   // end of struct tuple_to_python_tuple

      template <typename... Types>
      struct tuple_from_python_tuple {
        tuple_from_python_tuple() {
          using boost::python::type_id;
          using namespace boost::python::converter;
          registry::push_back(&convertible, &construct,
                              type_id<std::tuple<Types...>>());
        }

        static void* convertible(PyObject* ptr) {
          using namespace boost::python;
          if (!PyTuple_Check(ptr)) {
            return nullptr;
          }
          if (PyTuple_Size(ptr) != sizeof...(Types)) {
            return nullptr;
          }
          handle<> h(borrowed(ptr));
          tuple l(h);
          stl_input_iterator<object> p(l);
          if (!is_convertible_to_tuple<Types...>::exe(p)) {
            return nullptr;
          }
          return ptr;
        }  // end of convertible

        static void construct(
            PyObject* ptr,
            boost::python::converter::rvalue_from_python_stage1_data* data) {
          using namespace boost::python;
          using namespace boost::python::converter;
          using py_storage = rvalue_from_python_storage<std::tuple<Types...>>;
          assert(PyTuple_Check(ptr));
          handle<> h(borrowed(ptr));
          tuple l(h);
          // Grab pointer to memory into which to construct the new tuple<T1,T2>
          auto storage = reinterpret_cast<py_storage*>(data)->storage.bytes;
          // in-place construct the new tuple using the character data
          // extracted from the python object
          build_tuple<Types...>::exe(storage, l);
          // Stash the memory chunk pointer for later use by boost.python
          data->convertible = storage;
        }
      };

    }  // end of namespace internals

    template <typename... Types>
    static void initializeTupleConverter() {
      // register the to-python converter
      boost::python::to_python_converter<
          std::tuple<Types...>, internals::tuple_to_python_tuple<Types...>>();
      // register the from-python converter
      internals::tuple_from_python_tuple<Types...>();
    }  // end of initializeTupleConverter

  }  // end of namespace python

}  // end of namespace tfel

#endif /* LIB_TFEL_PYTHON_TUPLECONVERTER_H_ */
