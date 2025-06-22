/*!
 * \file   mfront/src/BoundsDescription.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   06 Mar 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "MFront/BoundsDescription.hxx"

namespace mfront {

  BoundsDescription::BoundsDescription() = default;
  BoundsDescription::BoundsDescription(const BoundsDescription&) = default;
  BoundsDescription::BoundsDescription(BoundsDescription&&) = default;
  BoundsDescription& BoundsDescription::operator=(const BoundsDescription&) =
      default;
  BoundsDescription& BoundsDescription::operator=(BoundsDescription&&) =
      default;
  BoundsDescription::~BoundsDescription() = default;

  void BoundsDescription::writeBoundsChecks(std::ostream& file) const {
    if (this->arraySize == 1u) {
      this->writeBoundsChecks(file, this->varName);
    } else {
      for (unsigned short i = 0; i != this->arraySize; ++i) {
        std::ostringstream n;
        n << this->varName;
        n << '[' << i << ']';
        this->writeBoundsChecks(file, n.str());
      }
    }
  }  // end of BoundsDescription::writeBoundsChecks

  void BoundsDescription::writeBoundsChecks(std::ostream& file,
                                            const std::string& n) const {
    const auto prec = file.precision();
    file.precision(14);
    if (this->category == Standard) {
      if (this->boundsType == Lower) {
        if (this->varCategory == ExternalStateVariableII) {
          file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "0\",this->" << n
               << "0,"
               << "static_cast<real>(" << this->lowerBound
               << "),this->policy);\n";
          file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "1\",this->" << n
               << "1,"
               << "static_cast<real>(" << this->lowerBound
               << "),this->policy);\n";
        } else {
          file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "\",this->" << n
               << ","
               << "static_cast<real>(" << this->lowerBound
               << "),this->policy);\n";
          if (this->varCategory == ExternalStateVariable) {
            file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "+d" << n
                 << "\",this->" << n << "+this->d" << n << ","
                 << "static_cast<real>(" << this->lowerBound
                 << "),this->policy);\n";
          }
        }
      } else if (this->boundsType == Upper) {
        if (this->varCategory == ExternalStateVariableII) {
          file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "0\",this->" << n
               << "0,"
               << "static_cast<real>(" << this->upperBound
               << "),this->policy);\n";
          file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "1\",this->" << n
               << "1,"
               << "static_cast<real>(" << this->upperBound
               << "),this->policy);\n";
        } else {
          file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "\",this->" << n
               << ","
               << "static_cast<real>(" << this->upperBound
               << "),this->policy);\n";
          if (this->varCategory == ExternalStateVariable) {
            file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "+d" << n
                 << "\",this->" << n << "+this->d" << n << ","
                 << "static_cast<real>(" << this->upperBound
                 << "),this->policy);\n";
          }
        }
      } else if (this->boundsType == LowerAndUpper) {
        if (this->varCategory == ExternalStateVariableII) {
          file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\"" << n
               << "0\",this->" << n << "0,"
               << "static_cast<real>(" << this->lowerBound << "),"
               << "static_cast<real>(" << this->upperBound
               << "),this->policy);\n";
          file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\"" << n
               << "1\",this->" << n << "1,"
               << "static_cast<real>(" << this->lowerBound << "),"
               << "static_cast<real>(" << this->upperBound
               << "),this->policy);\n";
        } else {
          file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\"" << n
               << "\",this->" << n << ","
               << "static_cast<real>(" << this->lowerBound << "),"
               << "static_cast<real>(" << this->upperBound
               << "),this->policy);\n";
          if (this->varCategory == ExternalStateVariable) {
            file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\"" << n << "+d"
                 << n << "\",this->" << n << "+this->d" << n << ","
                 << "static_cast<real>(" << this->lowerBound << "),"
                 << "static_cast<real>(" << this->upperBound
                 << "),this->policy);\n";
          }
        }
      }
    } else if (this->category == Physical) {
      if (this->boundsType == Lower) {
        if (this->varCategory == ExternalStateVariableII) {
          file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "0\",this->" << n
               << "0,"
               << "static_cast<real>(" << this->lowerBound << "));\n";
          file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "1\",this->" << n
               << "1,"
               << "static_cast<real>(" << this->lowerBound << "));\n";
        } else {
          file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "\",this->" << n
               << ","
               << "static_cast<real>(" << this->lowerBound << "));\n";
          if (this->varCategory == ExternalStateVariable) {
            file << "BoundsCheck<N>::lowerBoundCheck(\"" << n << "+d" << n
                 << "\",this->" << n << "+this->d" << n << ","
                 << "static_cast<real>(" << this->lowerBound << "));\n";
          }
        }
      } else if (this->boundsType == Upper) {
        if (this->varCategory == ExternalStateVariableII) {
          file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "0\",this->" << n
               << "0,"
               << "static_cast<real>(" << this->upperBound << "));\n";
          file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "1\",this->" << n
               << "1,"
               << "static_cast<real>(" << this->upperBound << "));\n";
        } else {
          file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "\",this->" << n
               << ","
               << "static_cast<real>(" << this->upperBound << "));\n";
          if (this->varCategory == ExternalStateVariable) {
            file << "BoundsCheck<N>::upperBoundCheck(\"" << n << "+d" << n
                 << "\",this->" << n << "+this->d" << n << ","
                 << "static_cast<real>(" << this->upperBound << "));\n";
          }
        }
      } else if (this->boundsType == LowerAndUpper) {
        if (this->varCategory == ExternalStateVariableII) {
          file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\"" << n
               << "0\",this->" << n << "0,"
               << "static_cast<real>(" << this->lowerBound << "),"
               << "static_cast<real>(" << this->upperBound << "));\n";
          file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\"" << n
               << "1\",this->" << n << "1,"
               << "static_cast<real>(" << this->lowerBound << "),"
               << "static_cast<real>(" << this->upperBound << "));\n";
        } else {
          file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\"" << n
               << "\",this->" << n << ","
               << "static_cast<real>(" << this->lowerBound << "),"
               << "static_cast<real>(" << this->upperBound << "));\n";
          if (this->varCategory == ExternalStateVariable) {
            file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\"" << n << "+d"
                 << n << "\",this->" << n << "+this->d" << n << ","
                 << "static_cast<real>(" << this->lowerBound << "),"
                 << "static_cast<real>(" << this->upperBound << "));\n";
          }
        }
      }
    } else {
      throw(std::runtime_error(
          "BoundsDescription::writeBoundsChecks : internal error"));
    }
    file.precision(prec);
  }  // end of BoundsDescription::writeBoundsChecks

}  // end of namespace mfront
