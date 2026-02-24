/*!
 * \file   tests/System/binary_write.cxx
 * \brief
 * \author Thomas Helfer
 * \date   05/10/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cstdio>
#include <limits>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/BinaryWrite.hxx"
#include "TFEL/System/BinaryRead.hxx"
#include "TFEL/System/STLContainerBinaryWrite.hxx"
#include "TFEL/System/STLContainerBinaryRead.hxx"

struct BinaryReadWriteTest final : public tfel::tests::TestCase {
  BinaryReadWriteTest()
      : tfel::tests::TestCase("TFEL/System", "BinaryReadWriteTest") {
  }  // end of BinaryReadWriteTest
  tfel::tests::TestResult execute() override {
    enum Value { VALUE1, VALUE2 };

    int fd[2];
    if (::pipe(fd) != 0) {
      return false;
    }
    // integers
    this->test(fd, false);
    this->test(fd, true);
    this->test<unsigned char>(fd, 12);
    this->test<unsigned short>(fd, 12);
    this->test<unsigned int>(fd, 12);
    this->test<long unsigned int>(fd, 12);
    this->test<char>(fd, 12);
    this->test<short>(fd, 12);
    this->test<int>(fd, 12);
    this->test<long int>(fd, 12);
    this->test(fd, VALUE1);
    this->test(fd, VALUE2);
    // pointers
    double value = 13.;
    this->test<void*>(fd, nullptr);
    this->test<double*>(fd, &value);
    // floatting point numbers
    this->test2<float>(fd, 1.23f);
    this->test2<double>(fd, 1.23);
    this->test2<long double>(fd, 1.23L);
    // stl containers
    this->test(fd, std::string{"my string"});
    this->test3<std::vector<std::string>>(fd, {"Lena", "Bettie"});
    this->test3<std::list<std::string>>(fd, {"Lena", "Bettie"});
    this->test3<std::set<std::string>>(fd, {"Lena", "Bettie"});
    this->test4<std::map<std::string, int>>(fd,
                                            {{"Lena", 1972}, {"Bettie", 1955}});
    return this->result;
  }  // end of execute
 private:
  template <typename T>
  void test(const int* fd, const T& v) {
    using namespace tfel::system;
    binary_write(fd[1], v);
    TFEL_TESTS_ASSERT(binary_read<T>(fd[0]) == v);
  }
  template <typename T>
  void test2(const int* fd, const T& v) {
    using namespace tfel::system;
    binary_write(fd[1], v);
    TFEL_TESTS_ASSERT(std::abs(binary_read<T>(fd[0]) - v) <
                      10 * std::numeric_limits<T>::epsilon());
  }
  template <typename T>
  void test3(const int* fd, const T& v) {
    using namespace tfel::system;
    binary_write(fd[1], v);
    auto v2 = binary_read<T>(fd[0]);
    TFEL_TESTS_ASSERT(v.size() == v2.size());
    if (v.size() == v2.size()) {
      auto p1 = v.cbegin();
      auto p2 = v2.cbegin();
      for (; p1 != v.cend(); ++p1, ++p2) {
        TFEL_TESTS_ASSERT(*p1 == *p2);
      }
    }
  }
  template <typename T>
  void test4(const int* fd, const T& v) {
    using namespace tfel::system;
    binary_write(fd[1], v);
    auto v2 = binary_read<T>(fd[0]);
    TFEL_TESTS_ASSERT(v.size() == v2.size());
    if (v.size() == v2.size()) {
      auto p1 = v.cbegin();
      auto p2 = v2.cbegin();
      for (; p1 != v.cend(); ++p1, ++p2) {
        TFEL_TESTS_ASSERT(p1->first == p2->first);
        TFEL_TESTS_ASSERT(p1->second == p2->second);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(BinaryReadWriteTest, "BinaryReadWriteTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("binary_write.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
