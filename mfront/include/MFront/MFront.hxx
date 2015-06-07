/*!
 * \file   mfront/include/MFront/MFront.hxx
 * \brief  This file declares the MFront class
 * \author Helfer Thomas
 * \date   22 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONT_H_
#define LIB_MFRONT_MFRONT_H_ 

#include<set>
#include<map>
#include<string>
#include<vector>
#include<fstream>

#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"MFront/MFrontBase.hxx"
#include"MFront/MFrontConfig.hxx"
#include"MFront/TargetsDescription.hxx"

namespace mfront{

  /*!
   * \brief the main class of MFront
   */
  struct MFRONT_VISIBILITY_EXPORT MFront
    : public tfel::utilities::ArgumentParserBase<MFront>,
      public MFrontBase
  {
    //! constructor
    MFront();
    /*!
     * constructor
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     */
    MFront(const int, const char *const *const);
    /*!
     * \brief treat a file (analyse and generate output files)
     * \param[in] f : file name
     * \return the target's description
     */
    virtual TargetsDescription
    treatFile(const std::string&) const;
    /*!
     * \brief execute MFront process
     */
    virtual void exe(void);
    //! destructor
    virtual ~MFront();

  protected :

    friend struct tfel::utilities::ArgumentParserBase<MFront>;

    //! treat an unknown argument
    virtual void treatUnknownArgument(void) final;
    //! return the current argument
    virtual const tfel::utilities::Argument&
    getCurrentCommandLineArgument() const override final;
    //! get the version description
    virtual std::string
    getVersionDescription(void) const override final;
    //! get the usage description
    virtual std::string
    getUsageDescription(void) const override final;
    /*!
     * \brief merge the given target's description with the internal
     * structure used to generate Makefiles
     */
    virtual void mergeTargetsDescription(const TargetsDescription&);

    virtual void treatHelpCommandsList(void);

    virtual void treatHelpCommand(void);

    virtual void treatSilentBuild(void);

    virtual void treatNoDeps(void);

    virtual void treatNoMelt(void);

    virtual void treatOMake(void);

    virtual void treatOBuild(void);

    virtual void treatMake(void);

    virtual void treatBuild(void);

    virtual void treatClean(void);

    virtual void treatTarget(void);

    virtual void treatOTarget(void);

    virtual void treatListParsers(void);

    virtual void registerArgumentCallBacks(void);

    virtual void analyseSourceDirectory(void);

    virtual void analyseSources(const std::string&);

    virtual void analyseEntryPoints(const std::string&);

    virtual void analyseDependencies(const std::string&);

    virtual void analyseMakefileSpecificTargets(void);

    virtual void analyseCppFlags(void);

    virtual void writeSourcesLists(void);

    virtual void writeEntryPointsLists(void);

    virtual void writeDependenciesLists(void);

    virtual void writeSpecificTargets(void);

    virtual void writeCppFlags(void);

    virtual std::pair<bool,std::pair<std::string,std::string> >
    getLibraryDependencies(const std::string&);

    virtual std::string
    getLibraryLinkDependencies(const std::string&);

    virtual std::string
    sortLibraryList(const std::string&);

    virtual void
    generateMakeFile(void);

    virtual void
    buildLibraries(const std::string&);

    virtual void
    cleanLibraries(void);

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ || defined __APPLE__)
    virtual void
    treatWin32(void);
#endif /* LIB_MFRONT_H_ */

    std::ofstream makeFile;

    std::map<std::string,std::pair<std::vector<std::string>,
				   std::vector<std::string> > > targets;

    std::map<std::string,std::set<std::string> > sources;

    std::map<std::string,std::set<std::string> > epts;

    std::map<std::string,std::vector<std::string> > dependencies;

    std::set<std::string> analysers;

    std::set<std::string> cppflags;

    std::set<std::string> specifiedTargets;

    std::string sys;

    bool oflags0;

    bool oflags;

    bool oflags2;

    bool genMake;

    bool buildLibs;

    bool cleanLibs;

    bool silentBuild;

    bool nodeps;

    bool melt;

  }; // end of class MFront

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONT_H_ */
