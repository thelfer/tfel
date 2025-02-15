/*!
 * \file   TFEL/Math/ST2toST2/ST2toST2TransposeExpr.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30 août 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2TRANSPOSEEXPR_HXX
#define LIB_TFEL_MATH_ST2TOST2TRANSPOSEEXPR_HXX

#include <cmath>
#include <cstddef>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel::math {

  /*!
   * \brief an expression for the transpose of an object matching the
   * `ST2toST2Concept` concept.
   * \tparam A: type of the object to be transposed
   * \pre A must match the `ST2toST2Concept` concept
   */
  template <ST2toST2Concept A>
  struct TFEL_VISIBILITY_LOCAL ST2toST2TransposeExpr : public ExprBase {
    //! a simple alias
    using RunTimeProperties = EmptyRunTimeProperties;
    //! a simple alias
    using IndexType = unsigned short;
    //! a simple alias
    using NumType = numeric_type<A>;
    //! \return the runtime properties
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return EmptyRunTimeProperties();
    }

   protected:
    typedef NumType value_type;
    typedef NumType* pointer;
    typedef const NumType* const_pointer;
    typedef NumType& reference;
    typedef const NumType& const_reference;
    typedef IndexType size_type;
    typedef ptrdiff_t difference_type;
    /*!
     * \brief access operator
     * \param[in] i: row number
     * \param[in] j: colum number
     */
    TFEL_HOST_DEVICE constexpr ST2toST2TransposeExpr(A l) noexcept : a(l) {}
    /*!
     * \brief access operator
     * \param[in] i: row number
     * \param[in] j: colum number
     */
    TFEL_HOST_DEVICE constexpr NumType operator()(
        const IndexType i, const IndexType j) const noexcept {
      return this->a(j, i);
    }  // end of operator()
    //! storage for the object
    ArgumentStorage<A> a;
  };

}  // end of  namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOST2TRANSPOSEEXPR_HXX */
