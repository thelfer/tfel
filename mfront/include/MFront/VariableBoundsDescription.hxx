/*!
 * \file   mfront/include/MFront/VariableBoundsDescription.hxx
 * \brief  This file declares the VariableBoundsDescription class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_VARIABLEBOUNDSDESCRIPTION_H_
#define _LIB_VARIABLEBOUNDSDESCRIPTION_H_ 

#include<string>
#include"TFEL/Config/TFELConfig.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT VariableBoundsDescription
  {
    enum BoundsType{Lower,Upper,LowerAndUpper};
    VariableBoundsDescription() = default;
    VariableBoundsDescription(VariableBoundsDescription&&) = default;
    VariableBoundsDescription(const VariableBoundsDescription&) = default;
    VariableBoundsDescription& operator=(VariableBoundsDescription&&) = default;
    VariableBoundsDescription& operator=(const VariableBoundsDescription&) = default;
    ~VariableBoundsDescription() noexcept;
    BoundsType     boundsType;
    std::string    varName;
    unsigned short varNbr;
    unsigned int lineNumber;
    double lowerBound;
    double upperBound;
  }; // end of class VariableBoundsDescription

} // end of namespace mfront

#endif /* _LIB_VARIABLEBOUNDSDESCRIPTION_H */

