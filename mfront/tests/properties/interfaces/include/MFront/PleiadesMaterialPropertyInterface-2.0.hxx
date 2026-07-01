/*
 * \brief PleiadesMaterialPropertyInterface
 *
 * \file PleiadesMaterialPropertyInterface.hh
 *
 *
 * \author: sb152252
 * \date 26 oct. 2017
 *
 * Copyright © CEA 2017
 */

#ifndef LIB_MFRONTPLEIADESLAWINTERFACE_H
#define LIB_MFRONTPLEIADESLAWINTERFACE_H

#include <string>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  struct PleiadesMaterialPropertyInterface
      : public AbstractMaterialPropertyInterface {
    PleiadesMaterialPropertyInterface() noexcept;

    using const_iterator =
        tfel::utilities::CxxTokenizer::TokensContainer::const_iterator;

    static std::string getName();
    //
    void setOptions(const DataMap&) override;
    std::pair<bool, const_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        const_iterator,
        const const_iterator) override;
    void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const override;
    void writeOutputFiles(const MaterialPropertyDescription& mpd,
                          const FileDescription& fd) const override;
    //! \brief destructor
    ~PleiadesMaterialPropertyInterface() override;

   private:
    /*!
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    void writeHeaderFile(const MaterialPropertyDescription& mpd,
                         const FileDescription& fd) const;

    /*!
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    void writeSrcFile(const MaterialPropertyDescription& mpd,
                      const FileDescription& fd) const;

  };  // end of PleiadesMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTPLEIADESLAWINTERFACE_H */
