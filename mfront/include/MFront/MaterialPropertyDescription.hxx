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
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront
{

  /*!
   * structure describing a material property
   */
  struct TFEL_VISIBILITY_EXPORT MaterialPropertyDescription
  {
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
     * static variables
     */
    StaticVariableDescriptionContainer staticVars;
  }; // end of MaterialProopertyDescription

} // end of namespace mfront

#endif /* _LIB_MFRONT_MATERIALPROPERTYDESCRIPTION_H */

