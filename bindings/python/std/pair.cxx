/*! 
 * \file   pair.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 jun 2010
 */

#include<map>

#include<boost/python.hpp>

#include"TFEL/Python/PairConverter.hxx"

BOOST_PYTHON_MODULE(_stl_pair)
{
  using namespace std;
  using namespace tfel::python;
  initializePairConverter<string,string>();
}
