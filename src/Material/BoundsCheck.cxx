/*!
 * \file  BoundsCheck.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/BoundsCheck.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel::material {

  void BoundsCheckBase::throwOutOfLowerBoundsException(
      const std::string_view n,
      const std::string_view v,
      const std::string_view b) {
    tfel::raise<OutOfBoundsException>(
        "BoundsCheckBase::throwOutOfLowerBoundsException: variable '" +
        std::string{n} + "' is below its lower bound (" + std::string{v} + "<" +
        std::string{b} + ")");
  }

  void BoundsCheckBase::throwOutOfUpperBoundsException(
      const std::string_view n,
      const std::string_view v,
      const std::string_view b) {
    tfel::raise<OutOfBoundsException>(
        "BoundsCheckBase::throwOutOfUpperBoundsException: variable '" +
        std::string{n} + "' is below its lower bound (" + std::string{v} + ">" +
        std::string{b} + ")");
  }

  void BoundsCheckBase::throwOutOfBoundsException(const std::string_view n,
                                                  const std::string_view v,
                                                  const std::string_view lb,
                                                  const std::string_view ub) {
    tfel::raise<OutOfBoundsException>(
        "BoundsCheckBase::throwOutOfLowerBoundsException: variable '" +
        std::string{n} + "' is out of its bounds (" + std::string{v} + "<" +
        std::string{lb} + " or " + std::string{v} + ">" + std::string{ub} +
        ")");
  }

  void BoundsCheckBase::displayOutOfLowerBoundsWarning(
      const std::string_view n,
      const std::string_view v,
      const std::string_view b) {
    std::cerr << "BoundsCheckBase::displayOutOfLowerBoundsWarning : variable '"
              << n << "' is below its lower bound (" << v << "<" << b << ")\n";
  }

  void BoundsCheckBase::displayOutOfUpperBoundsWarning(
      const std::string_view n,
      const std::string_view v,
      const std::string_view b) {
    std::cerr << "BoundsCheckBase::displayOutOfUpperBoundsWarning : variable '"
              << n << "' is below its lower bound (" << v << ">" << b << ")\n";
  }

  void BoundsCheckBase::displayOutOfBoundsWarning(const std::string_view n,
                                                  const std::string_view v,
                                                  const std::string_view lb,
                                                  const std::string_view ub) {
    std::cerr << "BoundsCheckBase::displayOutOfLowerBoundsWarning : variable '"
              << n << "' is out of its bounds (" << v << "<" << lb << " or "
              << v << ">" << ub << ")\n";
  }

}  // end of namespace tfel::material
