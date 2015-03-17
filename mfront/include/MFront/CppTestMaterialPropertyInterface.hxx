/*!
 * \file   mfront/include/MFront/CppTestMaterialPropertyInterface.hxx
 * \brief  This file declares the CppTestMaterialPropertyInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTCPPTESTLAWINTERFACE_H_
#define LIB_MFRONTCPPTESTLAWINTERFACE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront{

  struct CppTestMaterialPropertyInterface
    : public AbstractMaterialPropertyInterface
  {
    static std::string 
    getName(void);
    
    CppTestMaterialPropertyInterface();
    
    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&) override;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&) override;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&) override;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&) override;

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&) override;

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
		       const std::vector<std::string>&) override;
    /*!
     * \return a map associating to each library a list of entry
     * points (function or classes)
     * \param[in] library   : libary name
     * \param[in] material  : material name
     * \param[in] className : className
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedEntryPoints(const std::string&,
			    const std::string&,
			    const std::string&) override;

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
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
    virtual ~CppTestMaterialPropertyInterface();
    
  private:

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    registerTestBounds(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
			const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator
    nextToken(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
	      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
	      const std::string &msg="");

    std::ofstream srcFile;

    std::string srcFileName;
    
    std::vector<VariableBoundsDescription> testBounds ;

  }; // end of MfrontCppTestMaterialPropertyInterface

} // end of namespace mfront

#endif /* LIB_MFRONTCPPTESTLAWINTERFACE_H_ */

