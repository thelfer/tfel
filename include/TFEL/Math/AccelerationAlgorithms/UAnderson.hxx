/*!
 * \file   UAnderson.hxx
 * \brief
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_UANDERSON_HXX
#define LIB_TFEL_MATH_UANDERSON_HXX

#include "TFEL/Math/AccelerationAlgorithms/AndersonBase.hxx"

namespace tfel {

  namespace math {

    template <typename Field, typename real>
    struct UAnderson : public AndersonBase<Field, real> {
      //! constructor
      UAnderson(const typename AndersonBase<Field, real>::Allocator);
      //! Displacement fields for a new iteration
      //! \param uO,uN Old and new displacement field
      void newIter(Field*& uO, Field*& uN);
      //! First Iteration of a new time step
      //! \param uO,uN Old and new displacement field
      void restart(Field*& uO, Field*& uN);
    };  // end of struct UAnderson

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/AccelerationAlgorithms/UAnderson.ixx"

#endif /* LIB_TFEL_MATH_UANDERSON_HXX */
