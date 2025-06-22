/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2CONCEPT_HXX
#define LIB_TFEL_MATH_T2TOST2CONCEPT_HXX 1

#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Forward/T2toST2Concept.hxx"

namespace tfel {

  namespace math {

    template <class T>
    struct T2toST2Traits {
      typedef tfel::meta::InvalidType NumType;
      static constexpr unsigned short dime = 0u;
    };
    //! a simple alias
    template <class T>
    using T2toST2NumType = typename T2toST2Traits<T>::NumType;
    /*!
     * \class T2toST2Tag
     * \brief Helper class to characterise t2tost2.
     */
    struct T2toST2Tag {};

    template <class T>
    struct T2toST2Concept {
      typedef T2toST2Tag ConceptTag;

      typename T2toST2Traits<T>::NumType operator()(const unsigned short,
                                                    const unsigned short) const;

     protected:
      T2toST2Concept() = default;
      T2toST2Concept(T2toST2Concept&&) = default;
      T2toST2Concept(const T2toST2Concept&) = default;
      T2toST2Concept& operator=(const T2toST2Concept&) = default;
      ~T2toST2Concept() = default;
    };

    //! paratial specialisation for T2toST2
    template <typename Type>
    struct ConceptRebind<T2toST2Tag, Type> {
      using type = T2toST2Concept<Type>;
    };

    template <typename T>
    struct T2toST2Type {
      typedef T type;
    };

    template <typename T2toST2Type>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond,
        typename tfel::typetraits::AbsType<
            typename T2toST2Traits<T2toST2Type>::NumType>::type>::type
    abs(const T2toST2Type&);

