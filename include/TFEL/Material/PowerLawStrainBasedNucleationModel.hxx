/*!
 * \file   include/TFEL/Material/PowerLawStrainBasedNucleationModel.hxx
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

#ifndef LIB_TFEL_MATERIAL_POWERLAWSTRAINBASEDNUCLEATIONMODEL_HXX
#define LIB_TFEL_MATERIAL_POWERLAWSTRAINBASEDNUCLEATIONMODEL_HXX

#include <tuple>
#include <ostream>

namespace tfel {

  namespace material {

    /*!
     * \brief parameters of the strain version of the nucleation model
     * proposed by Chu and Needleman.
     *
     * Chu, C. C., and A. Needleman. .
     * “Void Nucleation Effects in Biaxially Stretched Sheets.”
     * Journal of Engineering Materials and Technology 102 (3): 249–56
     *
     * \tparam real: numeric type
     */
    template <typename real>
    struct PowerLawStrainBasedNucleationModelParameters {
      //! \brief factor in front of power law
      real fn;
      //! \brief minimal plastic strain
      real en;
      //! \brief exponent of the power law
      real m;
    };  // end of struct PowerLawStrainBasedNucleationModelParameters

    /*!
     * \brief stream operator
     * \return the output stream
     * \param[in] os: output stream
     * \param[in] p: parameters
     */
    template <typename real>
    std::ostream& operator<<(
        std::ostream&,
        const PowerLawStrainBasedNucleationModelParameters<real>&);
    /*!
     * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
     * the power law nucleation model
     *
     * \f[
     * A_{n} = f_{n}\langle\dfrac{p}{\epsilon_{n}}-1\rangle^{m}
     * \f]
     *
     * \tparam real: numeric type
     *
     * \param[in] params: parameters
     * \param[in] p: equivalent plastic strain
     */
    template <typename real>
    real computePowerLawStrainBasedNucleationModelPorosityRateFactor(
        const PowerLawStrainBasedNucleationModelParameters<real>&, const real);

    /*!
     * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
     * the power law nucleation model and its derivative
     * \f$\frac{\partial A_{n}}{\partial p}\f$ with respect to the
     * equivalent plastic strain \f$p\f$.
     *
     * \f[
     * A_{n} = f_{n}\langle\dfrac{p}{\epsilon_{n}}-1\rangle^{m}
     * \f]
     *
     * \tparam real: numeric type
     *
     * \param[in] params: parameters
     * \param[in] p: equivalent plastic strain
     */
    template <typename real>
    std::tuple<real, real>
    computePowerLawStrainBasedNucleationModelPorosityRateFactorAndDerivative(
        const PowerLawStrainBasedNucleationModelParameters<real>&, const real);

    /*!
     * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
     * the power law nucleation model
     *
     * \f[
     * A_{n} = f_{n}\langle\dfrac{p}{\epsilon_{n}}-1\rangle^{m}
     * \f]
     *
     * \tparam real: numeric type
     *
     * \param[in] params: parameters
     * \param[in] p: equivalent plastic strain at the beginning of the time
     * step. \param[in] dp: increment of the equivalent plastic strain.
     * \param[in] theta: parameter of the theta method.
     */
    template <typename real>
    real computePowerLawStrainBasedNucleationModelPorosityIncrement(
        const PowerLawStrainBasedNucleationModelParameters<real>&,
        const real,
        const real,
        const real);

    /*!
     * \brief computes the porosity rate factor \f$A_{n}\f$ as defined by
     * the power law nucleation model and its derivative
     * \f$\frac{\partial A_{n}}{\partial p}\f$ with respect to the
     * equivalent plastic strain \f$p\f$.
     *
     * \f[
     * A_{n} = f_{n}\langle\dfrac{p}{\epsilon_{n}}-1\rangle^{m}
     * \f]
     *
     * \tparam real: numeric type
     *
     * \param[in] params: parameters
     * \param[in] p: equivalent plastic strain
     * \param[in] p: equivalent plastic strain at the beginning of the time
     * step. \param[in] dp: increment of the equivalent plastic strain.
     * \param[in] theta: parameter of the theta method.
     */
    template <typename real>
    std::tuple<real, real>
    computePowerLawStrainBasedNucleationModelPorosityIncrementAndDerivative(
        const PowerLawStrainBasedNucleationModelParameters<real>&,
        const real,
        const real,
        const real);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/PowerLawStrainBasedNucleationModel.ixx"

#endif /* LIB_TFEL_MATERIAL_POWERLAWSTRAINBASEDNUCLEATIONMODEL_HXX */
