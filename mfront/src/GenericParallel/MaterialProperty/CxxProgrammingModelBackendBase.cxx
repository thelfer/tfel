/*!
 * \file   CxxProgrammingModelBackendBase.cxx
 * \brief  This file implements the `CxxProgrammingModelBackendBase` class
 * \author Thomas Helfer
 * \date 18/05/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */
#include <ostream>
#include "MFront/VariableDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/GenericParallel/MaterialProperty/CxxProgrammingModelBackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  CxxProgrammingModelBackendBase::CxxProgrammingModelBackendBase() = default;

  std::string CxxProgrammingModelBackendBase::getHeaderFileExtension() const {
    return "hxx";
  }  // end of getHeaderFileExtension

  std::string CxxProgrammingModelBackendBase::getSourceFileExtension() const {
    return "cxx";
  }  // end of getSourceFileExtension

  void CxxProgrammingModelBackendBase::writeSpecificIncludesInHeaderFile(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeSpecificIncludesInSourceFile

  void CxxProgrammingModelBackendBase::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include<ranges>\n"
       << "#include<concepts>\n"
       << "#include<type_traits>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void CxxProgrammingModelBackendBase::writeGlobalFunctions(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}  // end of writeGlobalFunction

  void CxxProgrammingModelBackendBase::writeCxxDeclarations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  void CxxProgrammingModelBackendBase::writeCxxImplementations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}

  void CxxProgrammingModelBackendBase::writeCImplementations(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    this->writeCFunctionBeginning(os, i, mpd);
    // the runtime of the backend (data transfers, submission of the kernels)
    // may throw exceptions, which must not cross the boundary of the C
    // function. They are reported as runtime errors of the backend, as the
    // errors of the CUDA and HIP runtimes: the kernels can't throw.
    os << "try{\n";
    this->writeExecutionPreparation(os, i, mpd);
    if (this->handlesDataTransfer()) {
      this->writeDataTransfersToDevice(os, i, mpd);
    }
    this->writeKernelCall(os, i, mpd, fd);
    this->writeResultsAnalysis(os, i, mpd);
    if (this->handlesDataTransfer()) {
      this->writeDataTransfersToHost(os, i, mpd);
    }
    writeHostExceptionsHandling(os);
    writeCFunctionEnd(os, i, mpd);
  }  // end of writeCImplementations

  void CxxProgrammingModelBackendBase::writeKernel(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool handleStrides) const {
    const auto types = i.getTypesDescription();
    const auto kernel =
        handleStrides ? "mfront_kernel" : "mfront_kernel_without_strides";
    // the captured variables are declared by `writeKernels`: the extended
    // lambdas of nvcc can't have initializers in their captures
    os << "auto " << kernel << " = [mfront_output";
    if (handleStrides) {
      os << ", mfront_output_stride";
    }
    for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
      os << ", mfront_arg" << idx;
    }
    if ((handleStrides) && (!mpd.inputs.empty())) {
      os << ", mfront_strides";
    }
    os << ", mfront_statuses";
    if (isBoundsCheckingRequired(mpd)) {
      os << ", mfront_out_of_bounds_policy";
      if (this->mergesPointStatusesInKernels()) {
        os << ", mfront_statistics";
      }
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      os << ", mfront_parameters";
    }
    os << "]" << this->getKernelSpecifiers() << "(" << types.integer_type
       << " mfront_idx){\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    this->writeKernelBody(os, i, mpd, handleStrides);
    os << "}; // end of " << kernel << "\n";
  }  // end of writeKernel

  void CxxProgrammingModelBackendBase::writeExecutionPreparation(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeExecutionPreparation

  void CxxProgrammingModelBackendBase::writeDataTransfersToHost(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeDataTransfersToHost

  void CxxProgrammingModelBackendBase::writeDataTransfersToDevice(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeDataTransfersToDevice

  void CxxProgrammingModelBackendBase::writeKernels(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    // variables captured by the kernels. The pointers to the arguments are
    // copied, since the array of those pointers given by the calling code
    // is generally not accessible by the device (for example, an array on
    // the stack of the calling code).
    for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
      os << "const auto mfront_arg" << idx << " = mfront_args[" << idx
         << "];\n";
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      const auto name = i.getFunctionName(mpd);
      const auto nname = i.getInterfaceInternalNamespace();
      const auto pcn = getMaterialPropertyParametersClassName(name);
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      os << "const auto mfront_parameters = "               //
         << "static_cast<" << nname << "::" << pcn << ">("  //
         << nname << "::" << hn << "::get" << hn << "());\n";
    }
    this->writeKernel(os, i, mpd, fd, false);
    this->writeKernel(os, i, mpd, fd, true);
  }  // end of writeKernels

  std::string CxxProgrammingModelBackendBase::getKernelSpecifiers() const {
    return "";
  }  // end of getKernelSpecifiers

  CxxProgrammingModelBackendBase::~CxxProgrammingModelBackendBase() = default;

}  // end of namespace mfront::generic_parallel::material_property
