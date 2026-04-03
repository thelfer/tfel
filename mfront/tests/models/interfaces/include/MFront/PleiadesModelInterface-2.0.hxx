/*!
 * \file
 * mfront/tests/models/interfaces/include/MFront/PleiadesModelInterface-2.0.hxx
 * \brief  This file declares the PleiadesModelInterface class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_PLEIADESMODELINTERFACE_HXX
#define LIB_MFRONT_PLEIADESMODELINTERFACE_HXX

#include "MFront/AbstractModelInterface.hxx"
#include "MFront/FileDescription.hxx"

namespace mfront {

  /*!
   * \brief Implements a interface for the `PLEIADES 2.0` kernel.
   */
  struct PleiadesModelInterface : public AbstractModelInterface {
    //! default constructor
    PleiadesModelInterface() noexcept;
    //! \return the name of the interface
    static std::string getName(void);

    std::pair<bool, TokensContainer::const_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        TokensContainer::const_iterator,
        const TokensContainer::const_iterator) override;
    /*! declare reserved names. Those are variables used internally
     * that may conflicts with user variables
     * \param[in] n : reserved names
     */
    void declareReservedNames(std::set<std::string>&) override;
    /*!
     * \brief write the output files
     * \param pdata : processing data
     * \param data  : PleiadesModel data
     */
    void writeOutputFiles(const FileDescription&,
                          const ModelDescription&) override;

    void getTargetsDescription(TargetsDescription&,
                               const ModelDescription&) override;
    //! destructor
    ~PleiadesModelInterface() override;

  };  // end of class PleiadesModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTPLEIADESMODELINTERFACE_HXX */
