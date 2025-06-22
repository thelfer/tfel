/*!
 * \file   mfront/include/MFront/MFront.hxx
 * \brief  This file declares the MFront class
 * \author Thomas Helfer
 * \date   22 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONT_HXX
#define LIB_MFRONT_MFRONT_HXX

#include <set>
#include <map>
#include <string>
#include <vector>
#include <iosfwd>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GeneratorOptions.hxx"
#include "MFront/TargetsDescription.hxx"

namespace mfront {

  /*!
   * \brief the main class of MFront
   */
  struct MFRONT_VISIBILITY_EXPORT MFront
      : public tfel::utilities::ArgumentParserBase<MFront>,
        public MFrontBase {
    //! \brief constructor
    MFront();
    /*!
     * \brief constructor
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     */
    MFront(const int, const char *const *const);
    /*!
     * \brief treat a file (analyse and generate output files)
     * \param[in] f : file name
     * \return the target's description
     */
    virtual TargetsDescription treatFile(const std::string &) const;
    //! \brief execute MFront process
    virtual void exe();
    //! \brief destructor
    ~MFront() override;

   protected:
    friend struct tfel::utilities::ArgumentParserBase<MFront>;

    //! treat an unknown argument
    void treatUnknownArgument() override final;
    //! a do nothing callback
    virtual void doNothing() final;
    //! return the current argument
    const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
    //! get the version description
    std::string getVersionDescription() const override final;
    //! get the usage description
    std::string getUsageDescription() const override final;
    //! treat the -G command line option
    virtual void treatGenerator();
    //! treat the -D command line option
    virtual void treatDefine();
    //! treat the --help-commands-list command line option
    virtual void treatHelpCommandsList();
    //! treat the --help-commands command line option
    virtual void treatHelpCommands();
    //! treat the --help-command command line option
    virtual void treatHelpCommand();
    //! treat the --help-behaviour-brick command line option
    virtual void treatHelpBehaviourBrick();
    //! treat the --no-melt command line option
    virtual void treatNoMelt();
    //! treat the --silent-build command line option
    virtual void treatSilentBuild();

    virtual void treatNoDeps();

    virtual void treatOMake();

    virtual void treatOBuild();

    virtual void treatMake();

    virtual void treatBuild();

    virtual void treatClean();

    virtual void treatTarget();

    virtual void treatOTarget();

    virtual void treatListParsers();

    virtual void registerArgumentCallBacks();

    virtual void analyseTargetsFile();

    virtual void writeTargetsDescription() const;

    virtual void buildLibraries(const std::string &);

    virtual void cleanLibraries();

#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    //! treat the --def-file command line option
    virtual void treatDefFile();
    //! generate def files
    virtual void generateDefsFiles();
#endif

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    virtual void treatWin32();
#endif

#ifndef _MSC_VER
    enum {MAKE, CMAKE} generator = MAKE;
#else
    enum { MAKE, CMAKE } generator = CMAKE;
#endif

    //! description of the targets that can be build
    TargetsDescription targets;

    //! generator options
    GeneratorOptions opts;
    //! targets to be build as specified by the user
    std::set<std::string> specifiedTargets;
    //! macros defined by the user on the command line
    std::set<std::string> defines;

#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    // libraries for which a def file must be generated
    std::set<std::string> defs;
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */

    bool genMake = false;

    bool buildLibs = false;

    bool cleanLibs = false;

  };  // end of class MFront

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONT_HXX */
