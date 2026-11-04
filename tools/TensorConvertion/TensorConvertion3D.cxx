/*!
 * \file   TensorConvertion.cxx
 * \brief
 * \author Thomas Helfer
 * \date 14/10/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

static int index(const unsigned short i, const unsigned short j) {
  if ((i == j) && (i < 3)) {
    return i;
  } else if ((i == 0) && (j == 1)) {
    return 3;
  } else if ((i == 1) && (j == 0)) {
    return 4;
  } else if ((i == 0) && (j == 2)) {
    return 5;
  } else if ((i == 2) && (j == 0)) {
    return 6;
  } else if ((i == 1) && (j == 2)) {
    return 7;
  } else if ((i == 2) && (j == 1)) {
    return 8;
  }
  return -1;
}

int main() {
  // CIJKL=1/J(K_{IJKN}*F_{LN}
  for (unsigned short i = 0; i != 3; ++i) {
    for (unsigned short j = 0; j != 3; ++j) {
      const auto ri = index(i, j);
      for (unsigned short k = 0; k != 3; ++k) {
        for (unsigned short l = 0; l != 3; ++l) {
          const auto rj = index(k, l);
          std::cout << "C(" << ri << "," << rj << ") = 1/J*(";
          for (unsigned short n = 0; n != 3; ++n) {
            if (n != 0) {
              std::cout << "+";
            }
            std::cout << "K(" << ri << "," << index(k, n) << ")*F("
                      << index(l, n) << ")";
          }
          std::cout << ");\n";
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
