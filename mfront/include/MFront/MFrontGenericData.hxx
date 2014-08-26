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

namespace mfront
{

  /*!
   * this class gathers some information about the MFront file being
   * treatd
   */
  struct MFrontGenericData
  {
    // const std::string&
    // getLibrary(void) const;
  protected:
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
  }; // end of struct MFrontGenericData

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTGENERICDATA_H */

