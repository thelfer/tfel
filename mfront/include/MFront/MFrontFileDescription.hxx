/*! 
 * \file  MFrontFileDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 jun 2010
 */

#ifndef   _LIB_MFRONT_MFRONTFILEDESCRIPTION_H_
#define   _LIB_MFRONT_MFRONTFILEDESCRIPTION_H_ 

#include<set>
#include<string>

#include"MFront/StaticVariableDescription.hxx"

namespace mfront
{

  /*!
   * this class gathers some information about the MFront file being
   * treatd
   */
  struct MFrontFileDescription
  {
    std::string className;
    std::string library;
    std::string description;
    std::string fileName;    
    std::string authorName;
    std::string date;
    std::string includes;
    std::set<std::string> varNames;
    std::set<std::string> staticVarNames;
    std::set<std::string> reservedNames;
    std::vector<std::string> materialLaws;
    std::string material; //< deprecated (moved in MFrontModelData
    StaticVariableDescriptionContainer staticVars; //< deprecated (moved in MFrontModelData
  }; // end of struct MFrontFileDescription

  //! typedef provided
  typedef MFrontFileDescription MFrontGenericData;

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTFILEDESCRIPTION_H */

