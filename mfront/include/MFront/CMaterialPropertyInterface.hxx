/*!
 * \file   mfront/include/MFront/CMaterialPropertyInterface.hxx
 * \brief  This file declares the CMaterialPropertyInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_CMATERIALPROPERTYINTERFACE_H_
#define LIB_CMATERIALPROPERTYINTERFACE_H_ 

#include"MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront{

  struct CMaterialPropertyInterface
    : public CMaterialPropertyInterfaceBase
  {
    static std::string 
    getName(void);
    
    CMaterialPropertyInterface();
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const MaterialPropertyDescription&) override;
    //! destructor
    virtual ~CMaterialPropertyInterface();
        
  private:

    /*!
     * \return the name of the generated library
     * \param[in] l: library name (given by the `@Library` keyword)
     * \param[in] m: material name (given by the `@Material` keyword)
     */
    virtual std::string
    getGeneratedLibraryName(const std::string&,
			    const std::string&) const;

    virtual void
    writeHeaderPreprocessorDirectives(const std::string&,
				      const std::string&);

    virtual void
    writeSrcPreprocessorDirectives(const std::string&,
				   const std::string&);

    virtual void
    writeBeginHeaderNamespace(void);

    virtual void
    writeEndHeaderNamespace(void);

    virtual void
    writeBeginSrcNamespace(void);

    virtual void
    writeEndSrcNamespace(void);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getHeaderFileName(const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getSrcFileName(const std::string&,
		   const std::string&);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getFunctionDeclaration(const std::string&,
			   const std::string&);

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    virtual std::string
    getCheckBoundsFunctionDeclaration(const std::string&,
				      const std::string&);

    virtual bool
    requiresCheckBoundsFunction(void) const;


  }; // end of CMaterialPropertyInterface

} // end of namespace mfront

#endif /* LIB_CMATERIALPROPERTYINTERFACE_H_ */

