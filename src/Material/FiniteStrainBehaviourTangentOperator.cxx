/*!
 * \file  src/Material/FiniteStrainBehaviourTangentOperator.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

namespace tfel {

  namespace material {

    std::vector<FiniteStrainBehaviourTangentOperatorBase::Flag>
    getFiniteStrainBehaviourTangentOperatorFlags() {
      return {FiniteStrainBehaviourTangentOperatorBase::DSIG_DF,
              FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF,
              FiniteStrainBehaviourTangentOperatorBase::C_TRUESDELL,
              FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI,
              FiniteStrainBehaviourTangentOperatorBase::C_TAU_JAUMANN,
              FiniteStrainBehaviourTangentOperatorBase::ABAQUS,
              FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE,
              FiniteStrainBehaviourTangentOperatorBase::DTAU_DF,
              FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF,
              FiniteStrainBehaviourTangentOperatorBase::DS_DF,
              FiniteStrainBehaviourTangentOperatorBase::DS_DDF,
              FiniteStrainBehaviourTangentOperatorBase::DS_DC,
              FiniteStrainBehaviourTangentOperatorBase::DS_DEGL,
              FiniteStrainBehaviourTangentOperatorBase::DT_DELOG};
    }

    std::string getFiniteStrainBehaviourTangentOperatorFlagType(
        const FiniteStrainBehaviourTangentOperatorBase::Flag f) {
      switch (f) {
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DF:
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF:
        case FiniteStrainBehaviourTangentOperatorBase::DTAU_DF:
        case FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF:
        case FiniteStrainBehaviourTangentOperatorBase::DS_DF:
        case FiniteStrainBehaviourTangentOperatorBase::DS_DDF:
          return "t2tost2";
        case FiniteStrainBehaviourTangentOperatorBase::C_TRUESDELL:
        case FiniteStrainBehaviourTangentOperatorBase::C_TAU_JAUMANN:
        case FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI:
        case FiniteStrainBehaviourTangentOperatorBase::ABAQUS:
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE:
        case FiniteStrainBehaviourTangentOperatorBase::DS_DC:
        case FiniteStrainBehaviourTangentOperatorBase::DS_DEGL:
        case FiniteStrainBehaviourTangentOperatorBase::DT_DELOG:
          return "st2tost2";
      }
      raise(
          "getFiniteStrainBehaviourTangentOperatorFlagType : "
          "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
    }

    std::string convertFiniteStrainBehaviourTangentOperatorFlagToString(
        const FiniteStrainBehaviourTangentOperatorBase::Flag f) {
      switch (f) {
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DF:
          return "DSIG_DF";
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF:
          return "DSIG_DDF";
        case FiniteStrainBehaviourTangentOperatorBase::C_TRUESDELL:
          return "C_TRUESDELL";
        case FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI:
          return "SPATIAL_MODULI";
        case FiniteStrainBehaviourTangentOperatorBase::ABAQUS:
          return "ABAQUS";
        case FiniteStrainBehaviourTangentOperatorBase::C_TAU_JAUMANN:
          return "C_TAU_JAUMANN";
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE:
          return "DSIG_DDE";
        case FiniteStrainBehaviourTangentOperatorBase::DTAU_DF:
          return "DTAU_DF";
        case FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF:
          return "DTAU_DDF";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DF:
          return "DS_DF";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DDF:
          return "DS_DDF";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DC:
          return "DS_DC";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DEGL:
          return "DS_DEGL";
        case FiniteStrainBehaviourTangentOperatorBase::DT_DELOG:
          return "DT_DELOG";
      }
      raise(
          "convertFiniteStrainBehaviourTangentOperatorFlagToString : "
          "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
    }

    std::string getFiniteStrainBehaviourTangentOperatorDescription(
        const FiniteStrainBehaviourTangentOperatorBase::Flag t) {
      switch (t) {
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DF:
          return "derivative of the Cauchy stress with respect to "
                 "the deformation gradient.";
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDF:
          return "derivative of the Cauchy stress with respect to "
                 "the deformation gradient increment";
        case FiniteStrainBehaviourTangentOperatorBase::C_TRUESDELL:
          return "derivative of the Cauchy stress with respect to "
                 "the deformation rate";
        case FiniteStrainBehaviourTangentOperatorBase::ABAQUS:
          return "tangent moduli associated with the Jaumann rate "
                 "of the Kirchhoff stress divided by the change of volume";
        case FiniteStrainBehaviourTangentOperatorBase::C_TAU_JAUMANN:
          return "tangent moduli associated with the Jaumann rate "
                 "of the Kirchhoff stress";
        case FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI:
          return "tangent moduli associated with Lie derivative of "
                 "the Kirchhoff stress";
        case FiniteStrainBehaviourTangentOperatorBase::DSIG_DDE:
          return "derivative of the Cauchy stress with respect to "
                 "the small strain deformation";
        case FiniteStrainBehaviourTangentOperatorBase::DTAU_DF:
          return "derivative of the Kirchhoff stress with respect to "
                 "the deformation gradient increment ";
        case FiniteStrainBehaviourTangentOperatorBase::DTAU_DDF:
          return "derivative of the Kirchhoff stress with respect to "
                 "the deformation gradient increment";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DF:
          return "derivative of the second Piola-Kirchhoff stress "
                 "with respect to the deformation gradient";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DDF:
          return "derivative of the second Piola-Kirchhoff stress "
                 "with respect to the deformation gradient increment";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DC:
          return "derivative of the second Piola-Kirchhoff stress "
                 "with respect to the right Cauchy-Green tensor";
        case FiniteStrainBehaviourTangentOperatorBase::DS_DEGL:
          return "derivative of the second Piola-Kirchhoff stress "
                 "with respect to the Green-Lagrange strain tensor";
        case FiniteStrainBehaviourTangentOperatorBase::DT_DELOG:
          return "derivative of the dual of the lagrangian Hencky strain "
                 "with respect to the lagrangian Hencky strain tensor";
      }
      raise(
          "getFiniteStrainBehaviourTangentOperatorDescription : "
          "unsupported FiniteStrainBehaviourTangentOperatorBase flag ");
    }

  }  // end of namespace material

}  // end of namespace tfel
