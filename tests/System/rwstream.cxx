/*!
 * \file   tests/System/rwstream.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   18 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "TFEL/System/wfstream.hxx"
#include "TFEL/System/rfstream.hxx"

static void write() {
  tfel::system::wfstream f("test.bin");
  f << "toto" << 12.;
  f << "tutu";
  f.close();
}

static void read() {
  tfel::system::rfstream r("test.bin");
  char c[5];
  double t;
  r.read(c, 4u);
  c[4] = '\0';
  assert(strcmp(c, "toto") == 0);
  r >> t;
  assert(std::abs(t - 12.) < 1.e-14);
  r.read(c, 4u);
  c[4] = '\0';
  assert(strcmp(c, "tutu") == 0);
  r.close();
}  // end of main

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  write();
  read();
  return EXIT_SUCCESS;
}
