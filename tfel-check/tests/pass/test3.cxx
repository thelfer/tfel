#include <cstdlib>
#include <iostream>

/*
 This test generate outputs to STDERR, to be verified by test3.check
 */
int main(void) {
  std::cerr << "STDERR MESSAGE" << std::endl;
  std::cerr << "0." << std::endl;
  std::cerr << "0.1" << std::endl;
  std::cerr << "0.11" << std::endl;
  std::cerr << "0.111" << std::endl;
  std::cerr << "0.111" << std::endl;
  return EXIT_SUCCESS;
}
