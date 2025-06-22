/*!
 * \file   mfront/include/MFront/CyranoMaterialPropertyInterface.hxx
 * \brief  This file declares the CyranoMaterialPropertyInterface class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANOMATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_CYRANOMATERIALPROPERTYINTERFACE_HXX

#include <string>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  // forward declaration
  struct LibraryDescription;

  /*!
   * \brief material properties' interface for EDF' `Cyrano` fuel
   * performance code.
   */
  struct CyranoMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    //! \return the interface name
    static std::string getName();
    //! \brief default constructor
    CyranoMaterialPropertyInterface();
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
    virtual std::string getCyranoFunctionName(
        const MaterialPropertyDescription&) const;
    //! destructor
    ~CyranoMaterialPropertyInterface() override;

   protected:
    /*!
     * \brief return the name of the header file
     */
    virtual std::string getHeaderFileName(const std::string&) const;
    /*!
     * \brief return the name of the source file
     */
    virtual std::string getSourceFileName(const std::string&) const;

   private:
    /*!
     * \brief generate the output files
     * \param[in] mpd: material property description
     * \param[in] fd: mfront file description
     */
    virtual void writeHeaderFile(const MaterialPropertyDescription&,
                                 const FileDescription&) const;
    /*!
     * \brief generate the output files
     * \param[in] mpd: material property description
     * \param[in] fd:  mfront file description
     */
    virtual void writeSrcFile(const MaterialPropertyDescription&,
                              const FileDescription&) const;
  };  // end of MfrontCyranoMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CYRANOMATERIALPROPERTYINTERFACE_HXX */
