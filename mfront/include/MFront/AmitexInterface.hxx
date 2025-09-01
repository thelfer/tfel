/*!
 * \file   mfront/include/MFront/AmitexInterface.hxx
 * \brief  This file declares the AmitexInterface class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_AMITEX_FFTP_INTERFACE_HXX
#define LIB_MFRONT_AMITEX_FFTP_INTERFACE_HXX

#include "MFront/CastemInterface.hxx"

namespace mfront {

  /*!
   * \brief Interface for behaviours for use in the `AMITEX_FFTP` solver based
   * on Fast-Fourier transform.
   */
  struct AmitexInterface : public CastemInterface {
    //! \brief return the name of the interface
    static std::string getName();
    //! \brief default constructor
    AmitexInterface();
    //! \brief destructor
    ~AmitexInterface() override;

   protected:
    void generateInputFileExample(const BehaviourDescription &,
                                  const FileDescription &) const override;
    void generateInputFileExampleForHypothesis(std::ostream &,
                                               const BehaviourDescription &,
                                               const Hypothesis) const override;
  };  // end of struct AmitexInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_AMITEX_FFTP_INTERFACE_HXX */
