/*!
 * \file  test2.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 23 avr 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <fstream>

int main() {
  using namespace std;
  ofstream file("test2.res");
  if (!file) {
    return EXIT_FAILURE;
  }
  file << "0. 1.e-23 2." << endl;
  file << "0. 1.e-23 1." << endl;
  file << "0. 1.e-23 2." << endl;
  file << "0.01 1.e-23 2." << endl;
  file << "0. 1.e-23 2." << endl;
  file << "0. 1.e-23 2." << endl;
  file << "0. 10.002 2." << endl;
  file.close();
  return EXIT_SUCCESS;
}
