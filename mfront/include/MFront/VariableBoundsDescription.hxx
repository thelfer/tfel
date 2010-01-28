/*!
 * \file   VariableBoundsDescription.hxx
 * \brief  This file declares the VariableBoundsDescription class
 * \author Helfer Thomas
 * \date   06 mai 2008
 */

#ifndef _LIB_VARIABLEBOUNDSDESCRIPTION_H_
#define _LIB_VARIABLEBOUNDSDESCRIPTION_H_ 

namespace mfront
{

  struct VariableBoundsDescription
  {
    enum BoundsType{Lower,Upper,LowerAndUpper};
    BoundsType     boundsType;
    std::string    varName;
    unsigned short varNbr;
    unsigned short lineNumber;
    double lowerBound;
    double upperBound;
  }; // end of class VariableBoundsDescription

} // end of namespace mfront

#endif /* _LIB_VARIABLEBOUNDSDESCRIPTION_H */

