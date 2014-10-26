/*!
 * \file   mfront/include/MFront/MFrontMaterialPropertyInterface.hxx
 * \brief  This file declares the MFrontMaterialPropertyInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTLAWINTERFACE_H_
#define _LIB_MFRONT_MFRONTLAWINTERFACE_H_ 

#include"MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront{

  struct MFrontMaterialPropertyInterface
    : public CMaterialPropertyInterfaceBase
  {
    static std::string 
    getName(void);
    
    MFrontMaterialPropertyInterface();

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

    /*!
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     */
    std::string
    getHeaderFileName(const std::string&,
		      const std::string&);

    /*!
     * \param[in] m : name of the material
     * \param[in] l : name of the law
     */
    std::string
    getFunctionName(const std::string&,
		    const std::string&) const;
    /*!
     * destructor
     */
    ~MFrontMaterialPropertyInterface();
        
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

  }; // end of MFrontMaterialPropertyInterface

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTLAWINTERFACE_H */

