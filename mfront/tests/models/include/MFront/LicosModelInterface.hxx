/*!
 * \file   mfront/tests/models/include/MFront/LicosModelInterface.hxx
 * \brief  This file declares the MFrontModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTMODELINTERFACE_H_
#define LIB_MFRONTMODELINTERFACE_H_ 

#include<fstream>
#include"MFront/AbstractModelInterface.hxx"

namespace mfront{

  struct MFrontModelInterface final
    : public AbstractModelInterface
  {
    static std::string 
    getName(void);

    MFrontModelInterface();

    virtual ~MFrontModelInterface();

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) override;

    virtual void
    declareReservedNames(std::set<std::string>&) override;

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual
    void writeOutputFiles(const FileDescription&,
			  const ModelDescription&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d  : target description
     * \param[in]  md : model description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const ModelDescription&) override;

  private:

    void
    openOutputFiles(void);
    
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    generateOutputFiles(const FileDescription&,
			const ModelDescription&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeHeaderFile(const FileDescription&,
		    const ModelDescription&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeSrcFile(const FileDescription&,
		 const ModelDescription&);
    
    void
    writeInitializeIntputsVariablesInitialValues(const ModelDescription&);

    void
    writeInitializeConstantMaterialProperties(const ModelDescription&);

    void
    writeInitializeInputsVariablesDepths(const ModelDescription&);

    void
    writeInitializeOutputsVariablesInitialValues(const ModelDescription&);

    void
    writeInitializeOutputsVariablesDepths(const ModelDescription&);

    void
    writeStaticVariableInitialization(const FileDescription&,
				      const ModelDescription&,
				      const StaticVariableDescription&);

    void
    writeAssignDefaultValue(const VariableDescriptionContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    void
    closeOutputFiles(void);

    static VariableDescriptionContainer::const_iterator
    findVariableDescription(const VarContainer&,
			    const std::string&);

    std::string
    getGenTypeGetMethod(const std::string&) const;

    std::string
    getGenTypeIsMethod(const std::string&) const;

    std::string
    getVariableName(const std::string&,
		    const ModelDescription&) const;

  private:

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
  }; // end of class MFrontModelInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTMODELINTERFACE_H */

