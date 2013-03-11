/*! 
 * \file  stensor.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 aoû 2010
 */

#include<boost/python.hpp>

#include<TFEL/Math/stensor.hxx>

template<unsigned short N>
static double&
stensor_getitem(tfel::math::stensor<N,double>& v,
		const unsigned short i)
{
  return v[i];
}

template<unsigned short N>
static void
stensor_setitem(tfel::math::stensor<N,double>& v,
		const unsigned short i,
		const double vi)
{
  v[i] = vi;
}

template<unsigned short N>
static void declarestensor(const char * const n)
{
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::math;
  using boost::python::iterator;

  class_<stensor<N,double> >(n,init<double>())
    .def("__getitem__",&stensor_getitem<N>,
	 return_value_policy<copy_non_const_reference>())
    .def("__setitem__",&stensor_setitem<N>,
	 with_custodian_and_ward<1,2>())
    .def("__iter__",iterator<stensor<N,double> >())
    ;

}

void
declarestensor()
{
  declarestensor<1u>("Stensor1D");
  declarestensor<2u>("Stensor2D");
  declarestensor<3u>("Stensor3D");
} // end of declarestensor
