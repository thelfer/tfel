/*!
 * \file   mfront/include/MFront/SymbolsGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_SYMBOLSGENERATOR_HXX
#define LIB_MFRONT_SYMBOLSGENERATOR_HXX

#include <set>
#include <iosfwd>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declarations
  struct BehaviourInterfaceBase;
  struct MaterialKnowledgeDescription;
  struct BehaviourDescription;
  struct FileDescription;

  /*!
   * This class provides some helper function for behaviour
   * interfaces to generate symbols related to the behaviour.
   */
  struct MFRONT_VISIBILITY_EXPORT SymbolsGenerator {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    /*!
     * \return a name used to create symbols for the  interface
     * \param[in] i    : standard behaviour interface
     * \param[in] n : name of the behaviour as defined by the interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     * \param[in] h : modelling hypothesis
     */
    virtual std::string getSymbolName(const BehaviourInterfaceBase &,
                                      const std::string &,
                                      const Hypothesis) const;
    /*!
     * \param[in] out: output file
     * \param[in] i: standard behaviour interface
     * \param[in] fd: file description
     * \param[in] n: name of the entry point
     */
    virtual void writeFileDescriptionSymbols(std::ostream &,
                                             const BehaviourInterfaceBase &,
                                             const FileDescription &,
                                             const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i: standard behaviour interface
     * \param[in] d: material knowledge description
     * \param[in] n: name of the entry point
     */
    virtual void writeValidatorSymbol(std::ostream &,
                                      const BehaviourInterfaceBase &,
                                      const BehaviourDescription &,
                                      const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i: standard behaviour interface
     * \param[in] d: material knowledge description
     * \param[in] n: name of the entry point
     */
    virtual void writeBuildIdentifierSymbol(std::ostream &,
                                            const BehaviourInterfaceBase &,
                                            const BehaviourDescription &,
                                            const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeEntryPointSymbol(std::ostream &,
                                       const BehaviourInterfaceBase &,
                                       const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeTFELVersionSymbol(std::ostream &,
                                        const BehaviourInterfaceBase &,
                                        const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeUnitSystemSymbol(std::ostream &,
                                       const BehaviourInterfaceBase &,
                                       const std::string &,
                                       const BehaviourDescription &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeMaterialSymbol(std::ostream &,
                                     const BehaviourInterfaceBase &,
                                     const BehaviourDescription &,
                                     const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeMaterialKnowledgeTypeSymbol(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    void writeTemperatureRemovedFromExternalStateVariablesSymbol(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeSpecificSymbols(std::ostream &,
                                      const BehaviourInterfaceBase &,
                                      const BehaviourDescription &,
                                      const FileDescription &,
                                      const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] mhs  : modelling hypotheses
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void generateGeneralSymbols(std::ostream &,
                                        const BehaviourInterfaceBase &,
                                        const BehaviourDescription &,
                                        const FileDescription &,
                                        const std::set<Hypothesis> &,
                                        const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i: behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     */
    virtual void generateSymbols(std::ostream &,
                                 const BehaviourInterfaceBase &,
                                 const BehaviourDescription &,
                                 const FileDescription &,
                                 const std::string &,
                                 const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i: behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeBehaviourTypeSymbols(std::ostream &,
                                           const BehaviourInterfaceBase &,
                                           const BehaviourDescription &,
                                           const std::string &) const = 0;
    /*!
     * \param[in] out  : output file
     * \param[in] i: behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeBehaviourKinematicSymbols(std::ostream &,
                                                const BehaviourInterfaceBase &,
                                                const BehaviourDescription &,
                                                const std::string &) const = 0;
    /*!
     * \brief write the symbols associated with the source file
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] name : behaviour name
     */
    virtual void writeSourceFileSymbols(std::ostream &,
                                        const BehaviourInterfaceBase &,
                                        const BehaviourDescription &,
                                        const FileDescription &,
                                        const std::string &) const;
    /*!
     * \brief write the symbols associated with the interface name
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] name : behaviour name
     */
    virtual void writeInterfaceNameSymbols(std::ostream &,
                                           const BehaviourInterfaceBase &,
                                           const BehaviourDescription &,
                                           const FileDescription &,
                                           const std::string &) const;
    /*!
     * \param[in] out   : output file
     * \param[in] mb    : behaviour description
     * \param[in] name  : name of the behaviour as defined by the interface
     *                    (generally taking into account the material
     *                     and the behaviour name)
     * \param[in] h     : modelling hypothesis
     */
    virtual void writeIsUsableInPurelyImplicitResolutionSymbols(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::string &,
        const Hypothesis) const;
    /*!
     * \brief write symbols associated with supported modelling
     * hypotheses
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb: behaviour description
     * \param[in] mhs: modelling hypotheses supported by the interface
     * \param[in] n: name of the behaviour as defined by the interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     */
    virtual void writeSupportedModellingHypothesis(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::set<Hypothesis> &,
        const std::string &) const;
    /*!
     * \brief write the symbols associated with the gradients
     * and the associated thermodynamic forces (number, names and types)
     * \param[in] out:  output file
     * \param[in] i:    behaviour interface
     * \param[in] mb:   behaviour description
     * \param[in] name: name of the behaviour as defined by the interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     */
    virtual void writeMainVariablesSymbols(std::ostream &,
                                           const BehaviourInterfaceBase &,
                                           const BehaviourDescription &,
                                           const std::string &) const;
    /*!
     * \brief write the symbols associated with the tangent operator structure
     * \param[in] out:  output file
     * \param[in] i:    behaviour interface
     * \param[in] mb:   behaviour description
     * \param[in] name: name of the behaviour as defined by the interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     *
     * \note By default, no tangent operator blocks are defined for finite
     * strain behaviours (including strain based behaviours based on a strain
     * measure different than the linearised strain), because most consistent
     * tangent operators used by the targeted solvers are not the derivative of
     * the Cauchy stress with respect to the deformation gradient.
     */
    virtual void writeTangentOperatorSymbols(std::ostream &,
                                             const BehaviourInterfaceBase &,
                                             const BehaviourDescription &,
                                             const std::string &) const;
    /*!
     * \brief write symbols associated with material properties for the
     * given hypothesis
     * \param[in] out : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb  : behaviour description
     * \param[in] n   : name of the behaviour as defined by the interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     * \param[in] h   : modelling hypothesis
     */
    virtual void writeMaterialPropertiesSymbols(std::ostream &,
                                                const BehaviourInterfaceBase &,
                                                const BehaviourDescription &,
                                                const std::string &,
                                                const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeStateVariablesSymbols(std::ostream &,
                                            const BehaviourInterfaceBase &,
                                            const BehaviourDescription &,
                                            const std::string &,
                                            const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeExternalStateVariablesSymbols(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::string &,
        const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeParametersSymbols(std::ostream &,
                                        const BehaviourInterfaceBase &,
                                        const BehaviourDescription &,
                                        const std::string &,
                                        const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeInitializeFunctionsSymbols(std::ostream &,
                                                 const BehaviourInterfaceBase &,
                                                 const BehaviourDescription &,
                                                 const std::string &,
                                                 const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writePostProcessingsSymbols(std::ostream &,
                                             const BehaviourInterfaceBase &,
                                             const BehaviourDescription &,
                                             const std::string &,
                                             const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeParameterDefaultValueSymbols(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::string &,
        const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeBoundsSymbols(std::ostream &,
                                    const BehaviourInterfaceBase &,
                                    const BehaviourDescription &,
                                    const std::string &,
                                    const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writePhysicalBoundsSymbols(std::ostream &,
                                            const BehaviourInterfaceBase &,
                                            const BehaviourDescription &,
                                            const std::string &,
                                            const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeRequirementsSymbols(std::ostream &,
                                          const BehaviourInterfaceBase &,
                                          const BehaviourDescription &,
                                          const std::string &,
                                          const Hypothesis) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : behaviour name
     */
    virtual void writeSymmetryTypeSymbols(std::ostream &,
                                          const BehaviourInterfaceBase &,
                                          const BehaviourDescription &,
                                          const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : behaviour name
     */
    virtual void writeElasticSymmetryTypeSymbols(std::ostream &,
                                                 const BehaviourInterfaceBase &,
                                                 const BehaviourDescription &,
                                                 const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeAdditionalSymbols(std::ostream &,
                                        const BehaviourInterfaceBase &,
                                        const BehaviourDescription &,
                                        const FileDescription &,
                                        const std::string &,
                                        const Hypothesis) const = 0;
    /*!
     * \brief write an integer symbol stating if the behaviour can compute the
     * internal energy
     * \param[out] f   : output stream
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeComputesInternalEnergySymbol(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::string &,
        const Hypothesis) const;
    /*!
     * \brief write an integer symbol stating if the behaviour can compute the
     * dissipated energy
     * \param[out] f   : output stream
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] h    : modelling hypothesis
     */
    virtual void writeComputesDissipatedEnergySymbol(
        std::ostream &,
        const BehaviourInterfaceBase &,
        const BehaviourDescription &,
        const std::string &,
        const Hypothesis) const;
    /*!
     * \param[out] f    : output stream
     * \param[in] i    : standard behaviour interface
     * \param[in]  name : name of the umat function
     * \param[in]  h    : modelling hypothesis
     * \param[in]  v    : variables external names and array size
     * \param[in]  t    : class of variable treated used as the name for the
     * generated symbol (for example,  'InternalStateVariables')
     * \param[in]  o    : offset
     */
    virtual void writeExternalNames(std::ostream &,
                                    const BehaviourInterfaceBase &,
                                    const std::string &,
                                    const Hypothesis &,
                                    const std::vector<std::string> &,
                                    const std::string &) const;
    /*!
     * \return if the interface treats the strain measure internally or if the
     * strain measure is handled by the calling code.
     */
    virtual bool handleStrainMeasure() const = 0;
    //! \brief destructor
    virtual ~SymbolsGenerator();

   protected:
    /*!
     * \brief write a symbol containing the types of a list of variables
     * \param[out] out: output stream
     * \param[in]  i: standard behaviour interface
     * \param[in]  name: name of the umat function
     * \param[in]  h: modelling hypothesis
     * \param[in]  variables: variables to be treated
     * \param[in]  variables_identifier: class of variable treated used as the
     * name for the generated symbol (for example,  'InternalStateVariables')
     */
    virtual void writeVariablesTypesSymbol(std::ostream &,
                                           const BehaviourInterfaceBase &,
                                           const std::string &,
                                           const Hypothesis,
                                           const VariableDescriptionContainer &,
                                           const std::string &) const;
    /*!
     * \param[out] f: output stream
     * \param[in]  s: symbol name
     * \param[in]  v: array of strings
     */
    virtual void writeArrayOfStringsSymbol(
        std::ostream &,
        const std::string &,
        const std::vector<std::string> &) const;
    /*!
     * \param[out] f: output stream
     * \param[in]  s: symbol name
     * \param[in]  v: array of ints
     */
    virtual void writeArrayOfIntsSymbol(std::ostream &,
                                        const std::string &,
                                        const std::vector<int> &) const;

  };  // end of struct SymbolsGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_SYMBOLSGENERATOR_HXX */
