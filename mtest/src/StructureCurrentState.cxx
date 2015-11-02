/*!
 * \file   StructureCurrentState.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   28 oct. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MTest/CurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"

namespace mtest{

  StructureCurrentState::StructureCurrentState() = default;

  StructureCurrentState::StructureCurrentState(StructureCurrentState&&)      = default;

  StructureCurrentState::StructureCurrentState(const StructureCurrentState&) = default;

  StructureCurrentState::~StructureCurrentState() noexcept = default;
  
} // end of namespace mtest
