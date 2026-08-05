/*!
 * \file   mfront/include/MFront/AbstractModelInterface.hxx
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

#ifndef LIB_MFRONTMODELVIRTUALINTERFACE_HXX
#define LIB_MFRONTMODELVIRTUALINTERFACE_HXX

#include <map>
#include <set>
#include <string>
#include <vector>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/ModelDescription.hxx"

namespace mfront {

  // forward declarations
  struct FileDescription;
  struct TargetsDescription;
  struct ModelDescription;

  struct MFRONT_VISIBILITY_EXPORT AbstractModelInterface {
    //! \brief a simple alias
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    //! \brief a simple alias
    using TokensContainer = CxxTokenizer::TokensContainer;
    //! \brief a simple alias
    using tokens_iterator = CxxTokenizer::TokensContainer::const_iterator;
    //! \brief a simple alias
    using DataMap = tfel::utilities::DataMap;
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
     * \param[out] names: reserved names
     */
    virtual void declareReservedNames(std::set<std::string>&) = 0;
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
        const tokens_iterator) = 0;
    /*!
     * \brief : fill the target descripton
     * \param[out] td: target description
     * \param[in]  md: model description
     */
    virtual void getTargetsDescription(TargetsDescription&,
                                       const ModelDescription&) = 0;
    /*!
     * \param fd: file description
     * \param md: model description
     */
    virtual void writeOutputFiles(const FileDescription&,
                                  const ModelDescription&) = 0;
    //! desctructor
    virtual ~AbstractModelInterface();
  };  // end of AbstractModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTMODELVIRTUALINTERFACE_HXX */
