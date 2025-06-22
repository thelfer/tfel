/*!
 * \file  bindings/python/tfel/glossary.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

void declareGlossaryEntry();

void declareGlossary();

BOOST_PYTHON_MODULE(glossary) {
  declareGlossaryEntry();
  declareGlossary();
}
