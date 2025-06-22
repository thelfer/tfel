/*!
 * \file  mfront/include/MFront/Europlexus/Europlexus.hxx
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

#ifndef LIB_MFRONT_EPX_HXX
#define LIB_MFRONT_EPX_HXX

#ifndef TFEL_EPX_INLINE
#define TFEL_EPX_INLINE inline
#endif /* LIB_MFRONT_EPX_HXX */

#ifndef TFEL_EPX_INLINE2
#define TFEL_EPX_INLINE2 inline
#endif /* LIB_MFRONT_EPX_HXX */

#include <cstdint>
#include <type_traits>

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace epx {

  using EuroplexusInt = int;
  using EuroplexusReal = double;

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
  typedef void (*StressFreeExpansionHandler)(EuroplexusReal *const,
                                             EuroplexusReal *const,
                                             const EuroplexusReal *const,
                                             const EuroplexusReal *const,
                                             const EuroplexusInt);

  struct EPXData {
    EuroplexusInt *const STATUS;
    EuroplexusReal *const STRESS;
    EuroplexusReal *const STATEV;
    EuroplexusReal *const DDSDDE;
    EuroplexusReal *const PNEWDT;
    EuroplexusInt *const BROKEN;
    char *const MSG;
    const EuroplexusInt NSTATV;
    const EuroplexusReal DTIME;
    const EuroplexusReal *const DV0;
    const EuroplexusReal *const DV1;
    const EuroplexusReal *const R;
    const EuroplexusReal *const PROPS;
    const EuroplexusInt NPROPS;
    const EuroplexusReal *const TEMP;
    const EuroplexusReal *const DTEMP;
    const EuroplexusReal *const PREDEF;
    const EuroplexusReal *const DPRED;
    const EuroplexusInt NPREDEF;
    const tfel::material::OutOfBoundsPolicy op;
    const StressFreeExpansionHandler &sfeh;
  };  // end of EPXData

}  // end of namespace epx

#endif /* LIB_MFRONT_EPX_HXX */
