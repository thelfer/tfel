/*!
 * \file  include/TFEL/Math/Tensor/MatrixViewFromTensor.hxx
 * \brief The MatrixViewFromTensor describes how a tensor can be interpreted as
 * a tiny matrix \author Thomas Helfer \brief  03 déc. 2013 \copyright Copyright
 * (C) 2006-2018 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_MATH_MATRIXVIEWFROMTENSOR_HXX
#define LIB_TFEL_MATH_MATRIXVIEWFROMTENSOR_HXX

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"

namespace tfel::math {

  namespace internals {
    template <unsigned short N>
    struct TensorConceptMatrixAccessOperator;
  }

  /*!
   * Matrix view from tensor
   */
  template <typename TensorType>
  struct MatrixViewFromTensorExpr {};  // end of struct MatrixViewFromTensorExpr

  template <typename TensorType>
  struct Expr<tmatrix<3u, 3u, numeric_type<TensorType>>,
              MatrixViewFromTensorExpr<TensorType>>
      : public ExprBase,
        public MatrixConcept<Expr<tmatrix<3u, 3u, numeric_type<TensorType>>,
                                  MatrixViewFromTensorExpr<TensorType>>> {
    static_assert(implementsTensorConcept<TensorType>());

    using NumType = numeric_type<TensorType>;
    typedef unsigned short IndexType;
    typedef EmptyRunTimeProperties RunTimeProperties;

    RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }

    Expr(TensorType t_) : t(t_) {}  // end of Expr

    NumType operator()(const unsigned short i, const unsigned short j) const {
      using tfel::math::internals::TensorConceptMatrixAccessOperator;
      constexpr auto N = getSpaceDimension<TensorType>();
      return TensorConceptMatrixAccessOperator<N>::exe(this->t, i, j);
    }  // end of operator() const

   protected:
    ArgumentStorage<TensorType> t;
  };  // end of struct Expr

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_MATRIXVIEWFROMTENSOR_HXX */
