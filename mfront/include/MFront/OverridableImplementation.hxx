/*!
 * \file   OverridableImplementation.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_OVERRIDABLEIMPLEMENTATION_HXX
#define LIB_MFRONT_OVERRIDABLEIMPLEMENTATION_HXX

#include "TFEL/Raise.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  /*!
   * \brief an helper structure that contains an enumeration list the string
   * valuated information in a `MFront` file that can be overriden.
   */
  struct OverridableStringInformationTags {
    /*
     * \brief list of string valuated information in a `MFront` file that can be
     * overriden.
     */
    enum Tags {
      MATERIAL_KNOWLEDGE_IDENTIFIER,
      MATERIAL_NAME,
      AUTHOR_NAME,
      DATE,
      DESCRIPTION
    };
  };  // end of OverridableStringInformation

  //! \brief an helper function
  [[noreturn]] MFRONT_VISIBILITY_EXPORT void throwOverridenValueAlreadySet();

  /*!
   * \brief overridable string valuated information
   */
  template <OverridableStringInformationTags::Tags Tag>
  struct OverridableStringInformation {
    /*!
     * \brief set the overriden value
     * \param[in] v: value
     */
    void setOverridenValue(const std::string&);
    //! \return the overriden value
    const std::string& getOverridenValue() const;

   private:
    std::string overriden_value;
  };

  /*!
   * \brief a data structure representing the implementation of a material
   * knowledge based on `MFront`.
   *
   * This datastructure allows to take an `MFront` file, called the source file
   * and override:
   *
   * - some metadata, such as the author name, the date, the description
   * - the parameters
   * - the bounds of the variables
   */
  struct MFRONT_VISIBILITY_EXPORT OverridableImplementation
      : OverridableStringInformationTags,
        protected OverridableStringInformation<
            OverridableStringInformationTags::MATERIAL_KNOWLEDGE_IDENTIFIER>,
        protected OverridableStringInformation<
            OverridableStringInformationTags::MATERIAL_NAME>,
        protected OverridableStringInformation<
            OverridableStringInformationTags::AUTHOR_NAME>,
        protected OverridableStringInformation<
            OverridableStringInformationTags::DATE>,
        protected OverridableStringInformation<
            OverridableStringInformationTags::DESCRIPTION> {
    /*!
     * \brief default construtor
     * \param[in] f: file
     */
    OverridableImplementation(const std::string&);
    /*!
     * \brief override the information associated with the given tag
     * tparam Tag: tag to the information that shall be overriden
     * \param[in] v: value
     */
    template <Tags Tag>
    void setOverridenValue(const std::string&);
    /*!
     * \return the string valuated  override associated the with given tag
     */
    template <Tags Tag>
    const std::string& getOverridenValue() const;
    /*!
     * \brief override a variable by a parameter
     * \param[in] n: external variable name
     * \param[in] v: parameter value
     */
    void overrideByAParameter(const std::string&, const double);
    //! \return the parameters'overrides
    const std::map<std::string, double>& getOverridingParameters() const;
    //! \return the target of the dsl
    AbstractDSL::DSLTarget getTargetType() const;
    /*!
     * \return the identifier (material property name, behaviour name, model
     * name) of the treated file
     */
    std::string getSourceMaterialKnowledgeIdentifier() const;
    //! \return the material described by the treated file
    std::string getSourceMaterialName() const;
    //! \return the file description associated with source file
    const FileDescription& getSourceFileDescription() const;
    //! \return the path to the source file
    const std::string& getSourceFilePath() const;
    //! \brief destructor
    ~OverridableImplementation();

   private:
    //! \brief domain specific language resulting from the analysis
    std::shared_ptr<const AbstractDSL> dsl;
    //! \brief path to the source file
    const std::string source;
    //! \brief overriden parameters
    std::map<std::string, double> parameters;
  };  // end of OverridableImplementation

  /*!
   * \brief write the implementation to the file
   * \param[in] i: implementation
   * \param[in] f: file name
   *
   * The extension of the file is used to determine the driver used.
   * Currently, the only driver available is associated with `madnex` files.
   */
  MFRONT_VISIBILITY_EXPORT void write(const OverridableImplementation&,
                                      const std::string&);

}  // end of namespace mfront

#include "MFront/OverridableImplementation.ixx"

#endif /* LIB_MFRONT_OVERRIDABLEIMPLEMENTATION_HXX */
