/*!
 * \file   mfront/include/MFront/AbstractMaterialPropertyInterface.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   16 jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABSTRACTMATERIALPROPERTYINTERFACE_HXX_
#define LIB_MFRONT_ABSTRACTMATERIALPROPERTYINTERFACE_HXX_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

namespace mfront {

  // forward declartion
  struct MaterialPropertyDescription;
  // forward declartion
  struct FileDescription;
  // forward declartion
  struct TargetsDescription;

  struct MFRONT_VISIBILITY_EXPORT AbstractMaterialPropertyInterface {
    //! a simple alias
    using tokens_iterator =
        tfel::utilities::CxxTokenizer::TokensContainer::const_iterator;
    /*!
     * \param[in] k  : keyword to be treated
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
        const tokens_iterator) = 0;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
                                  const FileDescription&) = 0;
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(TargetsDescription&,
                                       const MaterialPropertyDescription&) = 0;
    //! destructor
    virtual ~AbstractMaterialPropertyInterface();
  };  // end of AbstractMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTMATERIALPROPERTYINTERFACE_HXX_ */
