/*!
 * \file   mfront/include/MFront/FortranMaterialPropertyInterface.hxx
 * \brief  This file declares the FortranMaterialPropertyInterface class
 * \author Helfer Thomas
 * \date   1 décembre 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTFORTRANLAWINTERFACE_H_
#define LIB_MFRONTFORTRANLAWINTERFACE_H_ 

#include"MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront{

  struct FortranMaterialPropertyInterface
    : public CMaterialPropertyInterfaceBase
  {
    static std::string 
    getName(void);
    
    FortranMaterialPropertyInterface();
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const MaterialPropertyDescription&) override;
    //! destructor
    virtual ~FortranMaterialPropertyInterface();
        
  private:

    void
    writeParameterList(std::ostream&,
		       const VariableDescriptionContainer&);

    void
    writeInterfaceSpecificVariables(const VariableDescriptionContainer&);

    void
    writeHeaderPreprocessorDirectives(const std::string&,
				      const std::string&);

    void
    writeSrcPreprocessorDirectives(const std::string&,
				   const std::string&);

    void
    writeBeginHeaderNamespace(void);

    void
    writeEndHeaderNamespace(void);

    void
    writeBeginSrcNamespace(void);

    void
    writeEndSrcNamespace(void);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    std::string
    getHeaderFileName(const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    std::string
    getSrcFileName(const std::string&,
		   const std::string&);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    std::string
    getFunctionDeclaration(const std::string&,
			   const std::string&);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    std::string
    getCheckBoundsFunctionDeclaration(const std::string&,
				      const std::string&);

    bool
    requiresCheckBoundsFunction(void) const;

  }; // end of MfrontFortranMaterialPropertyInterface

} // end of namespace mfront

#endif /* LIB_MFRONTFORTRANLAWINTERFACE_H_ */

