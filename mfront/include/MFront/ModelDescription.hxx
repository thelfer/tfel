/*!
 * \file   mfront/include/MFront/ModelDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  12 jun 2010
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MODELDESCRIPTION_HXX
#define LIB_MFRONT_MODELDESCRIPTION_HXX

#include <set>
#include <map>
#include <string>
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/MaterialKnowledgeDescription.hxx"

namespace mfront {

  //! \brief class describing a model
  struct MFRONT_VISIBILITY_EXPORT ModelDescription
      : public MaterialKnowledgeDescription {
    //! \brief a model may contain several function
    struct MFRONT_VISIBILITY_EXPORT Function {
      //! \brief default constructor
      Function();
      //! \brief copy constructor
      Function(const Function&);
      //! \brief move constructor
      Function(Function&&);
      //! \brief assignement
      Function& operator=(const Function&);
      //! \brief move assignement
      Function& operator=(Function&&);
      //! \brief destructor
      ~Function();
      //! \brief list of variables used by the function
      std::set<std::string> usedVariables;
      //! \brief list of variables modified by the function
      std::set<std::string> modifiedVariables;
      //! \brief list of constant material properties used by the function
      std::set<std::string> constantMaterialProperties;
      //! \brief list of parameters used by the function
      std::set<std::string> parameters;
      //! \brief depths of each variables
      std::map<std::string, unsigned short> depths;
      //! \brief name of the function
      std::string name;
      //! \brief body of the function
      std::string body;
      //! \brief line starting the definition of the function in the initial
      //! \brief MFront file
      unsigned int line = 0u;
      //! \brief if true, the body of the function uses the time increment dt
      bool useTimeIncrement = false;
    };  // end of struct MFrontData::Function
    /*!
     * \brief decompose a variable name to get the basis and the depth
     * of the variable
     */
    std::pair<std::string, unsigned short> decomposeVariableName(
        const std::string&) const;
    //! \brief defaut constructor
    ModelDescription();
    //! \brief copy constructor
    ModelDescription(const ModelDescription&);
    //! \brief move constructor
    ModelDescription(ModelDescription&&);
    //! \brief assignement operator
    ModelDescription& operator=(const ModelDescription&);
    //! \brief move assignement operator
    ModelDescription& operator=(ModelDescription&&);
    /*!
     * \return the variable description with the given name
     * \param[in] n: variable name
     */
    const VariableDescription& getVariableDescription(const std::string&) const;
    /*!
     * \brief associate a glossary name to a variable
     * \param[in] v: variable name
     * \param[in] g: glossary name
     */
    void setGlossaryName(const std::string&, const std::string&);
    /*!
     * \brief associate a glossary name to a variable
     * \param[in] v: variable name
     * \param[in] g: glossary name
     */
    void setEntryName(const std::string&, const std::string&);
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
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     * \note this method is called internally by the registerVariable
     * and registerStaticVariable methods.
     */
    void reserveName(const std::string&);
    /*!
     * \brief register a static member name
     * \param[in] n : name
     */
    void registerMemberName(const std::string&);
    /*!
     * \brief register a static member name
     * \param[in] n : name
     */
    void registerStaticMemberName(const std::string&);
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    bool isNameReserved(const std::string&) const;
    //! \brief check and complete the physical bounds of variables
    void checkAndCompletePhysicalBoundsDeclaration();
    //! \brief destructor
    virtual ~ModelDescription();
    //! \brief use quantities if available
    std::optional<bool> use_qt;
    //! \return the list of reserved names
    std::set<std::string>& getReservedNames();
    //! \brief list of functions declared
    std::vector<Function> functions;
    //! \brief list of all outputs
    VariableDescriptionContainer outputs;
    //! \brief list of all inputs
    VariableDescriptionContainer inputs;
    //! \brief list of all parameters
    VariableDescriptionContainer parameters;
    //! \brief list of all constant material properties
    VariableDescriptionContainer constantMaterialProperties;
    //! \brief static variables
    StaticVariableDescriptionContainer staticVars;
    //! \brief material name
    std::string material;
    //! \brief library name
    std::string library;
    //! \brief class name
    std::string className;
    //! \brief name of the model
    std::string modelName;
    //! \brief additionnal header files
    std::string includes;
    //! \brief specific sources
    std::string sources;
    //! \brief private code
    std::string privateCode;
    //! \brief class member
    std::string members;
    std::set<std::string> domains;
    //! \brief list of material laws used
    std::vector<std::string> materialLaws;
    //! \brief list of variables names
    std::set<std::string> memberNames;
    //! \brief list of variables names
    std::set<std::string> staticMemberNames;
    //! \brief overriding parameters
    std::map<std::string, double> overriding_parameters;

   protected:
    /*!
     * \brief check that a variable exists
     * \param[in] v: variable name
     */
    void checkVariableExistence(const std::string&) const;
    //! \return the list of reserved names
    const std::set<std::string>& getReservedNames() const;

   private:
    /*!
     * \return the variable description with the given name
     * \param[in] n: variable name
     */
    VariableDescription& getVariableDescription(const std::string&);
    //! \brief set glossary names
    std::map<std::string, std::string> glossaryNames;
    //! \brief entry names
    std::map<std::string, std::string> entryNames;
    //! \brief list of reserved names
    std::set<std::string> reservedNames;
  };  // end of struct ModelDescription

  /*!
   * \brief convert a model description to a behaviour description
   * \return the behaviour description
   * \param[in] md: model description
   */
  BehaviourDescription convertToBehaviourDescription(const ModelDescription&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_MODELDESCRIPTION_HXX */
