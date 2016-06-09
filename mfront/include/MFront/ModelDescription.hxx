/*! 
 * \file   mfront/include/MFront/ModelDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTMODELDATA_H_
#define LIB_MFRONT_MFRONTMODELDATA_H_ 

#include<set>
#include<map>
#include<string>

#include"MFront/MFrontConfig.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront
{

  struct MFRONT_VISIBILITY_EXPORT ModelDescription
  {

    struct MFRONT_VISIBILITY_EXPORT Function
    {
      Function();
      Function(const Function&);
      Function(Function&&);
      Function& operator=(const Function&);
      Function& operator=(Function&&);
      ~Function();
      std::set<std::string> usedVariables;
      std::set<std::string> modifiedVariables;
      std::set<std::string> constantMaterialProperties;
      std::set<std::string> parameters;
      std::map<std::string,unsigned short> depths;
      std::string name;
      std::string body;
      unsigned int line = 0u;
      bool useTimeIncrement = false;
    }; // end of struct MFrontData::Function
    //! defaut constructor
    ModelDescription();
    //! copy constructor
    ModelDescription(const ModelDescription&);
    //! move constructor
    ModelDescription(ModelDescription&&);
    //! assignement operator
    ModelDescription& operator=(const ModelDescription&);
    //! move assignement operator
    ModelDescription& operator=(ModelDescription&&);
    /*!
     * \return true if a glossary name has been defined for the given
     * variable
     * \param[in] n: variable name
     */
    bool hasGlossaryName(const std::string&) const;
    /*!
     * \return the glossary name associated to the given variable
     * \param[in] n: variable name
     */
    std::string getGlossaryName(const std::string&) const;
    /*!
     * \return the external name of a variable
     * \param[in] n: variable name
     */
    const std::string&
    getExternalName(const std::string&) const;
    /*!
     * \brief associate a glossary name to a variable 
     * \param[in] v: variable name
     * \param[in] g: glossary name
     */
    virtual void setGlossaryName(const std::string&,
				 const std::string&);
    /*!
     * \brief associate a glossary name to a variable 
     * \param[in] v: variable name
     * \param[in] g: glossary name
     */
    virtual void setEntryName(const std::string&,
			      const std::string&);
    //! desctructor
    virtual ~ModelDescription();
    //! list of functions declared
    std::vector<Function> functions;
    //! list of all outputs
    VariableDescriptionContainer outputs;
    //! list of all inputs
    VariableDescriptionContainer inputs;
    //! list of all parameters
    VariableDescriptionContainer parameters;
    //! list of all constant material properties
    VariableDescriptionContainer constantMaterialProperties;
    //! static variables
    StaticVariableDescriptionContainer staticVars;
    //! material name
    std::string material;
    //! library name
    std::string library;
    //! class name
    std::string className;
    //! additionnal header files
    std::string includes;
    //!specific sources
    std::string sources;
    //! private code
    std::string privateCode;
    //! class member
    std::string members;
    std::set<std::string> domains;
    std::set<std::string> fieldNames;
    std::set<std::string> functionNames;
    std::map<std::string,double>      initialValues;
    std::map<std::string,std::string> defaultValues;
    std::vector<VariableBoundsDescription> bounds;
    std::vector<VariableBoundsDescription> physicalBounds;
    std::map<std::string,unsigned short> depths;
    //! list of material laws used
    std::vector<std::string> materialLaws;
    //! list of variables names
    std::set<std::string> memberNames;
    //! list of variables names
    std::set<std::string> staticMemberNames;
  protected:
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
    virtual void reserveName(const std::string&);
    /*!
     * \brief register a static member name
     * \param[in] n : name
     */
    virtual void registerMemberName(const std::string&);
    /*!
     * \brief register a static member name
     * \param[in] n : name
     */
    virtual void registerStaticMemberName(const std::string&);
    /*!
     * check that a variable exists
     * \param[in] v: variable name
     */
    virtual void checkVariableExistence(const std::string&) const;
    //! \return the list of reserved names
    std::set<std::string>&
    getReservedNames(void);
    //! \return the list of reserved names
    const std::set<std::string>&
    getReservedNames(void) const;
  private:
    //! set glossary names
    std::map<std::string,std::string> glossaryNames;
    //! entry names
    std::map<std::string,std::string> entryNames;
    //! list of reserved names
    std::set<std::string> reservedNames;
  }; // end of struct ModelDescription

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTMODELDATA_H_ */
