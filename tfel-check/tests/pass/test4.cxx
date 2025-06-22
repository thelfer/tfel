/*!
 * \file  test4.cxx
 * \brief
 * \author Petkantchin Remy
 * \brief 6 may 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <fstream>

int main(void) {
  using namespace std;
  ofstream file("test4.res");
  if (!file) {
    return EXIT_FAILURE;
  }
  file << "A B C" << endl;
  file << "0. 0.001 2." << endl;
  file << "0. 0.01 2." << endl;
  file << "0. 0.1 2." << endl;
  file << "0. 0.5 2." << endl;
  file << "0. 0.9 2." << endl;
  file << "0. 1 1." << endl;
  file << "0. 3141515 1." << endl;
  file << "0. 3141526 1." << endl;
  file << "0. 3141616 1." << endl;
  file << "0. 3142516 1." << endl;
  file << "0. 3144516 1." << endl;

  file.close();
  return EXIT_SUCCESS;
}
