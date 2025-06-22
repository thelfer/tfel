/*!
 * \file   mfront/include/MFront/MFront.hxx
 * \brief  This file declares the MFront class
 * \author Helfer Thomas
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

#if (!(defined _WIN32 || defined _WIN64)) || \
    ((defined __CYGWIN__) || (defined __MINGW32__)) || defined(__MINGW64__)
#define MFRONT_MAKE_SUPPORT
#endif /* (!(defined _WIN32 || defined _WIN64)) || ((defined __CYGWIN__)|| \
          (defined __MINGW32__)) || defined(__MINGW64__)  */

namespace mfront {

  /*!
   * \brief the main class of MFront
   */
  struct MFRONT_VISIBILITY_EXPORT MFront
      : public tfel::utilities::ArgumentParserBase<MFront>,
        public MFrontBase {
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
    virtual TargetsDescription treatFile(const std::string &) const;
    /*!
     * \brief execute MFront process
     */
    virtual void exe(void);
    //! destructor
    virtual ~MFront();

   protected:
    friend struct tfel::utilities::ArgumentParserBase<MFront>;

    //! treat an unknown argument
    virtual void treatUnknownArgument(void) final;
    //! a do nothing callback
    virtual void doNothing(void) final;
    //! return the current argument
    virtual const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
    //! get the version description
    virtual std::string getVersionDescription(void) const override final;
    //! get the usage description
    virtual std::string getUsageDescription(void) const override final;
    //! treat the -D command line option
    virtual void treatDefine(void);
    //! treat the --help-commands-list command line option
    virtual void treatHelpCommandsList(void);
    //! treat the --help-commands command line option
    virtual void treatHelpCommands(void);
    //! treat the --help-command
    virtual void treatHelpCommand(void);
    //! treat the --no-melt command line option
    virtual void treatNoMelt(void);
    //! treat the --silent-build command line option
    virtual void treatSilentBuild(void);

#ifdef MFRONT_MAKE_SUPPORT

    virtual void treatNoDeps(void);

    virtual void treatOMake(void);

    virtual void treatOBuild(void);

    virtual void treatMake(void);

    virtual void treatBuild(void);

    virtual void treatClean(void);

    virtual void treatTarget(void);

    virtual void treatOTarget(void);

#endif /* MFRONT_MAKE_SUPPORT */

    virtual void treatListParsers(void);

    virtual void registerArgumentCallBacks(void);

    virtual void analyseTargetsFile(void);

    virtual void writeTargetsDescription(void) const;

#ifdef MFRONT_MAKE_SUPPORT
    virtual void buildLibraries(const std::string &);

    virtual void cleanLibraries(void);
#endif /* MFRONT_MAKE_SUPPORT */

#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    //! treat the --def-file command line option
    virtual void treatDefFile(void);
    //! generate def files
    virtual void generateDefsFiles(void);
#endif

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    virtual void treatWin32(void);
#endif /* LIB_MFRONT_H_ */

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

#ifdef MFRONT_MAKE_SUPPORT

    bool genMake = false;

    bool buildLibs = false;

    bool cleanLibs = false;

#endif /* MFRONT_MAKE_SUPPORT */

  };  // end of class MFront

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONT_H_ */
