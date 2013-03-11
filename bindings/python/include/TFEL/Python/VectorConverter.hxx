/*! 
 * \file  VectorConverter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mars 2013
 */

#ifndef _LIB_TFEL_PYTHON_VECTORCONVERTER_H_
#define _LIB_TFEL_PYTHON_VECTORCONVERTER_H_ 

#include<vector>

#include<boost/python.hpp>
#include<boost/python/stl_iterator.hpp>

namespace tfel
{

  namespace python
  {

    /*!
     * convert vector converter to python list
     */
    template<typename T>
    struct vector_to_python_list
    {
      static PyObject* convert(const std::vector<T>& v)
      {
	using namespace boost::python;
	object get_iter = iterator<std::vector<T> >();
	object iter = get_iter(v);
	list l(iter);
	return incref(l.ptr());
      }
    };

    template<typename T>
    struct vector_from_python_list
    {

      vector_from_python_list()
      {
	using boost::python::type_id;
	using namespace boost::python::converter;
	registry::push_back(&convertible,&construct,
			    type_id<std::vector<T> > ());
      }

      static void* convertible(PyObject * ptr)
      {
	using namespace boost::python;
	if (!PyList_Check(ptr)){
	  return 0;
	}
	handle<> h(borrowed(ptr));
	list l(h);
	stl_input_iterator<object> p(l);
	stl_input_iterator<object> pe;
	while(p!=pe){
	  extract<T> e(*p);
	  if(!e.check()){
	    return 0;
	  }
	  ++p;
	}
	return ptr;
      }

      static void construct(PyObject* ptr,
			    boost::python::converter::rvalue_from_python_stage1_data * data)
      {
	using namespace boost::python;
	using namespace boost::python::converter;
	typedef rvalue_from_python_storage<std::vector<T> > py_storage;
	using std::vector;
	assert(PyList_Check(ptr));
	handle<> h(borrowed(ptr));
	list l(h);
	stl_input_iterator<object> p(l);
	stl_input_iterator<object> pe;
	// Grab pointer to memory into which to construct the new vector<T>
	void* storage = reinterpret_cast<py_storage *>(data)->storage.bytes;
	// in-place construct the new vector<T> using the character data
	// extraced from the python object
	new (storage) vector<T>();
	vector<T>& v = *(static_cast<vector<T> *>(storage));
	while(p!=pe){
	  extract<T> e(*p);
	  assert(e.check());
	  v.push_back(e());
	  ++p;
	}    
	// Stash the memory chunk pointer for later use by boost.python
	data->convertible = storage;
      }

    };

    template<typename T> 
    static void initializeVectorConverter()
    {
      using namespace boost::python;
      using std::vector;
      // register the to-python converter
      to_python_converter<vector<T>,vector_to_python_list<T> >();
      // register the from-python converter
      vector_from_python_list<T>();
    }

  } // end of namespace python

} // end of namespace tfel

#endif /* _LIB_TFEL_PYTHON_VECTORCONVERTER_H */

