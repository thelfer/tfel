/*! 
 * \file   MFrontModelData.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 jun 2010
 */

#ifndef _LIB_MFRONT_MFRONTMODELDATA_H_
#define _LIB_MFRONT_MFRONTMODELDATA_H_ 

#include<set>
#include<map>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT MFrontModelData
  {

    struct Function
    {
      std::set<std::string> usedVariables;
      std::set<std::string> modifiedVariables;
      std::set<std::string> constantMaterialProperties;
      std::set<std::string> localParameters;
      std::set<std::string> globalParameters;
      std::map<std::string,unsigned short> depths;
      std::string name;
      std::string body;
      unsigned short line;
      bool useTimeIncrement;
    }; // end of struct MFrontPleiadesModelParser::Function

    typedef std::vector<Function> FunctionContainer;
    FunctionContainer functions;

    VariableDescriptionContainer outputs;
    VariableDescriptionContainer inputs;
    VariableDescriptionContainer globalParameters;
    VariableDescriptionContainer localParameters;
    VariableDescriptionContainer constantMaterialProperties;

    std::set<std::string> domains;
    std::set<std::string> fieldNames;
    std::set<std::string> functionNames;
    std::map<std::string,std::string> glossaryNames;
    std::map<std::string,std::string> entryNames;
    std::map<std::string,double>      initialValues;
    std::map<std::string,std::string> defaultValues;
    std::vector<VariableBoundsDescription> boundsDescriptions;
    std::vector<VariableBoundsDescription> physicalBoundsDescriptions;
    std::map<std::string,unsigned short> depths;

  }; // end of struct MFrontModelData

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTMODELDATA_H */
