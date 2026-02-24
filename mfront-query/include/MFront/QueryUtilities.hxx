/*!
 * \file   mfront-query/include/MFront/QueryUtilities.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/07/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_QUERY_QUERYUTILITIES_HXX
#define LIB_MFRONT_QUERY_QUERYUTILITIES_HXX

#include <iosfwd>

namespace mfront {

  // forward declarations
  struct VariableBoundsDescription;

  /*!
   * \brief display the bounds on the given stream
   * \param[in] os: output stream
   * \param[in] b: bounds
   */
  void displayBounds(std::ostream&, const VariableBoundsDescription&);
  /*!
   * \brief display the values of the bounds on the given stream
   * \param[in] os: output stream
   * \param[in] b: bounds
   */
  void displayBoundsValue(std::ostream&, const VariableBoundsDescription&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_QUERY_QUERYUTILITIES_HXX */
