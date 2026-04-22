/*!
 * \file   mfront/src/GenericParallelMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/05/2022
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
#include "MFront/GenericParallel/MaterialProperty/ParallelSTLBackend.hxx"
#include "MFront/GenericParallel/MaterialProperty/BackendFactory.hxx"

namespace mfront {

  std::string GenericParallelMaterialPropertyInterface::getName() {
    return "generic-parallel";
  }

  GenericParallelMaterialPropertyInterface::
      GenericParallelMaterialPropertyInterface()
      : backend(std::make_unique<::mfront::generic_parallel::material_property::
                                     ParallelSTLBackend>()) {
  }  // end of GenericParallelMaterialPropertyInterface

  void GenericParallelMaterialPropertyInterface::setOptions(
      const DataMap& opts) {
    using ::mfront::generic_parallel::material_property::BackendFactory;
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
    types.output_status_type = "mfront_gmp_OutputStatus";
    return types;
  }  // end of getTypesDescription

  std::vector<std::string>
  GenericParallelMaterialPropertyInterface::getInterfaceHeaderFiles() const {
    return {"MFront/GenericMaterialProperty/Types.h",
            "MFront/GenericMaterialProperty/MaterialProperty.h"};
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
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(l.sources, this->getSourceFileName(name));
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(l.epts, name);
    d.headers.push_back(this->getHeaderFileName(name));
  }  // end of getLibraryDescription

  void GenericParallelMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto iccname = this->getInterfaceNameInCamelCase();
    const auto lib = iccname + getMaterialLawLibraryNameBase(mpd);
    this->getLibraryDescription(d, d.getLibrary(lib), mpd);
  }  // end of getTargetsDescription

  std::string GenericParallelMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto& material = mpd.material;
    const auto& className = mpd.className;
    if (material.empty()) {
      return className;
    }
    return material + "_" + className;
  }

  std::string GenericParallelMaterialPropertyInterface::getHeaderFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + '-' + i + '.' + this->backend->getHeaderFileExtension();
  }

  std::string GenericParallelMaterialPropertyInterface::getSourceFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + '-' + i + '.' + this->backend->getSourceFileExtension();
  }

  void GenericParallelMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of writeOutputFiles

  void GenericParallelMaterialPropertyInterface::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto types = this->getTypesDescription();
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
      os << "* \\date   " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      os << fd.description << '\n';
    }
    os << " */\n\n";

    os << "#ifndef " << makeUpperCase(name) << "_" << iucname << "_HH\n"
       << "#define " << makeUpperCase(name) << "_" << iucname << "_HH\n\n";
    writeExportDirectives(os);

    for (const auto& header : this->getInterfaceHeaderFiles()) {
      os << "#include \"" << header << "\"\n\n";
    }
    this->backend->writeSpecificIncludesInHeaderFile(os, *this, mpd);

    this->backend->writeCxxDeclarations(os, *this, mpd);

    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    this->backend->writeCDeclarations(os, *this, mpd);

    writeParametersSetterFunctionDeclaration(
        os, mpd,
        {.material_property_name = name,
         .floating_point_type = "double",
         .interface_namespace = this->getInterfaceName()});

    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n"
       << "#endif /* " << makeUpperCase(name) << "_" << iucname << "_HH */\n";
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
    const auto types = this->getTypesDescription();
    const auto iname = this->getInterfaceName();
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
      os << "* \\date   " << date << '\n';
    }
    os << " */\n\n"
       << "#include<algorithm>\n"
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
       << "#include<cmath>\n";
    os << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include\"TFEL/PhysicalConstants.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n"
       << "#include\"TFEL/Math/General/DerivativeType.hxx\"\n";
    if (useQuantities(mpd)) {
      os << "#include\"TFEL/Math/qt.hxx\"\n"
         << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    os << "#include\"" << name << "-" << iname << ".hxx\"\n\n";
    if (!includes.empty()) {
      os << includes << "\n\n";
    }
    this->backend->writeSpecificIncludesInSourceFile(os, *this, mpd);

    writeMaterialPropertyParametersHandler(os, mpd,
                                           {.material_property_name = name,
                                            .floating_point_type = "double",
                                            .interface_namespace = nname,
                                            .allow_copy_constructor = true});

    this->backend->writeGlobalFunctions(os, *this, mpd, fd);

    this->backend->writeCxxImplementations(os, *this, mpd, fd);

    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    writeFileDescriptionSymbols(os, name, fd);
    writeVariablesNamesSymbol(os, name, mpd);
    writeVariablesBoundsSymbols(os, name, mpd);
    writeValidatorSymbol(os, name, mpd);
    writeBuildIdentifierSymbol(os, name, mpd);
    writeEntryPointSymbol(os, name);
    writeTFELVersionSymbol(os, name);
    writeUnitSystemSymbol(os, name, mpd);
    writeInterfaceSymbol(os, name, this->getInterfaceNameInCamelCase());
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

    this->backend->writeCImplementations(os, *this, mpd, fd);

    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";

    this->backend->writeProxyObjects(os, *this, mpd);

    os.close();

    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "GenericParallelMaterialPropertyInterface::"
                        "writeSrcFile: end\n";
    }
  }  // end of GenericParallelMaterialPropertyInterface::writeSrcFile

  GenericParallelMaterialPropertyInterface::
      ~GenericParallelMaterialPropertyInterface() = default;

}  // end of namespace mfront
