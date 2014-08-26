/*! 
 * \file  MFrontGenericData.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 jun 2010
 */

#ifndef   _LIB_MFRONT_MFRONTGENERICDATA_H_
#define   _LIB_MFRONT_MFRONTGENERICDATA_H_ 

#include<set>
#include<string>

#include"MFront/StaticVariableDescription.hxx"

namespace mfront
{

  struct MFrontGenericData
  {
    std::string className;
    std::string material;
    std::string library;
    std::string description;
    std::string fileName;    
    std::string authorName;
    std::string date;
    std::string includes;
    StaticVariableDescriptionContainer staticVars;
    std::set<std::string> varNames;
    std::set<std::string> staticVarNames;
    std::set<std::string> reservedNames;
    std::vector<std::string> materialLaws;
  }; // end of struct MFrontGenericData

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTGENERICDATA_H */

