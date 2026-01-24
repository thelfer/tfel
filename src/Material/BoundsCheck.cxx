/*!
 * \file  BoundsCheck.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 janv. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include "TFEL/Material/BoundsCheck.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel {

  namespace material {

    void BoundsCheckBase::throwOutOfLowerBoundsException(const std::string& n,
                                                         const std::string& v,
                                                         const std::string& b) {
      throw(OutOfBoundsException(
          "BoundsCheckBase::throwOutOfLowerBoundsException : variable '" + n +
          "' is below its lower bound (" + v + "<" + b + ")"));
    }

    void BoundsCheckBase::throwOutOfUpperBoundsException(const std::string& n,
                                                         const std::string& v,
                                                         const std::string& b) {
      throw(OutOfBoundsException(
          "BoundsCheckBase::throwOutOfUpperBoundsException : variable '" + n +
          "' is above its upper bound (" + v + ">" + b + ")"));
    }

    void BoundsCheckBase::throwOutOfBoundsException(const std::string& n,
                                                    const std::string& v,
                                                    const std::string& lb,
                                                    const std::string& ub) {
      throw(OutOfBoundsException(
          "BoundsCheckBase::throwOutOfLowerBoundsException : variable '" + n +
          "' is out of its bounds (" + v + "<" + lb + " or " + v + ">" + ub +
          ")"));
    }

    void BoundsCheckBase::displayOutOfLowerBoundsWarning(const std::string& n,
                                                         const std::string& v,
                                                         const std::string& b) {
      using namespace std;
      cerr << "BoundsCheckBase::displayOutOfLowerBoundsWarning : variable '"
           << n << "' is below its lower bound (" << v << "<" << b << ")"
           << std::endl;
    }

    void BoundsCheckBase::displayOutOfUpperBoundsWarning(const std::string& n,
                                                         const std::string& v,
                                                         const std::string& b) {
      using namespace std;
      cerr << "BoundsCheckBase::displayOutOfUpperBoundsWarning : variable '"
           << n << "' is above its upper bound (" << v << ">" << b << ")"
           << endl;
    }

    void BoundsCheckBase::displayOutOfBoundsWarning(const std::string& n,
                                                    const std::string& v,
                                                    const std::string& lb,
                                                    const std::string& ub) {
      using namespace std;
      cerr << "BoundsCheckBase::displayOutOfLowerBoundsWarning : variable '"
           << n << "' is out of its bounds (" << v << "<" << lb << " or " << v
           << ">" << ub << ")" << endl;
    }

  }  // end of namespace material

}  // end of namespace tfel
