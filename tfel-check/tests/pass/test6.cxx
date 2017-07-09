/*
 * \file test6.cxx
 *  \brief tests the area comparison
 *  Created on: 13 juin 2013
 *      Author: rp238441
 */

#include<cstdlib>
#include<fstream>

int main() {
  using namespace std;
  ofstream file("test6.res");
  if (!file) {
    return EXIT_FAILURE;
  }
  file << "tps B C" << endl;
  file << "1. 4. 2." << endl;
  file << "2. 3. 1." << endl;
  file << "3.2 5. 1." << endl;
  file << "5. 10. 1." << endl;
  file.close();
  return EXIT_SUCCESS;
}

