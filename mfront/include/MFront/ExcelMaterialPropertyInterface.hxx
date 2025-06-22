/*!
 * \file   mfront/include/MFront/ExcelMaterialPropertyInterface.hxx
 * \brief  This file declares the ExcelMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTEXCELLAWINTERFACE_HXX
#define LIB_MFRONTEXCELLAWINTERFACE_HXX

#include <string>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct ExcelMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    static std::string getName();
    /*!
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    //! destructor
    virtual ~ExcelMaterialPropertyInterface() override;
  };  // end of MfrontExcelMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTEXCELLAWINTERFACE_HXX */
