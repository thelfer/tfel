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
    if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::SCALAR) {
    } else if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVECTOR) {
      os << "std::cerr << \"rotation of vector is unsupported\";\n"
         << "std::exit(-1);\n";
    } else if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::STENSOR) {
      writeStensorRotation(args);
    } else if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TENSOR) {
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
    auto idx = size_type{};
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
        if (SupportedTypes::getTypeFlag(v1.type) == SupportedTypes::SCALAR) {
          if (SupportedTypes::getTypeFlag(v2.type) == SupportedTypes::SCALAR) {
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::TVECTOR) {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported tangent operator type\";\n"
               << "std::exit(-1);\n";
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::STENSOR) {
            writeStensorRotation(args);
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::TENSOR) {
            writeTensorRotation(args);
          } else {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported gradient type\";\n"
               << "std::exit(-1);\n";
          }
        } else if (SupportedTypes::getTypeFlag(v1.type) ==
                   SupportedTypes::TVECTOR) {
          os << "std::cerr << \"" << f << ": \"\n"
             << "          << \"unsupported tangent operator type\";\n"
             << "std::exit(-1);\n";
        } else if (SupportedTypes::getTypeFlag(v1.type) ==
                   SupportedTypes::STENSOR) {
          if (SupportedTypes::getTypeFlag(v2.type) == SupportedTypes::SCALAR) {
            writeStensorRotation(args);
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::TVECTOR) {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported tangent operator type\";\n"
               << "std::exit(-1);\n";
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::STENSOR) {
            writeST2toST2Rotation(args);
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::TENSOR) {
            writeT2toST2Rotation(args);
          } else {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported gradient type\";\n"
               << "std::exit(-1);\n";
          }
        } else if (SupportedTypes::getTypeFlag(v1.type) ==
                   SupportedTypes::TENSOR) {
          if (SupportedTypes::getTypeFlag(v2.type) == SupportedTypes::SCALAR) {
            writeTensorRotation(args);
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::TVECTOR) {
            os << "std::cerr << \"" << f << ": \"\n"
               << "          << \"unsupported tangent operator type\";\n"
               << "std::exit(-1);\n";
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::STENSOR) {
            writeST2toT2Rotation(args);
          } else if (SupportedTypes::getTypeFlag(v2.type) ==
                     SupportedTypes::TENSOR) {
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
      ++idx;
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
  }  // end of GenericBehaviourInterface::getInterfaceName

  std::pair<bool, GenericBehaviourInterface::tokens_iterator>
  GenericBehaviourInterface::treatKeyword(BehaviourDescription& bd,
                                          const std::string& k,
                                          const std::vector<std::string>& i,
                                          tokens_iterator current,
                                          const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
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
    return {false, current};
  }  // end of GenericBehaviourInterface::treatKeyword

  std::set<GenericBehaviourInterface::Hypothesis>
  GenericBehaviourInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& bd) const {
    return bd.getModellingHypotheses();
  }  // end of GenericBehaviourInterface::getModellingHypothesesToBeTreated

  void GenericBehaviourInterface::writeInterfaceSpecificIncludes(
      std::ostream& os, const BehaviourDescription& bd) const {
    os << "#include \"MFront/GenericBehaviour/State.hxx\"\n"
       << "#include \"MFront/GenericBehaviour/BehaviourData.hxx\"\n";
    if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      os << "#include \"MFront/GenericBehaviour/ComputeStiffnessTensor.hxx\"\n";
    }
  }  // end of GenericBehaviourInterface::writeInterfaceSpecificIncludes

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
#if __cplusplus >= 201703L
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
#else  /* __cplusplus < 201703L */
    insert_if(l.link_libraries,
              "$(shell " + tfel_config +
                  " --library-dependency "
                  "--material --mfront-profiling --physical-constants)");
