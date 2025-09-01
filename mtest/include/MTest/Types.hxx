/*!
 * \file  mtest/include/MTest/Types.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTTYPES_HXX
#define LIB_MTEST_MTESTTYPES_HXX

#include <map>
#include <string>
#include <memory>
#include <cstddef>

namespace mtest {

  //! \brief a simple alias
  using real = double;
  //! \brief a simple alias
  struct Evolution;
  //! \brief a simple alias
  using EvolutionPtr = std::shared_ptr<Evolution>;
  //! \brief a simple alias
  using EvolutionManager = std::map<std::string, EvolutionPtr>;

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTTYPES_HXX */
