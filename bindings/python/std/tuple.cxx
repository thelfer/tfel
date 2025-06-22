/*!
 * \file   bindings/python/std/tuple.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  19/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <tuple>
#include <boost/python.hpp>
#include "TFEL/Python/TupleConverter.hxx"

BOOST_PYTHON_MODULE(_stl_tuple) {
  using namespace tfel::python;
  initializeTupleConverter<double>();
  initializeTupleConverter<double, double>();
  initializeTupleConverter<double, double, double>();
}
