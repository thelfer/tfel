/*!
 * \file   include/TFEL/Material/PowerLawStressBasedNucleationModel.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_POWERLAWSTRESSBASEDNUCLEATIONMODEL_HXX
#define LIB_TFEL_MATERIAL_POWERLAWSTRESSBASEDNUCLEATIONMODEL_HXX

#include <tuple>
#include <ostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/stensor.hxx"

namespace tfel {

  namespace material {

    //! \brief a simple alias
    template <typename StressStensorType>
    using PowerLawStressBasedNucleationModelStressType =
        tfel::math::StensorNumType<StressStensorType>;
    //! \brief a simple alias
    template <typename StressStensorType>
    using PowerLawStressBasedNucleationModelRealType =
        tfel::typetraits::base_type<
            tfel::math::StensorNumType<StressStensorType>>;
    //! \brief a simple alias
    template <typename StressStensorType>
    using PowerLawStressBasedNucleationModelInverseStressType =
        tfel::math::result_type<
            PowerLawStressBasedNucleationModelRealType<StressStensorType>,
            PowerLawStressBasedNucleationModelStressType<StressStensorType>,
            tfel::math::OpDiv>;
    //! \brief a simple alias
    template <typename StressStensorType>
    using PowerLawStressBasedNucleationModeDerivativeType = tfel::math::stensor<
        tfel::math::StensorTraits<StressStensorType>::dime,
        PowerLawStressBasedNucleationModelInverseStressType<StressStensorType>>;

    /*!
     * \brief parameters of the strain version of the nucleation model
     * proposed by Chu and Needleman.
     *
     * Chu, C. C., and A. Needleman. 1980.
     * “Void Nucleation Effects in Biaxially Stretched Sheets.”
     * Journal of Engineering Materials and Technology 102 (3): 249–56
     */
    template <typename StressStensorType>
    struct PowerLawStressBasedNucleationModelParameters {
      //! \brief factor in front of the power law
      PowerLawStressBasedNucleationModelRealType<StressStensorType> fn;
      //! \brief minimal stress value
      PowerLawStressBasedNucleationModelStressType<StressStensorType> sn;
      //! \brief exponent of the power law
      PowerLawStressBasedNucleationModelRealType<StressStensorType> m;
      //! \brief minimum equivalent plastic strain
      PowerLawStressBasedNucleationModelRealType<StressStensorType> pmin;
    };  // end of struct PowerLawStressBasedNucleationModelParameters

    /*!
     * \brief stream operator
     * \return the output stream
     * \tparam real: numeric type
     * \tparam use_qt: flag stating if quantities are used
     * \param[in] os: output stream
     * \param[in] p: parameters
     */
    template <typename StressStensorType>
    std::ostream& operator<<(
        std::ostream&,
        const PowerLawStressBasedNucleationModelParameters<StressStensorType>&);
    /*!
     * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
     * the power law nucleation model.
     *
     * \f[
     * A_{n} = f_{n}\langle\dfrac{\sigma_{I}}{s_{n}}-1\rangle^{m}
     * \f]
     *
     * \tparam StressStensorType: type of the stress tensor
     *
     * \param[in] params: parameters
     * \param[in] sig: stress tensor
     * \param[in] p: equivalent plastic strain
     */
    template <typename StressStensorType>
    PowerLawStressBasedNucleationModelRealType<StressStensorType>
    computePowerLawStressBasedNucleationModelPorosityRateFactor(
        const PowerLawStressBasedNucleationModelParameters<StressStensorType>&,
        const StressStensorType&,
        const PowerLawStressBasedNucleationModelRealType<StressStensorType>&);

    /*!
     * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
     * the power law nucleation model and its derivative
     * \f$\frac{\partial A_{n}}{\partial \underline{\sigma}}\f$ with respect to
     * the stress.
     *
     * \f[
     * A_{n} = f_{n}\langle\dfrac{\sigma_{I}}{s_{n}}-1\rangle^{m}
     * \quad\text{if}\quad p \geq p_{min}
     * \f]
     *
     * \tparam StressStensorType: type of the stress tensor
     *
     * \param[in] params: parameters
     * \param[in] sig: stress tensor
     * \param[in] p: equivalent plastic strain
     */
    template <typename StressStensorType>
    std::tuple<
        PowerLawStressBasedNucleationModelRealType<StressStensorType>,
        PowerLawStressBasedNucleationModeDerivativeType<StressStensorType>>
    computePowerLawStressBasedNucleationModelPorosityRateFactorAndDerivative(
        const PowerLawStressBasedNucleationModelParameters<StressStensorType>&,
        const StressStensorType&,
        const PowerLawStressBasedNucleationModelRealType<StressStensorType>&);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/PowerLawStressBasedNucleationModel.ixx"

#endif /* LIB_TFEL_MATERIAL_POWERLAWSTRESSBASEDNUCLEATIONMODEL_HXX */
