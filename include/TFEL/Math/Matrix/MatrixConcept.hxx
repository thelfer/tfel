/*!
 * \file   include/TFEL/Math/Matrix/MatrixConcept.hxx
 * \brief  This file declares the MathObjectTraits, MatrixConcept and MatrixType
 * classes. \author Thomas Helfer \date   04 May 2006 \copyright Copyright (C)
 * 2006-2018 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_MATH_MATRIXCONCEPT_HXX
#define LIB_TFEL_MATH_MATRIXCONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"

namespace tfel::math {

  /*!
   * \class MatrixTag
   * \brief Helper class to characterise matrices.
   */
  struct MatrixTag {};  // end of MatrixTag

  /*!
   * \class MatrixConcept
   * \brief A class used to model the concept of matrices.
   * Here we use the curiously recurring template pattern.
   * \param T, a matrix type.
   * \author Thomas Helfer
   * \date   04 May 2006
   */
  template <typename T>
  struct MatrixConcept {
    using ConceptTag = MatrixTag;

   protected:
    MatrixConcept() = default;
    MatrixConcept(MatrixConcept&&) = default;
    MatrixConcept(const MatrixConcept&) = default;
    MatrixConcept& operator=(const MatrixConcept&) = default;
    ~MatrixConcept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `MatrixConcept`.
   * \tparam MatrixType: type tested
   */
  template <typename MatrixType>
  constexpr bool implementsMatrixConcept() {
    return tfel::meta::implements<MatrixType, MatrixConcept>();
  }  // end of implementsMatrixConcept

  //! paratial specialisation for matrices
  template <typename Type>
  struct ConceptRebind<MatrixTag, Type> {
    using type = MatrixConcept<Type>;
  };

  //! \brief a simple alias for backward compatibility with versions prior
  //! to 4.0
  template <typename MatrixType>
  using MatrixTraits =
      std::conditional_t<implementsMatrixConcept<MatrixType>(),
                         MathObjectTraits<MatrixType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_MATRIXCONCEPT_HXX */
