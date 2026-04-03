/*!
 * \file   PipeProfileHander.cxx
 * \brief
 * \author Thomas Helfer
 * \date   14 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MTest/PipeProfile.hxx"
#include "MTest/PipeProfileHandler.hxx"

namespace mtest {

  PipeProfileHandler::PipeProfileHandler() = default;
  PipeProfileHandler::PipeProfileHandler(PipeProfileHandler&&) = default;
  PipeProfileHandler::PipeProfileHandler(const PipeProfileHandler&) = default;
  PipeProfileHandler& PipeProfileHandler::operator=(PipeProfileHandler&&) =
      default;
  PipeProfileHandler& PipeProfileHandler::operator=(const PipeProfileHandler&) =
      default;

}  // end of namespace mtest
