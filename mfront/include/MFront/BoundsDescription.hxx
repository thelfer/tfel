/*!
 * \file   mfront/include/MFront/BoundsDescription.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   05 mar 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BOUNDSDESCRIPTION_HXX_
#define LIB_MFRONT_BOUNDSDESCRIPTION_HXX_

#include <iosfwd>
#include <limits>

#include "MFront/MFrontConfig.hxx"
#include "MFront/SupportedTypes.hxx"

namespace mfront {

  /*!
   * a structure in charge of describing bounds
   */
  struct MFRONT_VISIBILITY_EXPORT BoundsDescription {
    enum Category { Physical, Standard };

    enum BoundsType { Lower, Upper, LowerAndUpper };

    enum VarCategory {
      MaterialProperty,
      LocalVariable,
      StateVariable,
      ExternalStateVariable,
      ExternalStateVariableII
    };

    BoundsDescription();
    BoundsDescription(const BoundsDescription&);
    BoundsDescription(BoundsDescription&&);
    BoundsDescription& operator=(const BoundsDescription&);
    BoundsDescription& operator=(BoundsDescription&&);
    ~BoundsDescription();

    void writeBoundsChecks(std::ostream&) const;

    // name of the variable on which the bounds acts
    std::string name;
    // component. If component is egal to -1, bounds acts of all
    // components
    short component = -1;
    // name of the variable and the component name
    std::string varName;

    SupportedTypes::TypeFlag varType = SupportedTypes::Scalar;
    Category category = Standard;
    VarCategory varCategory = LocalVariable;
    unsigned short arraySize = 1u;
    BoundsType boundsType = LowerAndUpper;
    long double lowerBound = std::numeric_limits<long double>::min();
    long double upperBound = std::numeric_limits<long double>::max();
    size_t lineNumber = 0u;

   protected:
    void writeBoundsChecks(std::ostream&, const std::string&) const;

  };  // end of class BoundsDescription

}  // end of namespace mfront

#endif /* LIB_MFRONT_BOUNDSDESCRIPTION_HXX_ */
