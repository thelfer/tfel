/*!
 * \file   SolverOptions.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   14 janv. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MTest/SolverOptions.hxx"

namespace mtest {

  SolverOptions::SolverOptions() = default;
  SolverOptions::SolverOptions(SolverOptions&&) = default;
  SolverOptions::SolverOptions(const SolverOptions&) = default;
  SolverOptions& SolverOptions::operator=(SolverOptions&&) = default;
  SolverOptions& SolverOptions::operator=(const SolverOptions&) = default;
  SolverOptions::~SolverOptions() = default;

}  // namespace mtest
