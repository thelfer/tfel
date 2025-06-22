/*!
 * \file  test1.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 23 avr 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cstring>

int main(void) {
  const char* my_env = ::getenv("TFEL_CHECK_TEST1_ENV");
  if (my_env == 0) {
    return EXIT_FAILURE;
  }
  if (::strcmp(my_env, "./libs2:./libs") != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
