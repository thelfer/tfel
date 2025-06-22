/*!
 * \file   include/TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx
 * \brief
 * \author Thomas Helfer
 * \date   22 nov. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_DECOMPOSITIONINPOSITIVEANDNEGATIVEPARTS_HXX
#define LIB_TFEL_MATH_STENSOR_DECOMPOSITIONINPOSITIVEANDNEGATIVEPARTS_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief compute the decomposition of a symmetric tensor in its
     * positive part and the derivative of this positive part.
     * \tparam DPPType     : type in which derivative of the positive part is
     * stored \tparam PPType      : type of positive part of the symmetric
     * tensor \tparam StensorType : type of the symmetric tensor \param[out] dpp
     * : derivative of the positive part of the symmetric tensor \param[out] pp
     * : positive part of the symmetric tensor \param[in]  s       : symmetric
     * tensor \param[in]  eps     : numerical precision used to decipher if two
     * eigenvalues are equal \pre DPPType must implement the ST2toST2Concept
     * \pre PPType must implement the StensorConcept
     * \pre StensorType must implement the StensorConcept
     */
    template <typename DPPType, typename PPType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<DPPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<PPType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            ST2toST2Traits<DPPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<PPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<StensorType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<PPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DPPType>>::cond,
        void>::type
    computeStensorPositivePartAndDerivative(DPPType&,
                                            PPType&,
                                            const StensorType&,
                                            const StensorNumType<StensorType>);

    /*!
     * \brief compute the decomposition of a symmetric tensor in its
     * positive part and the derivative of this positive part.
     * \tparam DPPType     : type in which derivative of the positive part is
     * stored \tparam PPType      : type of positive part of the symmetric
     * tensor \tparam StensorType : type of the symmetric tensor \param[out] dpp
     * : derivative of the positive part of the symmetric tensor \param[out] pp
     * : positive part of the symmetric tensor \param[in]  s       : symmetric
     * tensor \param[in]  eps     : numerical precision used to decipher if two
     * eigenvalues are equal \pre DPPType must implement the ST2toST2Concept
     * \pre PPType must implement the StensorConcept
     * \pre StensorType must implement the StensorConcept
     */
    template <typename DPPType, typename PPType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<DPPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<PPType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            ST2toST2Traits<DPPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<PPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<StensorType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<PPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DPPType>>::cond,
        void>::type
    computeStensorPositivePartAndDerivative(DPPType&,
                                            PPType&,
                                            const StensorType&,
                                            const StensorNumType<StensorType>);

    /*!
     * \brief compute the decomposition of a symmetric tensor in its
     * positive part and the derivative of this positive part.
     * \tparam DPPType     : type in which derivative of the positive part is
     * stored \tparam PPType      : type of positive part of the symmetric
     * tensor \tparam StensorType : type of the symmetric tensor \param[out] dpp
     * : derivative of the positive part of the symmetric tensor \param[out] pp
     * : positive part of the symmetric tensor \param[in]  s       : symmetric
     * tensor \param[in]  eps     : numerical precision used to decipher if two
     * eigenvalues are equal \pre DPPType must implement the ST2toST2Concept
     * \pre PPType must implement the StensorConcept
     * \pre StensorType must implement the StensorConcept
     */
    template <typename DPPType, typename PPType, typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<DPPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<PPType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            ST2toST2Traits<DPPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<PPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<StensorType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<PPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DPPType>>::cond,
        void>::type
    computeStensorPositivePartAndDerivative(DPPType&,
                                            PPType&,
                                            const StensorType&,
                                            const StensorNumType<StensorType>);
    /*!
     * \brief compute the decomposition of a symmetric tensor in a
     * positive and negative parts and their derivatives.
     * \tparam DPPType     : type in which derivative of the positive part is
     * stored \tparam DNPType     : type in which derivative of the negative
     * part is stored \tparam PPType      : type of positive part of the
     * symmetric tensor \tparam NPType      : type of negative part of the
     * symmetric tensor \tparam StensorType : type of the symmetric tensor
     * \param[out] dpp     : derivative of the positive part of the symmetric
     * tensor \param[out] dnp     : derivative of the negative part of the
     * symmetric tensor \param[out] pp      : positive part of the symmetric
     * tensor \param[out] np      : positive part of the symmetric tensor
     * \param[in]  s       : symmetric tensor
     * \param[in]  eps     : numerical precision used to decipher if two
     * eigenvalues are equal \pre DPPType must implement the ST2toST2Concept
     * \pre DNPType must implement the ST2toST2Concept
     * \pre PPType must implement the StensorConcept
     * \pre NPType must implement the StensorConcept
     * \pre StensorType must implement the StensorConcept
     */
    template <typename DPPType,
              typename DNPType,
              typename PPType,
              typename NPType,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<DPPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<DNPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<PPType, StensorConcept>::cond &&
            tfel::meta::Implements<NPType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            ST2toST2Traits<DPPType>::dime == StensorTraits<StensorType>::dime &&
            ST2toST2Traits<DNPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<PPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<NPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<StensorType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<PPType>>::cond &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<NPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DPPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DNPType>>::cond,
        void>::type
    computeStensorDecompositionInPositiveAndNegativeParts(
        DPPType&,
        DNPType&,
        PPType&,
        NPType&,
        const StensorType&,
        const StensorNumType<StensorType>);

    /*!
     * \brief compute the decomposition of a symmetric tensor in a
     * positive and negative parts and their derivatives.
     * \tparam DPPType     : type in which derivative of the positive part is
     * stored \tparam DNPType     : type in which derivative of the negative
     * part is stored \tparam PPType      : type of positive part of the
     * symmetric tensor \tparam NPType      : type of negative part of the
     * symmetric tensor \tparam StensorType : type of the symmetric tensor
     * \param[out] dpp     : derivative of the positive part of the symmetric
     * tensor \param[out] dnp     : derivative of the negative part of the
     * symmetric tensor \param[out] pp      : positive part of the symmetric
     * tensor \param[out] np      : positive part of the symmetric tensor
     * \param[in]  s       : symmetric tensor
     * \param[in]  eps     : numerical precision used to decipher if two
     * eigenvalues are equal \pre DPPType must implement the ST2toST2Concept
     * \pre DNPType must implement the ST2toST2Concept
     * \pre PPType must implement the StensorConcept
     * \pre NPType must implement the StensorConcept
     * \pre StensorType must implement the StensorConcept
     */
    template <typename DPPType,
              typename DNPType,
              typename PPType,
              typename NPType,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<DPPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<DNPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<PPType, StensorConcept>::cond &&
            tfel::meta::Implements<NPType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            ST2toST2Traits<DPPType>::dime == StensorTraits<StensorType>::dime &&
            ST2toST2Traits<DNPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<PPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<NPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<StensorType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<PPType>>::cond &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<NPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DPPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DNPType>>::cond,
        void>::type
    computeStensorDecompositionInPositiveAndNegativeParts(
        DPPType&,
        DNPType&,
        PPType&,
        NPType&,
        const StensorType&,
        const StensorNumType<StensorType>);

    /*!
     * \brief compute the decomposition of a symmetric tensor in a
     * positive and negative parts and their derivatives.
     * \tparam DPPType     : type in which derivative of the positive part is
     * stored \tparam DNPType     : type in which derivative of the negative
     * part is stored \tparam PPType      : type of positive part of the
     * symmetric tensor \tparam NPType      : type of negative part of the
     * symmetric tensor \tparam StensorType : type of the symmetric tensor
     * \param[out] dpp     : derivative of the positive part of the symmetric
     * tensor \param[out] dnp     : derivative of the negative part of the
     * symmetric tensor \param[out] pp      : positive part of the symmetric
     * tensor \param[out] np      : positive part of the symmetric tensor
     * \param[in]  s       : symmetric tensor
     * \param[in]  eps     : numerical precision used to decipher if two
     * eigenvalues are equal \pre DPPType must implement the ST2toST2Concept
     * \pre DNPType must implement the ST2toST2Concept
     * \pre PPType must implement the StensorConcept
     * \pre NPType must implement the StensorConcept
     * \pre StensorType must implement the StensorConcept
     */
    template <typename DPPType,
              typename DNPType,
              typename PPType,
              typename NPType,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<DPPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<DNPType, ST2toST2Concept>::cond &&
            tfel::meta::Implements<PPType, StensorConcept>::cond &&
            tfel::meta::Implements<NPType, StensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            ST2toST2Traits<DPPType>::dime == StensorTraits<StensorType>::dime &&
            ST2toST2Traits<DNPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<PPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<NPType>::dime == StensorTraits<StensorType>::dime &&
            StensorTraits<StensorType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<PPType>>::cond &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<NPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DPPType>>::cond &&
            tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<StensorNumType<StensorType>>,
                ST2toST2NumType<DNPType>>::cond,
        void>::type
    computeStensorDecompositionInPositiveAndNegativeParts(
        DPPType&,
        DNPType&,
        PPType&,
        NPType&,
        const StensorType&,
        const StensorNumType<StensorType>);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.ixx"

#endif /* LIB_TFEL_MATH_STENSOR_DECOMPOSITIONINPOSITIVEANDNEGATIVEPARTS_HXX */
