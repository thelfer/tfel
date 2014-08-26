/*!
 * \file   MFrontCyranoInterface.hxx
 * \brief  This file declares the MFrontCyranoInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONTCYRANOINTERFACE_H_
#define _LIB_MFRONTCYRANOINTERFACE_H_ 

#include<string>
#include<fstream>

#include"MFront/InterfaceBase.hxx"
#include"MFront/MFrontUMATInterfaceBase.hxx"

namespace mfront{

  /*!
   * Interface for behaviours for use in the Cast3M finite element
   * code.
   * 
   * The name of this interface comes to the fact that the Cast3M
   * choose to use an interface for user defined behaviours closed to
   * the umat interface of the Abaqus finite element solver. However,
   * conventions of Cast3M makes those two interfaces incompatibles.
   */
  struct MFrontCyranoInterface
    : public MFrontUMATInterfaceBase,
      protected InterfaceBase
  {

    static std::string 
    getName(void);
    
    MFrontCyranoInterface();

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&);
    
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    
    virtual void
    endTreatement(const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::map<std::string,std::string>&,
		  const std::map<std::string,std::string>&,
		  const MechanicalBehaviourDescription&);

    virtual void
    reset(void);

    ~MFrontCyranoInterface();
    
  protected:

    virtual std::string
    getLibraryName(const std::string&,
		   const std::string&) const;

    virtual std::string
    getInterfaceName(void) const;

    virtual std::string
    getFunctionName(const std::string&) const;

    std::string
    getBehaviourName(const std::string&,
		     const std::string&) const;

    virtual std::string
    getUmatFunctionName(const std::string&,
			const std::string&) const;


    virtual bool
    hasMaterialPropertiesOffset(const MechanicalBehaviourDescription&) const;

    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const;

    /*!
     * \param[in] out : output file
     * \param[in] mb  : behaviour description
     */
    virtual void
    writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(std::ostream&,
								      const MechanicalBehaviourDescription&) const;

    /*!
     * \param[in] out : output file
     */
    virtual void
    writeUMATxxMaterialPropertiesSymbols(std::ostream&,
					 const std::string&,
					 const MechanicalBehaviourDescription&,
					 const std::map<std::string,std::string>&,
					 const std::map<std::string,std::string>&) const;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] mb            : behaviour description
     */
    virtual void
    writeUMATxxBehaviourTypeSymbols(std::ostream&,
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
				 const std::map<std::string,std::string>&) const;
    /*!
     * \param[in] mb : behaviour description
     */
    virtual bool
    checkIfElasticPropertiesAreDeclared(const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] mb  : behaviour description
     * \param[in] mps : required materials properties
     */
    virtual bool
    doElasticPropertiesCheck(const MechanicalBehaviourDescription&,
			     const std::vector<std::string>&) const;

    virtual void
    writeCyranoFortranFunctionDefine(std::ostream&,
				     const std::string&);

    virtual void
    writeCyranoFunctionDeclaration(std::ostream&,
				   const std::string&);

    virtual void
    writeStandardCyranoFunction(std::ostream&,
				const std::string&,
				const std::string&,
				const std::string&,
				const std::string&,
				const std::map<std::string,std::string>&,
				const std::map<std::string,std::string>&,
				const MechanicalBehaviourDescription&) const;

    bool useTimeSubStepping;

    bool doSubSteppingOnInvalidResults;

    unsigned short maximumSubStepping;

  }; // end of MFrontCyranoInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTCYRANOINTERFACE_H */
