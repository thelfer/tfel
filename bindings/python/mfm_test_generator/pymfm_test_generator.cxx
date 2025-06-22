/*!
 * \file   bindings/python/mfront/pymfm_test_generator.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  10 avril 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

void declareMFMTestGeneratorFileExport();

BOOST_PYTHON_MODULE(_mfm_test_generator) {
  declareMFMTestGeneratorFileExport();
}
