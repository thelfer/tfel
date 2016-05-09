/*!
 * \file   mfront/include/MFront/MaterialPropertyDSL.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jui 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTMATERIALLAWPARSER_HXX_
#define LIB_MFRONTMATERIALLAWPARSER_HXX_ 

#include<string>
#include<set>

#include"MFront/DSLBase.hxx"
#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"

namespace mfront{

  // forward declaration
  struct AbstractMaterialPropertyInterface;

  /*!
   * Parser handling material properties
   */
  struct MFRONT_VISIBILITY_EXPORT MaterialPropertyDSL
    : public DSLBase,
      protected MaterialPropertyDescription
  {
    //! \return the target of the dsl
    virtual DSLTarget getTargetType(void) const override final;
    /*!
     * return the name of the parser
     */
    static std::string 
    getName(void);
    /*!
     * return a description of the parser
     */
    static std::string
    getDescription(void);
    /*!
     * default constructor
     */
    MaterialPropertyDSL();

    virtual void
    getKeywordsList(std::vector<std::string>&) const override;

    virtual void
    setInterfaces(const std::set<std::string>&) override;
    /*!
     * \brief analyse a file without generating any output
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     */
    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>& = {},
		const std::map<std::string,std::string>& = {}) override;
    /*!
     * \brief analyse the specified string.
     * \param[in] s : analyse a string
     */
    virtual void
    analyseString(const std::string&) override;
    /*!
     * \brief import a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commands.
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    virtual void importFile(const std::string&,
			    const std::vector<std::string>&,
			    const std::map<std::string,std::string>&) override;
    
    virtual const MaterialPropertyDescription&
    getMaterialPropertyDescription(void) const;

    //! destructor
    virtual ~MaterialPropertyDSL();

  protected:

    typedef void (MaterialPropertyDSL::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;
    /*!
     * \brief register a name.
     * \param[in] n : name
     */
    virtual void reserveName(const std::string&) override;
    /*!
     * method called by the analyseFile and analyseString method
     */
    virtual void analyse(void);
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName(void) const override;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    virtual void addMaterialLaw(const std::string&) override;
    /*!
     * \brief append the given code to the includes
     */
    virtual void appendToIncludes(const std::string&) override;
    /*!
     * \brief append the given code to the members
     */
    virtual void appendToMembers(const std::string&) override;
    /*!
     * \brief append the given code to the private code
     */
    virtual void appendToPrivateCode(const std::string&) override;
    /*!
     * \brief append the given code to the sources
     */
    virtual void appendToSources(const std::string&) override;
    /*!
     * \brief write the output files.
     * \note this shall be called after the analyseFile method.
     */
    virtual void generateOutputFiles(void) override;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void
    addStaticVariableDescription(const StaticVariableDescription&) override;

    void
    addInterface(const std::string&);
    /*!
     * treat the material keyword
     */
    virtual void
    treatMaterial(void);
    /*!
     * treat the library keyword
     */
    virtual void
    treatLibrary(void);

    void
    treatOutput(void);

    void
    treatInput(void);

    void
    treatLaw(void);

    void
    treatFunction(void);

    /*!
     * treat the setGlossaryName and the setEntryName method for
     * inputs or output.
     */
    void
    treatMethod(void);

    void
    treatInterface(void);

    void
    treatParameter(void);

    void
    treatConstant(void);

    void
    treatBounds(void);

    void
    treatPhysicalBounds(void);

    void
    treatUnknownKeyword(void) override;

    void
    registerBounds(std::vector<VariableBoundsDescription>&);
    
    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);
    /*!
     * list of registred interfaces
     */
    std::map<std::string,
	     std::shared_ptr<AbstractMaterialPropertyInterface>> interfaces;
    /*!
     * list of resgistred keywords
     */
    std::set<std::string> registredKeyWords;
    /*!
     * map of callbacks used during the file interpretation
     */
    CallBackContainer callBacks;

    /*!
     * a temporary object used to store the name of the variable on
     * which a method is called
     */
    std::string currentVar;
  }; // end of class MaterialPropertyDSL

} // end of namespace mfront  
  
#endif /* LIB_MFRONTMATERIALLAWPARSER_HXX_ */

