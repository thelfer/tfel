/*!
 * \file  test3.cxx
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
#include <iostream>

/*
 This test generate outputs to STDERR, to be verified by test3.check
 */
int main() {
  std::cerr << "STDERR MESSAGE" << std::endl;
  std::cerr << "0." << std::endl;
  std::cerr << "0.1" << std::endl;
  std::cerr << "0.11" << std::endl;
  std::cerr << "0.111" << std::endl;
  std::cerr << "0.111" << std::endl;
  return EXIT_SUCCESS;
}
