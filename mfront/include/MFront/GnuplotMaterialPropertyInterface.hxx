/*!
 * \file   mfront/include/MFront/GnuplotMaterialPropertyInterface.hxx
 * \brief  This file declares the GnuplotMaterialPropertyInterface class
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTGNUPLOTLAWINTERFACE_H_
#define LIB_MFRONTGNUPLOTLAWINTERFACE_H_

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct GnuplotMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    static std::string getName(void);

    GnuplotMaterialPropertyInterface();
    /*!
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) override;
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
    std::string eraseQuote(const std::string&);

    std::pair<bool, tokens_iterator> registerTestBounds(tokens_iterator,
                                                        const tokens_iterator);

    std::pair<bool, tokens_iterator> registerGraph(tokens_iterator,
                                                   const tokens_iterator);

    tokens_iterator nextToken(tokens_iterator,
                              tokens_iterator,
                              const std::string& msg = "");

    std::ofstream srcFile;

    std::string srcFileName;

    std::vector<VariableBoundsDescription> testBounds;

    std::string xLabel;
    std::string yLabel;
    // map : variable,datafile
    struct dataFile {
      //! constructor
      dataFile();
      //! copy constructor
      dataFile(const dataFile&);
      //! move constructor
      dataFile(dataFile&&);
      //! copy assignement operator
      dataFile& operator=(const dataFile&);
      //! move assignement operator
      dataFile& operator=(dataFile&&);
      //! destructor
      ~dataFile();
      std::string name;
      std::string legend;
    };
    std::multimap<std::string, dataFile> experimentalData;

  };  // end of MfrontGnuplotMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTGNUPLOTLAWINTERFACE_H_ */
