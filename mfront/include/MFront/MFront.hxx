/*!
 * \file   MFront.hxx
 * \brief  This file declares the MFront class
 * \author Helfer Thomas
 * \date   22 Nov 2006
 */

#ifndef _LIB_MFRONT_H_
#define _LIB_MFRONT_H_ 

#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<map>

#include"TFEL/Utilities/ArgumentParserBase.hxx"

#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  struct MFront
    : public tfel::utilities::ArgumentParserBase<MFront>
  {
    MFront(const int, const char *const *const);

    void treatUnknownArgument(void);

    void exe(void);

    ~MFront();

    static const std::string& getCallingName(void);

  private :

    static std::string callingName;

    static std::vector<std::string>
    tokenize(const std::string&,
	     const char);

    std::string
    getVersionDescription(void) const;

    std::string
    getUsageDescription(void) const;

    void treatVerbose(void);

    void treatWarning(void);

    void treatDebug(void);

    void treatInterface(void);

    void treatNoDeps(void);

    void treatNoMelt(void);

    void treatOMake(void);

    void treatOBuild(void);

    void treatMake(void);

    void treatBuild(void);

    void treatClean(void);

    void treatTarget(void);

    void treatOTarget(void);

    void treatFile(void);

    void treatListParsers(void);

    void registerArgumentCallBacks(void);

    void
    analyseSourceDirectory(void);

    void
    analyseSources(const std::string&);

    void
    analyseDependencies(const std::string&);

    void
    analyseMakefileSpecificTargets(void);

    void
    analyseGlobalIncludes(void);

    void
    writeSourcesLists(void);

    void
    writeDependenciesLists(void);

    void
    writeSpecificTargets(void);

    void
    writeGlobalIncludes(void);

    std::pair<bool,std::pair<std::string,std::string> >
    getLibraryDependencies(const std::string&);

    std::string
    getLibraryLinkDependencies(const std::string&);

    std::string
    sortLibraryList(const std::string&);

    void
    generateMakeFile(void);

    void
    buildLibraries(const std::string&);

    void
    cleanLibraries(void);

#ifndef __CYGWIN
    void
    treatWin32(void);
#endif /* __CYGWIN */

    std::ofstream makeFile;

    std::map<std::string,std::pair<std::vector<std::string>,
				   std::vector<std::string> > > targets;

    std::map<std::string,std::set<std::string> > sources;

    std::map<std::string,std::vector<std::string> > dependencies;

    std::set<std::string> interfaces;

    std::set<std::string> inputs;

    std::set<std::string> globalIncludes;

    std::set<std::string> specifiedTargets;

    std::string fileName;

    std::string sys;

    bool verboseMode;

    bool warningMode;

    bool debugMode;

    bool oflags;

    bool genMake;

    bool buildLibs;

    bool cleanLibs;

    bool nodeps;

    bool melt;

  }; // end of class MFront

} // end of namespace mfront

#endif /* _LIB_MFRONT_H */

