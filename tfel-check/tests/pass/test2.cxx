/*! 
 * \file  test2.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 23 avr 2010
 */

#include<cstdlib>
#include<fstream>

int main(void) {
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

