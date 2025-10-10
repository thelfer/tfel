/*!
 * \file  mfront/include/MFront/CalculiX/CalculiX.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CALCULIX_HXX
#define LIB_MFRONT_CALCULIX_HXX

#ifndef TFEL_CALCULIX_INLINE
#define TFEL_CALCULIX_INLINE inline
#endif /* LIB_MFRONT_CALCULIX_HXX */

#ifndef TFEL_CALCULIX_INLINE2
#define TFEL_CALCULIX_INLINE2 inline
#endif /* LIB_MFRONT_CALCULIX_HXX */

#include <cstdint>
#include <type_traits>

namespace calculix {

  //! integer type used by the calculix interface
  using CalculiXInt = int;
  //! floatting-point type used by the calculix interface
  using CalculiXReal = double;
  //! a simple alias
  using StressFreeExpansionHandler = void (*)(CalculiXReal *const,
                                              CalculiXReal *const,
                                              const CalculiXReal *const,
                                              const CalculiXReal *const,
                                              const CalculiXInt);

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_HXX */
