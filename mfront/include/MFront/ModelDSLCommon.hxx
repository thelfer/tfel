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

#include<vector>
#include<map>
#include<set>
#include<string>
#include<fstream>

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
    //! \return the target of the dsl
    virtual DSLTarget getTargetType(void) const final;
    /*!
     * \brief write the output files.
     * \note this shall be called after the analyseFile method.
     */
    virtual void generateOutputFiles(void) override;

    virtual void
    setInterfaces(const std::set<std::string>&);

    /*!
     * set the analysers to be used
     */
    virtual void
    setAnalysers(const std::set<std::string>&);

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    virtual std::vector<std::string>
    getGeneratedIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);

    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(void);
    /*!
     * \return a map associating to each library a list of entry
     * points (function or classes)
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedEntryPoints(void) override;

    static bool
    is(const ModelData&,
       const VariableDescriptionContainer&,
       const std::string&);

    virtual ~ModelDSLCommon();

  protected:

    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName(void) const;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    virtual void addMaterialLaw(const std::string&);
    /*!
     * \brief append the given code to the includes
     */
    virtual void appendToIncludes(const std::string&);
    /*!
     * \brief append the given code to the members
     */
    virtual void appendToMembers(const std::string&);
    /*!
     * \brief append the given code to the private code
     */
    virtual void appendToPrivateCode(const std::string&);
    /*!
     * \brief append the given code to the sources
     */
    virtual void appendToSources(const std::string&);
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void
    addStaticVariableDescription(const StaticVariableDescription&);
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

    void
    treatUnknownKeyword(void);

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
    treatGlobalParameter(void);

    void
    treatGlobalParameterMethod(void);

    void
    treatLocalParameter(void);

    void
    treatLocalParameterMethod(void);

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
    
    std::map<std::string,
	     std::vector<std::string> > sourcesLibrairiesDependencies;

    std::string currentVar;

  }; // end of class ModelDSLCommon

} // end of namespace mfront  

#endif /* LIB_MFRONTMODELPARSERCOMMON_H_ */

