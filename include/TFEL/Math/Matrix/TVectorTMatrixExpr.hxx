/*!
 * \file   include/TFEL/Math/Matrix/TVectorTMatrixExpr.hxx
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

#ifndef LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX
#define LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX

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

  template <unsigned short N,
            unsigned short M,
            VectorConcept A,
            MatrixConcept B>
  struct TVectorTMatrixExpr : public ExprBase {
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
    TVectorTMatrixExpr() = delete;

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
    };  // end of struc ColumnConstIterator

    struct VectorConstIterator {
      using VType = std::decay_t<A>;
      using NumType = numeric_type<VType>;
      TFEL_HOST_DEVICE constexpr VectorConstIterator(const VType& v_) noexcept
          : v(v_), i(0) {}
      TFEL_HOST_DEVICE constexpr VectorConstIterator& operator++() noexcept {
        ++i;
        return *this;
      }  // end of operator++
      TFEL_HOST_DEVICE constexpr auto operator*() const noexcept {
        return this->v(this->i);
      }

     private:
      const VType& v;
      size_type i;
    };  // end of struc VectorConstIterator

    TFEL_HOST_DEVICE constexpr TVectorTMatrixExpr(const A& l,
                                                  const B& r) noexcept
        : a(l), b(r) {}

    TFEL_HOST_DEVICE constexpr auto operator()(
        const IndexType i) const noexcept {
      using namespace tfel::fsalgo;
      return inner_product<N>::template exe<NumType>(VectorConstIterator(a),
                                                     ColumnConstIterator(b, i));
    }

   private:
    ArgumentStorage<A> a;
    ArgumentStorage<B> b;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX */
