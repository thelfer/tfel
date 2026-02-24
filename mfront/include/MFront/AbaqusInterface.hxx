/*!
 * \file   mfront/include/MFront/AbaqusInterface.hxx
 * \brief  This file declares the AbaqusInterface class
 * \author Thomas Helfer
 * \date   17 Jan 2007
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
    static std::string getName();
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
    std::pair<bool, tokens_iterator> treatKeyword(
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
    //! destructor
    ~AbaqusInterface() override;

   protected:
    std::string getInterfaceName() const override;
    /*!
     * write the initialisation of the driving variables
     * \param[out] os: output file
     * \param[in]  mb: mechanical behaviour description
     */
    virtual void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataGradientSetter(
        std::ostream&,
        const Gradient&,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeIntegrationDataGradientSetter(
        std::ostream&,
        const Gradient&,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the instruction of exporting a thermodynamic force in an
     * array \param[out] out : output stream \param[in]  a   : array name
     * \param[in]  f   : thermodynamic force
     * \param[in]  o   : thermodynamic force offset
     */
    virtual void exportThermodynamicForce(
        std::ostream&,
        const std::string&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
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
    std::vector<std::pair<std::string, std::string>>
    getBehaviourDataConstructorAdditionalVariables() const override;
    void completeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
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
