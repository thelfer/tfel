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
    ~PythonMaterialPropertyInterface() override;

  };  // end of PythonMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_PYTHONMATERIALPROPERTYINTERFACE_HXX */
