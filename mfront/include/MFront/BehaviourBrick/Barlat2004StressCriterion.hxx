/*!
 * \file   include/MFront/BehaviourBrick/Barlat2004StressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_BARLAT2004STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_BARLAT2004STRESSCRITERION_HXX

#include <array>
#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief This class describes the Barlat 2004 stress criterion which is
     * base on the following equivalent stress:
     * \f[
     * \phi\left(\underline{\bf s}\right) =
     * \sqrt[a]{
     *   \frac{1}{4}\left(
     *   \sum_{i=0}^{3}
     *   \sum_{j=0}^{3}
     *   \left|s'_{i}-s''_{j}\right|^{a}
     *   \right)
     * }
     * \f]
     * where \f$s'_{i}\f$ and \f$s''_{j}\f$ are the eigenvalues
     * computed from two tensors \f$\underline{\bf s}'\f$ and
     * \f$\underline{\bf s}''\f$ resulting from a linear
     * transformation of the stress tensor:
     * \f[
     * \left\{
     * \begin{aligned}
     *  \underline{\bf s}'  &= \underline{\underline{L}}'
     * \,\colon\,\underline{\bf s} \\
     *  \underline{\bf s}'' &=
     * \underline{\underline{L}}''\,\colon\,\underline{\bf s} \\ \end{aligned}
     * \right.
     * \f]
     *
     * The linear transformations \f$\underline{\underline{L}}'\f$ and
     * \f$\underline{\underline{L}}''\f$ can be deduced from linear
     * transformations \f$\underline{\underline{C}}'\f$ and
     * \f$\underline{\underline{C}}''\f$. on the deviator of the
     * stress as follows:
     *
     * \f[
     * \begin{aligned}
     * \underline{L}'
     * &=\underline{\underline{C}}'\,\colon\,\underline{\underline{M}} \\
     * \underline{L}''
     * &=\underline{\underline{C}}'\,\colon\,\underline{\underline{M}} \\
     * \end{aligned}
     * \f]
     * where \f$\underline{\underline{M}}\f$ is the transformation of
     * the stress to its deviator:
     * \f[
     * \underline{\underline{M}}=\underline{\underline{I}}-\frac{1}{3}\underline{I}\,\otimes\,\underline{I}
     * \f]
     *
     * The linear transformations \f$\underline{\underline{C}}'\f$ and
     * \f$\underline{\underline{C}}''\f$ are defined by \f$9\f$
     * coefficients (each) which describe the material orthotropy, as
     * follows:
     *
     * \f[
     * \underline{\underline{C}}'=
     * \begin{pmatrix}
     * 0 & -c'_{12} & -c'_{13} & 0 & 0 & 0 \\
     * -c'_{21} & 0 & -c'_{23} & 0 & 0 & 0 \\
     * -c'_{31} & -c'_{32} & 0 & 0 & 0 & 0 \\
     * 0 & 0 & 0 & c'_{44} & 0 & 0 \\
     * 0 & 0 & 0 & 0 & c'_{55} & 0 \\
     * 0 & 0 & 0 & 0 & 0 & c'_{66} \\
     * \end{pmatrix}
     * \quad
     * \text{and}
     * \quad
     * \underline{\underline{C}}''=
     * \begin{pmatrix}
     * 0 & -c''_{12} & -c''_{13} & 0 & 0 & 0 \\
     * -c''_{21} & 0 & -c''_{23} & 0 & 0 & 0 \\
     * -c''_{31} & -c''_{32} & 0 & 0 & 0 & 0 \\
     * 0 & 0 & 0 & c''_{44} & 0 & 0 \\
     * 0 & 0 & 0 & 0 & c''_{55} & 0 \\
     * 0 & 0 & 0 & 0 & 0 & c''_{66} \\
     * \end{pmatrix}
     * \f]
     */
    struct Barlat2004StressCriterion final : StressCriterionBase {
      std::vector<OptionDescription> getOptions() const override;
      std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&,
                      const Role) override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const std::string&,
                        const Role) override;
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
      ~Barlat2004StressCriterion() override;

     protected:
      //! coefficients of the first linear transformation
      std::array<MaterialProperty, 9u> l1;
      //! coefficients of the second linear transformation
      std::array<MaterialProperty, 9u> l2;
      //! Barlat exponent
      MaterialProperty a;
    };  // end of struct Barlat2004StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_BARLAT2004STRESSCRITERION_HXX */
