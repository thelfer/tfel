/*!
 * \file  bindings/python/tfel/system.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 aoû 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#ifndef _WIN32
void declareProcessManagerCommand();
#endif /* _WIN32 */
void declareExternalLibraryManager();
void declareLibraryInformation();
void declareExternalMaterialPropertyDescription();
void declareExternalBehaviourDescription();

BOOST_PYTHON_MODULE(system) {
#ifndef _WIN32
  declareProcessManagerCommand();
#endif /* _WIN32 */
  declareLibraryInformation();
  declareExternalLibraryManager();
  declareExternalMaterialPropertyDescription();
  declareExternalBehaviourDescription();
}
