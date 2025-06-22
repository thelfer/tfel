/*!
 * \file  test5.cxx
 * \brief
 * \author Petkantchin Remy
 * \brief 14 may 2013
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
  ofstream file("test5.res");
  if (!file) {
    return EXIT_FAILURE;
  }
  file << "tps B C" << endl;
  file << "1. 2. 2." << endl;
  file << "2. 3. 1." << endl;
  file << "3.2 6. 1." << endl;
  file << "4 7. 1." << endl;
  // file << "4.5 12. 1." << endl;
  file.close();
  return EXIT_SUCCESS;
}
