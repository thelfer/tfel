/*!
 * \file  BehaviourDocumentationGenerator.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H
#define LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct FileDescription;

  /*!
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct BehaviourDocumentationGenerator
      : public tfel::utilities::ArgumentParserBase<
            BehaviourDocumentationGenerator>,
        public MFrontBase {
    /*!
     * type of documentation to be generated
     */
    enum OutputType { FULL, WEB };
    /*!
     * build a BehaviourDocumentationGenerator object based on command line
     * arguments \param[in] argc : number of command line arguments \param[in]
     * argv : command line arguments \param[in] d    : behaviour domain specific
     * language \param[in] f    : behaviour domain specific language
     */
    BehaviourDocumentationGenerator(const int,
                                    const char *const *const,
                                    std::shared_ptr<AbstractBehaviourDSL>,
                                    const std::string &);
    //! treat the requests
    void exe(void);
    //! destructor
    ~BehaviourDocumentationGenerator();

   private:
    //! ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<
        BehaviourDocumentationGenerator>;
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks(void);
    //! return the current argument
    virtual const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
    //! treat an unknown argument
    virtual void treatWeb(void);
    //! treat an unknown argument
    virtual void treatUnknownArgument(void) final;
    //! get the version description
    virtual std::string getVersionDescription(void) const override final;
    //! get the usage description
    virtual std::string getUsageDescription(void) const override final;
    virtual void writeWebOutput(std::ostream &,
                                const BehaviourDescription &,
                                const FileDescription &) const;
    virtual void writeFullOutput(std::ostream &,
                                 const BehaviourDescription &,
                                 const FileDescription &) const;
    //! abstract behaviour dsl
    std::shared_ptr<AbstractBehaviourDSL> dsl;
    //! file name
    std::string file;
    //! type of ouput
    OutputType otype;
  };  // end of struct BehaviourDocumentationGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H */
