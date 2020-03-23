/*!
 * \file   PackagingInformation.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   22/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PACKAGINGINFORMATION_HXX
#define LIB_MTEST_PACKAGINGINFORMATION_HXX

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mtest {

  /*!
   * an helper structure used to store information for the packaging step
   */
  using PackagingInformation =
      tfel::utilities::GenType<tfel::math::vector<real>,
                               tfel::math::matrix<real>>;

}  // end of namespace mtest

#endif /* LIB_MTEST_PACKAGINGINFORMATION_HXX */
