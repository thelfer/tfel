/*!
 * \file   LSDYNAExplicitData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   29 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LSDYNA_LSDYNAEXPLICITDATA_HXX
#define LIB_MFRONT_LSDYNA_LSDYNAEXPLICITDATA_HXX

#include "TFEL/Math/General/StridedRandomAccessIterator.hxx"
#include "TFEL/Math/General/DifferenceRandomAccessIterator.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"

namespace lsdyna {

  template <typename T>
  struct LSDYNAExplicitData {
    //! a simple alias
    using const_iterator = const T*;
    //! a simple alias
    using strided_iterator = tfel::math::StridedRandomAccessIterator<T*>;
    //! a simple alias
    using strided_const_iterator =
        tfel::math::StridedRandomAccessIterator<const T*>;
    using diff_strided_const_iterator =
        tfel::math::DifferenceRandomAccessIterator<strided_const_iterator>;
    //! time increment
    const T dt;
    //! material properties
    const const_iterator props;
    /*!
     * \brief properties used to compute the stiffness tensor or the
     * thermal expansion, if required (see the
     * `@RequireStiffnessTensor` or `@RequireThermalExpansionTensor`
     * `MFront` keywords)
     */
    const const_iterator eth_props;
    const T density;
    const T tempOld;
    const strided_const_iterator fieldOld;
    const strided_const_iterator stateOld;
    const T enerInternOld;
    const T enerInelasOld;
    const T tempNew;
    const diff_strided_const_iterator dfield;
    const strided_iterator stateNew;
    T& enerInternNew;
    T& enerInelasNew;
    //! out of bounds policy
    const tfel::material::OutOfBoundsPolicy policy;
  };  // end of struct LSDYNAExplicitData

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_LSDYNAEXPLICITDATA_HXX */
