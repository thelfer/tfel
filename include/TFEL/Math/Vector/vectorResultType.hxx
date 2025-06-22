/*!
 * \file   include/TFEL/Math/Vector/vectorResultType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_VECTORRESULTTYPE_HXX
#define LIB_TFEL_VECTORRESULTTYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/matrix.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    struct matrix;

    /*
     * Partial specialisation for negation.
     */
    template <typename T>
    struct UnaryResultType<vector<T>, OpNeg> {
      typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and
     * scalars operations
     * \see   ResultType_
     */
    template <typename T, typename Scal>
    class ResultType_<VectorTag, ScalarTag, vector<T>, Scal, OpMult> {
      typedef typename ResultType<T, Scal, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tvector and
     * scalars operations
     * \see   ResultType_
     */
    template <typename Scal, typename T>
    class ResultType_<ScalarTag, VectorTag, Scal, vector<T>, OpMult> {
      typedef typename ResultType<Scal, T, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for vector
     * \see   ResultType
     */
    template <typename T, typename T2>
    class ResultType<vector<T>, vector<T2>, OpDotProduct> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;
      static constexpr bool isValid =
          tfel::typetraits::IsInvalid<ResBase_>::cond;

     public:
      typedef typename std::
          conditional<isValid, tfel::meta::InvalidType, ResBase_>::type type;
    };  // end of class ResultType<vector<T>,vector<T2>,OpDotProduct>

    /*!
     * \brief Partial specialisation for vector and
     * scalars operations
     * \see   ResultType_
     */
    template <typename T, typename Scal>
    class ResultType_<VectorTag, ScalarTag, vector<T>, Scal, OpDiv> {
      typedef typename ResultType<T, Scal, OpDiv>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for vector
     * \see   ResultType
     */
    template <typename T, typename T2>
    class ResultType<vector<T>, vector<T2>, OpDiadicProduct> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    matrix<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for vector
     * \see   ResultType
     */
    template <typename T, typename T2>
    class ResultType<vector<T>, vector<T2>, OpPlus> {
      typedef typename ResultType<T, T2, OpPlus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for vector
     * \see   ResultType
     */
    template <typename T, typename T2>
    class ResultType<vector<T>, vector<T2>, OpMinus> {
      typedef typename ResultType<T, T2, OpMinus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for vector
     * \see   ResultType
     */
    template <typename T, typename T2>
    class ResultType<vector<T>, vector<T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    vector<ResBase_>>::type type;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_VECTORRESULTTYPE_HXX */
