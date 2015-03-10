/*!
 * \file   mfront/include/MFront/AbstractDSL.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABSTRACTDSL_HXX_
#define LIB_MFRONT_ABSTRACTDSL_HXX_ 

#include<map>
#include<set>
#include<vector>
#include<string>

#include"MFront/MFrontConfig.hxx"

namespace mfront{

  // forward declaration
  struct FileDescription;

  /*!
   * Interface class for all domain specific languages.
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractDSL
  {
    //! list of dsl targets
    enum DSLTarget{
      MATERIALPROPERTYDSL,
      BEHAVIOURDSL,
      MODELDSL
    }; // end of 
    //! \return the target of the dsl
    virtual DSLTarget getTargetType(void) const = 0;
    //! \return the file description associated with the treated file
    virtual const FileDescription& getFileDescription() const = 0;
    /*!
     * \brief register a name.
     * \param[in] w : name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     * \note this method is called internally by the registerVariable
     * and registerStaticVariable methods.
     */
    virtual void
    reserveName(const std::string&,
		const bool) = 0;
    /*!
     * \brief register a variable name.
     * \param[in] v : variable name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     */
    virtual void
    registerVariable(const std::string&,
		     const bool) = 0;
    /*!
     * \brief register a static variable.
     * \param[in] v : variable name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     */
    virtual void
    registerStaticVariable(const std::string&) = 0;
    /*!
     * \brief analyse the specified file.
     * \param[in] f : file name
     * \param[in] c : keywords and arguments specificed on the command
     * line
     * \note The keywords and arguments specificed on the command
     * line are appended at the beginning of the file.
     */
    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>&) = 0;
    /*!
     * \brief treat the specified file.
     * \note This method shall be called *after* the analyseFile method
     */
    virtual void generateOutputFiles() = 0;

    virtual void
    setInterfaces(const std::set<std::string>&) = 0;

    virtual void
    setAnalysers(const std::set<std::string>&) = 0;

    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void
    getKeywordsList(std::vector<std::string>&) const = 0;

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
    /*!
     * \return a map associating to each library a list of entry
     * points (function or classes)
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedEntryPoints(void) = 0;
    /*!
     * An abstract dsl can define its own targets (something which is
     * not a library)
     * \return a map associating a target and a list of dependencies
     * and a list of command to build the target.
     * This will define the following Makefile rule:
     * \code{.txt}
     * target : dep1 dep2 ...
     *    cmd1
     *    cmd2
     *    ....
     * \endcode
     */
    virtual std::map<std::string,                           //< target name
		     std::pair<std::vector<std::string>,    //< dependencies to other targets
			       std::vector<std::string> > > //< commands
    getSpecificTargets(void) = 0;
    //! destructor
    virtual ~AbstractDSL();
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTDSL_HXX_ */

