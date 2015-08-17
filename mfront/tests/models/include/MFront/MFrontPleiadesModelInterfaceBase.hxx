/*!
 * \file   mfront/tests/models/include/MFront/MFrontPleiadesModelInterfaceBase.hxx
 * \brief  This file declares the MFrontPleiadesModelinterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTPLEIADESMODELINTERFACE_H_
#define LIB_MFRONTPLEIADESMODELINTERFACE_H_ 

#include<fstream>
#include"MFront/AbstractModelInterface.hxx"

namespace mfront{

  struct MFrontPleiadesModelInterfaceBase
    : public AbstractModelInterface
  {

    MFrontPleiadesModelInterfaceBase();

    virtual void declareReservedNames(std::set<std::string>&)  override;
    /*!
     * \param[in] k  : keyword treated
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d  : target description
     * \param[in]  md : model description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const ModelData&) override;
    /*!
     * \param pdata : processing data
     * \param data  : model data
     */
    virtual void writeOutputFiles(const FileDescription&,
				  const ModelData&) override;
    
  protected:

    virtual void
    openOutputFiles(void);

    virtual void
    closeOutputFiles(void);

    virtual void
    generateOutputFiles(const FileDescription&,
			const ModelData&);
    
    virtual void
    writeHeaderFile(const FileDescription&,
		    const ModelData&);

    virtual void
    writeSpecificPrivateMethodDeclaration(const FileDescription&,
					  const ModelData&);
    
    virtual void
    writeSrcFile(const FileDescription&,
		 const ModelData&);

    virtual void
    writeAssignDefaultValue(const VarContainer::const_iterator,
			    const std::map<std::string,std::string>::const_iterator);

    virtual std::string
    getGenTypeMethod(const std::string& type) const;

    virtual void
    setOutputFileNames(const ModelData&) = 0;

    virtual void
    writeGetGlobalParameter(const VarHandler&,
			    const ModelData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const ModelData&);

    virtual void
    buildDomainName(void) = 0;

    virtual std::string
    getApplicationName(void) const = 0;

    virtual void
    generateDomainsList(const ModelData&);

    virtual void
    writeFunctionCall(const ModelData&,
		      const std::vector<ModelData::Function>&);

    virtual void
    writeInitializeMethod(const ModelData&);

    virtual void
    writeInitializeOutputMethod(const ModelData&);

    virtual void
    writeInitializeInputMethod(const ModelData&);

    virtual void
    writeInitializeParametersMethod(const ModelData&);

    virtual bool
    initializeDefaultDomainListInConstrutor(const ModelData&) const;

    std::set<std::string> domains;

    std::ofstream headerFile;
    std::ofstream srcFile;
    std::string headerFileName;
    std::string srcFileName;

    bool hasDefaultConstructor;
    bool hasSpecializedDomain;
  }; // end of class MFrontPleiadesModelInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTPLEIADESMODELINTERFACE_H */

