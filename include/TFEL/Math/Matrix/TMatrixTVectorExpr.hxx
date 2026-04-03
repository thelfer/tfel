/*!
 * \file   include/TFEL/Math/Matrix/TMatrixTVectorExpr.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   06 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX
#define LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX

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
   * \brief A placeholder for the computation of product of a tiny
   * matrix and a tiny vector.
   * \tparam N : numbers of rows of the matrix
   * \tparam M : numbers of columns of the matrix
   * \tparam A : type of the tiny matrix object (can be a reference)
   * \tparam B : type of the tiny vector object (can be a reference)
   */
  template <unsigned short N,
            unsigned short M,
            MatrixConcept A,
            VectorConcept B>
  struct TMatrixTVectorExpr : public ExprBase {
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! \brief return the runtime properties
    TFEL_HOST_DEVICE constexpr RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }  // end of getRunTimeProperties
    /*!
     * a pseudo iterator allowing to iterate over values in the same
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
    //! a pseudo iterator for the vector. This iterator works even if
    // VType does not provide an iterator
    struct VectorConstIterator {
      using VType = std::decay_t<B>;
      using NumType = numeric_type<VType>;
      TFEL_HOST_DEVICE constexpr VectorConstIterator(const VType& v_) : v(v_) {}
      TFEL_HOST_DEVICE constexpr VectorConstIterator& operator++() {
        ++i;
        return *this;
      }  // end of operator++
      TFEL_HOST_DEVICE constexpr NumType operator*() const {
        return this->v(this->i);
      }

     private:
      VType v;
      unsigned short i = 0;
    };  // end of struc VectorConstIterator

   protected:
    //! a simple alias
    using Result = BinaryOperationResult<A, B, OpMult>;
    //! a simple alias
    using NumType = numeric_type<Result>;
    //! a simple alias
    using IndexType = index_type<Result>;

    typedef NumType value_type;
    typedef NumType* pointer;
    typedef const NumType* const_pointer;
    typedef NumType& reference;
    typedef const NumType& const_reference;
    typedef IndexType size_type;
    typedef ptrdiff_t difference_type;

    TMatrixTVectorExpr() = delete;

    TFEL_HOST_DEVICE constexpr TMatrixTVectorExpr(A l, B r) : a(l), b(r) {}

    TFEL_HOST_DEVICE constexpr auto operator()(const IndexType i) const {
      using namespace tfel::fsalgo;
      return inner_product<M>::template exe<NumType>(RowConstIterator(a, i),
                                                     VectorConstIterator(b));
    }

    ArgumentStorage<A> a;
    ArgumentStorage<B> b;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX */
