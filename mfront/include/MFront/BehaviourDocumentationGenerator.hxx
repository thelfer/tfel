/*! 
 * \file  BehaviourDocumentationGenerator.hxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H
#define LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H 

#include<string>
#include<vector>
#include<memory>
#include<utility>
#include<functional>

#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/MFrontBase.hxx"

namespace mfront{

  // forward declaration
  struct AbstractBehaviourDSL;

  /*!
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct BehaviourDocumentationGenerator
    : public tfel::utilities::ArgumentParserBase<BehaviourDocumentationGenerator>,
      public MFrontBase
  {
    /*!
     * build a BehaviourDocumentationGenerator object based on command line arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : behaviour domain specific language
     */
    BehaviourDocumentationGenerator(const int, const char *const *const,
				    std::shared_ptr<AbstractBehaviourDSL>,
				    const std::string&);
    //! treat the requests
    void exe(void);
    //! destructor
    ~BehaviourDocumentationGenerator();
  private:
    //! ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<BehaviourDocumentationGenerator>;
    //! \brief register call-backs associated with command line arguments
    void registerCommandLineCallBacks(void);
    //! return the current argument
    virtual const tfel::utilities::Argument&
    getCurrentCommandLineArgument() const final;
    //! treat an unknown argument
    virtual void treatUnknownArgument(void) final;
    //! get the version description
    virtual std::string
    getVersionDescription(void) const final;
    //! get the usage description
    virtual std::string
    getUsageDescription(void) const final;
    //! abstract behaviour dsl
    std::shared_ptr<AbstractBehaviourDSL> dsl;
    //! file name
    std::string file;    
  }; // end of struct BehaviourDocumentationGenerator

} // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H */

