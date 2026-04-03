/*!
 * \file   PipeProfileHandler.hxx
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

#ifndef LIB_MTEST_PIPEPROFILEHANDLER_HXX
#define LIB_MTEST_PIPEPROFILEHANDLER_HXX

#include <iosfwd>
#include <memory>
#include <vector>

namespace mtest {

  // forward declaration
  struct PipeProfile;

  /*!
   * structure in charge of handling pipe profiles and an output
   * stream
   */
  struct PipeProfileHandler {
    PipeProfileHandler();
    PipeProfileHandler(PipeProfileHandler&&);
    PipeProfileHandler(const PipeProfileHandler&);
    PipeProfileHandler& operator=(PipeProfileHandler&&);
    PipeProfileHandler& operator=(const PipeProfileHandler&);
    std::shared_ptr<std::ostream> out;
    std::vector<std::shared_ptr<PipeProfile>> profiles;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEPROFILEHANDLER_HXX */
