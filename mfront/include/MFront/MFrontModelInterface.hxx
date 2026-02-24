/*!
 * \file   MFrontModelInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTMODELINTERFACE_HXX
#define LIB_MFRONT_MFRONTMODELINTERFACE_HXX

#include "MFront/AbstractModelInterface.hxx"

namespace mfront {

  /*!
   * \brief
   */
  struct MFrontModelInterface final : public AbstractModelInterface {
    //! \return the name of the interface
    static std::string getName();
    //! constructor
    MFrontModelInterface();
    /*!
     * \param[in]     k:  key being treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in,out] p:  iterator to the token after the key being treated
     * \param[out]    pe: iterator past the end of the file
     * \return a pair. The first member of the pair is true if the
     * keyword was handled by interface. The second member of the pair
     * is an iterator after the last token used to treat the keyword.
     * pair.
     */
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \param[out] names: reserved names
     */
    void declareReservedNames(std::set<std::string>&) override;
    /*!
     * \brief write the output files for the given model description
     * \param[in] fd: file description
     * \param[in] md: model description
     */
    void writeOutputFiles(const FileDescription&,
                          const ModelDescription&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] td: target description
     * \param[in]  md: model description
     */
    void getTargetsDescription(TargetsDescription&,
                               const ModelDescription&) override;
    //! destructor
    ~MFrontModelInterface() override;
  };  // end of struct MFrontModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTMODELINTERFACE_HXX */
