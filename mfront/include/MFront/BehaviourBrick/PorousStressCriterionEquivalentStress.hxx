/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/PorousStressCriterionEquivalentStress.hxx
 * \brief  This file declares the PorousStressCriterionEquivalentStress class
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_POROUSSTRESSCRITERIONEQUIVALENTSTRESS_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_POROUSSTRESSCRITERIONEQUIVALENTSTRESS_HXX

#include <string>
#include <memory>
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"

namespace mfront::bbrick {

  /*!
   * \brief an helper class describing the equivalent stress on which a
   * generalised porous stress criterion is built.
   *
   * A generalised porous stress criterion is a porous stress criterion in
   * which the von Mises stress is replaced by an equivalent stress chosen by
   * the user among the available stress criteria. This class handles the
   * `equivalent_stress` option of such a criterion and generates the calls to
   * the functions of the `TFEL/Material` library, which are assumed to have
   * the following names and arguments, `<name>` being the name of the porous
   * stress criterion:
   *
   * - `compute<name>Stress(s, seq, f, params, seps)`,
   * - `compute<name>StressNormal(s, seq, dseq_ds, f, params, seps)`,
   * - `compute<name>StressSecondDerivative(s, seq, dseq_ds, d2seq_dsds, f,
   *   params, seps)`,
   *
   * where `seq`, `dseq_ds` and `d2seq_dsds` are the equivalent stress and its
   * first and second derivatives with respect to the stress.
   */
  struct MFRONT_VISIBILITY_EXPORT PorousStressCriterionEquivalentStress {
    //! \brief a simple alias
    using Role = StressCriterion::Role;
    //! \return the description of the `equivalent_stress` option
    [[nodiscard]] static OptionDescription getOption();
    /*!
     * \brief constructor
     * \param[in] n: name of the porous stress criterion
     */
    explicit PorousStressCriterionEquivalentStress(const std::string&);
    /*!
     * \brief create and initialize the equivalent stress
     * \param[in,out] bd: behaviour description
     * \param[in,out] dsl: abstract behaviour dsl
     * \param[in] id: flow id
     * \param[in] d: options of the porous stress criterion
     * \param[in] r: role of the porous stress criterion
     */
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const tfel::utilities::DataMap&,
                    const Role);
    /*!
     * \brief method called at the end of the input file processing
     * \param[in] bd: behaviour description
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] id: flow id
     * \param[in] r: role of the porous stress criterion
     */
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const Role);
    /*!
     * \return the code computing the elastic prediction of the porous stress
     * criterion.
     * \param[in] id: flow id
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     */
    [[nodiscard]] std::string computeElasticPrediction(
        const std::string&,
        const BehaviourDescription&,
        const StressPotential&) const;
    /*!
     * \return the code computing the porous stress criterion.
     * \param[in] id: flow id
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     */
    [[nodiscard]] std::string computeCriterion(const std::string&,
                                               const BehaviourDescription&,
                                               const StressPotential&) const;
    /*!
     * \return the code computing the porous stress criterion and its normal.
     * \param[in] id: flow id
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     * \param[in] r: role of the porous stress criterion
     */
    [[nodiscard]] std::string computeNormal(const std::string&,
                                            const BehaviourDescription&,
                                            const StressPotential&,
                                            const Role) const;
    /*!
     * \return the code computing the porous stress criterion, its normal and
     * the derivatives of the normal.
     * \param[in] id: flow id
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     * \param[in] r: role of the porous stress criterion
     */
    [[nodiscard]] std::string computeNormalDerivative(
        const std::string&,
        const BehaviourDescription&,
        const StressPotential&,
        const Role) const;
    //! \brief destructor
    ~PorousStressCriterionEquivalentStress();

   private:
    //! \brief name of the porous stress criterion
    const std::string name;
    //! \brief equivalent stress
    std::shared_ptr<StressCriterion> sc;
  };  // end of struct PorousStressCriterionEquivalentStress

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POROUSSTRESSCRITERIONEQUIVALENTSTRESS_HXX \
        */
