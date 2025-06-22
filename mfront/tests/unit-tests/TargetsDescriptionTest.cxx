/*!
 * \file   TargetsDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <sstream>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/TargetsDescription.hxx"

struct TargetsDescriptionTest final : public tfel::tests::TestCase {
  TargetsDescriptionTest()
      : tfel::tests::TestCase("MFront", "TargetsDescriptionTest") {
  }  // end of TargetsDescriptionTest

  tfel::tests::TestResult execute() override {
    const char* const td =
        "{"
        " library : {"
        "  name   : \"libVanadiumAlloy\";"
        "  type   : SHARED_LIBRARY;"
        "  suffix : \"so\";"
        "  sources : {"
        "      \"VanadiumAlloy_PoissonRatio_SRMA.cxx\","
        "	\"VanadiumAlloy_YoungModulus_SRMA.cxx\""
        "	};"
        "  ldflags : {"
        "      \"-lm\""
        "	};"
        "  epts : {"
        "      \"VanadiumAlloy_PoissonRatio_SRMA\","
        "	\"VanadiumAlloy_PoissonRatio_SRMA_checkBounds\","
        "	\"VanadiumAlloy_YoungModulus_SRMA\","
        "	\"VanadiumAlloy_YoungModulus_SRMA_checkBounds\""
        "	};"
        "  };"
        " library : {"
        "  name   : \"libVanadiumAlloy-java\";"
        "  type   : SHARED_LIBRARY;"
        "  suffix : \"so\";"
        "  sources : {"
        "      \"VanadiumAlloy_PoissonRatio_SRMA-java.cxx\","
        "	\"VanadiumAlloy_YoungModulus_SRMA-java.cxx\""
        "	};"
        "  cppflags : {"
        "      \" -I/usr/lib/jvm/default-java/include "
        "-I/usr/lib/jvm/default-java/include "
        "-I/usr/lib/jvm/default-java/include\""
        "	};"
        "  ldflags : {"
        "      \"-lm\""
        "	};"
        "  epts : {"
        "      \"VanadiumAlloy.PoissonRatio_SRMA\","
        "	\"VanadiumAlloy.YoungModulus_SRMA\""
        "	};"
        "  };"
        " headers : {"
        "    \"VanadiumAlloy_PoissonRatio_SRMA.hxx\","
        "      \"VanadiumAlloy_YoungModulus_SRMA.hxx\""
        "      };"
        " target : {"
        "  name : \"../java/VanadiumAlloy.class\";"
        "  dependencies : {"
        "      \"../java/VanadiumAlloy.java\""
        "	};"
        "  commands : {"
        "      \"javac ../java/VanadiumAlloy.java\""
        "	};"
        "  };"
        " target : {"
        "  name : \"all\";"
        "  dependencies : {"
        "      \"../java/VanadiumAlloy.class\""
        "	};"
        "  };"
        "};";
    // reading a target description from a string
    auto t = extract(td);
    // testing the value read
    this->check(t);
    // saving the target to a stream and read it back
    std::ostringstream ss;
    ss << t;
    // and get it back !
    auto t2 = extract(ss.str());
    // testing the value read again
    this->check(t2);
    return this->result;
  }

 private:
  static mfront::TargetsDescription extract(const std::string& td) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(td);
    auto pb = t.begin();
    return mfront::read<mfront::TargetsDescription>(pb, t.end());
  }
  void check(const mfront::TargetsDescription& t) {
    using namespace mfront;
    // libraries
    TFEL_TESTS_ASSERT(describes(t, "libVanadiumAlloy"));
    TFEL_TESTS_ASSERT(describes(t, "libVanadiumAlloy-java"));
    TFEL_TESTS_ASSERT(!describes(t, "libVanadiumAlloy2"));
    TFEL_TESTS_CHECK_THROW(t.getLibrary("libVanadiumAlloy2"),
                           std::runtime_error);
    const auto& l1 = t.getLibrary("libVanadiumAlloy");
    TFEL_TESTS_ASSERT(l1.name == "libVanadiumAlloy");
    TFEL_TESTS_ASSERT(l1.suffix == "so");
    TFEL_TESTS_ASSERT(l1.type == LibraryDescription::SHARED_LIBRARY);
    TFEL_TESTS_ASSERT(l1.sources.size() == 2u);
    TFEL_TESTS_ASSERT(l1.sources.at(0) ==
                      "VanadiumAlloy_PoissonRatio_SRMA.cxx");
    TFEL_TESTS_ASSERT(l1.sources.at(1) ==
                      "VanadiumAlloy_YoungModulus_SRMA.cxx");
    TFEL_TESTS_ASSERT(l1.cppflags.empty());
    TFEL_TESTS_ASSERT(l1.ldflags.size() == 1u);
    TFEL_TESTS_ASSERT(l1.ldflags.at(0) == "-lm");
    TFEL_TESTS_ASSERT(l1.epts.size() == 4u);
    TFEL_TESTS_ASSERT(l1.epts.at(0) == "VanadiumAlloy_PoissonRatio_SRMA");
    TFEL_TESTS_ASSERT(l1.epts.at(1) ==
                      "VanadiumAlloy_PoissonRatio_SRMA_checkBounds");
    TFEL_TESTS_ASSERT(l1.epts.at(2) == "VanadiumAlloy_YoungModulus_SRMA");
    TFEL_TESTS_ASSERT(l1.epts.at(3) ==
                      "VanadiumAlloy_YoungModulus_SRMA_checkBounds");
    const auto& l2 = t.getLibrary("libVanadiumAlloy-java");
    TFEL_TESTS_ASSERT(l2.name == "libVanadiumAlloy-java");
    TFEL_TESTS_ASSERT(l2.suffix == "so");
    TFEL_TESTS_ASSERT(l2.type == LibraryDescription::SHARED_LIBRARY);
    TFEL_TESTS_ASSERT(l2.sources.size() == 2u);
    TFEL_TESTS_ASSERT(l2.sources.at(0) ==
                      "VanadiumAlloy_PoissonRatio_SRMA-java.cxx");
    TFEL_TESTS_ASSERT(l2.sources.at(1) ==
                      "VanadiumAlloy_YoungModulus_SRMA-java.cxx");
    TFEL_TESTS_ASSERT(l2.cppflags.size() == 1u);
    TFEL_TESTS_ASSERT(l2.cppflags.at(0) ==
                      " -I/usr/lib/jvm/default-java/include "
                      "-I/usr/lib/jvm/default-java/include "
                      "-I/usr/lib/jvm/default-java/include");
    TFEL_TESTS_ASSERT(l2.ldflags.size() == 1u);
    TFEL_TESTS_ASSERT(l2.ldflags.at(0) == "-lm");
    TFEL_TESTS_ASSERT(l2.epts.size() == 2u);
    TFEL_TESTS_ASSERT(l2.epts.at(0) == "VanadiumAlloy.PoissonRatio_SRMA");
    TFEL_TESTS_ASSERT(l2.epts.at(1) == "VanadiumAlloy.YoungModulus_SRMA");
    // headers
    TFEL_TESTS_ASSERT(t.headers.size() == 2u);
    TFEL_TESTS_ASSERT(t.headers.at(0) == "VanadiumAlloy_PoissonRatio_SRMA.hxx");
    TFEL_TESTS_ASSERT(t.headers.at(1) == "VanadiumAlloy_YoungModulus_SRMA.hxx");
    // specific targets
    const auto& st = t.specific_targets;
    TFEL_TESTS_ASSERT(st.find("all") != st.end());
    TFEL_TESTS_ASSERT(st.find("../java/VanadiumAlloy.class") != st.end());
    TFEL_TESTS_ASSERT(st.size() == 2u);
    const auto& all = st.at("all");
    TFEL_TESTS_ASSERT(all.deps.size() == 1u);
    TFEL_TESTS_ASSERT(all.deps.at(0) == "../java/VanadiumAlloy.class");
    TFEL_TESTS_ASSERT(all.cmds.empty());
    const auto& jclass = st.at("../java/VanadiumAlloy.class");
    TFEL_TESTS_ASSERT(jclass.deps.size() == 1u);
    TFEL_TESTS_ASSERT(jclass.deps.at(0) == "../java/VanadiumAlloy.java");
    TFEL_TESTS_ASSERT(jclass.cmds.size() == 1u);
    TFEL_TESTS_ASSERT(jclass.cmds.at(0) == "javac ../java/VanadiumAlloy.java");
  }
};

TFEL_TESTS_GENERATE_PROXY(TargetsDescriptionTest, "TargetsDescriptionTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("TargetsDescription.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
