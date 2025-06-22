/*!
 * \file   include/MFront/BehaviourBrick/Hill1948StressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HILL1948STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HILL1948STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief This class describes the Hill 1948 criterion based on
     * the Hill stress \f$\sigma^{\mathrm{H}}_{\mathrm{eq}}\f$:
     * \f[
     * \sigma^{\mathrm{H}}_{\mathrm{eq}}=
     * H_F\left(\sigma_{11}-\sigma_{22}\right)^2+
     * H_G\left(\sigma_{22}-\sigma_{33}\right)^2+
     * H_H\left(\sigma_{33}-\sigma_{11}\right)^2+
     * 2H_L\sigma_{12}^{2}+
     * 2H_M\sigma_{13}^{2}+
     * 2H_N\sigma_{23}^{2}
     * \f]
     *
     * The previous expression can by rewritten by introducing the Hill tensor
     * \f$\underline{\underline{H}}\f$ as:
     * \f[
     * \sigma^{\mathrm{H}}_{\mathrm{eq}}=
     * \underline{\sigma}\,\colon\,\underline{\underline{H}}\,\colon\,\underline{\sigma}
     * \f]
     * with:
     * \f[
     * \underline{\underline{H}}=
     * \left(
     * \begin{array}{cccccc}
     * H_F+H_H & -H_F  & -H_H  & 0 & 0 & 0 \\
     * -H_F  & H_G+H_F & -H_G  & 0 & 0 & 0 \\
     * -H_H  & -H_G  & H_H+H_G & 0 & 0 & 0 \\
     * 0   & 0   & 0   & H_L & 0 & 0 \\
     * 0   & 0   & 0   & 0 & H_M & 0 \\
     * 0   & 0   & 0   & 0 & 0 & H_N \\
     * \end{array}
     * \right)
     * \f]
     */
    struct Hill1948StressCriterion final : StressCriterionBase {
      std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&,
                      const Role) override;
      std::vector<OptionDescription> getOptions() const override;
      std::string computeElasticPrediction(
          const std::string&,
          const BehaviourDescription&,
          const StressPotential&) const override;
      std::string computeCriterion(const std::string&,
                                   const BehaviourDescription&,
                                   const StressPotential&) const override;
      std::string computeNormal(const std::string&,
                                const BehaviourDescription&,
                                const StressPotential&,
                                const Role) const override;
      std::string computeNormalDerivative(const std::string&,
                                          const BehaviourDescription&,
                                          const StressPotential&,
                                          const Role) const override;
      bool isNormalDeviatoric() const override;
      //! destructor
      ~Hill1948StressCriterion() override;
    };  // end of struct Hill1948StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HILL1948STRESSCRITERION_HXX */
