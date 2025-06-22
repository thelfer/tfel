/*!
 * \file   TextDataUtilities.hxx
 * \brief
 * \author Thomas Helfer
 * \date   11 janv. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_TEXTDATAUTILITIES_HXX
#define LIB_MTEST_TEXTDATAUTILITIES_HXX

#include <string>
#include <vector>

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "TFEL/Utilities/TextData.hxx"

namespace mtest {

  /*!
   * \brief extract the values of the a TextData object using a
   * specified formulae
   * \param[in] d: data
   * \param[in] m: evolution manager
   * \param[in] f: formulae
   */
  MTEST_VISIBILITY_EXPORT std::vector<double> eval(
      const tfel::utilities::TextData&,
      const EvolutionManager&,
      const std::string&);

}  // namespace mtest

#endif /* LIB_MTEST_TEXTDATAUTILITIES_HXX */
