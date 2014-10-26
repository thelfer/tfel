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

#ifndef _LIB_MFRONTCLAWINTERFACE_H_
#define _LIB_MFRONTCLAWINTERFACE_H_ 

#include"MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront{

  struct CMaterialPropertyInterface
    : public CMaterialPropertyInterfaceBase
  {
    static std::string 
    getName(void);
    
    CMaterialPropertyInterface();

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     * \param const std::vector<std::string>&, library links
     */
    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(const std::string&,
		       const std::string&,
		       const std::string&,
		       const std::vector<std::string>&);

    ~CMaterialPropertyInterface();
        
  private:

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


  }; // end of MfrontCMaterialPropertyInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTCLAWINTERFACE_H */

