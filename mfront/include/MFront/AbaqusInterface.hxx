/*!
 * \file   mfront/include/MFront/AbaqusInterface.hxx
 * \brief  This file declares the AbaqusInterface class
 * \author Thomas Helfer
 * \date 17/01/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUSINTERFACE_HXX
#define LIB_MFRONT_ABAQUSINTERFACE_HXX

#include <string>
#include <iosfwd>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/AbaqusInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief the interface the Abaqus Standard finite element solver
   */
  struct AbaqusInterface : public AbaqusInterfaceBase {
    //! \return the name of the interface
    [[nodiscard]] static std::string getName();
    //
    [[nodiscard]] std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    /*!
     * \param[out] d  : target description
     * \param[out] bd : behaviour description
     */
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;
    //! \brief destructor
    ~AbaqusInterface() override;

   protected:
    [[nodiscard]] std::string getInterfaceName() const override;
    [[nodiscard]] std::vector<std::pair<std::string, std::string>>
    getBehaviourDataConstructorAdditionalVariables() const override;
    void completeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeBehaviourDataGradientSetter(
        std::ostream&,
        const Gradient&,
        const SupportedTypes::TypeSize) const override;
    void writeIntegrationDataGradientSetter(
        std::ostream&,
        const Gradient&,
        const SupportedTypes::TypeSize) const override;
    void exportThermodynamicForce(
        std::ostream&,
        const std::string&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void writeBehaviourDataThermodynamicForceSetter(
        std::ostream&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     * \param[in] sfeh: name of the function used to handle the stress
     *                  expansion
     * \param[in] h:    modelling hypothesis
     */
    virtual void writeFunctionBase(std::ostream&,
                                   const BehaviourDescription&,
                                   const std::string&,
                                   const std::string&,
                                   const Hypothesis) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     * \param[in] h:    modelling hypothesis
     */
    virtual void writeSmallStrainFunction(std::ostream&,
                                          const BehaviourDescription&,
                                          const std::string&,
                                          const Hypothesis) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     * \param[in] h:    modelling hypothesis
     */
    virtual void writeFiniteStrainFunction(std::ostream&,
                                           const BehaviourDescription&,
                                           const std::string&,
                                           const Hypothesis) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     * \param[in] h:    modelling hypothesis
     */
    virtual void writeFiniteRotationSmallStrainFunction(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&,
        const Hypothesis) const;
    /*!
     * \brief write the call to the base function
     * \param[in] out:  output file
     * \param[in] mb:   mechanical behaviour description
     * \param[in] name: name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                  and the behaviour name)
     * \param[in] h:    modelling hypothesis
     */
    virtual void writeMieheApelLambrechtLogarithmicStrainFunction(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&,
        const Hypothesis) const;
    /*!
     * \brief boolean stating the we want a comparison of the user
     * defined tangent operator with a numerical approximation.
     */
    bool compareToNumericalTangentOperator = false;
    /*!
     * \brief perturbation value used for the computation of the
     * numerical approximation of the tangent operator
     */
    double strainPerturbationValue = 1.e-6;
    /*!
     * \brief comparison critera value used for comparing of the user
     * defined tangent operator with a numerical approximation.
     */
    double tangentOperatorComparisonCriterion = 1.e7;
  };  // end of AbaqusInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABAQUSINTERFACE_HXX */