    /*!
     * \brief compute de derivative of the push-forward of a symmetric
     * second order tensor with respect to the deformation gradient
     * knowing the value of this derivative in the initial
     * configuration.
     *
     * This function is typically used to compute the derivative of
     * the Kirchhoff stress tensor knowing the derivative of the second
     * Piola-Kirschoff stress.
     *
     * \param[out] : derivative of the push-forward symmetric tensor
     * \param[in]  : derivative of the orginal tensor
     * \param[in]  : orginal tensor (second Piola-Kirschoff stress)
     * \param[in]  : deformation gradient
     */
    template <typename T2toST2ResultType,
              typename T2toST2Type,
              typename StensorType,
              typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2ResultType, T2toST2Concept>::cond &&
            tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2ResultType>::dime == 1u &&
            T2toST2Traits<T2toST2Type>::dime == 1u &&
            StensorTraits<StensorType>::dime == 1u &&
            TensorTraits<TensorType>::dime == 1u &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result,
                typename T2toST2Traits<T2toST2ResultType>::NumType>::cond,
        void>::type
    computePushForwardDerivative(T2toST2ResultType&,
                                 const T2toST2Type&,
                                 const StensorType&,
                                 const TensorType&);
    /*!
     * \brief compute de derivative of the push-forward of a symmetric
     * second order tensor with respect to the deformation gradient
     * knowing the value of this derivative in the initial
     * configuration.
     *
     * This function is typically used to compute the derivative of
     * the Kirchhoff stress tensor knowing the derivative of the second
     * Piola-Kirschoff stress.
     *
     * \param[out] : derivative of the push-forward symmetric tensor
     * \param[in]  : derivative of the orginal tensor
     * \param[in]  : orginal tensor (second Piola-Kirschoff stress)
     * \param[in]  : deformation gradient
     */
    template <typename T2toST2ResultType,
              typename T2toST2Type,
              typename StensorType,
              typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2ResultType, T2toST2Concept>::cond &&
            tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2ResultType>::dime == 2u &&
            T2toST2Traits<T2toST2Type>::dime == 2u &&
            StensorTraits<StensorType>::dime == 2u &&
            TensorTraits<TensorType>::dime == 2u &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result,
                typename T2toST2Traits<T2toST2ResultType>::NumType>::cond,
        void>::type
    computePushForwardDerivative(T2toST2ResultType&,
                                 const T2toST2Type&,
                                 const StensorType&,
                                 const TensorType&);
    /*!
     * \brief compute de derivative of the push-forward of a symmetric
     * second order tensor with respect to the deformation gradient
     * knowing the value of this derivative in the initial
     * configuration.
     *
     * This function is typically used to compute the derivative of
     * the Kirchhoff stress tensor knowing the derivative of the second
     * Piola-Kirschoff stress.
     *
     * \param[out] : derivative of the push-forward symmetric tensor
     * \param[in]  : derivative of the orginal tensor
     * \param[in]  : orginal tensor (second Piola-Kirschoff stress)
     * \param[in]  : deformation gradient
     */
    template <typename T2toST2ResultType,
              typename T2toST2Type,
              typename StensorType,
              typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2ResultType, T2toST2Concept>::cond &&
            tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2ResultType>::dime == 3u &&
            T2toST2Traits<T2toST2Type>::dime == 3u &&
            StensorTraits<StensorType>::dime == 3u &&
            TensorTraits<TensorType>::dime == 3u &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result,
                typename T2toST2Traits<T2toST2ResultType>::NumType>::cond,
        void>::type
    computePushForwardDerivative(T2toST2ResultType&,
                                 const T2toST2Type&,
                                 const StensorType&,
                                 const TensorType&);
    /*!
     * \brief compute the Cauchy stress derivative from the Kirchhoff
     * stress derivative with respect to the deformation gradient
     * \param[out] ds   : Cauchy stress derivative
     * \param[in]  dt_K : Kirchhoff stress derivative
     * \param[in]  s    : Cauchy stress
     * \param[in]  F    : deformation gradient
     */
    template <typename T2toST2ResultType,
              typename T2toST2Type,
              typename StensorType,
              typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2ResultType, T2toST2Concept>::cond &&
            tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2ResultType>::dime ==
                T2toST2Traits<T2toST2Type>::dime &&
            T2toST2Traits<T2toST2ResultType>::dime ==
                StensorTraits<StensorType>::dime &&
            T2toST2Traits<T2toST2ResultType>::dime ==
                TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result,
                typename T2toST2Traits<T2toST2ResultType>::NumType>::cond,
        void>::type
    computeCauchyStressDerivativeFromKirchhoffStressDerivative(
        T2toST2ResultType&,
        const T2toST2Type&,
        const StensorType&,
        const TensorType&);
    /*!
     * \brief compute the Kirchhoff stress derivative from the Cauchy
     * stress derivative with respect to the deformation gradient
     * \param[out] dt_K : Kirchhoff stress derivative
     * \param[in]  ds   : Cauchy stress derivative
     * \param[in]  s    : Cauchy stress
     * \param[in]  F    : deformation gradient
     */
    template <typename T2toST2ResultType,
              typename T2toST2Type,
              typename StensorType,
              typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<T2toST2ResultType, T2toST2Concept>::cond &&
            tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            T2toST2Traits<T2toST2ResultType>::dime ==
                T2toST2Traits<T2toST2Type>::dime &&
            T2toST2Traits<T2toST2ResultType>::dime ==
                StensorTraits<StensorType>::dime &&
            T2toST2Traits<T2toST2ResultType>::dime ==
                TensorTraits<TensorType>::dime &&
            tfel::typetraits::IsFundamentalNumericType<
                typename TensorTraits<TensorType>::NumType>::cond &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename T2toST2Traits<T2toST2Type>::NumType,
                    typename StensorTraits<StensorType>::NumType,
                    OpPlus>::Result,
                typename T2toST2Traits<T2toST2ResultType>::NumType>::cond,
        void>::type
    computeKirchhoffStressDerivativeFromCauchyStressDerivative(
        T2toST2ResultType&,
        const T2toST2Type&,
        const StensorType&,
        const TensorType&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/T2toST2/T2toST2Concept.ixx"

#endif /* LIB_TFEL_MATH_T2TOST2CONCEPT_HXX */
