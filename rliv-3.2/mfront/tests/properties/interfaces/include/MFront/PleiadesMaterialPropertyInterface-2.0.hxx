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
  : public AbstractMaterialPropertyInterface
{

  PleiadesMaterialPropertyInterface();

  using const_iterator = tfel::utilities::CxxTokenizer::TokensContainer::const_iterator;

  static std::string getName();

  /*!
   * \param[in] k:  keyword treated
   * \param[in] i:  list of interfaces to which the keyword is restricted
   * \param[in] p:  iterator to the current token
   * \param[in] pe: iterator past the end of the file
   * \return a pair. The first entry is true if the keyword was
   * treated by the interface. The second entry is an iterator after
   * the last token treated.
   */
  std::pair<bool, const_iterator>
  treatKeyword(const std::string&,
	       const std::vector<std::string>&,
	       const_iterator,
	       const const_iterator) override;
  /*!
   * \brief : fill the target descripton
   * \param[out] d   : target description
   * \param[in]  mpd : material property description
   */
  void getTargetsDescription(TargetsDescription&,
			     const MaterialPropertyDescription&) const override;

  /*!
   * \brief generate the output files
   * \param[in] mpd : material property description
   * \param[in] fd  : mfront file description
   */
  void writeOutputFiles(const MaterialPropertyDescription& mpd,
                        const FileDescription& fd) const override;

  ~PleiadesMaterialPropertyInterface() override;

 private:
  /*!
   * \param[in] mpd : material property description
   * \param[in] fd  : mfront file description
   */
  void writeHeaderFile(const MaterialPropertyDescription& mpd, const FileDescription& fd) const;

  /*!
   * \param[in] mpd : material property description
   * \param[in] fd  : mfront file description
   */
  void writeSrcFile(const MaterialPropertyDescription& mpd, const FileDescription& fd) const;

};  // end of MfrontPleiadesLawInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTPLEIADESLAWINTERFACE_H */
