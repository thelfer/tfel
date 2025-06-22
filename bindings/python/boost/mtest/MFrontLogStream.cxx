/*!
 * \file  bindings/python/mfront/MFrontLogStream.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 sept. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <boost/python.hpp>

#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"

static void pySetVerboseMode(const std::string& m) {
  if (m == "Quiet") {
    mfront::setVerboseMode(mfront::VERBOSE_QUIET);
  } else if (m == "Level0") {
    mfront::setVerboseMode(mfront::VERBOSE_LEVEL0);
  } else if (m == "Level1") {
    mfront::setVerboseMode(mfront::VERBOSE_LEVEL1);
  } else if (m == "Level2") {
    mfront::setVerboseMode(mfront::VERBOSE_LEVEL2);
  } else if (m == "Level3") {
    mfront::setVerboseMode(mfront::VERBOSE_LEVEL3);
  } else if (m == "Debug") {
    mfront::setVerboseMode(mfront::VERBOSE_DEBUG);
  } else if (m == "Full") {
    mfront::setVerboseMode(mfront::VERBOSE_FULL);
  } else {
    mfront::setVerboseMode(m);
  }
}

void declareMFrontLogStream();

void declareMFrontLogStream() {
  using namespace boost;
  using namespace boost::python;
  using namespace mfront;

  void (*ptr)(const VerboseLevel) = setVerboseMode;

  enum_<VerboseLevel>("VerboseLevel")
      .value("VERBOSE_QUIET", VERBOSE_QUIET)
      .value("VERBOSE_LEVEL0", VERBOSE_LEVEL0)
      .value("VERBOSE_LEVEL1", VERBOSE_LEVEL1)
      .value("VERBOSE_LEVEL2", VERBOSE_LEVEL2)
      .value("VERBOSE_LEVEL3", VERBOSE_LEVEL3)
      .value("VERBOSE_DEBUG", VERBOSE_DEBUG)
      .value("VERBOSE_FULL", VERBOSE_FULL);

  def("setVerboseMode", ptr);
  def("setVerboseMode", pySetVerboseMode);
}
