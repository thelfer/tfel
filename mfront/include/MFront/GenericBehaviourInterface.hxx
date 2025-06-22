/*!
 * \file   mfront/include/GenericBehaviourInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX
#define LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX

#include "MFront/StandardBehaviourInterface.hxx"

namespace mfront {

  /*!
   * \brief a generic behaviour interface
   * See https://github.com/thelfer/MFrontGenericInterfaceSupport
   */
  struct GenericBehaviourInterface : public StandardBehaviourInterface {
    //! \return the interface name
    static std::string getName();
    std::string getInterfaceName() const override;
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;

    void writeBehaviourConstructorHeader(std::ostream&,
                                         const BehaviourDescription&,
                                         const Hypothesis,
                                         const std::string&) const override;
    void writeBehaviourConstructorBody(std::ostream&,
                                       const BehaviourDescription&,
                                       const Hypothesis) const override;

    void writeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;

    void exportMechanicalData(std::ostream&,
                              const Hypothesis,
                              const BehaviourDescription&) const override;

    std::string getFunctionNameBasis(const std::string&) const override;

    virtual std::string getLibraryName(const BehaviourDescription&) const;

    //! destructor
    ~GenericBehaviourInterface() override;

   protected:
    /*!
     * \return the name of the function generated for the given hypothesis
     * \param[in] base name for the function to be generated
     */
    virtual std::string getFunctionNameForHypothesis(const std::string&,
                                                     const Hypothesis) const;
    /*!
     * \brief write the generation of an MTest file for the given hypothesis
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void generateMTestFile(std::ostream&,
                                   const BehaviourDescription&,
                                   const Hypothesis) const;
    /*!
     * \brief write the pre-processings associated with a standard finite strain
     * behaviour.
     *
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void writeStandardFiniteStrainBehaviourPreProcessing(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;
    /*!
     * \brief write the pre-processings associated with the Green-Lagrange
     * strain measure.
     *
     * This methods calls `writeStrainMeasureCommonPreProcessing1` method, add
     * the pre-processings specific the Green-Lagrange strain measure, and
     * finally calls the `writeStrainMeasureCommonPreProcessing2` method.
     *
     *
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void writeGreenLagrangeStrainMeasurePreProcessing(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;
    /*!
     * \brief write the pre-processings associated with the Hencky strain
     * measure.
     *
     * This methods calls `writeStrainMeasureCommonPreProcessing1` method, add
     * the pre-processings specific the Green-Lagrange strain measure, and
     * finally calls the `writeStrainMeasureCommonPreProcessing2` method.
     *
     * This methods introduces the following variables:
     *
     * - `lgh0` and `lgh1` which are instances of the `LogarithmicStrainHandler`
     *    at the beginning of the time step and at the end of the time step
     *    respectively.
     *
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void writeHenckyStrainMeasurePreProcessing(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;
    /*!
     * \brief this method introduces and initialize some variables used to
     * handle strain measures:
     */
    virtual void writeStrainMeasureCommonPreProcessing1(std::ostream&,
                                                        const Hypothesis) const;

    virtual void writeStrainMeasureCommonPreProcessing2(
        std::ostream&, const std::string&) const;
    /*!
     * \brief write the post-processings associated with the Green-Lagrange
     * strain measure.
     *
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void writeGreenLagrangeStrainMeasurePostProcessing(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;
    /*!
     * \brief write the post-processings associated with the Hencky strain
     * measure.
     *
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void writeHenckyStrainMeasurePostProcessing(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;
    /*!
     * \brief write the post-processings associated with a standard finite
     * strain behaviour.
     *
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void writeStandardFiniteStrainBehaviourPostProcessing(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;

  };  // end of struct GenericBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX */
