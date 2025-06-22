/*!
 * \file   include/TFEL/TypeTraits/Promote.hxx
 * \brief  This file declares the Promote traits class.
 * \author Thomas Helfer
 * \date   03 May 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_PROMOTE_HXX
#define LIB_TFEL_PROMOTE_HXX

#include <limits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Forward/Complex.hxx"

namespace tfel {

  namespace typetraits {

    /*!
     * \class Promote
     * \brief Returns the greater type able to contain the type A and
     * B.
     *
     * \param  A, the first  type
     * \param  B, the second type
     * \return type, the result
     *
     * \pre    A and B must fundamental numeric types
     * \see    IsFundamentalNumericType
     *
     * \author Thomas Helfer
     * \date   03 May 2006
     */
    template <typename A, typename B>
    struct Promote;

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/TypeTraits/Promote/Promote.ixx"

#endif /* LIB_TFEL_PROMOTE_HXX */
