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

#ifndef LIB_MFRONT_UMATXXSYMBOLSGENERATOR_HXX
#define LIB_MFRONT_UMATXXSYMBOLSGENERATOR_HXX

#include <set>
#include <iosfwd>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct StandardBehaviourInterface;
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct FileDescription;

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
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
    virtual std::string getSymbolName(const StandardBehaviourInterface &,
                                      const std::string &,
                                      const Hypothesis) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeEntryPointSymbol(std::ostream &,
                                       const StandardBehaviourInterface &,
                                       const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeTFELVersionSymbol(std::ostream &,
                                        const StandardBehaviourInterface &,
                                        const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by the interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeMaterialSymbol(std::ostream &,
                                     const StandardBehaviourInterface &,
                                     const BehaviourDescription &,
                                     const std::string &) const;
    /*!
     * \param[in] out: output file
     * \param[in] i    : standard behaviour interface
     * \param[in] n:   name of the entry point
     */
    virtual void writeMaterialKnowledgeTypeSymbol(
        std::ostream &,
        const StandardBehaviourInterface &,
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
                                      const StandardBehaviourInterface &,
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
                                        const StandardBehaviourInterface &,
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
                                 const StandardBehaviourInterface &,
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
                                           const StandardBehaviourInterface &,
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
    virtual void writeBehaviourKinematicSymbols(
        std::ostream &,
        const StandardBehaviourInterface &,
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
                                        const StandardBehaviourInterface &,
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
                                           const StandardBehaviourInterface &,
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
        const StandardBehaviourInterface &,
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
        const StandardBehaviourInterface &,
        const BehaviourDescription &,
        const std::set<Hypothesis> &,
        const std::string &) const;
    /*!
     * \brief write the symbols associated with the driving variables
     * and the associated thermodynamic forces (number, names and types)
     * \param[in] out:  output file
     * \param[in] i:    behaviour interface
     * \param[in] mb:   behaviour description
     * \param[in] name: name of the behaviour as defined by the interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     */
    virtual void writeMainVariablesSymbols(std::ostream &,
                                           const StandardBehaviourInterface &,
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
    virtual void writeMaterialPropertiesSymbols(
        std::ostream &,
        const StandardBehaviourInterface &,
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
                                            const StandardBehaviourInterface &,
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
    virtual void writeExternalStateVariablesSymbols(
        std::ostream &,
        const StandardBehaviourInterface &,
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
    virtual void writeParametersSymbols(std::ostream &,
                                        const StandardBehaviourInterface &,
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
        const StandardBehaviourInterface &,
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
                                    const StandardBehaviourInterface &,
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
                                            const StandardBehaviourInterface &,
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
                                          const StandardBehaviourInterface &,
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
                                          const StandardBehaviourInterface &,
                                          const BehaviourDescription &,
                                          const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] i    : standard behaviour interface
     * \param[in] mb   : behaviour description
     * \param[in] name : behaviour name
     */
    virtual void writeElasticSymmetryTypeSymbols(
        std::ostream &,
        const StandardBehaviourInterface &,
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
                                        const StandardBehaviourInterface &,
                                        const BehaviourDescription &,
                                        const FileDescription &,
                                        const std::string &,
                                        const Hypothesis) const = 0;
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
                                    const StandardBehaviourInterface &,
                                    const std::string &,
                                    const Hypothesis &,
                                    const std::vector<std::string> &,
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
    /*!
     * \return if the interface treats the strain measure internally or if the
     * strain measure is handled by the calling code.
     */
    virtual bool handleStrainMeasure() const = 0;

    //! \brief destructor
    virtual ~SymbolsGenerator();

  };  // end of struct SymbolsGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_UMATXXSYMBOLSGENERATOR_HXX */
