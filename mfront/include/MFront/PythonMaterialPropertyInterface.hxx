/*!
 * \file   mfront/include/MFront/PythonMaterialPropertyInterface.hxx
 * \brief  This file declares the PythonMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_PYTHONMATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_PYTHONMATERIALPROPERTYINTERFACE_HXX

#include <string>
#include <fstream>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct PythonMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    static std::string getName();

    PythonMaterialPropertyInterface();
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
    ~PythonMaterialPropertyInterface() override;

  };  // end of PythonMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_PYTHONMATERIALPROPERTYINTERFACE_HXX */
