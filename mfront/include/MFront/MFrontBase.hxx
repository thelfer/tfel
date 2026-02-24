/*!
 * \file  MFrontBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBASE_HXX
#define LIB_MFRONT_MFRONTBASE_HXX

#include <set>
#include <string>
#include <vector>
#include <memory>

#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/TargetsDescription.hxx"

namespace mfront {

  // forward declaration
  struct AbstractDSL;

  /*!
   * \brief base class for the MFront and MFrontQuery classes
   */
  struct MFRONT_VISIBILITY_EXPORT MFrontBase {
    /*!
     * \param[in] f : file name
     * \param[in] dsl_options: options passed to the DSL
     * \return an abstract dsl that will handle the file
     */
    static std::shared_ptr<AbstractDSL> getDSL(
        const std::string&, const tfel::utilities::DataMap& = {});
    /*!
     * \return an abstract dsl that will handle the source described by the
     * given iterators
     * \param[in] pt: iterator to the first token of the source
     * \param[in] pte: iterator to the last token  of the source
     * \param[in] dsl_options: options passed to the DSL
     */
    static std::shared_ptr<AbstractDSL> getDSL(
        tfel::utilities::CxxTokenizer::const_iterator,
        const tfel::utilities::CxxTokenizer::const_iterator,
        const tfel::utilities::DataMap& = {});
    /*!
     * \brief add a new DSL option
     * \param[in] o: option. Must of a string of the form
     * `<option_name>:<option_value>`.
     */
    static void addDSLOption(const std::string&);
    /*!
     * \brief add a new material property DSL option
     * \param[in] o: option. Must of a string of the form
     * `<option_name>:<option_value>`.
     */
    static void addMaterialPropertyDSLOption(const std::string&);
    /*!
     * \brief add a new behaviour DSL option
     * \param[in] o: option. Must of a string of the form
     * `<option_name>:<option_value>`.
     */
    static void addBehaviourDSLOption(const std::string&);
    /*!
     * \brief add a new model DSL option
     * \param[in] o: option. Must of a string of the form
     * `<option_name>:<option_value>`.
     */
    static void addModelDSLOption(const std::string&);
    /*!
     * \brief parse a DSL options file
     * \param[in] f: option file in JSON-like format.
     */
    static void parseDSLOptionsFile(const std::string&);
    /*!
     * \brief parse a DSL options file
     * \param[in] f: option file in JSON-like format.
     */
    static void parseMaterialPropertyDSLOptionsFile(const std::string&);
    /*!
     * \brief parse a DSL options file
     * \param[in] f: option file in JSON-like format.
     */
    static void parseBehaviourDSLOptionsFile(const std::string&);
    /*!
     * \brief parse a DSL options file
     * \param[in] f: option file in JSON-like format.
     */
    static void parseModelDSLOptionsFile(const std::string&);
    //! \brief constructor
    MFrontBase();
    //! \brief add a new interaface
    void setInterface(const std::string&);
    //! desctructor
    virtual ~MFrontBase();

   protected:
    //! \return the current Argument
    virtual const tfel::utilities::Argument& getCurrentCommandLineArgument()
        const = 0;
    virtual bool treatUnknownArgumentBase();
    /*!
     * \brief method that must be called once all the arguments have been
     * parsed.
     *
     * \note This methods checks that the `material_identifier`,
     * `material_property_identifier`, `behaviour_identifier` and
     * `model_identifier` members are empty. If not, it means that those
     * identifiers have been defined after the last input files.
     *
     * \see the `addInputPaths` method for details
     */
    virtual void finalizeArgumentsParsing();
    /*!
     * \brief append paths generated from the given argument to the input files
     * \param[in] p: path generator
     *
     * \note for `madnex` files, the path are generated using the information
     * provided by the `material_identifier`, `material_property_identifier`,
     * `behaviour_identifier` and `model_identifier` members. Those members are
     * cleared after the treatment.
     */
    virtual void addInputPaths(const std::string&);
    //! \brief treat the `--verbose` command line option
    virtual void treatVerbose();
    //! \brief treat the `--unicode-output` command line option
    virtual void treatUnicodeOutput();
    //! \brief treat the `--search-path` command line option
    virtual void treatSearchPath();
    //! \brief treat the `--madnex-search-path` command line option
    virtual void treatMadnexSearchPath();
    /*!
     * \brief treat the `--intall-path` (or `--install-prefix`) command line
     * option
     */
    virtual void treatInstallPath();
    //! \brief treat the `--dsl-option` command line option
    virtual void treatDSLOption();
    //! \brief treat the `--material-property-dsl-option` command line option
    virtual void treatMaterialPropertyDSLOption();
    //! \brief treat the `--behaviour-dsl-option` command line option
    virtual void treatBehaviourDSLOption();
    //! \brief treat the `--model-dsl-option` command line option
    virtual void treatModelDSLOption();
    //! \brief treat the `--dsl-options-file` command line option
    virtual void treatDSLOptionsFile();
    /*!
     * \brief treat the `--material-property-dsl-options-file` command line
     * option
     */
    virtual void treatMaterialPropertyDSLOptionsFile();
    //! \brief treat the `--behaviour-dsl-options-file` command line option
    virtual void treatBehaviourDSLOptionsFile();
    //! \brief treat the `--model-dsl-options-file` command line option
    virtual void treatModelDSLOptionsFile();
    //! \brief treat the `--report-warnings` command line option
    virtual void treatReportWarnings();
    //! \brief treat the `--warning-error` command line option
    virtual void treatWarningError();
    //! \brief treat the `--debug` command line option
    virtual void treatDebug();
    //! \brief treat the `--pedantic` command line option
    virtual void treatPedantic();
    //! \brief treat the `--interface` command line option
    virtual void treatInterface();
    //! \brief treat the `--material-identifier` command line option
    virtual void treatMaterialIdentifier();
    //! \brief treat the `--material-property` command line option
    virtual void treatMaterialPropertyIdentifier();
    //! \brief treat the `--all-material-properties` command line option
    virtual void treatAllMaterialProperties();
    //! \brief treat the `--behaviour` command line option
    virtual void treatBehaviourIdentifier();
    //! \brief treat the `--all-behaviours` command line option
    virtual void treatAllBehaviours();
    //! \brief treat the `--model` command line option
    virtual void treatModelIdentifier();
    //! \brief treat the `--all-models` command line option
    virtual void treatAllModels();
    /*!
     * \brief external commands specified on the command line through
     * an `--@XXX` option. Those external commands are applied to
     * all input files.
     */
    std::vector<std::string> ecmds;
    /*!
     * \brief substitutions specified on the command line through an
     * `--@XXX@=YYY` option: every occurrence of `XXX` in the input
     * files will be replaced by `YYY`
     */
    std::map<std::string, std::string> substitutions;
    //! \brief material identifier for the next input file
    std::string material_identifier;
    //! \brief material property identifier for the next input file
    std::string material_property_identifier;
    //! \brief behaviour identifier for the next input file
    std::string behaviour_identifier;
    //! \brief model identifier for the next input file
    std::string model_identifier;
    //! \brief list of all input files
    std::set<std::string> inputs;
    /*!
     * \brief list of interfaces declared on the command line or explicitely
     * added through the set interface method.
     */
    std::set<std::string> interfaces;
  };  // end of struct MFrontBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBASE_HXX */
