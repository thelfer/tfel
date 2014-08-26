/*! 
 * \file  MFrontUMATInterfaceBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 */

#ifndef _LIB_MFRONT_MFRONTUMATINTERFACEBASE_H_
#define _LIB_MFRONT_MFRONTUMATINTERFACEBASE_H_ 

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/SupportedTypes.hxx"
#include"MFront/VarHandler.hxx"
#include"MFront/MechanicalBehaviourDescription.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{
  
  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct MFrontUMATInterfaceBase
    : public SupportedTypes,
      public MFrontBehaviourVirtualInterface
  {

    MFrontUMATInterfaceBase();

    virtual void
    setVerboseMode(void);

    virtual void
    setDebugMode(void);

    virtual void
    setWarningMode(void);

    /*!
     * set if dynamically allocated arrays are allowed
     * \param[in] b : boolean
     */
    virtual void
    allowDynamicallyAllocatedArrays(const bool);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&);

    virtual void
    exportMechanicalData(std::ofstream&,
			 const std::string&,
			 const MechanicalBehaviourDescription&);
    
    virtual void
    writeBehaviourConstructor(std::ofstream&,
			      const std::string&,
			      const MechanicalBehaviourDescription&,
			      const std::string&);
    
    virtual void
    writeBehaviourDataConstructor(std::ofstream&,
				  const std::string&,
				  const MechanicalBehaviourDescription&);
    
    virtual void
    writeIntegrationDataConstructor(std::ofstream&,
				    const std::string&,
				    const MechanicalBehaviourDescription&);


    virtual void
    reset(void);
    
    virtual ~MFrontUMATInterfaceBase();

  protected:

    static const std::string&
    getGlossaryName(const std::map<std::string,std::string>&,
		    const std::map<std::string,std::string>&,
		    const std::string&);

    virtual std::string
    getLibraryName(const std::string&,
		   const std::string&) const = 0;

    virtual bool
    readBooleanValue(const std::string&,
		     tfel::utilities::CxxTokenizer::TokensContainer::const_iterator&,
		     const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) const;

    virtual bool
    hasMaterialPropertiesOffset(const MechanicalBehaviourDescription&) const = 0;

    virtual std::string
    getInterfaceName(void) const = 0;

    virtual std::string
    getFunctionName(const std::string&) const = 0;

    virtual std::vector<std::string>
    getGlossaryNames(const VarContainer&,
		     const std::map<std::string,std::string>&,
		     const std::map<std::string,std::string>&) const;

    virtual void
    appendGlossaryNames(std::vector<std::string>&,
			const VarContainer&,
			const std::map<std::string,std::string>&,
			const std::map<std::string,std::string>&) const;


    virtual void
    writeGlossaryNames(std::ostream&,
		       const std::vector<std::string>&,
		       const std::string&,
		       const std::string&,
		       const unsigned short = 0u) const;

    virtual void
    checkParametersType(bool&,
			bool&,
			bool&,
			const VarContainer&) const;

    virtual std::string
    getHeaderDefine(const std::string&,
		    const std::string&,
		    const std::string&) const;

    virtual void
    getExtraSrcIncludes(std::ostream&,
			const MechanicalBehaviourDescription&);

    virtual void
    writeVisibilityDefines(std::ostream&) const;

    virtual void
    writeSetParametersFunctionsDeclarations(std::ostream&,
					    const std::string&,
					    const MechanicalBehaviourDescription&) const;

    virtual void
    writeSetParametersFunctionsImplementations(std::ostream&,
					       const std::string&,
					       const std::string&,
					       const MechanicalBehaviourDescription&) const;

    virtual void
    generateMTestFile1(std::ostream& out) const;

    virtual void
    generateMTestFile2(std::ostream&,
		       const std::string&,
		       const std::string&,
		       const std::string&,
		       const MechanicalBehaviourDescription&,
		       const std::map<std::string,std::string>&,
		       const std::map<std::string,std::string>&) const;

    virtual void
    writeMTestFileGeneratorSetRotationMatrix(std::ostream&,
					     const MechanicalBehaviourDescription&) const;

    virtual void
    writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(std::ostream&,
								      const MechanicalBehaviourDescription&) const = 0;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] file          : source file 
     * \param[in] mb            : behaviour description
     * \param[in] glossaryNames : glossary names
     * \param[in] entrNames     : entry    names
     */
    virtual void
    generateUMATxxSymbols(std::ostream&,
			  const std::string&,
			  const std::string&,
			  const MechanicalBehaviourDescription&,
			  const std::map<std::string,std::string>&,
			  const std::map<std::string,std::string>&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxBehaviourTypeSymbols(std::ostream&,
				    const std::string&,
				    const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] file : source file
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxSourceFileSymbols(std::ostream&,
				 const std::string&,
				 const std::string&,
				 const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] mb            : behaviour description
     */
    virtual void
    writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(std::ostream&,
							 const std::string&,
							 const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] mb            : behaviour description
     * \param[in] glossaryNames : glossary names
     * \param[in] entrNames     : entry    names
     */
    virtual void
    writeUMATxxMaterialPropertiesSymbols(std::ostream&,
					 const std::string&,
					 const MechanicalBehaviourDescription&,
					 const std::map<std::string,std::string>&,
					 const std::map<std::string,std::string>&) const = 0;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] mb            : behaviour description
     * \param[in] glossaryNames : glossary names
     * \param[in] entrNames     : entry    names
     */
    virtual void
    writeUMATxxStateVariablesSymbols(std::ostream&,
				      const std::string&,
				      const MechanicalBehaviourDescription&,
				      const std::map<std::string,std::string>&,
				      const std::map<std::string,std::string>&) const;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] mb            : behaviour description
     * \param[in] glossaryNames : glossary names
     * \param[in] entrNames     : entry    names
     */
    virtual void
    writeUMATxxExternalStateVariablesSymbols(std::ostream&,
					     const std::string&,
					     const MechanicalBehaviourDescription&,
					     const std::map<std::string,std::string>&,
					     const std::map<std::string,std::string>&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxSymmetryTypeSymbols(std::ostream&,
				    const std::string&,
				    const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxElasticSymmetryTypeSymbols(std::ostream&,
					   const std::string&,
					   const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] file          : source file
     * \param[in] mb            : behaviour description
     * \param[in] glossaryNames : glossary names
     * \param[in] entrNames     : entry    names
     */
    virtual void
    writeUMATxxAdditionalSymbols(std::ostream&,
				 const std::string&,
				 const std::string&,
				 const MechanicalBehaviourDescription&,
				 const std::map<std::string,std::string>&,
				 const std::map<std::string,std::string>&) const = 0;
    /*!
     * \param[in] out : output file
     */
    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const = 0;

    bool generateMTestFile;
    
    bool verboseMode;

    bool debugMode;

    bool warningMode;

  }; // end of struct MFrontUMATInterfaceBase

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTUMATINTERFACEBASE_H */

