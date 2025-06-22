/*!
 * \file   bindings/python/std/pair.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  10 jun 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <boost/python.hpp>
#include "TFEL/Python/PairConverter.hxx"

BOOST_PYTHON_MODULE(_stl_pair) {
  using namespace tfel::python;
  initializePairConverter<std::string, std::string>();
}
