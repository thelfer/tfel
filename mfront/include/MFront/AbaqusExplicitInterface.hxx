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
    //!\return the name of the interface
    static std::string getName();
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;
    //! destructor
    ~AbaqusExplicitInterface() override;

   protected:
    std::string getInterfaceName() const override;
    void writeBehaviourConstructorHeader(std::ostream&,
                                         const BehaviourDescription&,
                                         const Hypothesis,
                                         const std::string&) const override;
    void writeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeBehaviourDataGradientSetter(
        std::ostream&,
        const Gradient&,
        const SupportedTypes::TypeSize) const override;
    void writeBehaviourDataThermodynamicForceSetter(
        std::ostream&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const override;
    void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeIntegrationDataGradientSetter(
        std::ostream&,
        const Gradient&,
        const SupportedTypes::TypeSize) const override;
    void exportMechanicalData(std::ostream&,
                              const Hypothesis,
                              const BehaviourDescription&) const override;
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
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
