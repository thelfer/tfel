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

#ifndef _LIB_MFRONTMODELINTERFACE_H_
#define _LIB_MFRONTMODELINTERFACE_H_ 

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
			  const ModelData&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d  : target description
     * \param[in]  md : model description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const ModelData&) override;

  private:

    void
    openOutputFiles(void);
    
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    generateOutputFiles(const FileDescription&,
			const ModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeHeaderFile(const FileDescription&,
		    const ModelData&);

    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    void
    writeSrcFile(const FileDescription&,
		 const ModelData&);
    
    void
    writeInitializeIntputsVariablesInitialValues(const ModelData&);

    void
    writeInitializeConstantMaterialProperties(const ModelData&);

    void
    writeInitializeInputsVariablesDepths(const ModelData&);

    void
    writeInitializeOutputsVariablesInitialValues(const ModelData&);

    void
    writeInitializeOutputsVariablesDepths(const ModelData&);

    void
    writeStaticVariableInitialization(const FileDescription&,
				      const ModelData&,
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
		    const ModelData&) const;

  private:

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
  }; // end of class MFrontModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTMODELINTERFACE_H */

