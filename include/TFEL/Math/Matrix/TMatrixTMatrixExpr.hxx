/*!
 * \file   include/TFEL/Math/Matrix/TMatrixTMatrixExpr.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   19/03/2024
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TMATRIXTMATRIXEXPR_HXX
#define LIB_TFEL_MATH_TMATRIXTMATRIXEXPR_HXX

#include <cstddef>
#include <iterator>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/FSAlgorithm/inner_product.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/RunTimeCheck.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  /*!
   * \brief an expression representing the product of a (N, K) tiny matrix
   * by a (K, M) tiny matrix.
   */
  template <unsigned short N,
            unsigned short M,
            unsigned short K,
            MatrixConcept A,
            MatrixConcept B>
  struct TMatrixTMatrixExpr : public ExprBase {
    using RunTimeProperties = EmptyRunTimeProperties;
    using Result = BinaryOperationResult<A, B, OpMult>;

    using NumType = numeric_type<Result>;
    using IndexType = index_type<Result>;
    using value_type = NumType;
    using pointer = NumType*;
    using const_pointer = const NumType*;
    using reference = NumType&;
    using const_reference = const NumType&;
    using size_type = IndexType;
    using difference_type = ptrdiff_t;

    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }

   protected:
    TMatrixTMatrixExpr() = delete;

    /*!
     * \brief a pseudo iterator allowing to iterate over values in the same
     * row
     */
    struct RowConstIterator {
      using MType = std::decay_t<A>;
      using NumType = numeric_type<MType>;
      //! default constructor
      TFEL_HOST_DEVICE constexpr RowConstIterator(const MType& m_,
                                                  const unsigned short i_)
          : m(m_), i(i_) {}
      //! go to the next column
      TFEL_HOST_DEVICE constexpr RowConstIterator& operator++() {
        ++j;
        return *this;
      }  // end of operator++
      //! \return the current matrix value
      TFEL_HOST_DEVICE constexpr NumType operator*() const {
        return this->m(this->i, this->j);
      }

     private:
      const MType& m;          //! reference to the underlying matrix
      const unsigned short i;  //! row index
      unsigned short j = 0;    //! current column
    };                         // end of struc RowConstIterator

    struct ColumnConstIterator {
      using MType = std::decay_t<B>;
      using NumType = numeric_type<MType>;
      TFEL_HOST_DEVICE constexpr ColumnConstIterator(const MType& m_,
                                                     const size_type j_)
          : m(m_), i(0), j(j_) {}
      TFEL_HOST_DEVICE constexpr ColumnConstIterator& operator++() noexcept {
        ++i;
        return *this;
      }  // end of operator++
      TFEL_HOST_DEVICE constexpr auto operator*() const noexcept {
        return m(i, j);
      }

     private:
      const MType& m;
      size_type i;
      const size_type j;
    };  // end of struct ColumnConstIterator

    TFEL_HOST_DEVICE constexpr TMatrixTMatrixExpr(const A& l,
                                                  const B& r) noexcept
        : a(l), b(r) {}

    TFEL_HOST_DEVICE constexpr auto operator()(
        const IndexType i, const IndexType j) const noexcept {
      using namespace tfel::fsalgo;
      return inner_product<K>::template exe<NumType>(RowConstIterator(a, i),
                                                     ColumnConstIterator(b, j));
    }

   private:
    //! \brief left hand side
    ArgumentStorage<A> a;
    //! \brief right hand side
    ArgumentStorage<B> b;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TMATRIXTMATRIXEXPR_HXX */
