/*!
 * \file   mfront/src/HookeStressPotential.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/HookeStressPotential.hxx"

namespace mfront {

  namespace bbrick {

    HookeStressPotential::HookeStressPotential() = default;

    bool HookeStressPotential::handleIsotropicDamage() const{
      return false;
    }  // end of HookeStressPotential::handleIsotropicDamage()

    HookeStressPotential::~HookeStressPotential() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
