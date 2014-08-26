/*! 
 * \file  MaterialPropertyDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 févr. 2014
 */

#ifndef _LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_H_
#define _LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_H_ 

#include<map>
#include<vector>
#include<string>

#include"MFront/LawFunction.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront
{

  /*!
   * structure describing a material property
   */
  struct TFEL_VISIBILITY_EXPORT MaterialPropertyDescription
  {
    /*!
     * \return the glossary names of each input variables
     */
    std::vector<std::string>
    getInputVariableGlossaryNames(void);
    /*!
     * body of the material property
     */
    LawFunction f;
    /*!
     * list of inputs
     */
    VariableDescriptionContainer inputs;
    /*!
     * output
     */
    std::string output;
    /*!
     * law name
     */
    std::string law;
    /*!
     * material name
     */
    std::string material;
    /*!
     * library name
     */
    std::string library;
    /*!
     * class name
     */
    std::string className;
    /*!
     * included header files
     */
    std::string includes;
    /*!
     * specific sources
     */
    std::string sources;
    /*!
     * private code
     */
    std::string privateCode;
    /*!
     * class member
     */
    std::string members;
    /*!
     * bounds description
     */
    std::vector<VariableBoundsDescription> boundsDescriptions;
    /*!
     * physical bounds description
     */
    std::vector<VariableBoundsDescription> physicalBoundsDescriptions;
    /*!
     * glossary names
     */
    std::map<std::string,std::string> glossaryNames;
    /*!
     * entry names
     */
    std::map<std::string,std::string> entryNames;
    /*!
     * parameters
     */
    std::vector<std::string> parameters;
    /*!
     * parameters default value
     */
    std::map<std::string,double> parametersValues;
    /*!
     * list of material laws used
     */
    std::vector<std::string> materialLaws;
    /*!
     * static variables
     */
    StaticVariableDescriptionContainer staticVars;
  }; // end of MaterialProopertyDescription

} // end of namespace mfront

#endif /* _LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_H */

