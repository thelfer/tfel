/*!
 * \file   AbaqusInterfaceBase.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUSINTERFACEBASE_HXX
#define LIB_MFRONT_ABAQUSINTERFACEBASE_HXX

#include <string>
#include <iosfwd>

#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief base class for the interfaces to:
   * - the Abaqus Standard finite element solver (see `AbaqusInterface`)
   * - the Abaqus Explicit finite element solver (see `AbaqusExplicitInterface`)
   */
  struct AbaqusInterfaceBase : public UMATInterfaceBase {
    /*!
     * This enum defines the various finite strain strategies
     * available to resuse HPP laws into finite strain computations.
     *
     * The "finite rotation small strain" strategy is defined in the
     * reference of the Code-Aster finite element software:
     * [R5.03.22] Loi de comportement en grandes rotations et petites
     * déformations
     *
     * The logarithmic strain strategy has been introduced by Miehe,
     * Apel and Lambrecht:
     * Miehe C., Apel N., Lambrecht M.: Anisotropic additive plasticity in the
     * logarithm strain space : modular kinematic formulation and implementation
     * based on incremental minimization principles for standard materials.,
     * Computer Methods in Applied Mechanics and Engineering, 191, pp.5383-5425,
     * 2002. This strategy is also developped int the reference of the
     * Code-Aster finite element software: [R5.03.24] Modèles de grandes
     * déformations GDEF_LOG et GDEF_HYPO_ELAS
     */
    enum FiniteStrainStrategy {
      UNDEFINEDSTRATEGY,
      NATIVEFINITESTRAINSTRATEGY,
      FINITEROTATIONSMALLSTRAIN,
      MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN
    };  // end of enum FiniteStrainStrategy
    /*!
     * This enum defines how orthotropy is handled by the behaviour
     */
    enum OrthotropyManagementPolicy {
      UNDEFINEDORTHOTROPYMANAGEMENTPOLICY,
      NATIVEORTHOTROPYMANAGEMENTPOLICY,
      MFRONTORTHOTROPYMANAGEMENTPOLICY
    };  // end of enum FiniteStrainStrategy
    //! destructor
    virtual ~AbaqusInterfaceBase();

   protected:
    /*!
     * \brief return the state variable offset used for variables used
     * internally by the abaqus interface
     * \param[in] mb: behaviour description
     * \param[in] : behaviour description
     */
    virtual unsigned short getStateVariablesOffset(const BehaviourDescription&,
                                                   const Hypothesis) const;
    /*!
     * \param[in] k  : keyword treated
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool, tokens_iterator> treatCommonKeywords(
        const std::string&, tokens_iterator, const tokens_iterator);
    /*!
     * \return the list of supported keywords
     */
    std::vector<std::string> getCommonKeywords(void) const;
    /*!
     * \return the name of the generated library
     * \param[in] mb : behaviour description
     */
    virtual std::string getLibraryName(
        const BehaviourDescription&) const override;
    /*!
     * \brief write a  specialisation of the AbaqusTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void writeAbaqusBehaviourTraits(std::ostream&,
                                            const BehaviourDescription&,
                                            const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void writeUMATxxAdditionalSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&,
        const FileDescription&) const override;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void writeUMATxxSpecificSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&,
        const FileDescription&) const override;
    /*!
     * \param[in] out : output file
     */
    virtual void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const override;

    virtual std::string getModellingHypothesisTest(
        const Hypothesis) const override;

    virtual std::map<UMATInterfaceBase::Hypothesis, std::string>
    gatherModellingHypothesesAndTests(
        const BehaviourDescription&) const override;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
                      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription&,
                                const Hypothesis) const override;
    /*!
     * \return the name of the function generated by the interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string getFunctionName(const std::string&) const override;
    /*!
     * \return the name of the function generated by the interface
     * \param[in] n: name of the behaviour as defined by interface
     *               (generally taking into account the material
     *               and the behaviour name)
     * \param[in] h: modelling hypothesis
     */
    virtual std::string getFunctionNameForHypothesis(const std::string&,
                                                     const Hypothesis) const;
    /*!
     * \return the list of modelling hypotheses treated by the interface
     * \param[in] mb : behaviour description
     */
    virtual std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    /*!
     * \return the input file example
     * \param[in] mb: behaviour description
     * \param[in] fd: file description
     * \param[in] b: if true, write the example for
     * Abaqus-Standard. If false, write the example for
     * Abaqus-Explicit
     */
    virtual void writeInputFileExample(const BehaviourDescription&,
                                       const FileDescription&,
                                       const bool) const;

    virtual void writeDepvar(std::ostream&,
                             int&,
                             const Hypothesis&,
                             const VariableDescription&,
                             const std::string&) const;
    //! selected finite strain strategy
    FiniteStrainStrategy fss = UNDEFINEDSTRATEGY;
    //! selected orthotropy management policy
    OrthotropyManagementPolicy omp = UNDEFINEDORTHOTROPYMANAGEMENTPOLICY;
  };  // end of struct AbaqusInterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABAQUSINTERFACEBASE_HXX_ */
