/*!
 * \file  include/TFEL/Math/General/Forward/General.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_GENERAL_FORWARD_GENERAL_HXX
#define LIB_TFEL_MATH_GENERAL_FORWARD_GENERAL_HXX

namespace tfel {

  namespace math {

    template <typename A>
    class NegObjectRandomAccessConstIterator;

    template <typename A, typename Fct>
    class FctObjectRandomAccessConstIterator;

    template <typename A, typename B, typename Op>
    class ScalarObjectRandomAccessConstIterator;

    template <typename A, typename B, typename Op>
    class ObjectScalarRandomAccessConstIterator;

    template <typename A, typename B, typename Op>
    class ObjectObjectRandomAccessConstIterator;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_GENERAL_FORWARD_GENERAL_HXX */
