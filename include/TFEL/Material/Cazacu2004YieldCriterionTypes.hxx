/*!
 * \file   include/TFEL/Material/Cazacu2004YieldCriterionTypes.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/05/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CAZACU2004YIELDCRITERIONTYPES_HXX
#define LIB_TFEL_MATERIAL_CAZACU2004YIELDCRITERIONTYPES_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using Cazacu2004StressType = tfel::math::numeric_type<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using Cazacu2004BaseType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using Cazacu2004InvertStressType =
      tfel::math::result_type<Cazacu2004BaseType<StressStensor>,
                              Cazacu2004StressType<StressStensor>,
                              tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using Cazacu2004StressNormalType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          Cazacu2004BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using Cazacu2004StressSecondDerivativeType =
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StressStensor>(),
                           Cazacu2004InvertStressType<StressStensor>>;

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_CAZACU2004YIELDCRITERIONTYPES_HXX */
