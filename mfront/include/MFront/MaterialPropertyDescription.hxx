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

#include "MFront/LawFunction.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/VariableBoundsDescription.hxx"

namespace mfront {

  //! structure describing a material property
  struct MFRONT_VISIBILITY_EXPORT MaterialPropertyDescription {
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
    //! destructor
    ~MaterialPropertyDescription();
    //! body of the material property
    LawFunction f;
    //! list of inputs
    VariableDescriptionContainer inputs;
    //! list of parameters
    VariableDescriptionContainer parameters;
    //! output
    VariableDescription output;
    //! law name
    std::string law;
    //! material name
    std::string material;
    //! library name
    std::string library;
    //! class name
    std::string className;
    //! included header files
    std::string includes;
    //! specific sources
    std::string sources;
    //! private code
    std::string privateCode;
    //! class member
    std::string members;
    //! list of material laws used
    std::vector<std::string> materialLaws;
    //! static variables
    StaticVariableDescriptionContainer staticVars;

   protected:
    //! list of reserved names
    std::set<std::string> reservedNames;
    //! list of variables names
    std::set<std::string> memberNames;
    //! list of variables names
    std::set<std::string> staticMemberNames;
    /*!
     * \return the variable description with the given name
     * \param[in] n: variable name
     */
    VariableDescription& getVariableDescription(const std::string&);
  };  // end of MaterialProopertyDescription

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_HXX */
