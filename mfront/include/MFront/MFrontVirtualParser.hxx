/*!
 * \file   MFrontVirtualParser.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTVIRTUALPARSER_HXX_
#define _LIB_MFRONTVIRTUALPARSER_HXX_ 

#include<string>
#include<map>
#include<set>
#include<vector>

namespace mfront{

  struct MFrontVirtualParser
  {
    virtual void setVerboseMode() = 0;

    virtual void setDebugMode() = 0;

    virtual void setWarningMode() = 0;

    virtual void treatFile(const std::string&) = 0;

    virtual void writeOutputFiles() = 0;

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void) = 0;

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void) = 0;

    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void) = 0;

    virtual std::vector<std::string>
    getGeneratedIncludes(void) = 0;

    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void) = 0;

    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(void) = 0;

    virtual void
    setInterfaces(const std::set<std::string>&) = 0;

    virtual ~MFrontVirtualParser();
  };

} // end of namespace mfront

#endif /* _LIB_MFRONTVIRTUALPARSER_HXX */

