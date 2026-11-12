/*!
 * \file   mfront/src/GenericParallelMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date 04/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <string>
#include <set>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/GenericParallel/AbstractBackend.hxx"
#include "MFront/GenericParallel/BackendFactory.hxx"
#include "MFront/GenericParallel/ParallelSTLBackend.hxx"
#include "MFront/GenericParallel/StatisticsKeys.hxx"

namespace mfront {

  //! \brief a simple alias
  using GenericBase = GenericMaterialPropertyInterfaceBase;

  //! \brief identifier of the check of the finiteness of the result
  static constexpr std::size_t invalid_result_check =
      GenericBase::invalid_result_check;

  //! \return the name of the function object evaluating a material property
  //! at one point
  static std::string getEvaluationName(const std::string& name) {
    return name + "_Evaluation";
  }  // end of getEvaluationName

  /*!
   * \brief write the declarations of the function of batches and of the
   * report function of a material property
   * \param[out] os: output stream
   * \param[in] mpd: material property description
   * \param[in] name: name of the material property
   */
  static void writeDeclarations(std::ostream& os,
                                const MaterialPropertyDescription& mpd,
                                const std::string& name) {
    os << "/*!\n"
       << " * \\brief compute the value of '" << mpd.className
       << "' material property on a batch of points (see "
       << "`mfront_gpmp_BatchData`)\n"
       << " *\n"
       << " * \\param[out] mfront_output_status: output status\n"
       << " * \\param[in] mfront_batch: data of the batch\n"
       << " * \\return the status, also given by the output status\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel(\n"
       << "mfront_gpmp_OutputStatus* const,\n"
       << "const mfront_gpmp_BatchData* const);\n\n"
       << "/*!\n"
       << " * \\brief analyse the statistics of an evaluation of '"
       << mpd.className << "' material property, once the computations are "
       << "completed, and report the result as a synchronous evaluation "
       << "does\n"
       << " *\n"
       << " * \\param[out] mfront_output_status: output status\n"
       << " * \\param[in] mfront_statistics: statistics given by the "
       << "calling code, read on the host\n"
       << " * \\return the status, also given by the output status\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel_report(\n"
       << "mfront_gpmp_OutputStatus* const,\n"
       << "const mfront_gpmp_Statistics* const);\n\n";
  }  // end of writeDeclarations

  /*!
   * \brief write the code analysing the statistics `mfront_statistics` in
   * the function reporting the result. The reported point is the first point
   * where the evaluation failed, otherwise the first point where the result
   * is unreliable: the status, the bounds status and the message describe
   * the check retained at this point.
   * \param[in] os: output stream
   * \param[in] mpd: material property description
   */
  static void writeStatisticsAnalysis(std::ostream& os,
                                      const MaterialPropertyDescription& mpd) {
    const auto ns = std::string{"::mfront::gpmp::"};
    os << "const auto mfront_number_of_failures = static_cast<"
       << "mfront_gmp_size_type>(mfront_statistics->number_of_failures);\n"
       << "const auto mfront_number_of_unreliable_results = static_cast<"
       << "mfront_gmp_size_type>("
       << "mfront_statistics->number_of_unreliable_results);\n"
       << "mfront_output_status->number_of_failures = "
       << "mfront_number_of_failures;\n"
       << "mfront_output_status->number_of_unreliable_results = "
       << "mfront_number_of_unreliable_results;\n"
       << "if (mfront_statistics->first_failure != " << ns << "no_event) {\n"
       << "mfront_output_status->first_failure_index = static_cast<"
       << "mfront_gmp_size_type>(" << ns
       << "getPointIndex(mfront_statistics->first_failure));\n"
       << "}\n"
       << "if (mfront_statistics->first_unreliable_result != " << ns
       << "no_event) {\n"
       << "mfront_output_status->first_unreliable_result_index = static_cast<"
       << "mfront_gmp_size_type>(" << ns
       << "getPointIndex(mfront_statistics->first_unreliable_result));\n"
       << "}\n"
       // reported point: first failure, otherwise first unreliable result
       << "const bool mfront_failure = "
       << "mfront_statistics->first_failure != " << ns << "no_event;\n"
       << "const auto mfront_key = mfront_failure ? "
       << "mfront_statistics->first_failure : "
       << "mfront_statistics->first_unreliable_result;\n"
       << "if (mfront_key == " << ns << "no_event) {\n"
       << "return 0;\n"
       << "}\n"
       << "const auto mfront_check = " << ns << "getCheck(mfront_key);\n"
       << "const auto mfront_points = \" at point \" + std::to_string(" << ns
       << "getPointIndex(mfront_key)) + \" (\" + "
       << "std::to_string(mfront_number_of_failures) + \" failure(s), \" + "
       << "std::to_string(mfront_number_of_unreliable_results) + "
       << "\" unreliable result(s))\";\n"
       << "if (mfront_check == " << invalid_result_check << "u) {\n"
       << "return " << ns << "report(*mfront_output_status, -4, "
       << "\"invalid result\" + mfront_points);\n"
       << "}\n";
    // as with the `generic` interface, the status and the bounds status are
    // negative if the evaluation failed (physical bound, or bound with the
    // strict policy), positive otherwise (bound with the warning policy)
    auto write_check = [&os, &ns](const std::size_t c, const std::string& msg,
                                  const std::size_t rank) {
      os << "if (mfront_check == " << c << "u) {\n"
         << "const auto mfront_status = mfront_failure ? -1 : 1;\n"
         << "mfront_output_status->bounds_status = mfront_status * " << rank
         << ";\n"
         << "return " << ns << "report(*mfront_output_status, mfront_status, "
         << "\"" << msg << "\" + mfront_points);\n"
         << "}\n";
    };
    auto for_each_variable = [&mpd](const auto& f) {
      for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
        f(mpd.inputs[idx], idx);
      }
      f(mpd.output, mpd.inputs.size());
    };
    auto has_upper_bound = [](const VariableBoundsDescription& b) {
      return (b.boundsType == VariableBoundsDescription::UPPER) ||
             (b.boundsType == VariableBoundsDescription::LOWERANDUPPER);
    };
    auto has_lower_bound = [](const VariableBoundsDescription& b) {
      return (b.boundsType == VariableBoundsDescription::LOWER) ||
             (b.boundsType == VariableBoundsDescription::LOWERANDUPPER);
    };
    for_each_variable([&write_check, &has_upper_bound, &has_lower_bound](
                          const VariableDescription& v, const std::size_t idx) {
      if (!v.hasPhysicalBounds()) {
        return;
      }
      const auto& b = v.getPhysicalBounds();
      if (has_upper_bound(b)) {
        write_check(GenericBase::getPhysicalBoundCheck(idx, true),
                    v.name + " is over its physical upper bound", idx + 1);
      }
      if (has_lower_bound(b)) {
        write_check(GenericBase::getPhysicalBoundCheck(idx, false),
                    v.name + " is below its physical lower bound", idx + 1);
      }
    });
    for_each_variable([&write_check, &has_upper_bound, &has_lower_bound, &mpd](
                          const VariableDescription& v, const std::size_t idx) {
      if (!v.hasBounds()) {
        return;
      }
      const auto& b = v.getBounds();
      if (has_upper_bound(b)) {
        write_check(GenericBase::getBoundCheck(mpd, idx, true),
                    v.name + " is over its upper bound", idx + 1);
      }
      if (has_lower_bound(b)) {
        write_check(GenericBase::getBoundCheck(mpd, idx, false),
                    v.name + " is below its lower bound", idx + 1);
      }
    });
    // the keys are only built by the kernels of this material property
    os << "return " << ns << "report(*mfront_output_status, -5, "
       << "\"invalid statistics (unknown check)\");\n";
  }  // end of writeStatisticsAnalysis

  /*!
   * \brief write the function analysing the statistics of an evaluation
   * (`<name>_generic_parallel_report`), called by the generated function of
   * batches for a synchronous evaluation, or by the calling code
   * \param[out] os: output stream
   * \param[in] mpd: material property description
   * \param[in] name: name of the material property
   */
  static void writeReportFunction(std::ostream& os,
                                  const MaterialPropertyDescription& mpd,
                                  const std::string& name) {
    os << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel_report("
       << "mfront_gpmp_OutputStatus* const mfront_output_status,\n"
       << "const mfront_gpmp_Statistics* const mfront_statistics){\n"
       << "return ::mfront::gpmp::handleExceptions(mfront_output_status, "
       << "[mfront_output_status, mfront_statistics] {\n"
       << "if (mfront_statistics == nullptr) {\n"
       << "return ::mfront::gpmp::report(*mfront_output_status, -5, "
       << "\"no statistics given\");\n"
       << "}\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      writeStatisticsAnalysis(os, mpd);
    } else {
      os << "return 0;\n";
    }
    os << "});\n"
       << "} // end of " << name << "_generic_parallel_report\n\n";
  }  // end of writeReportFunction

  /*!
   * \brief write the function object evaluating a material property at one
   * point (see `mfront::gpmp::PointEvaluator`). It takes the values of the
   * arguments and the out of bounds policy, and returns the value and the
   * status of the point. The parameters, if they are not treated as static
   * variables, are members of this function object.
   * \param[out] os: output stream
   * \param[in] i: interface
   * \param[in] mpd: material property description
   * \param[in] fd: file description
   * \param[in] name: name of the material property
   */
  static void writeEvaluation(std::ostream& os,
                              const GenericParallelMaterialPropertyInterface& i,
                              const MaterialPropertyDescription& mpd,
                              const FileDescription& fd,
                              const std::string& name) {
    const auto qt = useQuantities(mpd);
    const auto nargs = mpd.inputs.size();
    const auto static_parameters = areParametersTreatedAsStaticVariables(mpd);
    const auto checks = !areRuntimeChecksDisabled(mpd);
    // internal linkage: the libraries of several backends of a material
    // property may be loaded in the same process
    os << "namespace {\n\n"
       << "//! \\brief evaluation of the '" << mpd.className
       << "' material property at one point\n"
       << "struct " << getEvaluationName(name) << " {\n"
       << "TFEL_HOST_DEVICE ::mfront::gpmp::PointResult operator()(\n"
       << "[[maybe_unused]] const std::array<mfront_gmp_real, " << nargs
       << ">& mfront_args,\n"
       << "[[maybe_unused]] const mfront_gmp_OutOfBoundsPolicy "
       << "mfront_out_of_bounds_policy) const {\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    // parameters
    for (const auto& p : mpd.parameters) {
      if (static_parameters) {
        const auto pv =
            p.getAttribute<double>(VariableDescription::defaultValue);
        os << "constexpr auto " << p.name << " = "
           << (qt ? p.type : std::string{"double"}) << "(" << pv << ");\n";
      } else {
        os << "const auto " << p.name << " = "
           << (qt ? p.type : std::string{"real"}) << "(this->mfront_parameters."
           << p.name << ");\n";
      }
    }
    // arguments
    for (std::size_t idx = 0; idx != nargs; ++idx) {
      const auto& v = mpd.inputs[idx];
      const auto value = "mfront_args[" + std::to_string(idx) + "]";
      os << "const auto " << v.name << " = "
         << (qt ? v.type + "(" + value + ")" : value) << ";\n";
    }
    const auto result = qt ? mpd.output.name + ".getValue()" : mpd.output.name;
    // evaluation at the point, written as for the `generic` interface: a
    // failed check ends the evaluation at this point (the output is `NaN`)
    os << "auto mfront_status = ::mfront::gpmp::PointStatus{};\n"
       << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    GenericBase::writePointEvaluation(
        os, mpd,
        GenericBase::PointEvaluationOptions{
            .bounds_check_macro =
                "NO_" + i.getInterfaceNameInUpperCase() + "_BOUNDS_CHECK",
            .out_of_bounds_policy_prefix =
                i.getOutOfBoundsPolicyEnumerationPrefix(),
            .failure =
                [](std::ostream& out, const GenericBase::BoundCheck& c) {
                  out << "mfront_status.report(" << c.identifier
                      << "u, true);\n"
                      << "return {std::numeric_limits<mfront_gmp_real>::"
                      << "quiet_NaN(), mfront_status};\n";
                },
            .warning =
                [](std::ostream& out, const GenericBase::BoundCheck& c) {
                  out << "mfront_status.report(" << c.identifier
                      << "u, false);\n";
                }});
    if (checks) {
      // as with the `generic` interface, the output is the computed value
      os << "if(!tfel::math::ieee754::isfinite(" << result << ")){\n"
         << "mfront_status.reportInvalidResult(" << invalid_result_check
         << "u);\n"
         << "}\n";
    }
    os << "return {" << result << ", mfront_status};\n"
       << "}\n";
    if ((!static_parameters) && (!mpd.parameters.empty())) {
      os << "//! \\brief values of the parameters\n"
         << i.getInterfaceInternalNamespace()
         << "::" << getMaterialPropertyParametersClassName(name)
         << " mfront_parameters;\n";
    }
    os << "}; // end of struct " << getEvaluationName(name) << "\n\n"
       << "} // end of namespace\n\n";
  }  // end of writeEvaluation

  /*!
   * \brief write the function of batches of a material property
   * (`<name>_generic_parallel`). It checks the pointers, accepts an empty
   * batch without any other check, gets the parameters (status `-6` if they
   * are invalid), and calls the runtime of the material properties (see
   * `mfront::gpmp::evaluateBatch`) with the evaluation at one point and the
   * primitive of the backend.
   * \param[out] os: output stream
   * \param[in] i: interface
   * \param[in] backend: backend
   * \param[in] mpd: material property description
   * \param[in] name: name of the material property
   */
  static void writeBatchFunction(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const generic_parallel::AbstractBackend& backend,
      const MaterialPropertyDescription& mpd,
      const std::string& name) {
    const auto nargs = mpd.inputs.size();
    const auto checks = !areRuntimeChecksDisabled(mpd);
    // the identifier of a check is stored in 16 bits of the keys of the
    // statistics
    tfel::raise_if(GenericBase::getBoundCheck(mpd, nargs, false) >
                       generic_parallel::maximal_number_of_checks,
                   "GenericParallelMaterialPropertyInterface::"
                   "writeBatchFunction: too many arguments");
    os << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel("
       << "mfront_gpmp_OutputStatus* const mfront_output_status,\n"
       << "const mfront_gpmp_BatchData* const mfront_batch){\n"
       << "return ::mfront::gpmp::handleExceptions(mfront_output_status, "
       << "[mfront_output_status, mfront_batch] {\n"
       << "if (mfront_batch == nullptr) {\n"
       << "return ::mfront::gpmp::report(*mfront_output_status, -5, "
       << "\"no batch data given\");\n"
       << "}\n"
       // an empty batch is accepted without any check: nothing is read,
       // written or submitted (the fields and the queue may be null)
       << "if (mfront_batch->number_of_points == 0) {\n"
       << "return mfront_output_status->status;\n"
       << "}\n";
    auto evaluation = getEvaluationName(name) + "{}";
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      const auto nname = i.getInterfaceInternalNamespace();
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      const auto pcn = getMaterialPropertyParametersClassName(name);
      os << "const auto& mfront_parameters_handler = " << nname << "::" << hn
         << "::get" << hn << "();\n";
      // as with the `generic` interface, the parameters are not checked if
      // the runtime checks are disabled
      if (checks) {
        os << "if (!mfront_parameters_handler.ok) {\n"
           << "return ::mfront::gpmp::report(*mfront_output_status, -6, "
           << "mfront_parameters_handler.msg);\n"
           << "}\n";
      }
      evaluation = getEvaluationName(name) + "{static_cast<const " + nname +
                   "::" + pcn + "&>(mfront_parameters_handler)}";
    }
    os << "constexpr auto mfront_arguments_names = std::array<const char*, "
       << nargs << ">{";
    for (std::size_t idx = 0; idx != nargs; ++idx) {
      os << (idx == 0 ? "" : ", ") << '"' << mpd.inputs[idx].getExternalName()
         << '"';
    }
    os << "};\n";
    backend.writeCall(os, "evaluateBatch", "*mfront_output_status",
                      "*mfront_batch, " + evaluation +
                          ", mfront_arguments_names, " +
                          (checks ? "true" : "false") + ", &" + name +
                          "_generic_parallel_report");
    os << "});\n"
       << "} // end of " << name << "_generic_parallel\n\n";
  }  // end of writeBatchFunction

  std::string GenericParallelMaterialPropertyInterface::getName() {
    return "generic-parallel";
  }

  GenericParallelMaterialPropertyInterface::
      GenericParallelMaterialPropertyInterface()
      : backend(std::make_unique<
                ::mfront::generic_parallel::ParallelSTLBackend>()) {
  }  // end of GenericParallelMaterialPropertyInterface

  void GenericParallelMaterialPropertyInterface::setOptions(
      const DataMap& opts) {
    using ::mfront::generic_parallel::BackendFactory;
    using tfel::utilities::DataStructure;
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addDataTypeValidator<DataStructure>("backend");
    validator.validate(opts);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "GenericParallelMaterialPropertyInterface::setOptions: begin\n";
    }
    const auto ds = [&opts]() -> tfel::utilities::DataStructure {
      if (contains(opts, "backend")) {
        return DataStructure::convert(get(opts, "backend"));
      }
      auto r = DataStructure{};
      r.name = "stlpar";
      return r;
    }();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "GenericParallelMaterialPropertyInterface::setOptions: "
                        "creating backend '"
                     << ds.name << "'\n";
    }
    auto& f = BackendFactory::get();
    this->backend = f.generate(ds.name, ds.data);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "GenericParallelMaterialPropertyInterface::setOptions: end\n";
    }
  }  // end of setOptions

  GenericParallelMaterialPropertyInterface::TypesDescription
  GenericParallelMaterialPropertyInterface::getTypesDescription() const {
    auto types = TypesDescription{};
    types.real_type = "mfront_gmp_real";
    types.integer_type = "mfront_gmp_size_type";
    types.out_of_bounds_policy_type = "mfront_gmp_OutOfBoundsPolicy";
    types.output_status_type = "mfront_gpmp_OutputStatus";
    return types;
  }  // end of getTypesDescription

  std::vector<std::string>
  GenericParallelMaterialPropertyInterface::getInterfaceHeaderFiles() const {
    return {"MFront/GenericMaterialProperty/Types.h",
            "MFront/GenericParallelMaterialProperty/MaterialProperty.h"};
  }  // end of getInterfaceHeaderFiles

  std::string GenericParallelMaterialPropertyInterface::getInterfaceName()
      const {
    return GenericParallelMaterialPropertyInterface::getName();
  }  // end of getInterfaceName

  std::string
  GenericParallelMaterialPropertyInterface::getInterfaceInternalNamespace()
      const {
    return "generic_parallel";
  }  // end of getInterfaceInternalNamespace

  std::string
  GenericParallelMaterialPropertyInterface::getInterfaceNameInCamelCase()
      const {
    return "GenericParallel";
  }  // end of getInterfaceNameInCamelCase

  std::string
  GenericParallelMaterialPropertyInterface::getInterfaceNameInUpperCase()
      const {
    return "GENERIC_PARALLEL";
  }  // end of getInterfaceNameInUpperCase

  std::string GenericParallelMaterialPropertyInterface::
      getOutOfBoundsPolicyEnumerationPrefix() const {
    return "GENERIC_MATERIALPROPERTY";
  }  // end of getOutOfBoundsPolicyEnumerationPrefix

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  GenericParallelMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    const auto iname = this->getInterfaceName();
    const auto iccname = this->getInterfaceNameInCamelCase();
    tfel::raise_if((std::find(i.begin(), i.end(), iname) != i.end()) ||
                       (std::find(i.begin(), i.end(), iccname) != i.end()),
                   "GenericParallelMaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void GenericParallelMaterialPropertyInterface::getLibraryDescription(
      TargetsDescription& d,
      LibraryDescription& l,
      const MaterialPropertyDescription& mpd) const {
    const auto name = this->getFunctionName(mpd);
    insert_if(l.sources, this->getSourceFileName(name));
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(l.epts, name + "_generic_parallel");
    d.headers.push_back(this->getHeaderFileName(name));
  }  // end of getLibraryDescription

  void GenericParallelMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto iccname = this->getInterfaceNameInCamelCase();
    const auto lib = iccname + getMaterialLawLibraryNameBase(mpd) + '-' +
                     this->backend->getName();
    this->getLibraryDescription(d, d.getLibrary(lib), mpd);
  }  // end of getTargetsDescription

  std::string GenericParallelMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto& material = mpd.material;
    const auto& className = mpd.className;
    const auto name = material.empty() ? className : material + "_" + className;
    // the names of the batch functions and of their metadata end with
    // `_generic_parallel`
    tfel::raise_if(name.find("_generic_parallel") != std::string::npos,
                   "GenericParallelMaterialPropertyInterface::getFunctionName: "
                   "the name of the material property ('" +
                       name + "') must not contain '_generic_parallel'");
    return name;
  }

  std::string GenericParallelMaterialPropertyInterface::getHeaderFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + '-' + i + '-' + this->backend->getName() + ".hxx";
  }

  std::string GenericParallelMaterialPropertyInterface::getSourceFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + '-' + i + '-' + this->backend->getName() + '.' +
           this->backend->getSourceFileExtension();
  }

  void GenericParallelMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of writeOutputFiles

  void GenericParallelMaterialPropertyInterface::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto name = this->getFunctionName(mpd);
    const auto fn = "include/" + this->getHeaderFileName(name);
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "GenericParallelMaterialPropertyInterface::"
                        "writeHeaderFile: begin\n";
    }
    std::ofstream os{fn};
    tfel::raise_if(
        !os,
        "GenericParallelMaterialPropertyInterface::writeOutputFiles: "
        "unable to open '" +
            fn + "'");
    os.exceptions(std::ios::badbit | std::ios::failbit);
    os << "/*!\n"
       << "* \\file   " << fn << "\n"
       << "* \\brief  "
       << "this file declares the " << name << " MaterialLaw.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      os << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      os << "* \\date " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      os << fd.description << '\n';
    }
    os << " */\n\n";

    // the name of the backend is part of the header guard, so that the
    // headers of several backends can be included in the same file
    const auto guard = makeUpperCase(name) + "_" + iucname + "_" +
                       makeUpperCase(this->backend->getName()) + "_HH";
    os << "#ifndef " << guard << "\n"
       << "#define " << guard << "\n\n";
    writeExportDirectives(os);

    for (const auto& header : this->getInterfaceHeaderFiles()) {
      os << "#include \"" << header << "\"\n\n";
    }

    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    writeDeclarations(os, mpd, name);

    writeParametersSetterFunctionDeclaration(
        os, mpd,
        {.material_property_name = name,
         .floating_point_type = "double",
         .interface_namespace = this->getInterfaceName()});

    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n"
       << "#endif /* " << guard << " */\n";
    os.close();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "GenericParallelMaterialPropertyInterface::"
                        "writeHeaderFile: end\n";
    }
  }  // end of writeHeaderFile

  void GenericParallelMaterialPropertyInterface::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "GenericParallelMaterialPropertyInterface::"
                        "writeSrcFile: begin\n";
    }
    const auto nname = this->getInterfaceInternalNamespace();
    const auto name = this->getFunctionName(mpd);
    const auto fn = "src/" + this->getSourceFileName(name);
    std::ofstream os{fn};
    tfel::raise_if(
        !os,
        "GenericParallelMaterialPropertyInterface::writeOutputFiles: "
        "unable to open '" +
            fn + "'");
    os.exceptions(std::ios::badbit | std::ios::failbit);
    // precision of the bounds and of the default values of the parameters
    os.precision(14);
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& date = fd.date;
    const auto& includes = mpd.includes;
    os << "/*!\n"
       << "* \\file   " << fn << '\n'
       << "* \\brief  "
       << "this file implements the " << name << " MaterialLaw.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!author.empty()) {
      os << "* \\author " << author << '\n';
    }
    if (!date.empty()) {
      os << "* \\date " << date << '\n';
    }
    os << " */\n\n";
    os << "#include<algorithm>\n"
       << "#include<iterator>\n"
       << "#include<iostream>\n"
       << "#include<sstream>\n"
       << "#include<fstream>\n"
       << "#include<cstring>\n"
       << "#include<cstdlib>\n"
       << "#include<cerrno>\n"
       << "#include<string>\n"
       << "#include<vector>\n"
       << "#include<locale>\n"
       << "#include<cmath>\n"
       << "#include<limits>\n";
    os << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include\"TFEL/PhysicalConstants.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n"
       << "#include\"TFEL/Math/General/DerivativeType.hxx\"\n"
       << "#include\"MFront/GenericParallel/MemorySpace.hxx\"\n"
       << "#include\"MFront/GenericParallelMaterialProperty/Evaluate.hxx\"\n";
    if (useQuantities(mpd)) {
      os << "#include\"TFEL/Math/qt.hxx\"\n"
         << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    os << "#include\"" << this->getHeaderFileName(name) << "\"\n\n";
    if (!includes.empty()) {
      os << includes << "\n\n";
    }
    // primitive of the backend. Its header includes the header of the
    // execution policy of the backend
    this->backend->writeSpecificIncludes(os);
    os << '\n';

    writeMaterialPropertyParametersHandler(os, mpd,
                                           {.material_property_name = name,
                                            .floating_point_type = "double",
                                            .interface_namespace = nname});

    writeEvaluation(os, *this, mpd, fd, name);

    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    writeFileDescriptionSymbols(os, name, fd);
    writeVariablesNamesSymbol(os, name, mpd);
    writeVariablesBoundsSymbols(os, name, mpd);
    writeValidatorSymbol(os, name, mpd);
    writeCompatibilityModeSymbol(os, name, mpd);
    writeBuildIdentifierSymbol(os, name, mpd);
    writeEntryPointSymbol(os, name);
    writeTFELVersionSymbol(os, name);
    writeUnitSystemSymbol(os, name, mpd);
    writeInterfaceSymbol(os, name, this->getInterfaceNameInCamelCase());
    // backend, version of the generated functions, memory space and
    // execution policy
    this->backend->writeSymbols(os, name);
    writeLawSymbol(os, name, mpd.law);
    writeMaterialSymbol(os, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(os, name,
                                     MaterialKnowledgeType::MATERIALPROPERTY);
    writeParametersSymbols(os, name, mpd);
    exportStringSymbol(
        os, name + "_src",
        tfel::utilities::tokenize(file, tfel::system::dirSeparator()).back());

    writeParametersSetterFunctionImplementation(
        os, mpd,
        {.material_property_name = name,
         .floating_point_type = "double",
         .interface_namespace = nname});

    writeReportFunction(os, mpd, name);
    writeBatchFunction(os, *this, *(this->backend), mpd, name);

    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";

    os.close();

    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "GenericParallelMaterialPropertyInterface::"
                        "writeSrcFile: end\n";
    }
  }  // end of GenericParallelMaterialPropertyInterface::writeSrcFile

  GenericParallelMaterialPropertyInterface::
      ~GenericParallelMaterialPropertyInterface() = default;

}  // end of namespace mfront
