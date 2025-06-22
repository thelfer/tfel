/*!
 * \file   mfront/src/main.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef small
#undef small
#endif /* small */
#endif /* __CYGWIN__ */

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "TFEL/Config/TFELConfig.hxx"
#include "MFront/InitDSLs.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/MFront.hxx"

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
TFEL_NORETURN static void mfront_terminate_handler() {
  if (auto pe = std::current_exception()) {
    // we have an exception
    try {
      std::rethrow_exception(pe);
    } catch (const std::exception &e) {
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
int main(const int argc, const char *const *const argv) {
  auto find = [argv, argc](const char *v) {
    for (auto a = argv; a != argv + argc; ++a) {
      if (::strcmp(*a, v) == 0) {
        return true;
      }
    }
    return false;
  };
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
  if (!find("--no-terminate-handler")) {
    std::set_terminate(mfront_terminate_handler);
  }
#endif /* ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)  */
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  const auto bg = !find("--no-gui");
#endif
  mfront::initDSLs();
  mfront::initInterfaces();
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  try {
#endif /* __CYGWIN__ */
    mfront::MFront mfront(argc, argv);
    mfront.exe();
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  } catch (std::exception &e) {
    if (bg) {
      MessageBox(nullptr, e.what(), "mfront", 0);
    } else {
      std::cerr << e.what() << std::endl;
    }
    return EXIT_FAILURE;
  } catch (...) {
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */
  return EXIT_SUCCESS;
}
