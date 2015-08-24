/*!
 * \file   mfront/include/MFront/CMaterialPropertyInterfaceBase.hxx
 * \brief  This file declares the CMaterialPropertyInterfaceBase class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTCLAWINTERFACEBASE_H_
#define LIB_MFRONTCLAWINTERFACEBASE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront{

  struct CMaterialPropertyInterfaceBase
    : public AbstractMaterialPropertyInterface
  {
    
    CMaterialPropertyInterfaceBase();
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
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
				  const FileDescription&) override;
    //! destructor
    virtual ~CMaterialPropertyInterfaceBase();
    
  protected:

    virtual void
    writeParameterList(std::ostream&,
		       const VariableDescriptionContainer&);

    virtual void
    writeInterfaceSpecificVariables(const VariableDescriptionContainer&);

    virtual void
    writeBeginHeaderNamespace(void) = 0;

    virtual void
    writeEndHeaderNamespace(void) = 0;

    virtual void
    writeBeginSrcNamespace(void) = 0;

    virtual void
    writeEndSrcNamespace(void) = 0;

    virtual void
    writeHeaderPreprocessorDirectives(const MaterialPropertyDescription&);

    virtual void
    writeSrcPreprocessorDirectives(const MaterialPropertyDescription&);

    virtual std::string
    getCallingConvention(void) const;
    
    virtual bool
    requiresCheckBoundsFunction(void) const = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getFunctionName(const std::string&,
		    const std::string&) const = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getCheckBoundsFunctionName(const std::string&,
			       const std::string&) const = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getHeaderFileName(const std::string&,
		      const std::string&) const = 0;

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getSrcFileName(const std::string&,
		   const std::string&) const = 0;

    /*
     */
    virtual void
    writeHeaderFile(const MaterialPropertyDescription&,
		    const FileDescription&);
    
    /*
     */
    void writeSrcFile(const MaterialPropertyDescription&,
		      const FileDescription&);

    std::ofstream headerFile;

    std::ofstream srcFile;

    std::string headerFileName;

    std::string srcFileName;
    
  }; // end of MfrontCMaterialPropertyInterfaceBase

} // end of namespace mfront

#endif /* LIB_MFRONTCLAWINTERFACEBASE_H_ */

