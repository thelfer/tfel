/*!
 * \file  tests/Utilities/gentype3.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 janv. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#include "TFEL/Utilities/GenTypeBase.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

typedef tfel::meta::GenerateTypeList<int, std::string>::type HoldedTypes;

struct TFEL_VISIBILITY_EXPORT GenType
    : tfel::utilities::GenTypeBase<HoldedTypes> {
  using tfel::utilities::GenTypeBase<HoldedTypes>::operator=;

  std::string dumpToString() const;

  void dump(std::ostream&) const;

  void load(const std::string&, const std::string&);

 private:
  struct DumpToString;
  struct Dump;
  struct Load;

};  // end of struct TFEL_VISIBILITY_EXPORT GenType

struct GenType::DumpToString {
  using return_type = std::string;

  template <typename T>
  static return_type apply(const T& v) {
    using namespace std;
    ostringstream out;
    out << v;
    return out.str();
  }
};

struct GenType::Dump {
  using return_type = void;

  Dump(std::ostream& s) : stream(s) {}

  template <typename T>
  return_type operator()(const T& v) {
    using namespace std;
    this->stream << v;
  }

 private:
  Dump& operator=(const Dump&) = delete;
  Dump& operator=(Dump&&) = delete;

  std::ostream& stream;
};

struct GenType::Load {
  using return_type = void;

  Load(const std::string& s) : value(s) {}

  template <typename T>
  return_type operator()(T& v) {
    using namespace std;
    istringstream is(this->value);
    is >> v;
  }

 private:
  std::string value;
};

std::string GenType::dumpToString() const {
  using namespace tfel::utilities;
  return apply<DumpToString>(*this);
}

void GenType::dump(std::ostream& s) const {
  using namespace tfel::utilities;
  Dump d(s);
  return apply(d, *this);
}

void GenType::load(const std::string& t, const std::string& v) {
  if (t == "int") {
    this->set<int>(0);
  } else if (t == "std::string") {
    this->set<std::string>(std::string());
  } else {
    throw("load");
  }
  Load l(v);
  return tfel::utilities::apply(l, *this);
}

struct GenTypeTest3 final : public tfel::tests::TestCase {
  GenTypeTest3()
      : tfel::tests::TestCase("TFEL/Utilities", "GenTypeTest3") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    GenType g;
    g = 21;
    ostringstream os;
    TFEL_TESTS_ASSERT(g.is<int>());
    TFEL_TESTS_CHECK_EQUAL(g.get<int>(), 21);
    TFEL_TESTS_CHECK_EQUAL(g.dumpToString(), "21");
    g.dump(os);
    TFEL_TESTS_CHECK_EQUAL(os.str(), "21");

    os.str("");
    g = string("toto");
    TFEL_TESTS_ASSERT(g.is<string>());
    TFEL_TESTS_CHECK_EQUAL(g.get<string>(), "toto");
    TFEL_TESTS_CHECK_EQUAL(g.dumpToString(), "toto");
    g.dump(os);
    TFEL_TESTS_CHECK_EQUAL(os.str(), "toto");

    os.str("");
    g.load("int", "21");
    TFEL_TESTS_ASSERT(g.is<int>());
    TFEL_TESTS_CHECK_EQUAL(g.get<int>(), 21);
    TFEL_TESTS_CHECK_EQUAL(g.dumpToString(), "21");
    g.dump(os);
    TFEL_TESTS_CHECK_EQUAL(os.str(), "21");

    return this->result;
  }  // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(GenTypeTest3, "GenType");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("gentype3.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
