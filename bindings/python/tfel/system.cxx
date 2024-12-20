
/*!
 * \file  bindings/python/tfel/system.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 aoû 2010
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>

#ifndef _WIN32
void declareProcessManagerCommand(pybind11::module_&);
#endif /* _WIN32 */
void declareExternalLibraryManager(pybind11::module_&);
void declareLibraryInformation(pybind11::module_&);
void declareExternalMaterialKnowledgeDescription(pybind11::module_&);
void declareExternalMaterialPropertyDescription(pybind11::module_&);
void declareExternalBehaviourDescription(pybind11::module_&);

PYBIND11_MODULE(system, m) {
#ifndef _WIN32
  declareProcessManagerCommand(m);
#endif /* _WIN32 */
  declareLibraryInformation(m);
  declareExternalLibraryManager(m);
  declareExternalMaterialKnowledgeDescription(m);
  declareExternalMaterialPropertyDescription(m);
  declareExternalBehaviourDescription(m);
}
