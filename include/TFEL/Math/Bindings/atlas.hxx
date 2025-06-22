/*!
 * \file  include/TFEL/Math/Bindings/atlas.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ATLAS_H_
#define LIB_TFEL_MATH_ATLAS_H_

#include <stdexcept>

extern "C" {
#include <clapack.h>
}

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"

namespace tfel {

  namespace math {

    namespace atlas {

      void gesv(tfel::math::matrix<double>& A, tfel::math::vector<double>& b) {
        using namespace std;
        using namespace tfel::math;
        using tfel::math::vector;
        vector<int> pv(b.size(), 0);
        if (clapack_dgesv(CblasRowMajor, b.size(), 1, &A(0, 0), b.size(),
                          &pv[0], &b(0), b.size()) != 0) {
          throw(runtime_error("gesv failed"));
        }
      }

    }  // namespace atlas

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ATLAS_H_ */
