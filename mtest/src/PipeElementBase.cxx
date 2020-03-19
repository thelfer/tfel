/*!
 * \file   PipeElementBase.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   18/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include "MTest/PipeMesh.hxx"
#include "MTest/PipeElementBase.hxx"

namespace mtest{

  static real computeInnerRadius(const PipeMesh& m, const size_type i) {
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    // radial position of the first node
    return Ri + dr * i;
  } // end of computeInnerRadius

  static real computeOuterRadius(const PipeMesh& m, const size_type i) {
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    // radial position of the second node
    return Ri + dr * (i + 1);
  } // end of computeOuterRadius

  PipeElementBase::PipeElementBase(const PipeMesh& m,
                                   const Behaviour& b,
                                   const size_type n)
      : inner_radius(computeInnerRadius(m, n)),
        outer_radius(computeOuterRadius(m, n)),
        behaviour(b),
        index(n) {}  // end of PipeElementBase::PipeElementBase

  PipeElementBase::~PipeElementBase() = default;

}  // end of namespace mtest
