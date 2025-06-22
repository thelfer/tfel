/*!
 * \file
 * include/TFEL/Material/LogarithmicStrainComputeAxialStrainIncrementElasticPrediction.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14/05/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LOGARITHMICSTRAINCOMPUTEAXIALSTRAININCREMENTELASTICPREDICTION_HXX
#define LIB_TFEL_MATERIAL_LOGARITHMICSTRAINCOMPUTEAXIALSTRAININCREMENTELASTICPREDICTION_HXX

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/stensor.hxx"

namespace tfel {

  namespace material {

    /*!
     * \brief This method computes an elastic prediction of the
     * increment of the total logarithmic axial strain in axisymmetrical
     * generalised plane strain.
     *
     * This increment \f$\Delta\varepsilon_{zz}^{\mathrm{to,log}}\f$
     * satisfies this following equation:
     * \f[
     * \theta\,D_{11}\,\Delta\varepsilon_{zz}^{\mathrm{to,log}}+
     * \theta\,D_{10}\,\Delta\varepsilon_{rr}^{\mathrm{to,log}}+
     * \theta\,D_{12}\,\Delta\varepsilon_{\theta\theta}^{\mathrm{to,log}}=
     * \exp\left(
     * \left.\varepsilon_{zz}^{\mathrm{to,log}}\right|_{t}+
     * \theta\,\Delta\varepsilon_{zz}^{\mathrm{to,log}}
     * \right)\,\Pi_{zz}
     * -\left.T_{zz}\right|_{t}
     * \f]
     * where \f$\left.T_{zz}\right|_{t}\f$ is the value of the
     * logarithmic stress at the beginning of the time step.
     * \f$\left.T_{zz}\right|_{t}\f$ is computed as follows:
     * \f[
     * \left.T_{zz}\right|_{t} =
     * D_{11}\,\left.\varepsilon_{zz}^{\mathrm{el}}\right|_{t}+
     * D_{10}\,\left.\varepsilon_{rr}^{\mathrm{el}}\right|_{t}+
     * D_{12}\,\left.\varepsilon_{\theta\theta}^{\mathrm{el}}\right|_{t}
     * \f]
     *
     * \param[in] D10: component of the elastic stiffness tensor
     * \param[in] D11: component of the elastic stiffness tensor
     * \param[in] D12: component of the elastic stiffness tensor
     * \param[in] eel: elastic strain at the beginning of the time step
     * \param[in] eto_zz: axial component of the logarithmic strain at the
     *                    beginning of the time step
     * \param[in] deto: total (in plane) increment of the logarithmic
     *                  strain. Axially, deto(1) contains the strains generated
     *                  by the stress-free expansions, if any.
     * \param[in] Pzz: axial component of the first Piola-Kirchoff stress
     *                 at \f$t+\theta\,\Delta\,t\f$
     * \param[in] t: theta value
     * \param[in] e: criterion value
     * \return the elastic prediction of the axial stress.
     */
    template <typename stress>
    tfel::typetraits::base_type<stress>
    computeAxialStrainIncrementElasticPrediction(
        const stress,
        const stress,
        const stress,
        const tfel::math::stensor<1u, tfel::typetraits::base_type<stress>>&,
        const tfel::typetraits::base_type<stress>,
        const tfel::math::stensor<1u, tfel::typetraits::base_type<stress>>&,
        const stress,
        const tfel::typetraits::base_type<stress>,
        const tfel::typetraits::base_type<stress>);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/LogarithmicStrainComputeAxialStrainIncrementElasticPrediction.ixx"

#endif /* LIB_TFEL_MATERIAL_LOGARITHMICSTRAINCOMPUTEAXIALSTRAININCREMENTELASTICPREDICTION_HXX \
        */
