/*!
 * \file   mfront/include/MFront/CastemMaterialPropertyInterface.hxx
 * \brief  This file declares the CastemMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTCASTEMLAWINTERFACE_HXX
#define LIB_MFRONTCASTEMLAWINTERFACE_HXX

#include <string>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  // forward declaration
  struct LibraryDescription;

  struct MFRONT_VISIBILITY_EXPORT CastemMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    static std::string getName();

    CastemMaterialPropertyInterface();
    /*!
     * \brief append the generated material property to the given library
     * description. This method allows to choose to which library is attached
     * the material property (which is not the case of the
     * `getTargetsDescription` method).
     * \param[in] d: targets description
     * \param[in] l: library description
     * \param[in] mpd: material property description
     */
    virtual void getLibraryDescription(
        TargetsDescription& d,
        LibraryDescription&,
        const MaterialPropertyDescription&) const;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void writeOutputFiles(const MaterialPropertyDescription&,
                          const FileDescription&) const override;
    /*!
     * \return the name of the generated function
     * \param[in] mpd : material property description
     */
    virtual std::string getCastemFunctionName(
        const MaterialPropertyDescription&) const;
    //! destructor
    ~CastemMaterialPropertyInterface() override;

   protected:
    virtual std::string getHeaderFileName(const std::string&) const;

    virtual std::string getSourceFileName(const std::string&) const;

   private:
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeHeaderFile(const MaterialPropertyDescription&,
                                 const FileDescription&) const;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeSrcFile(const MaterialPropertyDescription&,
                              const FileDescription&) const;
  };  // end of MfrontCastemMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTCASTEMLAWINTERFACE_HXX */
