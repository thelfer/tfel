/*!
 * \file   mfront/include/MFront/AbaqusExplicitInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   16 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUSEXPLICITINTERFACE_HXX
#define LIB_MFRONT_ABAQUSEXPLICITINTERFACE_HXX

#include "MFront/AbaqusInterfaceBase.hxx"

namespace mfront {

  /*!
   * Interface dedicated to the Abaqus-Explicit finite element solver
   */
  struct AbaqusExplicitInterface : public AbaqusInterfaceBase {
    /*!
     * \return the name of the interface
     */
    static std::string getName();
    /*!
     * \brief write output files
     * \param[in] mb : behaviour description
     * \param[in] fd : mfront file description
     */
    virtual void endTreatment(const BehaviourDescription&,
                              const FileDescription&) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : behaviour description
     */
    virtual void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \param[out] d  : target description
     * \param[in]  bd : behaviour description
     */
    virtual void getTargetsDescription(TargetsDescription&,
                                       const BehaviourDescription&) override;
    //! destructor
    virtual ~AbaqusExplicitInterface();

   protected:
    /*!
     * \return the name of the interface
     */
    virtual std::string getInterfaceName() const override;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile           : output file
     * \param[in] mb                      : behaviour description
     * \param[in] initStateVarsIncrements : constructor part assigning
     *                                      default value (zero) to state
     *                                      variable increments
     */
    virtual void writeBehaviourConstructor(std::ostream&,
                                           const BehaviourDescription&,
                                           const std::string&) const override;
    /*!
     * \brief write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] h             : modelling hypothesis
     * \param[in] mb            : behaviour description
     */
    virtual void writeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * write the initialisation of the driving variables
     * \param[in] os : output file
     * \param[in] mb : behaviour description
     */
    virtual void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataDrivingVariableSetter(
        std::ostream&,
        const DrivingVariable&,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the initialisation of a thermodynamic force
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataThermodynamicForceSetter(
        std::ostream&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const override;

    virtual void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : behaviour description
     */
    virtual void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeIntegrationDataDrivingVariableSetter(
        std::ostream&,
        const DrivingVariable&,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] h  : hypothesis
     * \param[in] mb : behaviour description
     */
    virtual void exportMechanicalData(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * \param[in,out] mb: behaviour description
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    void writeUMATxxBehaviourTypeSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&) const override;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    void writeUMATxxBehaviourKinematicSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&) const override;
    /*!
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeChecks(std::ostream&,
                             const BehaviourDescription&,
                             const std::string&,
                             const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeNativeBehaviourCall(std::ostream&,
                                          const BehaviourDescription&,
                                          const std::string&,
                                          const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeNativeBehaviourIntegration(std::ostream&,
                                                 const BehaviourDescription&,
                                                 const std::string&,
                                                 const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeFiniteRotationSmallStrainBehaviourCall(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&,
        const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeFiniteRotationSmallStrainIntegration(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&,
        const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeLogarithmicStrainBehaviourCall(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&,
        const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeLogarithmicStrainIntegration(std::ostream&,
                                                   const BehaviourDescription&,
                                                   const std::string&,
                                                   const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeFiniteStrainBehaviourCall(std::ostream&,
                                                const BehaviourDescription&,
                                                const std::string&,
                                                const Hypothesis) const;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeFiniteStrainIntegration(std::ostream&,
                                              const BehaviourDescription&,
                                              const std::string&,
                                              const Hypothesis) const;
    /*!
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     * \param[in]  h:   modelling hypothesis
     */
    virtual void writeComputeElasticPrediction(std::ostream&,
                                               const BehaviourDescription&,
                                               const std::string&,
                                               const Hypothesis) const;
    /*!
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     */
    virtual void writeIntegrateLoop(std::ostream&,
                                    const BehaviourDescription&) const;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABAQUSEXPLICITINTERFACE_HXX */
