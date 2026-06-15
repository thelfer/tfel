
/*!
 * \file  mfront/include/MFront/MaterialKnowledgeDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX
#define LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX

#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/MaterialKnowledgeAttribute.hxx"

namespace mfront {

  /*!
   * \brief base class for the description of material knowledge
   */
  struct MFRONT_VISIBILITY_EXPORT MaterialKnowledgeDescription {
    //! \brief standard option and attribute name
    static const char* const defaultOutOfBoundsPolicy;
    //! \brief standard option and attribute name
    static const char* const disableRuntimeChecks;
    //! \brief standard option name
    static const char* const runtimeModificationOfTheOutOfBoundsPolicy;
    //! \brief attribute name
    static const char* const parametersAsStaticVariables;
    //! \brief attribute name
    static const char* const initializeParametersFromFile;
    //! \brief attribute name
    static const char* const validator;
    //! \brief attribute name
    static const char* const buildIdentifier;
    /*!
     * \brief throw an exception saying that no attribute with the given name
     * exists
     */
    [[noreturn]] static void throwUndefinedAttribute(const std::string_view);
    //! \brief default constructor
    MaterialKnowledgeDescription();
    //! \brief move constructor
    MaterialKnowledgeDescription(MaterialKnowledgeDescription&&);
    //! \brief copy constructor
    MaterialKnowledgeDescription(const MaterialKnowledgeDescription&);
    //! \brief default assignement
    MaterialKnowledgeDescription& operator=(
        const MaterialKnowledgeDescription&);
    //! \brief move assignement
    MaterialKnowledgeDescription& operator=(MaterialKnowledgeDescription&&);
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const std::string&,
                      const MaterialKnowledgeAttribute&,
                      const bool);
    /*!
     * \brief update an existing new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const std::string_view,
                         const MaterialKnowledgeAttribute&);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool hasAttribute(const std::string_view) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    T& getAttribute(const std::string_view) requires(
        isMaterialKnowledgeAttributeType<T>());
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    const T& getAttribute(const std::string_view) const
        requires(isMaterialKnowledgeAttributeType<T>());
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     * \param[in] v: default value if the attribute is not defined
     */
    template <typename T>
    T getAttribute(const std::string_view, const T&) const
        requires(isMaterialKnowledgeAttributeType<T>());
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, MaterialKnowledgeAttribute, std::less<>>&
    getAttributes() const;
    /*!
     * \brief add an external `MFront` file and the list of interfaces used to
     * treat it.
     *
     * \param[in] f: full path to the external file
     * \param[in] interfaces: list of interfaces used to treat the file
     */
    void addExternalMFrontFile(const std::string_view,
                               const std::vector<std::string>&,
                               const tfel::utilities::DataMap&);
    //! \return the external `MFront` files
    const std::map<std::string,                           // file path
                   std::tuple<std::vector<std::string>,   // list of interfaces
                              tfel::utilities::DataMap>,  // DSL options
                   std::less<>>&
    getExternalMFrontFiles() const;
    /*!
     * \brief return if an unit system has been associated to the material
     * knowledge
     */
    bool hasUnitSystem() const;
    /*!
     * \brief associate an unit system to the material knowledge
     * \param[in] s: unit system
     */
    void setUnitSystem(const std::string_view);
    /*!
     * \brief associate an unit system to the material knowledge
     * \param[in] s: unit system
     */
    const std::string& getUnitSystem() const;
    //! \brief destructor
    ~MaterialKnowledgeDescription();

   protected:
    //! \brief unit system
    std::optional<std::string> unit_system;
    //! \brief behaviour attributes
    std::map<std::string, MaterialKnowledgeAttribute, std::less<>> attributes;
    /*!
     * \brief external mfront files
     * - key: mfront file name (full path)
     * - value: list of interfaces to be used and DSL options
     * This list of external mfront files will be used to generate the
     * associated sources.
     */
    std::map<std::string,
             std::tuple<std::vector<std::string>,   // list of interfaces
                        tfel::utilities::DataMap>,  // DSL options
             std::less<>>
        externalMFrontFiles;
  };  // end of struct MaterialKnowledgeDescription

  /*!
   * \brief set if runtime checks shall be disabled
   * \param[out] d: material knowledge description
   * \param[in] b: boolean
   */
  MFRONT_VISIBILITY_EXPORT void setDisableRuntimeChecks(
      MaterialKnowledgeDescription&, const bool);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::disableRuntimeChecks`
   * attribute if it is defined, `false` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT bool areRuntimeChecksDisabled(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief set the default out of bounds policy
   * \param[out] d: material knowledge description
   * \param[in] policy: out of bounds policy
   */
  MFRONT_VISIBILITY_EXPORT void setDefaultOutOfBoundsPolicy(
      MaterialKnowledgeDescription&, const std::string&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::defaultOutOfBoundsPolicy`
   * attribute if it is defined, `tfel::material::None` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT tfel::material::OutOfBoundsPolicy
  getDefaultOutOfBoundsPolicy(const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::defaultOutOfBoundsPolicy`
   * attribute if it is defined, `None` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT std::string getDefaultOutOfBoundsPolicyAsString(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::defaultOutOfBoundsPolicy`
   * attribute if it is defined, `NONE` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT std::string
  getDefaultOutOfBoundsPolicyAsUpperCaseString(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::runtimeModificationOfTheOutOfBoundsPolicy`
   * attribute if it is defined, `true` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT bool allowRuntimeModificationOfTheOutOfBoundsPolicy(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::parametersAsStaticVariables`
   * attribute if it is defined, `false` otherwise.
   * \return if the parameters are treated as static variables.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT bool areParametersTreatedAsStaticVariables(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::initializeParametersFromFile`
   * attribute if it is defined, `true` otherwise.
   * \return if the initialization of parameters from a file shall be allowed.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT bool allowsParametersInitializationFromFile(
      const MaterialKnowledgeDescription&);

}  // end of namespace mfront

#include "MFront/MaterialKnowledgeDescription.ixx"

#endif /* LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX */
