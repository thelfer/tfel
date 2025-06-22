/*!
 * \file  TensorConceptIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORCONCEPTIO_HXX
#define LIB_TFEL_MATH_TENSORCONCEPTIO_HXX

#include <ostream>
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel {

  namespace math {

    // Serialisation operator
    template <typename T>
    std::ostream& operator<<(std::ostream&, const TensorConcept<T>&);

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const TensorConcept<T>& s) {
      unsigned short i;
      os << "[ ";
      for (i = 0; i != TensorDimeToSize<TensorTraits<T>::dime>::value; ++i) {
        os << s(i) << " ";
      }
      os << "]";
      return os;
    }  // end of operator <<

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORCONCEPTIO_HXX */
