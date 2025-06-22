/*!
 * \file   bindings/python/tfel/tests.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  31 août 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

void declareTestResult();

BOOST_PYTHON_MODULE(tests) { declareTestResult(); }
