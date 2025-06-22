/*!
 * \file  mfront/include/MFront/MaterialPropertyDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 26 févr. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_HXX
#define LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_HXX

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iosfwd>
#include <optional>
#include <string_view>

#include "MFront/LawFunction.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/MaterialKnowledgeDescription.hxx"

namespace mfront {

  // forward declaration
  struct FileDescription;

  //! structure describing a material property
  struct MFRONT_VISIBILITY_EXPORT MaterialPropertyDescription
      : public MaterialKnowledgeDescription {
    //! default constructor
    MaterialPropertyDescription();
    //! copy constructor
    MaterialPropertyDescription(const MaterialPropertyDescription&);
    //! move constructor
    MaterialPropertyDescription(MaterialPropertyDescription&&);
    //! standard assignement
    MaterialPropertyDescription& operator=(const MaterialPropertyDescription&);
    //! move assignement
    MaterialPropertyDescription& operator=(MaterialPropertyDescription&&);
    /*!
     * \return the variable description with the given name
     * \param[in] n: variable name
     */
    const VariableDescription& getVariableDescription(const std::string&) const;
    //! \return the external names of each input variables.
    std::vector<std::string> getInputVariablesExternalNames();
    /*!
     * \return true if the given variable is the name of a input
     * \param[in] n: name
     */
    bool isInputName(const std::string&) const;
    /*!
     * \return true if the given variable is the name of a parameter
     * \param[in] n: name
     */
    bool isParameterName(const std::string&) const;
    /*!
     * \brief associate a glossary name to a variable
     * \param[in] n : variable name
     * \param[in] g : glossary name
     */
    void setGlossaryName(const std::string&, const std::string&);
    /*!
     * \return true if the given name is used as a glossary name
     * \param[in] g: glossary name
     */
    bool isGlossaryNameUsed(const std::string&) const;
    /*!
     * \brief associate an entry name to a variable
     * \param[in] n: variable name
     * \param[in] e: entry name
     */
    void setEntryName(const std::string&, const std::string&);
    /*!
     * \return true if the given name is used as a entry name
     * \param[in] e: entry name
     */
    bool isEntryNameUsed(const std::string&) const;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    void addMaterialLaw(const std::string&);
    /*!
     * \brief append the given code to the includes
     */
    void appendToIncludes(const std::string&);
    /*!
     * \brief append the given code to the members
     */
    void appendToMembers(const std::string&);
    /*!
     * \brief append the given code to the private code
     */
    void appendToPrivateCode(const std::string&);
    /*!
     * \brief append the given code to the sources
     */
    void appendToSources(const std::string&);
    /*!
     * \brief register a name.
     * \param[in] n : name
     */
    void reserveName(const std::string&);
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    bool isNameReserved(const std::string&) const;
    //! \brief check and complete the physical bounds of variables
    void checkAndCompletePhysicalBoundsDeclaration();
    /*!
     * \return a variable description associated with the given
     * external name.
     * \param[in] n: external name.
     */
    const VariableDescription& getVariableDescriptionByExternalName(
        const std::string&) const;
    //! \brief destructor
    ~MaterialPropertyDescription();
    //! \brief use quantities if available
    std::optional<bool> use_qt;
    //! \brief body of the material property
    LawFunction f;
    //! \brief list of inputs
    VariableDescriptionContainer inputs;
    //! \brief list of parameters
    VariableDescriptionContainer parameters;
    //! \brief output
    VariableDescription output;
    //! \brief law name
    std::string law;
    //! \brief material name
    std::string material;
    //! \brief library name
    std::string library;
    //! \brief class name
    std::string className;
    //! \brief included header files
    std::string includes;
    //! \brief specific sources
    std::string sources;
    //! \brief private code
    std::string privateCode;
    //! \brief class member
    std::string members;
    //! \brief list of material laws used
    std::vector<std::string> materialLaws;
    //! \brief static variables
    StaticVariableDescriptionContainer staticVars;

   protected:
    //! \brief list of reserved names
    std::set<std::string> reservedNames;
    //! \brief list of variables names
    std::set<std::string> memberNames;
    //! \brief list of variables names
    std::set<std::string> staticMemberNames;
    /*!
     * \return the variable description with the given name
     * \param[in] n: variable name
     */
    VariableDescription& getVariableDescription(const std::string&);
  };  // end of MaterialPropertyDescription

  /*!
   * \return if the material property shall use quantities
   * \param[in] mpd: material property description
   */
  MFRONT_VISIBILITY_EXPORT bool useQuantities(
      const MaterialPropertyDescription&);

  /*!
   * \brief an helper function which writes useful declarations at the beginning
   * of the function body.
   *
   * This function:
   *
   * - imports the namespace std (`use namespace std`)
   * - declares TFEL scalar type aliases (see the `writeScalarStandardTypedefs`
   *   function).
   * - imports material laws used by the material property (see the
   *   `writeStaticVariables` function).
   * - declares the static variables declared by the material property (see the
   *   `writeMaterialLaws` function).
   *
   * \param[in, out] os: output stream
   * \param[in] mpd: material property description
   * \param[in] fd: file description
   * \param[in] numeric_type: numeric type used by the interface
   * \param[in] bool: allow quantities
   */
  MFRONT_VISIBILITY_EXPORT void writeBeginningOfMaterialPropertyBody(
      std::ostream&,
      const MaterialPropertyDescription&,
      const FileDescription&,
      const std::string_view,
      const bool);
  /*!
   * \brief an helper function which writes the declaration of some `TFEL`
   * standard scalar alias. The `tfel::config::ScalarTypes` class for details.
   * \param[in, out] os: output stream
   * \param[in] mpd: material property description
   * \param[in] numeric_type: numeric type used by the interface
   * \param[in] bool: allow quantities
   */
  MFRONT_VISIBILITY_EXPORT void writeScalarStandardTypedefs(
      std::ostream&,
      const MaterialPropertyDescription&,
      const std::string_view,
      const bool);
  /*!
   * \return the name of a text file allowing the change the name of the
   * parameters of the given material property
   * \param[in] mpd: material property description
   */
  MFRONT_VISIBILITY_EXPORT std::string getParametersFileName(
      const MaterialPropertyDescription&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_HXX */