#endif /* __cplusplus < 201703L */
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(l.epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of GenericBehaviourInterface::getTargetsDescription

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
    const auto is_finite_strain_through_strain_measure =
        (bd.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
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
      const auto f = this->getFunctionNameForHypothesis(name, h);
      out << "/*!\n"
          << " * \\param[in,out] d: material data\n"
          << " */\n"
          << "MFRONT_SHAREDOBJ int " << f
          << "(mfront_gb_BehaviourData* const);\n\n";
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
        << "#include\"TFEL/Math/Tensor/TensorView.hxx\"\n"
        << "#include\"TFEL/Math/Stensor/StensorView.hxx\"\n";
    out << "#include\"TFEL/Math/Stensor/StensorView.hxx\"\n"
        << "#include\"TFEL/Math/Tensor/TensorView.hxx\"\n"
        << "#include\"TFEL/Math/t2tot2.hxx\"\n"
        << "#include\"TFEL/Math/T2toT2/T2toT2View.hxx\"\n"
        << "#include\"TFEL/Math/t2tost2.hxx\"\n"
        << "#include\"TFEL/Math/T2toST2/T2toST2View.hxx\"\n";
    if (is_finite_strain_through_strain_measure) {
      out << "#include\"TFEL/Material/"
             "FiniteStrainBehaviourTangentOperator.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << bd.getClassName() << ".hxx\"\n";
    if (bd.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          if (ms == BehaviourDescription::LINEARISED) {
            out << "#include "
                << "\"MFront/"
                   "GenericBehaviourSmallStrainMTestFileGenerator.hxx\"\n";
          } else {
            out << "#include "
                << "\"MFront/"
                   "GenericBehaviourFiniteStrainMTestFileGenerator.hxx\"\n";
          }
        } else {
          out << "#include "
              << "\"MFront/"
                 "GenericBehaviourSmallStrainMTestFileGenerator.hxx\"\n";
        }
      } else if (bd.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "#include "
            << "\"MFront/"
               "GenericBehaviourFiniteStrainMTestFileGenerator.hxx\"\n";
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeInterfaceSpecificIncludes: "
            "unsupported behaviour type for MTest file generation");
      }
    }
    out << "#include\"MFront/GenericBehaviour/"
           "Integrate.hxx\"\n\n";
    out << "#include\"MFront/GenericBehaviour/" << header << "\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

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
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);
    // parameters
    this->writeSetParametersFunctionsImplementations(out, bd, name);
    // behaviour implementations
    const auto type = bd.getBehaviourType();
    for (const auto h : mhs) {
      const auto f = this->getFunctionNameForHypothesis(name, h);
      out << "MFRONT_SHAREDOBJ int " << f
          << "(mfront_gb_BehaviourData* const d){\n"
          << "using namespace tfel::material;\n";
      if ((type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
          (is_finite_strain_through_strain_measure)) {
        out << "using TangentOperator = "
               "FiniteStrainBehaviourTangentOperatorBase;\n";
      }
      out << "using real = mfront::gb::real;\n"
          << "constexpr const auto h = ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n"
          << "using Behaviour = " << bd.getClassName() << "<h,real,false>;\n";
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
      // choice of the tangent operator
      if ((type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
          (is_finite_strain_through_strain_measure)) {
        out << "// stress measure \n"
            << "enum struct StressMeasure { PK1, PK2, CAUCHY };\n"
            << "const auto sm = [&d]{\n"
            << "  if(d->K[1]<0.5){\n"
            << "    return StressMeasure::CAUCHY;\n"
            << "  } else if (d->K[1]<1.5){\n"
            << "    return StressMeasure::PK2;\n"
            << "  } else if (d->K[1]<2.5){\n"
            << "    return StressMeasure::PK1;\n"
            << "  } else {\n"
            << "  std::cerr << \"invalid choice for the \"\n"
            << "               \"stress measure\";\n"
            << "  std::exit(-1);\n"
            << "  }\n"
            << "}();\n"
            << "// stiffness type\n"
            << "const auto smf = [&d]{\n"
            << "  if((d->K[0]>-0.5)&&(d->K[0]<0.5)){\n"
            << "    // no stiffness requested, \n"
            << "    // returned value is meaningless\n"
            << "    return TangentOperator::DSIG_DF;\n"
            << "  }\n"
            << "  if(d->K[2]<0.5){\n"
            << "    return TangentOperator::DSIG_DF;\n"
            << "  } else if (d->K[2]<1.5){\n"
            << "    return TangentOperator::DS_DEGL;\n"
            << "  } else if (d->K[2]<2.5){\n"
            << "    return TangentOperator::DPK1_DF;\n"
            << "  } else {\n"
            << "  std::cerr << \"invalid choice for consistent tangent \"\n"
               "               \"operator\\n\";\n"
            << "  std::exit(-1);\n"
            << "  }\n"
            << "}();\n";
      }
      if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        this->writeStandardFiniteStrainBehaviourPreProcessing(out, bd, h);
      }
      // treating strain measures
      if (is_finite_strain_through_strain_measure) {
        const auto ms = bd.getStrainMeasure();
        if (ms == BehaviourDescription::GREENLAGRANGE) {
          this->writeGreenLagrangeStrainMeasurePreProcessing(out, bd, h);
        } else if (ms == BehaviourDescription::HENCKY) {
          this->writeHenckyStrainMeasurePreProcessing(out, bd, h);
        } else {
          raise("unsupported strain measure");
        }
      }
      // behaviour integration
      out << "const auto r = mfront::gb::integrate<Behaviour>(*d,";
      if (type == BehaviourDescription::GENERALBEHAVIOUR) {
        out << "Behaviour::STANDARDTANGENTOPERATOR";
      } else if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << "Behaviour::STANDARDTANGENTOPERATOR";
      } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "smf";
      } else if (type == BehaviourDescription::COHESIVEZONEMODEL) {
        out << "Behaviour::STANDARDTANGENTOPERATOR";
      } else {
        raise("unsupported behaviour type");
      }
      out << ", " << name << "_getOutOfBoundsPolicy());\n";
      // treating strain measures
      if (is_finite_strain_through_strain_measure) {
        // post-processing
        const auto ms = bd.getStrainMeasure();
        if (ms == BehaviourDescription::GREENLAGRANGE) {
          this->writeGreenLagrangeStrainMeasurePostProcessing(out, bd, h);
        } else if (ms == BehaviourDescription::HENCKY) {
          this->writeHenckyStrainMeasurePostProcessing(out, bd, h);
        } else {
          raise("unsupported strain measure");
        }
      }
      if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        this->writeStandardFiniteStrainBehaviourPostProcessing(out, bd, h);
      }
      if (this->shallGenerateMTestFileOnFailure(bd)) {
        out << "if(r!=1){\n";
        this->generateMTestFile(out, bd, h);
        out << "}\n";
      }
      out << "return r;\n"
          << "} // end of " << f << "\n\n";
    }
    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";
    out.close();
  }  // end of GenericBehaviourInterface::endTreatment

  void GenericBehaviourInterface::generateMTestFile(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    const auto fs = [&bd] {
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          return ms != BehaviourDescription::LINEARISED;
        } else {
          return false;
        }
      } else if (bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        tfel::raise(
            "GenericBehaviourInterface::writeInterfaceSpecificIncludes: "
            "unsupported behaviour type for MTest file generation");
      }
      return true;
    }();
    unsigned int offset = 0;
    const auto name = bd.getLibrary() + bd.getClassName();
    out << "try{\n";
    if (fs) {
      out << "mfront::GenericBehaviourFiniteStrainMTestFileGenerator mg(\""
          << this->getLibraryName(bd) << "\",\"" << name << "\");\n";
    } else {
      out << "mfront::GenericBehaviourSmallStrainMTestFileGenerator mg(\""
          << this->getLibraryName(bd) << "\",\"" << name << "\");\n";
    }
    out << "mg.setModellingHypothesis(tfel::material::ModellingHypothesis:"
           ":"
        << tfel::material::ModellingHypothesis::toUpperCaseString(h) << ");\n"
        << "// must be declared after setting the hypothesis\n"
        << "const auto TVectorSize = mg.getTVectorSize();\n"
        << "const auto StensorSize = mg.getStensorSize();\n"
        << "const auto TensorSize  = mg.getTensorSize();\n"
        << "const auto dt = std::max(d->dt,1.e-50);\n"
        << "mg.setHandleThermalExpansion(false);\n"
        << "mg.addTime(0.);\n"
        << "mg.addTime(dt);\n";
    if (fs) {
      out << "mg.setDeformationGradientTensorAtTheBeginningOfTheTimeStep(d->"
             "s0.gradients);\n"
          << "mg.setDeformationGradientTensorAtTheEndOfTheTimeStep(d->s1."
             "gradients);\n"
          << "mg.setStressTensor(d->s0.thermodynamic_forces);\n";
    } else {
      out << "mg.setStrainTensorAtTheBeginningOfTheTimeStep(d->"
             "s0.gradients);\n"
          << "mg.setStrainTensorAtTheEndOfTheTimeStep(d->s1."
             "gradients);\n"
          << "mg.setStressTensor(d->s0.thermodynamic_forces);\n";
    }
    for (const auto& m : mprops.first) {
      const auto flag = SupportedTypes::getTypeFlag(m.type);
      tfel::raise_if(flag != SupportedTypes::SCALAR,
                     "GenericBehaviourInterface::generateMTestFile: "
                     "unsupported external state variable type "
                     "in mtest file generation");
      if (m.arraySize == 1u) {
        if (offset == 0) {
          out << "mg.addMaterialProperty(\"" << m.name
              << "\",*(d->s1.material_properties));\n";
        } else {
          out << "mg.addMaterialProperty(\"" << m.name
              << "\",*(d->s1.material_properties+" << offset << "));\n";
        }
        ++offset;
      } else {
        for (unsigned short s = 0; s != m.arraySize; ++s, ++offset) {
          if (offset == 0) {
            out << "mg.addMaterialProperty(\"" << m.name << "[" << s
                << "]\",*(d->s1.material_properties));\n";
          } else {
            out << "mg.addMaterialProperty(\"" << m.name << "[" << s << "]\","
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
        if (flag == SupportedTypes::SCALAR) {
          out << "mg.addInternalStateVariable(\"" << ivname
              << "\",SupportedTypes::SCALAR,&(d->s0.internal_state_variables["
              << ivoffset << "]));\n";
          ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
        } else {
          out << "mg.addInternalStateVariable(\"" << ivname
              << "\",SupportedTypes::STENSOR,&(d->s0.internal_state_variables["
              << ivoffset << "]));\n";
          ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
        }
      } else {
        if (v.arraySize >= SupportedTypes::ArraySizeLimit) {
          out << "for(unsigned short i=0;i!=" << v.arraySize << ";++i){\n";
          out << "auto name =  \"" << ivname
              << "[\" + std::to_string(i)+ \"]\";\n";
          if (flag == SupportedTypes::SCALAR) {
            out << "mg.addInternalStateVariable(name,SupportedTypes::SCALAR,&"
                   "d->s0.internal_state_variables["
                << ivoffset << "]+i);\n";
          } else {
            out << "mg.addInternalStateVariable(name,SupportedTypes::STENSOR,"
                   "&"
                   "d->s0.internal_state_variables["
                << ivoffset << "]+i);\n";
          }
          out << "}\n";
          if (flag == SupportedTypes::SCALAR) {
            ivoffset += SupportedTypes::TypeSize(v.arraySize, 0u, 0u, 0u);
          } else {
            ivoffset += SupportedTypes::TypeSize(0u, 0u, v.arraySize, 0u);
          }
        } else {
          for (unsigned short i = 0; i != v.arraySize; ++i) {
            if (flag == SupportedTypes::SCALAR) {
              out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                  << "]\",SupportedTypes::SCALAR,&(d->s0.internal_state_"
                     "variables["
                  << ivoffset << "]));\n";
              ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
            } else {
              out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                  << "]\",SupportedTypes::STENSOR,&(d->s0.internal_state_"
                     "variables["
                  << ivoffset << "]));\n";
              ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
            }
          }
        }
      }
    }
    auto p = externalStateVarsHolder.begin();
    for (offset = 0; p != externalStateVarsHolder.end(); ++p) {
      auto flag = SupportedTypes::getTypeFlag(p->type);
      tfel::raise_if(flag != SupportedTypes::SCALAR,
                     "GenericBehaviourInterface::generateMTestFile: "
                     "unsupported external state variable type "
                     "in mtest file generation");
      const auto& evname = d.getExternalName(p->name);
      if (p->arraySize == 1u) {
        if (offset == 0) {
          out << "mg.addExternalStateVariableValue(\"" << evname
              << "\",0,*(d->s0.external_state_variables));\n"
              << "mg.addExternalStateVariableValue(\"" << evname
              << "\",dt,*(d->s1.external_state_variables));\n";
        } else {
          out << "mg.addExternalStateVariableValue(\"" << evname
              << "\",0,*(d->s0.external_state_variables+" << offset << "));\n"
              << "mg.addExternalStateVariableValue(\"" << evname
              << "\",dt,*(d->s1.external_state_variables+" << offset << "));\n";
        }
        ++offset;
      } else {
        if (p->arraySize >= SupportedTypes::ArraySizeLimit) {
          out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
          out << "auto name = \"" << evname
              << "[\" +std::to_string(i)+\"]\";\n";
          if (offset == 0) {
            out << "mg.addExternalStateVariableValue(name,0,*(d->s0.external_"
                   "state_variables+i));\n"
                   "mg.addExternalStateVariableValue(name,dt,*(d->s1."
                   "external_state_variables+i));\n";
          } else {
            out << "mg.addExternalStateVariableValue(name,"
                   "0,*(d->s0.external_state_variables+"
                << offset << "+i));\n";
            out << "mg.addExternalStateVariableValue(name,"
                   "dt,*(d->s1.external_state_variables+"
                << offset << "+i));\n";
          }
          out << "}\n";
          offset += p->arraySize;
        } else {
          for (unsigned short i = 0; i != p->arraySize; ++i, ++offset) {
            if (offset == 0) {
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\",0,*(d->s0.external_state_variables));\n"
                  << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\",dt,*(d->s1.external_state_variables));\n";
            } else {
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\","
                     "0,*(d->s0.external_state_variables+"
                  << offset << "));\n"
                  << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\","
                     "dt,*(d->s1.external_state_variables+"
                  << offset << "))\n";
            }
          }
        }
      }
    }
    out << "mg.generate(\"" + name + "\");\n"
        << "static_cast<void>(TVectorSize); // remove gcc warning\n"
        << "static_cast<void>(StensorSize); // remove gcc warning\n"
        << "static_cast<void>(TensorSize);  // remove gcc warning\n"
        << "} catch(std::exception& mtest_generation_exception){\n"
        << "std::cerr << \"MTest file generation failed: \" << "
        << "mtest_generation_exception.what() << \"\\n\";\n"
        << "} catch(...){\n"
        << "std::cerr << \"MTest file generation failed\\n\";"
        << "}\n";
  }  // end of GenericBehaviourInterface::generateMTestFile

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
  }  // end of GenericBehaviourInterface::getLibraryName

  std::string GenericBehaviourInterface::getFunctionNameBasis(
      const std::string& n) const {
    return n;
  }  // end of GenericBehaviourInterface::getFunctionName

  std::string GenericBehaviourInterface::getFunctionNameForHypothesis(
      const std::string& n, const Hypothesis h) const {
    return n + "_" + ModellingHypothesis::toString(h);
  }  // end of GenericBehaviourInterface::getFunctionNameForHypothesis

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
         << "BehaviourData<hypothesis,Type,use_qt>(mgb_d),\n"
         << bd.getClassName()
         << "IntegrationData<hypothesis,Type,use_qt>(mgb_d)";
    } else {
      os << ": " << bd.getClassName()
         << "BehaviourData<hypothesis,Type,false>(mgb_d),\n"
         << bd.getClassName()
         << "IntegrationData<hypothesis,Type,false>(mgb_d)";
    }
    if (!initStateVarsIncrements.empty()) {
      os << ",\n" << initStateVarsIncrements;
    }
  }  // end of GenericBehaviourInterface::writeBehaviourConstructorHeader

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
    auto vsize = [&throw_if](const SupportedTypes::TypeFlag f) {
      if (f == SupportedTypes::TVECTOR) {
        return "TVectorSize";
      } else if (f == SupportedTypes::STENSOR) {
        return "StensorSize";
      } else if (f != SupportedTypes::TENSOR) {
        throw_if(true, "invalid variable type");
      }
      return "TensorSize";
    };
    auto init = [&os, &vsize, this](
                    SupportedTypes::TypeSize& o, const VariableDescription& v,
                    const std::string& n, const std::string& src) {
      const auto f = this->getTypeFlag(v.type);
      if (f == SupportedTypes::SCALAR) {
        os << "this->" << n << " = mgb_d." << src << "[" << o << "];\n";
      } else {
        const auto s = vsize(f);
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<" << s << ">::exe(mgb_d." << src << "+" << o
             << ","
             << "this->" << n << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<" << s << " >::exe(mgb_d." << src
             << ",this->" << n << ".begin());\n";
        }
      }
    };  // end of init
    auto odv = SupportedTypes::TypeSize{};
    auto oth = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& dv = mv.first;
      const auto& th = mv.second;
      throw_if(dv.arraySize != 1, "arrays of gradients are not supported");
      throw_if(th.arraySize != 1,
               "arrays of thermodynamic forces are not supported");
      // driving variable
      const auto dvname =
          Gradient::isIncrementKnown(dv) ? dv.name : dv.name + "0";
      init(odv, dv, dvname, "s0.gradients");
      if (!Gradient::isIncrementKnown(dv)) {
        const auto f = this->getTypeFlag(dv.type);
        if (f == SupportedTypes::SCALAR) {
          os << "this->" << dv.name << "1 = mgb_d.s1.gradients[" << odv
             << "];\n";
        } else {
          const auto s = vsize(f);
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
        const auto f = this->getTypeFlag(dv.type);
        if (f == SupportedTypes::SCALAR) {
          os << "this->d" << dv.name << " = "
             << "mgb_d.s1.gradients[" << odv << "] - "
             << "mgb_d.s0.gradients[" << odv << "];\n";
        } else {
          const auto s = vsize(f);
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
      }
      init(oth, th, th.name, "s0.thermodynamic_forces");
      odv += SupportedTypes::getTypeSize(dv.type, dv.arraySize);
      oth += SupportedTypes::getTypeSize(th.type, th.arraySize);
    }
    if (bd.requiresStressFreeExpansionTreatment(h)) {
      os << "std::pair<StressFreeExpansionType,StressFreeExpansionType> "
            "mgb_dl01_l0;\n"
         << "this->computeStressFreeExpansion(mgb_dl01_l0);\n";
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          if ((ms == BehaviourDescription::LINEARISED) ||
              (ms == BehaviourDescription::GREENLAGRANGE)) {
            os << "this->eto  -= mgb_dl01_l0.first;\n"
               << "this->deto -= mgb_dl01_l0.second-mgb_dl01_l0.first;\n";
          } else if (ms == BehaviourDescription::HENCKY) {
            os << "this->eto[0]  -= std::log(1+mgb_dl01_l0.first[0]);\n"
               << "this->eto[1]  -= std::log(1+mgb_dl01_l0.first[1]);\n"
               << "this->eto[2]  -= std::log(1+mgb_dl01_l0.first[2]);\n"
               << "this->deto[0] -= "
                  "std::log((1+mgb_dl01_l0.second[0])/"
                  "(1+mgb_dl01_l0.first[0]));\n"
               << "this->deto[1] -= "
                  "std::log((1+mgb_dl01_l0.second[1])/"
                  "(1+mgb_dl01_l0.first[1]));\n"
               << "this->deto[2] -= "
                  "std::log((1+mgb_dl01_l0.second[2])/"
                  "(1+mgb_dl01_l0.first[2]));\n";
          } else {
            throw_if(true, "unsupported finite strain strategy");
          }
        } else {
          os << "this->eto  -= mgb_dl01_l0.first;\n"
             << "this->deto -= mgb_dl01_l0.second-mgb_dl01_l0.first;\n";
        }
      }
    }
  }  // end of GenericBehaviourInterface::writeBehaviourConstructorBody

  void GenericBehaviourInterface::writeBehaviourDataConstructor(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    // write variable initializer
    // this is captured for gcc 4.7.2
    auto wvi = [&os, this](bool& first, const VariableDescription& v,
                           const std::string& src,
                           const SupportedTypes::TypeSize& o,
                           const std::string& eo) {
      if (v.arraySize != 1u) {
        return;
      }
      os << (first ? "\n: " : ",\n");
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if (flag == SupportedTypes::SCALAR) {
        if (eo.empty()) {
          os << v.name << "(" << src << "[" << o << "])";
        } else {
          os << v.name << "(" << src << "[" << eo << "+" << o << "])";
        }
      } else if ((flag == SupportedTypes::TVECTOR) ||
                 (flag == SupportedTypes::STENSOR) ||
                 (flag == SupportedTypes::TENSOR)) {
        if (eo.empty()) {
          os << v.name << "(&" << src << "[" << o << "])";
        } else {
          os << v.name << "(&" << src << "[" << eo << " " << o << "])";
        }
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataConstructor: "
            "internal error, tag unsupported");
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
    auto wvi2 = [&os, &bd, this](
                    const VariableDescription& v, const std::string& src,
                    const SupportedTypes::TypeSize& c, const std::string& eo) {
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
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if (bd.useDynamicallyAllocatedVector(v.arraySize)) {
        os << "this->" << v.name << ".resize(" << v.arraySize << ");\n";
        os << "for(unsigned short idx=0;idx!=" << v.arraySize << ";++idx){\n";
        switch (flag) {
          case SupportedTypes::SCALAR:
            os << "this->" << v.name << "[idx] = " << src << "["
               << get_offset(o) << "+idx];\n";
            break;
          case SupportedTypes::TVECTOR:
            os << "tfel::fsalgo::copy<TVectorSize>::exe(&" << src << "["
               << get_offset(o) << "+idx*TVectorSize],this->" << v.name
               << "[idx].begin());\n";
            break;
          case SupportedTypes::STENSOR:
            os << "this->" << v.name << "[idx].import(&" << src << "["
               << get_offset(o) << "+idx*StensorSize]);\n";
            break;
          case SupportedTypes::TENSOR:
            os << "tfel::fsalgo::copy<TensorSize>::exe(&" << src << "["
               << get_offset(o) << "+idx*TensorSize],this->" << v.name
               << "[idx].begin());\n";
            break;
          default:
            tfel::raise(
                "GenericBehaviourInterface::writeBehaviourDataConstructor: "
                "internal error, tag unsupported");
        }
        os << "}\n";
      } else {
        for (int index = 0; index != v.arraySize; ++index) {
          switch (flag) {
            case SupportedTypes::SCALAR:
              os << "this->" << v.name << "[" << index << "] = " << src << "["
                 << get_offset(o) << "];\n";
              break;
            case SupportedTypes::TVECTOR:
              os << "tfel::fsalgo::copy<TVectorSize>::exe(&" << src << "["
                 << get_offset(o) << "],this->" << v.name << "[" << index
                 << "].begin());\n";
              break;
            case SupportedTypes::STENSOR:
              os << "this->" << v.name << "[" << index << "].import(&" << src
                 << "[" << get_offset(o) << "]);\n";
              break;
            case SupportedTypes::TENSOR:
              os << "tfel::fsalgo::copy<TensorSize>::exe(&" << src << "["
                 << get_offset(o) << "],"
                 << "this->" << v.name << "[" << index << "].begin());\n";
              break;
            default:
              tfel::raise(
                  "GenericBehaviourInterface::writeBehaviourDataConstructor: "
                  "internal error, tag unsupported");
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
        os << "constexpr const auto mgb_eoffset = " << emps_offset << ";\n";
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
  }  // end of GenericBehaviourInterface::writeBehaviourDataConstructor

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
      if (ev.arraySize != 1u) {
        o += this->getTypeSize(ev.type, ev.arraySize);
        continue;
      }
      if (SupportedTypes::getTypeFlag(ev.type) != SupportedTypes::SCALAR) {
        tfel::raise(
            "GenericBehaviourInterface::writeIntegrationDataConstructor: "
            "internal error, tag unsupported");
      }
      os << ",\nd" << ev.name << "(mgb_d.s1.external_state_variables[" << o
         << "]-"
         << "mgb_d.s0.external_state_variables[" << o << "])";
      o += this->getTypeSize(ev.type, ev.arraySize);
    }
    os << "\n{\n";
    o = SupportedTypes::TypeSize{};
    for (const auto& ev : evs) {
      if (ev.arraySize == 1u) {
        o += this->getTypeSize(ev.type, ev.arraySize);
        continue;
      }
      if (SupportedTypes::getTypeFlag(ev.type) != SupportedTypes::SCALAR) {
        tfel::raise(
            "GenericBehaviourInterface::writeIntegrationDataConstructor: "
            "internal error, tag unsupported");
      }
      if (bd.useDynamicallyAllocatedVector(ev.arraySize)) {
        os << "this->d" << ev.name << ".resize(" << ev.arraySize << ");\n";
        os << "for(unsigned short idx=0;idx!=" << ev.arraySize << ";++idx){\n";
        os << "this->d" << ev.name << "[idx] = "
           << "mgb_d.s1.external_state_variables[" << o << "+idx] - "
           << "mgb_d.s0.external_state_variables[" << o << "+idx];\n";
        os << "}\n";
        o += this->getTypeSize(ev.type, ev.arraySize);
      } else {
        for (int index = 0; index != ev.arraySize; ++index) {
          os << "this->d" << ev.name << "[" << index << "] = "
             << "mgb_d.s1.external_state_variables[" << o << "] - "
             << "mgb_d.s0.external_state_variables[" << o << "];\n";
          o += SupportedTypes::getTypeSize(ev.type, 1u);
        }
      }
    }
    os << "}\n\n";
  }  // end of GenericBehaviourInterface::writeIntegrationDataConstructor

  void GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters

  void GenericBehaviourInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {}  // end of
  // GenericBehaviourInterface::writeIntegrationDataMainVariablesSetters

  void GenericBehaviourInterface::exportMechanicalData(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    auto vsize = [](const SupportedTypes::TypeFlag f) {
      if (f == SupportedTypes::TVECTOR) {
        return "TVectorSize";
      } else if (f == SupportedTypes::STENSOR) {
        return "StensorSize";
      } else if (f != SupportedTypes::TENSOR) {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters:"
            " "
            "invalid variable type");
      }
      return "TensorSize";
    };
    auto export_variable = [this, &os, &vsize](
                               const VariableDescription& v,
                               const char* const dest,
                               const SupportedTypes::TypeSize o) {
      const auto f = this->getTypeFlag(v.type);
      if (v.arraySize == 1u) {
        if (f == SupportedTypes::SCALAR) {
          os << "mbg_s1." << dest << "[" << o << "] "
             << "= this->" << v.name << ";\n";
        } else {
          const auto s = vsize(f);
          if (o.isNull()) {
            os << "tfel::fsalgo::copy<" << s << ">::exe("
               << "this->" << v.name << ".begin(), "
               << "mbg_s1." << dest << ");\n";
          } else {
            os << "tfel::fsalgo::copy<" << s << ">::exe("
               << "this->" << v.name << ".begin(), "
               << "mbg_s1." << dest << " + " << o << ");\n";
          }
        }
      } else {
        auto c = o;
        for (unsigned short idx = 0; idx != v.arraySize; ++idx) {
          if (f == SupportedTypes::SCALAR) {
            os << "mbg_s1." << dest << "[" << c << "] "
               << "= this->" << v.name << "[" << idx << "];\n";
          } else {
            const auto s = vsize(f);
            if (c.isNull()) {
              os << "tfel::fsalgo::copy<" << s << ">::exe("
                 << "this->" << v.name << "[" << idx << "].begin(), "
                 << "mbg_s1." << dest << ");\n";
            } else {
              os << "tfel::fsalgo::copy<" << s << ">::exe("
                 << "this->" << v.name << "[" << idx << "].begin(), "
                 << "mbg_s1." << dest << " + " << c << ");\n";
            }
          }
          c += this->getTypeSize(v.type, 1u);
        }
      }
    };
    const auto& d = bd.getBehaviourData(h);
    os << "void exportStateData(mfront::gb::State& mbg_s1) const\n";
    os << "{\n";
    os << "using namespace tfel::math;\n";
    auto o = SupportedTypes::TypeSize{};
    for (const auto& v : bd.getMainVariables()) {
      const auto& th = v.second;
      if (th.arraySize != 1) {
        tfel::raise(
            "GenericBehaviourInterface::exportMechancialData: "
            "array of thermodynamical forces are not supported");
      }
      export_variable(th, "thermodynamic_forces", o);
      o += SupportedTypes::getTypeSize(th.type, th.arraySize);
    }
    o = SupportedTypes::TypeSize{};
    for (const auto& iv : d.getPersistentVariables()) {
      export_variable(iv, "internal_state_variables", o);
      o += SupportedTypes::getTypeSize(iv.type, iv.arraySize);
    }
    os << "} // end of exportStateData\n\n";
  }  // end of GenericBehaviourInterface::exportMechanicalData

  void GenericBehaviourInterface::writeStrainMeasureCommonPreProcessing1(
      std::ostream& out, const Hypothesis h) const {
    // space dimension
    const auto N = tfel::material::getSpaceDimension(h);
    // unsymmetric tensor size
    const auto ts = tfel::material::getTensorSize(h);
    out << "tfel::math::st2tost2<" << N << ",real> K;\n";
    // now converting the deformation gradient
    out << "tfel::math::tensor<" << N << ",real> F0;\n";
    out << "tfel::math::tensor<" << N << ",real> F1;\n";
    out << "tfel::math::stensor<" << N << ",real> s0;\n";
    out << "tfel::fsalgo::copy<" << ts << ">::exe(d->s0.gradients"
        << ",F0.begin());\n";
    out << "tfel::fsalgo::copy<" << ts << ">::exe(d->s1.gradients"
        << ",F1.begin());\n";
  }  // end of GenericBehaviourInterface::writeStrainMeasureCommonPreProcessing1

  void
  GenericBehaviourInterface::writeStandardFiniteStrainBehaviourPreProcessing(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    // space dimension
    const auto N = tfel::material::getSpaceDimension(h);
    // unsymmetric tensor size
    const auto ts = tfel::material::getTensorSize(h);
    const auto b = [this, bd, &h] {
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        const auto as =
            this->checkIfAxialDeformationGradientIsDefinedAndGetItsOffset(bd,
                                                                          h);
        return as.first;
      }
      return true;
    }();
    out << "tfel::math::stensor<" << N << ",real> s0;\n"
        << "tfel::math::stensor<" << N << ",real> s1;\n"
        << "auto *const thermodynamic_forces0_old = "
           "d->s0.thermodynamic_forces;\n"
        << "auto *const thermodynamic_forces1_old = "
           "d->s1.thermodynamic_forces;\n"
        << "if(sm!=StressMeasure::CAUCHY){\n";
    if (!b) {
      out << "return -1;\n";
    } else {
      out << "tfel::math::tensor<" << N << ",real> F0;\n"
          << "tfel::fsalgo::copy<" << ts << ">::exe(d->s0.gradients"
          << ",F0.begin());\n";
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        const auto as =
            this->checkIfAxialDeformationGradientIsDefinedAndGetItsOffset(bd,
                                                                          h);
        if (!as.first) {
          out << "return -1;\n";
        } else {
          out << "const auto F0zz = d->s0.internal_state_variables["
              << as.second.getValueForModellingHypothesis(h) << "];\n";
          if (h == ModellingHypothesis::PLANESTRESS) {
            out << "F0[2] += F0zz;\n";
          } else {
            out << "F0[1] += F0zz;\n";
          }
        }
      }
      out << "if(sm==StressMeasure::PK1){\n"
          << "tfel::math::TensorView<" << N
          << ",real> pk0(d->s0.thermodynamic_forces);\n"
          << "s0 = "
             "tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk0,"
             "F0);"
             "\n"
          << "} else if(sm==StressMeasure::PK2){\n"
          << "tfel::math::StensorView<" << N
          << ",real> S0(d->s0.thermodynamic_forces);\n"
          << "s0 = "
             "tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S0,"
             "F0);"
             "\n"
          << "} else {\n"
          << "  std::cerr << \"invalid choice for the \"\n"
          << "               \"stress measure\";\n"
          << "  std::exit(-1);\n"
          << "}\n"
          << "d->s0.thermodynamic_forces = s0.begin();\n"
          << "d->s1.thermodynamic_forces = s1.begin();\n";
    }
    out << "}\n";
  }  // end of
  // GenericBehaviourInterface::writeStandardFiniteStrainBehaviourPreProcessing

  void GenericBehaviourInterface::writeGreenLagrangeStrainMeasurePreProcessing(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "GenericBehaviourInterface::"
          "writeGreenLagrangeStrainMeasurePreProcessing: " +
          m);
    };
    // space dimension
    const auto N = tfel::material::getSpaceDimension(h);
    // unsymmetric tensor size
    const auto ts = tfel::material::getTensorSize(h);
    // symmetric tensor size
    const auto ss = tfel::material::getStensorSize(h);
    //
    this->writeStrainMeasureCommonPreProcessing1(out, h);
    // here we are calling computeGreenLagrangeTensor *before*
    // updating F0 and F1 in plane stress hypotheses.
    // This ensures that eto_zz is zero.
    out << "auto e0 = tfel::math::computeGreenLagrangeTensor(F0);\n"
        << "auto e1 = tfel::math::computeGreenLagrangeTensor(F1);\n";
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      const auto as = this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd, h);
      out << "const auto F0zz = "
             "std::sqrt(1+2*(d->s0.internal_state_variables["
          << as.second.getValueForModellingHypothesis(h) << "]));\n";
      if (!as.first) {
        raise("the axial strain is not defined");
      }
      if (h == ModellingHypothesis::PLANESTRESS) {
        out << "F0[2] += F0zz;\n";
      } else {
        out << "F0[1] += F0zz;\n";
      }
    }
    out << "auto S0 = tfel::math::stensor<" << N << ",real>{};\n"
        << "auto S1 = tfel::math::stensor<" << N << ",real>{};\n"
        << "if (sm == StressMeasure::CAUCHY) {\n"
        << "tfel::fsalgo::copy<" << ss << ">::exe(d->s0.thermodynamic_forces"
        << ",s0.begin());\n"
        << "S0 = tfel::math::"
        << "convertCauchyStressToSecondPiolaKirchhoffStress("
           "s0, F0);\n"
        << "} else if (sm == StressMeasure::PK1) {\n"
        << "auto pk0 = tfel::math::tensor<" << N << ",real>{};\n"
        << "tfel::fsalgo::copy<" << ts << ">::exe(d->s0.thermodynamic_forces"
        << ",pk0.begin());\n"
        << "s0 = tfel::math::"
        << "convertFirstPiolaKirchhoffStressToCauchyStress(pk0,F0);\n"
        << "S0 = tfel::math::"
        << "convertCauchyStressToSecondPiolaKirchhoffStress("
           "s0, F0);\n"
        << "} else if (sm == StressMeasure::PK2) {\n"
        << "tfel::fsalgo::copy<" << ss << ">::exe(d->s0.thermodynamic_forces"
        << ",S0.begin());\n"
        << "s0 = convertSecondPiolaKirchhoffStressToCauchyStress(S0,F0);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for the \"\n"
        << "               \"stress measure\";\n"
        << "  std::exit(-1);\n"
        << "}\n";
    this->writeStrainMeasureCommonPreProcessing2(out, "S");
  }  // end of
  // GenericBehaviourInterface::writeGreenLagrangeStrainMeasurePreProcessing

  void GenericBehaviourInterface::writeHenckyStrainMeasurePreProcessing(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "GenericBehaviourInterface::writeHenckyStrainMeasurePreProcessing:"
          " " +
          m);
    };
    // space dimension
    const auto N = tfel::material::getSpaceDimension(h);
    // unsymmetric tensor size
    const auto ts = tfel::material::getTensorSize(h);
    // symmetric tensor size
    const auto ss = tfel::material::getStensorSize(h);
    //
    this->writeStrainMeasureCommonPreProcessing1(out, h);
    out << "const auto setting = (smf==TangentOperator::DSIG_DF) ? \n"
        << "LogarithmicStrainHandler<" << N << ",real>::EULERIAN :\n"
        << "LogarithmicStrainHandler<" << N << ",real>::LAGRANGIAN;\n"
        << "LogarithmicStrainHandler<" << N << ",real> lgh0(setting,F0);\n"
        << "LogarithmicStrainHandler<" << N << ",real> lgh1(setting,F1);\n"
        << "auto e0 = lgh0.getHenckyLogarithmicStrain();\n"
        << "auto e1 = lgh1.getHenckyLogarithmicStrain();\n";
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      const auto as = this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd, h);
      if (!as.first) {
        raise("the axial strain is not defined");
      }
      out << "lgh0.updateAxialDeformationGradient("
          << "std::exp(d->s0.internal_state_variables["
          << as.second.getValueForModellingHypothesis(h) << "]));\n";
    }
    out << "auto T0 = tfel::math::stensor<" << N << ",real>{};\n"
        << "auto T1 = tfel::math::stensor<" << N << ",real>{};\n"
        << "if (sm == StressMeasure::CAUCHY) {\n"
        << "tfel::fsalgo::copy<" << ss << ">::exe(d->s0.thermodynamic_forces"
        << ",s0.begin());\n"
        << "T0 = lgh0.convertFromCauchyStress(s0);\n"
        << "} else if (sm == StressMeasure::PK1) {\n"
        << "auto pk0 = tfel::math::tensor<" << N << ",real>{};\n"
        << "tfel::fsalgo::copy<" << ts << ">::exe(d->s0.thermodynamic_forces"
        << ",pk0.begin());\n"
        << "s0 = tfel::math::"
        << "convertFirstPiolaKirchhoffStressToCauchyStress(pk0,F0);\n"
        << "T0 = lgh0.convertFromCauchyStress(s0);\n"
        << "} else if (sm == StressMeasure::PK2) {\n"
        << "auto S0 = tfel::math::stensor<" << N << ",real>{};\n"
        << "tfel::fsalgo::copy<" << ss << ">::exe(d->s0.thermodynamic_forces"
        << ",S0.begin());\n"
        << "s0 = convertSecondPiolaKirchhoffStressToCauchyStress(S0,F0);\n"
        << "T0 = lgh0.convertFromCauchyStress(s0);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for the \"\n"
        << "               \"stress measure\";\n"
        << "  std::exit(-1);\n"
        << "}\n";
    this->writeStrainMeasureCommonPreProcessing2(out, "T");
  }  // end of
     // GenericBehaviourInterface::writeHenckyStrainMeasurePreProcessing

  void GenericBehaviourInterface::writeStrainMeasureCommonPreProcessing2(
      std::ostream& out, const std::string& ss) const {
    out << "auto *const gradients0_old = d->s0.gradients;\n"
        << "auto *const gradients1_old = d->s1.gradients;\n"
        << "auto *const thermodynamic_forces0_old = "
           "d->s0.thermodynamic_forces;\n"
        << "auto *const thermodynamic_forces1_old = "
           "d->s1.thermodynamic_forces;\n"
        << "auto *const K_old = d->K;\n"
        << "K[0] = d->K[0];\n"
        << "d->s0.gradients = e0.begin();\n"
        << "d->s1.gradients = e1.begin();\n"
        << "d->s0.thermodynamic_forces = " << ss << "0.begin();\n"
        << "d->s1.thermodynamic_forces = " << ss << "1.begin();\n"
        << "d->K = K.begin();\n"
        << "const auto bp = K[0]<-0.5;\n"
        << "const auto bk = K[0]>0.5;\n";
  }  // end of
     // GenericBehaviourInterface::writeStrainMeasureCommonPreProcessing2

  void
  GenericBehaviourInterface::writeStandardFiniteStrainBehaviourPostProcessing(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    // space dimension
    const auto N = tfel::material::getSpaceDimension(h);
    // unsymmetric tensor size
    const auto ts = tfel::material::getTensorSize(h);
    out << "if((r) && (sm != StressMeasure::CAUCHY)){\n"
        << "tfel::math::tensor<" << N << ",real> F1;\n"
        << "tfel::fsalgo::copy<" << ts << ">::exe(d->s1.gradients"
        << ",F1.begin());\n";
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      const auto as =
          this->checkIfAxialDeformationGradientIsDefinedAndGetItsOffset(bd, h);
      if (!as.first) {
        out << "return -1;\n";
      } else {
        out << "const auto F1zz = d->s1.internal_state_variables["
            << as.second.getValueForModellingHypothesis(h) << "];\n";
        if (h == ModellingHypothesis::PLANESTRESS) {
          out << "F1[2] += F1zz;\n";
        } else {
          out << "F1[1] += F1zz;\n";
        }
      }
    }
    out << "d->s0.thermodynamic_forces = thermodynamic_forces0_old;\n"
        << "d->s1.thermodynamic_forces = thermodynamic_forces1_old;\n"
        << "if(sm==StressMeasure::PK1){\n"
        << "tfel::math::TensorView<" << N
        << ",real> pk1(d->s1.thermodynamic_forces);\n"
        << "pk1 = "
           "tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s1,"
           "F1);\n"
        << "} else if(sm==StressMeasure::PK2){\n"
        << "tfel::math::StensorView<" << N
        << ",real> S1(d->s1.thermodynamic_forces);\n"
        << "S1 = "
           "tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s1,"
           "F1);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for the \"\n"
        << "               \"stress measure\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "}\n"
        << "if((!r) && (sm!=StressMeasure::CAUCHY)){\n"
        << "d->s0.thermodynamic_forces = thermodynamic_forces0_old;\n"
        << "d->s1.thermodynamic_forces = thermodynamic_forces1_old;\n"
        << "}\n";
  }  // end of
  // GenericBehaviourInterface::writeStandardFiniteStrainBehaviourPostProcessing

  void GenericBehaviourInterface::writeGreenLagrangeStrainMeasurePostProcessing(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "GenericBehaviourInterface::"
          "writeGreenLagrangeStrainMeasurePostProcessing: " +
          m);
    };
    const auto N = tfel::material::getSpaceDimension(h);
    // symmetric tensor size
    const auto ss = tfel::material::getStensorSize(h);
    out << "d->s0.gradients = gradients0_old;\n"
        << "d->s1.gradients = gradients1_old;\n"
        << "d->s0.thermodynamic_forces = thermodynamic_forces0_old;\n"
        << "d->s1.thermodynamic_forces = thermodynamic_forces1_old;\n"
        << "d->K = K_old;\n"
        << "if(r){\n"
        << "if(bp){\n"
        << "if(smf==TangentOperator::DSIG_DF){\n"
        << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DSIG_DF,"
        << "TangentOperator::DS_DEGL>(K,F0,F0,s0);\n"
        << "} else if (smf==TangentOperator::DS_DEGL){\n"
        << "tfel::fsalgo::copy<" << ss * ss << ">::exe(K.begin(),d->K);\n"
        << "} else if (smf==TangentOperator::DPK1_DF){\n"
        << "tfel::math::T2toT2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DPK1_DF,"
        << "TangentOperator::DS_DEGL>(K,F0,F0,s0);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for consistent tangent \"\n"
        << "               \"operator\\n\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "} else { // if(bp)\n";
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      const auto as = this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd, h);
      if (!as.first) {
        raise("the axial strain is not defined");
      }
      out << "const auto F1zz = "
             "std::sqrt(1+2*(d->s1.internal_state_variables["
          << as.second.getValueForModellingHypothesis(h) << "]));\n";
      if (h == ModellingHypothesis::PLANESTRESS) {
        out << "F1[2] += F1zz;\n";
      } else {
        out << "F1[1] += F1zz;\n";
      }
    }
    out << "auto s1 = tfel::math::"
           "convertSecondPiolaKirchhoffStressToCauchyStress("
           "S1,F1);\n"
        << "if(sm==StressMeasure::CAUCHY){\n"
        << "tfel::fsalgo::copy<" << ss
        << ">::exe(s1.begin(),d->s1.thermodynamic_forces);\n"
        << "} else if(sm==StressMeasure::PK1){\n"
        << "tfel::math::TensorView<" << N
        << ",real> pk1(d->s1.thermodynamic_forces);\n"
        << "pk1 = "
           "tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s1,"
           "F1);\n"
        << "} else if(sm==StressMeasure::PK2){\n"
        << "tfel::fsalgo::copy<" << ss
        << ">::exe(S1.begin(),d->s1.thermodynamic_forces);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for the \"\n"
        << "               \"stress measure\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "if(bk){\n"
        << "if(smf==TangentOperator::DSIG_DF){\n"
        << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DSIG_DF,"
        << "TangentOperator::DS_DEGL>(K,F0,F1,s1);\n"
        << "} else if (smf == TangentOperator::DS_DEGL) {\n"
        << "tfel::fsalgo::copy<" << ss * ss << ">::exe(K.begin(),d->K);\n"
        << "} else if (smf==TangentOperator::DPK1_DF){\n"
        << "tfel::math::T2toT2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DPK1_DF,"
        << "TangentOperator::DS_DEGL>(K,F0,F1,s1);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for consistent tangent \"\n"
        << "               \"operator\\n\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "} // end of if(bk)\n"
        << "} // end of if(bp)\n"
        << "}\n";
  }  // end of
     // GenericBehaviourInterface::writeGreenLagrangeStrainMeasurePostProcessing

  void GenericBehaviourInterface::writeHenckyStrainMeasurePostProcessing(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "GenericBehaviourInterface::writeHenckyStrainMeasurePostProcessing:"
          " " +
          m);
    };
    const auto N = tfel::material::getSpaceDimension(h);
    // symmetric tensor size
    const auto ss = tfel::material::getStensorSize(h);
    out << "d->s0.gradients = gradients0_old;\n"
        << "d->s1.gradients = gradients1_old;\n"
        << "d->s0.thermodynamic_forces = thermodynamic_forces0_old;\n"
        << "d->s1.thermodynamic_forces = thermodynamic_forces1_old;\n"
        << "d->K = K_old;\n";
    out << "if(r){\n";
    out << "if(bp){\n"
        << "if(smf==TangentOperator::DSIG_DF){\n"
        << "const auto Cs = "
           "lgh0.convertToSpatialTangentModuli(K,T0);\n"
        << "const auto Dt = convert<TangentOperator::DTAU_DF,"
        << "TangentOperator::SPATIAL_MODULI>(Cs,F0,F0,s0);\n"
        << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DSIG_DF,"
        << "        TangentOperator::DTAU_DF>(Dt,F0,F0,s0);\n"
        << "} else if(smf==TangentOperator::DS_DEGL){\n"
        << "tfel::math::ST2toST2View<" << N
        << ",real>(d->K) = "
           "lgh0.convertToMaterialTangentModuli(K,T0);\n"
        << "} else if(smf==TangentOperator::DPK1_DF){\n"
        << "const auto Cse = "
           "lgh0.convertToMaterialTangentModuli(K,T0);\n"
        << "tfel::math::T2toT2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DPK1_DF,"
        << "        TangentOperator::DS_DEGL>(Cse,F0,F0,s0);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for consistent tangent \"\n"
        << "               \"operator\\n\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "} else { // if(bp)\n";
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      const auto as = this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd, h);
      if (!as.first) {
        raise("the axial strain is not defined");
      }
      out << "lgh1.updateAxialDeformationGradient("
          << "std::exp(d->s1.internal_state_variables["
          << as.second.getValueForModellingHypothesis(h) << "]));\n";
    }
    out << "const auto s1 = lgh1.convertToCauchyStress(T1);\n"
        << "if(sm==StressMeasure::CAUCHY){\n"
        << "tfel::fsalgo::copy<" << ss
        << ">::exe(s1.begin(),d->s1.thermodynamic_forces);\n"
        << "} else if(sm==StressMeasure::PK1){\n"
        << "tfel::math::TensorView<" << N
        << ",real> pk1(d->s1.thermodynamic_forces);\n"
        << "pk1 = "
           "tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s1,"
           "F1);\n"
        << "} else if(sm==StressMeasure::PK2){\n"
        << "tfel::math::StensorView<" << N
        << ",real> S1(d->s1.thermodynamic_forces);\n"
        << "S1 = "
           "tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s1,"
           "F1);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for the \"\n"
        << "               \"stress measure\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "if(bk){\n"
        << "if(smf==TangentOperator::DSIG_DF){\n"
        << "const auto Cs = "
           "lgh1.convertToSpatialTangentModuli(K,T1);\n"
        << "const auto Dt = convert<TangentOperator::DTAU_DF,"
        << "                        "
           "TangentOperator::SPATIAL_MODULI>(Cs,F0,F1,s1);\n"
        << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DSIG_DF,"
        << "        TangentOperator::DTAU_DF>(Dt,F0,F1,s1);\n"
        << "} else if(smf==TangentOperator::DS_DEGL){\n"
        << "tfel::math::ST2toST2View<" << N
        << ",real>(d->K) = "
           "lgh1.convertToMaterialTangentModuli(K,T1);\n"
        << "} else if(smf==TangentOperator::DPK1_DF){\n"
        << "const auto Cse = "
           "lgh1.convertToMaterialTangentModuli(K,T1);\n"
        << "tfel::math::T2toT2View<" << N << ",real>(d->K) = "
        << "convert<TangentOperator::DPK1_DF,"
        << "        TangentOperator::DS_DEGL>(Cse,F0,F1,s1);\n"
        << "} else {\n"
        << "  std::cerr << \"invalid choice for consistent tangent \"\n"
        << "               \"operator\\n\";\n"
        << "  std::exit(-1);\n"
        << "}\n"
        << "} // end of if(bk)\n"
        << "} // end of if(bp)\n"
        << "}\n";
  }  // end of
     // GenericBehaviourInterface::writeHenckyStrainMeasurePostProcessing

  GenericBehaviourInterface::~GenericBehaviourInterface() = default;

}  // end of namespace mfront
