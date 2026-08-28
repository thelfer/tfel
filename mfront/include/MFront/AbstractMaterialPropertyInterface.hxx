/*!
 * \file   mfront/include/MFront/AbstractMaterialPropertyInterface.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   16 jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABSTRACTMATERIALPROPERTYINTERFACE_HXX
#define LIB_MFRONT_ABSTRACTMATERIALPROPERTYINTERFACE_HXX

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

namespace mfront {

  // forward declarations
  struct MaterialPropertyDescription;
  struct FileDescription;
  struct TargetsDescription;

  /*!
   * \brief abstract interface for material property interface.
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractMaterialPropertyInterface {
    //! \brief a simple alias
    using DataMap = tfel::utilities::DataMap;
    //! \brief a simple alias
    using tokens_iterator =
        tfel::utilities::CxxTokenizer::TokensContainer::const_iterator;
    /*!
     * \brief give options to the interface
     *
     * Those options may have been read from a configuration file or from the
     * command line
     *
     * \param[in] opts: options
     */
    virtual void setOptions(const DataMap&) = 0;
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
     * \param[in] mpd: material property description
     * \param[in] fd:  mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
                                  const FileDescription&) const = 0;
    /*!
     * \brief : fill the target descripton
     * \param[out] d:   target description
     * \param[in]  mpd: material property description
     */
    virtual void getTargetsDescription(
        TargetsDescription&, const MaterialPropertyDescription&) const = 0;
    //! \brief destructor
    virtual ~AbstractMaterialPropertyInterface();
  };  // end of AbstractMaterialPropertyInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTMATERIALPROPERTYINTERFACE_HXX */
