/*!
 * \file   mfront/include/MFront/UMATInterface.hxx
 * \brief  This file declares the UMATInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTUMATINTERFACE_H_
#define LIB_MFRONTUMATINTERFACE_H_ 

#include<string>
#include<fstream>

#include"MFront/InterfaceBase.hxx"
#include"MFront/UMATInterfaceBase.hxx"

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
  struct UMATInterface
    : public UMATInterfaceBase,
      protected InterfaceBase
  {
    
    /*!
     * This enum defines the various finite strain strategies
     * available to resuse HPP laws into finite strain computations.
     *
     * The "finite rotation small strain" strategy is defined in the
     * reference of the Code-Aster finite element software:
     * [R5.03.22] Loi de comportement en grandes rotations et petites déformations
     *
     * The logarithmic strain strategy has been introduced by Miehe,
     * Apel and Lambrecht:
     * Miehe C., Apel N., Lambrecht M.: Anisotropic additive plasticity in the logarithm strain space : modular
     * kinematic formulation and implementation based on incremental minimization principles for
     * standard materials., Computer Methods in Applied Mechanics and Engineering, 191,
     * pp.5383-5425, 2002.
     * This strategy is also developped int the reference of the Code-Aster finite element software:
     * [R5.03.24] Modèles de grandes déformations GDEF_LOG et GDEF_HYPO_ELAS
     *
     * The 1D logarithmic strain strategy has been designed for fuel
     * performance code written in small deformation. The handling of
     * finite strains is done within the behaviour.
     * See Helfer Thomas: Extension of monodimensional fuel
     * performance codes to finite strain analysis using a lagrangian
     * logarithmic strain framework. Proposed to Nuclear Engineering
     * And Design.
     */
    enum FiniteStrainStrategy{
      NONE,
      FINITEROTATIONSMALLSTRAIN,
      MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN,
      LOGARITHMICSTRAIN1D
    }; // end of enum FiniteStrainStrategy

    static std::string 
    getName(void);
    /*!
     * \return true if the interface shall support plane stress
     * hypothesis through a generic algorithm.
     * \return false if the behaviour handles the plane stress
     * hypothesis or if the behaviour don't handle the generalised
     * plane strain hypothesis.
     */
    static bool
    usesGenericPlaneStressAlgorithm(const BehaviourDescription&);
    /*!
     * \brief default constructor
     */
    UMATInterface();
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const BehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const BehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const BehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const BehaviourDescription&) const;
    /*!
     * \return a map associating to each library a list of entry
     * points (function or classes)
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedEntryPoints(const BehaviourDescription&) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ofstream&,
				   const BehaviourDescription&) const;
    
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) override;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatement(const BehaviourDescription&,
		  const FileDescription&) const;

    virtual void reset(void) override;

    virtual ~UMATInterface();
    
  protected:

    /*!
     * \return a pair. If first is true, the "axial strain" was found
     * and the second contains its offset
     * \param[in] mb : material description
     */
    std::pair<bool,SupportedTypes::TypeSize>
    checkIfAxialStrainIsDefinedAndGetItsOffset(const BehaviourDescription&) const;

    virtual std::string
    getLibraryName(const BehaviourDescription&) const;

    static std::string
    treatScalar(const std::string&);

    static std::string
    treatScalar(const std::string&,
		const unsigned short);

    static std::string
    treatStensor(const Hypothesis,
		 const std::string&);

    static std::string
    treatStensor(const Hypothesis,
		 const std::string&,
		 const unsigned short);

    static std::string
    treatTensor(const Hypothesis,
		const std::string&);

    static std::string
    treatTensor(const Hypothesis,
		const std::string&,
		const unsigned short);

    static unsigned short
    getStensorSize(const Hypothesis);

    virtual std::string
    getInterfaceName(void) const;

    /*!
     * \return the name of the function generated by the Cyrano interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string
    getFunctionName(const std::string&) const;

    std::string
    getBehaviourName(const BehaviourDescription&) const;

    virtual std::string
    getUmatFunctionName(const BehaviourDescription&) const;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : mechanical behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
		      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription&,
				const Hypothesis) const;

    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxBehaviourTypeSymbols(std::ostream&,
				    const std::string&,
				    const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     * \param[in] h    : modelling hypothesis
     */
    virtual void
    writeUMATxxAdditionalSymbols(std::ostream&,
				 const std::string&,
				 const Hypothesis,
				 const BehaviourDescription&,
				 const FileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxSpecificSymbols(std::ostream&,
			       const std::string&,
			       const BehaviourDescription&,
			       const FileDescription&) const;

    virtual void
    generateGibianeDeclaration(const BehaviourDescription&,
			       const FileDescription&) const;

    virtual void
    writeVariableDescriptionContainerToGibiane(std::ostream&,
					       const Hypothesis,
					       const VariableDescriptionContainer&) const;

    virtual void
    writeGibianeInstruction(std::ostream&,
			    const std::string&) const;

    virtual void
    writeUmatFortranFunctionDefine(std::ostream&,
				   const std::string&) const;

    virtual void
    writeUmatFunctionDeclaration(std::ostream&,
				 const std::string&) const;
    /*!
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void
    writeFiniteRotationSmallStrainUmatFunction(std::ostream&,
					       const std::string&,
					       const std::string&,
					       const BehaviourDescription&) const;
    /*!
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void
    writeMieheApelLambrechtLogarithmicStrainUmatFunction(std::ostream&,
							 const std::string&,
							 const std::string&,
							 const BehaviourDescription&) const;
    /*!
     * \brief write the implementation of the umat function
     * corresponding to the LOGARITHMICSTRAIN1D finite strain strategy
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void
    writeLogarithmicStrain1DUmatFunction(std::ostream&,
					 const std::string&,
					 const std::string&,
					 const BehaviourDescription&) const;
    /*!
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void
    writeStandardUmatFunction(std::ostream&,
			      const std::string&,
			      const std::string&,
			      const BehaviourDescription&) const;
    /*!
     * \brief write a  specialisation of the CyranoTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void
    writeUMATBehaviourTraits(std::ostream&,
			     const BehaviourDescription&,
			     const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \brief plane stress handling requires to have access to the
     * axial strain, but it is not obvious nor always possible to get it...
     * This method handles all the cases.
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] c   : beginning of the call
     * \param[in] c2  : code computing the axial deformation gradient from the axial strain
     */    
    virtual void
    writeFiniteStrainStrategiesPlaneStressSpecificCall(std::ostream&,
						       const BehaviourDescription&,
						       const std::string&,
						       const std::string&) const;

    virtual std::string
    getModellingHypothesisTest(const Hypothesis) const;

    virtual std::map<UMATInterfaceBase::Hypothesis,std::string>
    gatherModellingHypothesesAndTests(const BehaviourDescription& mb) const;

    virtual std::set<tfel::material::ModellingHypothesis::Hypothesis>
    getModellingHypothesesToBeTreated(const BehaviourDescription&) const;

    std::vector<FiniteStrainStrategy> finiteStrainStrategies;

    bool useTimeSubStepping;

    bool doSubSteppingOnInvalidResults;

    unsigned short maximumSubStepping;

  }; // end of UMATInterface

} // end of namespace mfront

#endif /* LIB_MFRONTUMATINTERFACE_H_ */
