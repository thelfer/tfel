/*!
 * \file  mfront/include/MFront/Ansys/Ansys.hxx
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

#ifndef LIB_MFRONT_ANSYS_HXX
#define LIB_MFRONT_ANSYS_HXX

#ifndef TFEL_ANSYS_INLINE
#define TFEL_ANSYS_INLINE inline
#endif /* LIB_MFRONT_ANSYS_HXX */

#ifndef TFEL_ANSYS_INLINE2
#define TFEL_ANSYS_INLINE2 inline
#endif /* LIB_MFRONT_ANSYS_HXX */

#include <cstdint>
#include <type_traits>

namespace ansys {

  //! integer type used by the ansys interface
  using AnsysInt = int;
  //! floatting-point type used by the ansys interface
  using AnsysReal = double;

  //! a simple metafunction
  template <typename real>
  struct StressFreeExpansionHandlerType {
    /*!
     * prototype of a function handling stress-free expansion at the
     * beginning of the time step
     *
     * - first  paramater : driving variable
     * - second paramater : driving variable at the end of the time step
     *   or driving variable increment
     * - third paramater  : expansion value at the beginning of the time
     *   step
     * - fourth paramater : expansion value at the end of the time step
     * - third  parameter : spatial dimension
     */
    using type = void (*)(real *const,
                          real *const,
                          const real *const,
                          const real *const,
                          const AnsysInt);
  };

  //! a simple alias
  template <typename real>
  using StressFreeExpansionHandler =
      typename StressFreeExpansionHandlerType<real>::type;

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYS_HXX */
