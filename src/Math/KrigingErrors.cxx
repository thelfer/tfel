/*!
 * \file  src/Math/KrigingErrors.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include "TFEL/Math/Kriging/KrigingErrors.hxx"

namespace tfel::math {

  KrigingErrorInvalidLength::KrigingErrorInvalidLength()
      : std::runtime_error(std::string(
            "number of locations is different from number of values")) {
  }  // end of KrigingErrorInvalidLength::KrigingErrorInvalidLength()

  KrigingErrorInvalidLength::~KrigingErrorInvalidLength() noexcept = default;

  KrigingErrorNoDataSpecified::KrigingErrorNoDataSpecified()
      : std::runtime_error(std::string("no data specified")) {
  }  // end of KrigingErrorNoDataSpecified::KrigingErrorNoDataSpecified()

  KrigingErrorNoDataSpecified::~KrigingErrorNoDataSpecified() noexcept =
      default;

  KrigingErrorInsufficientData::KrigingErrorInsufficientData()
      : std::runtime_error(std::string("insufficient data")) {
  }  // end of KrigingErrorInsufficientData::KrigingErrorInsufficientData()

  KrigingErrorInsufficientData::~KrigingErrorInsufficientData() noexcept =
      default;

}  // end of namespace tfel::math
