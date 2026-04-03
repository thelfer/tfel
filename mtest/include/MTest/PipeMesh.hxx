/*!
 * \file   PipeMesh.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEMESH_HXX
#define LIB_MTEST_PIPEMESH_HXX

#include "MTest/Types.hxx"

namespace mtest {

  //! structure describing the pipe mesh
  struct PipeMesh {
    //! how the pipe is modelled
    enum ElementType {
      DEFAULT,
      LINEAR,
      QUADRATIC,
      CUBIC
    };  // end of enum ElementType
    //! inner radius
    real inner_radius = real(-1);
    //! outer radius
    real outer_radius = real(-1);
    //! number of elements
    int number_of_elements = -1;
    //! element type
    ElementType etype = DEFAULT;
  };  // end of struct PipeMesh

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEMESH_HXX */
