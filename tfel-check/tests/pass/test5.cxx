/*!
 * \file  test5.cxx
 * \brief
 * \author Petkantchin Remy
 * \brief 14 may 2013
 */

#include<cstdlib>
#include<fstream>

int main() {
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
  //file << "4.5 12. 1." << endl;
  file.close();
  return EXIT_SUCCESS;
}
