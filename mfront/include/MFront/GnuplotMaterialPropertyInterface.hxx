/*!
 * \file   mfront/include/MFront/GnuplotMaterialPropertyInterface.hxx
 * \brief  This file declares the GnuplotMaterialPropertyInterface class
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTGNUPLOTLAWINTERFACE_H_
#define LIB_MFRONTGNUPLOTLAWINTERFACE_H_ 

#include<map>
#include<vector>
#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront{

  struct GnuplotMaterialPropertyInterface
    : public AbstractMaterialPropertyInterface
  {
    static std::string 
    getName(void);
    
    GnuplotMaterialPropertyInterface();
    
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
    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, name of the output library
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const std::string&, includes
     * \param const std::string&, output name
     * \param const VariableDescriptionContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const StaticVariableDescriptionContainer&, static variables
     * \param const std::map<std::string,std::string>&, glossary names,
     * \param const std::map<std::string,std::string>&, entry names,
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     * \param const bool, use template
     * \param const std::vector<std::string>&, namespaces
     */
    void writeOutputFiles(const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const VariableDescriptionContainer&,
			  const std::vector<std::string>&,
			  const std::map<std::string,std::string>&,
			  const std::map<std::string,std::string>&,
			  const StaticVariableDescriptionContainer&,
			  const std::vector<std::string>&,
			  const std::map<std::string,double>&,
			  const LawFunction&,
			  const std::vector<VariableBoundsDescription>&,
			  const std::vector<VariableBoundsDescription>&,
			  const bool,
			  const std::vector<std::string>&) override;
    
    
    virtual void reset(void) override;

    virtual ~GnuplotMaterialPropertyInterface() override;
    
  private:

    std::string
    eraseQuote(const std::string&);

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    
    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    registerTestBounds(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
			const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    registerGraph(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator
    nextToken(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
	      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
	      const std::string &msg="");

    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const VariableDescriptionContainer&, inputs
     * \param const StaticVariableDescriptionContainer&, static variables
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     * \param const bool, use template
     * \param const std::vector<std::string>&, namespaces
     */
    void writeSrcFile(const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const VariableDescriptionContainer&,
		      const StaticVariableDescriptionContainer&,
		      const std::vector<std::string>&,
		      const std::map<std::string,double>&,
		      const LawFunction&,
		      const std::vector<VariableBoundsDescription>&,
		      const std::vector<VariableBoundsDescription>&,
		      const bool useTemplate,
		      const std::vector<std::string>& namespaces);

    std::ofstream srcFile;

    std::string srcFileName;
    
    std::vector<VariableBoundsDescription> testBounds ;

    std::string xLabel ;
    std::string yLabel ;
    //map : variable,datafile
    struct dataFile {
      std::string name;
      std::string legend;
    };
    std::multimap< std::string, dataFile > experimentalData ;
    
  }; // end of MfrontGnuplotMaterialPropertyInterface

} // end of namespace mfront

#endif /* LIB_MFRONTGNUPLOTLAWINTERFACE_H_ */
