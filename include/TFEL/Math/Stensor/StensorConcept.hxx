/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.hxx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORCONCEPT_HXX
#define LIB_TFEL_MATH_STENSORCONCEPT_HXX 1

#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel {

  namespace math {

    template <typename StensorType>
    struct StensorTraits {
      typedef tfel::meta::InvalidType NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = 0u;
    };

    template <typename StensorType>
    using StensorNumType = typename StensorTraits<StensorType>::NumType;

    template <typename StensorType>
    using StensorBaseType =
        tfel::typetraits::base_type<StensorNumType<StensorType>>;

    /*!
     * \class StensorTag
     * \brief Helper class to characterise stensors.
     */
    struct StensorTag {};  // end of StensorTag

    template <typename T>
    struct StensorType {
      typedef T type;
    };

    template <typename T>
    struct StensorConcept {
      typedef StensorTag ConceptTag;

      StensorNumType<T> operator()(const unsigned short) const;

      StensorNumType<T> operator[](const unsigned short) const;

     protected:
      StensorConcept() = default;
      StensorConcept(StensorConcept&&) = default;
      StensorConcept(const StensorConcept&) = default;
      StensorConcept& operator=(const StensorConcept&) = default;
      ~StensorConcept() = default;
    };

    //! paratial specialisation for symmetric tensors
    template <typename Type>
    struct ConceptRebind<StensorTag, Type> {
      using type = StensorConcept<Type>;
    };

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        typename tfel::typetraits::AbsType<StensorNumType<StensorType>>::type>::
        type
        abs(const StensorType&);

    template <typename T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                                StensorNumType<T>>::type
        trace(const T& s);

    template <typename T>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond,
                                StensorNumType<T>>::type
        sigmaeq(const T& s);

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 1u,
                            typename StensorType<T>::type>::type
    deviator(const T&);

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 2u,
                            typename StensorType<T>::type>::type
    deviator(const T&);

    template <typename T>
    typename std::enable_if<tfel::meta::Implements<T, StensorConcept>::cond &&
                                StensorTraits<T>::dime == 3u,
                            typename StensorType<T>::type>::type
    deviator(const T&);
    /*!
     * \brief return the size of a symmetric tensor for the given
     * dimension
     * \param[in] d: space dimension
     */
    TFELMATH_VISIBILITY_EXPORT unsigned short getStensorSize(
        const unsigned short);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] dJ: determinant derivative
     * \param[in]  s: argument
     */
    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u &&
            StensorTraits<StensorResultType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(StensorResultType&, const StensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] dJ: determinant derivative
     * \param[in]  s: argument
     */
    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u &&
            StensorTraits<StensorResultType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(StensorResultType&, const StensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] dJ: determinant derivative
     * \param[in]  s: argument
     */
    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u &&
            StensorTraits<StensorResultType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(StensorResultType&, const StensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] dJ: determinant derivative
     * \param[in]  s:  argument
     */
    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u &&
            StensorTraits<StensorResultType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeviatorDeterminantDerivative(StensorResultType&,
                                         const StensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] dJ: determinant derivative
     * \param[in]  s:  argument
     */
    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u &&
            StensorTraits<StensorResultType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeviatorDeterminantDerivative(StensorResultType&,
                                         const StensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] dJ: determinant derivative
     * \param[in]  s:  argument
     */
    template <typename StensorResultType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorResultType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u &&
            StensorTraits<StensorResultType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result,
                StensorNumType<StensorResultType>>::cond,
        void>::type
    computeDeviatorDeterminantDerivative(StensorResultType&,
                                         const StensorType&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* LIB_TFEL_MATH_STENSORCONCEPT_HXX */
