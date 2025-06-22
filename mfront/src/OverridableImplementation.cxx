/*!
 * \file   mfront/src/OverridableImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   01/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/File.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MaterialKnowledgeDescription.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/OverridableImplementation.hxx"

namespace mfront {

  static std::shared_ptr<const AbstractDSL> generateAbstractDSL(
      const std::string& f) {
    auto dsl = MFrontBase::getDSL(f);
    dsl->analyseFile(f, {}, {});
    dsl->endsInputFileProcessing();
    return dsl;
  }  // end of generateAbstractDSL

  void throwOverridenValueAlreadySet() {
    tfel::raise(
        "OverridableStringInformation::setOverridenValue: "
        "value already overriden");
  }  // end of throwOverridenValueAlreadySet

  OverridableImplementation::OverridableImplementation(const std::string& f)
      : dsl(generateAbstractDSL(f)), source(f) {
    const auto params = this->dsl->getOverridenParameters();
    this->parameters.insert(params.begin(), params.end());
  }  // end of OverridableImplementation

  const std::map<std::string, double>&
  OverridableImplementation::getOverridingParameters() const {
    return this->parameters;
  }  // end of getOverridingParameters

  void OverridableImplementation::overrideByAParameter(const std::string& n,
                                                       const double v) {
    const auto vn = dsl->getOverridableVariableNameByExternalName(n);
    this->parameters[vn] = v;
  }  // end of overrideByAParameter

  AbstractDSL::DSLTarget OverridableImplementation::getTargetType() const {
    return this->dsl->getTargetType();
  }  // end of getTargetType

  const FileDescription& OverridableImplementation::getSourceFileDescription()
      const {
    return this->dsl->getFileDescription();
  }  // end of getSourceFileDescription

  std::string OverridableImplementation::getSourceMaterialKnowledgeIdentifier()
      const {
    return this->dsl->getMaterialKnowledgeIdentifier();
  }  // end of getSourceMaterialKnowledgeIdentifier

  std::string OverridableImplementation::getSourceMaterialName() const {
    return this->dsl->getMaterialName();
  }  // end of getSourceMaterialName

  const std::string& OverridableImplementation::getSourceFilePath() const {
    return this->source;
  }  // end of getSourceFilePath

  const std::map<std::string, std::vector<std::string>, std::less<>>&
  OverridableImplementation::getExternalMFrontFiles() const {
    return this->dsl->getMaterialKnowledgeDescription()
        .getExternalMFrontFiles();
  }  // end of getExternalMFrontFiles

  std::string OverridableImplementation::getMaterialKnowledgeIdentifier()
      const {
    const auto& n =
        this->getOverridenValue<Tags::MATERIAL_KNOWLEDGE_IDENTIFIER>();
    return n.empty() ? this->getSourceMaterialKnowledgeIdentifier() : n;
  }

  std::string OverridableImplementation::getMaterial() const {
    const auto& m = this->getOverridenValue<Tags::MATERIAL_NAME>();
    return m.empty() ? this->getSourceMaterialName() : m;
  }

  OverridableImplementation::~OverridableImplementation() = default;

  static std::string getPathBaseName(const OverridableImplementation& i) {
    const auto mkt = [&i]() -> std::string {
      const auto t = i.getTargetType();
      if (t == AbstractDSL::MATERIALPROPERTYDSL) {
        return "MaterialProperties";
      } else if (t == AbstractDSL::BEHAVIOURDSL) {
        return "Behaviours";
      } else if (t != AbstractDSL::MODELDSL) {
        tfel::raise(
            "mfront::getPathBaseName: "
            "unsupported DSL target");
      }
      return "Models";
    }();
    const auto m = i.getMaterial();
    if (!m.empty()) {
      return "MFront/" + m + '/' + mkt;
    }
    return "MFront/" + mkt;
  }  // end of getPath

  static std::string getPath(const OverridableImplementation& i) {
    const auto b = getPathBaseName(i);
    const auto n = i.getMaterialKnowledgeIdentifier();
    return b + '/' + n;
  }  // end of getPath

#ifdef MFRONT_HAVE_MADNEX
  static std::string getSourceFileContent(const std::string& f) {
    if ((tfel::utilities::starts_with(f, "madnex:")) ||
        (tfel::utilities::starts_with(f, "mdnx:")) ||
        (tfel::utilities::starts_with(f, "edf:"))) {
      const auto path = decomposeImplementationPathInMadnexFile(f);
      const auto& material = std::get<2>(path);
      const auto& name = std::get<3>(path);
      const auto impl = madnex::getMFrontImplementation(
          std::get<0>(path), std::get<1>(path), material, name);
      return impl.source;
    }
    std::ifstream file(f);
    if (!file) {
      tfel::raise("mfront::getSourceFileContent: can't open file '" +
                  std::string{f} + "'");
    }
    std::ostringstream s;
    s << file.rdbuf();
    return s.str();
  }  // end of getSourceFileContent

  static std::vector<std::string> getPaths(const OverridableImplementation& i) {
    auto r = std::vector<std::string>{};
    auto append_if = [&r](const std::string& p) {
      if (std::find(r.begin(), r.end(), p) == r.end()) {
        r.push_back(p);
      }
    };  // end of append_if
    append_if(getPath(i));
    for (const auto& d : i.getExternalMFrontFiles()) {
      OverridableImplementation impl(d.first);
      for (const auto& p : getPaths(impl)) {
        append_if(p);
      }
    }
    return r;
  }  // end of getPaths

  static madnex::MFrontImplementation getMFrontImplementation(
      const OverridableImplementation& i) {
    using Tags = OverridableImplementation::Tags;
    auto copy_if = [](std::string& dest, const std::string& src1,
                      const std::string& src2) {
      dest = !src1.empty() ? src1 : src2;
    };  // end of copy_if
    const auto& fd = i.getSourceFileDescription();
    madnex::MFrontImplementation impl;
    copy_if(impl.name,
            i.getOverridenValue<Tags::MATERIAL_KNOWLEDGE_IDENTIFIER>(),
            i.getSourceMaterialKnowledgeIdentifier());
    if (impl.name.empty()) {
      tfel::raise(
          "mfront::getMFrontImplementation: "
          "no material knowledge identifier defined");
    }
    copy_if(impl.metadata.author, i.getOverridenValue<Tags::AUTHOR_NAME>(),
            fd.authorName);
    copy_if(impl.metadata.date, i.getOverridenValue<Tags::DATE>(), fd.date);
    copy_if(impl.metadata.description, i.getOverridenValue<Tags::DESCRIPTION>(),
            fd.description);
    impl.source = getSourceFileContent(i.getSourceFilePath());
    impl.parameters = i.getOverridingParameters();
    return impl;
  }

  static void writeMadnexFile(
      const OverridableImplementation& i,
      const std::string& f,
      const std::vector<std::string>& already_treated_implementations) {
    const auto impl = getMFrontImplementation(i);
    auto file = [&f] {
      std::ifstream infile(f);
      if (infile.good()) {
        return madnex::File(f, H5F_ACC_RDWR);
      }
      return madnex::File(f, H5F_ACC_TRUNC);
    }();
    auto r = file.getRoot();
    // check if paths already exists
    for (const auto& p : getPaths(i)) {
      if (madnex::exists(r, p)) {
        tfel::raise(
            "mfront::writeMadnexFile: "
            "path '" +
            p + "' already exists");
      }
    }
    //
    if (!madnex::exists(r, "tests")) {
      madnex::createGroup(r, "tests");
    }
    //
    const auto m = i.getMaterial();
    madnex::createGroup(r, "MFront");
    if (!m.empty()) {
      madnex::createGroup(r, "MFront/" + m);
    }
    auto g = madnex::createGroup(r, getPathBaseName(i));
    const auto gpath = getPath(i);
    // writing dependencies
    auto already_treated_implementations2 = already_treated_implementations;
    already_treated_implementations2.push_back(gpath);
    for (const auto& d : i.getExternalMFrontFiles()) {
      writeMadnexFile(OverridableImplementation(d.first), f,
                      already_treated_implementations2);
    }
    //
    madnex::write(g, impl);
  }    // end of writeMadnexFile
#endif /* MFRONT_HAVE_MADNEX */

  void write(const OverridableImplementation& i, const std::string& f) {
    const auto ext = [&f]() -> std::string {
      const auto p = f.find(".");
      if (p != std::string::npos) {
        return f.substr(p + 1);
      }
      return "";
    }();
#ifdef MFRONT_HAVE_MADNEX
    if ((ext == "madnex") || (ext == "mdnx") || (ext == "edf")) {
      writeMadnexFile(i, f, {});
    } else {
      tfel::raise("write: unsupported file extension '" + ext + "'");
    }
#else  /* MFRONT_HAVE_MADNEX */
    static_cast<void>(i);
    tfel::raise("write: unsupported file extension '" + ext + "'");
#endif /* MFRONT_HAVE_MADNEX */
  }    // end of write

  void write(const OverridableImplementation& i,
             const std::string& t,
             const std::string& f) {
    std::ifstream infile(f);
    if (!infile.good()) {
      // file doesn't exists, copying the template file
      tfel::system::systemCall::copy(t, f);
    }
    write(i, f);
  }  // end of write

  std::string getDestinationPathInMadnexFile(
      const OverridableImplementation& i) {
    return getPath(i);
  }  // end of getDestinationPathInMadnexFile

}  // end of namespace mfront
