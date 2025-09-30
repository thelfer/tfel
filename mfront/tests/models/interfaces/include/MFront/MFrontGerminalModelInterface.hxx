/*!
 * \file
 * mfront/tests/models/interfaces/include/MFront/MFrontGerminalModelInterface.hxx
 * \brief  This file declares the MFrontGerminalModelInterface class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTGERMINALMODELINTERFACE_HXX
#define LIB_MFRONTGERMINALMODELINTERFACE_HXX

#include "MFront/MFrontPleiadesModelInterfaceBase.hxx"

namespace mfront {

  struct MFrontGerminalModelInterface final
      : public MFrontPleiadesModelInterfaceBase {
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
    virtual std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;

   private:
    virtual void setOutputFileNames(const ModelDescription&) override;

    virtual void writeGetConstantMaterialProperty(
        const VarHandler&, const ModelDescription&) override;

    virtual std::string getApplicationName() const override;

    virtual void writeInitializeMethod(const ModelDescription&) override;

    virtual void writeSpecificPrivateMethodDeclaration(
        const FileDescription&, const ModelDescription&) override;

    virtual void generateDomainsList(const ModelDescription&) override;

    virtual void buildDomainName() override;

    virtual bool initializeDefaultDomainListInConstrutor(
        const ModelDescription&) const override;

    virtual void writeInitializeParametersMethod(
        const ModelDescription&) override;

  };  // end of class MFrontGerminalModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTGERMINALMODELINTERFACE_HXX */
