/*!
 * \file   mfront/src/GenericBehaviourInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/GenericBehaviourSymbolsGenerator.hxx"
#include "MFront/GenericBehaviourInterface.hxx"

namespace mfront {

  /*!
   * \brief write the declaration of the rotation functions
   * \param[out] os: output stream
   * \param[in] i: generic behaviour interface
   * \param[in] bd: behaviour description
   * \param[in] n: name of the generated function
   */
  static void writeRotationFunctionsDeclarations(
      std::ostream& os,
      const GenericBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name) {
    if (bd.getSymmetryType() != mfront::ORTHOTROPIC) {
      return;
    }
    const auto mhs = i.getModellingHypothesesToBeTreated(bd);
    const auto is_finite_strain =
        (bd.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
        ((bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
         (bd.isStrainMeasureDefined()) &&
         (bd.getStrainMeasure() != BehaviourDescription::LINEARISED));
    for (const auto h : mhs) {
      const auto f = i.getFunctionNameForHypothesis(name, h);
      os << "/*!\n"
            " * \\brief rotate the gradients from the global frame to the"
            " material frame\n"
            " * \\param[out] dest: gradients in the material frame\n"
            " * \\param[in] src: gradients in the global frame\n"
            " * \\param[in] rv: rotation matrix\n"
            " */\n"
         << "MFRONT_SHAREDOBJ void " << f << "_rotateGradients("
         << "mfront_gb_real* const, "
         << "const mfront_gb_real* const, "
         << "const mfront_gb_real* const);\n\n"
         << "/*!\n"
            " * \\brief rotate an array of gradients from the global frame to"
            " the material frame\n"
            " * \\param[out] dest: pointer to the gradients array in the"
            " material frame\n"
            " * \\param[in] src: pointer to the gradients array in the global"
            " frame\n"
            " * \\param[in] rv: rotation matrix\n"
            " * \\param[in] s: number of entry of the array to be treated\n"
            " */\n"
         << "MFRONT_SHAREDOBJ void " << f << "_rotateArrayOfGradients("
         << "mfront_gb_real* const, "
         << "const mfront_gb_real* const, "
         << "const mfront_gb_real* const, "
         << "const mfront_gb_size_type);\n\n";
      if (is_finite_strain) {
        os << "/*!\n"
              " * \\brief rotate the Cauchy stress from the material "
              " frame to the global frame\n"
              " * \\param[out] dest: Cauchy stress in the global frame\n"
              " * \\param[in] src: Cauchy stress in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateThermodynamicForces_CauchyStress("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
              "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of Cauchy stresses from the material"
              " frame to the global frame\n"
              " * \\param[out] dest: array of Cauchy stresses in the global"
              " frame\n"
              " * \\param[in] src: array of Cauchy stresses in the material"
              " frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfThermodynamicForces_CauchyStress("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n"
           << "/*!\n"
              " * \\brief rotate the second Piola-Kirchhoff stress from the"
              " material frame to the global frame\n"
              " * \\param[out] dest: second Piola-Kirchhoff stress in the"
              " global frame\n"
              " * \\param[in] src: second Piola-Kirchhoff stress in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateThermodynamicForces_PK2Stress("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of second Piola-Kirchhoff stresses"
              " from the material frame to the global frame\n"
              " * \\param[out] dest: array of second Piola-Kirchhoff stresses"
              " in the global frame\n"
              " * \\param[out] src: array of second Piola-Kirchhoff stresses"
              " in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfThermodynamicForces_PK2Stress("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n"
           << "/*!\n"
              " * \\brief rotate the first Piola-Kirchhoff stress from the"
              " material frame to the global frame\n"
              " * \\param[out] dest: first Piola-Kirchhoff stress in the"
              " global frame\n"
              " * \\param[in] src: first Piola-Kirchhoff stress in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateThermodynamicForces_PK1Stress("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of the first Piola-Kirchhoff"
              " stresses from the material frame to the global frame\n"
              " * \\param[out] dest: array of first Piola-Kirchhoff stresses"
              " in the global frame\n"
              " * \\param[out] src: array of first Piola-Kirchhoff stresses"
              " in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfThermodynamicForces_PK1Stress("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n"
           << "/*!\n"
              " * \\brief rotate the derivative of the Cauchy stress with"
              " respect to the deformation gradient from the material frame"
              " to the global frame\n"
              " * \\param[out] dest: derivative of the Cauchy stress with"
              " respect to the deformation gradient in the global frame\n"
              " * \\param[in] src: derivative of the Cauchy stress with"
              " respect to the deformation gradient in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateTangentOperatorBlocks_dsig_dF("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of derivatives of the Cauchy stress"
              " with respect to the deformation gradient from the material"
              " frame to the global frame\n"
              " * \\param[out] dest: array of derivatives of the Cauchy stress"
              " with respect to the deformation gradient in the global "
              " frame\n"
              " * \\param[in] src: array of derivatives of the Cauchy stress"
              " with respect to the deformation gradient in the material"
              " frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfTangentOperatorBlocks_dsig_dF("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n"
           << "/*!\n"
              " * \\brief rotate the derivative of the first Piola-Kirchhoff"
              " stress with respect to the deformation gradient from the"
              " material frame to the global frame\n"
              " * \\param[out] dest: derivative of the first Piola-Kirchhoff"
              " stress with respect to the deformation gradient in the"
              " global frame\n"
              " * \\param[in] src: derivative of the first Piola-Kirchhoff"
              " stress with respect to the deformation gradient in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateTangentOperatorBlocks_dPK1_dF("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of derivatives of the first"
              " Piola-Kirchhoff stress with respect to the deformation"
              " gradient from the material frame to the global frame\n"
              " * \\param[out] dest: array of derivatives of the first"
              " Piola-Kirchhoff stress with respect to the deformation"
              " gradient in the global frame\n"
              " * \\param[in] src: derivative of the first Piola-Kirchhoff"
              " stress with respect to the deformation gradient in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfTangentOperatorBlocks_dPK1_dF("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n"
           << "/*!\n"
              " * \\brief rotate the derivative of the second Piola-Kirchhoff"
              " stress with respect to the Green-Lagrange strain from the"
              " material frame to the global frame\n"
              " * \\param[out] dest: derivative of the second Piola-Kirchhoff"
              " stress with respect to the Green-Lagrange strain in the"
              " global frame\n"
              " * \\param[in] src: derivative of the second Piola-Kirchhoff"
              " stress with respect to the Green-Lagrange strain in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateTangentOperatorBlocks_dPK2_dEGL("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of the derivatives of the second"
              " Piola-Kirchhoff stress with respect to the Green-Lagrange"
              " strain from the material frame to the global frame\n"
              " * \\param[out] dest: array of derivatives of the second"
              " Piola-Kirchhoff stress with respect to the Green-Lagrange"
              " strain in the global frame\n"
              " * \\param[out] src: array of derivatives of the second"
              " Piola-Kirchhoff stress with respect to the Green-Lagrange"
              " strain in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfTangentOperatorBlocks_dPK2_dEGL("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n"
           << "/*!\n"
              " * \\brief rotate the derivative of the Kirchhoff stress with"
              " respect to the spatial increment of the deformation gradient "
              "from the material frame"
              " to the global frame\n"
              " * \\param[out] dest: derivative of the Kirchhoff stress with"
              " respect to the spatial increment of the deformation gradient "
              "in the global frame\n"
              " * \\param[in] src: derivative of the Kirchhoff stress with"
              " respect to the spatial increment of the deformation gradient "
              "in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateTangentOperatorBlocks_dtau_ddF("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of derivatives of the Kirchhoff "
              "stress"
              " with respect to the spatial increment of the deformation "
              "gradient from the material"
              " frame to the global frame\n"
              " * \\param[out] dest: array of derivatives of the Kirchhoff "
              "stress"
              " with respect to the spatial increment of the deformation "
              "gradient in the global "
              " frame\n"
              " * \\param[in] src: array of derivatives of the Kirchhoff stress"
              " with respect to the spatial increment of the deformation "
              "gradient in the material"
              " frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfTangentOperatorBlocks_dtau_ddF("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n";
      } else {
        os << "/*!\n"
              " * \\brief rotate the thermodynamic forces from the material "
              " * frame to the global frame\n"
              " * \\param[out] dest: thermodynamic forces in the global frame\n"
              " * \\param[in] src: thermodynamic forces in the material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f << "_rotateThermodynamicForces("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate the thermodynamic forces from the material "
              " * frame to the global frame\n"
              " * \\param[out] dest: array of thermodynamic forces in the"
              " global frame\n"
              " * \\param[in] src: array of thermodynamic forces in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfThermodynamicForces("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
              "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n";
        os << "/*!\n"
              " * \\brief rotate the tangent operator blocks from the "
              " material frame to the global frame\n"
              " * \\param[out] dest: tangent operator blocks in the global"
              " frame\n"
              " * \\param[in] dest: tangent operator blocks in the material"
              " frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f << "_rotateTangentOperatorBlocks("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const);\n\n"
           << "/*!\n"
              " * \\brief rotate an array of tangent operators blocks from the "
              " * material frame to the global frame\n"
              " * \\param[out] dest: array of tangent operator blocks in the"
              " global frame\n"
              " * \\param[in] dest: array of tangent operator blocks in the"
              " material frame\n"
              " * \\param[in] rv: rotation matrix\n"
              " * \\param[in] s: number of entry of the array to be treated\n"
              " */\n"
           << "MFRONT_SHAREDOBJ void " << f
           << "_rotateArrayOfTangentOperatorBlocks("
           << "mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_real* const, "
           << "const mfront_gb_size_type);\n\n";
      }
    }  // end of for (const auto h : mhs)
  }    // end of writeRotationFunctionsDeclarations

  static void writeRotationMatrixDefinition(std::ostream& os, const bool b) {
    os << "const auto r = [&rv]() -> tfel::math::tmatrix<3,3,mfront::gb::real> "
          "{\n";
    os << "auto m = tfel::math::tmatrix<3,3,mfront::gb::real>{};\n"
       << "tfel::fsalgo::copy<9u>::exe(rv,m.begin());\n";
    if (b) {
      os << "return tfel::math::transpose(m);\n";
    } else {
      os << "return m;\n";
    }
    os << "}();\n";
  }  // end of writeRotationMatrixDefinition

  /*
   * \brief a simple helper structure to store the arguments of the functions in
   * charge of writing the rotations of tensors.
   */
  struct WriteRotationFunctionArgument {
    //! \brief output stream
    std::ostream& os;
    //! \brief considered modelling hypothesis
    const tfel::material::ModellingHypothesis::Hypothesis hypothesis;
    //! \brief name of the variable
    const std::string variable_name;
    //! \brief offset of the variable
    size_t variable_offset;
    /*!
     * \brief string describing an offset when an array of values for multiple
     * integration points
     */
    const std::string array_offset;
  };  // end of WriteRotationFunctionArgument

  static void writeSecondOrderTensorsRotation(
      WriteRotationFunctionArgument& args,
      const char* const type,
      const char* const view_type) {
    const auto d = tfel::material::getSpaceDimension(args.hypothesis);
    if ((args.variable_offset == 0) && (args.array_offset.empty())) {
      args.os << "const auto " << args.variable_name
              << " = tfel::math::" << type << "<" << d
              << ",mfront::gb::real>{src};\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest);\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ", r);\n";
    } else if (args.variable_offset == 0) {
      args.os << "const auto " << args.variable_name
              << " = tfel::math::" << type << "<" << d
              << ",mfront::gb::real>{src + " << args.array_offset << "};\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest + " << args.array_offset << ");\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ", r);\n";
    } else if (args.array_offset.empty()) {
      args.os << "const auto " << args.variable_name
              << " = tfel::math::" << type << "<" << d
              << ",mfront::gb::real>{src + " << args.variable_offset << "};\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest + " << args.variable_offset << ");\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ", r);\n";
    } else {
      args.os << "const auto " << args.variable_name
              << " = tfel::math::" << type << "<" << d
              << ",mfront::gb::real>{src + " << args.array_offset << " + "
              << args.variable_offset << "};\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest + " << args.variable_offset << ");\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ", r);\n";
    }
  }  // end of writeSecondOrderTensorsRotation

  static void writeStensorRotation(WriteRotationFunctionArgument& args) {
    writeSecondOrderTensorsRotation(args, "stensor", "StensorView");
  }  // end of writeStensorRotation

  static void writeTensorRotation(WriteRotationFunctionArgument& args) {
    writeSecondOrderTensorsRotation(args, "tensor", "TensorView");
  }  // end of writeTensorRotation

  static void writeFourthOrderTensorsRotation(
      WriteRotationFunctionArgument& args,
      const char* const type,
      const char* const view_type) {
    const auto d = tfel::material::getSpaceDimension(args.hypothesis);
    if ((args.variable_offset == 0) && (args.array_offset.empty())) {
      args.os << "auto " << args.variable_name << " = tfel::math::" << type
              << "<" << d << ",mfront::gb::real>{};\n"
              << "std::copy(src, src + " << args.variable_name << ".size(), "
              << args.variable_name << ".begin());\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest);\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ",r);\n";
    } else if (args.variable_offset == 0) {
      args.os << "auto " << args.variable_name << " = tfel::math::" << type
              << "<" << d << ",mfront::gb::real>{};\n"
              << "std::copy(src + " << args.array_offset << ", src + "
              << args.array_offset << " + " << args.variable_name << ".size(), "
              << args.variable_name << ".begin());\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest  + " << args.array_offset << ");\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ",r);\n";
    } else if (args.array_offset.empty()) {
      args.os << "auto " << args.variable_name << " = tfel::math::" << type
              << "<" << d << ",mfront::gb::real>{};\n"
              << "std::copy(src + " << args.variable_offset << ", src + "
              << args.variable_offset << " + " << args.variable_name
              << ".size(), " << args.variable_name << ".begin());\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest + " << args.variable_offset << ");\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ",r);\n";
    } else {
      args.os << "auto " << args.variable_name << " = tfel::math::" << type
              << "<" << d << ",mfront::gb::real>{};\n"
              << "std::copy(src + " << args.array_offset << "+"
              << args.variable_offset << ", src + " << args.array_offset
              << " + " << args.variable_offset << " + " << args.variable_name
              << ".size(), " << args.variable_name << ".begin());\n"
              << "auto " << args.variable_name
              << "_view = tfel::math::" << view_type << "<" << d
              << ",mfront::gb::real>(dest + " << args.array_offset << "+"
              << args.variable_offset << ");\n"
              << args.variable_name << "_view = tfel::math::change_basis("
              << args.variable_name << ",r);\n";
    }
  }  // end of writeFourthOrderTensorsRotation

  static void writeST2toST2Rotation(WriteRotationFunctionArgument& args) {
    writeFourthOrderTensorsRotation(args, "st2tost2", "ST2toST2View");
  }  // end of writeST2toST2Rotation

  static void writeT2toST2Rotation(WriteRotationFunctionArgument& args) {
    writeFourthOrderTensorsRotation(args, "t2tost2", "T2toST2View");
  }  // end of writeT2toST2Rotation

  static void writeST2toT2Rotation(WriteRotationFunctionArgument& args) {
    writeFourthOrderTensorsRotation(args, "st2tot2", "ST2toT2View");
  }  // end of writeST2toT2Rotation

  static void writeT2toT2Rotation(WriteRotationFunctionArgument& args) {
    writeFourthOrderTensorsRotation(args, "t2tot2", "T2toT2View");
  }  // end of writeT2toT2Rotation

  static void writeVariableRotation(
      std::ostream& os,
      const tfel::material::ModellingHypothesis::Hypothesis h,
      const VariableDescription& v,
      const SupportedTypes::TypeSize& o,
      const std::string& ao = "") {
    using size_type = size_t;
    if (v.arraySize != 1u) {
      tfel::raise(
          "writeVariableRotation: "
          "array of variables are not supported");
    }
    auto variable_offset =
        static_cast<size_type>(o.getValueForModellingHypothesis(h));
    auto args =
        WriteRotationFunctionArgument{os, h, v.name, variable_offset, ao};
    const auto f = SupportedTypes::getTypeFlag(v.type);
    if (f == SupportedTypes::SCALAR) {
    } else if (f == SupportedTypes::TVECTOR) {
      os << "std::cerr << \"rotation of vector is unsupported\";\n"
         << "std::exit(-1);\n";
    } else if (f == SupportedTypes::STENSOR) {
      writeStensorRotation(args);
    } else if (f == SupportedTypes::TENSOR) {
      writeTensorRotation(args);
    } else {
      tfel::raise("writeVariableRotation: unsupported variable type");
    }
  }  // end of writeVariableRotation

  static void writeGradientsRotationFunctionsImplementation(
      std::ostream& os,
      const GenericBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    if (bd.getSymmetryType() != mfront::ORTHOTROPIC) {
      return;
    }
    const auto f = i.getFunctionNameForHypothesis(name, h);
    os << "void " << f << "_rotateGradients("
       << "mfront_gb_real* const dest, "
       << "const mfront_gb_real* const src, "
          "const mfront_gb_real* const rv){\n";
    writeRotationMatrixDefinition(os, false);
    // offset of gradient variable
    auto og = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& g = mv.first;
      writeVariableRotation(os, h, g, og);
      og += SupportedTypes::getTypeSize(g.type, g.arraySize);
    }
    os << "}\n\n";
    os << "void " << f << "_rotateArrayOfGradients("
       << "mfront_gb_real* const dest, "
       << "const mfront_gb_real* const src, "
       << "const mfront_gb_real* const rv, "
       << "const mfront_gb_size_type s){\n";
    writeRotationMatrixDefinition(os, false);
    os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
    auto gsize = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& g = mv.first;
      gsize += SupportedTypes::getTypeSize(g.type, g.arraySize);
    }
    const auto goffset =
        "idx * " + std::to_string(gsize.getValueForModellingHypothesis(h));
    og = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& g = mv.first;
      writeVariableRotation(os, h, g, og, goffset);
      og += SupportedTypes::getTypeSize(g.type, g.arraySize);
    }
    os << "}\n";
    os << "}\n\n";
  }  // end of writeGradientsRotationFunctionsImplementation

  static void writeThermodynamicForcesRotationFunctionsImplementation(
      std::ostream& os,
      const GenericBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    if (bd.getSymmetryType() != mfront::ORTHOTROPIC) {
      return;
    }
    const auto is_finite_strain =
        (bd.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
        ((bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
         (bd.isStrainMeasureDefined()) &&
         (bd.getStrainMeasure() != BehaviourDescription::LINEARISED));
    const auto f = i.getFunctionNameForHypothesis(name, h);
    // rotate thermodynamic forces
    if (is_finite_strain) {
      const auto stensor_offset =
          "idx * " + std::to_string(tfel::material::getStensorSize(h));
      const auto tensor_offset =
          "idx * " + std::to_string(tfel::material::getTensorSize(h));
      os << "void " << f << "_rotateThermodynamicForces_CauchyStress("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_cauchy = WriteRotationFunctionArgument{os, h, "sig", 0, ""};
      writeStensorRotation(args_cauchy);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfThermodynamicForces_CauchyStress("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      auto args_cauchy_2 =
          WriteRotationFunctionArgument{os, h, "sig", 0, stensor_offset};
      writeStensorRotation(args_cauchy_2);
      os << "}\n";
      os << "}\n\n";
      os << "void " << f << "_rotateThermodynamicForces_PK2Stress("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_pk2 = WriteRotationFunctionArgument{os, h, "pk2", 0, ""};
      writeStensorRotation(args_pk2);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfThermodynamicForces_PK2Stress("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      auto args_pk2_2 =
          WriteRotationFunctionArgument{os, h, "pk2", 0, stensor_offset};
      writeStensorRotation(args_pk2_2);
      os << "}\n";
      os << "}\n\n";
      os << "void " << f << "_rotateThermodynamicForces_PK1Stress("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_pk1 = WriteRotationFunctionArgument{os, h, "pk1", 0, ""};
      writeTensorRotation(args_pk1);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfThermodynamicForces_PK1Stress("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      auto args_pk1_2 =
          WriteRotationFunctionArgument{os, h, "pk1", 0, tensor_offset};
      writeTensorRotation(args_pk1_2);
      os << "}\n";
      os << "}\n\n";
    } else {
      os << "void " << f << "_rotateThermodynamicForces("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto otf = SupportedTypes::TypeSize{};
      for (const auto& mv : bd.getMainVariables()) {
        const auto& tf = mv.second;
        writeVariableRotation(os, h, tf, otf, "");
        otf += SupportedTypes::getTypeSize(tf.type, tf.arraySize);
      }
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfThermodynamicForces("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      auto thsize = SupportedTypes::TypeSize{};
      for (const auto& mv : bd.getMainVariables()) {
        const auto& th = mv.second;
        thsize += SupportedTypes::getTypeSize(th.type, th.arraySize);
      }
      const auto thoffset =
          "idx * " + std::to_string(thsize.getValueForModellingHypothesis(h));
      otf = SupportedTypes::TypeSize{};
      for (const auto& mv : bd.getMainVariables()) {
        const auto& tf = mv.second;
        writeVariableRotation(os, h, tf, otf, thoffset);
        otf += SupportedTypes::getTypeSize(tf.type, tf.arraySize);
      }
      os << "}\n";
      os << "}\n\n";
    }
  }  // end of writeThermodynamicForcesRotationFunctionsImplementation

  static void writeTangentOperatorRotationFunctionsImplementationBody(
      std::ostream& os,
      const std::string& f,
      const BehaviourDescription& bd,
      const tfel::material::ModellingHypothesis::Hypothesis h,
      const std::string& array_offset = "") {
    using size_type = size_t;
    auto o = size_type{};
    for (const auto& b : bd.getTangentOperatorBlocks()) {
      const auto& v1 = b.first;
      const auto& v2 = b.second;
      if ((v1.arraySize != 1) || (v2.arraySize != 1)) {
        os << "std::cerr << \"" << f << ": \"\n"
           << "          << \"array of gradients are not supported\";\n"
           << "std::exit(-1);\n";
      } else {
        const auto to_name = "d" + v1.name + "_d" + v2.name;
        auto args =
            WriteRotationFunctionArgument{os, h, to_name, o, array_offset};
        const auto f1 = SupportedTypes::getTypeFlag(v1.type);
        const auto f2 = SupportedTypes::getTypeFlag(v2.type);
        if (f1 == SupportedTypes::SCALAR) {
          if (f2 == SupportedTypes::SCALAR) {
          } else if (f2 == SupportedTypes::TVECTOR) {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported tangent operator type\";\n"
               << "std::exit(-1);\n";
          } else if (f2 == SupportedTypes::STENSOR) {
            writeStensorRotation(args);
          } else if (f2 == SupportedTypes::TENSOR) {
            writeTensorRotation(args);
          } else {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported gradient type\";\n"
               << "std::exit(-1);\n";
          }
        } else if (f1 == SupportedTypes::TVECTOR) {
          os << "std::cerr << \"" << f << ": \"\n"
             << "          << \"unsupported tangent operator type\";\n"
             << "std::exit(-1);\n";
        } else if (f1 == SupportedTypes::STENSOR) {
          if (f2 == SupportedTypes::SCALAR) {
            writeStensorRotation(args);
          } else if (f2 == SupportedTypes::TVECTOR) {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported tangent operator type\";\n"
               << "std::exit(-1);\n";
          } else if (f2 == SupportedTypes::STENSOR) {
            writeST2toST2Rotation(args);
          } else if (f2 == SupportedTypes::TENSOR) {
            writeT2toST2Rotation(args);
          } else {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported gradient type\";\n"
               << "std::exit(-1);\n";
          }
        } else if (f1 == SupportedTypes::TENSOR) {
          if (f2 == SupportedTypes::SCALAR) {
            writeTensorRotation(args);
          } else if (f2 == SupportedTypes::TVECTOR) {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported tangent operator type\";\n"
               << "std::exit(-1);\n";
          } else if (f2 == SupportedTypes::STENSOR) {
            writeST2toT2Rotation(args);
          } else if (f2 == SupportedTypes::TENSOR) {
            writeT2toT2Rotation(args);
          } else {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported gradient type\";\n"
               << "std::exit(-1);\n";
          }
        } else {
          os << "std::cerr << \"" << f << ": \"\n"
             << "          << \"unsupported gradient type\";\n"
             << "std::exit(-1);\n";
        }
      }
      const auto s1 = static_cast<size_type>(
          SupportedTypes::getTypeSize(v1.type, v1.arraySize)
              .getValueForModellingHypothesis(h));
      const auto s2 = static_cast<size_type>(
          SupportedTypes::getTypeSize(v2.type, v2.arraySize)
              .getValueForModellingHypothesis(h));
      o += static_cast<size_type>(s1 * s2);
    }
  }  // end of writeTangentOperatorRotationFunctionsImplementationBody

  static void writeTangentOperatorRotationFunctionsImplementation(
      std::ostream& os,
      const GenericBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using size_type = size_t;
    if (bd.getSymmetryType() != mfront::ORTHOTROPIC) {
      return;
    }
    const auto f = i.getFunctionNameForHypothesis(name, h);
    const auto is_finite_strain =
        (bd.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
        ((bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
         (bd.isStrainMeasureDefined()) &&
         (bd.getStrainMeasure() != BehaviourDescription::LINEARISED));
    if (is_finite_strain) {
      const auto stsize = tfel::material::getStensorSize(h);
      const auto tsize = tfel::material::getTensorSize(h);
      os << "void " << f << "_rotateTangentOperatorBlocks_dsig_dF("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dsig_dF =
          WriteRotationFunctionArgument{os, h, "dsig_dF", 0, ""};
      writeT2toST2Rotation(args_dsig_dF);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfTangentOperatorBlocks_dsig_dF("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "const mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dsig_dF_2 = WriteRotationFunctionArgument{
          os, h, "dsig_dF", 0, "idx * " + std::to_string(stsize * tsize)};
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      writeT2toST2Rotation(args_dsig_dF_2);
      os << "}\n";
      os << "}\n\n";
      //
      os << "void " << f << "_rotateTangentOperatorBlocks_dPK1_dF("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dPK1_dF =
          WriteRotationFunctionArgument{os, h, "dpk1_dF", 0, ""};
      writeT2toT2Rotation(args_dPK1_dF);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfTangentOperatorBlocks_dPK1_dF("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "const mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      auto args_dPK1_dF_2 = WriteRotationFunctionArgument{
          os, h, "dpk1_dF", 0, "idx * " + std::to_string(tsize * tsize)};
      writeT2toT2Rotation(args_dPK1_dF_2);
      os << "}\n";
      os << "}\n\n";
      //
      os << "void " << f << "_rotateTangentOperatorBlocks_dPK2_dEGL("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dpk2_degl =
          WriteRotationFunctionArgument{os, h, "dpk2_degl", 0, ""};
      writeST2toST2Rotation(args_dpk2_degl);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfTangentOperatorBlocks_dPK2_dEGL("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real * const rv, "
         << "const mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dpk2_degl_2 = WriteRotationFunctionArgument{
          os, h, "dpk2_degl", 0, "idx * " + std::to_string(stsize * stsize)};
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      writeST2toST2Rotation(args_dpk2_degl_2);
      os << "}\n";
      os << "}\n\n";
      os << "void " << f << "_rotateTangentOperatorBlocks_dtau_ddF("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dtau_ddF =
          WriteRotationFunctionArgument{os, h, "dtau_ddF", 0, ""};
      writeT2toST2Rotation(args_dtau_ddF);
      os << "}\n\n";
      os << "void " << f << "_rotateArrayOfTangentOperatorBlocks_dtau_ddF("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
         << "const mfront_gb_real* const rv, "
         << "const mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      auto args_dtau_ddF_2 = WriteRotationFunctionArgument{
          os, h, "dtau_ddF", 0, "idx * " + std::to_string(stsize * tsize)};
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      writeT2toST2Rotation(args_dsig_dF_2);
      os << "}\n";
      os << "}\n\n";
    } else {
      os << "void " << f << "_rotateTangentOperatorBlocks("
         << "mfront_gb_real* const dest,"
         << "const mfront_gb_real* const src,"
         << "const mfront_gb_real* const rv){\n";
      writeRotationMatrixDefinition(os, true);
      writeTangentOperatorRotationFunctionsImplementationBody(
          os, f + "_rotateTangentOperatorBlocks", bd, h);
      os << "}\n\n";
      auto tob_size = size_type{};
      for (const auto& to : bd.getTangentOperatorBlocks()) {
        const auto& v1 = to.first;
        const auto& v2 = to.second;
        const auto v1size = SupportedTypes::getTypeSize(v1.type, v1.arraySize);
        const auto v2size = SupportedTypes::getTypeSize(v2.type, v2.arraySize);
        tob_size +=
            static_cast<size_type>(v1size.getValueForModellingHypothesis(h) *
                                   v2size.getValueForModellingHypothesis(h));
      }
      const auto tob_offset = "idx * " + std::to_string(tob_size);
      os << "void " << f << "_rotateArrayOfTangentOperatorBlocks("
         << "mfront_gb_real* const dest, "
         << "const mfront_gb_real* const src, "
            "const mfront_gb_real* const rv, "
         << "const mfront_gb_size_type s){\n";
      writeRotationMatrixDefinition(os, true);
      os << "for(mfront_gb_size_type idx=0; idx != s; ++idx){\n";
      writeTangentOperatorRotationFunctionsImplementationBody(
          os, f + "_rotateArrayOfTangentOperatorBlocks", bd, h, tob_offset);
      os << "}\n";
      os << "}\n\n";
    }
  }  // end of writeTangentOperatorRotationFunctionsImplementation

  static void writeRotationFunctionsImplementations(
      std::ostream& os,
      const GenericBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name) {
    const auto mhs = i.getModellingHypothesesToBeTreated(bd);
    for (const auto h : mhs) {
      writeGradientsRotationFunctionsImplementation(os, i, bd, name, h);
      writeThermodynamicForcesRotationFunctionsImplementation(os, i, bd, name,
                                                              h);
      writeTangentOperatorRotationFunctionsImplementation(os, i, bd, name, h);
    }
  }  // end of writeRotationFunctionsImplementations

  std::string GenericBehaviourInterface::getName() {
    return "generic";
  }  // end of getName

  unsigned short GenericBehaviourInterface::getAPIVersion() {
    return 1;
  }  // end of getAPIVersion

  std::string GenericBehaviourInterface::getInterfaceName() const {
    return "Generic";
  }  // end of getInterfaceName

  std::pair<bool, GenericBehaviourInterface::tokens_iterator>
  GenericBehaviourInterface::treatKeyword(BehaviourDescription& bd,
                                          const std::string& k,
                                          const std::vector<std::string>& i,
                                          tokens_iterator current,
                                          const tokens_iterator end) {
    using tfel::material::ModellingHypothesis;
    using tfel::utilities::CxxTokenizer;
    using tfel::utilities::Token;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "GenericBehaviourInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        const auto keys = std::vector<std::string>{
            {"@GenericInterfaceGenerateMTestFileOnFailure",
             "@GenerateMTestFileOnFailure"}};
        throw_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                 "unsupported key '" + k + "'");
      } else {
        return {false, current};
      }
    }
    if ((k == "@GenericInterfaceGenerateMTestFileOnFailure") ||
        (k == "@GenerateMTestFileOnFailure")) {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(k, current, end));
      return {true, current};
    }
    if ((k == "@SelectedModellingHypothesis") ||
        (k == "@SelectedModellingHypotheses")) {
      if (current == end) {
        tfel::raise(
            "GenericBehaviourInterface::treatKeyword: "
            "unexpected end of file");
      }
      if (this->selectedHypotheses) {
        tfel::raise(
            "GenericBehaviourInterface::treatKeyword: "
            "the modelling hypotheses to be treated have already been "
            "selected");
      }
      if (k == "@SelectedModellingHypothesis") {
        const auto h = ModellingHypothesis::fromString(current->value);
        ++(current);
        this->selectedHypotheses = std::set<ModellingHypothesis::Hypothesis>{};
        this->selectedHypotheses->insert(h);
        return {true, current};
      }
      if (k == "@SelectedModellingHypotheses") {
        auto hypotheses = std::set<Hypothesis>{};
        const auto values = CxxTokenizer::readList(
            "GenericBehaviourInterface::treatKeyword", "{", "}", current, end);
        for (const auto& v : values) {
          if (v.flag == Token::String) {
            hypotheses.insert(ModellingHypothesis::fromString(
                v.value.substr(1, v.value.size() - 2)));
          } else {
            hypotheses.insert(ModellingHypothesis::fromString(v.value));
          }
        }
        if (hypotheses.empty()) {
          tfel::raise(
              "GenericBehaviourInterface::treatKeyword: "
              "no hypothesis declared");
        }
        //
        this->selectedHypotheses = hypotheses;
        return {true, current};
      }
    }
    return {false, current};
  }  // end of treatKeyword

  std::set<GenericBehaviourInterface::Hypothesis>
  GenericBehaviourInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& bd) const {
    using tfel::material::ModellingHypothesis;
    const auto mhs = bd.getModellingHypotheses();
    if (this->selectedHypotheses) {
      for (const auto& h : *(this->selectedHypotheses)) {
        if (!mhs.contains(h)) {
          tfel::raise(
              "GenericBehaviourInterface::getModellingHypothesesToBeTreated: "
              "the modelling hypothesis '" +
              ModellingHypothesis::toString(h) +
              "' is not supported by the behaviour");
        }
      }
      return *(this->selectedHypotheses);
    }
    return mhs;
  }  // end of getModellingHypothesesToBeTreated

  void GenericBehaviourInterface::writeInterfaceSpecificIncludes(
      std::ostream& os, const BehaviourDescription& bd) const {
    os << "#include \"MFront/GenericBehaviour/State.hxx\"\n"
       << "#include \"MFront/GenericBehaviour/BehaviourData.hxx\"\n";
    if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      os << "#include \"MFront/GenericBehaviour/ComputeStiffnessTensor.hxx\"\n";
    }
  }  // end of writeInterfaceSpecificIncludes

  void GenericBehaviourInterface::getTargetsDescription(
      TargetsDescription& d, const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, name + "-generic.cxx");
    d.headers.push_back("MFront/GenericBehaviour/" + name + "-generic.hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(l.link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(l.epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of getTargetsDescription

  void GenericBehaviourInterface::endTreatment(
      const BehaviourDescription& bd, const FileDescription& fd) const {
    auto raise = [](const std::string& m) {
      tfel::raise("GenericBehaviourInterface::endTreatment: " + m);
    };
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/GenericBehaviour");
    const auto mhs = this->getModellingHypothesesToBeTreated(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto header = name + "-generic.hxx";
    const auto type = bd.getBehaviourType();
    const auto is_finite_strain_through_strain_measure =
        (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (bd.isStrainMeasureDefined()) &&
        (bd.getStrainMeasure() != BehaviourDescription::LINEARISED);
    std::ofstream out("include/MFront/GenericBehaviour/" + header);
    if (!out) {
      raise("could not open file '" + header + "'");
    }
    out << "/*!\n"
        << "* \\file   " << header << '\n'
        << "* \\brief  This file declares the umat interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto hg = this->getHeaderGuard(bd);
    out << "#ifndef " << hg << "\n"
        << "#define " << hg << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
        << "#include\"MFront/GenericBehaviour/BehaviourData.h\"\n\n";

    this->writeVisibilityDefines(out);
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    writeRotationFunctionsDeclarations(out, *this, bd, name);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, bd, name);

    for (const auto h : mhs) {
      const auto& d = bd.getBehaviourData(h);
      const auto f = this->getFunctionNameForHypothesis(name, h);
      // initialize function
      for (const auto& p : d.getInitializeFunctions()) {
        out << "/*!\n"
            << " * \\param[in] values: inputs of the initialize function\n"
            << " * \\param[in] d: material data\n"
            << " */\n"
            << "MFRONT_SHAREDOBJ int " << f << "_InitializeFunction_" << p.first
            << "(mfront_gb_BehaviourData* const,\n"
            << " const mfront_gb_real* const);\n\n";
      }
      out << "/*!\n"
          << " * \\param[in,out] d: material data\n"
          << " */\n"
          << "MFRONT_SHAREDOBJ int " << f
          << "(mfront_gb_BehaviourData* const);\n\n";
      // postprocessings
      for (const auto& p : d.getPostProcessings()) {
        out << "/*!\n"
            << " * \\param[out] values: results of the postprocessing\n"
            << " * \\param[in] d: material data\n"
            << " */\n"
            << "MFRONT_SHAREDOBJ int " << f << "_PostProcessing_" << p.first
            << "(mfront_gb_real* const, mfront_gb_BehaviourData* const);\n\n";
      }
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";
    out << "#endif /* " << hg << " */\n";
    out.close();

    const auto src = name + "-generic.cxx";
    out.open("src/" + src);
    if (!out) {
      raise("could not open file '" + src + "'");
    }

    out << "/*!\n"
        << "* \\file   " << src << '\n'
        << "* \\brief  This file implements the umat interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->writeVisibilityDefines(out);

    out << "#include<iostream>\n"
        << "#include<cstdlib>\n"
        << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Math/t2tot2.hxx\"\n"
        << "#include\"TFEL/Math/t2tost2.hxx\"\n";

    if (is_finite_strain_through_strain_measure) {
      out << "#include\"TFEL/Material/"
             "FiniteStrainBehaviourTangentOperator.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << bd.getClassName() << ".hxx\"\n";
    ///
    if (bd.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (is_finite_strain_through_strain_measure) {
          out << "#include \"MFront/"
              << "GenericBehaviourFiniteStrainMTestFileGenerator.hxx\"\n";
        } else {
          out << "#include \"MFront/"
              << "GenericBehaviourSmallStrainMTestFileGenerator.hxx\"\n";
        }
      } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "#include \"MFront/"
            << "GenericBehaviourFiniteStrainMTestFileGenerator.hxx\"\n";
      } else if (type == BehaviourDescription::GENERALBEHAVIOUR) {
        out << "#include \"MFront/GenericBehaviourMTestFileGenerator.hxx\"\n";
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeInterfaceSpecificIncludes: "
            "unsupported behaviour type for MTest file generation");
      }
    }
    out << "#include\"MFront/GenericBehaviour/GenericBehaviourTraits.hxx\"\n";
    // behaviour integration
    if ((type == BehaviourDescription::GENERALBEHAVIOUR) ||
        (type == BehaviourDescription::COHESIVEZONEMODEL)) {
      out << "#include\"MFront/GenericBehaviour/Integrate.hxx\"\n";
    } else if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (is_finite_strain_through_strain_measure) {
        const auto ms = bd.getStrainMeasure();
        if (ms == BehaviourDescription::GREENLAGRANGE) {
          out << "#include\"MFront/GenericBehaviour/"
              << "GreenLagrangeStrainIntegrate.hxx\"\n";
        } else if (ms == BehaviourDescription::HENCKY) {
          out << "#include\"MFront/GenericBehaviour/"
              << "LogarithmicStrainIntegrate.hxx\"\n";
        } else {
          raise("unsupported strain measure");
        }
      } else {
        out << "#include\"MFront/GenericBehaviour/Integrate.hxx\"\n";
      }
    } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "#include\"MFront/GenericBehaviour/"
          << "StandardFiniteStrainBehaviourIntegrate.hxx\"\n";
    } else {
      raise("unsupported behaviour type");
    }

    out << "#include\"MFront/GenericBehaviour/" << header << "\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, bd, name);

    // behaviour traits
    out << "namespace mfront::gb{\n\n";
    for (const auto h : mhs) {
      const auto hypothesis = "tfel::material::ModellingHypothesis::" +
                              ModellingHypothesis::toUpperCaseString(h);
      const auto behaviour = [&hypothesis, &bd] {
        if (bd.useQt()) {
          return bd.getClassName() + "<" + hypothesis + ", real, true>";
        }
        return bd.getClassName() + "<" + hypothesis + ", real, false>";
      }();
      out << "template<>\n"
          << "struct GenericBehaviourTraits<"
          << "tfel::material::" << behaviour << ">{\n"
          << "static constexpr auto hypothesis = " << hypothesis << ";\n"
          << "static constexpr auto N = "
          << "tfel::material::ModellingHypothesisToSpaceDimension<hypothesis>::"
          << "value;\n"
          << "static constexpr auto StensorSize = "
          << "tfel::material::ModellingHypothesisToStensorSize<hypothesis>::"
          << "value;\n"
          << "static constexpr auto TensorSize = "
          << "tfel::material::ModellingHypothesisToTensorSize<hypothesis>::"
          << "value;\n";
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        if (is_finite_strain_through_strain_measure) {
          const auto as =
              this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd, h);
          if (as.first) {
            out << "static constexpr bool "
                << "has_axial_strain_offset = true;\n";
            out << "static constexpr size_t axial_strain_offset = "  //
                << as.second << ";\n";
          } else {
            out << "static constexpr bool "
                << "has_axial_strain_offset = false;\n";
          }
        } else if (type ==
                   BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          const auto as =
              this->checkIfAxialDeformationGradientIsDefinedAndGetItsOffset(bd,
                                                                            h);
          if (as.first) {
            out << "static constexpr bool "
                << "has_axial_deformation_gradient_offset = true;\n";
            out << "static constexpr size_t axial_deformation_gradient_offset "
                << "= " << as.second << ";\n";
          } else {
            out << "static constexpr bool "
                << "has_axial_deformation_gradient_offset = false;\n";
          }
        }
      }
      out << "};\n\n";
    }
    out << "} // end of namespace mfront::gb\n\n";

    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    GenericBehaviourSymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, bd, fd, mhs, name);
    if (!bd.areAllMechanicalDataSpecialised(mhs)) {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      sg.generateSymbols(out, *this, bd, fd, name, uh);
    }
    for (const auto& h : mhs) {
      if (bd.hasSpecialisedMechanicalData(h)) {
        sg.generateSymbols(out, *this, bd, fd, name, h);
      }
    }

    writeRotationFunctionsImplementations(out, *this, bd, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, bd, name);
    // parameters
    this->writeSetParametersFunctionsImplementations(out, bd, name);
    // behaviour implementations
    for (const auto h : mhs) {
      const auto& d = bd.getBehaviourData(h);
      const auto f = this->getFunctionNameForHypothesis(name, h);
      // initialize functions
      for (const auto& p : d.getInitializeFunctions()) {
        out << "MFRONT_SHAREDOBJ int " << f << "_InitializeFunction_" << p.first
            << "(mfront_gb_BehaviourData* const d,\n"
            << "const mfront_gb_real* const values){\n"
            << "using namespace tfel::material;\n"
            << "using real = mfront::gb::real;\n"
            << "constexpr auto h = ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ";\n";
        if (bd.useQt()) {
          out << "using Behaviour = " << bd.getClassName()
              << "<h,real,true>;\n";
        } else {
          out << "using Behaviour = " << bd.getClassName()
              << "<h,real,false>;\n";
        }
        auto call_initialize_function =
            "executeInitializeFunction<Behaviour,  &Behaviour::execute" +
            p.first + "InitializeFunction>(" + "*d, values, " + name +
            "_getOutOfBoundsPolicy())";
        if ((type == BehaviourDescription::GENERALBEHAVIOUR) ||
            (type == BehaviourDescription::COHESIVEZONEMODEL)) {
          out << "const auto r = mfront::gb::" << call_initialize_function
              << ";\n";
        } else if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (is_finite_strain_through_strain_measure) {
            const auto ms = bd.getStrainMeasure();
            if (ms == BehaviourDescription::GREENLAGRANGE) {
              out << "const auto r = mfront::gb::green_lagrange_strain::"
                  << call_initialize_function << ";\n";
            } else if (ms == BehaviourDescription::HENCKY) {
              out << "const auto r = mfront::gb::logarithmic_strain::"
                  << call_initialize_function << ";\n";
            } else {
              raise("unsupported strain measure");
            }
          } else {
            out << "const auto r = mfront::gb::" << call_initialize_function
                << ";\n";
          }
        } else if (type ==
                   BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          out << "const auto r = mfront::gb::finite_strain::"
              << call_initialize_function << ";\n";
        } else {
          raise("unsupported behaviour type");
        }
        out << "return r;\n"
            << "}\n\n";
      }
      // behaviour integration
      out << "MFRONT_SHAREDOBJ int " << f
          << "(mfront_gb_BehaviourData* const d){\n"
          << "using namespace tfel::material;\n";
      out << "using real = mfront::gb::real;\n"
          << "constexpr auto h = ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
      if (bd.useQt()) {
        out << "using Behaviour = " << bd.getClassName() << "<h,real,true>;\n";
      } else {
        out << "using Behaviour = " << bd.getClassName() << "<h,real,false>;\n";
      }
      if (bd.getAttribute(BehaviourData::profiling, false)) {
        out << "using mfront::BehaviourProfiler;\n"
            << "using tfel::material::" << bd.getClassName() << "Profiler;\n"
            << "BehaviourProfiler::Timer total_timer(" << bd.getClassName()
            << "Profiler::getProfiler(),\n"
            << "BehaviourProfiler::TOTALTIME);\n";
      }
      if (this->shallGenerateMTestFileOnFailure(bd)) {
        out << "using mfront::SupportedTypes;\n";
      }
      if ((type == BehaviourDescription::GENERALBEHAVIOUR) ||
          (type == BehaviourDescription::COHESIVEZONEMODEL)) {
        out << "const auto r = mfront::gb::integrate<Behaviour>(*d, "
            << "Behaviour::STANDARDTANGENTOPERATOR, " << name
            << "_getOutOfBoundsPolicy());\n";
      } else if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (is_finite_strain_through_strain_measure) {
          const auto ms = bd.getStrainMeasure();
          if (ms == BehaviourDescription::GREENLAGRANGE) {
            out << "const auto r = "
                << "mfront::gb::green_lagrange_strain::integrate<Behaviour>("
                << "*d, " << name << "_getOutOfBoundsPolicy());\n";
          } else if (ms == BehaviourDescription::HENCKY) {
            out << "const auto r = "
                << "mfront::gb::logarithmic_strain::integrate<Behaviour>("
                << "*d, " << name << "_getOutOfBoundsPolicy());\n";
          } else {
            raise("unsupported strain measure");
          }
        } else {
          out << "const auto r = mfront::gb::integrate<Behaviour>(*d, "
              << "Behaviour::STANDARDTANGENTOPERATOR, " << name
              << "_getOutOfBoundsPolicy());\n";
        }
      } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "const auto r = "
            << "mfront::gb::finite_strain::integrate<Behaviour>(*d, "  //
            << name << "_getOutOfBoundsPolicy());\n";
      } else {
        raise("unsupported behaviour type");
      }
      if (this->shallGenerateMTestFileOnFailure(bd)) {
        out << "if(r!=1){\n";
        this->generateMTestFile(out, bd, h);
        out << "}\n";
      }
      out << "return r;\n"
          << "} // end of " << f << "\n\n";
    }
    // postprocessings
    for (const auto h : mhs) {
      const auto& d = bd.getBehaviourData(h);
      const auto f = this->getFunctionNameForHypothesis(name, h);
      // postprocessings
      for (const auto& p : d.getPostProcessings()) {
        out << "MFRONT_SHAREDOBJ int " << f << "_PostProcessing_" << p.first
            << "(mfront_gb_real* const values,\n"
            << "mfront_gb_BehaviourData* const d){\n"
            << "using namespace tfel::material;\n"
            << "using real = mfront::gb::real;\n"
            << "constexpr auto h = ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ";\n";
        if (bd.useQt()) {
          out << "using Behaviour = " << bd.getClassName()
              << "<h,real,true>;\n";
        } else {
          out << "using Behaviour = " << bd.getClassName()
              << "<h,real,false>;\n";
        }
        auto call_post_processing =
            "executePostProcessing<Behaviour,  &Behaviour::execute" + p.first +
            "PostProcessing, true>(" + "values, *d, " + name +
            "_getOutOfBoundsPolicy())";
        if ((type == BehaviourDescription::GENERALBEHAVIOUR) ||
            (type == BehaviourDescription::COHESIVEZONEMODEL)) {
          out << "const auto r = mfront::gb::" << call_post_processing << ";\n";
        } else if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (is_finite_strain_through_strain_measure) {
            const auto ms = bd.getStrainMeasure();
            if (ms == BehaviourDescription::GREENLAGRANGE) {
              out << "const auto r = mfront::gb::green_lagrange_strain::"
                  << call_post_processing << ";\n";
            } else if (ms == BehaviourDescription::HENCKY) {
              out << "const auto r = mfront::gb::logarithmic_strain::"
                  << call_post_processing << ";\n";
            } else {
              raise("unsupported strain measure");
            }
          } else {
            out << "const auto r = mfront::gb::" << call_post_processing
                << ";\n";
          }
        } else if (type ==
                   BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          out << "const auto r = mfront::gb::finite_strain::"
              << call_post_processing << ";\n";
        } else {
          raise("unsupported behaviour type");
        }
        out << "return r;\n"
            << "}\n\n";
      }
    }
    //
    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";
    out.close();
  }  // end of endTreatment

  static std::string as_string(const SupportedTypes::TypeFlag& f) {
    if (f == SupportedTypes::SCALAR) {
      return "SupportedTypes::SCALAR";
    }
    if (f == SupportedTypes::TVECTOR) {
      return "SupportedTypes::TVECTOR";
    }
    if (f == SupportedTypes::STENSOR) {
      return "SupportedTypes::STENSOR";
    }
    if (f == SupportedTypes::TENSOR) {
      return "SupportedTypes::TENSOR";
    }
    tfel::raise(
        "GenericBehaviourInterface::generateMTestFile: "
        "unsupported type flag");
  }  // end of as_string

  static void writeBehaviourVariablesDeclarations(
      std::ostream& os, const BehaviourDescription& bd) {
    const auto mvs = bd.getMainVariables();
    os << "using BehaviourVariablesDescription = "
       << "mfront::GenericBehaviourMTestFileGenerator::"
       << "BehaviourVariablesDescription;\n";
    os << "using VariableDescription = "
       << "BehaviourVariablesDescription::VariableDescription;\n";
    os << "const auto gradients = "
       << "std::array<VariableDescription," << mvs.size() << ">{";
    for (auto pmv = mvs.begin(); pmv != mvs.end();) {
      const auto& g = pmv->first;
      os << "{\"" << g.getExternalName() << "\", " << as_string(g.getTypeFlag())
         << "}";
      if (++pmv != mvs.end()) {
        os << ", ";
      }
    }
    os << "};\n";
    os << "const auto thermodynamic_forces = "
       << "std::array<VariableDescription," << mvs.size() << ">{";
    for (auto pmv = mvs.begin(); pmv != mvs.end();) {
      const auto& thf = pmv->second;
      os << "{\"" << thf.getExternalName() << "\", "
         << as_string(thf.getTypeFlag()) << "}";
      if (++pmv != mvs.end()) {
        os << ", ";
      }
    }
    os << "};\n";
    os << "const auto description = "
       << "BehaviourVariablesDescription{" << mvs.size() << ", "
       << "gradients.data(), "
       << "thermodynamic_forces.data()};\n\n";
  }  // end of writeBehaviourVariablesDeclarations

  void GenericBehaviourInterface::generateMTestFile(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    const auto type = bd.getBehaviourType();
    const auto generic_behaviour =
        type == BehaviourDescription::GENERALBEHAVIOUR;
    if (!((generic_behaviour) ||
          (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR))) {
      tfel::raise(
          "GenericBehaviourInterface::writeInterfaceSpecificIncludes: "
          "unsupported behaviour type for MTest file generation");
    }
    if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
    }
    const auto fs = [&bd, type] {
      if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        return true;
      }
      if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          return ms != BehaviourDescription::LINEARISED;
        }
      }
      return false;
    }();
    unsigned int offset = 0;
    const auto name = bd.getLibrary() + bd.getClassName();
    os << "try{\n";
    if (generic_behaviour) {
      writeBehaviourVariablesDeclarations(os, bd);
      os << "mfront::GenericBehaviourMTestFileGenerator mg("
         << "description, \"" << this->getLibraryName(bd) <<  //
          "\",\"" << name << "\");\n";
    } else {
      if (fs) {
        os << "mfront::GenericBehaviourFiniteStrainMTestFileGenerator mg(\""
           << this->getLibraryName(bd) << "\",\"" << name << "\");\n";
      } else {
        os << "mfront::GenericBehaviourSmallStrainMTestFileGenerator mg(\""
           << this->getLibraryName(bd) << "\",\"" << name << "\");\n";
      }
    }
    os << "mg.setModellingHypothesis("
       << "tfel::material::ModellingHypothesis::"
       << tfel::material::ModellingHypothesis::toUpperCaseString(h) << ");\n"
       << "// must be declared after setting the hypothesis\n"
       << "const auto TVectorSize = mg.getTVectorSize();\n"
       << "const auto StensorSize = mg.getStensorSize();\n"
       << "const auto TensorSize  = mg.getTensorSize();\n"
       << "const auto dt = std::max(d->dt,1.e-50);\n"
       << "mg.setHandleThermalExpansion(false);\n"
       << "mg.addTime(0.);\n"
       << "mg.addTime(dt);\n";
    if (generic_behaviour) {
      os << "mg.setGradientsAtTheBeginningOfTheTimeStep("
         << "d->s0.gradients);\n"
         << "mg.setGradientsAtTheEndOfTheTimeStep("
         << "d->s1.gradients);\n"
         << "mg.setThermodynamicForcesAtTheBeginningOfTheTimeStep("
         << "d->s0.thermodynamic_forces);\n";
    } else {
      if (fs) {
        os << "mg.setDeformationGradientTensorAtTheBeginningOfTheTimeStep("
           << "d->s0.gradients);\n"
           << "mg.setDeformationGradientTensorAtTheEndOfTheTimeStep("
           << "d->s1.gradients);\n";
      } else {
        os << "mg.setStrainTensorAtTheBeginningOfTheTimeStep("
           << "d->s0.gradients);\n"
           << "mg.setStrainTensorAtTheEndOfTheTimeStep("
           << "d->s1.gradients);\n";
      }
      os << "mg.setStressTensor(d->s0.thermodynamic_forces);\n";
    }
    for (const auto& m : mprops.first) {
      tfel::raise_if(!m.isScalar(),
                     "GenericBehaviourInterface::generateMTestFile: "
                     "unsupported material property  type '" +
                         m.type + "' in mtest file generation");
      if (m.arraySize == 1u) {
        if (offset == 0) {
          os << "mg.addMaterialProperty(\"" << m.getExternalName()
             << "\",*(d->s1.material_properties));\n";
        } else {
          os << "mg.addMaterialProperty(\"" << m.getExternalName()
             << "\",*(d->s1.material_properties+" << offset << "));\n";
        }
        ++offset;
      } else {
        for (unsigned short s = 0; s != m.arraySize; ++s, ++offset) {
          if (offset == 0) {
            os << "mg.addMaterialProperty(\"" << m.getExternalName() << "[" << s
               << "]\",*(d->s1.material_properties));\n";
          } else {
            os << "mg.addMaterialProperty(\"" << m.getExternalName() << "[" << s
               << "]\","
               << "*(d->s1.material_properties+" << offset << "));\n";
          }
        }
      }
    }
    SupportedTypes::TypeSize ivoffset;
    for (const auto& v : persistentVarsHolder) {
      auto flag = SupportedTypes::getTypeFlag(v.type);
      const auto& ivname = d.getExternalName(v.name);
      if (v.arraySize == 1u) {
        os << "mg.addInternalStateVariable(\"" << ivname << "\","
           << as_string(flag) << ","
           << "&(d->s0.internal_state_variables[" << ivoffset << "]));\n";
        ivoffset += SupportedTypes::TypeSize(flag);
      } else {
        if (v.arraySize >= SupportedTypes::ArraySizeLimit) {
          os << "for(unsigned short i=0;i!=" << v.arraySize << ";++i){\n";
          os << "auto name =  \"" << ivname
             << "[\" + std::to_string(i)+ \"]\";\n";
          os << "mg.addInternalStateVariable(name," << as_string(flag)
             << ",&d->s0.internal_state_variables[" << ivoffset << "]+i);\n";
          os << "}\n";
          ivoffset += SupportedTypes::TypeSize(v.arraySize, flag);
        } else {
          for (unsigned short i = 0; i != v.arraySize; ++i) {
            os << "mg.addInternalStateVariable(\"" << ivname << "[" << i
               << "]\"," << as_string(flag)
               << ",&(d->s0.internal_state_variables[" << ivoffset << "]));\n";
            ivoffset += SupportedTypes::TypeSize(flag);
          }
        }
      }
    }
    SupportedTypes::TypeSize evoffset;
    for (const auto& ev : externalStateVarsHolder) {
      auto flag = SupportedTypes::getTypeFlag(ev.type);
      const auto& evname = d.getExternalName(ev.name);
      if (ev.arraySize == 1u) {
        os << "mg.addExternalStateVariable(\"" << evname << "\","
           << as_string(flag) << ", "
           << "0,d->s0.external_state_variables + " << evoffset << ", "
           << "dt,d->s1.external_state_variables + " << evoffset << ", "
           << "false);\n";
        evoffset += SupportedTypes::TypeSize(flag);
      } else {
        if (ev.arraySize >= SupportedTypes::ArraySizeLimit) {
          os << "for(unsigned short i=0;i!=" << ev.arraySize << ";++i){\n";
          os << "const auto name = \"" << evname
             << "[\" +std::to_string(i)+\"]\";\n";
          os << "mg.addExternalStateVariable(name, "  //
             << as_string(flag) << ", "
             << "0,d->s0.external_state_variables + " << evoffset << " + i, "
             << "dt, d->s1.external_state_variables + " << evoffset << " + i, "
             << "false);\n";
          os << "}\n";
          evoffset += SupportedTypes::TypeSize(ev.arraySize, flag);
        } else {
          for (unsigned short i = 0; i != ev.arraySize; ++i) {
            os << "mg.addExternalStateVariable("
               << "\"" << evname << "[" << i << "]\", "  //
               << as_string(flag) << ", "
               << "0, d->s0.external_state_variables + " << evoffset << ", "
               << "dt, d->s1.external_state_variables + " << evoffset << ", "
               << "false);\n";
            evoffset += SupportedTypes::TypeSize(flag);
          }
        }
      }
    }
    os << "mg.generate(\"" + name + "\");\n"
       << "static_cast<void>(TVectorSize); // remove gcc warning\n"
       << "static_cast<void>(StensorSize); // remove gcc warning\n"
       << "static_cast<void>(TensorSize);  // remove gcc warning\n"
       << "} catch(std::exception& mtest_generation_exception){\n"
       << "std::cerr << \"MTest file generation failed: \" << "
       << "mtest_generation_exception.what() << \"\\n\";\n"
       << "} catch(...){\n"
       << "std::cerr << \"MTest file generation failed\\n\";"
       << "}\n";
  }  // end of generateMTestFile

  std::string GenericBehaviourInterface::getLibraryName(
      const BehaviourDescription& bd) const {
    if (bd.getLibrary().empty()) {
      if (!bd.getMaterialName().empty()) {
        return bd.getMaterialName() + "-generic";
      } else {
        return "Behaviour";
      }
    }
    return bd.getLibrary() + "-generic";
  }  // end of getLibraryName

  std::string GenericBehaviourInterface::getFunctionNameBasis(
      const std::string& n) const {
    return n;
  }  // end of getFunctionName

  std::string GenericBehaviourInterface::getFunctionNameForHypothesis(
      const std::string& n, const Hypothesis h) const {
    return n + "_" + ModellingHypothesis::toString(h);
  }  // end of getFunctionNameForHypothesis

  void GenericBehaviourInterface::writeBehaviourConstructorHeader(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis,
      const std::string& initStateVarsIncrements) const {
    os << "/*\n"
       << " * \\brief constructor for the " << this->getInterfaceName()
       << " interface\n"
       << " * \\param[in] mgb_d: behaviour data\n"
       << " */\n"
       << bd.getClassName() << "(const mfront::gb::BehaviourData& mgb_d)\n";
    if (bd.useQt()) {
      os << ": " << bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, use_qt>(mgb_d),\n"
         << bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, use_qt>(mgb_d)";
    } else {
      os << ": " << bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, false>(mgb_d),\n"
         << bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, false>(mgb_d)";
    }
    if (!initStateVarsIncrements.empty()) {
      os << ",\n" << initStateVarsIncrements;
    }
  }  // end of writeBehaviourConstructorHeader

  static void GenericBehaviourInterface_initializeVariable(
      std::ostream& os,
      const SupportedTypes::TypeSize& o,
      const VariableDescription& v,
      const std::string& n,
      const std::string& src) {
    if (v.arraySize == 1) {
      if (v.isScalar()) {
        os << "this->" << n << " = " << src << "[" << o << "];\n";
      } else {
        os << "this-> " << n << " = tfel::math::map<" << v.type << ">(";
        if (!o.isNull()) {
          os << src << "+" << o;
        } else {
          os << src;
        }
        os << ");\n";
      }
    } else {
      auto odv = o;
      for (unsigned short idx = 0; idx != v.arraySize; ++idx) {
        if (v.isScalar()) {
          os << "this->" << n << "[" << idx << "] = " << src << "[" << odv
             << "];\n";
        } else {
          os << "this-> " << n << "[" << idx << "] = tfel::math::map<" << v.type
             << ">(";
          if (!odv.isNull()) {
            os << src << "+" << odv;
          } else {
            os << src;
          }
          os << ");\n";
        }
        odv += SupportedTypes::getTypeSize(v.type, 1u);
      }
    }
  }  // end of GenericBehaviourInterface_initializeVariable

  void GenericBehaviourInterface::writeBehaviourConstructorBody(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const char* msg) {
      if (b) {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourConstructorBody: " +
            std::string(msg));
      }
    };
    // setting driving variables and thermodynamic forces
    const auto type = bd.getBehaviourType();
    auto odv = SupportedTypes::TypeSize{};
    auto oth = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& dv = mv.first;
      const auto& th = mv.second;
      if (th.arraySize != dv.arraySize) {
        tfel::raise("the array size of thermodynamic force '" + th.name +
                    "' is not "
                    "the same as the array size of gradient '" +
                    dv.name + "'");
      }
      const auto s = SupportedTypes::getTypeSize(dv.type, dv.arraySize);
      // driving variable
      const auto dvname =
          Gradient::isIncrementKnown(dv) ? dv.name : dv.name + "0";
      GenericBehaviourInterface_initializeVariable(os, odv, dv, dvname,
                                                   "mgb_d.s0.gradients");
      if (!Gradient::isIncrementKnown(dv)) {
        if (dv.isScalar()) {
          if (bd.useQt()) {
            os << "this->" << dv.name << "1 = " << dv.type
               << "(mgb_d.s1.gradients[" << odv << "]);\n";
          } else {
            os << "this->" << dv.name << "1 = mgb_d.s1.gradients[" << odv
               << "];\n";
          }
        } else {
          if (!odv.isNull()) {
            os << "tfel::fsalgo::copy<" << s << ">::exe("
               << "mgb_d.s1.gradients+" << odv << ","
               << "this->" << dv.name << "1.begin());\n";
          } else {
            os << "tfel::fsalgo::copy<" << s << " >::exe("
               << "mgb_d.s1.gradients,"
               << "this->" << dv.name << "1.begin());\n";
          }
        }
      } else {
        if (dv.arraySize == 1) {
          if (dv.isScalar()) {
            os << "this->d" << dv.name << " = "
               << "mgb_d.s1.gradients[" << odv << "] - "
               << "mgb_d.s0.gradients[" << odv << "];\n";
          } else {
            if (!odv.isNull()) {
              os << "tfel::fsalgo::transform<" << s << ">::exe("
                 << "mgb_d.s1.gradients+" << odv << ","
                 << "mgb_d.s0.gradients+" << odv << ","
                 << "this->d" << dv.name << ".begin(),std::minus<real>());\n";
            } else {
              os << "tfel::fsalgo::transform<" << s << ">::exe("
                 << "mgb_d.s1.gradients,"
                 << "mgb_d.s0.gradients,"
                 << "this->d" << dv.name << ".begin(),std::minus<real>());\n";
            }
          }
        } else {
          auto lodv = odv;  // local offset
          const auto ls = SupportedTypes::getTypeSize(dv.type, 1);
          for (unsigned short idx = 0; idx != dv.arraySize; ++idx) {
            if (dv.isScalar()) {
              os << "this->d" << dv.name << "[" << idx << "] = "
                 << "mgb_d.s1.gradients[" << idx << "] - "
                 << "mgb_d.s0.gradients[" << idx << "];\n";
            } else {
              if (!lodv.isNull()) {
                os << "tfel::fsalgo::transform<" << ls << ">::exe("
                   << "mgb_d.s1.gradients+" << lodv << ","
                   << "mgb_d.s0.gradients+" << lodv << ","
                   << "this->d" << dv.name << "[" << idx
                   << "].begin(),std::minus<real>());\n";
              } else {
                os << "tfel::fsalgo::transform<" << ls << ">::exe("
                   << "mgb_d.s1.gradients,"
                   << "mgb_d.s0.gradients,"
                   << "this->d" << dv.name << "[" << idx
                   << "].begin(),std::minus<real>());\n";
              }
            }
            lodv += SupportedTypes::getTypeSize(dv.type, 1);
          }
        }
      }
      odv += s;
    }
    if (bd.requiresStressFreeExpansionTreatment(h)) {
      os << "std::pair<StressFreeExpansionType,StressFreeExpansionType> "
            "mgb_dl01_l0;\n"
         << "this->computeStressFreeExpansion(mgb_dl01_l0);\n";
      if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          if ((ms == BehaviourDescription::LINEARISED) ||
              (ms == BehaviourDescription::GREENLAGRANGE)) {
            os << "this->eto  -= mgb_dl01_l0.first;\n"
               << "this->deto -= mgb_dl01_l0.second-mgb_dl01_l0.first;\n";
          } else if (ms == BehaviourDescription::HENCKY) {
            os << "this->eto[0]  -= strain(std::log(1+mgb_dl01_l0.first[0]));\n"
               << "this->eto[1]  -= strain(std::log(1+mgb_dl01_l0.first[1]));\n"
               << "this->eto[2]  -= strain(std::log(1+mgb_dl01_l0.first[2]));\n"
               << "this->deto[0] -= "
                  "strain(std::log((1+mgb_dl01_l0.second[0])/"
                  "(1+mgb_dl01_l0.first[0])));\n"
               << "this->deto[1] -= "
                  "strain(std::log((1+mgb_dl01_l0.second[1])/"
                  "(1+mgb_dl01_l0.first[1])));\n"
               << "this->deto[2] -= "
                  "strain(std::log((1+mgb_dl01_l0.second[2])/"
                  "(1+mgb_dl01_l0.first[2])));\n";
          } else {
            throw_if(true, "unsupported finite strain strategy");
          }
        } else {
          os << "this->eto  -= mgb_dl01_l0.first;\n"
             << "this->deto -= mgb_dl01_l0.second-mgb_dl01_l0.first;\n";
        }
      }
    }
  }  // end of writeBehaviourConstructorBody

  void GenericBehaviourInterface::writeBehaviourDataConstructor(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    // write variable initializer
    // this is captured for gcc 4.7.2
    auto wvi = [&os](bool& first, const VariableDescription& v,
                     const std::string& src, const SupportedTypes::TypeSize& o,
                     const std::string& eo) {
      if (v.arraySize != 1u) {
        return;
      }
      os << (first ? "\n: " : ",\n");
      if (v.isScalar()) {
        if (eo.empty()) {
          os << v.name << "(" << src << "[" << o << "])";
        } else {
          os << v.name << "(" << src << "[" << eo << "+" << o << "])";
        }
      } else {
        if (eo.empty()) {
          os << v.name << "(tfel::math::map<" << v.type << ">(" << src << " + "
             << o << "))";
        } else {
          os << v.name << "(tfel::math::map<" << v.type << ">(" << src << " + "
             << eo << " " << o << "))";
        }
      }
      first = false;
    };  // end of wvi
    auto wvci = [&wvi, this](bool& first,
                             const VariableDescriptionContainer& vs,
                             const std::string& src, const std::string& eo) {
      auto o = SupportedTypes::TypeSize{};
      for (const auto& v : vs) {
        wvi(first, v, src, o, eo);
        o += this->getTypeSize(v.type, v.arraySize);
      }
    };  // end of wvci
    // write variable initializer
    // this is captured for gcc 4.7.2
    auto wvi2 = [&os, &bd](const VariableDescription& v, const std::string& src,
                           const SupportedTypes::TypeSize& c,
                           const std::string& eo) {
      auto get_offset = [&eo](const SupportedTypes::TypeSize o) {
        std::ostringstream out;
        out << o;
        if (!eo.empty()) {
          return eo + '+' + out.str();
        }
        return out.str();
      };
      auto o = c;
      if (v.arraySize == 1u) {
        return;
      }
      if (bd.useDynamicallyAllocatedVector(v.arraySize)) {
        os << "this->" << v.name << ".resize(" << v.arraySize << ");\n";
        os << "for(unsigned short idx=0; idx != " << v.arraySize
           << "; ++idx){\n";
        if (v.isScalar()) {
          os << "this->" << v.name << "[idx] = " << v.type << "{" << src << "["
             << get_offset(o) << "+idx]};\n";
        } else {
          os << "this->" << v.name << "[idx] = tfel::math::map<" << v.type
             << ">(" << src << " + " << get_offset(o) << "+idx * "
             << SupportedTypes::getTypeSize(v.type, 1u) << ");\n";
        }
        os << "}\n";
      } else {
        for (unsigned short index = 0; index != v.arraySize; ++index) {
          if (v.isScalar()) {
            os << "this->" << v.name << "[" << index << "] = " << v.type << "{"
               << src << "[" << get_offset(o) << "]};\n";
          } else {
            os << "this->" << v.name << "[" << index << "] = tfel::math::map<"
               << v.type << ">(" << src << " + " << get_offset(o) << ");\n";
          }
          o += SupportedTypes::getTypeSize(v.type, 1u);
        }
      }
    };  // end of wvi2
    auto wvci2 = [&wvi2, this](const VariableDescriptionContainer& vs,
                               const std::string& src, const std::string& eo) {
      auto o = SupportedTypes::TypeSize{};
      for (const auto& v : vs) {
        wvi2(v, src, o, eo);
        o += this->getTypeSize(v.type, v.arraySize);
      }
    };  // end of wvci2
    const auto& d = bd.getBehaviourData(h);
    os << "/*\n"
       << " * \\brief constructor for the " << this->getInterfaceName()
       << " interface\n"
       << " * \\param[in] mgb_d: behaviour data\n"
       << " */\n"
       << bd.getClassName()
       << "BehaviourData(const mfront::gb::BehaviourData& mgb_d)";
    auto first = true;
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    auto ioffset = mprops.second;
    auto mps_offset = std::string{};
    auto emps_offset = std::string{};
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false))) {
      mps_offset =
          "mfront::gb::ThermoElasticMaterialPropertiesNumber<hypothesis,";
      if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        emps_offset = "mfront::gb::ElasticMaterialPropertiesNumber<hypothesis,";
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          mps_offset += "true, true";
          emps_offset += "true";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          mps_offset += "true, false";
          emps_offset += "false";
        } else {
          tfel::raise(
              "GenericBehaviourInterface::writeBehaviourDataConstructor:"
              "unsupported elastic symmetry type");
        }
        emps_offset += ">::value";
      } else {
        mps_offset += "false, false";
      }
      if (bd.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        if (bd.getSymmetryType() == mfront::ISOTROPIC) {
          mps_offset += ", true, true";
        } else if (bd.getSymmetryType() == mfront::ORTHOTROPIC) {
          mps_offset += ", true, false";
        } else {
          tfel::raise(
              "GenericBehaviourInterface::writeBehaviourDataConstructor:"
              "unsupported elastic symmetry type");
        }
      } else {
        mps_offset += ", false, false";
      }
      mps_offset += ">::value";
    }
    for (const auto& mp : d.getMaterialProperties()) {
      const auto& m =
          findBehaviourMaterialProperty(mprops.first, mp.getExternalName());
      auto offset = m.offset;
      offset -= ioffset;
      wvi(first, mp, "mgb_d.s1.material_properties", offset, mps_offset);
    }
    wvci(first, d.getPersistentVariables(), "mgb_d.s0.internal_state_variables",
         "");
    wvci(first, d.getExternalStateVariables(),
         "mgb_d.s0.external_state_variables", "");
    os << "\n{\n";
    //
    auto oth = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& th = mv.second;
      GenericBehaviourInterface_initializeVariable(
          os, oth, th, th.name, "mgb_d.s0.thermodynamic_forces");
      oth += SupportedTypes::getTypeSize(th.type, th.arraySize);
    }
    //
    for (const auto& mp : d.getMaterialProperties()) {
      const auto& m =
          findBehaviourMaterialProperty(mprops.first, mp.getExternalName());
      auto offset = m.offset;
      offset -= ioffset;
      wvi2(mp, "mgb_d.s1.material_properties", offset, mps_offset);
    }
    wvci2(d.getPersistentVariables(), "mgb_d.s0.internal_state_variables", "");
    wvci2(d.getExternalStateVariables(), "mgb_d.s0.external_state_variables",
          "");
    if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        const auto stac = [&bd] {
          if (bd.getAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor,
                  false)) {
            return "StiffnessTensorAlterationCharacteristic::UNALTERED";
          }
          return "StiffnessTensorAlterationCharacteristic::ALTERED";
        }();
        os << "computeIsotropicStiffnessTensor"
           << "<hypothesis," << stac << ",stress,real>(this->D,"
           << "mgb_d.s1.material_properties[0],\n"
           << "mgb_d.s1.material_properties[1]);\n";
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        if (bd.getAttribute(
                BehaviourDescription::requiresUnAlteredStiffnessTensor,
                false)) {
          os << "mfront::gb::computeOrthotropicUnAlteredElasticStiffnessTensor<"
                "hypothesis>(this->D,"
             << "mgb_d.s1.material_properties);\n";
        } else {
          os << "mfront::gb::computeOrthotropicAlteredElasticStiffnessTensor<"
                "hypothesis>(this->D,"
             << "mgb_d.s1.material_properties);\n";
        }
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataConstructor:"
            "unsupported symmetry type");
      }
    }
    if (bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      auto eoffset = std::string{};
      if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        eoffset = "mgb_eoffset";
        os << "constexpr auto mgb_eoffset = " << emps_offset << ";\n";
      }
      if (bd.getSymmetryType() == mfront::ISOTROPIC) {
        if (eoffset.empty()) {
          os << "this->A = mgb_d.s1.material_properties[0] * "
             << "ThermalExpansionCoefficientTensor::Id();\n";
        } else {
          os << "this->A = mgb_d.s1.material_properties[" << eoffset << "] * "
             << "ThermalExpansionCoefficientTensor::Id();\n";
        }
      } else if (bd.getSymmetryType() == mfront::ORTHOTROPIC) {
        os << "this->A = "
              "ThermalExpansionCoefficientTensor(thermalexpansion(0));\n";
        if (eoffset.empty()) {
          os << "this->A[0] = "
                "thermalexpansion(mgb_d.s1.material_properties[0]);\n"
             << "this->A[1] = "
                "thermalexpansion(mgb_d.s1.material_properties[1]);\n"
             << "this->A[2] = "
                "thermalexpansion(mgb_d.s1.material_properties[2]);\n";
        } else {
          os << "this->A[0] = thermalexpansion(mgb_d.s1.material_properties["
             << eoffset << "]);\n"
             << "this->A[1] = thermalexpansion(mgb_d.s1.material_properties["
             << eoffset << "+1]);\n"
             << "this->A[2] = thermalexpansion(mgb_d.s1.material_properties["
             << eoffset << "+2]);\n";
        }
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataConstructor:"
            "unsupported symmetry type");
      }
      os << ";\n";
    }
    os << "}\n\n";
  }  // end of writeBehaviourDataConstructor

  void GenericBehaviourInterface::writeIntegrationDataConstructor(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    const auto& evs = bd.getBehaviourData(h).getExternalStateVariables();
    os << "/*\n"
       << " * \\brief constructor for the " << this->getInterfaceName()
       << " interface\n"
       << " * \\param[in] mgb_d: behaviour data\n"
       << " */\n"
       << bd.getClassName()
       << "IntegrationData(const mfront::gb::BehaviourData& mgb_d)\n"
       << ": dt(mgb_d.dt)";
    auto o = SupportedTypes::TypeSize{};
    for (const auto& ev : evs) {
      if ((ev.arraySize != 1u) || (!ev.isScalar())) {
        o += this->getTypeSize(ev.type, ev.arraySize);
        continue;
      }
      os << ",\nd" << ev.name << "(mgb_d.s1.external_state_variables[" << o
         << "]-"
         << "mgb_d.s0.external_state_variables[" << o << "])";
      o += this->getTypeSize(ev.type, ev.arraySize);
    }
    os << "\n{\n";
    o = SupportedTypes::TypeSize{};
    for (const auto& ev : evs) {
      if ((ev.arraySize == 1u) && (ev.isScalar())) {
        o += this->getTypeSize(ev.type, ev.arraySize);
        continue;
      }
      if (ev.arraySize == 1u) {
        os << "this->d" << ev.name << " = "
           << "tfel::math::map<" << ev.type
           << ">(mgb_d.s1.external_state_variables + " << o << ")"
           << " - tfel::math::map<" << ev.type
           << ">(mgb_d.s0.external_state_variables + " << o << ");\n";
        o += this->getTypeSize(ev.type, ev.arraySize);
      } else {
        if (bd.useDynamicallyAllocatedVector(ev.arraySize)) {
          const auto s = this->getTypeSize(ev.type, 1u);
          os << "this->d" << ev.name << ".resize(" << ev.arraySize << ");\n";
          os << "for(unsigned short idx=0; idx != " << ev.arraySize
             << "; ++idx){\n";
          os << "this->d" << ev.name << "[idx] = "
             << "tfel::math::map<" << ev.type
             << ">(mgb_d.s1.external_state_variables + " << o << " + idx * "
             << s << ")"
             << " - tfel::math::map<" << ev.type
             << ">(mgb_d.s0.external_state_variables + " << o << " + idx * "
             << s << ");\n";
          os << "}\n";
          o += this->getTypeSize(ev.type, ev.arraySize);
        } else {
          for (int index = 0; index != ev.arraySize; ++index) {
            os << "this->d" << ev.name << "[" << index << "] = "
               << "tfel::math::map<" << ev.type
               << ">(mgb_d.s1.external_state_variables + " << o << ") - "
               << "tfel::math::map<" << ev.type
               << ">(mgb_d.s0.external_state_variables + " << o << ");\n";
            o += SupportedTypes::getTypeSize(ev.type, 1u);
          }
        }
      }
    }
    os << "}\n\n";
  }  // end of writeIntegrationDataConstructor

  void GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of writeBehaviourDataMainVariablesSetters

  void GenericBehaviourInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of writeIntegrationDataMainVariablesSetters

  void GenericBehaviourInterface::exportMechanicalData(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    auto export_variable = [this, &bd, &os](const VariableDescription& v,
                                            const char* const dest,
                                            const SupportedTypes::TypeSize o) {
      auto do_export = [&bd, &v, &os, &dest](
                           const SupportedTypes::TypeSize offset,
                           const auto& n) {
        if (v.isScalar()) {
          if (bd.useQt()) {
            os << "mbg_s1." << dest << "[" << offset << "] = "
               << "tfel::math::base_type_cast(" << n << ");\n";
          } else {
            os << "mbg_s1." << dest << "[" << offset << "] = " << n << ";\n";
          }
        } else {
          if (offset.isNull()) {
            os << "tfel::math::map<" << v.type << ">(mbg_s1." << dest
               << ") = " << n << ";\n";
          } else {
            os << "tfel::math::map<" << v.type << ">(mbg_s1." << dest << " + "
               << offset << ") = " << n << ";\n";
          }
        }
      };
      if (v.arraySize == 1u) {
        do_export(o, "this->" + v.name);
      } else {
        auto c = o;
        for (unsigned short idx = 0; idx != v.arraySize; ++idx) {
          do_export(c, "this->" + v.name + "[" + std::to_string(idx) + "]");
          c += this->getTypeSize(v.type, 1u);
        }
      }
    };
    const auto& d = bd.getBehaviourData(h);
    os << "TFEL_HOST_DEVICE void exportStateData(mfront::gb::State& mbg_s1) "
          "const\n";
    os << "{\n";
    os << "using namespace tfel::math;\n";
    auto o = SupportedTypes::TypeSize{};
    for (const auto& v : bd.getMainVariables()) {
      const auto& th = v.second;
      export_variable(th, "thermodynamic_forces", o);
      o += SupportedTypes::getTypeSize(th.type, th.arraySize);
    }
    o = SupportedTypes::TypeSize{};
    for (const auto& iv : d.getPersistentVariables()) {
      export_variable(iv, "internal_state_variables", o);
      o += SupportedTypes::getTypeSize(iv.type, iv.arraySize);
    }
    os << "} // end of exportStateData\n\n";
  }  // end of exportMechanicalData

  void GenericBehaviourInterface::writeBehaviourInitializeFunctions(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto initializeVariablesFromArrayOfValues =
        [&os](const std::vector<VariableDescription>& variables) {
          auto o = SupportedTypes::TypeSize{};
          for (const auto& v : variables) {
            if (v.arraySize == 1u) {
              if (v.getTypeFlag() == SupportedTypes::SCALAR) {
                os << "auto&& " << v.name;
              } else {
                os << "const auto " << v.name;
              }
              os << " = tfel::math::map<" << v.type
                 << ">(mfront_initialize_function_inputs + " << o << ");\n";
            } else {
              os << "const auto " << v.name
                 << " = tfel::math::map_array<const tfel::math::fsarray<"
                 << v.arraySize << "," << v.type
                 << ">>(mfront_initialize_function_inputs + " << o << ");\n";
            }
            o += v.getTypeSize();
          }
        };
    const auto& d = bd.getBehaviourData(h);
    for (const auto& [n, c] : d.getInitializeFunctions()) {
      const auto& initialize_function_variables =
          c.attributes.at(CodeBlock::used_initialize_function_variables)
              .get<std::vector<VariableDescription>>();
      os << "void execute" << n
         << "InitializeFunction(const NumericType* const ";
      if (!initialize_function_variables.empty()) {
        os << "mfront_initialize_function_inputs){\n";
      } else {
        os << "){\n";
      }
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using namespace tfel::material;\n";
      writeMaterialLaws(os, bd.getMaterialLaws());
      if (!initialize_function_variables.empty()) {
        initializeVariablesFromArrayOfValues(initialize_function_variables);
      }
      os << c.code << '\n'
         << "} // end of execute" << n << "InitializeFunction\n\n";
    }
  }  // end of writeBehaviourInitializeFunctions

  void GenericBehaviourInterface::writeBehaviourPostProcessings(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto initializeVariablesFromArrayOfValues =
        [&os](const std::vector<VariableDescription>& variables) {
          auto o = SupportedTypes::TypeSize{};
          for (const auto& v : variables) {
            if (v.arraySize == 1u) {
              if (v.getTypeFlag() == SupportedTypes::SCALAR) {
                os << "auto&& " << v.name;
              } else {
                os << "auto " << v.name;
              }
              os << " = tfel::math::map<" << v.type
                 << ">(mfront_postprocessing_outputs + " << o << ");\n";
            } else {
              os << "auto " << v.name
                 << " = tfel::math::map_array<tfel::math::fsarray<"
                 << v.arraySize << "," << v.type
                 << ">>(mfront_postprocessing_outputs + " << o << ");\n";
            }
            o += v.getTypeSize();
          }
        };
    const auto& d = bd.getBehaviourData(h);
    for (const auto& [n, c] : d.getPostProcessings()) {
      const auto& postprocessing_variables =
          c.attributes.at(CodeBlock::used_postprocessing_variables)
              .get<std::vector<VariableDescription>>();
      os << "void execute" << n << "PostProcessing(NumericType* const "
         << "mfront_postprocessing_outputs,\n"
         << "const BehaviourData& initial_state){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using namespace tfel::material;\n"
         << "static_cast<void>(initial_state);\n";
      writeMaterialLaws(os, bd.getMaterialLaws());
      initializeVariablesFromArrayOfValues(postprocessing_variables);
      os << c.code << '\n'
         << "} // end of execute" << n << "PostProcessing\n\n";
    }
  }  // end of writeBehaviourPostProcessings

  GenericBehaviourInterface::~GenericBehaviourInterface() = default;

}  // end of namespace mfront
