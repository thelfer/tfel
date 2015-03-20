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
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const MaterialPropertyDescription&) override;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
				  const FileDescription&) override;
    //! destructor
    virtual ~GnuplotMaterialPropertyInterface();
    
  private:

    std::string
    eraseQuote(const std::string&);
    
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
