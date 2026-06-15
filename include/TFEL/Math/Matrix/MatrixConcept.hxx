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
   * \class MatrixConceptBase
   * \brief A class used to model the concept of matrices.
   */
  template <typename MatrixType>
  struct MatrixConceptBase {
    using ConceptTag = MatrixTag;
  };

  /*!
   * \brief definition of the MatrixConcept concept
   * a class matching the matrix concept must expose the `MatrixTag` and have
   * access operators.
   */
  template <typename MatrixType>
  concept MatrixConcept =
      (std::is_same_v<typename std::decay_t<MatrixType>::ConceptTag,
                      MatrixTag>)&&(requires(const MatrixType t,
                                             const index_type<MatrixType> i,
                                             const index_type<MatrixType> j) {
        t(i, j);
      });

  //! paratial specialisation for matrices
  template <typename MatrixType>
  struct ConceptRebind<MatrixTag, MatrixType> {
    using type = MatrixConceptBase<MatrixType>;
  };

  //! \brief a simple alias for backward compatibility with versions prior
  //! to 4.0
  template <typename MatrixType>
  using MatrixTraits =
      std::conditional_t<MatrixConcept<MatrixType>,
                         MathObjectTraits<MatrixType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

  /*!
   * \brief an helper function which returns if the given type implements the
   * `MatrixConcept`.
   * \tparam MatrixType: type tested
   * \note function given for backward compatibility with versions prior
   * to 5.0
   */
  template <typename MatrixType>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsMatrixConcept() {
    return MatrixConcept<MatrixType>;
    // return tfel::meta::implements<MatrixType, MatrixConcept>();
  }  // end of implementsMatrixConcept

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_MATRIXCONCEPT_HXX */
