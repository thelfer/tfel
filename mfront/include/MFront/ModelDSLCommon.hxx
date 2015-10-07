/*!
 * \file   mfront/include/MFront/ModelDSLCommon.hxx
 * \brief  This file declares the ModelDSLCommon class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTMODELPARSERCOMMON_H_
#define LIB_MFRONTMODELPARSERCOMMON_H_ 

#include<set>
#include<map>
#include<vector>
#include<string>

#include"MFront/MFrontConfig.hxx"

#include"MFront/DSLBase.hxx"
#include"MFront/AbstractDSL.hxx"
#include"MFront/ModelData.hxx"

namespace mfront{

  // forward declaration
  struct AbstractModelInterface;

  struct MFRONT_VISIBILITY_EXPORT ModelDSLCommon
    : public DSLBase,
      protected ModelData
  {
    static bool
    is(const ModelData&,
       const VariableDescriptionContainer&,
       const std::string&);
    //! constructor
    ModelDSLCommon();
    //! \return the target of the dsl
    virtual DSLTarget getTargetType(void) const override final;
    /*!
     * \brief write the output files.
     * \note this shall be called after the analyseFile method.
     */
    virtual void generateOutputFiles(void) override;

    virtual void
    setInterfaces(const std::set<std::string>&) override;
    //! destructor
    virtual ~ModelDSLCommon();

  protected:
    /*!
     * \brief : add a new library dependency
     * \param[in] l : library dependency
     */
    virtual void addLibraryDependency(const std::string&) override;
    /*!
     * \brief register a name.
     * \param[in] n : name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     * \note this method is called internally by the registerVariable
     * and registerStaticVariable methods.
     */
    virtual void reserveName(const std::string&) override;
    /*!
     * \brief register a static member name
     * \param[in] n : name
     */
    virtual void registerMemberName(const std::string&);
    /*!
     * \brief register a static member name
     * \param[in] n : name
     */
    virtual void registerStaticMemberName(const std::string&);
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName(void) const override ;
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
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void
    addStaticVariableDescription(const StaticVariableDescription&) override;
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
    treatModel(void);

    void
    treatDomain(void);

    void
    treatDomains(void);

    virtual void
    treatUnknownKeyword(void) override;

    void
    treatBounds(void);

    void
    treatPhysicalBounds(void);

    void
    treatConstantMaterialProperty(void);

    void
    treatConstantMaterialPropertyMethod(void);

    void
    treatFunction(void);

    void
    treatOutput(void);

    void
    treatOutputMethod(void);

    void
    treatInput(void);

    void
    treatInputMethod(void);

    void
    treatParameter(void);

    void
    treatParameterMethod(void);

    void
    readDefaultValue(void);

    void
    registerBounds(std::vector<VariableBoundsDescription>&);

    std::pair<std::string,unsigned short>
    decomposeVariableName(const std::string&) const;

    bool
    isOutputVariable(const std::string&) const;

    bool
    isInputVariable(const std::string&) const;

    std::map<std::string,
	     std::shared_ptr<AbstractModelInterface>> interfaces;

    std::string currentVar;
  }; // end of class ModelDSLCommon

} // end of namespace mfront  

#endif /* LIB_MFRONTMODELPARSERCOMMON_H_ */

