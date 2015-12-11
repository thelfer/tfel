/*!
 * \file   PipeMesh.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_PIPEMESH_HXX_
#define _LIB_MTEST_PIPEMESH_HXX_

#include"MTest/Types.hxx"

namespace mtest{

  //! structure describing the pipe mesh
  struct PipeMesh{
    real inner_radius = real(-1);
    real outer_radius = real(-1);
    int  number_of_elements = -1;
  }; // end of struct PipeMesh

}

#endif /* _LIB_MTEST_PIPEMESH_HXX_ */
