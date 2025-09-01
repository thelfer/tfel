/*!
 * \file   GeneratorOptions.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/07/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/GeneratorOptions.hxx"

namespace mfront {

  GeneratorOptions::GeneratorOptions() = default;
  GeneratorOptions::GeneratorOptions(const GeneratorOptions&) = default;
  GeneratorOptions::GeneratorOptions(GeneratorOptions&&) noexcept = default;
  GeneratorOptions& GeneratorOptions::operator=(const GeneratorOptions&) =
      default;
  GeneratorOptions& GeneratorOptions::operator=(GeneratorOptions&&) noexcept =
      default;
  GeneratorOptions::~GeneratorOptions() = default;

}  // end of namespace mfront
