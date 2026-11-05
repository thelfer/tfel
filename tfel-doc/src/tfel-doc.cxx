/*!
 * \file  tfel-doc/src/tfel-doc.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <exception>

#include "TFEL/Utilities/TestDocMain.hxx"

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
[[noreturn]] static void tfel_doc_terminate_handler() {
  if (auto pe = std::current_exception()) {
    // we have an exception
    try {
      std::rethrow_exception(pe);
    } catch (const std::exception& e) {
      // additional action
      std::cerr << e.what() << std::endl;
    } catch (...) {
      std::cerr << "unknown exception thrown" << std::endl;
    }
  }
  ::exit(EXIT_FAILURE);
}
#endif /* ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
  auto find = [argv, argc](const char* v) {
    for (const auto* a = argv; a != argv + argc; ++a) {
      if (::strcmp(*a, v) == 0) {
        return true;
      }
    }
    return false;
  };
  if (!find("--no-terminate-handler")) {
    std::set_terminate(tfel_doc_terminate_handler);
  }
#endif /* ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)  */
  int r = EXIT_FAILURE;
#if not defined(__GLIBCXX__)
  try {
#endif /* not defined(__GLIBCXX__) */
    using namespace tfel::utilities;
    TestDocMain m(argc, argv);
    r = m.execute();
#if not defined(__GLIBCXX__)
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
#endif /* not defined(__GLIBCXX__) */
  return r;
}  // end of main
