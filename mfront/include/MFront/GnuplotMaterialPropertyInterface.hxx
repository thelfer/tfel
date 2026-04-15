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

#ifndef LIB_MFRONTGNUPLOTLAWINTERFACE_HXX
#define LIB_MFRONTGNUPLOTLAWINTERFACE_HXX

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
    static std::string getName();

    GnuplotMaterialPropertyInterface();
    //
    void setOptions(const DataMap&) override;
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    //! \brief destructor
    ~GnuplotMaterialPropertyInterface() override;

   private:
    std::string eraseQuote(const std::string&);

    std::pair<bool, tokens_iterator> registerTestBounds(tokens_iterator,
                                                        const tokens_iterator);

    std::pair<bool, tokens_iterator> registerGraph(tokens_iterator,
                                                   const tokens_iterator);

    tokens_iterator nextToken(tokens_iterator,
                              tokens_iterator,
                              const std::string& msg = "");
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
    std::map<std::string, VariableBoundsDescription> registredTestBounds;
    std::multimap<std::string, dataFile> experimentalData;
    std::string xLabel;
    std::string yLabel;
  };  // end of MfrontGnuplotMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTGNUPLOTLAWINTERFACE_HXX */
