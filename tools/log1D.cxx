/*!
 * \file   log1D.cxx
 * \brief
 * \author Thomas Helfer
 * \date   18 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <cstdlib>

int main() {
  for (unsigned short i = 0; i != 3; ++i) {
    for (unsigned short j = 0; j != 3; ++j) {
      if ((i == 0) && (j == 0)) {
        std::cout << "*DDSDDE     = (";
      } else {
        std::cout << "*(DDSDDE+" << i + 3 * j << ") = (";
      }
      if (i == j) {
        std::cout << "-STRESS[" << i << "]+";
        std::cout << "K[" << i + 3 * j << "]/(STRAN[" << j << "]+DSTRAN[" << j
                  << "]))";
        std::cout << "/(STRAN[" << i << "]+DSTRAN[" << i << "]);\n";
      } else {
        std::cout << "K[" << i + 3 * j << "]/((STRAN[" << j << "]+DSTRAN[" << j
                  << "])*"
                     "(STRAN["
                  << i << "]+DSTRAN[" << i << "]));\n";
      }
    }
  }
  return EXIT_SUCCESS;
}
