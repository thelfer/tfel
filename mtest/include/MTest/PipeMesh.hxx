/*!
 * \file   PipeMesh.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_PIPEMESH_HXX
#define LIB_MTEST_PIPEMESH_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MTest/Types.hxx"

namespace mtest{

  //! structure describing the pipe mesh
  struct PipeMesh{
    //! how the pipe is modelled
    enum ElementType {
#ifdef PIPETEST_HHO_SUPPORT
      HYBRID_HIGH_ORDER,
#endif /* PIPETEST_HHO_SUPPORT */
      CUBIC,
      QUADRATIC,
      LINEAR,
      DEFAULT
    };  // end of enum ElementType
    //! inner radius
    real inner_radius = real(-1);
    //! outer radius
    real outer_radius = real(-1);
    //! number of elements
    int  number_of_elements = -1;
    //! element type
    ElementType etype = DEFAULT;
    //! element data
    tfel::utilities::Data edata;
  }; // end of struct PipeMesh

} // end of namespace mtest

#endif /* LIB_MTEST_PIPEMESH_HXX */
