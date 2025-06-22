/*!
 * \file   mfront/include/MFront/AbstractDSL.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABSTRACTDSL_HXX
#define LIB_MFRONT_ABSTRACTDSL_HXX

#include <map>
#include <set>
#include <vector>
#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct FileDescription;
  // forward declaration
  struct TargetsDescription;
  // forward declaration
  struct MaterialKnowledgeDescription;
  // forward declaration
  struct MaterialPropertyDescription;

  //! \brief interface class for all domain specific languages.
  struct MFRONT_VISIBILITY_EXPORT AbstractDSL {
    //! \brief a simple alias
    using DSLOptions = tfel::utilities::DataMap;
    //! list of dsl targets
    enum DSLTarget { MATERIALPROPERTYDSL, BEHAVIOURDSL, MODELDSL };
    /*!
     * \brief structure used to describe a DSL option
     */
    struct DSLOptionDescription {
      std::string name;
      std::string description;
    };  // end of struct DSLOptionDescription
    //! \return the list of options accepted by the DSL
    virtual std::vector<DSLOptionDescription> getDSLOptions() const = 0;
    //! \return the target of the dsl
    virtual DSLTarget getTargetType() const = 0;
    //! \return the description of the considered material knowledge
    virtual const MaterialKnowledgeDescription&
    getMaterialKnowledgeDescription() const = 0;
    //! \return the file description associated with the treated file
    virtual const FileDescription& getFileDescription() const = 0;
    /*!
     * \return the identifier (material property name, behaviour name, model
     * name) of the treated file
     */
    virtual std::string getMaterialKnowledgeIdentifier() const = 0;
    //! \return the material described by the treated file
    virtual std::string getMaterialName() const = 0;
    /*!
     * \return the variable name associated with the given external name
     * \param[in] n: external name of the quantity overridable by a parameter
     *
     * Depending on the DSL's, a parameter may override a parameter, but also a
     * material property for example.
     */
    virtual std::string getOverridableVariableNameByExternalName(
        const std::string&) const = 0;
    //! \return the list of overriden parameters
    virtual std::map<std::string, double> getOverridenParameters() const = 0;
    /*!
     * \brief analyse a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commands (those commands are given through the
     * `--@??` option of the command line
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    virtual void analyseFile(const std::string&,
                             const std::vector<std::string>&,
                             const std::map<std::string, std::string>&) = 0;
    /*!
     * \brief analyse the specified string.
     * \param[in] s : analysed a string
     */
    virtual void analyseString(const std::string&) = 0;
    /*!
     * \brief import a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commands
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    virtual void importFile(const std::string&,
                            const std::vector<std::string>&,
                            const std::map<std::string, std::string>&) = 0;
    /*!
     * \brief method called at the end of the input file processing.
     * \note This method shall be called *after* the `analyseFile` or
     * `analyseString` methods.
     */
    virtual void endsInputFileProcessing() = 0;
    /*!
     * \return the target description
     * \note This method shall be called *after* the `analyseFile` or
     * `analyseString` methods.
     */
    virtual const TargetsDescription& getTargetsDescription() const = 0;
    /*!
     * \brief treat the specified file.
     * \note This method shall be called *after* the analyseFile method
     */
    virtual void generateOutputFiles() = 0;
    /*!
     * \brief set list of interfaces
     * \param[in] inames : list of interfaces
     */
    virtual void setInterfaces(const std::set<std::string>&) = 0;
    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void getKeywordsList(std::vector<std::string>&) const = 0;
    //! \brief destructor
    virtual ~AbstractDSL();
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTDSL_HXX */
