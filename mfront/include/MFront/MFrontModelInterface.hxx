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

  //! \brief interface used internally by MFront
  struct MFrontModelInterface final : public AbstractModelInterface {
    //! \return the name of the interface
    static std::string getName();
    //! \brief constructor
    MFrontModelInterface();
    //
    void setOptions(const DataMap &) override;
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void declareReservedNames(std::set<std::string>&) override;
    void writeOutputFiles(const FileDescription&,
                          const ModelDescription&) override;
    void getTargetsDescription(TargetsDescription&,
                               const ModelDescription&) override;
    //! \brief destructor
    ~MFrontModelInterface() override;
  };  // end of struct MFrontModelInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTMODELINTERFACE_HXX */
