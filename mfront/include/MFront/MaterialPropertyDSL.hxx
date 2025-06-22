/*!
 * \file   mfront/include/MFront/MaterialPropertyDSL.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   06 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMATERIALLAWPARSER_HXX
#define LIB_MFRONTMATERIALLAWPARSER_HXX

#include <string>
#include <set>

#include "MFront/DSLBase.hxx"
#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"

namespace mfront {

  // forward declaration
  struct AbstractMaterialPropertyInterface;

  /*!
   * Parser handling material properties
   */
  struct MFRONT_VISIBILITY_EXPORT MaterialPropertyDSL : public DSLBase {
    //! \return the target of the dsl
    virtual DSLTarget getTargetType() const override final;
    /*!
     * return the name of the parser
     */
    static std::string getName();
    /*!
     * return a description of the parser
     */
    static std::string getDescription();
    /*!
     * default constructor
     */
    MaterialPropertyDSL();

    virtual void getKeywordsList(std::vector<std::string>&) const override;

    virtual void setInterfaces(const std::set<std::string>&) override;
    /*!
     * \brief analyse a file without generating any output
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     */
    virtual void analyseFile(
        const std::string&,
        const std::vector<std::string>& = {},
        const std::map<std::string, std::string>& = {}) override;
    /*!
     * \brief analyse the specified string.
     * \param[in] s : analyse a string
     */
    virtual void analyseString(const std::string&) override;
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
                            const std::map<std::string, std::string>&) override;

    virtual const MaterialPropertyDescription& getMaterialPropertyDescription()
        const;

    //! destructor
    virtual ~MaterialPropertyDSL();

   protected:
    typedef void (MaterialPropertyDSL::*MemberFuncPtr)();
    typedef std::map<std::string, MemberFuncPtr> CallBackContainer;
    //! treat an unknown keyword
    virtual void treatUnknownKeyword() override;
    /*!
     * \brief register a name.
     * \param[in] n : name
     */
    virtual void reserveName(const std::string&) override;
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    virtual bool isNameReserved(const std::string&) const override;
    /*!
     * method called by the analyseFile and analyseString method
     */
    virtual void analyse();
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName() const override;
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
    virtual void generateOutputFiles() override;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void addStaticVariableDescription(
        const StaticVariableDescription&) override;
    /*!
     * \return the value of an integer constant
     * \param[in] n: variable name
     */
    virtual int getIntegerConstant(const std::string&) const override;
    /*!
     * \brief add a new interface
     * \param[in] n: interface name
     */
    virtual void addInterface(const std::string&);
    //! treat the `@Material` keyword
    virtual void treatMaterial();
    //! treat the `@Library` keyword
    virtual void treatLibrary();
    //! treat the `@Output` keyword
    virtual void treatOutput();
    //! treat the `@Input` keyword
    virtual void treatInput();
    //! treat the `@Law` keyword
    virtual void treatLaw();
    //! treat the `@Function` keyword
    virtual void treatFunction();
    /*!
     * treat the setGlossaryName and the setEntryName method for
     * inputs or output.
     */
    virtual void treatMethod();
    //! treat the `@Interface` keyword
    virtual void treatInterface();
    //! treat the `@Parameter` keyword
    virtual void treatParameter();
    //! treat the `@Constant` keyword
    virtual void treatConstant();
    //! treat the `@Bounds` keyword
    virtual void treatBounds();
    //! treat the `@PhysicalBounds` keyword
    virtual void treatPhysicalBounds();

    virtual void registerNewCallBack(const std::string&, const MemberFuncPtr);
    //! description of the material property
    MaterialPropertyDescription md;
    /*!
     * list of registred interfaces
     */
    std::map<std::string, std::shared_ptr<AbstractMaterialPropertyInterface>>
        interfaces;
    //! \brief list of resgistred keywords
    std::set<std::string> registredKeyWords;
    //! \brief map of callbacks used during the file interpretation
    CallBackContainer callBacks;
    /*!
     * a temporary object used to store the name of the variable on
     * which a method is called
     */
    std::string currentVar;
  };  // end of class MaterialPropertyDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTMATERIALLAWPARSER_HXX */
