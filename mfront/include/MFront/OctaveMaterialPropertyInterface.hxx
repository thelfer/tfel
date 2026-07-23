/*!
 * \file   OctaveMaterialPropertyInterface.hxx
 * \brief  This file declares the OctaveMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTOCTAVELAWINTERFACE_HXX
#define LIB_MFRONTOCTAVELAWINTERFACE_HXX

#include <string>
#include <fstream>
#include <vector>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/LawFunction.hxx"
#include "MFront/InterfaceBase.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct OctaveMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    static std::string getName();

    OctaveMaterialPropertyInterface();
    //
    void setOptions(const DataMap&) override;
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    //! \brief destructor
    ~OctaveMaterialPropertyInterface() override;

   private:
    static std::string toString(const unsigned short);

    static void replace(std::string&,
                        const std::string::value_type,
                        const std::string::value_type);

    std::string treatDescriptionString(const std::string&) const;

  };  // end of MfrontOctaveLawInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTOCTAVELAWINTERFACE_H */
