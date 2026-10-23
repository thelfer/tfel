/*!
 * \file   test-usermat.cxx
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "usermat.cpp"

int main() {
  ansys::UserMaterialManager m;
  return m.getBehaviour(2) != nullptr;
}
