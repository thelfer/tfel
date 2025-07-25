/*!
 * \file   mfm-test-generator/src/MFMTestGeneratorFileExport.cxx
 * \brief
 * \author Thomas Helfer
 * \date   01/03/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <fstream>

#ifdef MFM_TEST_GENERATOR_HAVE_MADNEX
#include "Madnex/Config.hxx"
#include "Madnex/File.hxx"
#endif /* MFM_TEST_GENERATOR_HAVE_MADNEX */

#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
#include "Madnex/MFMTestGeneratorTest.hxx"
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */

#include "TFEL/Raise.hxx"
#include "MFMTestGenerator/MFMTestGeneratorFileExport.hxx"

namespace mfmtg {

  static std::string getSourceFileContent(const std::string& f) {
    std::ifstream file(f);
    if (!file) {
      tfel::raise("mtest::getSourceFileContent: can't open file '" +
                  std::string{f} + "'");
    }
    std::ostringstream s;
    s << file.rdbuf();
    return s.str();
  }  // end of getSourceFileContent

  TestDescription::TestDescription() = default;
  TestDescription::TestDescription(TestDescription&&) = default;
  TestDescription::TestDescription(const TestDescription&) = default;
  TestDescription& TestDescription::operator=(TestDescription&&) = default;
  TestDescription& TestDescription::operator=(const TestDescription&) = default;
  TestDescription::~TestDescription() noexcept = default;

  void loadMFMTestGeneratorFileContent(TestDescription& d,
                                       const std::string& f) {
    d.content = getSourceFileContent(f);
  }  // end of loadMFMTestGeneratorFileContent

#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT

  static madnex::Group createGroups(madnex::Group& g,
                                    const std::vector<std::string>& names) {
    auto cg = g;
    for (const auto& n : names) {
      cg = madnex::createGroup(cg, n);
    }
    return cg;
  }  // end of names

  static void writeMadnexFile(const TestDescription& d, const std::string& f) {
    madnex::MFMTestGeneratorTest e;
    e.name = d.name;
    e.test = d.content;
    e.metadata.author = d.author;
    e.metadata.date = d.date;
    e.metadata.description = d.description;
    auto file = [&f] {
      std::ifstream infile(f);
      if (infile.good()) {
        return madnex::File(f, H5F_ACC_RDWR);
      }
      return madnex::File(f, H5F_ACC_TRUNC);
    }();
    auto g = [&file, &d] {
      const auto& material = d.material;
      const auto& bid = d.behaviour;
      tfel::raise_if(bid.empty(), "writeMadnexFile: empty behaviour");
      auto r = file.getRoot();
      if (!material.empty()) {
        return createGroups(r, {"MFront", material, "Behaviours", bid,
                                "MFMTestGeneratorTests"});
      }
      return createGroups(
          r, {"MFront", "Behaviours", bid, "MFMTestGeneratorTests"});
    }();
    madnex::write(g, e);
  }  // end of writeMadnexFile

#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */

  void write(const TestDescription& d, const std::string& f) {
    const auto ext = [&f]() -> std::string {
      const auto p = f.find(".");
      if (p != std::string::npos) {
        return std::string{f.substr(p + 1)};
      }
      return {};
    }();
#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
    if ((ext == "madnex") || (ext == "mdnx") || (ext == "edf")) {
      writeMadnexFile(d, f);
    } else {
      tfel::raise("write: unsupported file extension '" + ext + "'");
    }
#else  /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
    static_cast<void>(d);
    tfel::raise("write: unsupported file extension '" + ext + "'");
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
  }    // end of write

}  // namespace mfmtg
