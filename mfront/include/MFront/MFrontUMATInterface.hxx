/*!
 * \file   MFrontUMATInterface.hxx
 * \brief  This file declares the MFrontUMATInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONTUMATINTERFACE_H_
#define _LIB_MFRONTUMATINTERFACE_H_ 

#include<string>
#include<fstream>

#include"MFront/MFrontUMATInterfaceBase.hxx"

namespace mfront{

  struct MFrontUMATInterface
    : public MFrontUMATInterfaceBase
  {
    
    enum FiniteStrainStrategy{
      NONE,
      FINITEROTATIONSMALLSTRAIN
    }; // end of enum FiniteStrainStrategy

    static std::string 
    getName(void);
    
    MFrontUMATInterface();

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

    ~MFrontUMATInterface();
    
  protected:

    virtual std::string
    getLibraryName(const std::string&,
		   const std::string&) const;

    static std::string
    treatScalar(const std::string& s);

    static std::string
    treatStensor(const std::string& s,const unsigned short);

    static std::string
    treatCoefScalar(const std::string& s);

    static std::string
    treatCoefStensor(const std::string& s,const unsigned short);

    static std::string
    treatStateVarScalar(const std::string& s);

    static std::string
    treatStateVarStensor(const std::string& s,const unsigned short);

    virtual std::string
    getInterfaceName(void) const;

    virtual std::string
    getFunctionName(const std::string&) const;

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
    generateGibianeDeclaration(const std::string&,
			       const std::string&,
			       const std::string&,
			       const std::string&,
			       const MechanicalBehaviourDescription&);

    FiniteStrainStrategy finiteStrainStrategy;

    bool useTimeSubStepping;

    bool doSubSteppingOnInvalidResults;

    unsigned short maximumSubStepping;

  }; // end of MFrontUMATInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTUMATINTERFACE_H */
