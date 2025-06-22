/*!
 * \file   mfront/include/MFront/LawFunction.hxx
 * \brief  This file declares the LawFunction class
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_LAWFUNCTION_HXX
#define LIB_LAWFUNCTION_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  //! \brief a simple structure wrapping the body of a material property
  struct MFRONT_VISIBILITY_EXPORT LawFunction {
    //! \brief default constructor
    LawFunction();
    //! \brief move constructor
    LawFunction(LawFunction&&);
    //! \brief copy constructor
    LawFunction(const LawFunction&);
    //! \brief move assignement
    LawFunction& operator=(LawFunction&&);
    //! \brief copy assignement
    LawFunction& operator=(const LawFunction&);
    //! \brief destructor
    ~LawFunction();
    //! \brief body of the function
    std::string body;
    //! \brief line a which the function is defined.
    unsigned int line = 0u;
    //! \brief boolean stating that the output is modified by the body.
    bool modified = false;
  };  // end of struct LawFunction

}  // end of namespace mfront

#endif /* LIB_LAWFUNCTION_HXX */
