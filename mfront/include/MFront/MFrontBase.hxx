/*!
 * \file  MFrontBase.hxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBASE_H
#define LIB_MFRONT_MFRONTBASE_H

#include <set>
#include <string>
#include <vector>
#include <memory>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/TargetsDescription.hxx"

namespace mfront {

  // forward declaration
  struct AbstractDSL;

  /*!
   * Base class for the MFront and MFrontQuery classes
   */
  struct MFRONT_VISIBILITY_EXPORT MFrontBase {
    /*!
     * \param[in] f : file name
     * \return an abstract dsl that will handle the file
     */
    static std::shared_ptr<AbstractDSL> getDSL(const std::string&);
    //! constructor
    MFrontBase();
    //! add a new interaface
    void setInterface(const std::string&);
    //! desctructor
    virtual ~MFrontBase();

   protected:
    //! return the current Argument
    virtual const tfel::utilities::Argument& getCurrentCommandLineArgument()
        const = 0;
    virtual bool treatUnknownArgumentBase(void);
    //! treat the --verbose command line option
    virtual void treatVerbose(void);
    //! treat the --search-path command line option
    virtual void treatSearchPath(void);
    //! treat the --warning command line option
    virtual void treatWarning(void);
    //! treat the --debug command line option
    virtual void treatDebug(void);
    //! treat the --pedantic command line option
    virtual void treatPedantic(void);
    //! treat the --interface command line option
    virtual void treatInterface(void);
    /*!
     * \brief external commands specificed on the command line through
     * an --@XXX option
     */
    std::vector<std::string> ecmds;
    /*!
     * \brief substitutions specificed on the command line through an
     * --@XXX@=YYY option: every occurrence of @XXX@ in the input
     * files will be replaced by YYY
     */
    std::map<std::string, std::string> substitutions;
    //! list of all input files
    std::set<std::string> inputs;
    //! list of interfaces declared on the command line or explicitely
    //! added through the set interface method
    std::set<std::string> interfaces;
  };  // end of struct MFrontBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBASE_H */
