/*!
 * \file   mfront/include/MFront/CppMaterialPropertyInterface.hxx
 * \brief  This file declares the CppMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTCPPLAWINTERFACE_HXX
#define LIB_MFRONTCPPLAWINTERFACE_HXX

#include <string>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  /*!
   * \brief Interface for C++ material properties
   */
  struct CppMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    static std::string getName();
    //! \brief default constructor
    CppMaterialPropertyInterface();
    //
    void setOptions(const DataMap &) override;
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
    ~CppMaterialPropertyInterface() override;

   private:
    /*!
     * \brief generate the header file.
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeHeaderFile(const MaterialPropertyDescription&,
                                 const FileDescription&) const;
    /*!
     * \brief generate the source file.
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeSrcFile(const MaterialPropertyDescription&,
                              const FileDescription&) const;

  };  // end of MfrontCppMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTCPPLAWINTERFACE_HXX */
