/*! 
 * \file   mfront/include/MFront/ModelData.hxx
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

  struct MFRONT_VISIBILITY_EXPORT ModelData
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
      std::set<std::string> localParameters;
      std::set<std::string> globalParameters;
      std::map<std::string,unsigned short> depths;
      std::string name;
      std::string body;
      unsigned int line = 0u;
      bool useTimeIncrement = false;
    }; // end of struct MFrontData::Function

    using FunctionContainer=std::vector<Function> ;

    ModelData();
    ModelData(const ModelData&);
    ModelData(ModelData&&);
    ModelData& operator=(const ModelData&);
    ModelData& operator=(ModelData&&);
    ~ModelData();

    FunctionContainer functions;
    VariableDescriptionContainer outputs;
    VariableDescriptionContainer inputs;
    VariableDescriptionContainer globalParameters;
    VariableDescriptionContainer localParameters;
    VariableDescriptionContainer constantMaterialProperties;
    //!static variables
    StaticVariableDescriptionContainer staticVars;
    //! material name
    std::string material;
    std::string library;
    std::string className;
    //! additionnal header files
    std::string includes;
    //!specific sources
    std::string sources;
    //! private code
    std::string privateCode;
    /*!
     * class member
     */
    std::string members;
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
    //! list of material laws used
    std::vector<std::string> materialLaws;
  }; // end of struct ModelData

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTMODELDATA_H_ */
